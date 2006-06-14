// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDDocument.cpp				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/02/97
//     $Date: 2006/01/18 01:33:04 $
//  $History: PDDocument.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:40
//	Updated in $/Constructor/Source files/CO- Core/Documents/Project document
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/05/97    Time: 2:38p
//	Updated in $/ConstructorWin/Sources
//	Initial edit for compiling under MSVC (prefix file; /p; dynamic const
//	casts)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#if PP_Target_Carbon
	#include <UNavServicesDialogs.h>
#else
	#include <UConditionalDialogs.h>
#endif

// ===========================================================================

#include "PDDocument.h"

	// Core : Application
#include "CAPreferencesFile.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"
#include "DMSelection.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Display classes (Eric) : Dialog handlers
#include "StKeyDialogHandler.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Documents : Project document
#include "PDType.h"

	// Core : Editors : Generic editor
#include "RESession.h"
#include "RETypeEntry.h"
#include "RETypeList.h"

	// Core : Editors : Views : Data model
#include "VETypeList.h"

	// Core : Resource manager
#include "RMIdentifier.h"
#include "RMMap.h"
//#include "StResourceContext.h"

	// Core : Utility classes
#include "UMemoryUtils.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>
#include <LStaticText.h>

// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_SaveChanges		= 15050;
const PaneIDT	Pane_MessageCaption		= 'MSG ';
const PaneIDT	Pane_DontSave			= 'DONT';
const PaneIDT	Pane_Cancel				= 'CNCL';
const PaneIDT	Pane_Save				= 'SAVE';

const ResIDT	STR_FileTitles			= 15000;
const SInt16	str_Untitled			= 1;
const SInt16	str_UntitledNumbered	= 2;
const SInt16	str_TempSwapFile		= 3;

const ResIDT	STR_SaveChanges			= 15050;
const SInt16	str_ClosingPrefix		= 1;
const SInt16	str_ClosingSuffix		= 2;
const SInt16	str_QuittingPrefix		= 3;
const SInt16	str_QuittingSuffix		= 4;


// ===========================================================================

#pragma mark *** PDDocument ***

// ---------------------------------------------------------------------------
//		* PDDocument(...)
// ---------------------------------------------------------------------------
//	Constructor

PDDocument::PDDocument(
	LCommander*		inSuper)

: LSingleDoc(inSuper)

{

	// Clear pointers.

	mRMMap = nil;
	mSession = nil;

	// Place this in the AEOM hierarchy as a "document."
	
	SetModelKind(cDocument);
	
}


// ---------------------------------------------------------------------------
//		* ~PDDocument
// ---------------------------------------------------------------------------
//	Destructor

PDDocument::~PDDocument()
{
	
	// Cancel any transactions that may be lingering about.

	PostAction(nil);
	
	// Record which types were collapsed in main window.

	RecordCollapsedTypes();
	
	// Remove windows associated with this document.
	
	if (mRsrcContainer != nil) {
		ValidateObject_(mRsrcContainer.GetObject());
		mRsrcContainer->GoingAway();
	}
	else
		SignalCStr_("No resource container.");
	
	// If there is a project window open, close it now.
	
	if (mWindow != nil) {
		ValidateObject_(mWindow);
		mWindow->SetSuperModel(this);
	}
	
	// Remove editor session object.
	
	ValidateObject_(mSession);
	delete mSession;
	
	// Delete resource map.
	
	if (mRMMap != nil) {
		ValidateObject_(mRMMap);
		delete mRMMap;
	}
	else
		SignalCStr_("No resource map. Hmmm.");

}


// ===========================================================================

#pragma mark -
#pragma mark ** file initialization

// ---------------------------------------------------------------------------
//		* FinishCreate
// ---------------------------------------------------------------------------
//	Should be called after a document object is created to complete the
//	process of opening the file and setting up its data model.

void
PDDocument::FinishCreate(
	const FSSpec*	inFileSpec)
{

	// Validate pointers.

	ValidateThis_();

	// Set up undo/redo for the document.
	
	AddAttachment(new LUndoer);
	
	// Create resource map.

	ResIDT containerDSPC;
	
	SetupMapAndContainer(mRMMap, containerDSPC);	
	ValidateObject_(mRMMap);
	
	// Create a selection for resource items.
	
	mSelection = new DMSelection;
	ValidateObject_(mSelection.GetObject());

	try {

		// Set up resource container.
	
		DMReanimator* reanimator = DMReanimator::GetReanimator();
		ValidateObject_(reanimator);
	
		DMElementPtr tempContainer;
		reanimator->ElementsFromResource('DSPC', containerDSPC, tempContainer);
		ValidateObject_(tempContainer.GetObject());
		
		mRsrcContainer = (dynamic_cast<PDContainer*>(tempContainer.GetObject()));
		ValidateObject_(mRsrcContainer.GetObject());
		
		// Set up editor session.
		
		mSession = new RESession(this, dynamic_cast<PDContainer*>(tempContainer.GetObject()), mRMMap);
		ValidateObject_(mSession);
		
		mRsrcContainer->SetSession(mSession);
	
		// Read data from file (if applicable).
	
		if (inFileSpec != nil) {
			mFile = new LFile(*inFileSpec);
			ValidateObject_(mFile);
		}
		CommonOpenFile(mFile, mRsrcContainer);
	
		// Check settings record for collapsed resource types.
		
		if (mFile != nil)
			CheckCollapsedTypes();
		
		// Make sure the basic resource types have folders, even if they are empty.
		
		OpenDefaultFolders();
	
		// Build & display window for editing.
	
		BuildEditWindow();
	
	}
	catch(...) {
		mRsrcContainer = nil;
		mSelection = nil;

		if (mSession != nil)
			delete mSession;
		if (mRMMap != nil)
			delete mRMMap;

		#if SKIPOMPARSE
			throw;
		#endif
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** file closing confirmation

// ---------------------------------------------------------------------------
//		* Close
// ---------------------------------------------------------------------------
//	Close the document and any associated windows..

void
PDDocument::Close()
{

	// Validate pointers.

	ValidateThis_();

	// Make sure we can do this first.

	if ((mSuperCommander == nil) || mSuperCommander->AllowSubRemoval(this)) {
	
		// Okay, close all associated windows.

		ValidateObject_(mRsrcContainer.GetObject());	
		mRsrcContainer->GoingAway();
		
		// Now delete self.
		
		delete this;

	}
}


// ---------------------------------------------------------------------------
//		* AttemptClose
// ---------------------------------------------------------------------------
//	Overriden to improve the dialog handling.

void
PDDocument::AttemptClose(
	Boolean		/* inRecordIt */)
{

	// Validate pointers.

	ValidateThis_();
	
	// Ensure that memory allocations succeed.
	
	StCriticalSection crit;
	
	// Decide whether to close and/or save the file.
	// Default is to close file.
	
	Boolean closeIt = true;
	FSSpec fileSpec;
	fileSpec.parID = 1;
	fileSpec.vRefNum = 1;
	fileSpec.name[0] = 0;
	
	// If document is modified, make sure we confirm the change.
	
	if (IsModified()) {
	
		// Ask user what to do.
	
		StApplicationContext appContext;
		MessageT response = DoSaveChangesDialog(PPob_SaveChanges, str_ClosingPrefix);
		
		// Got answer.
		
		switch (response) {
		
			// Okay button hit: Just do a normal save.
		
			case msg_OK:

				if (mIsSpecified) {
					DoSave();
					SendAEClose(kAEYes, fileSpec, false);
				}
				else {
					closeIt = AskSaveAs(fileSpec, false);
					if (closeIt)
						SendAEClose(kAEYes, fileSpec, false);
				}
				break;
				
			// Cancel button hit: Abort close.
			
			case msg_Cancel:
				closeIt = false;
				break;
			
			// Don't save button hit: Close file anyway.
			
			default:
				SendAEClose(kAENo, fileSpec, false);
				
		}
	}
	else {
	
		// Document is unmodified: close w/o saving.
		
		SendAEClose(kAENo, fileSpec, false);

	}

	// Unless otherwise instructed, we close the file now.

	if (closeIt)
		Close();

}


// ---------------------------------------------------------------------------
//		* AttemptQuitSelf
// ---------------------------------------------------------------------------
//	Overriden to improve dialog handling.

Boolean
PDDocument::AttemptQuitSelf(
	SInt32	inSaveOption)
{

	// Validate pointers.

	ValidateThis_();
	Boolean allowQuit = true;

	// See if file needs to be saved.
	
	if (IsModified()) {
	
		// It does, do it automatically if AppleEvent instructed us to do so.
	
		if (inSaveOption == kAEYes) {
		
			// Interaction with user not required. Just save it.
		
			DoSave();
			
		}
		else if (inSaveOption == kAEAsk) {
		
			// Interaction with user is required.
		
			StApplicationContext appContext;
			MessageT response = DoSaveChangesDialog(PPob_SaveChanges, str_QuittingPrefix);
			
			// We have our answer.
			
			switch (response) {
			
				// OK button hit: Save file.
			
				case msg_OK:
					if (mIsSpecified)
						DoSave();
					else {
						FSSpec fileSpec;
						fileSpec.vRefNum = 1;
						fileSpec.parID = 1;
						fileSpec.name[0] = 0;
						allowQuit = AskSaveAs(fileSpec, false);
					}
					break;
				
				// Cancel button hit: Don't save, don't quit.
				
				case msg_Cancel:
					allowQuit = false;
					break;
			}
		}
	}
	
	// If quitting, close this file.
	
	if (allowQuit) {
		ValidateObject_(mRsrcContainer.GetObject());
		mRsrcContainer->GoingAway();
		delete this;
	}

	return allowQuit;
}


// ===========================================================================

#pragma mark -
#pragma mark ** duplicate-file testing

// ---------------------------------------------------------------------------
//		* OpenIfMatch
// ---------------------------------------------------------------------------
//	User has double-clicked on the file that was named. If this document
//	already has that file opened, just bring the main window to front.
//	Return true if the file matches, false if not.

Boolean
PDDocument::OpenIfMatch(
	FSSpec*		inMacFSSpec)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mWindow);

	// If don't have a file, it certainly isn't the file we're being
	//	asked about. :-)

	if (mFile == nil)
		return false;

	ValidateObject_(mFile);
	
	// Ask the file for its file spec.

	FSSpec docFileSpec;
	mFile->GetSpecifier(docFileSpec);

	// See if it matches.

	if (!::EqualString(docFileSpec.name, inMacFSSpec->name, false, true))
		return false;				// filenames don't match

	if (docFileSpec.vRefNum != inMacFSSpec->vRefNum)
		return false;				// volume IDs don't match

	if (docFileSpec.parID != inMacFSSpec->parID)
		return false;				// parent directories don't match
	
	// It's a match. Bring our window to front and tell the app not to reopen this file.
	
	mWindow->Select();
	return true;

}


// ===========================================================================

#pragma mark -
#pragma mark ** file opening

// ---------------------------------------------------------------------------
//		* CommonOpenFile										[protected]
// ---------------------------------------------------------------------------
//	Called from constructor of base class to handle the common file-opening
//	behaviors. Should be called just after the container object and streaming
//	agents have been created.

void
PDDocument::CommonOpenFile(
	LFile*			inFile,
	PDContainer*	inRsrcContainer)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRMMap);
	ValidateObject_(inRsrcContainer);
	ValidateObject_(mSelection.GetObject());
	mRsrcContainer = inRsrcContainer;

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
	if (mFile != nil) {

		ValidateObject_(mFile);
		OpenResourceFile(mFile);

		mRMMap->ScanNewMainFile(mFile);
		mIsSpecified = true;
		mRMMap->UpdateComplete();
		mRsrcContainer->ResetModified();
	}
	
	// Initialize the selection.

	mSelection->SetRootElement(mRsrcContainer);

}


// ---------------------------------------------------------------------------
//		* OpenRF												[protected]
// ---------------------------------------------------------------------------
//	Attempt to open the file for read/write access. If that fails, try for
//	read-only access and show a dialog explaining why we can't change
//	the file.

void
PDDocument::OpenResourceFile(
	LFile*		/* inFile */)
{
	// ??? can't share code
}


// ---------------------------------------------------------------------------
//		* NameNewDoc											[protected]
// ---------------------------------------------------------------------------
//	Give the document a unique identifier. Used only for new
//	untitled documents.

void
PDDocument::NameNewDoc(
	ResIDT	inFileTitles)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mWindow);

	// Make sure we have a valid file titles string.
	
	if (inFileTitles == 0)
		inFileTitles = STR_FileTitles;

	// Start with the default name ("untitled").

	StApplicationContext appContext;
	LStr255 name(inFileTitles, str_Untitled);
	SInt32 num = 0;

	while (UWindows::FindNamedWindow(name) != nil) {
			
		// An existing window has the current name; increment counter and try again.

		::GetIndString(name, inFileTitles, str_UntitledNumbered);
		num++;
		LStr255 numStr((SInt32) num);
		name += numStr;
	}		

	// Now that we've found a unique name, set window title.
	
	mWindow->SetDescriptor(name);

}


// ---------------------------------------------------------------------------
//		* OpenDefaultFolders									[protected]
// ---------------------------------------------------------------------------
//	Create "folders" for the default resource types, as specified by the
//	RSCP resources.

void
PDDocument::OpenDefaultFolders()
{

	// Validate pointers.

	ValidateThis_();

	// Get global resource type list. It will contain instructions
	// on which types should be opened.

	RETypeList* typeList = RETypeList::GetTypeList();
	ValidateObject_(typeList);
	
	LFastArrayIterator typeIter(typeList->GetResTypeEntries());
	RETypeEntry* typeEntry;
	
	while (typeIter.Next(&typeEntry)) {
		
		ValidateObject_(typeEntry);

		// Okay, we have a resource type entry. If it asks to have
		// a default folder created, do it now.
		
		if (typeEntry->CreateEmptyFolder())
			OpenFolderForType(typeEntry->GetPrimaryResType());
		
	}
}


// ---------------------------------------------------------------------------
//		* CheckCollapsedTypes									[protected]
// ---------------------------------------------------------------------------
//	Collapse all resource types which weren't expanded last time we
//	were displaying a file.


void
PDDocument::CheckCollapsedTypes()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRsrcContainer.GetObject());

	// Build list of known types. We start by assuming that all types
	// will be expanded, unless we find the type in the list of
	// collapsed types.
	
	LArray expandedList(sizeof (ResType));
	DMFastIterator iter(mRsrcContainer->GetResourceTypeList()->GetSubElements());

	PDType* typeObj;
	ResType theType;

	while (iter.NextElement()) {
		typeObj = (dynamic_cast<PDType*>(iter.CurrentElement()));
		ValidateObject_(typeObj);
		
		theType = typeObj->GetPrimaryResType();
		expandedList.InsertItemsAt(1, LArray::index_Last, &theType);
	}

	// Now look in prefs file and see which types should be collapsed.

	StPreferencesContext prefs;
	if (prefs.IsValid()) {					//* 2.4a2: BUG FIX #1072: added if statement

		StResource collPrefRsrc('Proj', 128, false, true);
	
		// Collapse all types which were not expanded when the file was last saved.
	
		if (collPrefRsrc != nil) {
			LHandleStream typeStream(collPrefRsrc);
			try {
				while (typeStream.GetMarker() < typeStream.GetLength()) {
					typeStream >> theType;
					expandedList.Remove(&theType);
				}
			}
			catch(...) {
			}
			typeStream.DetachDataHandle();
		}
	}
	
	// Now expand the remaining types.
	
	LArrayIterator typeIter(expandedList);
	while (typeIter.Next(&theType)) {
	
		typeObj = mRsrcContainer->FindResType(RMIdentifier(theType), false);
		if (typeObj != nil) {
			ValidateObject_(typeObj);
			typeObj->ExpandResourceList();
		}
	}
}


// ---------------------------------------------------------------------------
//		* RecordCollapsedTypes									[protected]
// ---------------------------------------------------------------------------
//	Remember which resource types are collapsed so we don't open them
//	next time a file is opened.

void
PDDocument::RecordCollapsedTypes()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRMMap);
	ValidateObject_(mRsrcContainer.GetObject());

	// Record all types which are not expanded.

	LHandleStream typeStream;
	DMFastIterator iter(mRsrcContainer->GetResourceTypeList()->GetSubElements());

	while (iter.NextElement()) {
		PDType* type = (dynamic_cast<PDType*>(iter.CurrentElement()));
		ValidateObject_(type);
		if (!type->IsExpanded()) {
			ResType theType = type->GetPrimaryResType();
			typeStream << theType;
		}
	}

	//* 2.3a3: BUG FIX #431: put this in preferences file now
	
	StPreferencesContext prefs;
	if (prefs.IsValid()) {					//* 2.4a2: BUG FIX #1072: added if statement
		StNewResource collPrefRsrc('Proj', 128, typeStream.GetLength(), true);
	
		::BlockMoveData(*(typeStream.GetDataHandle()), *((Handle) collPrefRsrc), typeStream.GetLength());
		collPrefRsrc.Write();
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
PDDocument::AskSaveAs(
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
	isGood = UNavServicesDialogs::AskSaveFile(defaultName, fileType_Default, outFSSpec,
		isReplacing);
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
//		* RawSave												[protected]
// ---------------------------------------------------------------------------
//	Save our editable data to an existing file.

void
PDDocument::RawSave(
	const FSSpec&	/* inTempFileSpec */,
	const FSSpec&	/* inCurrentFileSpec */)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRsrcContainer.GetObject());
	ValidateObject_(mRMMap);
	
	// Make sure all pending changes are recorded.

	mRsrcContainer->CollectChanges();

	// Write data to the resource fork.

	mRMMap->UpdateRF();

	// Clear modified flags.

	mRMMap->UpdateComplete();
	mRsrcContainer->ResetModified();

}


// ---------------------------------------------------------------------------
//		* IsModified											[protected]
// ---------------------------------------------------------------------------
//	Return true if there are any changes to the file that can be saved.

Boolean
PDDocument::IsModified()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRMMap);
	ValidateObject_(mRsrcContainer.GetObject());

	Boolean isModified = false;
	
	// Ask resource map, data model, then document object if they're modified.

	if (mRMMap->IsModified())
		isModified = true;

	if (mRsrcContainer->IsSubModified(true))
		isModified = true;
	
	if (mRsrcContainer->IsModified())
		isModified = true;

	if (LSingleDoc::IsModified())
		isModified = true;

	// icw -- Tell the Window Manager our modification status so it can update
	// the proxy icon.
	if (GetWindow() != NULL)
		::SetWindowModified(GetWindow()->GetMacWindow(), isModified);

	return isModified;

}


// ---------------------------------------------------------------------------
//		* DoSaveChangesDialog									[protected]
// ---------------------------------------------------------------------------
//	General solution for save changes before quit/close/revert dialogs.

MessageT
PDDocument::DoSaveChangesDialog(
	ResIDT		inDialogPPob,
	SInt32		inStringIndex)
{

	// Validate pointers.

	ValidateThis_();

	// Create dialog.

	StApplicationContext appContext;
	StKeyDialogHandler dialog(inDialogPPob, this);
	ValidateObject_(dialog.GetDialog());
	
	// Set message text as appropriate.
	
	LStr255 messageStr(STR_SaveChanges, inStringIndex);

	Str255 docName;
	GetDescriptor(docName);
	messageStr += docName;
	
	LStr255 messageStr2(STR_SaveChanges, inStringIndex+1);
	messageStr += messageStr2;
	
	LStaticText* messageCaption = dynamic_cast<LStaticText*>(dialog.GetDialog()->FindPaneByID(Pane_MessageCaption));
	ValidateObject_(messageCaption);
	messageCaption->SetDescriptor(messageStr);
	
	// Run the dialog.
	
	dialog.GetDialog()->ProcessCommand(cmd_PositionWindow);
	dialog.GetDialog()->Show();
	MessageT theMessage = msg_Nothing;
	do {
		theMessage = dialog.DoDialog();
	} while (theMessage == msg_Nothing);
	dialog.GetDialog()->ProcessCommand(cmd_RecordPosition);

	// Make sure menus get updated.

	LCommander::SetUpdateCommandStatus(true);

	// Return result of dialog.

	return theMessage;

}


// ===========================================================================

#pragma mark -
#pragma mark ** file closing

// ---------------------------------------------------------------------------
//		* RemoveSubCommander									[protected]
// ---------------------------------------------------------------------------
//	If the window is going away, remove any dangling pointers to it.

void
PDDocument::RemoveSubCommander(
	LCommander*		inSub)
{

	// Validate input parameters.

	ValidateThis_();

	// If our window is going away, remove its pointer.
	
	if (inSub == mWindow)
		mWindow = nil;

	// Let commander cancel its pointers.

	LCommander::RemoveSubCommander(inSub);

}