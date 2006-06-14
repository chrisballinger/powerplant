// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLEditor.cpp				© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================


// ===========================================================================

#include "TCLEditor.h"
#include "DMListAttribute.h"
#include "DMReanimator.h"
#include "LWindowPositioner.h"
#include "RESession.h"
#include "TCLTabControlListTable.h"
#include "RFResource.h"
#include "TCLModelTabControl.h"
#include "TCLModelTabInfo.h"
#include "DMIterator.h"

#include <UResourceMgr.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	DSPC_TabControlData	= 5276;
const ResIDT	DSPC_TabInfoData	= 5277;

const ResIDT	PPob_TabEditor		= 5275;
const PaneIDT	Pane_HierarchyView	= 'TCLT';


// ===========================================================================

#pragma mark *** TCLEditor ***

// ---------------------------------------------------------------------------
//		* TCLEditor()
// ---------------------------------------------------------------------------
//	Constructor

TCLEditor::TCLEditor(
	LCommander*		inSuper,
	RESession&		inSession)

	: REEditor(inSuper, inSession)
{
	RegisterTCLClasses();
}


// ---------------------------------------------------------------------------
//		* ~TCLEditor
// ---------------------------------------------------------------------------
//	Destructor

TCLEditor::~TCLEditor()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** editor override hooks

// ---------------------------------------------------------------------------
//		* ReadResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read the tab# resource.

void
TCLEditor::ReadResourceDataSelf()
{
	// Validate pointers.
	ValidateThis_();
	ValidateSimpleObject_(mPrimaryResource);

	// Build input stream for the resource.
	Handle tabData = mPrimaryResource->GetResData();
	ValidateHandle_(tabData);

	LHandleStream tabStream(tabData);

	// Build a menu header item.
	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->ElementsFromResource('DSPC', DSPC_TabControlData, mDataModelRoot);
	ValidateObject_(mDataModelRoot.GetObject());

	TCLModelTabControl* tabControl = dynamic_cast<TCLModelTabControl*>(mDataModelRoot.GetObject());
	ValidateObject_(tabControl);

	DMListAttribute* tabInfoList = tabControl->GetTabInfo();
	ValidateObject_(tabInfoList);
	
	SInt16	version;
	tabStream >> version;
	tabControl->SetVersion(version);
	SignalIf_(version != 0);
	
	SInt16	tabInfoCount;
	tabStream >> tabInfoCount;
	
	while (tabInfoCount-- > 0)
	{
		// Create a data model representation for the next menu item.
		
		DMElementPtr tabInfoObject = nil;

		reanimator->ElementsFromResource('DSPC', DSPC_TabInfoData, tabInfoObject);
		ValidateObject_(tabInfoObject.GetObject());
		
		TCLModelTabInfo* tabInfo = dynamic_cast<TCLModelTabInfo*>(tabInfoObject.GetObject());
		ValidateObject_(tabInfo);
		
		SInt16	iconSuiteID;
		Str255	text;
		SInt16	fillWord;
		SInt32	fillLong;
		
		tabStream >> iconSuiteID;
		tabInfo->SetIconSuiteID(iconSuiteID);
		
		tabStream >> text;
		tabInfo->SetTabName(text);
		
		tabStream >> fillWord;
		tabStream >> fillLong;

		tabInfoList->InsertElementAt(LArray::index_Last, tabInfoObject, true);
	}

	mSession.GetSelectionForEditor(this, mSelection);
	ValidateObject_(mSelection.GetObject());

	if (mSelection->GetRootElement() == nil)
		mSelection->SetRootElement(mDataModelRoot);
}


// ---------------------------------------------------------------------------
//		* WriteResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Write the 'tab#' resource format.

void
TCLEditor::WriteResourceDataSelf()
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());
	ValidateSimpleObject_(mPrimaryResource);

	// Build output stream for the resource.
	LHandleStream tabStream;

	TCLModelTabControl* tabControl = dynamic_cast<TCLModelTabControl*>(mDataModelRoot.GetObject());
	ValidateObject_(tabControl);

	DMListAttribute* tabInfoList = tabControl->GetTabInfo();
	ValidateObject_(tabInfoList);

	// Write out the version
	SInt16	version = tabControl->GetVersion();
	tabStream << version;

	SInt16	tabInfoCount;
	LSharableArray subElements = tabInfoList->GetSubElements();
	tabInfoCount = subElements.GetCount();
	tabStream << tabInfoCount;

	DMFastIterator iter(tabInfoList->GetSubElements());
	while (iter.NextElement())
	{
		TCLModelTabInfo* tabInfoItem = dynamic_cast<TCLModelTabInfo*>(iter.CurrentElement());
		ValidateObject_(tabInfoItem);
		
		SInt16	iconSuiteID;
		LStr255 text;
		SInt16	fillWord = 0;
		SInt32	fillLong = 0;
		
		iconSuiteID = tabInfoItem->GetIconSuiteID();
		tabStream << iconSuiteID;
		
		tabInfoItem->GetTabName(text);
		tabStream << (ConstStringPtr) text;
		
		tabStream << fillWord;
		tabStream << fillLong;
	}
	
	// Write the 'tab#' resource data.
	mPrimaryResource->SetResData(tabStream.GetDataHandle());
}


// ---------------------------------------------------------------------------
//		* OpenEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Open the tab control list editor window.

LWindow*
TCLEditor::OpenEditorWindowSelf()
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());

	// Create the window.
	LWindow* window = nil;
	{
		StApplicationContext appContext;
		window = LWindow::CreateWindow(PPob_TabEditor, this);
		ValidateObject_(window);
	}

	// Set the window title to the resource name.
	LStr255 title;
	mSession.GetWindowTitleForEditor(this, title);
	window->SetDescriptor(title);

	// Attach hierarchy view.
	TCLTabControlListTable* outlineTable = dynamic_cast<TCLTabControlListTable*>
		(window->FindPaneByID(Pane_HierarchyView));
	ValidateObject_(outlineTable);
	outlineTable->SetSelection(mSelection);
	outlineTable->SetElement(mDataModelRoot);
	
	// Position & show the window.
	window->DoSetZoom(true);
	window->ProcessCommand(cmd_PositionWindow);
	window->Show();

	return window;
}


// ===========================================================================

#pragma mark -
#pragma mark ** editor override hooks

// ---------------------------------------------------------------------------
//		* RegisterTCLClasses							[static, protected]
// ---------------------------------------------------------------------------
//	Register the display classes used by the tab control list editor.

void
TCLEditor::RegisterTCLClasses()
{
	// Do this only once.
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register tab control list view display classes.
	RegisterClass_(TCLTabControlListTable);

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->RegisterClass(TCLModelTabControl::class_ID, TCLModelTabControl::CreateFromStream);
	reanimator->RegisterClass(TCLModelTabInfo::class_ID, TCLModelTabInfo::CreateFromStream);
}
