// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVItem.cpp					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 05/31/96
//     $Date: 2006/01/18 01:32:43 $
//	$History: OVItem.cpp $
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:35
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 12  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 01/28/97   Time: 19:57
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Moved multi-line text support to LOutlineItem.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 12/12/96   Time: 17:36
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added CollectTentativeChanges method so Save is enabled when IPE is
//	changing. (Bug fix #1075.)
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:16
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added override of Expand which collapses the item if an exception is
//	thrown.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/19/96   Time: 15:39
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed a bug in StartInPlaceEdit. If the window was brought to front by
//	editing, the edit field wasn't installed in the correct location in the
//	command hierarchy.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/04/96   Time: 15:02
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Updated IPEChanged to show validation failure alert.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 10/31/96   Time: 18:18
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed placement of modified-flag indicator.
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 10/24/96   Time: 00:26
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Fixed RefreshRow.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 19:12
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added RefreshRow and RefreshCell methods. Improvements to in-place editing.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/16/96   Time: 23:35
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added double-click as a shortcut for property inspector.
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

#include "OVItem.h"

	// Core : Data model : Exceptions
#include "DMValidationException.h"

	// Core : Data model : Undo/redo
#include "StDMAttributeSetter.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Property inspector
#include "UPropertyInspector.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>

	// PowerPlant : CA : Text editing
#include <LInPlaceEditField.h>



// ===========================================================================
//	MyDisableUndoCommand
// ===========================================================================

#pragma mark *** MyDisableUndoCommand ***

class MyDisableUndoCommand : public LAttachment
{
public:
							MyDisableUndoCommand();
	virtual					~MyDisableUndoCommand();

protected:
	virtual void			ExecuteSelf(
									MessageT			inMessage,
									void*				ioParam);
private:
	bool didExecute;
};

// ---------------------------------------------------------------------------
//		* MyDisableUndoCommand
// ---------------------------------------------------------------------------

MyDisableUndoCommand::MyDisableUndoCommand()
	: LAttachment(msg_CommandStatus), didExecute(false)
{
}

// ---------------------------------------------------------------------------
//		* ~MyDisableUndoCommand
// ---------------------------------------------------------------------------

MyDisableUndoCommand::~MyDisableUndoCommand()
{
}

// ---------------------------------------------------------------------------
//		* ExecuteSelf
// ---------------------------------------------------------------------------

void
MyDisableUndoCommand::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{
	SetExecuteHost(true);
	if (inMessage == msg_CommandStatus)
	{
		SCommandStatus* cmdStatus = (SCommandStatus*) ioParam;
		if (cmdStatus->command == cmd_Undo)
		{
			// If this is the first time we're checking the status
			// of cmd_Undo, then we'll always disable it.  This prevents
			// the user from being able to undo a previous resource id change
			// when they just started a new in-place edit session.  Bug
			// WB1-23297 caused a crash when a user started a new in-place
			// edit session and then hit Undo to undo a resource id change
			// because the in-place edit session was never terminated before
			// the undo fired.  Once we've disabled this once, we'll allow
			// cmd_Undo to be enabled so the user can undo typing in the
			// current in-place edit session.
			
			if (not didExecute)
			{
				cmdStatus->enabled = false;
				SetExecuteHost(false);
				didExecute = true;
			}
		}
	}
}

// ===========================================================================

#pragma mark *** OVItem ***

// ---------------------------------------------------------------------------
//		* OVItem()
// ---------------------------------------------------------------------------
//	Constructor

OVItem::OVItem()

: OVListener(this)

{
}


// ---------------------------------------------------------------------------
//		* ~OVItem
// ---------------------------------------------------------------------------
//	Destructor

OVItem::~OVItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** disclosure triangle

// ---------------------------------------------------------------------------
//		* CanExpand
// ---------------------------------------------------------------------------
//	Returns true if there are subelements that can be displayed
//	(i.e. a disclosure triangle is required).

Boolean
OVItem::CanExpand() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// If our element is not a container, we can't expand.
	
	if (mContainer == nil)
		return false;
	ValidateObject_(mContainer.GetObject());	

	// We have a container, see if it has any subelements.

	DMElement* container = mContainer;
	DMContainerMixin* mixin = (dynamic_cast<DMContainerMixin*>(container));
	ValidateObject_(mixin);
	
	if (mixin != nil)
		return mixin->GetSubElements().GetCount() > 0;
	else
		return false;

}


// ---------------------------------------------------------------------------
//		* Expand
// ---------------------------------------------------------------------------
//	Overridden to ensure that the expansion is reversed if it fails.

void
OVItem::Expand()
{
	try {
		LOutlineItem::Expand();
	}
	catch(...) {
		Collapse();
		throw;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** modified flag

// ---------------------------------------------------------------------------
//		* IsModified
// ---------------------------------------------------------------------------

Boolean
OVItem::IsModified() const
{

	// Validate pointers.

	ValidateThis_();
	
	// If no element, we're not modified. (This shouldn't happen.)

	if (mElement == nil)
		return false;
	else {
		ValidateObject_(mElement.GetObject());
		return mElement->IsModified();
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** subelement accessors

// ---------------------------------------------------------------------------
//		* GetSubElementItems
// ---------------------------------------------------------------------------
//	Return the subelement array.

const LArray*
OVItem::GetSubElementItems() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return sub item array.
	
	return mSubItems;

}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* GetCellForAttribute
// ---------------------------------------------------------------------------
//	Given an attribute, find a cell that will display it (if any).

TableIndexT
OVItem::GetCellForAttribute(
	const DMAttribute*	inAttribute) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inAttribute);
	ValidateObject_(const_cast<OVItem*>(this)->mOVTable);

	// Get the attribute's key. We'll match by key.

	DM_AttributeKey key = inAttribute->GetKey();

	// Iterate over the row's cells, looking for this key.

	TableIndexT rows, cols;
	mOVTable->GetTableSize(rows, cols);
		
	for (TableIndexT col = 1; col <= cols; col++) {
		if (GetAttributeKeyForCell(col) == key)
			return col;
	}
	
	// Didn't find it.
	
	return 0;

}


// ---------------------------------------------------------------------------
//		* GetAttributeForCell
// ---------------------------------------------------------------------------
//	Given a cell index, return the attribute that's displayed by
//	that cell (if any).

DMAttribute*
OVItem::GetAttributeForCell(
	TableIndexT		inColumn) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mElement.GetObject());

	// Look for an attribute by key. Use GetAttributeKeyForCell
	// to identify the key.
	
	return mElement->FindAttributeByKey(GetAttributeKeyForCell(inColumn));

}


// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Given a cell index, return the attribute key that's displayed by
//	that cell (if any). Should be overriden.

DM_AttributeKey
OVItem::GetAttributeKeyForCell(
	TableIndexT		/* inColumn */) const
{
	return 0;		// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* RefreshRow
// ---------------------------------------------------------------------------
//	Trigger a redraw of the entire row.

void
OVItem::RefreshRow()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOutlineTable);

	// Get frame of table.
	
	Rect frame;
	mOutlineTable->CalcLocalFrameRect(frame);
	
	STableCell cell(FindRowForItem(), 1);
	SInt32 left, top, right, bottom;
	mOutlineTable->GetImageCellBounds(cell, left, top, right, bottom);
	
	SPoint32 imageLoc;
	mOutlineTable->GetImageLocation(imageLoc);
	
	left = imageLoc.h;
	right = imageLoc.h + (frame.right - frame.left);
	
	// See if any of this row is visible.
	
	Boolean	insideFrame = mOutlineTable->ImageRectIntersectsFrame(left, top, right, bottom);
	if (insideFrame) {

		SPoint32 imagePt;
		imagePt.h = left;
		imagePt.v = top;
		
		Rect refreshRect;
		mOutlineTable->ImageToLocalPoint(imagePt, topLeft(refreshRect));
		
		refreshRect.right = refreshRect.left + (right - left);
		refreshRect.bottom = refreshRect.top + (bottom - top);
		
		mOutlineTable->FocusDraw();
		mOutlineTable->InvalPortRect(&refreshRect);
	
	}
}


// ---------------------------------------------------------------------------
//		* RefreshCell
// ---------------------------------------------------------------------------
//	Trigger a redraw of a single cell.

void
OVItem::RefreshCell(
	TableIndexT		inColumn)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOutlineTable);

	// Ask the table to refresh this cell.
	
	STableCell cell(FindRowForItem(), inColumn);
	mOutlineTable->RefreshCell(cell);
	
}


// ---------------------------------------------------------------------------
//		* MakeDragRegion
// ---------------------------------------------------------------------------
//	Overriden to ensure that a drag region is formed even when there is
//	no selection in the first column (as the case in the menu editor).

void
OVItem::MakeDragRegion(
	const STableCell&	inCell,
	RgnHandle			outGlobalDragRgn,
	Rect&				outGlobalItemBounds)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Find out how many columns are in the table.
	
	TableIndexT rows, cols;
	mOVTable->GetTableSize(rows, cols);
	
	// Overridden to substitute the first cell where a selection is drawn.
	
	STableCell cell = inCell;
	cell.col = 1;
	
	while (cell.col <= cols) {
	
		SOutlineDrawContents drawInfo;
		ResetDrawContents(cell, drawInfo);
		GetDrawContentsSelf(cell, drawInfo);
		
		if (drawInfo.outShowSelection) {
			LOutlineItem::MakeDragRegion(cell, outGlobalDragRgn, outGlobalItemBounds);
			return;
		}
		
		cell.col++;
	}

	// No selection shown anywhere, just choose the cell we were originally asked for.

	LOutlineItem::MakeDragRegion(inCell, outGlobalDragRgn, outGlobalItemBounds);

}


// ===========================================================================

#pragma mark -
#pragma mark ** in-place editing

// ---------------------------------------------------------------------------
//		* StartInPlaceEdit
// ---------------------------------------------------------------------------
//	Overriden to set the a value message for the new edit field. We listen
//	for that message and return the value to an attribute when done.

void
OVItem::StartInPlaceEdit(
	const STableCell&	inCell)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Make sure there is only one item selected.
	
	DMSelection* selection = mOVTable->GetSelection();
	if (selection != nil) {
		ValidateObject_(selection);
		if (selection->GetSelectedElements().GetCount() > 1)
			return;
	}
	
	// Make sure this window is on top.
	
	LWindow* window = LWindow::FetchWindowObject(GetWindowFromPort(mOVTable->GetMacPort()));
	ValidateObject_(window);
	window->Select();
	window->Activate();
	
	// Create the edit field.
	
	LEditableOutlineItem::StartInPlaceEdit(inCell);
	ValidateObject_(mEditField);

	// Set up the listener relationship with this edit field.

	mEditField->SetValueMessage(msg_IPEChanged);
	mEditField->AddListener(this);
	mEditField->AddAttachment(new MyDisableUndoCommand());

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for in-place editor value change messages in addition to
//	data model messages.

void
OVItem::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();
	
	// If in-place editing changed, we need to record the new value.

	if (inMessage == msg_IPEChanged)
		IPEChanged();
	else
		OVListener::ListenToMessage(inMessage, ioParam);

}


// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	An attribute's value has changed. If it's an attribute that we're displaying,
//	make sure the display is redrawn.

void
OVItem::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);

	// See if we're displaying this attribute.
	// If so, redraw the cell.

	TableIndexT col = GetCellForAttribute(inMessage->mAttribute);
	if (col != 0)
		RefreshCell(col);

}


// ---------------------------------------------------------------------------
//		* ModifiedFlagChanged									[protected]
// ---------------------------------------------------------------------------
//	If the modified flag has changed, trigger a redraw of the flag.

void
OVItem::ModifiedFlagChanged(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// If modified flag changed, trigger a redraw.

	if (inElement == mElement)
		RefreshModifiedFlag();

}


// ---------------------------------------------------------------------------
//		* ContainerListChanged									[protected]
// ---------------------------------------------------------------------------

void
OVItem::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();

	// See if this container is the one that's adding the node.
	// If not, ignore the message.

	if ((mContainer != nil) && (inMessage->mContainer == mContainer)) {

		ValidateObject_(mContainer.GetObject());
		
		// Okay, it's our node. See if we need to refresh the
		// disclosure triangle.
		
		DMContainerMixin* cont = (dynamic_cast<DMContainerMixin*>(inMessage->mContainer));
		ValidateObject_(cont);

		if (cont->GetSubElements().GetCount() < 2)
			RefreshDisclosureTriangle();

	}

	// Otherwise we can let the listener handle it.

	OVListener::ContainerListChanged(inMessage);
	
}


// ---------------------------------------------------------------------------
//		* CollectTentativeChanges								[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that Save command is enabled when user has started
//	typing in an in-place edit field.

void
OVItem::CollectTentativeChanges(
	DM_CollectTentativeChanges*		inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See if the collect-changes message relates to our element.
	
	if (inMessage->mElement == mElement) {
	
		// It does, see if we have an edit field.
		
		if (mEditField != nil) {
		
			// See if IPE text differs from attribute's text.
			
			LStr255 ipeText;
			mEditField->GetDescriptor(ipeText);
			
			DMAttribute* attr = GetAttributeForCell(sInPlaceEditCell.col);
			ValidateObject_(attr);
			
			LStr255 attrText;
			attr->GetTextValue(attrText);
			
			if (attrText.CompareTo(ipeText) != 0)
				inMessage->mIsChanged = true;
		
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** subitem list

// ---------------------------------------------------------------------------
//		* ExpandSelf											[protected]
// ---------------------------------------------------------------------------
//	Create items for all of the subelements (if any).

void
OVItem::ExpandSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Create an outline item for each subelement.

	CreateItemsForSubElements();

}


// ---------------------------------------------------------------------------
//		* AddSubItem											[protected]
// ---------------------------------------------------------------------------
//	Notification from the table that an item has been added as a subitem
//	of this item. If we're keeping a sorted list, make sure the item goes in
//	the list at the correct index.

void
OVItem::AddSubItem(
	LOutlineItem*	inSubItem,
	LOutlineItem*	inAfterItem)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSubItem);
	if (inAfterItem != nil)
		ValidateObject_(inAfterItem);

	// Call inherited.

	LOutlineItem::AddSubItem(inSubItem, inAfterItem);

	// Record position of new item in sorted subitems array.

	if (mSubElements != nil) {
		ValidateObject_(mSubElements);
		OVItem* item = (dynamic_cast<OVItem*>(inSubItem));
		ValidateObject_(item);
		mSubElements->InsertItemAt(1, item->GetElement());
	}
}


// ---------------------------------------------------------------------------
//		* RemoveSubItem											[protected]
// ---------------------------------------------------------------------------
//	Notification from the table that a subitem of this item has been removed.
//	Keep our subelements list in sync.

void
OVItem::RemoveSubItem(
	LOutlineItem*	inSubItem)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSubItem);
	
	// If we're keeping a list of sorted subelements,
	// delete this item from it.
	
	if (mSubElements != nil) {
		ValidateObject_(mSubElements);
		OVItem* item = (dynamic_cast<OVItem*>(inSubItem));
		ValidateObject_(item);
		mSubElements->RemoveItem(item->GetElement());
	}

	// Call inherited's remove.

	LOutlineItem::RemoveSubItem(inSubItem);

}


// ===========================================================================

#pragma mark -
#pragma mark ** property inspector

// ---------------------------------------------------------------------------
//		* DoubleClick											[protected]
// ---------------------------------------------------------------------------
//	Overriden to trigger a property inspector on the double-clicked item.

void
OVItem::DoubleClick(
	const STableCell&			/* inCell */,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);

	// If we have a selection, use it to trigger inspection of the first selected item.
	
	DMSelection* selection = mOVTable->GetSelection();
	if (selection != nil) {
		ValidateObject_(selection);
		UPropertyInspector::InspectSelection(selection, mOVTable);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** in-place editing

// ---------------------------------------------------------------------------
//		* IPEChanged											[protected]
// ---------------------------------------------------------------------------
//	Notification that the in-place editor's value has changed. Make sure
//	that the data model is updated to reflect this change.

void
OVItem::IPEChanged()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mEditField);
	ValidateObject_(mOVTable);

	// In-place editor's value has changed. Pass it along to data model.
	
	LStr255 ipeText;
	mEditField->GetDescriptor(ipeText);
	
	DMAttribute* attr = GetAttributeForCell(sInPlaceEditCell.col);
	ValidateObject_(attr);

	try {
	
		// Try setting the value of the attribute.
	
		StDMAttributeSetter setter(attr);
		setter.SetUndoerHost(mOVTable);
		setter.SetSelection(mOVTable->GetSelection());
		setter.SetTextValue(ipeText);

	}
	catch(const DMValidationException& error) {
	
		// Value was unacceptable. Show dialog explaining why.
	
		error.ShowValidationFailedAlert();

	}
	catch(...) {
	
		SignalCStr_("Unexpected exception type.");

	}
}


// ---------------------------------------------------------------------------
//		* PrepareDrawContents									[protected]
// ---------------------------------------------------------------------------
//	Overriden to disable in-place editing for disabled elements.

void
OVItem::PrepareDrawContents(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mElement.GetObject());
	
	// Disable in-place editing if element is disabled.

	if (!mElement->IsEnabled())
		ioDrawContents.outCanDoInPlaceEdit = false;

	// Now prepare draw contents as usual.

	LEditableOutlineItem::PrepareDrawContents(inCell, ioDrawContents);

}


// ===========================================================================

#pragma mark -
#pragma mark ** drag-receive behaviors

// ---------------------------------------------------------------------------
//		* ChooseDragPlacement									[protected]
// ---------------------------------------------------------------------------
//	The mouse is in this cell during a drag-receive. We get first crack at
//	deciding where to place the dragged item.

Boolean
OVItem::ChooseDragPlacement(
	DragReference			inDragRef,
	ItemReference			inItemRef,
	const DMElement*		inElement,
	DM_DragPlacement&		ioPlacement,
	const Point&			inDragMouseLocal,
	const STableCell&		inCell)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mElement.GetObject());
	ValidateObject_(inElement);
	
	// Find out what we're supposed to draw.

	SOutlineDrawContents drawInfo;
	GetDrawContents(inCell, drawInfo);

	// If there's an icon, hit test it.

	Boolean hitContent = false;	
	if (drawInfo.outHasIcon && (drawInfo.outIconSuite != nil)) {
		if (::PtInIconSuite(inDragMouseLocal, &drawInfo.prIconFrame,
							drawInfo.outIconAlign, drawInfo.outIconSuite))
			hitContent = true;
	}
	
	// If there's text, hit test it.

	if (::PtInRect(inDragMouseLocal, &drawInfo.prTextFrame))
		hitContent = true;

	// Now try deciding placement. If we hit the content, we first ask our element
	// if it wants the new element on its own. If not, we always try placing as a
	// subelement, regardless of the mouse location.
	
	if (hitContent) {
		ioPlacement.mInsertionIndex = 1;
		if (mElement->CanHostElement(inElement, &ioPlacement))
			return true;
		else
			return ChooseDragPlacementAsSub(inDragRef, inItemRef, inElement,
											ioPlacement, inDragMouseLocal);
	}

	// Didn't hit content, we try placing this item as a sub only if
	// the mouse is in the bottom half of the cell.

	SInt16 midVert = (drawInfo.ioCellBounds.top + drawInfo.ioCellBounds.bottom) >> 1;

	if ((inDragMouseLocal.v >= midVert) && 
	   ((mSuperItem == nil) || (inDragMouseLocal.h >= mLeftEdge))) {
		return ChooseDragPlacementAsSub(inDragRef, inItemRef, inElement,
										ioPlacement, inDragMouseLocal);
	}
	else {
		OVItem* superItem = (dynamic_cast<OVItem*>(mSuperItem));
		if (superItem != nil) {
			ValidateObject_(superItem);
			return superItem->ChooseDragPlacementAsSub(inDragRef, inItemRef,
										inElement, ioPlacement, inDragMouseLocal);
		}
		if (mOVTable != nil) {
			ValidateObject_(mOVTable);
			return mOVTable->ChooseDragPlacementAsSub(inDragRef, inItemRef,
										inElement, ioPlacement, inDragMouseLocal);
		}
		return false;
	}	

}


// ===========================================================================

#pragma mark -
#pragma mark ** modified flag drawing

// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	Draw the modified flag as a row adornment.

void
OVItem::DrawRowAdornments(
	const Rect&		/* inLocalRowRect */)
{

	// Validate pointers.

	ValidateThis_();

	// The only adornment drawn at this level is the modified flag.

	DrawModifiedFlag();

}


// ---------------------------------------------------------------------------
//		* DrawModifiedFlag										[protected]
// ---------------------------------------------------------------------------
//	Draw the modified flag if the item is modified.

void
OVItem::DrawModifiedFlag()
{

	// Validate pointers.

	ValidateThis_();

	// If this item isn't modified, there's nothing to draw.

	if (!IsModified())
		return;
	
	// Okay, it's modified. Find the location of the modified flag.
	
	Rect ovalRect;
	if (!CalcModifiedFlagRect(ovalRect))
		return;
	
	// Got the location. Draw a small black circle in that location.

	StColorPenState::Normalize();
	::PaintOval(&ovalRect);
	
}


// ---------------------------------------------------------------------------
//		* RefreshModifiedFlag									[protected]
// ---------------------------------------------------------------------------
//	Cause the modified flag's area to be redrawn.

void
OVItem::RefreshModifiedFlag()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOutlineTable);

	// Find location of modified flag and redraw it.

	if (mOutlineTable != nil) {
		if (mOutlineTable->FocusDraw()) {
			Rect frame;
			if (CalcModifiedFlagRect(frame)) {
				mOutlineTable->CalcPortFrameRect(frame);
				mOutlineTable->InvalPortRect(&frame);
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* CalcModifiedFlagRect									[protected]
// ---------------------------------------------------------------------------
//	Calculate the enclosing rectangle for the modified flag.

Boolean
OVItem::CalcModifiedFlagRect(
	Rect&	outModFlagRect)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOutlineTable);

	// Find first column of this row.

	STableCell cell;
	cell.row = FindRowForItem();
	cell.col = 1;
	
	if (cell.row == 0)
		return false;
	if (!mOutlineTable->GetLocalCellRect(cell, outModFlagRect))
		return false;

	// Place modified flag just to the left of the "first level indent"
	// position.
	
	SInt16 mid = (outModFlagRect.top + outModFlagRect.bottom) >> 1;
	outModFlagRect.top = mid - 1;
	outModFlagRect.bottom = mid + 3;
	outModFlagRect.left += mOutlineTable->GetFirstLevelIndent() - 6;
	outModFlagRect.right = outModFlagRect.left + 4;
	return true;
	
}