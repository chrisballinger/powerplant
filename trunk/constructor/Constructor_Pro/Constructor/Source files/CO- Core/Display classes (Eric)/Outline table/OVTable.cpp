// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVTable.cpp					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/31/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: OVTable.cpp $
//	
//	*****************  Version 17  *****************
//	User: scouten      QDate: 03/05/97   Time: 09:56
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Silenced "unused parameter" warnings for CW Win32 build in
//	MakeDragImage routine.
//	
//	*****************  Version 16  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Rolled in an update from Clint toi make table activate/deactivate
//	correctly in multi-view windows.
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 02/07/97   Time: 13:39
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Rolled in Clint's fixes 02/06/97.
//	
//	*****************  Version 13  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 12/17/96   Time: 00:09
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added an override to RestorePlace to fix a bug in LPane::RestorePlace.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 11/04/96   Time: 13:38
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added ScrollElementIntoView method.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/04/96   Time: 12:56
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed a crashing bug in FindCommandStatus.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/03/96   Time: 22:26
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed a commenting error.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/03/96   Time: 22:19
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed a bug in ScrollImageBy. Drag hiliting wasn't being redrawn
//	properly when scrolling.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:33
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Updated for new undo/redo menu string architecture.
//
//	*****************  Version 5   *****************
//	User: scouten      Date: 10/24/96   Time: 01:17
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added SortSelection call to TrackDrag so dragged items didn't get randomly resequenced.
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 10/23/96   Time: 23:27
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added support for drag-to-trash.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 21:33
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Fixed missing validation in CanHostElement.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 19:20
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added duplicate command. Improved scrolling behavior (esp. WRT dragging and selections).
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:44
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "OVTable.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMClipboard.h"
#include "DMDragTask.h"
#include "DMIterator.h"

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"
#include "OVSelector.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// Core : Utility classes
//#include "DragExtras.h"
#ifndef __DRAG__
#include <Drag.h>
#endif

	// PowerPlant : Table classes
#include <LTableMultiGeometry.h>

	// PowerPlant : Utility classes
#include <UAttachments.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>

	// PowerPlant : CA : Outline table
#include <LInPlaceOutlineKeySelector.h>
#include <LOutlineRowSelector.h>
#include <LTablePaneHostingGeometry.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

const UInt16 mask_NonShiftModifiers = cmdKey + optionKey + controlKey;


// ===========================================================================

#pragma mark *** OVTable ***

// ---------------------------------------------------------------------------
//		* OVTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

OVTable::OVTable(
	LStream*	inStream,
	SInt16		inRowHeight,
	Boolean		inAcceptReturnKey)

: LOutlineTable(inStream),
  OVListener(nil),
  DMDropArea(this)

{

	// Not currently dragging... :-)

	mCurrentDragRef = 0;

	// Set up defualt table geometry.

	SetOVTable(this);
	SetTableGeometry(new LTableMultiGeometry(this, 80, inRowHeight));

	// Set up keyboard selection and scrolling.

	AddAttachment(new LInPlaceOutlineKeySelector(this, msg_AnyMessage, inAcceptReturnKey));
	AddAttachment(new LKeyScrollAttachment(this));

	// Try to become default commander in the window.

	if (mSuperCommander != nil) {
		ValidateObject_(mSuperCommander);
		mSuperCommander->SetLatentSub(this);
	}

}


// ---------------------------------------------------------------------------
//		* ~OVTable
// ---------------------------------------------------------------------------
//	Destructor

OVTable::~OVTable()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection accessors

// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Call once before displaying this table to set the selection object
//	for this table.

void
OVTable::SetSelection(
	DMSelection* inSelection)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSelection);
	
	// Record selection.

	mSelection = inSelection;
	DMDropArea::SetSelection(inSelection);

	// Create table selection.

	SetTableSelector(new OVSelector(this, mSelection));

}


// ===========================================================================

#pragma mark -
#pragma mark ** subitem accessors

// ---------------------------------------------------------------------------
//		* GetSubElementItems
// ---------------------------------------------------------------------------
//	Return the sub items array.

const LArray*
OVTable::GetSubElementItems() const
{
	ValidateThis_();
	return &mFirstLevelItems;
}


// ---------------------------------------------------------------------------
//		* InsertItem
// ---------------------------------------------------------------------------
//	Insert an item into the table. See description under
//	LOutlineTable::InsertItem.

void
OVTable::InsertItem(
	LOutlineItem*	inOutlineItem,
	LOutlineItem*	inSuperItem,
	LOutlineItem*	inAfterItem,
	Boolean			inRefresh,
	Boolean			inAdjustImageSize)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inOutlineItem);

	// Let outline table insert element.

	LOutlineTable::InsertItem(inOutlineItem, inSuperItem,
								inAfterItem, inRefresh, inAdjustImageSize);

	// Keep a record of the OVItem.

	if ((inSuperItem == nil) && (mSubElements != nil)) {
		
		ValidateObject_(mSubElements);
	
		OVItem* item = (dynamic_cast<OVItem*>(inOutlineItem));
		ValidateObject_(item);

		mSubElements->InsertItemAt(1, item->GetElement());

	}
}


// ---------------------------------------------------------------------------
//		* RemoveItem
// ---------------------------------------------------------------------------
//	Remove an item from the table. See description under
//	LOutlineTable::RemoveItem.

void
OVTable::RemoveItem(
	LOutlineItem*	inOutlineItem,
	Boolean			inRefresh,
	Boolean			inAdjustImageSize)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inOutlineItem);

	// Yank the item out of our OVItem list.

	if (mSubElements != nil) {
		ValidateObject_(mSubElements);
		OVItem* item = (dynamic_cast<OVItem*>(inOutlineItem));
		ValidateObject_(item);
		mSubElements->RemoveItem(item->GetElement());
	}
	
	// Let the outline table forget about it also.
	
	LOutlineTable::RemoveItem(inOutlineItem, inRefresh, inAdjustImageSize);

}


// ===========================================================================

#pragma mark -
#pragma mark ** mouse-down

// ---------------------------------------------------------------------------
//		* Click
// ---------------------------------------------------------------------------
//	Overriden to ensure that this table becomes the target after a click.

void
OVTable::Click(
	SMouseDownEvent& inMouseDown)
{

	// Validate pointers.

	ValidateThis_();

	// Check target chain. Make sure this view is the active commander
	// after a click.

	if (IsActive()) {
		if (!IsOnDuty())
			SwitchTarget(this);
	}
	else if (mOnDuty == triState_Off) {
		if (mSuperCommander != nil) {
			ValidateObject_(mSuperCommander);
			mSuperCommander->SetLatentSub(this);
		}
	}

	// Overriden to accept clicks that bring this window to front.

	LPane* clickedPane = FindSubPaneHitBy(inMouseDown.wherePort.h, inMouseDown.wherePort.v);
	if (clickedPane != nil) {
	
		// Subpane was hit, let it handle the click.
	
		ValidateObject_(clickedPane);
		clickedPane->Click(inMouseDown);

	}
	else {
	
		// This is our click. Ignore whether it came as a "select click."
	
		PortToLocalPoint(inMouseDown.whereLocal);
		UpdateClickCount(inMouseDown);
		
		if (ExecuteAttachments(msg_Click, &inMouseDown)) {
			ClickSelf(inMouseDown);
		}
	}
	

}


// ===========================================================================

#pragma mark -
#pragma mark ** menu command behaviors

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Enable the paste command.

void
OVTable::FindCommandStatus(
	CommandT		inCommand,
	Boolean&		outEnabled,
	Boolean&		outUsesMark,
	UInt16&			outMark,
	Str255			outName)
{

	// Validate pointers.

	ValidateThis_();

	// Dispatch command.

	switch (inCommand) {
	
		// Paste: Enabled if clipboard is acceptable and table data is enabled.

		case cmd_Paste:
			if (mSelection != nil) {
				ValidateObject_(mSelection.GetObject());
				outEnabled = mSelection->GetRootElement()->IsEnabled() && CanPaste();
			}
			else
				outEnabled = false;
			break;
		
		// Duplicate: Enabled if selection is not empty and table data is enabled.
		
		case cmd_Duplicate:
			if (mSelection != nil) {
				ValidateObject_(mSelection.GetObject());
				outEnabled = mSelection->SelectionIsNotEmpty() && mSelection->GetRootElement()->IsEnabled();
			}
			break;
		
		// Otherwise, let superclass handle it.
	
		default:
			LCommander::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);

	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Accept the paste command.

Boolean
OVTable::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();

	// Dispatch command.

	switch (inCommand) {
	
		// Paste: Convert clipboard to data model elements.

		case cmd_Paste:
			DoPaste();
			return true;
			
		// Duplicate: Duplicate the selected elements.
		
		case cmd_Duplicate:
			DoDuplicate();
			return true;
		
		// Otherwise, let superclass handle it.

		default:
			return LCommander::ObeyCommand(inCommand, ioParam);
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard behaviors

// ---------------------------------------------------------------------------
//		* CanPaste
// ---------------------------------------------------------------------------
//	Return true if the named elements can be pasted into the data model here.

Boolean
OVTable::CanPaste() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Sanity check: Make sure there's a data model clipboard.
	
	DMClipboard* clip = DMClipboard::GetDMClipboard();
	ValidateObject_(clip);

	// Get the list of clipboard items.
	
	DMFastIterator iter(clip->GetDMScrap());
	DM_DragPlacement placement = { nil, 0, true };

	while (iter.NextElement()) {

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);

		if (CanHostElement(element, &placement))
			return true;

	}

	return false;

}


// ---------------------------------------------------------------------------
//		* DoPaste
// ---------------------------------------------------------------------------
//	Copy items from the clipboard into the data model.

void
OVTable::DoPaste()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Sanity check: Make sure there's a data model clipboard (and a selection).
	
	DMClipboard* clip = DMClipboard::GetDMClipboard();
	ValidateObject_(clip);
	
	// Scan the list of clipboard items.
	// We are looking for a placement for these items.

	DM_DragPlacement placement = { nil, 0, true };
	Boolean hasPlacement = false;
	
	DMFastIterator iter(clip->GetDMScrap());
	while (iter.NextElement()) {

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		
		if (CanHostElement(element, &placement)) {
			hasPlacement = true;
			break;
		}
	}
	
	// If no placement was created, the paste is rejected.
	
	if (!hasPlacement)
		return;

	// Start a transaction.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_Paste);
	mSelection->SelectNone();

	// Get the list of clipboard items.
	// Clone each item and place it in.
	
	iter.ResetTo(0);
	Boolean firstItem = true;

	while (iter.NextElement()) {

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);

		DM_DragPlacement tempPlace = placement;
		if (CanHostElement(element, &tempPlace)) {

			DMElementPtr clone = element->Clone();
			ValidateObject_(clone.GetObject());

			HostElement(clone, &placement);
			mSelection->SelectAdd(clone);

			if (placement.mInsertionIndex != LArray::index_Last)
				placement.mInsertionIndex++;
			
			if (firstItem) {
				ScrollElementIntoView(clone, false);
				firstItem = false;
			}
	
		}
	}

	// Commit transaction.

	trans.Commit();

}


// ---------------------------------------------------------------------------
//		* DoDuplicate
// ---------------------------------------------------------------------------
//	Duplicate all selected items in the table. Behaves sort of like the
//	Copy and Paste commands, except that the clipboard isn't affected.

void
OVTable::DoDuplicate()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Create a temporary list of objects. This is sort of like a clipboard
	// except that it doesn't affect the "real" clipboard.

	LSharableArray tempList;

	// Okay, we can copy items.
	// Request a non-recursive selection list.

	DMSelectionPtr copyList;
	copyList = (DMSelection*) mSelection->Clone();
	ValidateObject_(copyList.GetObject());
	
	copyList->EliminateRecursion();
	copyList->SortSelection();
	
	// Clone each item and add it to the temporary list.
	
	DMFastIterator cloneIter(copyList->GetSelectedElements());
	while (cloneIter.NextElement()) {

		DMElement* element = cloneIter.CurrentElement();
		ValidateObject_(element);

		DMElementPtr clone = element->Clone();
		ValidateObject_(clone.GetObject());

		tempList.InsertItemAt(LArray::index_Last, clone);

	}
	
	// Okay, now we have the copied list. Now we "paste" the items back in.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_Duplicate);

	// Scan the list of clipboard items.
	// We are looking for a placement for these items.

	DM_DragPlacement placement = { nil, 0, true };
	Boolean hasPlacement = false;
	
	DMFastIterator iter(tempList);
	while (iter.NextElement()) {

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		
		if (CanHostElement(element, &placement)) {
			hasPlacement = true;
			break;
		}
	}
	
	// If no placement was created, the paste is rejected.
	
	if (!hasPlacement) {
		SignalCStr_("Can't place duplicated items.");
		return;
	}

	// Empty selection.
	
	mSelection->SelectNone();

	// Get the list of cloned items.
	// Place each cloned item back into the data model.
	
	iter.ResetTo(0);
	Boolean firstItem = true;

	while (iter.NextElement()) {
		
		// Find an element in the list.

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);

		// Make sure it's acceptable.

		DM_DragPlacement tempPlace = placement;
		if (CanHostElement(element, &tempPlace)) {
		
			// It is acceptable. Add it to the data model.

			HostElement(element, &placement);
			mSelection->SelectAdd(element);

			if (placement.mInsertionIndex != LArray::index_Last)
				placement.mInsertionIndex++;
			
			// If this is the first item added to the data model,
			// make sure it's visible.
			
			if (firstItem) {
				ScrollElementIntoView(element, false);
				firstItem = false;
			}
		}
	}
	
	// Commit transaction.

	trans.Commit();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** auto-scrolling

// ---------------------------------------------------------------------------
//		* ScrollElementIntoView
// ---------------------------------------------------------------------------
//	Scroll the table so that a specific element is visible. Can also select
//	the new element.

void
OVTable::ScrollElementIntoView(
	const DMElement*	inElement,
	Boolean				inSelectCell)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);

	// Find display object for this cell.

	OVItem* newDisplay = FindItemForSubElement(inElement, true);
	if (newDisplay != nil) {
	
		// Found it. Scroll it into selection.
	
		ValidateObject_(newDisplay);
		STableCell cell(FindRowForItem(newDisplay), 1);
		ScrollCellIntoFrame(cell);
		
		// If so requested, also select the cell

		if (inSelectCell) {
			UnselectAllCells();
			SelectCell(cell);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** temporary helper for pane-hosting geometry

// ---------------------------------------------------------------------------
//		* SetCellPane
// ---------------------------------------------------------------------------

void
OVTable::SetCellPane(
	const STableCell&	inCell,
	LPane*				inPane)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mTableGeometry);

	// Find table geometry.
	
	LTablePaneHostingGeometry* hostGeom = (dynamic_cast<LTablePaneHostingGeometry*>(mTableGeometry));
	ValidateObject_(hostGeom);

	// Let geometry object host the pane.

	hostGeom->SetCellPane(inCell, inPane);

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* ScrollImageBy
// ---------------------------------------------------------------------------
//	Overriden to ensure that selection indications get redrawn properly
//	as we scroll, and also to ensure that drag-target hiliting isn't
//	damaged by scrolling.

void
OVTable::ScrollImageBy(
	SInt32		inLeftDelta,
	SInt32		inTopDelta,
	Boolean		inRefresh)
{

	// Validate pointers.

	ValidateThis_();

	// Force a redraw of any selection indication.

	UpdatePort();
	
	// If dragging and the view is hilited, remove the part
	// of the drag hiliting that will be scrolled away.

	Boolean wasHilited = mIsHilited;
	
	if (wasHilited)
		UnhiliteDropArea(mDragRef);

	// Scroll the view.

	LView::ScrollImageBy(inLeftDelta, inTopDelta, inRefresh);

	// If dragging, restore the drag hilite.
	
	if (wasHilited)
		HiliteDropArea(mDragRef);

}


// ---------------------------------------------------------------------------
//		* RestorePlace											[protected]
// ---------------------------------------------------------------------------
//	Overridden to fix a bug in LPane::RestorePlace.

void
OVTable::RestorePlace(
	LStream*	inPlace)
{

	// Replacement for LPane::RestorePlace. Grrr...

	SDimension16 size;
	*inPlace >> size.width;
	*inPlace >> size.height;
	*inPlace >> mFrameLocation.h;
	*inPlace >> mFrameLocation.v;

	ResizeFrameTo(size.width, size.height, false);

	// Exact copy of LView::RestorePlace.

	*inPlace >> mImageLocation.h;
	*inPlace >> mImageLocation.v;
	
	CalcPortOrigin();					// Recalibrate internal settings
	CalcRevealedRect();
										// Restore info for SubPanes
	LArrayIterator iterator(mSubPanes);
	LPane	*theSub;
	while (iterator.Next(&theSub)) {
		theSub->RestorePlace(inPlace);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** window sizing

// ---------------------------------------------------------------------------
//		* CalcStandardSize										[protected]
// ---------------------------------------------------------------------------
//	Calculate the preferred size for this window based on our image size.

void
OVTable::CalcStandardSize(
	GReferencedWindow* inWindow)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inWindow);

	// Find our image size.

	SDimension32 hierImageSize;
	GetImageSize(hierImageSize);

	// Calculate overscan (i.e. how far are we inside the window).
	
	SDimension16 overscanSize;
	SDimension16 windowFrameSize;
	inWindow->GetFrameSize(windowFrameSize);
	overscanSize.width = windowFrameSize.width - mFrameSize.width;
	overscanSize.height = windowFrameSize.height - mFrameSize.height;

	// Set window's size.

	SDimension16 windStandardSize;
	windStandardSize.width = ((hierImageSize.width > 16000)
								? 16000
								: hierImageSize.width) + overscanSize.width;
	windStandardSize.height = ((hierImageSize.height > 16000)
								? 16000
								: hierImageSize.height) + overscanSize.height;

	inWindow->SetStandardSize(windStandardSize);
	
}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Install ourselves as the zoomer for the window.

void
OVTable::FinishCreateSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Do inherited initialization.

	LOutlineTable::FinishCreateSelf();

	// Find window.

	LView* topView = this;
	while (topView->GetSuperView() != nil) {
		ValidateObject_(topView);
		topView = topView->GetSuperView();
	}

	// Install this view as zoom controller for window.
	
	GReferencedWindow* theWindow = (dynamic_cast<GReferencedWindow*>(topView));
	if (theWindow != nil) {
		ValidateObject_(theWindow);
		theWindow->SetZoomer(this);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag origination

// ---------------------------------------------------------------------------
//		* TrackDrag												[protected]
// ---------------------------------------------------------------------------
//	The user is dragging a selected item in this table.
//	Start a Drag Manager task to handle the drag.

void
OVTable::TrackDrag(
	const STableCell&		inCell,
	const SMouseDownEvent&	inMouseDown)
{

	// Validate pointers.

	ValidateThis_();
	
	// Make sure we have a selection.
	
	if (mSelection == nil)
		return;
	ValidateObject_(mSelection.GetObject());

	// Request a non-recursive selection list.

	DMSelectionPtr copyList;
	copyList = (DMSelection*) mSelection->Clone();
	ValidateObject_(copyList.GetObject());

	copyList->EliminateRecursion();
	copyList->SortSelection();

	if (copyList->GetSelectedElements().GetCount() < 1)
		return;
	
	// Start a drag.
	
	DMDragTask drag(inMouseDown.macEvent);

	// Add all selected items to the drag.
	
	RgnHandle tempRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(tempRgn);
#else
	ValidateHandle_((Handle) tempRgn);
#endif

	Rect itemBounds;
	
	{
		DMFastIterator iter(copyList->GetSelectedElements());
		while (iter.NextElement()) {
	
			DMElement* element = iter.CurrentElement();
			ValidateObject_(element);
	
			if (MakeDragRegion(element, tempRgn, itemBounds)) {
				drag.AddDragElement(element, tempRgn, &itemBounds);
			}
		}
	}

#if PP_Target_Carbon
	ThrowIfNil_(tempRgn);
#else
	ValidateHandle_((Handle) tempRgn);
#endif
	::DisposeRgn(tempRgn);

	// Make the translucent drag image for the dragged item.

	RgnHandle imageRgn = nil;
	GWorldPtr imageGWorld = nil;
	MakeDragImage(drag.GetDragReference(), inCell, inMouseDown, imageRgn, imageGWorld);
	
	// Track the drag.
	
	drag.DoDrag();

	// If dragged to trash, delete items.
	
	if (drag.WasDraggedToTrash())
		ProcessCommand(cmd_Clear);

	// Dispose image.
	
	if (imageRgn != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(imageRgn);
#else
		ValidateHandle_((Handle) imageRgn);
#endif
		::DisposeRgn(imageRgn);
	}
	if (imageGWorld != nil) {
		ValidatePtr_((Ptr) imageGWorld);
		::DisposeGWorld(imageGWorld);
	}

}


// ---------------------------------------------------------------------------
//		* MakeDragRegion										[protected]
// ---------------------------------------------------------------------------
//	Create the outlined region of the items to be dragged.

Boolean
OVTable::MakeDragRegion(
	const DMElement*	inElement,
	RgnHandle			outGlobalDragRgn,
	Rect&				outGlobalItemBounds)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
#if PP_Target_Carbon
	ThrowIfNil_(outGlobalDragRgn);
#else
	ValidateHandle_((Handle) outGlobalDragRgn);
#endif

	// Make sure there's a display item for this element.

	OVItem* item = FindItemForSubElement(inElement, true);
	if (item != nil) {
	
		ValidateObject_(item);
	
		// There is, ask it to build a display region.
	
		STableCell cell(FindRowForItem(item), 1);
		::SetEmptyRgn(outGlobalDragRgn);
		::SetRect(&outGlobalItemBounds, 0, 0, 0, 0);
		item->MakeDragRegion(cell, outGlobalDragRgn, outGlobalItemBounds);
		
		// Now build region for subitems as well.
		// There will be a small duplication of effort here as this
		// item's outline region will get built again, but that's harmless.
		// It ensures correct behavior if MakeSubItemDragRegion
		// is overriden.
		
		MakeSubItemDragRegion(item, outGlobalDragRgn);

		// Successfully found item bounds.
		
		return true;

	}

	// No display item, no item bounds.

	return false;

}


// ---------------------------------------------------------------------------
//		* MakeSubItemDragRegion									[protected]
// ---------------------------------------------------------------------------
//	If dragging an item, create the outline region for its subitems as well.
//	If you do not drag subitems with the main item, override this method
//	with a no-op method.

void
OVTable::MakeSubItemDragRegion(
	OVItem*		inItem,
	RgnHandle	outGlobalDragRgn)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inItem);
#if PP_Target_Carbon
	ThrowIfNil_(outGlobalDragRgn);
#else
	ValidateHandle_((Handle) outGlobalDragRgn);
#endif

	// Add to the existing drag region.

	Rect ignore;
	STableCell cell(FindRowForItem(inItem), 1);
	inItem->MakeDragRegion(cell, outGlobalDragRgn, ignore);

	// If there are more subitems, continue recursion.
	
	const LArray* subItems = inItem->GetSubElementItems();
	if (subItems != nil) {

		LFastArrayIterator iter(*subItems);
		OVItem* item;

		while (iter.Next(&item)) {
			ValidateObject_(item);
			MakeSubItemDragRegion(item, outGlobalDragRgn);
		}
	}
}


// ---------------------------------------------------------------------------
//		* MakeDragImage											[protected]
// ---------------------------------------------------------------------------
//	If translucent dragging is supported on this system, make a drag image
//	for the single cell that started the drag.

void
OVTable::MakeDragImage(
	DragReference			inDragRef,
	const STableCell&		inCell,
	const SMouseDownEvent&	/* inMouseDown */,
	RgnHandle&				ioImageRgn,
	GWorldPtr&				ioImageGWorld)
{

#ifndef WINVER

	// Validate pointers.

	ValidateThis_();

	// Make sure Drag Manager supports translucent dragging.

	SInt32 response;
	OSErr err = ::Gestalt(gestaltDragMgrAttr, &response);
	if ((err != noErr) || !(response & 0x08 /* (1L << gestaltDragMgrHasImageSupport) */))
		return;

	// Okay, translucency is allowed. Build image.
	// First we find the outline item for this cell.
	
	OVItem* item = (dynamic_cast<OVItem*>(FindItemForRow(inCell.row)));
	ValidateObject_(item);

	// Allocate region for image mask.
	
	ioImageRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(ioImageRgn);
#else
	ValidateHandle_((Handle) ioImageRgn);
#endif

	// Ask item where the image will be drawn.

	Rect ignoreRect = { 0, 0, 0, 0 };
	item->MakeDragRegion(inCell, ioImageRgn, ignoreRect);
	Rect imageRect;
#if PP_Target_Carbon
	::GetRegionBounds(ioImageRgn, &imageRect);
#else
	imageRect = (**ioImageRgn).rgnBBox;
#endif

	// Build GWorld to hold the image.
	
	err = ::NewGWorld(&ioImageGWorld, 8, &imageRect, nil, nil, useTempMem);
	if (err != noErr)
		err = ::NewGWorld(&ioImageGWorld, 8, &imageRect, nil, nil, 0);
	if (err != noErr)
		return;
		
	// Save current port.
	
	CGrafPtr savePort;
	GDHandle saveDevice;
	::GetGWorld(&savePort, &saveDevice);
#if PP_Target_Carbon
	ThrowIfNil_(saveDevice);
#else
	ValidateHandle_((Handle) saveDevice);
#endif

	// Get the pixel map from the GWorld. Lock pixels before drawing.
	
	PixMapHandle imagePixMap = ::GetGWorldPixMap(ioImageGWorld);
#if PP_Target_Carbon
	ThrowIfNil_(imagePixMap);
#else
	ValidateHandle_((Handle) imagePixMap);
#endif

	// Copy what's already on the screen at this location.
	
	Rect portFrame = imageRect;
	GlobalToPortPoint(topLeft(portFrame));
	GlobalToPortPoint(botRight(portFrame));
	PortToLocalPoint(topLeft(portFrame));
	PortToLocalPoint(botRight(portFrame));
	
	BitMap *theSrc;
	BitMap *theDst;
	Rect dstRect;
#if PP_Target_Carbon
	theSrc = (BitMap*) *::GetPortPixMap(GetMacPort());
	theDst = (BitMap*) *::GetPortPixMap((GrafPtr) ioImageGWorld);
	::GetPortBounds(ioImageGWorld, &dstRect);
#else
	theSrc = &(GetMacPort()->portBits);
	theDst = &((GrafPtr) ioImageGWorld)->portBits;
	dstRect = ioImageGWorld->portRect;
#endif

	::CopyBits(
			theSrc,									// srcBits
			theDst,									// dstBits
			&portFrame,								// srcRect
			&dstRect,								// dstRect
			srcCopy,								// mode
			nil);									// maskRgn

	// Clean up after drawing.
	
	::UnlockPixels(imagePixMap);
	::SetGWorld(savePort, saveDevice);
	LView::OutOfFocus(nil);
	
	// It worked. Set the drag image.
	
	Point imageOffset = { 0, 0 };
	::SetDragImage(inDragRef, imagePixMap,
					ioImageRgn, imageOffset, kDragRegionAndImage);

#else
	
	// Translucent dragging not supported on Windows.
	
	#pragma unused (inDragRef)
	#pragma unused (inCell)
	#pragma unused (ioImageRgn)
	#pragma unused (ioImageGWorld)
	
#endif		// WINVER

}


// ===========================================================================

#pragma mark -
#pragma mark ** drag receive

// ---------------------------------------------------------------------------
//		* ChoosePlacementForElement								[protected]
// ---------------------------------------------------------------------------
//	Try to place the dragged item in the element closest to the mouse location.

Boolean
OVTable::ChoosePlacementForElement(
	DragReference		inDragRef,
	ItemReference		inItemRef,
	const DMElement*	inElement,
	DM_DragPlacement&	ioPlacement) 
{

	// Validate pointers.

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

	UInt32 width, height;
	ValidateObject_(mTableGeometry);
	mTableGeometry->GetTableDimensions(width, height);

	if (imagePt.v < 0)
		imagePt.v = 0;
	if (imagePt.h < 0)
		imagePt.h = 0;
	if (imagePt.v >= static_cast<SInt32>(height))
		imagePt.v = height - 1;
	if (imagePt.h >= static_cast<SInt32>(width))
		imagePt.h = width - 1;

	// If we hit a cell with a display item, let the item handle placement.

	STableCell hitCell;
	if (GetCellHitBy(imagePt, hitCell)) {

		OVItem* item = (dynamic_cast<OVItem*>(FindItemForRow(hitCell.row)));
		ValidateObject_(item);
		return item->ChooseDragPlacement(inDragRef, inItemRef,
								inElement, ioPlacement, dragMouse, hitCell);

	}

	// Didn't hit a cell or item didn't want it, can't accept element.
	
	return false;

}


// ---------------------------------------------------------------------------
//		* EnterDropArea											[protected]
// ---------------------------------------------------------------------------
//	Overriden to remove selection drawing and replace it with placement
//	indication.

void
OVTable::EnterDropArea(
	DragReference		inDragRef,
	Boolean				inDragHasLeftSender)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// Save drag reference.
	
	mCurrentDragRef = inDragRef;

	// Make selection appear empty so we can "select" the target item
	// as mouse moves over it.

	mDragSelection = (DMSelection*) mSelection->Clone();
	ValidateObject_(mDragSelection.GetObject());
	
	mSelection->SelectNone();
	UpdatePort();
	
	// Make the selection indicators appear active, even if the window
	// isn't the front window.
	
	SuperActivate();
	
	// Draw blue drag-target indicator.

	DMDropArea::EnterDropArea(inDragRef, inDragHasLeftSender);

}


// ---------------------------------------------------------------------------
//		* LeaveDropArea											[protected]
// ---------------------------------------------------------------------------
//	Overriden to remove placement drawing and restore selection indication.

void
OVTable::LeaveDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mDragSelection.GetObject());

	// Remove drag-target indicator.

	DMDropArea::LeaveDropArea(inDragRef);
	
	// Forget current drag reference.
	
	mCurrentDragRef = 0;
	
	// Remove insertion point indicator.
	
	RefreshPlacementIndicator(mPreviousPlacement, true, true);
	RefreshPlacementIndicator(mCurrentPlacement, true, true);
	
	// Restore original selection. Select none for now, and redraw
	// original selection on next update event (unless something
	// changes it between now and then).
	
	mSelection->SelectNone();
	UpdatePort();
	
	if (!mSuperView->IsActive())
		SuperDeactivate();
	
	mSelection->CopySelectionList(mDragSelection);
	mDragSelection = nil;
	
}


// ---------------------------------------------------------------------------
//		* InsideDropArea										[protected]
// ---------------------------------------------------------------------------
//	Overriden to draw placement indicator and selection.

void
OVTable::InsideDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// Auto-scroll and set cursor.

	DMDropArea::InsideDropArea(inDragRef);

	// If target item has changed, update "selection."
	
	Boolean needsUpdate = false;
		
	if (mPreviousPlacement.mHostElement != mCurrentPlacement.mHostElement) {

		// Find the appropriate object to highlight.

		DMElement* selectMe = nil;
		
		if (mCurrentPlacement.mHostElement != nil) {

			ValidateObject_(mCurrentPlacement.mHostElement);
			if (FindItemForSubElement(mCurrentPlacement.mHostElement, true) != nil)
				selectMe = mCurrentPlacement.mHostElement;
			else
				selectMe = mCurrentPlacement.mHostElement->GetAffectedObject();
		}

		// Update the temporary selection.

		if (selectMe != nil) {
			ValidateObject_(selectMe);
			mSelection->SelectSimple(selectMe);
		}
		else
			mSelection->SelectNone();

		needsUpdate = true;
		
	}

	// Redraw the insertion point cursor if the placement has changed
	// or the cursor-blink time has elapsed.

	if ((mPreviousPlacement.mHostElement != mCurrentPlacement.mHostElement) ||
	 	(mPreviousPlacement.mInsertionIndex != mCurrentPlacement.mInsertionIndex)) {
		
		// Placement has changed, invalidate old and new placements.
		
		RefreshPlacementIndicator(mPreviousPlacement, true, true);
		RefreshPlacementIndicator(mCurrentPlacement, true, true);
		needsUpdate = true;
		
		// Restart insertion point blink.
		
		mInsertionPointVisible = true;
		mInsertionPointNextTick = ::TickCount() + ::GetCaretTime();
		
	}
	else if (::TickCount() > mInsertionPointNextTick) {
	
		// Placement hasn't changed, but cursor blink time has elapsed.
	
		mInsertionPointVisible = !mInsertionPointVisible;
		mInsertionPointNextTick = ::TickCount() + ::GetCaretTime();
		
		RefreshPlacementIndicator(mCurrentPlacement, false, true);
		needsUpdate = true;
		
	}

	// Redraw the port so this selection is visible.

	if (needsUpdate)
		UpdatePort();

}


// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Be sure to redraw drag-target highlighting and/or placement indicator
//	when the port is updated during a drag.

void
OVTable::DrawSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Draw the table.

	LOutlineTable::DrawSelf();

	// If we're tracking a drag, we need to do some special drawing.
	
	if (mCurrentDragRef != 0) {
		
		// Redraw insertion point.
		
		DrawPlacementIndicator(mCurrentPlacement);

		// Redraw drag-target highlighting.
		
		if (mIsHilited) {
			StColorPenState::Normalize();
			::OffsetRgn(mUpdateRgn, mPortOrigin.h, mPortOrigin.v);
			::UpdateDragHilite(mCurrentDragRef, mUpdateRgn);
			::OffsetRgn(mUpdateRgn, -mPortOrigin.h, -mPortOrigin.v);
		}
	}

}


// ---------------------------------------------------------------------------
//		* DrawPlacementIndicator								[protected]
// ---------------------------------------------------------------------------
//	Called by OVTable::DrawSelf to redraw the placement indicator while
//	tracking a drag.

void
OVTable::DrawPlacementIndicator(
	const DM_DragPlacement&	inPlacement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Make sure we have a valid placement.

	Point displayLoc;
	if (!CalcPlacementDisplayLoc(inPlacement, displayLoc))
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
		::LineTo(mFrameSize.width - 6, displayLoc.v);
	}
}


// ---------------------------------------------------------------------------
//		* RefreshPlacementIndicator								[protected]
// ---------------------------------------------------------------------------
//	Overriden to trigger a refresh of the placement indicator.

void
OVTable::RefreshPlacementIndicator(
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
	
	// Refresh the small triangle just to left of placement.
	
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

	// Refresh insertion point.

	if (inRefreshInsertionPoint) {
		Rect invalRect = { displayLoc.v, displayLoc.h + 4,
						   displayLoc.v + 1, mFrameSize.width - 5 };
		CalcPortFrameRect(invalRect);
		InvalPortRect(&invalRect);
	}

}


// ---------------------------------------------------------------------------
//		* CalcPlacementDisplayLoc								[protected]
// ---------------------------------------------------------------------------
//	Calculate the place where the insertion point should be drawn.

Boolean
OVTable::CalcPlacementDisplayLoc(
	const DM_DragPlacement&	inPlacement,
	Point&					outDisplayLoc)
{

	// Validate pointers.

	ValidateThis_();

	// Look for excuses not to make a placement. A host that's not
	// a container is a good excuse.
	
	DMContainerMixin* container = (dynamic_cast<DMContainerMixin*>(
									const_cast<DMElement*>(inPlacement.mHostElement)));

	if (container == nil)
		return false;

	ValidateObject_(container);
	
	// Inserting at index 0 is also a good excuse.
	
	if (inPlacement.mInsertionIndex < 1)
		return false;
	
	// If we're inserting at the end, that's a special case.
	
	OVItem* item = nil;
	SInt16 horizPlace = 0;
	Boolean belowCell = false;

	if (inPlacement.mInsertionIndex <= container->GetSubElements().GetCount()) {
	
		// Inserting before end of list, this is relatively easy to compute.
	
		belowCell = false;
		DMElement* element;
		container->GetSubElements().FetchItemAt(inPlacement.mInsertionIndex, &element);
		item = FindItemForSubElement(element, true);
		if (item == nil)
			return false;
			
		ValidateObject_(item);

		horizPlace = item->mLeftEdge;
	
	}
	else if (container->GetSubElements().GetCount() == 0) {
		
		// Inserting into empty list, also easy to compute.
		
		belowCell = true;
		
		DMElement* element = inPlacement.mHostElement;
		while (1) {

			ValidateObject_(element);

			item = FindItemForSubElement(element, true);
			if (item != nil)
				break;

			element = element->GetSuperElement();
			if (element == nil)
				return false;
			
		}
		
		ValidateObject_(item);
		horizPlace = item->mLeftEdge + item->mIndentSize;
	
	}
	else {
	
		// Inserting at the end of list, this is tricky.
		
		belowCell = true;

		// Find the last item in the list.
		
		DMElement* element;
		container->GetSubElements().FetchItemAt(container->GetSubElements().GetCount(), &element);
		ValidateObject_(element);
		
		item = FindItemForSubElement(element, true);
		if (item == nil)
			return false;

		ValidateObject_(item);
		horizPlace = item->mLeftEdge;

		// From here, we traverse the item list sequentially
		// looking for the first item which is at a nesting level
		// above this item. Yes, in case you were wondering,
		// this is an ugly hack.
		
		TableIndexT row = item->FindRowForItem();
		while (++row <= mRows) {
			
			OVItem* nextItem = (OVItem*) FindItemForRow(row);
			ValidateObject_(nextItem);
			
			if (nextItem == nil)
				break;
			if (nextItem->mLeftEdge < horizPlace)
				break;
			
			item = nextItem;
			
		}
	}

	// Found the cell we should be drawing near, figure placement.

	if (item == nil)
		return false;					// last minute sanity check
		
	ValidateObject_(item);
	
	STableCell cell(FindRowForItem(item), 1);
	Rect cellBounds;
	if (!GetLocalCellRect(cell, cellBounds))
		return false;
	
	if (belowCell)
		outDisplayLoc.v = cellBounds.bottom - 1;
	else
		outDisplayLoc.v = cellBounds.top;
	
	outDisplayLoc.h = horizPlace;
	return true;
	
}


// ---------------------------------------------------------------------------
//		* MakePlacementTriangleRgn								[protected]
// ---------------------------------------------------------------------------
//	Given the insertion point location, create a region that is the
//	triangle indicator to the left of the insertion point.

RgnHandle
OVTable::MakePlacementTriangleRgn(
	Point	inIndicatorPoint)
{

	// Validate pointers.

	ValidateThis_();

	// Create a small triangle just to left of placement.
	// Caller will fill it or invalidate it.
	
	const SInt16 trHalfSize = 3;

	RgnHandle trRegion = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(trRegion);
#else
	ValidateHandle_((Handle) trRegion);
#endif

	::OpenRgn();
	::MoveTo(inIndicatorPoint.h - trHalfSize, inIndicatorPoint.v - trHalfSize + 1);
	::LineTo(inIndicatorPoint.h, inIndicatorPoint.v);
	::LineTo(inIndicatorPoint.h - trHalfSize, inIndicatorPoint.v + trHalfSize);
	::LineTo(inIndicatorPoint.h - trHalfSize, inIndicatorPoint.v - trHalfSize + 1);
	::CloseRgn(trRegion);

	return trRegion;

}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard/drag-and-drop primitives

// ---------------------------------------------------------------------------
//		* CanHostElement										[protected]
// ---------------------------------------------------------------------------
//	Return true if the new element can be hosted at the specified location.

Boolean
OVTable::CanHostElement(
	DMElement*			inElement,
	DM_DragPlacement*	ioPlacement) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	ValidateObject_(mElement.GetObject());

	// See if our main element will accept this element.

	if (mElement->CanHostElement(inElement, ioPlacement)) {
	
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
	else
		return false;
	
}


// ---------------------------------------------------------------------------
//		* HostElement											[protected]
// ---------------------------------------------------------------------------
//	Host the new element at the specified location.

void
OVTable::HostElement(
	DMElement*			inElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Route hosting request to data model.

	if ((ioPlacement != nil) && (ioPlacement->mHostElement != nil)) {
		ValidateObject_(ioPlacement->mHostElement);
		ioPlacement->mHostElement->HostElement(inElement, ioPlacement);
	}
}