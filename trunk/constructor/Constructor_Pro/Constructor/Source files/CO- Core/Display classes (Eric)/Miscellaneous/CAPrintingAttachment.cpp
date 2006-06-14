// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAPrintingAttachment.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/16/96
//     $Date: 2006/01/18 01:32:42 $
//  $History: CAPrintingAttachment.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 03/20/97   Time: 10:50
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Fixed a bug in DoPrintJob.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 12:02p
//	Updated in $/ConstructorWin/Sources
//	Added MSVC prefix file; other changes
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/30/97   Time: 18:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Fixed ExecuteSelf so that it calls FindCommandStatus. (Bug fix #1259.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/17/96   Time: 11:27
//	Updated in $/Constructor/Source files/Display classes (Eric)/Miscellaneous
//	Fixed a syntax error.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/17/96   Time: 00:14
//	Created in $/Constructor/Source files/Display classes (Eric)/Miscellaneous
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CAPrintingAttachment.h"

	// Core : Application
#include "CAPreferencesFile.h"

	// PowerPlant : Utilities
#include <UResourceMgr.h>

// ===========================================================================
//		* Class variables
// ===========================================================================

LPrintSpec CAPrintingAttachment::sPrintRecordSpec;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResType	PageSetup_ResType		= 'PGSU';
const ResIDT	PageSetup_ResID			= 23800;

const PaneIDT	Pane_DocumentName		= 'DOC ';
const PaneIDT	Pane_ResourceID			= 'Rid ';
const PaneIDT	Pane_DateField			= 'DATE';
const PaneIDT	Pane_PageNumber			= 'PAGE';

const ResIDT	STR_PrintoutStrings		= 23800;
const SInt16	str_DateTimeSeparator	= 1;				// ", "


// ===========================================================================

#pragma mark *** CAPrintingAttachment ***

// ---------------------------------------------------------------------------
//		* CAPrintingAttachment(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor.

CAPrintingAttachment::CAPrintingAttachment(
	LStream*	inStream)

: LAttachment(inStream)

{

	// Read stream data.

	(*inStream)	>> mPrintoutPPobID
				>> mPrintoutPlaceholderID
				>> mPrintoutTbarPlaceholderID
				>> mPrintedPaneID
				>> mPrintedTitleBarID;

	// Make page setup persistent across all views. Try to grab page setup from
	// preferences resource.

#if __PowerPlant__ >= 0x02112002
	// icw -- Handle ownership is different with Get/SetPrintRecord() in PP 2.1.1d2

	StHandleBlock thePrintRecordH(Handle_Nil);
	thePrintRecordH.Adopt(sPrintRecordSpec.GetPrintRecord());
	if (not thePrintRecordH.IsValid()) {
		StPreferencesContext prefsFile;
		thePrintRecordH.Adopt(::Get1Resource(PageSetup_ResType, PageSetup_ResID));
		if (thePrintRecordH.IsValid()) {
			ValidateHandle_(thePrintRecordH);
			::DetachResource(thePrintRecordH);
			sPrintRecordSpec.SetPrintRecord(thePrintRecordH);
		}
	}

#else

	if (sPrintRecordSpec.GetPrintRecord() == nil) {
		StPreferencesContext prefsFile;
		THPrint thePrintRecordH = (THPrint) ::Get1Resource(PageSetup_ResType, PageSetup_ResID);
		if (thePrintRecordH != nil) {
			ValidateHandle_((Handle) thePrintRecordH);
			::DetachResource((Handle) thePrintRecordH);
			sPrintRecordSpec.SetPrintRecord(thePrintRecordH);
		}
	}

#endif // __PowerPlant__ >= 0x02112002
	
	// Can't find page setup in prefs file. Use default.
	
/*	if (sPrintRecordSpec.GetPrintRecord() == nil)
	{
		StPrintSession	session(sPrintRecordSpec);
		sPrintRecordSpec.SetToAppDefault();
	}*/
}


// ---------------------------------------------------------------------------
//		* ~CAPrintingAttachment
// ---------------------------------------------------------------------------
//	Destructor

CAPrintingAttachment::~CAPrintingAttachment()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** event dispatching

// ---------------------------------------------------------------------------
//		* ExecuteSelf											[protected]
// ---------------------------------------------------------------------------
//	Catches Page Setup, Print, and Print One commands.

void
CAPrintingAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();

	// Common initialization & sanity check.

	SetExecuteHost(true);
	
	// Dispatch event.
	
	switch (inMessage) {
	
		case msg_CommandStatus:
			FindCommandStatus((SCommandStatus*) ioParam);
			break;
		
		case cmd_PageSetup:
			DoPageSetup();
			SetExecuteHost(false);
			break;

		case cmd_Print:
			DoPrint();
			SetExecuteHost(false);
			break;

		case cmd_PrintOne:
			DoPrintOne();
			SetExecuteHost(false);
			break;

	}
}


// ---------------------------------------------------------------------------
//		* FindCommandStatus										[protected]
// ---------------------------------------------------------------------------
//	Enables Page Setup, Print, and Print One commands.

void
CAPrintingAttachment::FindCommandStatus(
	SCommandStatus*		inCommandStatus)
{

	// Validate pointers.

	ValidateThis_();
	ThrowIfNil_(inCommandStatus);

	// Don't allow host commander to take command status, unless specifically allowed.

	SetExecuteHost(false);
	
	// Make sure menu title strings come from application's resource fork, not
	// any user document.
	
	StApplicationContext appContext;
	
	// Dispatch command.

	switch (inCommandStatus->command) {

		case cmd_PageSetup:
		case cmd_Print:
		case cmd_PrintOne:
			*(inCommandStatus->enabled) = true;
			break;
			
		default:
			SetExecuteHost(true);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** printing commands

// ---------------------------------------------------------------------------
//		* DoPageSetup											[protected]
// ---------------------------------------------------------------------------
//	Run the Page Setup command.

void
CAPrintingAttachment::DoPageSetup()
{

	// Validate pointers.
	
	ValidateThis_();
	
	StPrintSession	session(sPrintRecordSpec);
	StDesktopDeactivator	deactivator;
	
	if (UPrinting::AskPageSetup(sPrintRecordSpec)) {
	
		// User okayed new Page Setup. Record settings in prefs file.
		
#if __PowerPlant__ >= 0x02112002
		// icw -- Handle ownership is different with Get/SetPrintRecord() in PP 2.1.1d2

		StHandleBlock thePrintRecordH(Handle_Nil);
		thePrintRecordH.Adopt(sPrintRecordSpec.GetPrintRecord());
		StNewResource pageSetup(PageSetup_ResType, PageSetup_ResID, ::GetHandleSize(thePrintRecordH));
		::BlockMoveData(*((Handle) thePrintRecordH), *((Handle) pageSetup), ::GetHandleSize(thePrintRecordH));
		pageSetup.Write();

#else

		Handle thePrintRecordH = (Handle) sPrintRecordSpec.GetPrintRecord();
		StNewResource pageSetup(PageSetup_ResType, PageSetup_ResID, ::GetHandleSize(thePrintRecordH));
		::BlockMoveData(*thePrintRecordH, *((Handle) pageSetup), ::GetHandleSize(thePrintRecordH));
		pageSetup.Write();

#endif // __PowerPlant__ >= 0x02112002

	}
}


// ---------------------------------------------------------------------------
//		* DoPrint												[protected]
// ---------------------------------------------------------------------------
//	Run the Print command.

void
CAPrintingAttachment::DoPrint()
{

	// Validate pointers.

	ValidateThis_();
	
	// Make sure we have a print record.
	StPrintSession	session(sPrintRecordSpec);
	
#if __PowerPlant__ >= 0x02112002
	// icw -- Handle ownership is different with Get/SetPrintRecord() in PP 2.1.1d2

	StHandleBlock	printRecH(Handle_Nil);
	printRecH.Adopt(sPrintRecordSpec.GetPrintRecord());
	if (not printRecH.IsValid())
	{
//		sPrintRecordSpec.SetToAppDefault();
#if PP_Target_Carbon
		sPrintRecordSpec.GetPrintSettings();	// make a default page settings record
#endif
		printRecH.Adopt(sPrintRecordSpec.GetPrintRecord());
	}
	
	if (not printRecH.IsValid()) {
		SignalCStr_("Couldn't create print record. Possibly no printer chosen.");
		return;
	}

#else

	if (sPrintRecordSpec.GetPrintRecord() == nil)
	{
#warning JWW - is this correct with new printing?
//		sPrintRecordSpec.SetToAppDefault();
#if PP_Target_Carbon
		sPrintRecordSpec.GetPrintSettings();	// make a default page settings record
#endif
	}
	
	if (sPrintRecordSpec.GetPrintRecord() == nil) {
		SignalCStr_("Couldn't create print record. Possibly no printer chosen.");
		return;
	}

#endif // __PowerPlant__ >= 0x02112002
	
	StDesktopDeactivator	deactivator;
	
	// Ask user for print settings.
	
	Boolean	printIt = UPrinting::AskPrintJob(sPrintRecordSpec);
	
	// If user okayed the job, print it now.
	
	if (printIt)
		DoPrintJob();
}


// ---------------------------------------------------------------------------
//		* DoPrintOne											[protected]
// ---------------------------------------------------------------------------
//	Run the Print One command.

void
CAPrintingAttachment::DoPrintOne()
{

	// Validate pointers.

	ValidateThis_();

	// Make sure we have a print record.
	StPrintSession	session(sPrintRecordSpec);
	
#if __PowerPlant__ >= 0x02112002
	// icw -- Handle ownership is different with Get/SetPrintRecord() in PP 2.1.1d2
	
	StHandleBlock	thePrintRecordH(Handle_Nil);
	thePrintRecordH.Adopt(sPrintRecordSpec.GetPrintRecord());
	if (not thePrintRecordH.IsValid())
	{
//		sPrintRecordSpec.SetToAppDefault();
#if PP_Target_Carbon
		sPrintRecordSpec.GetPrintSettings();	// make a default page settings record
#endif
		thePrintRecordH.Adopt(sPrintRecordSpec.GetPrintRecord());
	}
	
	if (not thePrintRecordH.IsValid()) {
		SignalCStr_("Couldn't create print record. Possibly no printer chosen.");
		return;
	}
	
	// Fill in default printout settings.

	sPrintRecordSpec.SetPrintJobParams(1, UPrinting::max_Pages, 1);
	
	// Print it now.
	
	DoPrintJob();

#else

	if (sPrintRecordSpec.GetPrintRecord() == nil)
	{
#warning JWW - is this correct with new carbon printing?
//		sPrintRecordSpec.SetToAppDefault();
#if PP_Target_Carbon
		sPrintRecordSpec.GetPrintSettings();	// make a default page settings record
#endif
	}
	
	if (sPrintRecordSpec.GetPrintRecord() == nil) {
		SignalCStr_("Couldn't create print record. Possibly no printer chosen.");
		return;
	}
	
	// Fill in default printout settings.
	
	THPrint thePrintRecordH = sPrintRecordSpec.GetPrintRecord();
	(**thePrintRecordH).prJob.iFstPage = 1;
	(**thePrintRecordH).prJob.iLstPage = UPrinting::max_Pages;
	(**thePrintRecordH).prJob.iCopies = 1;
	
	// Print it now.
	
	DoPrintJob();

#endif // __PowerPlant__ >= 0x02112002

}


// ---------------------------------------------------------------------------
//		* DoPrintJob											[protected]
// ---------------------------------------------------------------------------
//	Common code used by DoPrint and DoPrintOne methods.

void
CAPrintingAttachment::DoPrintJob()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find the view that will be installed in the printout.
	
	LWindow* ourWindow = (dynamic_cast<LWindow*>(mOwnerHost));
	ValidateObject_(ourWindow);
	
	LPane* printedPane = nil;
	if (mPrintedPaneID != 0)
		printedPane = ourWindow->FindPaneByID(mPrintedPaneID);
	
	LPane* printedTitleBar = nil;
	if (mPrintedTitleBarID != 0)
		printedTitleBar = ourWindow->FindPaneByID(mPrintedTitleBarID);

	LPrintout* thePrintout = nil;

	try {
	
		// Create the printout view.

		{
			StApplicationContext appContext;
			thePrintout = LPrintout::CreatePrintout(mPrintoutPPobID);
			ValidateObject_(thePrintout);
		}
		
		thePrintout->SetPrintSpec(sPrintRecordSpec);

		// Move the printed pane to the printout.

		LPlaceHolder* thePlaceHolder =
				(dynamic_cast<LPlaceHolder*>(thePrintout->FindPaneByID(mPrintoutPlaceholderID)));
		ValidateObject_(thePlaceHolder);
		thePlaceHolder->InstallOccupant(printedPane);
	
		// Move the title bar to the printout (if any).

		if (printedTitleBar != nil) {
			LPlaceHolder* theTBarHolder =
					(dynamic_cast<LPlaceHolder*>(thePrintout->FindPaneByID(mPrintoutTbarPlaceholderID)));
			ValidateObject_(theTBarHolder);
			theTBarHolder->InstallOccupant(printedTitleBar);
		}
	
		// Add document and window titles to printout header.
		
		LCaption* fDocumentName = (LCaption*) thePrintout->FindPaneByID(Pane_DocumentName);
		ValidateObject_(fDocumentName);
		
		LCaption* fResourceID = (LCaption*) thePrintout->FindPaneByID(Pane_ResourceID);
		ValidateObject_(fResourceID);

		LDocument* document = nil;
		LCommander* commander = ourWindow;
		
		while (commander != nil) {
//			ValidateObject_(commander);				// can't validate, since commander might be stack-based (application)
			document = (dynamic_cast<LDocument*>(commander));
			if (document != nil)
				break;
			commander = commander->GetSuperCommander();
		}

		LStr255 windowName;
		ourWindow->GetDescriptor(windowName);
		
		LStr255 docName;
		if (document != nil) {
			ValidateObject_(document);
			document->GetDescriptor(docName);
		}
		
		if (docName.Length() == 0)
			docName = windowName;
		
		if (windowName == docName)
			windowName[(UInt8)0] = 0;
		
		fDocumentName->SetDescriptor(docName);
		fResourceID->SetDescriptor(windowName);
		
		// Add date and time to printout header.
	
		LCaption* fDateField = (LCaption*) thePrintout->FindPaneByID(Pane_DateField);
		ValidateObject_(fDateField);

		LStr255 tempStr;
		LStr255 tempStr2;

		unsigned long dateTime;							// set date & time
		::GetDateTime(&dateTime);
		::DateString(dateTime, abbrevDate, tempStr, nil);
		{
			StApplicationContext appContext;
			tempStr += LStr255(STR_PrintoutStrings, str_DateTimeSeparator);
		}
		::TimeString(dateTime, false, tempStr2, nil);
		tempStr += tempStr2;
		
		fDateField->SetDescriptor(tempStr);
		
		// Print it.

#if PP_Target_Carbon
		StClipRgnState noFlash(nil);
#else
		StVisRgn noFlash(ourWindow->GetMacPort());
#endif
		thePrintout->DoPrintJob();
		delete thePrintout;
		
	}

	catch(...) {
		if (thePrintout)
			delete thePrintout;
	}

	// Make sure printed pane is redrawn properly.

	printedPane->Refresh();
	if (printedTitleBar != nil)
		printedTitleBar->Refresh();

}