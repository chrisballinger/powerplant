// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SLStringListTable.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/02/96
//     $Date: 2006/01/18 01:33:52 $
//	$History: SLStringListTable.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 03/17/97   Time: 18:11
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Fixed problems with selection and creating a new item. (Bug fix #1251.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:09
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/28/97   Time: 20:03
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Added support for multi-line strings.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/17/96   Time: 16:40
//	Updated in $/Constructor/Source files/Editors/String list
//	Added code to make string cells more sensitive for in-place editing.
//	(Bug fix #1133.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:44
//	Updated in $/Constructor/Source files/Editors/String list
//	Changed NewStringItem to trigger in-place editing when a new string is
//	created.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/04/96   Time: 16:52
//	Created in $/Constructor/Source files/Editors/String list
//	Fixed a commenting error.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "SLStringListTable.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Attribute classes
#include "DMListAttribute.h"
#include "DMStringAttribute.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMEditAttachment.h"

#include "DMIterator.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_SLEditMenuStrings		= 5220;

// ===========================================================================

#pragma mark *** SLStringListTable ***

// ---------------------------------------------------------------------------
//		* SLStringListTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

SLStringListTable::SLStringListTable(
	LStream*	inStream)

: OVTable(inStream, 17, true)

{

	// Establish item factory for table.

	SetItemFactory(new SLItemFactory);

	// Set edit strings.
	
	DMDropArea::SetEditMenuStrings(STR_SLEditMenuStrings);

}


// ---------------------------------------------------------------------------
//		* ~SLStringListTable
// ---------------------------------------------------------------------------
//	Destructor

SLStringListTable::~SLStringListTable()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu command status

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Overriden to enable the New String command.

void
SLStringListTable::FindCommandStatus(
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
	
		// New dynamic item: Enable iff a dynamic list item is selected.
	
		case cmd_NewItem:
		case cmd_NewItemDialog:
			ValidateObject_(mElement.GetObject());
			outEnabled = mElement->IsEnabled();
			break;
		
		// Otherwise, let supercommander handle it.
		
		default:
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);

	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Respond to the New String command.

Boolean
SLStringListTable::ObeyCommand(
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
			NewStringItem();
			return true;
	
		case cmd_Duplicate:
			DuplicateStrings();
			return true;

		// Otherwise: Let supercommander handle it.
	
		default:
			return OVTable::ObeyCommand(inCommand, ioParam);

	}
}


// ---------------------------------------------------------------------------
//		* NewStringItem
// ---------------------------------------------------------------------------
//	Create a new string in the string list.

void
SLStringListTable::NewStringItem()
{

	// Validate pointers.

	ValidateThis_();

	// See where the dynamic item is.

	DMListAttribute* attr = (dynamic_cast<DMListAttribute*>(mElement.GetObject()));;
	ValidateObject_(attr);

	// We have a valid list item. Set up a transaction for the new item.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_NewItem);

	// Duplicate prototype item.
	
	DMAttributePtr newItem;
	newItem = (DMAttribute*) attr->GetPrototype()->Clone();
	ValidateObject_(newItem.GetObject());
	
	// JCD 980619 - find the location where to insert. If there is
	// no selection, we'll insert at the end. If there is a selection
	// however, we will insert after the selected item. If there is more
	// than one item selected, we will insert after the last item in the
	// selection.

	UInt32 after = LArray::index_Last;
	
	// get the selection
/*	
	// JCD 980623 - stopped work 1/2 through... not certain if this is
	// the best approach. Need to research it a bit more. Removing
	// the partial functionality until I can determine the best course
	// of action (I've spent too much time on this... will hopefully
	// revisit later)

	DMSelection	*theSelection = GetSelection();
	ValidateObject_(theSelection);
	
	// how many items are in the selection?
	
	Uint32 theCount = theSelection->GetSelectedElements().GetCount();
	
	// if nothing in the selection, we can quit now

	if ( theCount > 0 ) {
	
		// we have some items... let's pick the last one in the array/selection.

		DMElement			*theLast;		
		theSelection->GetSelectedElements().FetchItemAt(LArray::index_Last, &theLast);
		ValidateObject_(theLast);
		
		// got the last one. Let's find where in the main list this happens
		// to be. After this is where we'll insert our new item.
		
		OVItem	theLast;
		ArrayIndexT theIndex = GetItems().FetchIndexOf(&theLast);
		
		if ( theIndex < -1 ) {
			SignalPStr_("\pum");
		}
	}
*/	
	// Insert it into the container.
	
	attr->InsertElementAt(after+1, newItem, true);
	
	// Set selection to new item.
	
	mSelection->SelectSimple(newItem);

	// Make sure it's visible and start in-place editing.

	OVItem* item = FindItemForSubElement(newItem, true);
	ValidateObject_(item);
	
	STableCell cell;
	cell.row = FindRowForItem(item);
	cell.col = 2;
	ScrollCellIntoFrame(cell);

	item->StartInPlaceEdit(cell);	

	// Commit the transaction.
	
	trans.Commit();
	
}


// ---------------------------------------------------------------------------
//		* DuplicateStrings
// ---------------------------------------------------------------------------
//	Duplicate all selected items in the table. Behaves sort of like the
//	Copy and Paste commands, except that the clipboard isn't affected.

void
SLStringListTable::DuplicateStrings()
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
	
	// Clone each item and add it to the temporary list.
	DMFastIterator cloneIter(copyList->GetSelectedElements());
	while (cloneIter.NextElement())
	{
		DMElement* element = cloneIter.CurrentElement();
		ValidateObject_(element);

		DMElementPtr clone = element->Clone();
		ValidateObject_(clone.GetObject());

		tempList.InsertItemAt(LArray::index_Last, clone);
	}
	
	// Okay, now we have the copied list. Now we "paste" the items back in.
	StDMTransactionBuilder trans(this, mSelection, cmd_Duplicate);

	// Empty selection.
	mSelection->SelectNone();

	// Get the list of cloned items.
	// Place each cloned item back into the data model.
	DMFastIterator iter(tempList);
	while (iter.NextElement())
	{
		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);

		// Add the string to the end of the list
		DMListAttribute* attr = (dynamic_cast<DMListAttribute*>(mElement.GetObject()));;
		ValidateObject_(attr);
		attr->InsertElementAt(LArray::index_Last, element, true);

		// Add this duplicated string to the selection
		mSelection->SelectAdd(element);
		
		// Make sure the first duplicated string is in view
		if (iter.GetCurrentIndex() == 1)
			ScrollElementIntoView(element, false);
	}

	// Commit transaction.
	trans.Commit();
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Add edit menu handler for edit strings.

void
SLStringListTable::SetSelection(
	DMSelection*	inSelection)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSelection);

	// Let outline table record selection.

	OVTable::SetSelection(inSelection);
	
	// Create edit-menu handler attachment.

	AddAttachment(new DMEditAttachment(
					this,						// inActionHost
					mSelection,					// inSelection
					STR_SLEditMenuStrings));	// inEditMenuStrings

}


// ===========================================================================

#pragma mark -
#pragma mark ** internal configuration

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Configure the table display.

void
SLStringListTable::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Do inherited configuration.

	OVTable::FinishCreateSelf();

	// Set up default columns.
	
	InsertCols(2, 0, nil, 0, false);
	SetColWidth(48, 1, 1);
	SetColWidth(mFrameSize.width - 48, 2, 2);

}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Create display items for each named type.

void
SLStringListTable::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have a list attribute.

	DMListAttribute* listAttr = (dynamic_cast<DMListAttribute*>(inElement));
	ValidateObject_(listAttr);
	SetContainer(listAttr);

}


// ===========================================================================

#pragma mark -
#pragma mark ** custom resizing behaviors

// ---------------------------------------------------------------------------
//		* ResizeFrameBy											[protected]
// ---------------------------------------------------------------------------
//	Make sure the table column follows the window size.

void
SLStringListTable::ResizeFrameBy(
	SInt16		inWidthDelta,
	SInt16		inHeightDelta,
	Boolean		inRefresh)
{

	// Validate pointers.
	
	ValidateThis_();

	// Resize the view.

	LView::ResizeFrameBy(inWidthDelta, inHeightDelta, inRefresh);
	
	// Stretch the string text column to fill space.
	
	if (inWidthDelta != 0)
		SetColWidth(GetColWidth(2) + inWidthDelta, 2, 2);

}


// ===========================================================================

#pragma mark -
#pragma mark *** SLItemFactory ***

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------

OVItem*
SLItemFactory::CreateItemForElement(
	DMElement*	inElement) 
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// See what type of object this is.

	if ((dynamic_cast<DMStringAttribute*>(inElement)) != nil)
		return new SLStringItem;
	else
		return nil;

}

// ===========================================================================

#pragma mark -
#pragma mark *** SLStringItem ***

// ---------------------------------------------------------------------------
//		* SLStringItem()
// ---------------------------------------------------------------------------
//	Constructor

SLStringItem::SLStringItem()
{
	mStringAttr = nil;
}


// ---------------------------------------------------------------------------
//		* ~SLStringItem
// ---------------------------------------------------------------------------
//	Destructor

SLStringItem::~SLStringItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Return the attribute key for each column.

DM_AttributeKey
SLStringItem::GetAttributeKeyForCell(
	TableIndexT		inColumn) const
{
	switch (inColumn) {
	
		case 2:
			return 'pSTR';
		
		default:
			return 0;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this element is a VETypeEntry.

void
SLStringItem::SetElementSelf(
	DMElement*	inElement)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a DMStringAttribute.

	mStringAttr = (dynamic_cast<DMStringAttribute*>(inElement));
	ValidateObject_(mStringAttr);

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	We draw only one column: the class name.

void
SLStringItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mStringAttr);
	
	// Return draw contents.

	LStr255 tempStr;

	switch (inCell.col) {
	
		case 1:
			DMListAttribute* listAttr;
			listAttr = (dynamic_cast<DMListAttribute*>(mStringAttr->GetSuperElement()));
			ValidateObject_(listAttr);
			
			tempStr = (SInt32) listAttr->GetSubElements().FetchIndexOf(&mStringAttr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			
			ioDrawContents.outTextTraits.justification = teFlushRight;
			ioDrawContents.outTextBaseline++;
			break;
		
		case 2:
			mStringAttr->GetTextValue(tempStr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			
			ioDrawContents.outCanDoInPlaceEdit = true;
			ioDrawContents.outShowSelection = true;
			ioDrawContents.outMultiLineText = true;
			break;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** mouse-down behaviors

// ---------------------------------------------------------------------------
//		* SingleClick											[protected]
// ---------------------------------------------------------------------------
//	Overridden to make string cell immediately editable.

void
SLStringItem::SingleClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mStringAttr);
	
	// Start in-place editing, without typical 1-second delay.

	if ((inCell.col == 2) && (mStringAttr->IsEnabled()))
		StartInPlaceEdit(inCell);

}


// ---------------------------------------------------------------------------
//		* TrackEmptyClick										[protected]
// ---------------------------------------------------------------------------
//	Overridden to make entire cell sensitive for IPE.

void
SLStringItem::TrackEmptyClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		inMouseDown,
	const SOutlineDrawContents&	inDrawContents)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(mStringAttr);
	
	// Make entire cell sensitive for in-place editing.

	if ((inCell.col == 2) && (mStringAttr->IsEnabled())) {
		mOVTable->UnselectAllCells();
		mOVTable->SelectCell(inCell);
		StartInPlaceEdit(inCell);
	}
	else
		LOutlineItem::TrackEmptyClick(inCell, inMouseDown, inDrawContents);

}


// ---------------------------------------------------------------------------
//		* UpdateRowSize											[protected]
// ---------------------------------------------------------------------------
//	Make sure the table row resizes itself to fit the text. Text may
//	be on multiple lines.

void
SLStringItem::UpdateRowSize()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Calculate height of attribute text.
	
	TableIndexT row = FindRowForItem();
	STableCell cell(row, 2);
	SInt16 height = CalcRowHeightForCell(cell);

	// Set vertical size based on font info.

	if (height < 15)
		height = 15;
	
	if (mOVTable->GetRowHeight(row) != height) {

		mOVTable->SetRowHeight(height, row, row);

		STableCell topLeftCell(row, 1);
		STableCell botRightCell;
		mOVTable->GetTableSize(botRightCell.row, botRightCell.col);
		mOVTable->RefreshCellRange(topLeftCell, botRightCell);

	}
}


// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that UpdateRowSize is called when string text
//	changes.

void
SLStringItem::ValueChanged(
	DM_ValueChange*		/* inMessage */)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Check the size of this row.
	
	UpdateRowSize();
	
	// Trigger a redraw in the value column.
	
	STableCell cell(FindRowForItem(), 2);
	mOVTable->RefreshCell(cell);
	
}
