// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAGenericTarget.cpp			© 1994-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/05/94
//     $Date: 2006/04/12 08:47:44 $
//  $History: CAGenericTarget.cpp $
//	
//	*****************  Version 28  *****************
//	User: scouten      QDate: 03/18/97   Time: 15:28
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Changed default debug action to low-level debugger. Seems to work
//	better with latest MW debugger.
//	
//	*****************  Version 27  *****************
//	User: scouten      QDate: 03/17/97   Time: 14:32
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Command-Shift-A now shows & hides alignment palette. (Bug fix #1077.)
//	
//	*****************  Version 26  *****************
//	User: scouten      QDate: 03/17/97   Time: 14:20
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Added launch-time profiling code.
//	
//	*****************  Version 25  *****************
//	User: scouten      QDate: 03/06/97   Time: 11:42
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Finished conversion to PDDocument class.
//	
//	*****************  Version 24  *****************
//	User: scouten      QDate: 03/06/97   Time: 11:38
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Changed return type of MakeEmptyProject and MakeProjectFromFile from
//	RSDocument* to PDDocument*.
//	
//	*****************  Version 23  *****************
//	User: Andrew       Date: 3/05/97    Time: 3:15p
//	Updated in $/ConstructorWin/Core/Source
//	Unscoped HLock since Altura #defines it away.
//	
//	*****************  Version 22  *****************
//	User: scouten      QDate: 02/20/97   Time: 13:19
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Improved commenting.
//	
//	*****************  Version 21  *****************
//	User: Andrew       Date: 2/17/97    Time: 3:53p
//	Updated in $/ConstructorWin/Sources
//	hack; debug settings are ignored in windows version.
//	
//	*****************  Version 20  *****************
//	User: Andrew       Date: 2/06/97    Time: 2:24p
//	Updated in $/ConstructorWin/Sources
//	Conditionally include MercutioMBAR header
//	
//	*****************  Version 19  *****************
//	User: Andrew       Date: 2/04/97    Time: 11:31a
//	Updated in $/ConstructorWin/Sources
//	Integrated changes made for Windows-hosted version
//	
//	*****************  Version 18  *****************
//	User: scouten      QDate: 02/01/97   Time: 14:08
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Added Windows application object.
//	
//	*****************  Version 17  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:18
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Fixed CR/LF problem
//	
//	*****************  Version 16  *****************
//	User: scouten      QDate: 01/21/97   Time: 14:59
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Rolled in Clint's fixes for MSL.
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 01/07/97   Time: 18:44
//	Updated in $/Constructor/Source files/Application
//	Updated to Clint's drop 01/06/97.
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 12/16/96   Time: 11:34
//	Updated in $/Constructor/Source files/Application
//	Changed RememberOpenWindows so that it records window position for
//	catalog and alignment palette. (Bug fix #1057.)
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 12/12/96   Time: 14:50
//	Updated in $/Constructor/Source files/Application
//	Fixed stray preferences resource. (Bug fix #1072.)
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 12/05/96   Time: 19:19
//	Updated in $/Constructor/Source files/Application
//	Removed Display Classes command.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:24
//	Updated in $/Constructor/Source files/Application
//	Added catalog to RememberOpenWindows. Increased size of grow zone from
//	20K to 32K.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 11/27/96   Time: 09:16
//	Updated in $/Constructor/Source files/Application
//	Removed catalog document behaviors. Added support for new catalog
//	window.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/24/96   Time: 23:28
//	Updated in $/Constructor/Source files/Application
//	Finished support for alignment palette.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/14/96   Time: 14:52
//	Updated in $/Constructor/Source files/Application
//	Disabled cmd_NewCatalog cases in FindCommandStatus and ObeyCommand.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/13/96   Time: 10:35
//	Updated in $/Constructor/Source files/Application
//	Moved OpenDisplayClassWindow from target-specific application objects
//	to CAGenericTarget.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:31
//	Updated in $/Constructor/Source files/Application
//	Fixed another bug with the DebugNewValidateAllBlocks call.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:27
//	Updated in $/Constructor/Source files/Application
//	Wrapped DebugNewValidateAllBlocks in #if DEBUG_NEW macros.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:41
//	Updated in $/Constructor/Source files/Application
//	Added Save All and Close All commands.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/02/96   Time: 15:02
//	Updated in $/Constructor/Source files/Application
//	Added cmd_NewItemDialog menu command.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 18:15
//	Checked in '$/Constructor/Source files/Application'
//	Comment: Disabled automatic QC debugging.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:20
//	Created
//	Comment: Generic application (common code to all targets)
//
// ===========================================================================

#ifdef WINVER
	#include "ctor.h"
#endif

#if PP_Target_Carbon
	#include <UNavServicesDialogs.h>
#else
	#include <UConditionalDialogs.h>
#endif

// ===========================================================================

#include "CAGenericTarget.h"
#include <LStaticText.h>
#include <LCaption.h>

#if Constructor_ForMacOS
	#include "CAMacOSTarget.h"
#elif Constructor_ForWindows
	#include "CAWindowsTarget.h"
#elif Constructor_ForJava
	#include "CAJavaTarget.h"
#elif Constructor_ForPilot
	#include "CAPilotTarget.h"
#else
	#error Unknown target
#endif

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Application
#include "CAPreferencesFile.h"
#include "Constructor.PPob.h"

	// Core : Catalog
#include "CLWindow.h"

	// Core : Data model : Core classes
#include "StDMSetup.h"

	// Core : Data model : Utilities
#include "DMClipboard.h"

	// Core : Display classes (Eric) : Dialog handlers
#include "StAboutDialogHandler.h"
#include "StKeyDialogHandler.h"

	// Core : Display classes (Eric) : Window menu
#include "LWindowAppAttachment.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Documents : Generic resource file
#include "PDDocument.h"

	// Core : Editors : Generic editor
#include "RETypeList.h"

	// Core : Editors : Views : Data model
#include "VETypeList.h"

	// Core : Editors : Views : User interface
#include "VEEditor.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// Core : Utility classes
#include "CAGrowZone.h"
#include "Constructor.new.h"
#include "UApplicationFile.h"
#include "UMemoryUtils.h"

	// PowerPlant : In progress
#ifndef WINVER
	#include <LMercutioMBAR.h>
#endif

	// PowerPlant : Utility classes
#include <UProfiler.h>
#include <UResourceMgr.h>

#ifndef __GNUC__
		// Libraries : DebugNew
	#include "DebugNew.h"

		// Libraries : MetroNub utilities
	#include "MetroNubUtils.h"

		// Libraries : QC API
	#include "QCAPI.h"

		// Libraries : Spotlight interface
	#include "SpotlightAPI.h"
#endif

	// Libraries : MoreFiles
//#include "FSpCompat.h"

#if Constructor_Debug
	#include <UOnyx.h>
	#include <UHeapUtils.h>
	#include <UDebugNew.h>
	
	#include <LCommanderTree.h>
	#include <LPaneTree.h>
	#include <LTreeWindow.h>
#endif

#include "CScrollWheelAttachment.h"

	// MSL Headers
#include <new> //for bad_alloc
using namespace std;


// ===========================================================================

#ifdef WINVER
	EDebugAction	gDebugThrow;
	EDebugAction	gDebugSignal;
#endif

// ===========================================================================

#ifndef ProfileLaunch
#define ProfileLaunch	0				// set to zero to run normally,
										// set to one to profile launch
										//		(quits immediately)
#endif

#ifndef ProfileConstructor
#define ProfileConstructor	0			// zero runs normally, one profiles
										// all of Constructor
#endif

#if ProfileLaunch && ProfileConstructor
#error "Cannot profile both the launch and entire app simultaneously"
#endif

// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT		ALRT_NoDragAndDrop	= 1400;
const ResIDT		ALRT_NoThreads		= 1401;
const ResIDT		ALRT_MinSystemVers	= 1402;

const ResIDT		PPob_AboutBox		= 1450;

const ResIDT		PPob_CantOpenFile	= 1460;
const PaneIDT		Pane_FileName		= 'NAME';
const PaneIDT		Pane_Reason			= 'RSN ';

const ExceptionCode	Err_OpeningProcess	= 1465;
const ExceptionCode Err_OpeningSystem   = 1466;
const ExceptionCode Err_OpeningNoResFork = 1467;

const ResIDT		STR_OpenErrors			= 1465;
const SInt16		OpenErrors_OpenApp		= 1;
const SInt16		OpenErrors_OpenSystem	= 2;
const SInt16		OpenErrors_NoResFork	= 3;

const ResIDT		STR_TargetInfo		= 1099;
const SInt16		str_ProductName		= 1;
const SInt16		str_PrefsFileName	= 2;

const ResIDT		STR_GenericStrings		= 1466;
const SInt16		str_NavServicesError	= 1;
const SInt16		str_RebuildDesktopDB	= 2;


// ===========================================================================
// 		* Template inclusion for building under MSVC++
// ===========================================================================

#ifdef WINVER
template <class T>
StValueChanger<T>::StValueChanger(
	T	&ioVariable,
	T	inNewValue)
		: mVariable(ioVariable),
		  mOrigValue(ioVariable)
{
	ioVariable = inNewValue;
}


template <class T>
StValueChanger<T>::~StValueChanger()
{
	mVariable = mOrigValue;
}

// Explicit Template instantiation for some common types
	
template class	StValueChanger<Boolean>;
template class	StValueChanger<EDebugAction>;
#endif


// ===========================================================================
//		* main
// ===========================================================================
//	Do application initialization, create the application object,
//	and run the main event loop.

int
#ifdef WINVER
AppMain()
#else
main(void)
#endif
{

	// Launch-time profiling support.

	#if ProfileLaunch
	StProfileSection launchProf("\pLaunch.prof", 7000, 100);
	#endif
	
	#if ProfileConstructor
	StProfileSection constrProf("\pConstrutor.prof", 32000, 250);
	#endif

	// Initialize memory and toolbox.

	InitializeHeap(15);
	UQDGlobals::InitializeToolbox();
	::InitCursor();
	::FlushEvents(everyEvent, nil);
	
	UScreenPort::Initialize();
	
	// make sure at least Mac OS 7.5
	
	SInt32 sysVersion;
	OSErr err = ::Gestalt(gestaltSystemVersion, &sysVersion);
	if ( err || (sysVersion < 0x750) ) {
		StApplicationContext appContext;
		::StopAlert(ALRT_MinSystemVers, nil);
		::ExitToShell();
	}
	
	// Make sure Drag Manager is available.
	
	if (!LDragAndDrop::DragAndDropIsPresent()) {
		StApplicationContext appContext;
		::StopAlert(ALRT_NoDragAndDrop, nil);
		::ExitToShell();
	}

	// Set up PowerPlant assertion handling. When running with
	// the MW debugger, we want to stop anytime an assertion fails
	// (i.e. a Signal_ macro is thrown). In all other cases,
	// we want to see an assertion-failed dialog.
	
	#if Constructor_Debug
		if (AmIBeingMWDebugged())
			UDebugging::gDebugSignal = debugAction_LowLevelDebugger; // debugAction_SourceDebugger;
		else
			UDebugging::gDebugSignal = debugAction_Alert;
	#else
		UDebugging::gDebugSignal = debugAction_Alert;			// should be "alert" for production builds
	#endif

	// Disable PowerPlant exception alerts. When debugging, the
	// "Break on C++ Exception" menu command can be used to stop
	// the program when an exception is thrown. There's no longer
	// any need for the program to introduce a stop.
	
	UDebugging::gDebugThrow = debugAction_Nothing;
	
	// Replace DebugNew's error handler.
	
	#if Constructor_Debug
		DebugNewSetErrorHandler(BetterDebugNewErrorHandler);
	#endif
	
	// replace QC's error handler
	
	#if Constructor_Debug && PP_QC_Support
		UQC::InstallErrorHandler();
	#endif
	
	// Enable QC debugging.
	
	#if Constructor_Debug && PP_QC_Support
		if (QCInstalled() == kQCNoErr)
			QCActivate(nil);
	#endif

	// Find application file. We'll need this to scan subfolders, etc.

	UApplicationFile::FindApplicationFile();

	// Set up grow zone.

	CAGrowZone grow(32768);
	
	// Create menu bar. We do this early so leak detection can ignore
	// its "leaks".

#if !Constructor_Mercutio || defined(WINVER)
	UEnvironment::InitEnvironment();
	LMenuBar *theMenuBar = new LMenuBar(MBAR_Initial);
	theMenuBar->SetModifierKeys( cmdKey + optionKey );
	OSStatus status;
	ResIDT theMenuID;
	MenuHandle theMenuHandle;
	SInt16 theItem;
	long result;
	::Gestalt(gestaltMenuMgrAttr, &result);
	if ((result & gestaltMenuMgrAquaLayoutMask) == gestaltMenuMgrAquaLayoutMask)
	{
		theMenuBar->FindMenuItem(cmd_Quit, theMenuID, theMenuHandle, theItem);  // find the quit menu
		LMenu* theMenu = theMenuBar->FetchMenu(theMenuID);	// get the menu object
		theMenu->RemoveItem(theItem);	// remove the quit item
		theMenu->RemoveItem(theItem - 1);	// remove the divider preceeding the quit item
	}

	// Use dynamic menu items.  The errors should be non-fatal -- if support for the
	// kMenuItemAttrDynamic mask isn't implemented (prior to CarbonLib 1.1) we'll just
	// have duplicate names in the menus.
	theMenuBar->FindMenuItem(cmd_Close, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);
	theMenuBar->FindMenuItem(cmd_CloseAll, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);

	theMenuBar->FindMenuItem(cmd_Save, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);
	theMenuBar->FindMenuItem(cmd_SaveAll, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);

	theMenuBar->FindMenuItem(cmd_Print, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);
	theMenuBar->FindMenuItem(cmd_PrintOne, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);

	theMenuBar->FindMenuItem(cmd_NewItem, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);
	theMenuBar->FindMenuItem(cmd_NewItemDialog, theMenuID, theMenuHandle, theItem);
	status = ChangeMenuItemAttributes(theMenuHandle, theItem, kMenuItemAttrDynamic, 0);
#else
	new LMercutioMBAR(MBAR_Initial);
#endif
	
	// Register PowerPlant display classes.
	
	RegisterAllConstructorPPClasses();
	
	// All static allocations have happened by now. Reset leak counters.
	// Don't bother resetting DebugNew's leak counters since it reports
	// the "reset" leaks as <unknown>. Grrr...
	
	#if Constructor_Debug && !__MACH__
		SLResetLeaks();
	#endif

	// Bracket here so DebugNew can catch application-level leaks.

	{
	
		// Configure the data model.
		
		StDMSetup dataModel;

		// Start up the preferences file.

		LStr255 prefsFileName(STR_TargetInfo, str_PrefsFileName);
		CAPreferencesFile thePrefs(prefsFileName);

		// Set up the correct application object for the target.

		#if Constructor_ForMacOS
			CAMacOSTarget theApp;
		#elif Constructor_ForWindows
			CAWindowsTarget theApp;
		#elif Constructor_ForJava
			CAJavaTarget theApp;
		#elif Constructor_ForPilot
			CAPilotTarget theApp;
		#else
			#error Unknown target
		#endif
		
		// Run the application.

		StApplicationContext appContext;
		theApp.Run();
	
	}
	
	// If debugging, it's time to report on leaked C++ objects.
	
	#if Constructor_Debug
	
		// We can remove a few known leaked objects.

		LMenuBar* globalMenuBar = LMenuBar::GetCurrentMenuBar();
		ValidateObject_(globalMenuBar);
		delete globalMenuBar;

		URegistrar::DisposeClassTable();
		
		LPeriodical::DeleteIdlerAndRepeaterQueues();
		
		LModelDirector *theDirector = LModelDirector::GetModelDirector();
		delete theDirector;

		LModelObject::DestroyLazyList();

		UScreenPort::Dispose();
		
		LComparator *theCompare = LComparator::GetComparator();
		delete theCompare;
		
		LLongComparator *theLongCompare = LLongComparator::GetComparator();
		delete theLongCompare;


		// Report any remaining leaked object.
	
#if !__MACH__
		SLDisable();
#endif
		DebugNewReportLeaks();
#if !__MACH__
		SLEnable();
#endif
		
	#endif
	
	return 0;
}


// ===========================================================================

#pragma mark -
#pragma mark *** CAGenericTarget ***

// ---------------------------------------------------------------------------
//		* CAGenericTarget
// ---------------------------------------------------------------------------
//	Application constructor

CAGenericTarget::CAGenericTarget()
{

	// Clear global window pointers.

	mAlignmentPalette = nil;
	
	// Create the clipboard object.
	
	AddAttachment(new DMClipboard);
}


// ---------------------------------------------------------------------------
//		* ~CAGenericTarget
// ---------------------------------------------------------------------------
//	Destructor

CAGenericTarget::~CAGenericTarget()
{

	// Tear down global view type list.

	VETypeList::ReleaseTypeList();
	
	// Tear down global resource type list.
	
	RETypeList::ReleaseTypeList();

#if Constructor_Debug
//	Forget_(mDebugMenuAttachment);
#endif

}


// ---------------------------------------------------------------------------
//	¥ CatchException											   [protected]
// ---------------------------------------------------------------------------
//	Called when Run() catches an exception with an LException parameter

void
CAGenericTarget::CatchException(
	const LException&	inException)
{
	if (inException.GetErrorCode() == kNavMissingKindStringErr)
	{
		// For some reason, many users have complained that Constructor
		// won't open and save files because kNavMissingKindStringErr is being
		// returned from the Nav Services calls.  So here we'll tell the user
		// why they're getting this particular error and what they should do to
		// get around the problem.
		
		SInt16		itemHit;
		LStr255		errorString(STR_GenericStrings, str_NavServicesError);
		LStr255		explnString(STR_GenericStrings, str_RebuildDesktopDB);
		OSStatus	status;
		AlertStdAlertParamRec	alertParams;
		
		alertParams.movable = true;
		alertParams.helpButton = false;
		alertParams.filterProc = NULL;
		alertParams.defaultText = NULL;
		alertParams.cancelText = NULL;
		alertParams.otherText = NULL;
		alertParams.defaultButton = kAlertStdAlertOKButton;
		alertParams.cancelButton = 0;
		alertParams.position = kWindowDefaultPosition;

		status = ::StandardAlert(kAlertStopAlert, errorString, explnString, &alertParams, &itemHit);
	}
	else
		LDocApplication::CatchException(inException);
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu commands

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Set default menu titles for menu items that can change dynamically.

void
CAGenericTarget::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{

	// Dispatch command.

	switch (inCommand) {
	
		// New and Open: Enabled if memory is available.
	
		case cmd_New:
		case cmd_Open:
			outEnabled = !UMemoryUtils::MemoryIsLow();
			break;

		// Close All: Enabled if there are any documents.
		
		case cmd_CloseAll:
			outEnabled = LDocument::GetDocumentList().GetCount() > 0;
			break;
		
		// Save All: Enabled if there are any modified documents.
		
		case cmd_SaveAll: {
			outEnabled = false;
			LArrayIterator docIter(LDocument::GetDocumentList());
			LDocument* document;
			while (docIter.Next(&document)) {
				ValidateObject_(document);
				if (document->IsModified()) {
					outEnabled = true;
					break;
				}
			}
			break;
		}
		
		// Clipboard items: Set standard text for menu item.
		
		case cmd_Cut:
			outEnabled = false;
			::GetIndString(outName, STR_DefaultEditStrings, str_CutDefault);
			break;
		case cmd_Copy:
			outEnabled = false;
			::GetIndString(outName, STR_DefaultEditStrings, str_CopyDefault);
			break;
		case cmd_Paste:
			outEnabled = false;
			::GetIndString(outName, STR_DefaultEditStrings, str_PasteDefault);
			break;
		case cmd_Clear:
			outEnabled = false;
			::GetIndString(outName, STR_DefaultEditStrings, str_ClearDefault);
			break;
		case cmd_Duplicate:
			outEnabled = false;
			::GetIndString(outName, STR_DefaultEditStrings, str_Duplicate);
			break;
		case cmd_NewItem:
		case cmd_NewItemDialog:
			outEnabled = false;
			::GetIndString(outName, STR_DefaultEditStrings, str_NewItem);
			break;
		case cmd_EditItem:
			outEnabled = false;
			::GetIndString(outName, STR_DefaultEditStrings, str_EditItem);
			break;

		// Property inspector: Disable.

		case cmd_PropertyInspector:
			outEnabled = false;
			outUsesMark = true;
			outMark = 0;
			break;
		
		// Catalog, Alignment Palette and Show Types: Enable.
	
		case cmd_Catalog:
		case cmd_AlignPalette:
			outEnabled = !UMemoryUtils::MemoryIsLow();
			outUsesMark = true;
			outMark = 0;
			break;

		// Otherwise, let application class handle it.

		default:
			LDocApplication::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
			break;
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Respond to requests for global palettes only.

Boolean
CAGenericTarget::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	// Dispatch command.

	switch (inCommand) {
	
		// Close All: Close all documents.
	
		case cmd_CloseAll: {
			LArrayIterator docIter(LDocument::GetDocumentList());
			LDocument* document;
			while (docIter.Next(&document)) {
				ValidateObject_(document);
				document->AttemptClose(true);
			}
			return true;
		}
		
		// Save All: Save all modified documents.
		
		case cmd_SaveAll: {
			LArrayIterator docIter(LDocument::GetDocumentList());
			LDocument* document;
			while (docIter.Next(&document)) {
				ValidateObject_(document);
				if (document->IsModified())
					document->DoSave();
			}
			return true;
		}
		
		// Catalog: Open catalog window.
		
		case cmd_Catalog:
			CLWindow::OpenCatalogWindow();
			return true;

		// Alignment palette: Create or show palette.

		case cmd_AlignPalette:
			if (mAlignmentPalette != nil) {
				ValidateObject_(mAlignmentPalette);
				if (mAlignmentPalette->IsVisible())
					mAlignmentPalette->Hide();
				else
					mAlignmentPalette->Show();
			}
			else
				mAlignmentPalette = VEEditor::CreateAlignmentPalette();
			return true;

		// Otherwise: See if PP application wants it.

		default:
			return LDocApplication::ObeyCommand(inCommand, ioParam);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization behaviors

// ---------------------------------------------------------------------------
//		* Run
// ---------------------------------------------------------------------------
//	Overriden to see that global windows are set up and torn down
//	properly.

void
CAGenericTarget::Run()
{

	// Make the global resource type list.
	
	MakeResTypeList();
	
	// Make the global view class list.
	
	MakeTypeList();

	// Reopen global windows that were open when we closed down.
	
	ReopenGlobalWindows();
	
	// Run the application.
	
	#if ProfileLaunch
	
		// If profiling launch cycle, do the startup behaviors
		// but don't do the event loop.
	
		MakeMenuBar();
		MakeModelDirector();
		Initialize();
		
		SwitchTarget(this);
		::InitCursor();
		UpdateMenus();

	#else

		// Not profiling launch cycle, run normally.

		LApplication::Run();

	#endif

	// Remember which global windows were open.
	
	RememberOpenWindows();

}


// ---------------------------------------------------------------------------
//		* MakeMenuBar
// ---------------------------------------------------------------------------
//	We moved the make menu bar code to main() for debugging reasons.
//	This routine is overriden to disable the default "new LMenuBar" code.
//	Also installs the requisite code for window list menu.

void
CAGenericTarget::MakeMenuBar()
{

	// Set up window menu.
	
	AddAttachment(new LWindowAppAttachment);
	LWindowAppAttachment::SetWindowMenu(menu_Windows);
	
}

// ---------------------------------------------------------------------------
//		* Initialize
// ---------------------------------------------------------------------------
//	We override this for debugging purposes

void
CAGenericTarget::Initialize()
{
	LDocApplication::Initialize();
	
	if (CFM_AddressIsResolved_(InstallStandardEventHandler))
	{
		// Only add CScrollWheelAttachment if CarbonLib 1.1 is available
		AddAttachment(new CScrollWheelAttachment);
	}
	
	#if Constructor_Debug
	
			// Ensure the Tree classes are registered
/*		RegisterClass_(LCommanderTree);
		RegisterClass_(LPaneTree);
		RegisterClass_(LTreeWindow);

		mDebugMenuAttachment = new LDebugMenuAttachment;
		ValidateObject_(mDebugMenuAttachment);
		mDebugMenuAttachment->InitDebugMenu();
		AddAttachment( mDebugMenuAttachment );*/
	
	#endif
}


// ===========================================================================

#pragma mark -
#pragma mark ** debugging behaviors
	
// ---------------------------------------------------------------------------
//		* UseIdleTime
// ---------------------------------------------------------------------------
//	Overriden to trigger periodic heap scrambling, heap purging, and heap
//	checking.

//	JCD 980608 - removed - PP debugging classes take over this functionality.

/*
void
CAGenericTarget::UseIdleTime(
	const EventRecord& inMacEvent)
{

	// Do standard idle behaviors.
	
	LDocApplication::UseIdleTime(inMacEvent);

	// Trigger debugging checks if enough time has elapsed.
		
	#if Constructor_Debug

		static nextDebugTime = 0;
		if (::TickCount() > nextDebugTime) {

			// Trigger QC heap testing.
			
			if ( UQC::IsInstalled() ) {
				UQC::ScrambleHeap();
				UQC::CheckHeap();
				UQC::BoundsCheck();
			}
			
			// Trigger DebugNew block testing. Spotlight chokes on
			// DebugNew's pointer validation, so we disable it here.
			
			UDebugNew::ValidateAll();
			
			// Purge application heap.
			
			UHeapUtils::PurgeHeap();
			
			// Wait a while before doing this again. Once every few seconds
			// sounds right to me.
			
			nextDebugTime = ::TickCount() + 240;
		
		}

	#endif

}
*/

// ===========================================================================

#pragma mark -
#pragma mark ** low-memory overrides

// ---------------------------------------------------------------------------
//		* DispatchEvent
// ---------------------------------------------------------------------------
//	Overridden to show alert when command fails due to low memory.

void
CAGenericTarget::DispatchEvent(
	const EventRecord&	inMacEvent)
{
	try {
		LEventDispatcher::DispatchEvent(inMacEvent);
	}
	catch (bad_alloc&) {
		UMemoryUtils::ShowOutOfMemoryAlert();
	}
	catch (const LException& err) {
		switch (err.GetErrorCode()) {
			case err_NilPointer:
			case memFullErr:
				UMemoryUtils::ShowOutOfMemoryAlert();
				break;
			default:
				throw;
		}
	}
}


// ---------------------------------------------------------------------------
//		* SendAEQuit
// ---------------------------------------------------------------------------
//	Overridden to ensure that command succeeds even if memory is low.

void
CAGenericTarget::SendAEQuit()
{
	StCriticalSection crit;
	LApplication::SendAEQuit();
}


// ===========================================================================

#pragma mark -
#pragma mark ** new file behaviors

// ---------------------------------------------------------------------------
//		* MakeNewDocument										[protected]
// ---------------------------------------------------------------------------
//	Called from LDocApplication::ObeyCommand. Creates a new, untitled
//	document. In Constructor, this means create a new project file.
//	The pure virtual method MakeEmptyProject is used to create the appropriate
//	project for this target.

LModelObject*
CAGenericTarget::MakeNewDocument()
{
	try {
		return MakeEmptyProject();
	}
	catch(bad_alloc&) {
		Throw_(memFullErr);
	}
	return nil;
}


// ===========================================================================

#pragma mark -
#pragma mark ** opening existing files

// ---------------------------------------------------------------------------
//		* SendAEOpenDoc											[protected]
// ---------------------------------------------------------------------------
//	Overriden from LDocApplication to show a dialog if the file can't be
//	opened.

void
CAGenericTarget::SendAEOpenDoc(
	FSSpec& inFileSpec)
{
	try {
		AppleEvent	openEvent;
		UAppleEventsMgr::MakeAppleEvent(kCoreEventClass, kAEOpen, openEvent);
		
		OSErr err = ::AEPutParamPtr(&openEvent, keyDirectObject, typeFSS,
								&inFileSpec, sizeof(FSSpec));
		ThrowIfOSErr_(err);
		
		UAppleEventsMgr::SendAppleEvent(openEvent);
	}
	catch (const LException& inErr) {
		
		// JCD 980615 - previously if the above failed, we would
		// display a dialog saying why it failed. Due to various issues
		// with how this method would be called (and then the error
		// handling would or wouldn't be called, depending how you
		// opened a file (via File:Open or via Finder drag and drop))
		// we can't quite do the same. So we're kinda going to fake
		// things here...
		//
		// We have to do this because otherwise the error handler in
		// OpenDocument might be called twice (if you open via the
		// File menu) and we don't want that cause it looks poor.
		
		// if the error is one of our OpenDocument errors, we'll just
		// swallow it up.. it should have already been handled if we
		// got to this point. If it's not, then we'll continue on
		// through.
		
		if (	(inErr.GetErrorCode() != Err_OpeningProcess) &&
				(inErr.GetErrorCode() != Err_OpeningSystem) &&
				(inErr.GetErrorCode() != Err_OpeningNoResFork) ) {
			OpenDocument(&inFileSpec);
		}
	
		
		
		// Couldn't open document. Show a dialog explaining why.
/*	
		StApplicationContext appContext;
		StKeyDialogHandler dialog(PPob_CantOpenFile, this);
		ValidateObject_(dialog.GetDialog());
		
		// Set the caption containing the file name.

		LPane* caption = dialog.GetDialog()->FindPaneByID(Pane_FileName);
		ValidateObject_(caption);
		caption->SetDescriptor(inFileSpec.name);

		// Set the caption containing the reason for failure.

		caption = dialog.GetDialog()->FindPaneByID(Pane_Reason);
		ValidateObject_(caption);

		LStr255 reason((Int32) inErr);		//! TEMPORARY: would like to have a string for error
		if (inErr == Err_OpeningProcess)
			reason.Assign((ResIDT) STR_OpenErrors, 1);
		caption->SetDescriptor(reason);
		
		// Run the dialog.

		dialog.GetDialog()->ProcessCommand(cmd_PositionWindow);
		dialog.GetDialog()->Show();	
		MessageT message = msg_Nothing;
		while (message == msg_Nothing) {
			message = dialog.DoDialog();
		}
		
		// Remember the dialog's position, in case it moved.

		dialog.GetDialog()->ProcessCommand(cmd_RecordPosition);

		// Be sure that menus are updated properly.

		LCommander::SetUpdateCommandStatus(true);
*/

	}
}


// ---------------------------------------------------------------------------
//		* OpenDocument											[protected]
// ---------------------------------------------------------------------------
//	Open a document. Checks to make sure that the file doesn't belong to
//	another application or the System. Opening a running application
//	or System file is a bad thing.

void
CAGenericTarget::OpenDocument(
	FSSpec*		inMacFSSpecPtr)
{

	// See if this file is already open.
	
	LArrayIterator docIter(LDocument::GetDocumentList());
	LDocument* testDoc;

	while (docIter.Next(&testDoc)) {

		PDDocument* testPDDoc = (dynamic_cast<PDDocument*>(testDoc));
		ValidateObject_(testPDDoc);

		if (testPDDoc->OpenIfMatch(inMacFSSpecPtr))
			return;

	}

	// Disallow opening an open application.
	
	ProcessSerialNumber psn = { 0, kNoProcess };
	FSSpec processAppSpec;
	ProcessInfoRec infoRec;
	infoRec.processInfoLength = sizeof (infoRec);
	infoRec.processName = 0;
	infoRec.processAppSpec = &processAppSpec;
	
	while (::GetNextProcess(&psn) == noErr) {
		if (::GetProcessInformation(&psn, &infoRec) == noErr) {
			if (!::EqualString(processAppSpec.name, inMacFSSpecPtr->name, false, true))
				continue;				// filenames don't match
			if (processAppSpec.vRefNum != inMacFSSpecPtr->vRefNum)
				continue;				// volume IDs don't match
			if (processAppSpec.parID != inMacFSSpecPtr->parID)
				continue;				// parent directories don't match
			
			// The file is a running process. Don't open it.

			DoCantOpenFileDialog(inMacFSSpecPtr->name, Err_OpeningProcess);
			Throw_(Err_OpeningProcess);
		}
	}
	
	// Disallow opening the current system file.
	
	do {

		StResourceContext sysContext(0);
		LStr255 fileName;
		FCBPBRec fcbRec;
		fcbRec.ioCompletion = nil;
		fcbRec.ioNamePtr = (StringPtr) fileName;
		fcbRec.ioVRefNum = 0;
		fcbRec.ioRefNum = ::CurResFile();
		fcbRec.ioFCBIndx = 0;
		if (::PBGetFCBInfoSync(&fcbRec) == noErr) {
			if (!::EqualString(fileName, inMacFSSpecPtr->name, false, true))
				continue;				// filenames don't match
			if (fcbRec.ioFCBVRefNum != inMacFSSpecPtr->vRefNum)
				continue;				// volume IDs don't match
			if (fcbRec.ioFCBParID != inMacFSSpecPtr->parID)
				continue;				// parent directories don't match

			// The file is the system file. Don't open it.

			DoCantOpenFileDialog(inMacFSSpecPtr->name, Err_OpeningSystem);
			Throw_(Err_OpeningSystem);

		}

		// else system file isn't open: we're really hosed

	} while (0);
	
	// JCD 980615 - check to ensure the file even has a resource fork in the
	// first place. We do allow opening of any file, but without a resource
	// fork, things are kinda useless (it's ok if there's a resource fork but
	// it's empty).
	//
	// This code is identical to MoreFiles' CheckForForks() (we can't call it
	// here as Jim Luther limited the function in scope).
	//
	// This fixes MW05619.
	//

	// icw -- Change to only complain if both forks are empty.  Constructor
	// can now work with flat resource files.
	
	HParamBlockRec pb;	
	pb.fileParam.ioNamePtr = inMacFSSpecPtr->name;
	pb.fileParam.ioVRefNum = inMacFSSpecPtr->vRefNum;
	pb.fileParam.ioFVersNum = 0;
	pb.fileParam.ioDirID = inMacFSSpecPtr->parID;
	pb.fileParam.ioFDirIndex = 0;
	ThrowIfOSErr_(::PBHGetFInfoSync(&pb));
	
	if ((pb.fileParam.ioFlRLgLen == 0) and (pb.fileParam.ioFlLgLen == 0))
	{
		// no resource fork, complain
		// We could modify string rsrc to say both forks empty, but I think
		// saying no resource fork is good enough.
		DoCantOpenFileDialog(inMacFSSpecPtr->name, Err_OpeningNoResFork);
		Throw_(Err_OpeningNoResFork);
	}
	
	// Okay, the file isn't open... create a new doc object with the associated file.
	// Find out what kind of file it is: this will affect how we open it.

	FInfo fileInfo;
	ThrowIfOSErr_(::FSpGetFInfo(inMacFSSpecPtr, &fileInfo));
	
	try {
		MakeProjectFromFile(*inMacFSSpecPtr);
	}
	catch (bad_alloc&) {
		Throw_(memFullErr);
	}

}


// ---------------------------------------------------------------------------
//		* DoStdFileOpenDialog									[protected]
// ---------------------------------------------------------------------------
//	Choose a file to edit.

void
CAGenericTarget::DoStdFileOpenDialog(void)
{
	bool isGood;
	FSSpec theSpec;
	
#if PP_Target_Carbon
	OSType			openFileTypes[] = { 'rsrc', 'RSRC' };
	SInt16			numOpenFileTypes = sizeof(openFileTypes) / sizeof(OSType);
	LFileTypeList	fileTypes(numOpenFileTypes, openFileTypes);
	NavDialogOptionFlags	flags;
	UNavServicesDialogs::LFileChooser	chooser;
	
	flags = kNavAllowPreviews + kNavAllFilesInPopup + kNavDontAddTranslateItems +
		kNavDontAutoTranslate + kNavSelectAllReadableItem + kNavSupportPackages +
		kNavAllowOpenPackages;
	flags &= ~kNavAllowMultipleFiles;		// Can't select multiple files

	NavDialogOptions*	options = chooser.GetDialogOptions();
	options->dialogOptionFlags = flags;

	isGood = chooser.AskOpenFile(fileTypes);

	if (isGood)
	{
		chooser.GetFileSpec(1, theSpec);
	}
#else
	isGood = UConditionalDialogs::AskOpenOneFile('rsrc', theSpec,
		kNavAllowPreviews + kNavAllFilesInPopup + kNavDontAddTranslateItems + kNavDontAutoTranslate);
#endif
	
	if (isGood)
		SendAEOpenDoc(theSpec);

}


// ---------------------------------------------------------------------------
//	* DoCantOpenFileDialog						[protected]
// ---------------------------------------------------------------------------
//	JCD 980615 - OpenDocument() is called when you wish to open a file. It
// typically will be called directly from the AppleEvent handlers, or will
// be called by SendAEOpenDoc(). The importance here is that OpenDocument
// throws exceptions if there are problems. If we were called via the
// AppleEvent handlers, we'd never get a chance to handle the error... so I
// rewrote OpenDocument a bit (and SendAEOpenDoc) to extract out the error
// handling code into this.

void
CAGenericTarget::DoCantOpenFileDialog(
	ConstStringPtr	inFilename,
	ExceptionCode	inError )
{
	StApplicationContext appContext;
	StKeyDialogHandler dialog(PPob_CantOpenFile, this);
	ValidateObject_(dialog.GetDialog());
	
	// Set the caption containing the file name.

	LStaticText* caption = dynamic_cast<LStaticText*>(dialog.GetDialog()->FindPaneByID(Pane_FileName));
	ValidateObject_(caption);
	caption->SetDescriptor(inFilename);

	// Set the caption containing the reason for failure.

	caption = dynamic_cast<LStaticText*>(dialog.GetDialog()->FindPaneByID(Pane_Reason));
	ValidateObject_(caption);

	LStr255 reason(static_cast<SInt32>(inError));		//! TEMPORARY: would like to have a string for error
	SInt16 index = -1;
	if (inError == Err_OpeningProcess) {
		index = OpenErrors_OpenApp;
	} else if ( inError == Err_OpeningSystem) {
		index = OpenErrors_OpenSystem;
	} else if ( inError == Err_OpeningNoResFork) {
		index = OpenErrors_NoResFork;
	}

	if ( index > -1 ) {
		reason.Assign(STR_OpenErrors, index);
	}
	
	caption->SetDescriptor(reason);
	
	// Run the dialog.

	dialog.GetDialog()->ProcessCommand(cmd_PositionWindow);
	dialog.GetDialog()->Show();	
	MessageT message = msg_Nothing;
	while (message == msg_Nothing) {
		message = dialog.DoDialog();
	}
	
	// Remember the dialog's position, in case it moved.

	dialog.GetDialog()->ProcessCommand(cmd_RecordPosition);

	// Be sure that menus are updated properly.

	LCommander::SetUpdateCommandStatus(true);
}

// ===========================================================================

#pragma mark -
#pragma mark ** resource type list behaviors
	
// ---------------------------------------------------------------------------
//		* MakeResTypeList										[protected]
// ---------------------------------------------------------------------------
//	Called when Constructor opens. Build the list of resource types that
//	can be edited.

void
CAGenericTarget::MakeResTypeList()
{
	RETypeList::GetTypeList();
}


// ===========================================================================

#pragma mark -
#pragma mark ** global windows

// ---------------------------------------------------------------------------
//		* RememberOpenWindows									[protected]
// ---------------------------------------------------------------------------
//	Called when Constructor quits. Remember which global windows (palettes,
//	types list, etc.) were open, so they can be opened again when Constructor
//	is launched again.

void
CAGenericTarget::RememberOpenWindows()
{

	// Create the data stream which will record the window types
	// that are open. The stream format is a list of command IDs
	// which will be executed when the program is launched again.
	
	// We record only the command numbers for those windows that
	// are currently open.
	
	LHandleStream windowList;

	// Let subclass specify which windows are open.

	RememberOpenWindowsSelf(windowList);
	
	// Check for alignment palette.
	
	if (mAlignmentPalette != nil) {
		ValidateObject_(mAlignmentPalette);
		if (mAlignmentPalette->IsVisible()) {
			mAlignmentPalette->ProcessCommand(cmd_RecordPosition);
			windowList << (SInt32) cmd_AlignPalette;
		}
	}

	// Check for catalog.
	
	LWindow* catalog = CLWindow::GetCatalogWindow();
	if (catalog != nil) {
		catalog->ProcessCommand(cmd_RecordPosition);
		windowList << (SInt32) cmd_Catalog;
	}

	// Write this data to prefs file.
	
	StPreferencesContext prefsContext;
	if (prefsContext.IsValid()) {					//* 2.4a2: BUG FIX #1072: added if statement
	
		StNewResource windowListR('WOPN', 1000, 0, true);
													//! TEMPORARY: magic numbers
		ValidateHandle_(windowListR);
	
		::SetHandleSize(windowListR, windowList.GetLength());
		// icw -- Only call BlockMoveData if we have something to write.
		// This was causing a problem on Mac OS X Public Beta for some odd reason.
		if (windowList.GetLength() != 0)
		{
			::BlockMoveData(*(windowList.GetDataHandle()),
							*((Handle) windowListR),
							windowList.GetLength());
		}
		windowListR.Write();
	
	}
}


// ---------------------------------------------------------------------------
//		* RememberOpenWindowsSelf								[protected]
// ---------------------------------------------------------------------------
//	Override hook. Remember what target-specific global windows are open
//	by writing the command numbers that re-create those windows to the
//	stream.

void
CAGenericTarget::RememberOpenWindowsSelf(
	LStream&	/* inCommandStream */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* ReopenGlobalWindows									[protected]
// ---------------------------------------------------------------------------
//	Called when Constructor is launched. Reopens the global windows which were
//	open when Constructor was last closed down.

void
CAGenericTarget::ReopenGlobalWindows()
{

	// See if we have window prefs resource.
	
	StPreferencesContext prefsContext;
	if (!prefsContext.IsValid())				//* 2.4a2: BUG FIX #1072: added if statement
		return;
		
	StResource windowListR('WOPN', 1000, false, true);
												//! TEMPORARY: magic numbers
	if (((Handle) windowListR) == nil)
		return;
	
	// Yes, we have one. The window open prefs resources is a list of
	// commands that get executed. These commands will results in
	// the same set of windows being reopened as were opened when
	// Constructor was last quit.
	
	ValidateHandle_(windowListR);
	LHandleStream windowList((Handle) windowListR);

	while (windowList.GetLength() - windowList.GetMarker() >= 4) {
		try {
			CommandT windowCmd = 0;
			windowList >> windowCmd;
			ProcessCommand(windowCmd, nil);
		}
		catch(...) {
			// ignore errors & continue
		}
	}

	// On our way out, we have to make sure handle doesn't get double disposed.
	
	windowList.DetachDataHandle();	

}


// ---------------------------------------------------------------------------
//		* AllowSubRemoval										[protected]
// ---------------------------------------------------------------------------
//	If alignment palette wants to close, hide it rather than deleting it.

Boolean
CAGenericTarget::AllowSubRemoval(
	LCommander*		inSub)
{

	// Validate pointers.
	
	ValidateObject_(inSub);
	
	// If it's the alignment palette, just hide it. It's not allowed to go away.

	if (inSub == mAlignmentPalette) {
		ValidateObject_(mAlignmentPalette);
		mAlignmentPalette->Hide();
		return false;
	}
	else
		return LCommander::AllowSubRemoval(inSub);

}


// ===========================================================================

#pragma mark -
#pragma mark ** about box

// ---------------------------------------------------------------------------
//		* ShowAboutBox											[protected]
// ---------------------------------------------------------------------------
//	Show the application's about box until the user clicks on it.

void
CAGenericTarget::ShowAboutBox()
{

	// Load the dialog box resource.

	StApplicationContext appContext;
	StAboutDialogHandler theAboutBox(PPob_AboutBox, this);
	appContext.Exit();
	ValidateObject_(theAboutBox.GetDialog());
	
	// Ensure that top line in about box has the proper product name.
	
									//! TEMPORARY: magic number
	LCaption* product = dynamic_cast<LCaption*>(theAboutBox.GetDialog()->FindPaneByID('PROD'));
	ValidateObject_(product);
	LStr255 productStr(STR_TargetInfo, str_ProductName);
	product->SetDescriptor(productStr);

	// Ensure that the line above credits is erased to white.
	
									//! TEMPORARY: magic number
	LPane* line = theAboutBox.GetDialog()->FindPaneByID('LINE');
	ValidateObject_(line);
	line->AddAttachment(new LEraseAttachment);
	
	// Find the pane that will host the version number.
	
									//! TEMPORARY: magic number
	LCaption* versCaption = dynamic_cast<LCaption*>(theAboutBox.GetDialog()->FindPaneByID('VERS'));
	ValidateObject_(versCaption);
	LStr255 tempStr;
	versCaption->GetDescriptor(tempStr);
	
	// Read our 'vers' 1 resource to get the version number.
	
	appContext.Enter();
	Handle vers1Resource = ::Get1Resource('vers', 1);
	ValidateHandle_(vers1Resource);
	HLock(vers1Resource);
	VersRec* versRecP = (VersRec*) *vers1Resource;
	
	// Tack the version number onto the existing string,
	// then set the caption to use the combined string.

	tempStr += versRecP->shortVersion;
	versCaption->SetDescriptor(tempStr);
	::ReleaseResource(vers1Resource);
	
	// Run the about box dialog until the user clicks on it.
	
	theAboutBox.SetSleepTime(0);
	theAboutBox.GetDialog()->Show();
	while (theAboutBox.DoDialog() != cmd_Close)
		{ }

	// Dispose of the picture displayed by the about box.
	
									//! TEMPORARY: magic number
	Handle aboutBoxPict = ::Get1Resource('PICT', 1450);
	ValidateHandle_(aboutBoxPict);
	::ReleaseResource(aboutBoxPict);

}

#include "GSharableString.h"