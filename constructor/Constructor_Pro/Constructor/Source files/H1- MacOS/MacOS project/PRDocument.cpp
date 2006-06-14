// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PRDocument.cpp				© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/95
//	   $Date: 2006/04/12 08:48:30 $
//	$History: PRDocument.cpp $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:04
//	Updated in $/Constructor/Source files/H1- MacOS/MacOS project
//	Improved commenting.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:53
//	Updated in $/Constructor/Source files/H1- MacOS/MacOS project
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:50
//	Updated in $/Constructor/Source files/H1- MacOS/MacOS project
//	Fixed CR/LF problem
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:33
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Added support for CTYP editor.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/07/96   Time: 12:28
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Added support for bitmap editors.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/04/96   Time: 16:33
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Added support for SLEditor.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/13/96   Time: 16:57
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Added support for PPob editor.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:10
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Fixed a commenting error.
//	
//	*****************  Version 3  ***************** 
//	User: scouten      QDate: 11/05/96   Time: 15:09
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Resource editor shell overhaul.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 18:02
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Added support for Txtr editor.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:57
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#include "PRDocument.h"

	// Core : Headers
#include "Constructor.file.h"
#include "Constructor.menu.h"

	// MacOS : Editors : Bitmap
#include "PT_Resources.h"
#include "PTEditor.h"

	// MacOS : Editors : Custom types
#include "CTEditor.h"

	// MacOS : Editors : Menu editor
#include "MEEditor.h"

	// MacOS : Editors : String list
#include "SLEditor.h"

#include "TCLEditor.h"

	// MacOS : Editors : Text traits
#include "TTEditor.h"

	// MacOS : Editors : Views : PowerPlant
#include "VPEditor.h"

#include "UMemoryUtils.h"
#include "FileCopy.h"
#include "MoreFilesExtras.h"

#if PP_Target_Carbon
	#include "CFlatFileDesignator.h"
#else
	#include <UConditionalDialogs.h>
#endif


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	DSPC_PRContainer		= 15600;

const ResIDT	STR_FileTitles			= 15000;
const SInt16	str_Untitled			= 1;
const SInt16	str_UntitledNumbered	= 2;
const SInt16	str_TempSwapFile		= 3;


enum
{
	bigCopyBuffSize  = 0x00004000,	// from MoreFiles' FileCopy.c
	minCopyBuffSize  = 0x00000200
};


// ===========================================================================

#pragma mark *** PRDocument ***

// ---------------------------------------------------------------------------
//		* PRDocument
// ---------------------------------------------------------------------------
//	Constructor

PRDocument::PRDocument(
	LCommander*		inSuper)

: RSDocument(inSuper)

{
	mIsFlattened = false;
	mUnflattenedFile = nil;
}


// ---------------------------------------------------------------------------
//		* ~PRDocument
// ---------------------------------------------------------------------------
//	Destructor

PRDocument::~PRDocument()
{
	if (mUnflattenedFile)
	{
		OSErr	err;
		FSSpec	unflattenedSpec;
		
		mUnflattenedFile->GetSpecifier(unflattenedSpec);
		delete mUnflattenedFile;
		mUnflattenedFile = nil;
		
		// delete the unflattened file from disk (it's just a temporary file anyways)
		err = ::FSpDelete(&unflattenedSpec);
		if (err == fLckdErr)
		{
			// Unlock it and try to delete it again
			err = ::FSpRstFLock(&unflattenedSpec);
			if (err == noErr)
			{
				::FSpDelete(&unflattenedSpec);
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* FinishCreate
// ---------------------------------------------------------------------------

void
PRDocument::FinishCreate(
	const FSSpec*	inFileSpec)
{
	OSErr	err;
	
	err = ::IsFlattenedResourceFile(inFileSpec, &mIsFlattened);
	ThrowIfOSErr_(err);
	
	if (mIsFlattened)
	{
		CreateNewUnflattenedFile(inFileSpec);
	}
	
	// Finish opening the file.

	RSDocument::FinishCreate(inFileSpec);

	// Read custom display class descriptions from this file.
	
	if (mFile != nil) {
	
		RFMap* mapRF = (dynamic_cast<RFMap*>(mRMMap));
		ValidateObject_(mapRF);
	
		VETypeList* typeList = VETypeList::GetTypeList();
		if (typeList != nil) {
			ValidateObject_(typeList);
			typeList->ScanProjectFile(*inFileSpec, *mapRF);
		}
	}
}


// ---------------------------------------------------------------------------
//		* CreateNewUnflattenedFile								[protected]
// ---------------------------------------------------------------------------
//	Takes a flattened resource file (the resource map and information is
//	actually contained in the data fork) and creates a new unflattened
//	file, with all of the information copied over.
//	Sets mUnflattenedFile to the new file and returns a pointer to it.
 
LFile*
PRDocument::CreateNewUnflattenedFile(
	ConstFSSpecPtr	inFileSpec)
{
	// Validate pointers.
	
	ValidateThis_();
	
	OSErr	err;
	FSSpec	unflattenedSpec;
	SInt16	foundVRefNum;
	SInt32	foundDirID;
	SInt16	srcRefNum;
	SInt16	destRefNum;
	
	err = ::FindFolder(inFileSpec->vRefNum, kTemporaryFolderType, kCreateFolder,
				&foundVRefNum, &foundDirID);
	// JWW - The volume might not be what we asked, especially if it's on OS X...
	// ...and on OS X, it looks like FindFolder returns an error, so don't throw
	if ((err != noErr) || (foundVRefNum != inFileSpec->vRefNum))
	{
		// ...and in that case, just use the document folder instead why not?
		foundVRefNum = inFileSpec->vRefNum;
		foundDirID = inFileSpec->parID;
	}

	LStr255	tempName;
	UInt32	ticks = ::TickCount();

	do {
		tempName = (SInt32) ticks++;
		tempName += inFileSpec->name;
		if (tempName.Length() > 31)
		{
			tempName[0] = 31;
		}
		
		err = ::FSMakeFSSpec(foundVRefNum, foundDirID, tempName, &unflattenedSpec);
		if ((err != noErr) and (err != fnfErr))
			Throw_(err);
	} while (err != fnfErr);


	// Delete any existing unflattened file
	if (mUnflattenedFile)
	{
		FSSpec	oldUnflattenedSpec;
		
		mUnflattenedFile->GetSpecifier(oldUnflattenedSpec);
		delete mUnflattenedFile;
		mUnflattenedFile = nil;
		
		// delete the unflattened file from disk (it's just a temporary file anyways)
		err = ::FSpDelete(&oldUnflattenedSpec);
		if (err == fLckdErr)
		{
			// Unlock it and try to delete it again
			err = ::FSpRstFLock(&oldUnflattenedSpec);
			if (err == noErr)
			{
				::FSpDelete(&oldUnflattenedSpec);
			}
		}
	}

	// Create the new unflattened file
	mUnflattenedFile = new LFile(unflattenedSpec);

	// Create the new file and create the resource map
	mUnflattenedFile->CreateNewFile(Type_CreatorCode, Type_MacOSDocument, smSystemScript);
	destRefNum = mUnflattenedFile->OpenResourceFork(fsRdWrPerm);

	LFile	flatFile(*inFileSpec);
	srcRefNum = flatFile.OpenDataFork(fsRdPerm);

	StPointerBlock	buffer(bigCopyBuffSize, false, false);
	if (not buffer.IsValid())
	{
		StPointerBlock	smallBuffer(minCopyBuffSize, true, false);
		buffer.Adopt(smallBuffer.Release());
	}

	err = ::CopyFork(srcRefNum, destRefNum, buffer, ::GetPtrSize(buffer));
	
	SInt32	resForkEOF;
	
	ThrowIfOSErr_(::GetEOF(destRefNum, &resForkEOF));

	// Close the resource fork (we'll leave the flat file's data fork open)
	mUnflattenedFile->CloseResourceFork();

	// The CopyFork routine may have killed the resource map in the case
	// where the data fork was empty.  This will recreate an empty resource map.
	if (resForkEOF == 0)
	{
		::FSpDelete(&unflattenedSpec);
		mUnflattenedFile->CreateNewFile(Type_CreatorCode, Type_MacOSDocument, smSystemScript);
	}
	
	// Copy the File Manager attributes over to the new file
	err = ::FSpCopyFileMgrAttributes(inFileSpec, &unflattenedSpec, true);

	return mUnflattenedFile;
}


// ===========================================================================

#pragma mark -
#pragma mark ** file opening

// ---------------------------------------------------------------------------
//		* SetupMapAndContainer									[protected]
// ---------------------------------------------------------------------------
//	Configure resource map and data model container.

void
PRDocument::SetupMapAndContainer(
	RMMap*&		outRMMap,
	ResIDT&		outContainerDSPC)
{

	// Use standard MacOS resource map.

	RSDocument::SetupMapAndContainer(outRMMap, outContainerDSPC);
	
	// Use our container object.
	
	outContainerDSPC = DSPC_PRContainer;

}


// ---------------------------------------------------------------------------
//		* CommonOpenFile										[protected]
// ---------------------------------------------------------------------------
//	Called from constructor of base class to handle the common file-opening
//	behaviors. Should be called just after the container object and streaming
//	agents have been created.

void
PRDocument::CommonOpenFile(
	LFile*			inFile,
	PDContainer*	inRsrcContainer)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRMMap);
	ValidateObject_(inRsrcContainer);
	ValidateObject_(mSelection.GetObject());
	mRsrcContainer = inRsrcContainer;

	if (mIsFlattened)
	{
		// Configure resource container object to listen for RMMap's change messages.

		mRsrcContainer->SetRMMap(mRMMap);
		mRsrcContainer->AddListener(mSelection);
		mSelection->SetRootElement(mRsrcContainer);

		// If there is a file, open it and read its resources. Scanning the resource map
		// causes RMResource objects to get created and RMMap_ChangeMessage objects to
		// be sent. These change messages cause the container to create objects representing
		// the editable objects in the resource fork. This becomes the displayed
		// object hierarchy.
		
		mFile = inFile;
		if (mUnflattenedFile == nil)
		{
			FSSpec	flatSpec;
			inFile->GetSpecifier(flatSpec);
			mUnflattenedFile = CreateNewUnflattenedFile(&flatSpec);
		}

		if (mUnflattenedFile != nil) {

			ValidateObject_(mUnflattenedFile);
			OpenResourceFile(mUnflattenedFile);

			mRMMap->ScanNewMainFile(mUnflattenedFile);
			mIsSpecified = true;
			mRMMap->UpdateComplete();
			mRsrcContainer->ResetModified();
		}
		
		// Initialize the selection.

		mSelection->SetRootElement(mRsrcContainer);
	}
	else
	{
		RSDocument::CommonOpenFile(inFile, inRsrcContainer);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** file saving

// ---------------------------------------------------------------------------
//		* AskSaveAs												[protected]
// ---------------------------------------------------------------------------
//	Mostly borrowed from LDocument::AskSaveAs.
//	Overriden to send a message to listeners if the window title changes.

Boolean
PRDocument::AskSaveAs(
	FSSpec&		outFSSpec,
	Boolean		inRecordIt)
{

	// Validate pointers.

	ValidateThis_();
	
	// Ensure that memory allocations succeed.
	
	StCriticalSection crit;
	
	// Get "Save As" string.

	Str255 saveAsPrompt;
	Str255 defaultName;
	::GetIndString(saveAsPrompt, STRx_Standards, str_SaveAs);
	GetDescriptor(defaultName);
	
	// Show Standard File dialog. We use CustomPutFile because
	// we want to ensure that the dialog is properly placed on
	// multiple monitors. StandardPutFile doesn't seem to do this
	// properly.

	// JCD 980616 - Some talks with Eric Scouten lead me to believe
	// this is a problem only with certain OS versions..... plus that
	// the bug is with StandardPutFile. Therefore, I'm going to change
	// this to still use CustomPutFile, but use -1,-1 as the "where"
	// parameter to have it auto-center. We have updated the minimum
	// system version to Mac OS 7.5, so this might end up working out
	// (QA will test, and if this doiesn't change, it seems to work :)
	//
	// This fixed MW07459.
	
	bool isGood;
	bool isReplacing;
#if PP_Target_Carbon
	// icw - Use custom Navigation Services save dialog so we can ask whether
	// to save this as a flattened resource file
	CFlatFileDesignator	designator;

	designator.SetFileType(fileType_Default);
	isGood = designator.AskDesignateFlatFile(defaultName, mIsFlattened);
	if (isGood) {
		designator.GetFileSpec(outFSSpec);
		isReplacing = designator.IsReplacing();
		mIsFlattened = designator.IsFlatened();
	}
#else
	isGood = UConditionalDialogs::AskSaveFile(defaultName, fileType_Default, outFSSpec,
		isReplacing);
#endif
	
	if (isGood) {
		
		// User clicked the Save button. Send AE for recording.
		// (This is one of the few recordable parts of Constructor...)
	
		if (inRecordIt)
			SendAESaveAs(outFSSpec, fileType_Default, false);

		// Now save document.

		DoAESave(outFSSpec, fileType_Default);
	}

	// Return true if we saved the document.

	return isGood;

}


// ---------------------------------------------------------------------------
//		* DoAESave												[protected]
// ---------------------------------------------------------------------------
//	Respond to an AppleEvent to save the file.

void
PRDocument::DoAESave(
	FSSpec&		inFileSpec,
	OSType		/* inFileType */)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRFMap);
	ValidateObject_(mWindow);
	
	// Clear any pending actions.
	
	PostAction(nil);

	// Do we have a file already?
	
	OSErr	err;
	Boolean	oldFileIsFlat = false;

	if (mIsSpecified) {
	
		// Yes, cache its location.

		ValidateObject_(mFile);	

		FSSpec documentSpec;
		mFile->GetSpecifier(documentSpec);
				
		// Check if previous file is flat (mIsFlattened is for the new file already)
		err = ::IsFlattenedResourceFile(&documentSpec, &oldFileIsFlat);
		ThrowIfOSErr_(err);

		// First make sure we're not saving in place.
	
		if ((documentSpec.vRefNum == inFileSpec.vRefNum)
		  && (documentSpec.parID == inFileSpec.parID)
		  && (::EqualString(documentSpec.name, inFileSpec.name, false, true))) {

			// Filespecs are the same, just do a normal save.

		  	DoSave();
			return;
		}
		
		// We're saving to a different file, duplicate existing file to new filespec.

		if (mIsFlattened)
		{
			// The new file is a flattened resource file

			if (not mUnflattenedFile)
			{
				LFile	tempNewFile(inFileSpec);
				tempNewFile.CreateNewDataFile(Type_CreatorCode, Type_MacOSDocument, smSystemScript);
				mUnflattenedFile = CreateNewUnflattenedFile(&inFileSpec);
			}
			
			SaveCDEFs();							//* 2.1.2: BUG FIX #370
			
			FSSpec	unflattenedSpec;
			mUnflattenedFile->GetSpecifier(unflattenedSpec);
			
			mFile->CloseResourceFork();				// necessary for copying on some file servers
			mUnflattenedFile->CloseResourceFork();
			
			::FSpDelete(&inFileSpec);
			if (oldFileIsFlat)
			{
				// Moving data from data fork to data fork, so just use FileCopy()
				
				err = ::FileCopy(documentSpec.vRefNum, documentSpec.parID, documentSpec.name,
									inFileSpec.vRefNum, inFileSpec.parID, nil, inFileSpec.name,
									nil, 0, true);

				OpenResourceFile(mUnflattenedFile);	// reopen original file's resource fork
				mRFMap->SetMainFile(mUnflattenedFile);
				if (err)
					Throw_(err);
			}
			else
			{
				// Moving data from resource fork to data fork, so use CopyFork()
				
				LFile	tempNewFile(inFileSpec);
				tempNewFile.CreateNewDataFile(Type_CreatorCode, Type_MacOSDocument, smSystemScript);

				SInt16	srcRefNum;
				SInt16	destRefNum;

				StPointerBlock	buffer(bigCopyBuffSize, false, false);
				if (not buffer.IsValid())
				{
					StPointerBlock	smallBuffer(minCopyBuffSize, true, false);
					buffer.Adopt(smallBuffer.Release());
				}
				
				srcRefNum = mFile->OpenResourceFork(fsRdPerm);
				destRefNum = tempNewFile.OpenDataFork(fsRdWrPerm);
				
				ThrowIfOSErr_(::CopyFork(srcRefNum, destRefNum, buffer, ::GetPtrSize(buffer)));
				
				mUnflattenedFile = CreateNewUnflattenedFile(&inFileSpec);

				OpenResourceFile(mFile);				// reopen original file's resource fork
				mRFMap->SetMainFile(mFile);
			}
		}
		else
		{
			// The new file is a regular resource file

			SaveCDEFs();							//* 2.1.2: BUG FIX #370
			mFile->CloseResourceFork();				// necessary for copying on some file servers
			::FSpDelete(&inFileSpec);

			if (oldFileIsFlat)
			{
				// Copy mUnflattenedFile to new regular resource file
				FSSpec	unflattenedSpec;
				mUnflattenedFile->GetSpecifier(unflattenedSpec);
				mUnflattenedFile->CloseResourceFork();	// necessary for copying on some file servers
				err = ::FileCopy(unflattenedSpec.vRefNum, unflattenedSpec.parID, unflattenedSpec.name,
									inFileSpec.vRefNum, inFileSpec.parID, nil, inFileSpec.name,
									nil, 0, true);

				OpenResourceFile(mUnflattenedFile);		// reopen original file's resource fork
				mRFMap->SetMainFile(mUnflattenedFile);
				if (err)
					Throw_(err);
			}
			else
			{
				// Copy mFile to new regular resource file

				err = ::FileCopy(documentSpec.vRefNum, documentSpec.parID, documentSpec.name,
									inFileSpec.vRefNum, inFileSpec.parID, nil, inFileSpec.name,
									nil, 0, true);

				OpenResourceFile(mFile);				// reopen original file's resource fork
				mRFMap->SetMainFile(mFile);
				if (err)
					Throw_(err);
			}
		}
	}
	else
	{
		// No pre-existing file, make sure we start from an empty document.
	
		err = ::FSpCreate(&inFileSpec, GetCreatorCode(),
									  GetFileTypeCode(), smSystemScript);
		if (err == dupFNErr) {
		
			// File already exists, delete it and try again.
		
			ThrowIfOSErr_(::FSpDelete(&inFileSpec));
			ThrowIfOSErr_(::FSpCreate(&inFileSpec, GetCreatorCode(),
											GetFileTypeCode(), smSystemScript));
		}
		else
			ThrowIfOSErr_(err);
	
		// Add this new file to the recent items menu
		ProcessCommand(cmd_AddFileToRecentItems, &inFileSpec);
	}

	// Make sure there's a resource fork.
	
		// NOTE: This is safe. FSpCreateResFile is defined as a no-op in case the 
		// resource fork already exists.

	// We have already taken care of mUnflattenedFile having a resource fork
	if (not oldFileIsFlat and not mIsFlattened)
	{
		::FSpCreateResFile(&inFileSpec, GetCreatorCode(), GetFileTypeCode(), smSystemScript);
		err = ::ResError();
		if (err != dupFNErr)				// resource fork already exists
			ThrowIfOSErr_(err);
	}

	// Try saving to this file.
	
	LFile* newFile = nil;
	try {
	
		if (mIsFlattened)
		{
			// Create a new file & update resource context.
		
			newFile = new LFile(inFileSpec);
			ValidateObject_((LFile*) newFile);

			mUnflattenedFile = CreateNewUnflattenedFile(&inFileSpec);
			ValidateObject_(mUnflattenedFile);

			OpenResourceFile(mUnflattenedFile);
			mRFMap->SetMainFile(mUnflattenedFile);
		
			// Now save as though we already had the file open.
			
			RawSave(inFileSpec, inFileSpec);
			
			// Set file type/creator for this file.
			
			::FSpChangeCreatorType(&inFileSpec, GetCreatorCode(), GetFileTypeCode());
			
			// Copy the unflattened resource fork over to the flat data fork
			StPointerBlock	buffer(bigCopyBuffSize, false, false);
			if (not buffer.IsValid())
			{
				StPointerBlock	smallBuffer(minCopyBuffSize, true, false);
				buffer.Adopt(smallBuffer.Release());
			}

			SInt16	srcRefNum;
			SInt16	destRefNum;

			srcRefNum = mUnflattenedFile->OpenResourceFork(fsRdPerm);
			destRefNum = newFile->OpenDataFork(fsRdWrPerm);

			err = ::CopyFork(srcRefNum, destRefNum, buffer, ::GetPtrSize(buffer));

		}
		else
		{
			// Create a new file & update resource context.
		
			newFile = new LFile(inFileSpec);
			ValidateObject_((LFile*) newFile);

			OpenResourceFile((LFile*)newFile);
			mRFMap->SetMainFile((LFile*)newFile);
		
			// Now save as though we already had the file open.
			
			RawSave(inFileSpec, inFileSpec);
			
			// Set file type/creator for this file.
			
			::FSpChangeCreatorType(&inFileSpec, GetCreatorCode(), GetFileTypeCode());
		}
	}
	catch(...) {
	
		// Delete new file.
		
		if (newFile != nil) {
			ValidateObject_((LFile*) newFile);
			delete (LFile*)newFile;
		}
		::FSpDelete(&inFileSpec);
		
		// Revert to original resource context.
		
		mRFMap->SetMainFile(mFile);

		// Rethrow the exception.

		#if SKIPOMPARSE
			throw;
		#endif
			
	}

	// Now that we're done, get rid of old file reference.

	if (mFile != nil) {
		ValidateObject_(mFile);
		delete mFile;
	}
	mFile = (LFile*)newFile;
	
	// Update all window titles.
	
	mIsSpecified = true;
	mWindow->SetDescriptor(inFileSpec.name);
	
}


// ---------------------------------------------------------------------------
//		* DoSave												[protected]
// ---------------------------------------------------------------------------
//	Save the file on top of itself (i.e. save changes).

void
PRDocument::DoSave()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRFMap);
	ValidateObject_(mFile);

	// Ensure that memory allocations succeed.
	
	StCriticalSection crit;

	// Finalize any pending actions while resource file is still open.

	PostAction(nil);

 	// Make a name for temporary file using base string provided by caller and a time stamp.
	
	LStr255 tempFileName(STR_FileTitles, str_TempSwapFile);
	if (tempFileName.Length() > 20)
		tempFileName[(UInt8)0] = 20;
	
	UInt32 time;
	::GetDateTime(&time);
	LStr255 tempFileNumber((SInt32) time);
	tempFileName += tempFileNumber;

	// Find Temporary Items folder on source file's volume and create temp file there.

	FSSpec documentSpec;
	mFile->GetSpecifier(documentSpec);

	SInt16 theVRef;
	SInt32 theDirID;
	OSErr theErr = ::FindFolder(documentSpec.vRefNum, kTemporaryFolderType,
					kCreateFolder, &theVRef, &theDirID);
	
	// JWW - The volume might not be what we asked, especially if it's on OS X...
	// ...and on OS X, it looks like FindFolder returns an error, so don't throw
	if ((theErr != noErr) || (theVRef != documentSpec.vRefNum))
	{
		// ...and in that case, just use the document folder instead why not?
		theVRef = documentSpec.vRefNum;
		theDirID = documentSpec.parID;
	}

	FSSpec tempFileSpec;
	theErr = ::FSMakeFSSpec(theVRef, theDirID, tempFileName, &tempFileSpec);
	if ((theErr != noErr) && (theErr != fnfErr))
		ThrowOSErr_(theErr);

	try {


		if (mIsFlattened)
		{
			// Make a copy of the file.
		
			SaveCDEFs();							//* 2.1.2: BUG FIX #370

			FSSpec unflattenedSpec;
			mUnflattenedFile->GetSpecifier(unflattenedSpec);

			mUnflattenedFile->CloseResourceFork();	//ugh! required for saving on remote volumes
			::FSpDelete(&tempFileSpec);
			ThrowIfOSErr_(::FileCopy(unflattenedSpec.vRefNum, unflattenedSpec.parID,
								unflattenedSpec.name,
								tempFileSpec.vRefNum, tempFileSpec.parID, nil, tempFileName,
								nil, 0, true));

			// Open temp file and make changes.

			LFile tempFile(tempFileSpec);
			OpenResourceFile(&tempFile);
			mRFMap->SetMainFile(&tempFile);
			RawSave(tempFileSpec, documentSpec);
			
			// Save is done. Now swap files back.
			//
			// With flat files, we'll copy the temp resource fork into the
			// original file's flattened data fork.
			
			SInt16	srcRefNum;
			SInt16	destRefNum;

			StPointerBlock	buffer(bigCopyBuffSize, false, false);
			if (not buffer.IsValid())
			{
				StPointerBlock	smallBuffer(minCopyBuffSize, true, false);
				buffer.Adopt(smallBuffer.Release());
			}

			tempFile.CloseResourceFork();
			mFile->CloseDataFork();
			
			srcRefNum = tempFile.OpenResourceFork(fsRdPerm);
			destRefNum = mFile->OpenDataFork(fsRdWrPerm);

			ThrowIfOSErr_(::CopyFork(srcRefNum, destRefNum, buffer, ::GetPtrSize(buffer)));
			
			// The Save is done.  Now swap the temp and unflattened files.  This fixes
			// bug WB1-42415 where some changes to a flattened ppob file were overwritten
			// unless you closed and reopened the flat ppob after each save.
			tempFile.CloseResourceFork();
			ThrowIfOSErr_(::FSpExchangeFiles(&unflattenedSpec, &tempFileSpec));
			
			::FSpDelete(&tempFileSpec);		// deletes old document

		}
		else
		{
			// Make a copy of the file.
		
			SaveCDEFs();							//* 2.1.2: BUG FIX #370

			mFile->CloseResourceFork();				//ugh! required for saving on remote volumes
			::FSpDelete(&tempFileSpec);
			ThrowIfOSErr_(::FileCopy(documentSpec.vRefNum, documentSpec.parID, documentSpec.name,
								tempFileSpec.vRefNum, tempFileSpec.parID, nil, tempFileName,
								nil, 0, true));

			// Open temp file and make changes.

			LFile tempFile(tempFileSpec);
			OpenResourceFile(&tempFile);
			mRFMap->SetMainFile(&tempFile);
			RawSave(tempFileSpec, documentSpec);
			
			// Save is done. Now swap files back.

			tempFile.CloseResourceFork();
			ThrowIfOSErr_(::FSpExchangeFiles(&documentSpec, &tempFileSpec));
			::FSpDelete(&tempFileSpec);		// deletes old document
		}
	}
	catch (...) {
	
		// Get rid of temp file.

		::FSpDelete(&tempFileSpec);
		
		if (mIsFlattened)
		{
			// Reopen (untouched) main document.
			
			OpenResourceFile(mUnflattenedFile);
			mRFMap->SetMainFile(mUnflattenedFile);
		}
		else
		{
			// Reopen (untouched) main document.
			
			OpenResourceFile(mFile);
			mRFMap->SetMainFile(mFile);
		}

		// Rethrow exception so it gets reported.

		#if SKIPOMPARSE				// grrr-
			throw;
		#endif
	}

	if (mIsFlattened)
	{
		// Reopen (untouched) main document.
		
		OpenResourceFile(mUnflattenedFile);
		mRFMap->SetMainFile(mUnflattenedFile);
	}
	else
	{
		// Reopen (untouched) main document.
		
		OpenResourceFile(mFile);
		mRFMap->SetMainFile(mFile);
	}

}


// ---------------------------------------------------------------------------
//		* RawSave												[protected]
// ---------------------------------------------------------------------------
//	Overridden to change the file type from Constructor 1.0
//	to Constructor 2.x.

void
PRDocument::RawSave(
	const FSSpec&	inTempFileSpec,
	const FSSpec&	inCurrentFileSpec)
{

	// Validate pointers.

	ValidateThis_();
	
	// Save the file.
	
	RSDocument::RawSave(inTempFileSpec, inCurrentFileSpec);

	// Check to see if the file is a Constructor 1.0 document.
	// If so, change its type to Constructor 2.x.

	if (mFile != nil) {
	
		ValidateObject_(mFile);
	
		FSSpec fileSpec;
		FInfo fileInfo;
		mFile->GetSpecifier(fileSpec);
		ThrowIfOSErr_(::FSpGetFInfo(&fileSpec, &fileInfo));

		if ((fileInfo.fdCreator == 'cnst') && (fileInfo.fdType == 'docu')) {	// tsk, tskÉ Nick, shouldnÕt you have used upper case?
			fileInfo.fdCreator = Type_CreatorCode;
			fileInfo.fdType = Type_MacOSDocument;
			ThrowIfOSErr_(::FSpSetFInfo(&fileSpec, &fileInfo));
		}
	}
}


// ---------------------------------------------------------------------------
//		* GetCreatorCode										[protected]
// ---------------------------------------------------------------------------
//	Return the creator code for this document type.

OSType
PRDocument::GetCreatorCode()
{
	return Type_CreatorCode;
}



// ---------------------------------------------------------------------------
//		* GetFileTypeCode										[protected]
// ---------------------------------------------------------------------------
//	Return the file type code for this document type.

OSType
PRDocument::GetFileTypeCode()
{
	return Type_MacOSDocument;
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource editors

// ---------------------------------------------------------------------------
//		* CreateEditorForResource								[protected]
// ---------------------------------------------------------------------------
//	Create the resource editor for a given resource type.

REEditor*
PRDocument::CreateEditorForResource(
	ResType		inResType,
	ResIDT		/* inResID */)
{

	// Validate pointers.

	ValidateThis_();

	// What editor we create depends on the resource type.

	REEditor* theEditor = nil;

	switch (inResType) {

		case 'PPob':
			theEditor = new VPEditor(this, *mSession);
			break;

		case 'MBAR':
		case 'MENU':
			theEditor = new MEEditor(this, *mSession);
			break;
		
		case 'Txtr':
			theEditor = new TTEditor(this, *mSession);
			break;
		
		case 'CTYP':
			theEditor = new CTEditor(this, *mSession);
			break;

		case ImageType_LargeIcon:
		case ImageType_LargeIconWithMask:
		case ImageType_Large4BitIcon:
		case ImageType_Large8BitIcon:
		case ImageType_SmallIconWithMask:
		case ImageType_Small4BitIcon:
		case ImageType_Small8BitIcon:
		case ImageType_MiniIconWithMask:
		case ImageType_Mini4BitIcon:
		case ImageType_Mini8BitIcon:
		case ImageType_ColorIcon:
		case ImageType_Picture:
		case ImageType_Pattern:
		case ImageType_PixPat:
		case ImageType_Cursor:
		case ImageType_ColorCursor:
			theEditor = new PTEditor(this, *mSession);
			break;
		
		case 'STR#':
			theEditor = new SLEditor(this, *mSession);
			break;

		case 'tab#':
			theEditor = new TCLEditor(this, *mSession);
			break;

		default:
			SignalCStr_("Attempt to edit unknown resource type");
	}

	return theEditor;
}




// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		* IsFlattenedResourceFile
// ---------------------------------------------------------------------------

OSErr
IsFlattenedResourceFile(
	ConstFSSpecPtr	inFile,
	Boolean*		outIsFlat)
{
	OSErr		err;
	CInfoPBRec	pb;
	
	if (not inFile)
	{
		// This can occur when we create a new project document (Cmd-N)
		*outIsFlat = false;
		return noErr;
	}

	pb.hFileInfo.ioNamePtr = (StringPtr)inFile->name;
	pb.hFileInfo.ioVRefNum = inFile->vRefNum;
	pb.hFileInfo.ioDirID = inFile->parID;
	pb.hFileInfo.ioFDirIndex = 0;

	err = PBGetCatInfoSync(&pb);
	if (err == noErr)
	{
		if (pb.hFileInfo.ioFlAttrib & kioFlAttribDirMask)
		{
			// This is a directory
			*outIsFlat = false;
			return paramErr;
		}
		else
		{
			UInt32	dfSize;
			UInt32	rfSize;
			SInt16	dfRefNum;
			SInt32	filePos;
			
			dfSize = pb.hFileInfo.ioFlLgLen;
			rfSize = pb.hFileInfo.ioFlRLgLen;
			
			if (rfSize > 0)
			{
				*outIsFlat = false;
			}
			else if (dfSize == 0)
			{
				// This file has no data or resource fork.
				*outIsFlat = false;
			}
			else
			{
				// Only the data fork is non-empty.
				// Now we need to determine if it contains resources or not.
				UInt32	firstFourWords[4];
				SInt32	byteCount;
				
				err = FSpOpenDF(inFile, fsRdPerm, &dfRefNum);
				if (err)	return err;
				
				err = GetFPos(dfRefNum, &filePos);
				
				byteCount = sizeof(firstFourWords);

				err = FSRead(dfRefNum, &byteCount, &firstFourWords);
				if (err == noErr)
				{
					// Test is based on resource file format as described in IM: More Mac Toolbox
					// <http://developer.apple.com/techpubs/mac/MoreToolbox/MoreToolbox-99.html#HEADING99-0>
					//
					// First four words of the file represent the resource header
					// Word1:  Offset from beginning of resource fork to resource data
					// Word2:  Offset from beginning of resource fork to resource map
					// Word3:  Length of resource data
					// Word4:  Length of resource map
					//
					// So...
					// (Word1 + Word3 + Word4) == (Word2 + Word4) == size of resource fork

					if ((byteCount == sizeof(firstFourWords)) and
						(EndianU32_BtoN(firstFourWords[0]) + EndianU32_BtoN(firstFourWords[2]) +
						 EndianU32_BtoN(firstFourWords[3]) == dfSize) and
						(EndianU32_BtoN(firstFourWords[1]) + EndianU32_BtoN(firstFourWords[3]) == dfSize))
					{
						*outIsFlat = true;
					}
				}
				
				err = SetFPos(dfRefNum, fsFromStart, filePos);

				err = FSClose(dfRefNum);
			}
		}
	}
	
	return err;
}

