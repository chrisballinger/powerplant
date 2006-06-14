// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEOutlineTable.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/11/96
//	   $Date: 2006/01/18 01:33:11 $
//	$History: MEOutlineTable.cpp $
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:23
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed a bug in HostDragElement that caused duplicate menus to be
//	inserted when dragging from the project windows. (Bug fix #1271.)
//	
//	*****************  Version 14  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:04p
//	Updated in $/ConstructorWin/Sources
//	Initial merge for MSVC-hosted build
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 01/22/97   Time: 20:10
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed Constructor_ForJava #ifdefs.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 01/07/97   Time: 18:12
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Rolled in an update from Clint 01/06/97.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 12/08/96   Time: 20:27
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Added support for moving title bar in menu editor window.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 12/07/96   Time: 13:52
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/07/96   Time: 11:29
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Updated toRobin's drop 12/06/96.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/06/96   Time: 23:55
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:07
//	Updated in $/Constructor/Source files/Editors/Menus
//	Updated for new undo/redo menu string architecture.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/02/96   Time: 15:06
//	Updated in $/Constructor/Source files/Editors/Menus
//	Added cmd_NewItemDialog menu command.
//
//	*****************  Version 4   *****************
//	User: scouten      Date: 10/24/96   Time: 11:45
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Fixed bugs in drag-receive handling.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/24/96   Time: 01:18
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Finished menu editor.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 20:56
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Baseline working version of menu editor.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:59
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "MEOutlineTable.h"

	// Core : Headers
#include "Constructor.menu.h"
#include "Constructor.ME.menu.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMDragTask.h"
#include "DMEditAttachment.h"

	// Core : Documents : Generic resource file
#include "RSResource.h"

	// Core : Editors : Menus
#include "MEEditor.h"
#include "MEMenuBarItem.h"
#include "MEMenuHeaderItem.h"
#include "MEMenuEntryItem.h"
#include "MEModelMenu.h"
#include "MEModelMenuBar.h"
#include "MEModelMenuItem.h"
#include "MEStrings.h"

	// Core : Property inspector
#include "UPropertyInspector.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : CA : Text editing
#include <LInPlaceEditField.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** MEOutlineTable ***

// ---------------------------------------------------------------------------
//		* MEOutlineTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

MEOutlineTable::MEOutlineTable(
	LStream* inStream)

: OVTable(inStream, 16, false)

{
	
	// Create outline item factory.

	SetItemFactory(new MEItemFactory);

	// Reset state variables.
	
	mMenuBar = nil;
	mMenuHeader = nil;
	mMEEditor = nil;

	// Configure undo/redo strings.

	DMDropArea::SetEditMenuStrings(STR_MEEditMenuStrings);

}


// ---------------------------------------------------------------------------
//		* ~MEOutlineTable
// ---------------------------------------------------------------------------
//	Destructor

MEOutlineTable::~MEOutlineTable()
{
	TakeOffDuty();
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection accessor

// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Overriden to set up edit-menu attachment.

void
MEOutlineTable::SetSelection(
	DMSelection*	inSelection)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSelection);

	// Set table's selection.

	OVTable::SetSelection(inSelection);

	// Configure edit menu handling.

	AddAttachment(new DMEditAttachment(
					this,						// inActionHost
					mSelection,					// inSelection
					STR_MEEditMenuStrings));	// inEditMenuStrings

}


// ===========================================================================

#pragma mark -
#pragma mark ** menu commands

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Overridden to enable New Item, New Separator Item, and New Menu commands.

void
MEOutlineTable::FindCommandStatus(
	CommandT		inCommand,
	Boolean&		outEnabled,
	Boolean&		outUsesMark,
	UInt16&			outMark,
	Str255			outName)
{

	switch (inCommand) {
	
		case cmd_NewItem:
		case cmd_NewItemDialog:
		case cmd_NewSeparatorItem:
			if (mMenuHeader != nil) {
				ValidateObject_(mMenuHeader);
				outEnabled = mMenuHeader->IsEnabled() && !(mMenuHeader->IsFakeMenu());
			}
			break;

		case cmd_NewMenu:
			if (mMenuBar != nil) {
				ValidateObject_(mMenuBar);
				outEnabled = mMenuBar->IsEnabled();
			}
			break;

		default:
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
	
	}

}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Overridden to respond to New Item, New Separator Item, and New
//	Menu commands.

Boolean
MEOutlineTable::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	switch (inCommand) {
	
		case cmd_NewItem:
		case cmd_NewItemDialog:
		case cmd_NewSeparatorItem:
			CreateNewItem(inCommand);
			break;
			
		case cmd_NewMenu:
			CreateNewMenu(inCommand);
			break;

		default:
			return OVTable::ObeyCommand(inCommand, ioParam);
	
	}

	return true;

}


// ---------------------------------------------------------------------------
//		* CreateNewItem
// ---------------------------------------------------------------------------
//	Create a new menu item. The menu command code is passed in so that
//	the correct type of item can be created.

void
MEOutlineTable::CreateNewItem(
	CommandT	inCommand)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mMEEditor);
	ValidateObject_(mSelection.GetObject());
	
	// Clone the existing menu item prototype.

	DMElementPtr originalItem;
	mMEEditor->GetPrototypeMenuItem(originalItem);
	ValidateObject_(originalItem.GetObject());
	
	MEModelMenuItem* itemME = (dynamic_cast<MEModelMenuItem*>(originalItem.GetObject()));
	ValidateObject_(itemME);
	
	// Configure attributes.

	switch (inCommand) {
	
		case cmd_NewItem:
		case cmd_NewItemDialog: {
			StApplicationContext appContext;
			LStr255 newItemText(STR_MEMiscStrings, str_UntitledItem);
			itemME->SetMenuText(newItemText);
			itemME->SetEnabled(1);
			break;
		}
		
		case cmd_NewSeparatorItem: {
			itemME->SetDividerLine(1);
			break;
		}
		
		default:
			SignalCStr_("Unknown new item command.");
	}
	
	// Now add it to the data model. Find preferred location.

	DM_DragPlacement placement = { nil, 0, true };
	if (!CanHostElement(itemME, &placement)) {
		SignalCStr_("Can't place new item.");
		return;
	}
	
	// Create transaction to record the change.
	
	StDMTransactionBuilder trans(this, mSelection, inCommand);
	
	// Empty selection.
	
	mSelection->SelectNone();

	// It is acceptable. Add it to the data model.

	HostElement(itemME, &placement);
	mSelection->SelectAdd(itemME);

	// Make sure it's visible.

	OVItem* item = FindItemForSubElement(itemME, true);
	ValidateObject_(item);
	
	STableCell cell;
	cell.row = FindRowForItem(item);
	cell.col = MenuColumn_ItemText;
	ScrollCellIntoFrame(cell);

	if (inCommand != cmd_NewSeparatorItem)
		item->StartInPlaceEdit(cell);	

	// Commit the transaction.
	
	trans.Commit();

}


// ---------------------------------------------------------------------------
//		* CreateNewMenu
// ---------------------------------------------------------------------------
//	Create a new menu. Only available if editing a menu bar.

void
MEOutlineTable::CreateNewMenu(
	CommandT	inCommand)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mMEEditor);
	ValidateObject_(mMenuBar);
	ValidateObject_(mSelection.GetObject());
	
	// Clone the existing menu item prototype.

	DMElementPtr menuDM;
	mMEEditor->GetPrototypeMenu(menuDM);
	ValidateObject_(menuDM.GetObject());
	
	MEModelMenu* menuME = (dynamic_cast<MEModelMenu*>(menuDM.GetObject()));
	ValidateObject_(menuME);
	
	// Now add it to the data model. Find preferred location.

	DMContainerAttribute* menuList = mMenuBar->GetMenus();
	ValidateObject_(menuList);
	SInt32 position = menuList->GetSubElements().FetchIndexOf(&mMenuHeader) + 1;

	// Choose menu title based on insertion position.
	
	LStr255 title;
	mMEEditor->GetDefaultTitleForPosition(position,title);
	menuME->SetMenuTitle(title);
	menuME->SetNewMenu(true);
	
	// Create transaction to record the change.
	
	StDMTransactionBuilder trans(this, mSelection, inCommand);
	
	// Empty selection.
	
	mSelection->SelectNone();

	// It is acceptable. Add it to the data model.

	menuList->InsertElementAt(position, menuME, true);
	mSelection->SelectAdd(menuME);

	// Make sure it's visible.

	OVItem* item = FindItemForSubElement(menuME, true);
	ValidateObject_(item);
	
	STableCell cell;
	cell.row = FindRowForItem(item);
	cell.col = MenuColumn_ItemText;
	ScrollCellIntoFrame(cell);

	item->StartInPlaceEdit(cell);	

	// Commit the transaction.
	
	trans.Commit();

}


// ===========================================================================

#pragma mark -
#pragma mark ** key-event processing

// ---------------------------------------------------------------------------
//		* ProcessKeyPress
// ---------------------------------------------------------------------------
//	Overriden to respond to left and right arrow keys (to switch menus
//	in menu bar). We override ProcessKeyPress instead of HandleKeyPress
//	since the outline key selector attachment will grab key events
//	otherwise.

Boolean
MEOutlineTable::ProcessKeyPress(
	const EventRecord&	inKeyEvent)
{

	// Validate pointers.
	
	ValidateThis_();

	// Decode the key-down message.

	SInt16 theKey = inKeyEvent.message & charCodeMask;
	switch (theKey) {

		case char_LeftArrow:
			SelectLeftMenu();
			return true;
			
		case char_RightArrow:
			SelectRightMenu();
			return true;
			
		case char_Enter:
		case char_Return:
			ValidateObject_(mSelection.GetObject());
			UPropertyInspector::InspectSelection(mSelection, this);
			return true;

		default:
			return LCommander::ProcessKeyPress (inKeyEvent);

	}
}


// ---------------------------------------------------------------------------
//		* SelectLeftMenu
// ---------------------------------------------------------------------------
//	Select the menu to the left of the current menu. Only available if
//	editing a menu bar.

void
MEOutlineTable::SelectLeftMenu()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// If no menu bar, no arrow keys.
	
	if (mMenuBar == nil)
		return;
	
	// Iterate over menus, looking for menu immediately prior to the shown menu.
	
	MEModelMenu* previousMenu = nil;

	MEMenuBarIterator iter(mMenuBar);
	while (iter.NextMenu()) {

		if (iter.CurrentMenu() == mMenuHeader) {
			if (previousMenu != nil) {
				mSelection->SelectSimple(previousMenu);
				ShowMenu(previousMenu);
			}
			return;
		}
		
		previousMenu = iter.CurrentMenu();
	}
		
}


// ---------------------------------------------------------------------------
//		* SelectRightMenu
// ---------------------------------------------------------------------------
//	Select the menu to the right of the current menu. Only available if
//	editing a menu bar.

void
MEOutlineTable::SelectRightMenu()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// If no menu bar, no arrow keys.
	
	if (mMenuBar == nil)
		return;
	
	// Iterate over menus, looking for menu immediately prior to the shown menu.
	
	Boolean foundCurrent = false;

	MEMenuBarIterator iter(mMenuBar);
	while (iter.NextMenu()) {

		if (foundCurrent) {
			mSelection->SelectSimple(iter.CurrentMenu());
			ShowMenu(iter.CurrentMenu());
			return;
		}

		if (iter.CurrentMenu() == mMenuHeader)
			foundCurrent = true;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* AdjustImageSize										[protected]
// ---------------------------------------------------------------------------
//	Overriden to extend image. A bit of desktop space beyond the end of
//	the menu frame is desirable.

void
MEOutlineTable::AdjustImageSize(
	Boolean		inRefresh)
{

	// Validate pointers.

	ValidateThis_();

	// If inserting lots of items, no need to recalculate image size each time.

	if (mOkayToAdjustImageSize) {
		
		// Do normal image size calculation.
	
		LTableView::AdjustImageSize(false);
		
		// Add space to bottom and right to allow for desktop space.

		SDimension32 imageSize;
		GetImageSize(imageSize);
		
		imageSize.width += 20;
		imageSize.height += 20;

		// If editing a menu bar, we want to make sure that
		// all menu titles are visible.
		
		if (mMenuBar != nil) {
			
			// Iterate through all menus, doing nothing in
			// particular with any of them.
			
			MEMenuBarIterator iter(mMenuBar);
			while (iter.NextMenu()) { }
			
			// Now that we're at the end of the list, find out
			// how wide the list was. Make sure there's room for
			// all menus.
			
			Point textPoint;
			iter.GetTextOrigin(textPoint);
			
			if (textPoint.h > imageSize.width)
				imageSize.width = textPoint.h + 20;
			
		}

		// Now set the final image size.
		
		ResizeImageTo(imageSize.width, imageSize.height, inRefresh);

	}
}


// ---------------------------------------------------------------------------
//		* ScrollImageBy
// ---------------------------------------------------------------------------
//	Overriden to improve drag-receive scrolling.

void
MEOutlineTable::ScrollImageBy(
	SInt32		inLeftDelta,
	SInt32		inTopDelta,
	Boolean		inRefresh)
{

	// Validate pointers.

	ValidateThis_();
	
	// Since the background is changed for the drag hiliting,
	// we can't use Drag{Pre,Post}Scroll or we'll end up with
	// white garbage all over the screen when we scroll.
	// If not hiliting, we can use the regular ScrollBits routine.
	
	if (!mIsHilited) {
		OVTable::ScrollImageBy(inLeftDelta, inTopDelta, inRefresh);
		return;
	}

	// Okay, we're hiliting. Remove the drag border and redraw.
	
	FocusDraw();
	mIsHilited = false;
	UnhiliteDropArea(mDragRef);
	
	// Scroll the view.

	LView::ScrollImageBy(inLeftDelta, inTopDelta, inRefresh);

	// Now redraw the hiliting.

	FocusDraw();
	mIsHilited = true;
	HiliteDropArea(mDragRef);

}


// ---------------------------------------------------------------------------
//		* SetColWidth
// ---------------------------------------------------------------------------
//	Overridden to slide the title bar elements as menu frame moves.

void
MEOutlineTable::SetColWidth(
	UInt16			inWidth,
	TableIndexT		inFromCol,
	TableIndexT		inToCol)
{

	// Validate pointers. 

	ValidateThis_();
	ValidateObject_(mSuperView);

	// Resize the column.

	LTableView::SetColWidth(inWidth, inFromCol, inToCol);

	// Slide the title bar elements over.
	
	if (inFromCol == 1) {
		LPane* titleBar = mSuperView->GetSuperView()->FindPaneByID('TBAR');		//! TEMPORARY: magic number
		ValidateObject_(titleBar);
		titleBar->PlaceInSuperFrameAt(inWidth, 0, true);
	}
}


// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Overriden to draw menu frame and desktop background.

void
MEOutlineTable::DrawSelf()
{

	// Validate pointers.

	ValidateThis_();
	
	// Draw menu bar trimmings.
	
	DrawMenuAdornments();
	
	// Draw the table.
	
	OVTable::DrawSelf();

}


// ---------------------------------------------------------------------------
//		* DrawMenuAdornments									[protected]
// ---------------------------------------------------------------------------
//	Draw desktop background, menu frame, and menu bar simulation.

void
MEOutlineTable::DrawMenuAdornments()
{

	// Validate pointers.

	ValidateThis_();

	// Find out where the table is drawn.

	Rect frame;
	CalcLocalFrameRect(frame);
	
	// Find the bottom of the menu bar.
	
	Rect menuBarCorner;
	STableCell cell(1, MenuColumn_LeftGap);
	SInt32 left, right, top, bottom;
	
	if (IsValidCell(cell)) {
		GetImageCellBounds(cell, left, top, right, bottom);
		menuBarCorner.left = left;
		menuBarCorner.top = top;
		menuBarCorner.right = right;
		menuBarCorner.bottom = bottom;
	}
	else {
		menuBarCorner.left = 0;
		menuBarCorner.top = 0;
		menuBarCorner.bottom = 20;
		menuBarCorner.right = 20;
	}
	
	// Find the bottom right corner of the menu.
	
	Rect menuBottom;
	GetTableSize(cell.row, cell.col);
	cell.col = MenuColumn_RightGap;

	if (IsValidCell(cell)) {
		GetImageCellBounds(cell, left, top, right, bottom);
		menuBottom.left = left;
		menuBottom.top = top;
		menuBottom.right = right;
		menuBottom.bottom = bottom;
	}
	else {
		menuBottom = menuBarCorner;
		menuBottom.left = menuBottom.right;
		menuBottom.right += 20;
	}
	
	// Erase menu frame area to gray. If dragging to this window, we don't
	// paint the outer two pixels of the frame.
	
	Rect backFrame = frame;
	if (mIsHilited)
		::InsetRect(&backFrame, 2, 2);

	backFrame.top = menuBarCorner.bottom;
	if (backFrame.top < frame.top) {
		backFrame.top = frame.top;
		if (mIsHilited) {
			if (backFrame.top < frame.top + 2)
				backFrame.top = frame.top + 2;
		}
	}
	
	RGBColor theColor = UGAColorRamp::GetColor(2);
	::RGBBackColor(&theColor);
	
	Rect leftArea = { backFrame.top, backFrame.left, backFrame.bottom, menuBarCorner.right };
	::EraseRect(&leftArea);
	
	Rect middleArea = { menuBottom.bottom, menuBarCorner.right, backFrame.bottom, menuBottom.left };
	::EraseRect(&middleArea);
	
	Rect rightArea = { backFrame.top, menuBottom.left, backFrame.bottom, backFrame.right };
	::EraseRect(&rightArea);
	
#if PP_Target_Carbon
	if (backFrame.top < menuBottom.bottom)
	{
		Rect menuFrame = {menuBarCorner.bottom - 1, menuBarCorner.right, menuBottom.bottom + 1, menuBottom.left + 1};
		::DrawThemeMenuBackground(&menuFrame, kThemeMenuTypePullDown);
	}
#else
	// Draw border at bottom of menu.
	
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor(&theColor);
	::MoveTo(0, menuBarCorner.bottom - 1);
	::LineTo(frame.right, menuBarCorner.bottom - 1);
	
	// Draw frame around menu. If this is an empty menu, skip this step.
	
	if (backFrame.top < menuBottom.bottom) {
	
		// Draw basic frame.
	
		Rect menuFrame = { menuBarCorner.bottom - 1, menuBarCorner.right, menuBottom.bottom + 1, menuBottom.left + 1 };
		::FrameRect(&menuFrame);
		
		// Draw extra shadow at bottom and right.
		
		::MoveTo(menuFrame.right, menuFrame.top + 3);
		::LineTo(menuFrame.right, menuFrame.bottom);
		::LineTo(menuFrame.left + 3, menuFrame.bottom);
		
	}
#endif
}


// ---------------------------------------------------------------------------
//		* ShowMenu												[protected]
// ---------------------------------------------------------------------------
//	Change the menu that's being shown in the outline table.
//	Should only be called when editing a menu bar.

void
MEOutlineTable::ShowMenu(
	MEModelMenu*	inMenu)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mMenuBar);
	if (inMenu != nil)
		ValidateObject_(inMenu);
	
	// No need to change anything if we're already showing this menu.

	if ((mMenuHeader == inMenu) && (inMenu != nil))
		return;
	
	// Cancel link to old menu.

	mMenuHeader = nil;

	// Remove the display objects for previous menu.
	
	while (mRows > 0) {
		LOutlineItem* item = FindItemForRow(1);
		ValidateObject_(item);
		RemoveItem(item, false, false);
	}
	
	// Choose a new menu to display.
	
	if (inMenu != nil) {
	
		// Caller specified a menu. Choose it.
		
		mMenuHeader = inMenu;
	
	}
	else {
	
		// Caller did not specify a menu. Choose first in menu list.
	
		DMContainerAttribute* menuList = mMenuBar->GetMenus();
		ValidateObject_(menuList);
		
		if (menuList->GetSubElements().GetCount() > 0)
			menuList->GetSubElements().FetchItemAt(1, &mMenuHeader);
		else
			mMenuHeader = nil;

	}

	// Create display elements for this menu.
	
	if (mMenuHeader != nil) {
		ValidateObject_(mMenuHeader);
		CreateItemForElement(mMenuHeader, false, true);
		mMenuHeader->AddListener(this);
	}

	// Update display size and redraw table.
	
	UpdateLeftGap();
	
	// Make sure this menu title is visible.
	
	STableCell cell(1, MenuColumn_ItemText);//MenuColumn_MarkChar);
	ScrollCellIntoFrame(cell);

}


// ---------------------------------------------------------------------------
//		* UpdateLeftGap											[protected]
// ---------------------------------------------------------------------------
//	Typically called by ShowMenu. Update the size of the left column
//	(which is the gap to left of menu frame) so that the menu frame
//	lines up under the current menu's frame.

void
MEOutlineTable::UpdateLeftGap()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mMenuBar);
	if (mMenuHeader != nil)
		ValidateObject_(mMenuHeader);

	// Stop in-place editing.
	
	STableCell ipeCell;
	GetCurrentInPlaceEditCell(ipeCell);
	
	if (ipeCell.row != 0) {
		
		OVItem* item = (dynamic_cast<OVItem*>(FindItemForRow(ipeCell.row)));
		ValidateObject_(item);
		
		LInPlaceEditField* ipeField = item->GetEditField();
		if (ipeField != nil) {
		
			ValidateObject_(ipeField);
			ipeField->StopEditing();
			
		}
	}

	// Find the desired menu.
	
	MEMenuBarIterator iter(mMenuBar);
	while (iter.NextMenu()) {

		// See which menu we're on.
		
		MEModelMenu* menu = iter.CurrentMenu();
		ValidateObject_(menu);
		
		// See if this is the menu we want to show.

		if ((menu == mMenuHeader) || (mMenuHeader == nil)) {
		
			// It is. Configure table to display this menu's items.
		
			Rect menuText;
			iter.GetTextRect(menuText);
			
			// Adjust the width of the left gap so that menu items
			// will line up with menu title.
			
			SetColWidth(menuText.left, MenuColumn_LeftGap, MenuColumn_LeftGap);

			// Make sure this menu title is visible.
			
			STableCell cell(1, MenuColumn_ItemText);//MenuColumn_MarkChar);
			ScrollCellIntoFrame(cell);

			// Don't need to finish this loop.
			
			break;

		}
	}
	
	// Trigger a redraw of table.
	
	AdjustImageSize(false);
	Refresh();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** mouse-down behaviors

// ---------------------------------------------------------------------------
//		* TrackEmptyClick										[protected]
// ---------------------------------------------------------------------------
//	If clicking on menu bar, see if we need to select a new menu.

void
MEOutlineTable::TrackEmptyClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		inMouseDown,
	const SOutlineDrawContents&	inDrawContents)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// If click falls below menu bar or we're editing a
	// single menu, we'll try drag selection.
	
	if ((inCell.row > 1) || (mMenuBar == nil)) {
		LOutlineTable::TrackEmptyClick(inCell, inMouseDown, inDrawContents);
		return;
	}
	
	// We have a menu bar click, iterate over the menus to 
	// see which one was hit.
	
	// Make sure we're the current port.

	FocusDraw();
	
	// Clear the selection.
	
	mSelection->SelectNone();
	
	// Hit test menu titles.
	
	MEMenuBarIterator iter(mMenuBar);
	while (iter.NextMenu()) {
	
		// Find out what menu we're on.
		
		MEModelMenu* menu = iter.CurrentMenu();
		ValidateObject_(menu);
	
		// Hit test this menu.

		Rect textRect;
		iter.GetTextRect(textRect);
		
		if (::PtInRect(inMouseDown.whereLocal, &textRect)) {
			
			// We hit this menu. Select it and change the menu display.
			
			Boolean changingMenus = false;
			if (menu != mMenuHeader)
				changingMenus = true;
			
			mSelection->SelectSimple(menu);
			ShowMenu(menu);
			UpdatePort();
			
			// See if user wants to drag or edit this menu.
			
			if (changingMenus) {
				SMouseDownEvent noShiftEvent = inMouseDown;
				noShiftEvent.macEvent.modifiers &= ~shiftKey;
				ClickSelf(noShiftEvent);
			}

			return;
		}
	}
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Configure column sizes.

void
MEOutlineTable::FinishCreateSelf()
{

	// Do inherited initialization.

	ValidateThis_();
	OVTable::FinishCreateSelf();

	// Set up columns.

#if Constructor_ForMacOS
	InsertCols(	7,				// PP has command ID column
#else
	InsertCols(	6,				// others don't
#endif
				0,				// inAfterCol
				nil,			// inDataPtr
				0,				// inDataSize
				false);			// inRefresh

	SetColWidth( 19, MenuColumn_LeftGap, MenuColumn_LeftGap);		// gap to left of menu and menu frame
	SetColWidth( 12, MenuColumn_MarkChar, MenuColumn_MarkChar);		// mark character
	SetColWidth(169, MenuColumn_ItemText, MenuColumn_ItemText);		// menu item text
	SetColWidth( 52, MenuColumn_CloverChar, MenuColumn_CloverChar);	// command key (cloverleaf character)
	SetColWidth( 12, MenuColumn_CmdKey, MenuColumn_CmdKey);			// command key (command key)
	SetColWidth( 10, MenuColumn_RightGap, MenuColumn_RightGap);		// space to right of menu and menu frame
#if Constructor_ForMacOS
	SetColWidth( 90, MenuColumn_CmdID, MenuColumn_CmdID);			// command ID
#endif

}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Receive the element that we'll be drawing. May be either an MEModelMenu
//	or MEModelMenuBar.

void
MEOutlineTable::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// See if this is a menu bar.
	
	mMenuBar = (dynamic_cast<MEModelMenuBar*>(inElement));
	if (mMenuBar != nil) {
		
		ValidateObject_(mMenuBar);
		
		// Create outline item for first menu in menu bar.
		
		ShowMenu(nil);
		
	}
	else {
	
		// Make sure this is a menu header.
		
		mMenuHeader = (dynamic_cast<MEModelMenu*>(inElement));
		ValidateObject_(mMenuHeader);
	
		// Create outline item for menu header.
		
		CreateItemForElement(mMenuHeader, false, true);
	
	}
		
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag receive

// ---------------------------------------------------------------------------
//		* ItemIsAcceptable										[protected]
// ---------------------------------------------------------------------------
//	Overriden to allow multiple menus to be dragged to the menu bar.

Boolean
MEOutlineTable::ItemIsAcceptable(
	DragReference		inDragRef,
	ItemReference		inItemRef)
{

	// Validate pointers.

	ValidateThis_();

	// Make sure this is a local drag.

	DragAttributes dragAttrs;
	if (::GetDragAttributes(inDragRef, &dragAttrs) != noErr)
		return false;

	if ((dragAttrs & kDragInsideSenderApplication) == 0)
		return false;

	// Okay, it's a local drag. Look for the data model flavor.
	
	FlavorFlags flavorFlags;
	if (::GetFlavorFlags(inDragRef, inItemRef, DM_DragFlavor, &flavorFlags) != noErr)
		return false;

	// Got data model flavor. We can cast the item reference to a data model element.
	
	DMElement* element = (DMElement*) inItemRef;
	ValidateObject_(element);
	
	// Here is the override code. If we're dragging a resource and we've
	// already come up with a placement, then just make sure this resource is also OK.
	
	if (mCurrentPlacement.mHostElement != nil) {
		RSResource* res = (dynamic_cast<RSResource*>(element));
		if (res != nil)
			return ChoosePlacementForElement(inDragRef, inItemRef, element, mCurrentPlacement);
	}

	// Otherwise, normal drag code is OK.

	return DMDropArea::ItemIsAcceptable (inDragRef, inItemRef);

}


// ---------------------------------------------------------------------------
//		* ChoosePlacementForElement								[protected]
// ---------------------------------------------------------------------------
//	Find a home for the element, given the current mouse location.

Boolean
MEOutlineTable::ChoosePlacementForElement(
	DragReference		inDragRef,
	ItemReference		inItemRef,
	const DMElement*	inElement,
	DM_DragPlacement&	ioPlacement)
{

	ValidateThis_();
	ValidateObject_(inElement);

	// Convert mouse location to image coordinates and a cell index.
	// Pin coordinates to a cell boundary so it always hits a cell.
	
	Point dragMouse;
	ThrowIfOSErr_(::GetDragMouse(inDragRef, &dragMouse, nil));
	
	FocusDraw();
	GlobalToPortPoint(dragMouse);
	PortToLocalPoint(dragMouse);
	
	SPoint32 imagePt;
	LocalToImagePoint(dragMouse, imagePt);
	if (imagePt.v < 0)
		imagePt.v = 0;
	if (imagePt.h < 0)
		imagePt.h = 0;
	if (imagePt.v >= mImageSize.height)
		imagePt.v = mImageSize.height - 1;

	// See if we're dragging a menu item.

	const MEModelMenuItem* menuItem = dynamic_const_cast(inElement,DMElement*,MEModelMenuItem*);
	Boolean draggingMenuItem = (menuItem != nil);

	// If we hit the menu bar, we take the click ourselves.
	// Otherwise, let the outline item handle it.
	
	STableCell hitCell;
	GetCellHitBy(imagePt, hitCell);
	Boolean draggingToMbar = (hitCell.row == 1);
	
	if ((mMenuBar != nil) && ((!draggingMenuItem) || draggingToMbar))
		return ChoosePlacementInMenuBar(inDragRef, inItemRef, inElement, ioPlacement, imagePt);
	else
		return OVTable::ChoosePlacementForElement(inDragRef, inItemRef, inElement, ioPlacement);

}


// ---------------------------------------------------------------------------
//		* ChoosePlacementInMenuBar								[protected]
// ---------------------------------------------------------------------------
//	User is dragging to menu bar. If it's a menu, show insertion point
//	in menu bar. If it's a menu item, switch menus.

Boolean
MEOutlineTable::ChoosePlacementInMenuBar(
	DragReference		inDragRef,
	ItemReference		inItemRef,
	const DMElement*	inElement,
	DM_DragPlacement&	ioPlacement,
	const SPoint32&		inImagePt)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	ValidateObject_(mMenuBar);
	
	// See if we're dragging a menu. We can accept either a menu resource
	// or a menu header from this document.
	
	Boolean draggingMenu = false;
	Boolean draggingMenuItem = false;
	
	// See if this is a resource item.
	
	const RSResource* resource = dynamic_const_cast(inElement,DMElement*,RSResource*);
	if (resource != nil) {
	
		// Looks like it's a resource. Make sure it's a Menu resource.
		
		ValidateObject_(resource);
		if (mMEEditor->GetMenuResourceType() == resource->GetResType()) {

			// It is: Accept the resource if it comes from this document.
		
			DMElement* menuSuper = (const_cast<RSResource*>(resource));
			while (menuSuper->GetSuperElement() != nil) {
				menuSuper = menuSuper->GetSuperElement();
				ValidateObject_(menuSuper);
			}
			
			if (mMenuBar->IsSubElementOf(menuSuper))
				draggingMenu = true;
			
			// Make sure we're not already editing this resource.
			
			REEditor* theEditor = (REEditor*) resource->FindUserInterfaceObject('edit');
			if (theEditor == mMEEditor)
				return false;
		
		}
	}

	// See if this is a menu header.
	
	const MEModelMenu* menu = dynamic_const_cast(inElement,DMElement*,MEModelMenu*);
	if (menu != nil)
		draggingMenu = true;
	
	// See if this is a menu item.
	
	const MEModelMenuItem* menuItem = dynamic_const_cast(inElement,DMElement*,MEModelMenuItem*);
	if (menuItem != nil)
		draggingMenuItem = true;

	// If it's neither a menu nor a menu item, there's no home for this object.
	
	if (!(draggingMenu || draggingMenuItem))
		return false;
	
	// Okay, we can accept this item.
	
	if (draggingMenuItem) {
	
		// Dragging a menu item. Dragging across the menu bar will cause
		// it to switch menus. This makes it easy to drag an item from
		// one menu to another.
		
		Point imagePt;
		imagePt.h = inImagePt.h;
		imagePt.v = inImagePt.v;
		
		MEMenuBarIterator iter(mMenuBar);
		while (iter.NextMenu()) {

			// Hit test this menu's title frame.
	
			Rect textRect;
			iter.GetTextRect(textRect);
			if (::PtInRect(imagePt, &textRect)) {
			
				// We've found the menu containing the mouse location.
				// See if we have to change menus.
			
				if (iter.CurrentMenu() != mMenuHeader) {
				
					// We're in a new menu. Change the menu display.
				
					ShowMenu(iter.CurrentMenu());
					UpdatePort();

				}
				
				// No need to continue hit testing.
				
				break;

			}
		}

		// Now see if this menu will accept the dragged menu item.
		
		return OVTable::ChoosePlacementForElement(inDragRef, inItemRef, inElement, ioPlacement);

	}
	
	// Not dragging a menu item. Must be a menu title.
	
	SignalIf_(!draggingMenu);
	
	// Choose a location between other menu titles.

	ioPlacement.mHostElement = mMenuBar->GetMenus();
	ioPlacement.mInsertionIndex = 1;

	MEMenuBarIterator iter(mMenuBar);
	while (iter.NextMenu()) {

		Rect textRect;
		iter.GetTextRect(textRect);
		SInt16 midpoint = (textRect.right + textRect.left) >> 1;

		// If drag cursor is to the left of the midpoint of this menu title, we've got our spot.

		if (inImagePt.h < midpoint)
			break;

		// It's to right of midpoint, flag the spot but keep looking.
		
		ioPlacement.mInsertionIndex++;
	
	}

	// Accept the menu drag.
	
	return true;

}


// ---------------------------------------------------------------------------
//		* HostDragElement										[protected]
// ---------------------------------------------------------------------------
//	Place the element. If dragging a Menu resource object, it needs to be
//	converted to an MEModelMenu first.

DMElement*
MEOutlineTable::HostDragElement(
	DMElement*			inElement,
	Rect&				inItemLocalBounds,
	DM_DragPlacement&	ioCurrentPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	ValidateObject_(ioCurrentPlacement.mHostElement);
	
	// See if this is a resource drop.
	// If not, the inherited code will handle it just fine. :-)
	
	RSResource* resource = (dynamic_cast<RSResource*>(inElement));
	if (resource == nil)
		return DMDropArea::HostDragElement(inElement, inItemLocalBounds, ioCurrentPlacement);

	// It is a resource drop. Ask the editor if it is a MENU resource.
	
	ValidateObject_(resource);
	ThrowIf_(mMEEditor->GetMenuResourceType() != resource->GetResType());
	
	// Ask our editor to inflate it for us.

	ValidateObject_(mMEEditor);
	
	DMElementPtr menuObject;
	mMEEditor->AddMenu(resource->GetResID(), menuObject);
	ValidateObject_(menuObject.GetObject());
	
	menuObject->ResetModified();

	// Okay... now we have a menu object. Inherited can host it for us.
	
	ioCurrentPlacement.ioInsertAsDuplicate = false;
	return DMDropArea::HostDragElement(menuObject, inItemLocalBounds, ioCurrentPlacement);

}


// ---------------------------------------------------------------------------
//		* HiliteDropArea										[protected]
// ---------------------------------------------------------------------------
//	Overriden to ensure that menu background gets hilited properly.

void
MEOutlineTable::HiliteDropArea(
	DragReference	inDragRef)
{
	RefreshDropBorder();
	UpdatePort();
	DMDropArea::HiliteDropArea(inDragRef);
}


// ---------------------------------------------------------------------------
//		* UnhiliteDropArea										[protected]
// ---------------------------------------------------------------------------
//	Overriden to ensure that menu background gets redrawn properly.

void
MEOutlineTable::UnhiliteDropArea(
	DragReference	inDragRef)
{
	DMDropArea::UnhiliteDropArea(inDragRef);
	RefreshDropBorder();
	UpdatePort();
}


// ---------------------------------------------------------------------------
//		* RefreshDropBorder										[protected]
// ---------------------------------------------------------------------------
//	Refresh the two-pixel area at outer edges of frame. Usually called
//	by HiliteDropArea or UnhiliteDropArea. (DrawMenuAdornments draws
//	this area in white instead of gray during a drag receive. This allows
//	the area to be hilited.)

void
MEOutlineTable::RefreshDropBorder()
{

	// Validate pointers.

	ValidateThis_();
	FocusDraw();

	// We want to refresh a two-pixel area at the outer edges of our frame.
	// This corresponds to where the Drag Manager border is drawn.
	
	RgnHandle outerRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(outerRgn);
#else
	ValidateHandle_((Handle) outerRgn);
#endif
	
	RgnHandle innerRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(innerRgn);
#else
	ValidateHandle_((Handle) innerRgn);
#endif
	
	Rect frame;
	CalcLocalFrameRect(frame);
	
	::RectRgn(outerRgn, &frame);
	
	::InsetRect(&frame, 2, 2);
	::RectRgn(innerRgn, &frame);
	
	::DiffRgn(outerRgn, innerRgn, outerRgn);
	InvalPortRgn(outerRgn);
	
	// Release our temporary regions.
	
	::DisposeRgn(outerRgn);
	::DisposeRgn(innerRgn);
	
}


// ---------------------------------------------------------------------------
//		* DrawPlacementIndicator								[protected]
// ---------------------------------------------------------------------------
//	Draw insertion point in menu bar or between menu items as appropriate.

void
MEOutlineTable::DrawPlacementIndicator(
	const DM_DragPlacement&	inPlacement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Make sure we have a valid placement.

	Point displayLoc;
	if (!CalcPlacementDisplayLoc(inPlacement, displayLoc))
		return;
		
	// Are we drawing in the menu bar?
	
	if (displayLoc.v == 1) {
	
		// Yes, draw a vertical insertion point.
	
		FocusDraw();
		
		if (mInsertionPointVisible) {
			StColorPenState pen;
			pen.Normalize();
			Pattern thePattern;
			UQDGlobals::GetGrayPat(&thePattern);
			::PenPat(&thePattern);
			::PenSize(1, 1);
			::MoveTo(displayLoc.h, 1);
			::LineTo(displayLoc.h, 17);
		}
	
	}
	else {

		// Drawing placement between menu items.
		// Find the left edge of menu list.
		
		STableCell cell(1, MenuColumn_LeftGap);
		SInt32 left, right, top, bottom;
		
		if (IsValidCell(cell)) {
			GetImageCellBounds(cell, left, top, right, bottom);
			displayLoc.h = right + 8;
		}
		else
			return;
		
		// Find the right edge of menu list.

		cell.col = MenuColumn_RightGap;
		SInt32 displayRightEdge;
		
		if (IsValidCell(cell)) {
			GetImageCellBounds(cell, left, top, right, bottom);
			displayRightEdge = left - 6;
		}
		else
			return;
	
		// Draw the small triangle just to left of placement.
		
		FocusDraw();
	
		RgnHandle trRegion = MakePlacementTriangleRgn(displayLoc);
#if PP_Target_Carbon
		ThrowIfNil_(trRegion);
#else
		ValidateHandle_((Handle) trRegion);
#endif
	
		::PaintRgn(trRegion);
		::DisposeRgn(trRegion);
	
		// Draw insertion point.
	
		if (mInsertionPointVisible) {
			StColorPenState pen;
			pen.Normalize();
			Pattern thePattern;
			UQDGlobals::GetGrayPat(&thePattern);
			::PenPat(&thePattern);
			::PenSize(1, 1);
			::MoveTo(displayLoc.h + 4, displayLoc.v);
			::LineTo(displayRightEdge, displayLoc.v);
		}
	}

}


// ---------------------------------------------------------------------------
//		* RefreshPlacementIndicator								[protected]
// ---------------------------------------------------------------------------
//	Trigger redraw of insertion point.

void
MEOutlineTable::RefreshPlacementIndicator(
	const DM_DragPlacement&	inPlacement,
	Boolean					inRefreshTriangle,
	Boolean					inRefreshInsertionPoint)
{

	// Validate pointers.

	ValidateThis_();
	
	// Make sure we have a valid placement.

	Point displayLoc;
	if (!CalcPlacementDisplayLoc(inPlacement, displayLoc))
		return;
		
	// Are we drawing in the menu bar?
	
	if (displayLoc.v == 1) {
	
		// Yes, refresh the vertical insertion point.
	
		FocusDraw();
		
		if (inRefreshInsertionPoint) {
			Rect invalRect = { 1, displayLoc.h, 18, displayLoc.h + 1 };
			CalcPortFrameRect(invalRect);
			InvalPortRect(&invalRect);
		}
	
	}
	else {
	
		// Nope, we're drawing placement between menu items.
		// Find the left edge of menu list.
		
		STableCell cell(1, MenuColumn_LeftGap);
		SInt32 left, right, top, bottom;
		
		if (IsValidCell(cell)) {
			GetImageCellBounds(cell, left, top, right, bottom);
			displayLoc.h = right + 8;
		}
		
		// Find the right edge of menu list.

		cell.col = MenuColumn_RightGap;
		SInt32 displayRightEdge = 2000;
		if (IsValidCell(cell)) {
			GetImageCellBounds(cell, left, top, right, bottom);
			displayRightEdge = left - 6;
		}
	
		// Draw the small triangle just to left of placement.
		
		FocusDraw();
	
		if (inRefreshTriangle) {
			RgnHandle trRegion = MakePlacementTriangleRgn(displayLoc);
#if PP_Target_Carbon
			ThrowIfNil_(trRegion);
#else
			ValidateHandle_((Handle) trRegion);
#endif
		
			InvalPortRgn(trRegion);
			::DisposeRgn(trRegion);
		}
	
		// Draw insertion point.
	
		if (inRefreshInsertionPoint) {
			Rect invalRect = { displayLoc.v, displayLoc.h + 4,
							   displayLoc.v + 1, displayRightEdge + 1 };
			CalcPortFrameRect(invalRect);
			InvalPortRect(&invalRect);
		}
	}

}


// ---------------------------------------------------------------------------
//		* CalcPlacementDisplayLoc								[protected]
// ---------------------------------------------------------------------------
//	Helper used by DrawPlacementIndicator and RefreshPlacementIndicator
//	to decide where to draw insertion point.

Boolean
MEOutlineTable::CalcPlacementDisplayLoc(
	const DM_DragPlacement&	inPlacement,
	Point&					outDisplayLoc)
{

	// Validate pointers.

	ValidateThis_();
	
	// See if we're drawing into the menu bar.
	// If not, just use the normal placement calculation.
	
	Boolean placementIsInMenuBar = false;
	
	if (mMenuBar != nil) {
		ValidateObject_(mMenuBar);
		placementIsInMenuBar = (inPlacement.mHostElement == mMenuBar->GetMenus());
	}
	
	if (!placementIsInMenuBar)
		return OVTable::CalcPlacementDisplayLoc(inPlacement, outDisplayLoc);
	
	
	// We are drawing into the menu bar.
	// Find the location where we'll draw.
	
	outDisplayLoc.v = 1;
	SInt32 insertionLoc = inPlacement.mInsertionIndex;
	
	MEMenuBarIterator iter(mMenuBar);
	while (iter.NextMenu()) {
		if (--insertionLoc == 0)
			break;
	}

	Rect textRect;
	iter.GetTextRect(textRect);
	
	outDisplayLoc.h = textRect.left + 3;
	return true;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu adjusting

// ---------------------------------------------------------------------------
//		* PutOnDuty												[protected]
// ---------------------------------------------------------------------------
//	Add New Separator and New Menu commands.

void
MEOutlineTable::PutOnDuty( LCommander* /* inNewTarget */)
{

	// Validate pointers.

	ValidateThis_();

	// Find edit menu.

	LMenu* editMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(menu_Edit);
	ValidateSimpleObject_(editMenu);

	// Add menu items for new separator item & new submenu item.
	// Insert them all after the standard "New Item" command.

	SInt16 newIndex = editMenu->IndexFromCommand(cmd_NewItemDialog);
	StApplicationContext appContext;

	if (mMenuBar != nil)
		editMenu->InsertCommand(LStr255(STR_MEMiscStrings, str_NewMenuText), cmd_NewMenu, (newIndex++) - 2);

	editMenu->InsertCommand(LStr255(STR_MEMiscStrings, str_NewSepMenuText), cmd_NewSeparatorItem, newIndex++);
//	editMenu->InsertCommand(LStr255(STR_MEMiscStrings, str_NewSubMenuText), cmd_NewSubMenuItem, newIndex++);
	editMenu->InsertCommand( (StringPtr)"\001-", 0,  newIndex++);

}


// ---------------------------------------------------------------------------
//		* TakeOffDuty											[protected]
// ---------------------------------------------------------------------------
//	Remove extra menu items that were added by PutOnDuty.

void
MEOutlineTable::TakeOffDuty()
{

	// Find edit menu.

	LMenu* editMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(menu_Edit);
	ValidateSimpleObject_(editMenu);

	// Remove separator after new submenu item.

	SInt16 sepIndex = editMenu->IndexFromCommand(cmd_NewSeparatorItem /* cmd_NewSubMenuItem */) + 1;
	editMenu->RemoveItem(sepIndex);

	// Remove menu items for new separator item & new submenu item.
	
	if (mMenuBar != nil)
		editMenu->RemoveCommand(cmd_NewMenu);

	editMenu->RemoveCommand(cmd_NewSeparatorItem);
//	editMenu->RemoveCommand(cmd_NewSubMenuItem);

}


// ===========================================================================

#pragma mark -
#pragma mark ** data model updates

// ---------------------------------------------------------------------------
//		* GoingAway												[protected]
// ---------------------------------------------------------------------------
//	If the menu we're displaying goes away, we have to choose a new menu.

void
MEOutlineTable::GoingAway(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// Let inherited take care of getting rid of display elements.
	
	OVListener::GoingAway(inElement);
	
	// See if this element was the menu we were showing.
	
	if (inElement == mMenuHeader) {
	
		// It was, choose another menu to take its place.
	
		ShowMenu(nil);
	
	}
	
}


// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	If a menu title is changing, redraw the menu bar.

void
MEOutlineTable::ValueChanged(
	DM_ValueChange* inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inMessage->mAttribute);
	ValidateObject_(inMessage->mObject);
	
	// See if this is an entry in the menu bar.
	// If so, we may have to redraw menu bar and/or resize
	// left gap.
	
	if ((mMenuBar != nil) && (inMessage->mObject != mMenuHeader)) {
		
		ValidateObject_(mMenuBar);
		
		DMContainerAttribute* menuList = mMenuBar->GetMenus();
		ValidateObject_(menuList);
		
		if (inMessage->mObject->GetSuperElement() == menuList) {
			
			// It is another menu. See if the title is changing.
			
			if (inMessage->mAttribute->GetKey() == MEModelMenu::attrKey_MenuTitle) {
				UpdateLeftGap();
				Refresh();
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* ContainerListChanged									[protected]
// ---------------------------------------------------------------------------
//	See if we are adding to menu bar. If so, redraw the menu bar and
//	select the newly added menu.

void
MEOutlineTable::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(inMessage->mMovedElement);
	
	// See if we're adding to a menu bar.

	if (mMenuBar != nil) {

		ValidateObject_(mMenuBar);
		if (inMessage->mContainer == mMenuBar->GetMenus()) {

			if (inMessage->mElementAdded) {
			
				MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(inMessage->mMovedElement));
				if (menu != nil) {
				
					// We are, show that menu.
				
					mSelection->SelectSimple(menu);
					ShowMenu(menu);
					Refresh();
					return;
				}
			}
			else {
			
				// Deleting a menu. Redraw display.
				
				Refresh();
			
			}
		}
	}

	// Not adding to menu bar, do normal display update.

	OVListener::ContainerListChanged(inMessage);

}


// ---------------------------------------------------------------------------
//		* CanHostElement										[protected]
// ---------------------------------------------------------------------------
//	Find a home for this element in menu bar or menu header.

Boolean
MEOutlineTable::CanHostElement(
	DMElement*			inElement,
	DM_DragPlacement*	ioPlacement) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	if (OVTable::CanHostElement(inElement, ioPlacement))
		return true;
	
	if (mMenuHeader != nil) {
	
		// See if menu header will accept items.
	
		ValidateObject_((const_cast<MEOutlineTable*>(this))->mMenuHeader);
		if (mMenuHeader->CanHostElement(inElement, ioPlacement)) {
		
			// It will, tweak placement so that it lands after
			// current selection.
		
			ValidateObject_(ioPlacement->mHostElement);
			
			DMContainerMixin* mixin = (dynamic_cast<DMContainerMixin*>(ioPlacement->mHostElement));
			if (mixin != nil) {
			
				// Look for the last element in this container that is already
				// selected. We'll drop in the new elements after that.
			
				ValidateObject_(mixin);
				DMIterator iter(const_cast<LSharableArray&>(mixin->GetSubElements()), LArrayIterator::from_End);
					// Can't use DMFastIterator. It doesn't support GetCurrentIndex().
				
				while (iter.PreviousElement()) {
					if (mSelection->IsSelected(iter.CurrentElement())) {
					
						// Found a selected element. This is our dropping place. :-)
						
						ioPlacement->mInsertionIndex = iter.GetCurrentIndex() + 1;
						break;
						
					}
				}
			}
			
			// Signal acceptable placement.
			
			return true;
		
		}
	}

	// Nobody wants it.
	
	return false;

}


// ===========================================================================

#pragma mark -
#pragma mark *** MEItemFactory ***

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------
//	Create display item for menu bar, menu title, or menu entry.

OVItem*
MEItemFactory::CreateItemForElement(
	DMElement* inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	if ((dynamic_cast<MEModelMenuItem*>(inElement)) != nil)
		return new MEMenuEntryItem;
	if ((dynamic_cast<MEModelMenu*>(inElement)) != nil) {
	
		// Return a different object if this is in a menu bar.
		// If it is, the second superelement will be a menu bar object.
	
		DMElement* super1 = inElement->GetSuperElement();
		if (super1 != nil) {
			ValidateObject_(super1);
			DMElement* super2 = super1->GetSuperElement();
			if (super2 != nil) {
				ValidateObject_(super2);
				if ((dynamic_cast<MEModelMenuBar*>(super2)) != nil)
					return new MEMenuBarItem;
			}
		}
		
		// Not in menu bar, just do the normal thing.
	
		return new MEMenuHeaderItem;
	}

	return nil;
}


// ===========================================================================

#pragma mark -
#pragma mark *** MEMenuBarIterator ***

// ---------------------------------------------------------------------------
//		* MEMenuBarIterator()
// ---------------------------------------------------------------------------
//	Constructor

MEMenuBarIterator::MEMenuBarIterator(
	MEModelMenuBar* inMenuBar)

: DMFastIterator(inMenuBar->GetMenus()->GetSubElements())

{
	mPosition = 18;
	UTextTraits::SetPortTextTraits((ResIDT) 0);
}


// ---------------------------------------------------------------------------
//		* ~MEMenuBarIterator
// ---------------------------------------------------------------------------
//	Destructor

MEMenuBarIterator::~MEMenuBarIterator()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu accessors

// ---------------------------------------------------------------------------
//		* NextMenu
// ---------------------------------------------------------------------------
//	Advance to the next menu in menu bar. Return false if at end of menu list.

Boolean
MEMenuBarIterator::NextMenu()
{

	// Reset font & size.
	
	::TextFont(0);
	::TextSize(12);

	// Advance horizontal position.

	if (CurrentMenu() != nil) {
		LStr255 text;
		GetText(text);
		mPosition += ::StringWidth(text) + 13;
	}
	
	// Advance iterator.
	
	return NextElement();

}


// ---------------------------------------------------------------------------
//		* CurrentMenu
// ---------------------------------------------------------------------------
//	Return the current menu header.

MEModelMenu*
MEMenuBarIterator::CurrentMenu()
{
	MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(CurrentElement()));
	if (menu != nil)
		ValidateObject_(menu);
	
	return menu;
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing accessors

// ---------------------------------------------------------------------------
//		* GetText
// ---------------------------------------------------------------------------
//	Return the title of the current menu.

void
MEMenuBarIterator::GetText(
	LStr255&	outText)
{

	// First, clear the text.

	outText[(UInt8)0] = 0;
	
	// Now get the menu object.
	
	MEModelMenu* menu = CurrentMenu();
	if (menu != nil) {
	
		// Yes, there is a menu. Get its title.
		
		ValidateObject_(menu);
		menu->GetMenuTitle(outText);
	
	}

}


// ---------------------------------------------------------------------------
//		* GetTextOrigin
// ---------------------------------------------------------------------------
//	Return the point at which the current menu's title should be drawn.

void
MEMenuBarIterator::GetTextOrigin(
	Point&		outTextOrigin)
{
	outTextOrigin.h = mPosition;
	outTextOrigin.v = 14;
}


// ---------------------------------------------------------------------------
//		* GetTextRect
// ---------------------------------------------------------------------------
//	Return a rectangle which encloses the current menu's title.

void
MEMenuBarIterator::GetTextRect(
	Rect&	outTextRect)
{
	LStr255 text;
	GetText(text);
	
	outTextRect.left = mPosition - 9;
	outTextRect.right = mPosition + ::StringWidth(text) + 9;
	outTextRect.top = 1;
	outTextRect.bottom = 19;
}