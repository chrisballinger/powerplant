// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSDocument.cpp				© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 02/01/96
//	   $Date: 2006/01/18 01:32:59 $
//	$History: RSDocument.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/06/97    Time: 11:39a
//	Updated in $/ConstructorWin/Sources
//	Added prefix file for MSVC; cast away volatile; ...
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:44
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:27
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/16/96   Time: 13:30
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Added code to read custom type descriptions from user files. (Bug fix
//	#1098.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/12/96   Time: 14:58
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Fixed stray preferences resource. (Bug fix #1072.) Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/07/96   Time: 14:21
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Fixed a memory leak that could occur if an exception was thrown while
//	the RSDocument constructor was running.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:30
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Added code to ensure that Save and Close commands work when memory
//	reserves are low.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:52
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:55
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSDocument.h"

	// Core : Application
#include "CAPreferencesFile.h"
#include "Constructor.file.h"

	// Core : Data model : Core classes
#include "DMReanimator.h"
#include "DMSelection.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Documents : Generic resource document
#include "RSOutlineTable.h"
#include "RSResource.h"
#include "RSSourceControl.h"
#include "RSSourceStatusButton.h"
#include "RSType.h"

	// Core : Editors : Generic
#include "RESession.h"
#include "RETypeEntry.h"
#include "RETypeList.h"

	// Core : Display classes (Eric) : Dialog handlers
#include "StKeyDialogHandler.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Editors : Views : Data model
#include "VETypeList.h"

	// Core : Resource manager
#include "RMIdentifier.h"
//#include "StResourceContext.h"

	// Core : Utility classes
#include "UMemoryUtils.h"
#include "UApplicationFile.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>

	// MoreFiles
#include "FileCopy.h"
//#include "FSpCompat.h"
#include "MoreFiles.h"
#include "MoreFilesExtras.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_ProjectWindow		= 15000;
const PaneIDT	Pane_RSOutlineTable		= 'RSot';
const PaneIDT	Pane_SourceStatus		= 'PROJ';

const ResIDT	STR_FileTitles			= 15000;
const SInt16	str_Untitled			= 1;
const SInt16	str_UntitledNumbered	= 2;
const SInt16	str_TempSwapFile		= 3;

const ResIDT	STR_MRODialogStrings	= 15001;
const SInt16	str_MROError			= 1;
const SInt16	str_MROExplaination		= 2;
const SInt16	str_MRODefaultText		= 3;
const SInt16	str_MROCancelText		= 4;

const ResIDT	STR_SaveChanges			= 15050;
const SInt16	str_ClosingPrefix		= 1;
const SInt16	str_ClosingSuffix		= 2;
const SInt16	str_QuittingPrefix		= 3;
const SInt16	str_QuittingSuffix		= 4;


// ---------------------------------------------------------------------------
//		* MyMROStandardAlertFilter
// ---------------------------------------------------------------------------
//	A special filter for our StandardAlert call which asks if we want to make
//	a file modify read-only.  The filter allows the user to press Cmd-M and
//	Cmd-O (just because it's normally used as a shortcut for OK) to simulate
//	the MRO button and it maps Escape, Cmd-Period, and Cmd-L to the Cancel
//	button.

pascal Boolean MyMROStandardAlertFilter(
	DialogRef inDialog,
	EventRecord* inEvent,
	DialogItemIndex* outItemHit);

pascal Boolean MyMROStandardAlertFilter(
	DialogRef inDialog,
	EventRecord* inEvent,
	DialogItemIndex* outItemHit)
{
	Boolean handled = false;
	char charCode;
	
	handled = StdFilterProc(inDialog, inEvent, outItemHit);
	if (not handled)
	{
		if ((inEvent->what == keyDown) or (inEvent->what == autoKey))
		{
			if (inEvent->modifiers == cmdKey)
			{
				charCode = (inEvent->message & charCodeMask);
				if ((charCode == 'm') or (charCode == 'o'))
				{
					*outItemHit = kAlertStdAlertOKButton;
					handled = true;
				}
			}
		}
	}
	if (not handled)
	{
		// Check for cancel (Escape or Cmd-Period)
		if (UKeyFilters::IsEscapeKey(*inEvent) or UKeyFilters::IsCmdPeriod(*inEvent))
		{
			*outItemHit = kAlertStdAlertCancelButton;
			handled = true;
		}
		// Check for Cmd-L
		else if ((inEvent->what == keyDown) or (inEvent->what == autoKey))
		{
			if (inEvent->modifiers == cmdKey)
			{
				charCode = (inEvent->message & charCodeMask);
				if (charCode == 'l')
				{
					*outItemHit = kAlertStdAlertCancelButton;
					handled = true;
				}
			}
		}

	}

	return handled;
}

// ===========================================================================

#pragma mark *** RSDocument ***

// ---------------------------------------------------------------------------
//		* RSDocument(LCommander*)
// ---------------------------------------------------------------------------
//	Constructor, specifying super commander


RSDocument::RSDocument(
	LCommander*		inSuper)

: PDDocument(inSuper)

{
	mRFMap = nil;
	RegisterRSClasses();
}


// ---------------------------------------------------------------------------
//		* ~RSDocument
// ---------------------------------------------------------------------------
//	Destructor

RSDocument::~RSDocument()
{

	// Detach any CDEF resources associated with this file.
	
	SaveCDEFs();									//* 2.1.2: BUG FIX #413
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreate
// ---------------------------------------------------------------------------
//	Read CTYPs from file.

void
RSDocument::FinishCreate(
	const FSSpec*	inFileSpec)
{

	// Finish opening the file.

	PDDocument::FinishCreate(inFileSpec);

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



// ===========================================================================

#pragma mark -
#pragma mark ** file opening

// ---------------------------------------------------------------------------
//		* SetupMapAndContainer									[protected]
// ---------------------------------------------------------------------------
//	Configure resource map and data model container.

void
RSDocument::SetupMapAndContainer(
	RMMap*&		outRMMap,
	ResIDT&		outContainerDSPC)
{
	
	// Create a new resource map.
	
	mRFMap = new RFMap;
	ValidateObject_(mRFMap);
	
	// Return object and container;

	outRMMap = mRFMap;
	outContainerDSPC = 0;

}


// ---------------------------------------------------------------------------
//		* OpenResourceFile										[protected]
// ---------------------------------------------------------------------------
//	Attempt to open the file for read/write access. If that fails, try for
//	read-only access and show a dialog explaining why we can't change
//	the file.

void
RSDocument::OpenResourceFile(
	LFile*	inFile)
{

	// Validate pointers.

	ValidateThis_();
	ThrowIfNil_(inFile);			// can't validate since it may be stack-based object
	ValidateObject_(mRsrcContainer.GetObject());
	ValidateObject_(mRsrcContainer->GetSourceControl());
	
	// Save the current resource context.

	StResourceContext saveContext;
	saveContext.Save();
	volatile SInt16 fileRefNum;

	{
		
		// Don't load resources from this file.
	
		StResLoad load(false);
		
		// First try opening file with read/write access.
		
		try {
			fileRefNum = inFile->OpenResourceFork(fsRdWrPerm);
		}
		catch (const LException& inErr) {
			
			// Couldn't open document for write access. Try for read access.
			
			fileRefNum = inFile->OpenResourceFork(fsRdPerm);
			mRsrcContainer->GetSourceControl()->SetFileIsLocked(true);

		}
	}
	
	// Restore resource context.
	
	saveContext.Exit();

}


// ---------------------------------------------------------------------------
//		* OpenFolderForType										[protected]
// ---------------------------------------------------------------------------
//	Create a "folder" for the named resource type.

void
RSDocument::OpenFolderForType(
	ResType		inResType)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRsrcContainer.GetObject());

	// Ask the resource map to create a type. Even with no
	// resources, this will trigger the data model to create
	// an RSType object.
	
	RMMap* map = mRsrcContainer->GetRMMap();
	ValidateObject_(map);
	
	map->FindResType(RMIdentifier(inResType), true);

}


// ---------------------------------------------------------------------------
//		* BuildEditWindow										[protected]
// ---------------------------------------------------------------------------
//	Create the window which will display this project file.

void
RSDocument::BuildEditWindow()
{

	// Validate pointers.

	ValidateThis_();

	// Create the window.

	{
		StApplicationContext appContext;
		mWindow = LWindow::CreateWindow(PPob_ProjectWindow, this);
		ValidateObject_(mWindow);
	}

	// Give it the same title as our document or choose an "untitled-n" name.
	
	if (mIsSpecified) {
		ValidateObject_(mFile);
		FSSpec outFileSpec;
		mFile->GetSpecifier(outFileSpec);
		mWindow->SetDescriptor(outFileSpec.name);
		// icw -- Set the window proxy to this file.
		::SetWindowProxyFSSpec(mWindow->GetMacWindow(), &outFileSpec);
	}
	else
	{
		NameNewDoc();
		FSSpec appFileSpec;
		UApplicationFile::GetApplicationFile(appFileSpec);
		// icw -- Set the window proxy using our creator and type since the
		// window is not represented by a file on disk.
		::SetWindowProxyCreatorAndType(mWindow->GetMacWindow(), Type_CreatorCode,
			Type_MacOSDocument, appFileSpec.vRefNum);
	}
	// icw -- Clear the new window modification status.
	::SetWindowModified(mWindow->GetMacWindow(), false);

	// Attach resource list hierarchy view.

	RSOutlineTable* outlineTable =
			(dynamic_cast<RSOutlineTable*>(mWindow->FindPaneByID(Pane_RSOutlineTable)));
	ValidateObject_(outlineTable);

	outlineTable->SetSelection(mSelection);
	outlineTable->SetElement(mRsrcContainer);
	outlineTable->SetSession(mSession);

	// Set up source code control display.

	RSSourceStatusButton* ssButton =
			(dynamic_cast<RSSourceStatusButton*>(mWindow->FindPaneByID(Pane_SourceStatus)));
	ValidateObject_(ssButton);
	ssButton->AttachSourceControl(mRsrcContainer->GetSourceControl());

	// Position & show the window.

	mWindow->DoSetZoom(true);
	mWindow->ProcessCommand(cmd_PositionWindow);
	mWindow->Show();

	// If the file is read only and not locked, display a dialog to the user
	// asking if they would like to MRO the file.  Some users are unaware of
	// the status button in the lower left corner of the window.
	PDSourceControl* sourceControl = mRsrcContainer->GetSourceControl();
	if (sourceControl != NULL)
	{
		if (sourceControl->HasSCCInfo() and !sourceControl->IsLocked()
			and sourceControl->IsReadOnly())
		{
			AlertStdAlertParamRec params;
			LStr255 error(STR_MRODialogStrings, str_MROError);
			LStr255 explaination(STR_MRODialogStrings, str_MROExplaination);
			LStr255 defaultButtonText(STR_MRODialogStrings, str_MRODefaultText);
			LStr255 cancelButtonText(STR_MRODialogStrings, str_MROCancelText);
			SInt16 itemHit;
			OSStatus status;
			
			params.movable = true;
			params.helpButton = false;
			params.filterProc = NewModalFilterUPP(&MyMROStandardAlertFilter);
			params.defaultText = defaultButtonText;
			params.cancelText = cancelButtonText;
			params.otherText = NULL;
			params.defaultButton = kAlertStdAlertCancelButton;
			params.cancelButton = 0;
			params.position = kWindowAlertPositionParentWindow;

			status = StandardAlert(kAlertNoteAlert, error, explaination, &params, &itemHit);
			if (status == noErr and itemHit == kAlertStdAlertOKButton)
			{
				sourceControl->ModifyReadOnly();
				ssButton->Refresh();
			}
			if (params.filterProc != NULL)
				DisposeModalFilterUPP(params.filterProc);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** file saving

// ---------------------------------------------------------------------------
//		* DoSave												[protected]
// ---------------------------------------------------------------------------
//	Save the file on top of itself (i.e. save changes).

void
RSDocument::DoSave()
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
	catch (...) {
	
		// Get rid of temp file.

		::FSpDelete(&tempFileSpec);
		
		// Reopen (untouched) main document.
		
		OpenResourceFile(mFile);
		mRFMap->SetMainFile(mFile);

		// Rethrow exception so it gets reported.

		#if SKIPOMPARSE				// grrr-
			throw;
		#endif
	}

	// Reopen (now updated) main document.

	OpenResourceFile(mFile);
	mRFMap->SetMainFile(mFile);

	// icw -- Update the window proxy and modification status.
	::SetWindowProxyFSSpec(mWindow->GetMacWindow(), &documentSpec);
	::SetWindowModified(mWindow->GetMacWindow(), false);
}


// ---------------------------------------------------------------------------
//		* DoAESave												[protected]
// ---------------------------------------------------------------------------
//	Respond to an AppleEvent to save the file.

void
RSDocument::DoAESave(
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
	
	if (mIsSpecified) {
	
		// Yes, cache its location.

		ValidateObject_(mFile);	

		FSSpec documentSpec;
		mFile->GetSpecifier(documentSpec);
		
		// First make sure we're not saving in place.
	
		if ((documentSpec.vRefNum == inFileSpec.vRefNum)
		  && (documentSpec.parID == inFileSpec.parID)
		  && (::EqualString(documentSpec.name, inFileSpec.name, false, true))) {

			// Filespecs are the same, just do a normal save.

		  	DoSave();
			return;
		}
		
		// We're saving to a different file, duplicate existing file to new filespec.

		SaveCDEFs();							//* 2.1.2: BUG FIX #370
		mFile->CloseResourceFork();				// necessary for copying on some file servers
		::FSpDelete(&inFileSpec);
		OSErr copyErr = ::FileCopy(documentSpec.vRefNum, documentSpec.parID, documentSpec.name,
							inFileSpec.vRefNum, inFileSpec.parID, nil, inFileSpec.name,
							nil, 0, true);

		OpenResourceFile(mFile);				// reopen original file's resource fork
		mRFMap->SetMainFile(mFile);
		if (copyErr)
			Throw_(copyErr);
		
	}
	else {
	
		// No pre-existing file, make sure we start from an empty document.
	
		OSErr err = ::FSpCreate(&inFileSpec, GetCreatorCode(),
									  GetFileTypeCode(), smSystemScript);
		if (err == dupFNErr) {
		
			// File already exists, delete it and try again.
		
			ThrowIfOSErr_(::FSpDelete(&inFileSpec));
			ThrowIfOSErr_(::FSpCreate(&inFileSpec, GetCreatorCode(),
											GetFileTypeCode(), smSystemScript));
		}
		else
			ThrowIfOSErr_(err);
	
	}

	// Make sure there's a resource fork.
	
		// NOTE: This is safe. FSpCreateResFile is defined as a no-op in case the 
		// resource fork already exists.

	::FSpCreateResFile(&inFileSpec, GetCreatorCode(), GetFileTypeCode(), smSystemScript);
	OSErr theErr = ::ResError();
	if (theErr != dupFNErr)				// resource fork already exists
		ThrowIfOSErr_(theErr);

	// Try saving to this file.
	
	volatile LFile* newFile = nil;
	try {
	
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
//		* SaveCDEFs												[protected]
// ---------------------------------------------------------------------------
//	Walk the resource map, looking for open CDEFs. The file is about to get
//	closed, which would result in the CDEFs getting disposed. If a window is
//	still open, this will cause it to crash.
//
//* 2.1.2: Added entire method for BUG FIX #370.

void
RSDocument::SaveCDEFs()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRFMap);
	
	// Walk through the document's resource context.
	// Prevent resource loading.

	StResLoad dontLoad(false);
	StResourceContext docContext(mRFMap->GetResourceContext());
	
	if (docContext.IsValid()) {

		// Identify all of the CDEF resources. Detach them
		// from the MacOS resource map.
	
		SInt32 resCount = ::Count1Resources('CDEF');
		while (resCount--) {
			Handle theCDEF = ::Get1IndResource('CDEF', resCount+1);
			if (*theCDEF == nil)
				::ReleaseResource(theCDEF);
			else {
				ValidateHandle_(theCDEF);
				::DetachResource(theCDEF);
			}
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource editors

// ---------------------------------------------------------------------------
//		* CreateEditorForResource								[protected]
// ---------------------------------------------------------------------------
//	Open a resource editor for the specified resource.

REEditor*
RSDocument::CreateEditorForResource(
	const RMIdentifier&	inResType,
	const RMIdentifier&	inResID)
{

	// If type or ID aren't numeric, we won't create an editor.

	if (!inResType.IsNumericID() || !inResID.IsNumericID())
		return nil;
	
	// Both are kosher, now create the editor.

	return CreateEditorForResource(inResType.GetNumericID(), inResID.GetNumericID());

}


// ---------------------------------------------------------------------------
//		* RecordCollapsedTypes									[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that the 'µMWC' 32000 resource is removed. It used
//	to be used to record collapsed types on a per-file basis.

void
RSDocument::RecordCollapsedTypes()
{

	// Do the standard recording.

	PDDocument::RecordCollapsedTypes();

	// If 'µMWC' resource exists, delete it.
	
	ValidateObject_(mRFMap);
	RFResource* settings = mRFMap->FindResource('µMWC', 32000, false);
	if (settings != nil) {
		ValidateObject_(settings);
		settings->DeleteResource();
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* RegisterRSClasses								[static, protected]
// ---------------------------------------------------------------------------
//	Register display classes and data model classes used by project files.

void
RSDocument::RegisterRSClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register project view classes.
	
	URegistrar::RegisterClass(RSOutlineTable::class_ID, (ClassCreatorFunc) RSOutlineTable::CreateFromStream);
	URegistrar::RegisterClass(RSSourceStatusButton::class_ID, (ClassCreatorFunc) RSSourceStatusButton::CreateFromStream);

	// Register project data model classes.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->RegisterClass(RSContainer::class_ID, RSContainer::CreateFromStream);
	reanimator->RegisterClass(RSType::class_ID, RSType::CreateFromStream);
	reanimator->RegisterClass(RSResource::class_ID, RSResource::CreateFromStream);

}
