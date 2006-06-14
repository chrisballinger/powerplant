// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CLWindow.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/26/96
//     $Date: 2006/01/18 01:32:29 $
//  $History: CLWindow.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 03/05/97   Time: 10:05
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Updated to new GA classes.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:04
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Added an #ifdef to support new GA classes. Remove "Class ID" caption if
//	class IDs aren't significant.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/20/97   Time: 13:24
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Improved commenting.
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/05/97    Time: 2:45p
//	Updated in $/ConstructorWin/Sources
//	Split initialization within case statement to make MSVC happy
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/04/97    Time: 2:48p
//	Updated in $/ConstructorWin/Sources
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/12/96   Time: 15:29
//	Updated in $/Constructor/Source files/Catalog
//	Fixed FinishCreateSelf so it doesn't detach a non-resource. (Bug fix
//	#1065.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/12/96   Time: 14:51
//	Updated in $/Constructor/Source files/Catalog
//	Fixed stray preferences resource. (Bug fix #1072.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/07/96   Time: 11:15
//	Updated in $/Constructor/Source files/Catalog
//	Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:56
//	Created in $/Constructor/Source files/Catalog
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CLWindow.h"

	// Core : Application
#include "CAPreferencesFile.h"

	// Core : Catalog
#include "CLDisplayClassTable.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Display classes (Robin) : Page controller
#include "LPageController.h"
#include "LIconTextTable.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Editors : Views : Data model
#include "VECatalogPage.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// PowerPlant : Utility classes
#include "UResourceMgr.h"
#include <LStaticText.h>

#define ALWAYS_USE_NEW_CATALOG 0		// 0 = use new scrolling catalog only on OS X, 1 = always

// ===========================================================================
//		* Class variables
// ===========================================================================

LWindow*	CLWindow::sCatalogWindow = nil;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_Catalog			= 20000;
const ResIDT	PPob_NewCatalog			= 20001;
const ResIDT	Txtr_IconLabel			= 20000;
const PaneIDT	Pane_PageController		= 'PCNT';
const PaneIDT	Pane_PageScroller		= 'SCRL';
const PaneIDT	Pane_DisplayClassTable	= 'CLDC';
const PaneIDT	Pane_AlphaHierToggle	= 'alph';

const ResType	CatPrefs_ResType		= 'CATL';
const ResIDT	CatPrefs_ResID			= 1000;


// ===========================================================================

#pragma mark *** CLWindow ***

// ---------------------------------------------------------------------------
//		* CLWindow()
// ---------------------------------------------------------------------------
//	Constructor

CLWindow::CLWindow(
	LStream*	inStream)

: GReferencedWindow(inStream)

{
	
	// Set the global singleton pointer.
	
	SignalIf_(sCatalogWindow != nil);
	sCatalogWindow = this;
	
	// Clear instance variables.
	
	mPageController = nil;
	mIconTextTable = nil;
	mClassTable = nil;
	
}


// ---------------------------------------------------------------------------
//		* ~CLWindow
// ---------------------------------------------------------------------------
//	Destructor

CLWindow::~CLWindow()
{

	// Reset the global singleton pointer.
	
	SignalIf_(sCatalogWindow != this);
	sCatalogWindow = nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** window creation

// ---------------------------------------------------------------------------
//		* OpenCatalogWindow										[static]
// ---------------------------------------------------------------------------
//	Create the catalog window and show it.

void
CLWindow::OpenCatalogWindow()
{

	// See if there is already a catalog window open.
	// If so, just bring it to the front.
	
	if (sCatalogWindow != nil) {
		ValidateObject_(sCatalogWindow);
		sCatalogWindow->Select();
		return;
	}
	
	// Ensure that classes are registered.
	
	RegisterCLClasses();

	// Okay, no such window. Build one.

	StApplicationContext appContext;
	LWindow* window;
	if (ALWAYS_USE_NEW_CATALOG || UEnvironment::GetOSVersion() >= 0x1000)
		window = LWindow::CreateWindow(PPob_NewCatalog, LCommander::GetTopCommander());
	else
		window = LWindow::CreateWindow(PPob_Catalog, LCommander::GetTopCommander());
	ValidateObject_(window);
	
	// Position & size the window.

	window->DoSetZoom(true);
	window->ProcessCommand(cmd_PositionWindow);
	
	// Before we show the window we need to synchronize the currently
	// selected button in the page controller so that it is revealed
	// if needed.

	if (ALWAYS_USE_NEW_CATALOG || UEnvironment::GetOSVersion() >= 0x1000)
	{
		LIconTextTable* theTable = dynamic_cast<LIconTextTable*>(window->FindPaneByID(Pane_PageController));
		ValidateObject_(theTable);
		if (theTable)
		{
			LScrollerView* theScroller = dynamic_cast<LScrollerView*>(window->FindPaneByID(Pane_PageScroller));
			ValidateObject_(theScroller);
			if (theScroller)
				theTable->SetScroller(theScroller);
			
			theTable->RevealSelectedIcon();
			
			// JWW - Must set the controller message after the controller is created so that
			// the initial population of the catalog window doesn't cause the preferences to
			// be written out and change the starting saved pane panel
			theTable->SetControllerMessage(Pane_PageController);
		}
	}
	else
	{
		LPageController* theController = dynamic_cast<LPageController*>(window->FindPaneByID(Pane_PageController));
		ValidateObject_(theController);
		if (theController)
			theController->RevealSelectedPageButton();
	}

	// Now show the window.
	
	window->Show();

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* RegisterCLClasses								[static, protected]
// ---------------------------------------------------------------------------
//	Register all the display classes that are used by the catalog.

void
CLWindow::RegisterCLClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register catalog display classes.
	
	URegistrar::RegisterClass(CLWindow::class_ID, (ClassCreatorFunc) CLWindow::CreateFromStream);
	URegistrar::RegisterClass(CLDisplayClassTable::class_ID, (ClassCreatorFunc) CLDisplayClassTable::CreateFromStream);

}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	The catalog window has been built. Hook up the page controller and
//	display class table to the view editor's data model.

void
CLWindow::FinishCreateSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Find the global type list.
	
	mTypeList = VETypeList::GetTypeList();
	
	// Create a selection.
	
	mSelection = new DMSelection;
	ValidateObject_(mSelection.GetObject());
	mSelection->SetRootElement(mTypeList);

	// Find the page controller.
	
	if (ALWAYS_USE_NEW_CATALOG || UEnvironment::GetOSVersion() >= 0x1000)
	{
		mIconTextTable = (dynamic_cast<LIconTextTable*>(FindPaneByID(Pane_PageController)));
		ValidateObject_(mIconTextTable);
		
		mIconTextTable->AddListener(this);
	}
	else
	{
		mPageController = (dynamic_cast<LPageController*>(FindPaneByID(Pane_PageController)));
		ValidateObject_(mPageController);
		
		mPageController->AddListener(this);
	}

	// Find the class table.
	
	mClassTable = (dynamic_cast<CLDisplayClassTable*>(FindPaneByID(Pane_DisplayClassTable)));
	ValidateObject_(mClassTable);
	
	mClassTable->SetSelection(mSelection);

	// Find the alpha/hierarchical toggle.
	
	mAlphaToggle = (dynamic_cast<LControl*>(FindPaneByID(Pane_AlphaHierToggle)));
	ValidateObject_(mAlphaToggle);
	mAlphaToggle->AddListener(this);

	// Set it up with initial pages.
	// In a later version, we'll want to set up a listener relationship with 
	// the type list so we can catch changes to the page list. This won't
	// happen in CW11.
	
	if (ALWAYS_USE_NEW_CATALOG || UEnvironment::GetOSVersion() >= 0x1000)
		ConfigureNewPageTitles();
	else
		ConfigurePageTitles();

	//If class IDS aren't significant in the table, hide the
	//classID header
	//CCP 2/26/97
	
	if (!mTypeList->GetClassIDsSignificant())
	{
		LStaticText *header = dynamic_cast<LStaticText*>(FindPaneByID('ID  '));
		ValidateObject_(header);
		header->Hide();
	}


	// See if we have catalog prefs resource.
	
	StPreferencesContext prefsContext;
	
	Handle catlPrefsR = nil;
	if (prefsContext.IsValid())											//* 2.4a2: BUG FIX #1072: added IsValid()
		catlPrefsR = ::Get1Resource(CatPrefs_ResType, CatPrefs_ResID);	//* 2.4a2: BUG FIX #1065: stopped using StResource
	
	if (catlPrefsR == nil) {
	
		// No prefs resource. Just select first page.
	
		InstallCatalogPage(1);

	}
	else {
		
		// Read the preferences data.
		
		ValidateHandle_(catlPrefsR);
		::DetachResource(catlPrefsR);
		
		LHandleStream catlPrefs(catlPrefsR);
		
		Boolean alphaSort;
		Str255 pageName;
		
		catlPrefs >> alphaSort >> pageName;
		
		// Try to find a page with the same name.
		// 96.12.05	rtm		Changed way the initial page is setup which eliminated
		// 					the while loop that was original here, the main difference
		//					being that we figure the page index and then set the button
		//					using that index
		
		if (ALWAYS_USE_NEW_CATALOG || UEnvironment::GetOSVersion() >= 0x1000)
		{
			LIconTextItem theItem;
			bool isFound = false;
			for (SInt16 i = 0; (i < mIconTextTable->GetIconCount()) && (!isFound); i++)
			{
				mIconTextTable->GetIconAt(i + 1, &theItem);
				if (::EqualString(theItem.theIconLabel, pageName, true, true))
				{
					mIconTextTable->SetSelectedIconByIndex(i + 1);
					InstallCatalogPage(i + 1);
					isFound = true;
				}
			}
			
			if (!isFound)
			{
				InstallCatalogPage(1);
			}
		}
		else
		{
			ArrayIndexT buttonIndex = mPageController->GetPageIndex(pageName);
			if (buttonIndex > 0) {
				mPageController->SetSelectedPageButtonByIndex(buttonIndex);
				InstallCatalogPage(buttonIndex);
			}
			else {
				InstallCatalogPage(1);
			}
		}

		if (alphaSort) {
			mAlphaToggle->SetValue(1);
			mClassTable->SetAlphabeticDisplay(true);
		}
		
	}

	// Listen to the grow zone.
	
	LGrowZone* growZone = LGrowZone::GetGrowZone();
	ThrowIfNil_(growZone);
	
	growZone->AddListener(this);

}


// ---------------------------------------------------------------------------
//		* ConfigureNewPageTitles								[protected]
// ---------------------------------------------------------------------------
//	Ask the view editor's type list for a list of catalog pages. Set up
//	the icon scroller to match these pages.

void
CLWindow::ConfigureNewPageTitles()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mIconTextTable);
	ValidateObject_(mTypeList.GetObject());

	// Clear any existing page titles.

	while (mIconTextTable->GetIconCount() > 0) {
		mIconTextTable->RemoveIconAt(1);
	}

	// Read through type list's list of catalog pages.
	
	DMFastIterator iter(mTypeList->GetCatalogPages()->GetSubElements());
	VECatalogPage* page;
	
	while (iter.Next(&page)) {
		
		ValidateObject_(page);
		
		LStr255 pageTitle;
		page->GetPageTitle(pageTitle);
		
		LIconTextItem theItem;
		theItem.theIconID = page->GetPageIconID();
		LString::CopyPStr(pageTitle, theItem.theIconLabel);
		theItem.theTextTraitsID = Txtr_IconLabel;
		mIconTextTable->AddIcon(&theItem);

	}
	
	// Choose the page that was most recently used.
	
	mIconTextTable->SetSelectedIconByIndex(1);
	
}


// ---------------------------------------------------------------------------
//		* ConfigurePageTitles									[protected]
// ---------------------------------------------------------------------------
//	Ask the view editor's type list for a list of catalog pages. Set up
//	the page controller to match these pages.

void
CLWindow::ConfigurePageTitles()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mPageController);
	ValidateObject_(mTypeList.GetObject());

	// Clear any existing page titles.

	while (mPageController->GetPageButtonCount() > 0) {
		mPageController->RemovePageButtonAt(1);
	}

	// Read through type list's list of catalog pages.
	
	DMFastIterator iter(mTypeList->GetCatalogPages()->GetSubElements());
	VECatalogPage* page;
	
	while (iter.Next(&page)) {
		
		ValidateObject_(page);
		
		LStr255 pageTitle;
		page->GetPageTitle(pageTitle);
		
		Str255 pageTitleStr;
		LString::CopyPStr(pageTitle, pageTitleStr);
		mPageController->AddPageButton(pageTitleStr, false); //new GA classes

	}
	
	// Choose the page that was most recently used.
	
	mPageController->SetSelectedPageButtonByIndex(1);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Catch messages from the grow zone, page controller, and alpha/hierarchy
//	toggle button.

void
CLWindow::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam) 
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Dispatch message.
	
	switch (inMessage) {

		// Grow zone: If out of memory, the catalog is expendable.
		
		case msg_GrowZone:
			DoClose();
			break;

		// Page controller: Change pages.
		
		case Pane_PageController:
			SInt16 pageNumber;
			pageNumber = *(SInt32*) ioParam;
			InstallCatalogPage(pageNumber);
			RecordCatalogPrefs();
			break;

		// Alpha/hier toggle: Change display.
		
		case Pane_AlphaHierToggle:
			SInt32 value = *(SInt32*) ioParam;
			ValidateObject_(mClassTable);
			mClassTable->SetAlphabeticDisplay(value);
			RecordCatalogPrefs();
			break;

	}
}

// ---------------------------------------------------------------------------
//		* InstallCatalogPage									[protected]
// ---------------------------------------------------------------------------
//	Install a catalog page by index.

void
CLWindow::InstallCatalogPage(
	SInt16	inIndex)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mClassTable);

	// Tell the outline table what catalog page to display.
	
	DMContainerAttribute* pages = mTypeList->GetCatalogPages();
	ValidateObject_(pages);
	
	VECatalogPage* page = nil;
	if (pages->GetSubElements().FetchItemAt(inIndex, &page)) {
		ValidateObject_(page);
		mClassTable->SetElement(page);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** window settings

// ---------------------------------------------------------------------------
//		* RecordCatalogPrefs									[protected]
// ---------------------------------------------------------------------------
//	Remember which catalog page was displayed and what the sort-by setting
//	is.

void
CLWindow::RecordCatalogPrefs()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mClassTable);
	if (ALWAYS_USE_NEW_CATALOG || UEnvironment::GetOSVersion() >= 0x1000)
		ValidateObject_(mIconTextTable);
	else
		ValidateObject_(mPageController);
	
	// Create preferences resource data.
	
	LHandleStream catlPrefs;
	
	Boolean alphaSort = mClassTable->GetAlphabeticDisplay();
	catlPrefs << alphaSort;
	
	Str255 pageName;
	if (ALWAYS_USE_NEW_CATALOG || UEnvironment::GetOSVersion() >= 0x1000)
	{
		LIconTextItem theItem;
		mIconTextTable->GetIconAt(mIconTextTable->GetValue() + 1, &theItem);
		LString::CopyPStr(theItem.theIconLabel, pageName);
	}
	else
	{
		LPageButton* button = mPageController->GetSelectedPageButton();
		if (button != nil) {
			ValidateObject_(button);
			button->GetDescriptor(pageName);
		}
	}
	
	catlPrefs << pageName;
	
	// Write this data to prefs file.
	
	StPreferencesContext prefsContext;
	if (prefsContext.IsValid()) {					//* 2.4a2: BUG FIX #1072: added if statement
	
		StNewResource catlPrefsR(CatPrefs_ResType, CatPrefs_ResID, 0, true);
		ValidateHandle_(catlPrefsR);
	
		::SetHandleSize(catlPrefsR, catlPrefs.GetLength());
		::BlockMoveData(*(catlPrefs.GetDataHandle()),
						*((Handle) catlPrefsR),
						catlPrefs.GetLength());
	
		catlPrefsR.Write();

	}
}