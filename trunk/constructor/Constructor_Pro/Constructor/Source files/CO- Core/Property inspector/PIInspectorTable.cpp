// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIInspectorTable.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/18/96
//     $Date: 2006/01/18 01:33:22 $
//	$History: PIInspectorTable.cpp $
//	
//	*****************  Version 23  *****************
//	User: scouten      QDate: 03/05/97   Time: 10:40
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Removed #include "PIResourceRefItem.h" for Win32 target.
//	
//	*****************  Version 22  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:49
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for hex string attribute.
//	
//	*****************  Version 21  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 20  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for multiple property inspectors. (Suggestion #1048.)
//	
//	*****************  Version 19  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 18  *****************
//	User: scouten      QDate: 02/01/97   Time: 14:59
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Isolated access to PIResourceRefItem behind Constructor_UseRF macro.
//	
//	*****************  Version 17  *****************
//	User: scouten      QDate: 01/31/97   Time: 14:35
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added a missing #include.
//	
//	*****************  Version 16  *****************
//	User: scouten      QDate: 01/31/97   Time: 14:25
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added PIResourceRefItem.
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 01/30/97   Time: 18:38
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed FindCommandStatus and ObeyCommand so they pass along the printing
//	commands. (Bug fix #1259.)
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 01/21/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed property inspector so it responds to Command-/ (Zoom Window).
//	(Bug fix #1174.)
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:49
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed SetElementFromSelection so that it can safely ignore elements
//	that are not DMObject.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 12/04/96   Time: 20:57
//	Updated in $/Constructor/Source files/Property inspector
//	Changed FindCommandStatus and ObeyCommand so they enable Save command
//	while PI is active window.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 12/04/96   Time: 18:38
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed a bug in SetElementFromSelection that prevented PI from clearing
//	table when a window went away.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:48
//	Updated in $/Constructor/Source files/Property inspector
//	Added ListenToMessage method. Now closes PI window
//	//	if memory is low.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/26/96   Time: 17:59
//	Updated in $/Constructor/Source files/Property inspector
//	Changed resource ID allocation to avoid conflict with Display classes
//	(Eric) folder.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/26/96   Time: 09:42
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed a commenting error.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/25/96   Time: 19:06
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for inspected object label. Changed behavior when the
//	selection is empty: it now leaves the previously-inspected object in
//	the table.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/04/96   Time: 13:55
//	Updated in $/Constructor/Source files/Property inspector
//	Added menu strings for dynamic list items. Updated to use new
//	OVTable::ScrollElementIntoView method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:35
//	Updated in $/Constructor/Source files/Property inspector
//	Updated for new undo/redo menu string architecture.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/02/96   Time: 15:10
//	Updated in $/Constructor/Source files/Property inspector
//	Added cmd_NewItemDialog menu command.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:36
//	Updated in $/Constructor/Source files/Property inspector
//	Bug fixes and standard sizing.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:08
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIInspectorTable.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Attribute classes
#include "DMContainerAttribute.h"
#include "DMFontStringAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMLabelledIntegerAttribute.h"
#include "DMListAttribute.h"
#if Constructor_UseRF
	#include "DMResourceRefAttribute.h"
#endif
#include "DMRGBColorAttribute.h"
#include "DMStringAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Property inspector
#include "PIAttributeItem.h"
#include "PIBooleanItem.h"
#include "PIColorItem.h"
#include "PIContainerItem.h"
#include "PIFontItem.h"
#include "PILabelledItem.h"
#include "PIStringItem.h"
#include "PIHexStringItem.h"
#if Constructor_UseRF
	#include "PIResourceRefItem.h"
#endif
#include "UPropertyInspector.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : Table classes
#include <UTableHelpers.h>

	// PowerPlant : CA : Outline table
#include <LOutlineRowSelector.h>

	// PowerPlant : CA : Table classes
#include <LTablePaneHostingGeometry.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>
#include <LStaticText.h>
#include <LBevelButton.h>

// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_PropertyInspectorMenu	= 21000;
const SInt16	str_NewSpace				= 1;
const SInt16	str_ClearSpace				= 2;
const SInt16	str_DuplicateSpace			= 3;

const PaneIDT	Pane_InspectedObject		= 'IOBJ';


// ===========================================================================

#pragma mark *** PIInspectorTable ***

// ---------------------------------------------------------------------------
//		* PIInspectorTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Takes no additional stream data.

PIInspectorTable::PIInspectorTable(
	LStream*	inStream)

: OVTable(inStream, 15, true)

{

	// Establish item factory.

	SetItemFactory(new PIItemFactory);
	
	// Reconfigure table geometry and selection.
	
	delete mTableGeometry;
	SetTableGeometry(new LTablePaneHostingGeometry(this, 80, 15));
	SetTableSelector(new LOutlineRowSelector(this));
	
	// Reset state variables.
	
	mSelectionChanged = false;
	mFollowUndoHost = nil;
	mInspectedObjectCaption = nil;
	mDetachPIButton = nil;
	mIsDetached = false;

}


// ---------------------------------------------------------------------------
//		* ~PIInspectorTable
// ---------------------------------------------------------------------------
//	Destructor

PIInspectorTable::~PIInspectorTable()
{

	// Validate pointers.

	ValidateThis_();
	
	// Tell the property inspector manager that this window is going away.

	LWindow* myWindow = LWindow::FetchWindowObject(GetWindowFromPort(GetMacPort()));
	ValidateObject_(myWindow);
	UPropertyInspector::ClosingInspectorWindow(myWindow);

}


// ===========================================================================

#pragma mark -
#pragma mark ** menu command status

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Enable property inspector menu commands.

void
PIInspectorTable::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{

	// Validate pointers.

	ValidateThis_();
	
	// Dispatch command.

	switch (inCommand) {
	
		// Property inspector: Enable and checkmark.

		case cmd_PropertyInspector: {
			outEnabled = true;
			outUsesMark = true;
			outMark = checkMark;
			break;
		}
		
		// New dynamic item: Enable iff a dynamic list item is selected.
	
		case cmd_NewItem:
		case cmd_NewItemDialog: {
			DMListAttribute* attr;
			UInt32 after;

			if (GetSelectedListItem(attr, after)) {
				ValidateObject_(attr);
				outEnabled = attr->IsEnabled();
				LStr255 name(STR_PropertyInspectorMenu, str_NewSpace);
				LStr255 itemType;
				attr->GetItemTitle(itemType);
				name += itemType;
				LString::CopyPStr(name, outName);
			}
			break;
		}
		
		// Clear dynamic item: Enable iff a dynamic list item is selected.
	
		case cmd_Clear: {
			DMListAttribute* attr;
			UInt32 after;

			if (GetSelectedListItem(attr, after) && (after != 0)) {
				ValidateObject_(attr);
				outEnabled = attr->IsEnabled();
				LStr255 name(STR_PropertyInspectorMenu, str_ClearSpace);
				LStr255 itemType;
				attr->GetItemTitle(itemType);
				name += itemType;
				LString::CopyPStr(name, outName);
			}
			break;
		}
			
		// Duplicate dynamic item: Enable iff a dynamic list item is selected.

		case cmd_Duplicate: {
			DMListAttribute* attr;
			UInt32 after;
			if (GetSelectedListItem(attr, after) && (after != 0)) {
				ValidateObject_(attr);
				outEnabled = attr->IsEnabled();
				LStr255 name(STR_PropertyInspectorMenu, str_DuplicateSpace);
				LStr255 itemType;
				attr->GetItemTitle(itemType);
				name += itemType;
				LString::CopyPStr(name, outName);
			}
			break;
		}
		
		// Paste, Duplicate, Close, Zoom: Let supercommander handle it.

		case cmd_Copy:
		case cmd_Cut:		
		case cmd_Paste:
		case cmd_Close:
		case cmd_PageSetup:
		case cmd_Print:
		case cmd_PrintOne:
		case cmd_ZoomWindow:
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
			break;
		
		// Undo/Save/most others: Let the window that contains the inspected object handle it.
		
		case cmd_Save:
		case cmd_Undo:
		default:
			if (mFollowUndoHost != nil) {
				ValidateObject_(mFollowUndoHost);
				mFollowUndoHost->ProcessCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
			}
			else
				OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
			break;
		
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Respond to property inspector menu commands.

Boolean
PIInspectorTable::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();
	
	// Dispatch command.
	
	switch (inCommand) {
	
		// New dynamic list item.
	
		case cmd_NewItem:
		case cmd_NewItemDialog:
			NewDynamicItem();
			return true;

		// Clear dynamic list item.
		
		case cmd_Clear:
			ClearDynamicItem();
			return true;
			
		// Duplicate dynamic list item.
		
		case cmd_Duplicate:
			DuplicateDynamicItem();
			return true;
		
		// Paste, Duplicate, Close, Zoom: Let supercommander handle it.

		case cmd_Copy:
		case cmd_Cut:		
		case cmd_Paste:
		case cmd_Close:
		case cmd_PageSetup:
		case cmd_Print:
		case cmd_PrintOne:
		case cmd_ZoomWindow:
			return OVTable::ObeyCommand(inCommand, ioParam);

		// Undo and post action: Let window that hosts the inspected object handle it.
			
		case cmd_Save:
		case cmd_Undo:
		case msg_PostAction:
		default:
			if (mFollowUndoHost != nil) {
				ValidateObject_(mFollowUndoHost);
				mFollowUndoHost->ProcessCommand(inCommand, ioParam);
				return true;
			}
			else
				return OVTable::ObeyCommand(inCommand, ioParam);
	
	}
}


// ---------------------------------------------------------------------------
//		* HandleKeyPress
// ---------------------------------------------------------------------------
//	Overriden to make backspace/delete an alias for Clear <dynamic item>.

Boolean
PIInspectorTable::HandleKeyPress(
	const EventRecord&	inKeyEvent)
{

	// Validate pointers.

	ValidateThis_();
	
	// If a dynamic list item is selected, try deleting it.
	
	SInt16 theKey = inKeyEvent.message & charCodeMask;
	if (theKey == char_Backspace) {
		ClearDynamicItem();
		return true;
	}
	else
		return OVTable::HandleKeyPress(inKeyEvent);

}


// ===========================================================================

#pragma mark -
#pragma mark ** undo/redo helpers

// ---------------------------------------------------------------------------
//		* PostAction
// ---------------------------------------------------------------------------
//	Overriden to route undoable actions to the inspected window, not our
//	supercommander. The property inspector window typically does not have
//	an undoer attachment.

void
PIInspectorTable::PostAction(
	LAction*	inAction)
{

	// Validate pointers.

	ValidateThis_();
	if (inAction != nil)
		ValidateObject_(inAction);
		
	// If there is a window that's displaying the inspected object,
	// let it keep track of the undo/redo chain.

	if (mFollowUndoHost != nil) {
		ValidateObject_(mFollowUndoHost);
		mFollowUndoHost->PostAction(inAction);			//* J-1.0a4: BUG FIX #922: route actions to data model host
	}
	else
		LCommander::PostAction(inAction);

}


// ===========================================================================

#pragma mark -
#pragma mark ** custom resizing behaviors

// ---------------------------------------------------------------------------
//		* CalcStandardSize
// ---------------------------------------------------------------------------
//	Set default size for table to 280 pixels.

void
PIInspectorTable::CalcStandardSize(
	GReferencedWindow*	inWindow)
{

	// Validate pointers.

	ValidateThis_();

	// Find our image size.

	SDimension32 hierImageSize;
	GetImageSize(hierImageSize);
	hierImageSize.width = 280;

	// Calculate overscan (i.e. how far are we inside the window).
	
	SDimension16 overscanSize;
	SDimension16 windowFrameSize;
	inWindow->GetFrameSize(windowFrameSize);
	overscanSize.width = windowFrameSize.width - mFrameSize.width;
	overscanSize.height = windowFrameSize.height - mFrameSize.height;

	// Set window's size.

	SDimension16 windStandardSize;
	windStandardSize.width = ((hierImageSize.width) > 16000
								? 16000
								: hierImageSize.width) + overscanSize.width;
	windStandardSize.height = ((hierImageSize.height) > 16000
								? 16000
								: hierImageSize.height) + overscanSize.height;

	inWindow->SetStandardSize(windStandardSize);
	
}


// ---------------------------------------------------------------------------
//		* ResizeFrameBy
// ---------------------------------------------------------------------------
//	Overriden to stretch the value column to fill the table.

void
PIInspectorTable::ResizeFrameBy(
	SInt16		inWidthDelta,
	SInt16		inHeightDelta,
	Boolean		inRefresh)
{

	// Validate pointers.

	ValidateThis_();

	// Resize the view.

	LView::ResizeFrameBy(inWidthDelta, inHeightDelta, inRefresh);
	
	// Stretch the value column to fill space.
	
	if (inWidthDelta != 0)
		SetColWidth(GetColWidth(2) + inWidthDelta, 2, 2);

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElement											[protected]
// ---------------------------------------------------------------------------
//	Unlike most other outline tables, the property inspector allows the
//	root element to be changed at will. If we are being assigned a new
//	element, empty the table and fill it again with attributes from the
//	new object.
//
//	Typically called by UPropertyInspector::InspectSelection.

void
PIInspectorTable::SetElement(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	if (inElement != nil)
		ValidateObject_(inElement);

	// See if the element has changed. If not, ignore request.
	
	if (inElement == mElement)
		return;

	// Make sure screen doesn't flash while changing elements.
	
#if PP_Target_Carbon
	StClipRgnState noFlash(nil);
#else
	StVisRgn noFlash(GetMacPort());
#endif
	Refresh();

	// Override to allow changing elements. If we are assigned
	// to an element already, empty the table.

	if (mElement != nil) {
	
		ValidateObject_(mElement.GetObject());

		while (mRows > 0) {
			LOutlineItem* item = FindItemForRow(1);
			ValidateObject_(item);
			RemoveItem(item, false, false);
		}
		
		AdjustImageSize(false);
		Refresh();
		
		mElement->RemoveListener(this);
		mElement = nil;
		mContainer = nil;
		
		// See if there's an inspected object title caption.
		// If so, clear its title.
		
		if (mInspectedObjectCaption != nil) {
			ValidateObject_(mInspectedObjectCaption);
			mInspectedObjectCaption->SetDescriptor( (StringPtr)"");
			mInspectedObjectCaption->Refresh();
		}
	}

	// Now call the inherited method for set element only
	// if the new element is non-nil.

	if (inElement != nil)
		OVTable::SetElement(inElement);	

}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this is a data model object.

void
PIInspectorTable::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have an object.

	DMObject* object = (dynamic_cast<DMObject*>(inElement));
	ValidateObject_(object);
	
	// Okay, it's an object. Create type views for all of its attributes.

	SetContainer(object);
	
	// See if there's an inspected object caption. If so, change its name.
	
	if (mInspectedObjectCaption != nil) {
	
		ValidateObject_(mInspectedObjectCaption);
		
		LStr255 displayableName;
		inElement->GetDisplayableName(displayableName);
		mInspectedObjectCaption->SetDescriptor(displayableName);
		mInspectedObjectCaption->Refresh();
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Overriden to draw the notebook line under the last item in the table.

void
PIInspectorTable::DrawSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Draw table.

	OVTable::DrawSelf();
	
	// Draw the notebook line under the last item.
	
	RGBColor theColor = UGAColorRamp::GetColor(2);
	::RGBForeColor(&theColor);
	::MoveTo(1, mImageSize.height);
	::LineTo(mImageSize.width - 2, mImageSize.height);

}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Set up the default size of the tables.

void
PIInspectorTable::FinishCreateSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Do inherited initialization.

	OVTable::FinishCreateSelf();
	
	// Set up default column widths.
	
	InsertCols(2, 0, nil, 0, false);
	SetColWidth(130, 1, 1);
	SetColWidth(150, 2, 2);
	
	// Find inspected object caption.
	
	LWindow* window = LWindow::FetchWindowObject(GetWindowFromPort(GetMacPort()));
	ValidateObject_(window);
	
	mInspectedObjectCaption = dynamic_cast<LStaticText*>(window->FindPaneByID(Pane_InspectedObject));
	if (mInspectedObjectCaption != nil)
		ValidateObject_(mInspectedObjectCaption);
	
	// Find detach selection button.
	
	mDetachPIButton = (dynamic_cast<LBevelButton*>(window->FindPaneByID('DTCH')));
	if (mDetachPIButton != nil) {
		ValidateObject_(mDetachPIButton);
		mDetachPIButton->AddListener(this);
	}

	// Listen to the grow zone.
	
	LGrowZone* growZone = LGrowZone::GetGrowZone();
	ThrowIfNil_(growZone);
	
	growZone->AddListener(this);

}


// ===========================================================================

#pragma mark -
#pragma mark ** dynamic items

// ---------------------------------------------------------------------------
//		* GetSelectedListItem									[protected]
// ---------------------------------------------------------------------------
//	See if there is a dynamic-list item currently selected. Typically called
//	from FindCommandStatus, ClearDynamicItem, or DuplicateDynamicItem.

Boolean
PIInspectorTable::GetSelectedListItem(
	DMListAttribute*&	outListAttr,
	UInt32&				outInsertAfter)
{

	// Validate pointers.

	ValidateThis_();

	// Find out what's selected. If no cells selected, or multiple rows
	// selected, bail out.
	
	STableCell cell;
	if (!GetNextSelectedCell(cell))
		return false;
	
	STableCell cell2 = cell;
	while (GetNextSelectedCell(cell2)) {
		if (cell2.row != cell.row)
			return false;
	}
	
	// Okay, grab the element at this index.
	
	OVItem* item = (dynamic_cast<OVItem*>(FindItemForRow(cell.row)));
	if (item == nil)
		return false;
	ValidateObject_(item);
	
	DMElement* element = item->GetElement();
	if (element == nil)
		return false;
	ValidateObject_(element);
	
	// First check: See if this is the container itself.
	
	DMListAttribute* attr = (dynamic_cast<DMListAttribute*>(element));
	if (attr != nil) {
		ValidateObject_(attr);
		outListAttr = attr;
		outInsertAfter = 0;
		return true;
	}

	// Second check: See if this is a sub of the container.
	
	ValidateObject_(element->GetSuperElement());
	attr = (dynamic_cast<DMListAttribute*>(element->GetSuperElement()));
	if (attr != nil) {
		ValidateObject_(attr);
		outListAttr = attr;
		outInsertAfter = attr->GetSubElements().FetchIndexOf(&element);
		return true;
	}
	
	// Nope. No list attribute.
	
	return false;
	
}


// ---------------------------------------------------------------------------
//		* NewDynamicItem										[protected]
// ---------------------------------------------------------------------------
//	If a dynamic list is selected, create a new item in the list. 

void
PIInspectorTable::NewDynamicItem()
{

	// Validate pointers.

	ValidateThis_();

	// See where the dynamic item is.

	DMListAttribute* attr;
	UInt32 after;
	if (!GetSelectedListItem(attr, after))
		return;
	
	ValidateObject_(attr);

	// We have a valid list item. Set up a transaction for the new item.
	
	StDMTransactionBuilder trans(mFollowUndoHost, mFollowSelection, cmd_NewItem);

	// Duplicate prototype item.
	
	DMAttributePtr newItem;
	newItem = (DMAttribute*) attr->GetPrototype()->Clone();
	ValidateObject_(newItem.GetObject());
	
	// Insert it into the container.
	
	attr->InsertElementAt(after+1, newItem, true);

	// Select the new item.
	
	ScrollElementIntoView(newItem, true);

	// Commit the transaction.
	
	trans.Commit();
	
}


// ---------------------------------------------------------------------------
//		* ClearDynamicItem										[protected]
// ---------------------------------------------------------------------------
//	If a dynamic-list item is selected, delete it.

void
PIInspectorTable::ClearDynamicItem()
{

	// Validate pointers.

	ValidateThis_();

	// See where the dynamic item is.

	DMListAttribute* attr;
	UInt32 after;
	if (!GetSelectedListItem(attr, after) || (after < 1))
		return;

	ValidateObject_(attr);
		
	// We have a valid list item. Set up a transaction for the deleted item.
	
	StDMTransactionBuilder trans(mFollowUndoHost, mFollowSelection, cmd_Clear);

	// Remove the currently selected item.
	
	DMElement* removeElement;
	attr->GetSubElements().FetchItemAt(after, &removeElement);
	ValidateObject_(removeElement);

	attr->RemoveElement(removeElement);

	// Commit the transaction.
	
	trans.Commit();
	
}


// ---------------------------------------------------------------------------
//		* DuplicateDynamicItem									[protected]
// ---------------------------------------------------------------------------
//	If a dynamic-list item is selected, make a copy of it and put the copy
//	in the list just below the original.

void
PIInspectorTable::DuplicateDynamicItem()
{

	// Validate pointers.

	ValidateThis_();

	// See where the dynamic item is.

	DMListAttribute* attr;
	UInt32 after;
	if (!GetSelectedListItem(attr, after))
		return;

	ValidateObject_(attr);
	
	// We have a valid list item. Set up a transaction for the new item.
	
	StDMTransactionBuilder trans(mFollowUndoHost, mFollowSelection, cmd_Duplicate);

	// Duplicate current item.
	
	DMElement* currentItem;
	attr->GetSubElements().FetchItemAt(after, &currentItem);
	ValidateObject_(currentItem);

	DMAttributePtr newItem;
	newItem = (DMAttribute*) currentItem->Clone();
	ValidateObject_(newItem.GetObject());
	
	// Insert it into the container.
	
	attr->InsertElementAt(after+1, newItem, true);

	// Select the new item.
	
	OVItem* newDisplay = FindItemForSubElement(newItem, true);
	if (newDisplay != nil) {
		ValidateObject_(newDisplay);
		STableCell cell(FindRowForItem(newDisplay), 1);
		UnselectAllCells();
		SelectCell(cell);
	}

	// Commit the transaction.
	
	trans.Commit();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection-following behaviors

// ---------------------------------------------------------------------------
//		* SetFollowSelection									[protected]
// ---------------------------------------------------------------------------
//	A new window has been brought to the front. The property inspector
//	should follow this window's selection.

void
PIInspectorTable::SetFollowSelection(
	DMSelection*	inFollowSelection,
	LCommander*		inFollowUndoHost)
{

	// Validate pointers.

	ValidateThis_();
	if (inFollowSelection != nil)
		ValidateObject_(inFollowSelection);
	if (inFollowUndoHost != nil)
		ValidateObject_(inFollowUndoHost);

	// Disconnect from old selection.

	if (mFollowSelection != nil) {
		ValidateObject_(mFollowSelection.GetObject());
		mFollowSelection->RemoveListener(this);
	}

	// Choose an element from new selection.

	mFollowSelection = inFollowSelection;
	SetElementFromSelection();
	
	// Start listening to new selection.

	if (mFollowSelection != nil) {
		ValidateObject_(mFollowSelection.GetObject());
		mFollowSelection->AddListener(this);
	}
	
	// Reroute undo/redo mapping.

	mFollowUndoHost = inFollowUndoHost;

}


// ---------------------------------------------------------------------------
//		* SelectionChanged										[protected]
// ---------------------------------------------------------------------------
//	The selection we're following has changed. See if we need to pick up
//	a new object. As an optimization, we don't reload the table until the
//	next event loop. This prevents it from going crazy when the selection
//	changes rapidly, as in drag-selection or Select All.

void
PIInspectorTable::SelectionChanged(
	DM_SelectionChanged*	inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inMessage->mSelection);
	
	// If our selection is changing, erase the PI table.
	// We'll pick a new item when the selection settles down
	// (thus the call to StartRepeating).

	if (inMessage->mSelection == mFollowSelection) {
		mSelectionChanged = true;
		StartRepeating();
	}
}


// ---------------------------------------------------------------------------
//		* GoingAway												[protected]
// ---------------------------------------------------------------------------
//	If the primary element we're watching is going away, then erase the
//	table contents.

void
PIInspectorTable::GoingAway(
	DMElement*	/* inElement */)
{

	// Validate pointers.

	ValidateThis_();
	
	// If detached, close the window and go away.
	
	if (mIsDetached) {
		LWindow* window = LWindow::FetchWindowObject(GetWindowFromPort(GetMacPort()));
		ValidateObject_(window);
		delete window;
		return;
	}
	
	// Clear table and wait for a new selection.

	SetElement(nil);

}


// ---------------------------------------------------------------------------
//		* SpendTime												[protected]
// ---------------------------------------------------------------------------
//	After the selection has changed and settled down, we pick up the first
//	object in the selection and inspect it.

void
PIInspectorTable::SpendTime(
	const EventRecord&	inMacEvent)
{

	// Validate pointers.

	ValidateThis_();

	// This is a bit ugly. Both DMDropArea and PIInspectorTable
	// have deferred tasks which run on the repeater queue. We use the
	// mSelectionChanged flag to signify that the task for PIInspectorTable
	// needs to fire.

	if (!mSelectionChanged) {
		DMDropArea::SpendTime(inMacEvent);
		return;
	}
	
	// Okay, it's our thing. Once through is enough.
	
	mSelectionChanged = false;
	StopRepeating();
	
	// Inspect first object in selection.

	SetElementFromSelection();

}


// ---------------------------------------------------------------------------
//		* SetElementFromSelection								[protected]
// ---------------------------------------------------------------------------
//	The selection has changed; pick up the first element in the selection
//	and inspect it. If the selection is empty, we empty the table.

void
PIInspectorTable::SetElementFromSelection()
{

	// Validate pointers.

	ValidateThis_();

	// See if selection is empty.
	
	Boolean emptySelection = false;
	if (mFollowSelection == nil) {
	
		// No selection object... it must be empty. :-)
	
		SetElement(nil);
		return;
		
	}
	else {

		// We have a selection, check its item count.

		ValidateObject_(mFollowSelection.GetObject());
		if (mFollowSelection->GetSelectedElements().GetCount() < 1)
			emptySelection = true;

	}
	
 	// If selection is empty, just leave the previous element's data in the table.
	  
	if (emptySelection)
		return;
	
	// Okay, we have a selection, open PI for first selected item ONLY.

	DMElement* element = nil;
	mFollowSelection->GetSelectedElements().FetchItemAt(1, &element);
	ValidateObject_(element);
	
	DMObject* object = (dynamic_cast<DMObject*>(element));
	if (object != nil)
		ValidateObject_(object);

	SetElement(object);
	
	// Look for hidden preference resource.
	
	StApplicationContext appContext;
	Handle piPref = ::Get1Resource('INSP', 21000);
	if (piPref != nil) {
		ValidateHandle_(piPref);
		if (::GetHandleSize(piPref) >= 1) {
			Boolean detach = ** ((Boolean**) piPref);
			if (detach)
				DetachFromSelection();
		}
	}
}


// ---------------------------------------------------------------------------
//		* DetachFromSelection									[protected]
// ---------------------------------------------------------------------------
//	Detach this property inspector from the selection. This operation
//	is not reversable.

void
PIInspectorTable::DetachFromSelection()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Make sure we're currently inspecting an object.
	// If not, the detach command is ignored.
	
	if (mElement == nil)
		return;
	ValidateObject_(mElement.GetObject());
	
	// Disable the detach button.
	
	if (mDetachPIButton != nil) {
		ValidateObject_(mDetachPIButton);
		mDetachPIButton->Disable();
	}
	
	// Disconnect from selection.
	
	mFollowSelection = nil;
	mIsDetached = true;

	// Retitle the window to match our object's title.
	
	LWindow* window = LWindow::FetchWindowObject(GetWindowFromPort(GetMacPort()));
	ValidateObject_(window);

	LStr255 displayableName;
	mElement->GetDisplayableName(displayableName);
	if (displayableName.Length() > 0)
		window->SetDescriptor(displayableName);

	// Tell the property inspector manager that we're no longer following selection.
	
	LWindow* myWindow = LWindow::FetchWindowObject(GetWindowFromPort(GetMacPort()));
	ValidateObject_(myWindow);
	UPropertyInspector::ClosingInspectorWindow(myWindow);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag-receive behaviors

// ---------------------------------------------------------------------------
//		* CanHostElement										[protected]
// ---------------------------------------------------------------------------
//	Overriden to prevent a crash in the event that there is no object
//	displayed by the table.

Boolean
PIInspectorTable::CanHostElement(
	DMElement*			inElement,
	DM_DragPlacement*	ioPlacement) const
{

	// Validate pointers.

	ValidateThis_();
	
	// If we aren't displaying anything, we can't host anything.
	
	if (mElement == nil)
		return false;
	else
		return OVTable::CanHostElement(inElement, ioPlacement);

}


// ---------------------------------------------------------------------------
//		* DragIsAcceptable										[protected]
// ---------------------------------------------------------------------------
//	Probably temporary, this routine just rejects all drags.

Boolean
PIInspectorTable::DragIsAcceptable(
	DragReference	/* inDragRef */)
{
	return false;
}


// ===========================================================================

#pragma mark -
#pragma mark ** low-memory handling

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	If we get the low-memory message from the grow zone object, close
//	the property inspector.

void
PIInspectorTable::ListenToMessage(
	MessageT		inMessage,
	void*			ioParam)
{

	switch (inMessage) {
	
		// Grow zone: Property inspector is expendable, close it if low on memory.
	
		case msg_GrowZone:
			LWindow* window;
			window = LWindow::FetchWindowObject(GetWindowFromPort(GetMacPort()));
			ValidateObject_(window);
			window->DoClose();
			break;
		
		// Detach PI button: Detach this inspector from the selection.
		
		case 'DTCH':
			DetachFromSelection();
			break;
		
		// Other messages: Pass along to table.
		
		default:
			OVTable::ListenToMessage(inMessage, ioParam);

	}
}


// ---------------------------------------------------------------------------
//		* FindUIObject											[protected]
// ---------------------------------------------------------------------------
//	Return pointer to PI.

void
PIInspectorTable::FindUIObject(
	DM_FindUIObject*	inFindUIMsg)
{

	// Validate pointers.
	
	ValidateThis_();
	ThrowIfNil_(inFindUIMsg);
	
	// If caller is looking for a property inspector, we're probably it.

	if ((inFindUIMsg->mElement == mElement) && (inFindUIMsg->mObjectIdentifier == object_ID))
		inFindUIMsg->mObjectPointer = this;

}


// ===========================================================================

#pragma mark -
#pragma mark *** PIItemFactory ***

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------
//	Create the appropriate element for the given element.

OVItem*
PIItemFactory::CreateItemForElement(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateObject_(inElement);

	// Make sure this is really an attribute.
	
	DMAttribute* attribute = (dynamic_cast<DMAttribute*>(inElement));
	ValidateObject_(attribute);
	
	// Disable display if so requested.
	
	if (!attribute->IncludeInDisplay())
		return nil;

	// Choose attribute item for the kind of attribute we have.

	if ((dynamic_cast<DMContainerAttribute*>(inElement)))
		return new PIContainerItem;
	else if ((dynamic_cast<DMFontStringAttribute*>(inElement)))
		return new PIFontItem;
	else if ((dynamic_cast<DMStringAttribute*>(inElement)))
	{
		if ((dynamic_cast<DMStringAttribute*>(inElement))->GetNeedsHex())
			return new PIHexStringItem;
		else
			return new PIStringItem;
	}
	else if ((dynamic_cast<DMLabelledIntegerAttribute*>(inElement)))
		return new PILabelledItem;
#if Constructor_UseRF
	else if ((dynamic_cast<DMResourceRefAttribute*>(inElement)))
		return new PIResourceRefItem;
#endif
	else if ((dynamic_cast<DMIntegerAttribute*>(inElement))) {
		DMIntegerAttribute* intAttr = (static_cast<DMIntegerAttribute*>(inElement));
		ValidateObject_(intAttr);
		if (intAttr->GetValueSize() == 1)
			return new PIBooleanItem;
		else
			return new PIStringItem;
	}
	else if ((dynamic_cast<DMRGBColorAttribute*>(inElement)))
		return new PIColorItem;
	else
		return new PIAttributeItem;

}