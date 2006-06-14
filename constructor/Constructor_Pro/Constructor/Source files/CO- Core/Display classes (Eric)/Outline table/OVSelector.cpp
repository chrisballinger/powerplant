// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVSelector.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/21/96
//     $Date: 2006/01/18 01:32:44 $
//  $History: OVSelector.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 4   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:05
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge of changes for MSVC build
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:24
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 19:15
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Improved commenting.
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

#include "OVSelector.h"

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"
#include "OVTable.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** OVSelector ***

// ---------------------------------------------------------------------------
//		* OVSelector(OVTable*, DMSelection*)
// ---------------------------------------------------------------------------
//	Constructor, specifying the table whose cells we're selecting and the
//	data model selection object.

OVSelector::OVSelector(
	OVTable*		inOutlineTable,
	DMSelection*	inSelection)

: LTableSelector(inOutlineTable),
  DMDragSelector(*inOutlineTable, inSelection)

{
	ValidateObject_(inSelection);
	ValidateObject_(inOutlineTable);

	mOutlineTable = inOutlineTable;
	inSelection->AddListener(this);
}


// ---------------------------------------------------------------------------
//		* ~OVSelector
// ---------------------------------------------------------------------------
//	Destructor

OVSelector::~OVSelector()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** cell selection overrides

// ---------------------------------------------------------------------------
//		* CellIsSelected
// ---------------------------------------------------------------------------
//	Return true if the named cell is selected.

Boolean
OVSelector::CellIsSelected(
	const STableCell&	inCell) const 
{
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	DMElement* element = FindElementForCell(inCell);
	if (element != nil) {
		ValidateObject_(element);
		return mSelection->IsSelected(element);
	}
	else
		return false;
}


// ---------------------------------------------------------------------------
//		* GetFirstSelectedCell
// ---------------------------------------------------------------------------
//	Find the first cell in the table that is selected.

STableCell
OVSelector::GetFirstSelectedCell() const
{
	ValidateThis_();

	STableCell cell(1, 1);
	cell.row = GetFirstSelectedRow();
	return cell;
}


// ---------------------------------------------------------------------------
//		* GetFirstSelectedRow
// ---------------------------------------------------------------------------
//	Find the first row in the table that is selected.

TableIndexT
OVSelector::GetFirstSelectedRow() const
{
	ValidateThis_();
	ValidateObject_(const_cast<OVSelector*>(this)->mOutlineTable);
	ValidateObject_(mSelection.GetObject());

	TableIndexT rows, cols;
	mOutlineTable->GetTableSize(rows, cols);
	
	STableCell cell(1, 1);
	for (cell.row = 1; cell.row <= rows; cell.row++) {

		DMElement* element = FindElementForCell(cell);
		if (element != nil) {
			ValidateObject_(element);
			if (mSelection->IsSelected(element))
				return cell.row;
		}
	}

	return 0;
}


// ---------------------------------------------------------------------------
//		* SelectAllCells
// ---------------------------------------------------------------------------
//	Select all cells in the outline.

void
OVSelector::SelectAllCells() 
{
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mOutlineTable);
	
	mSelection->SelectNone();

	TableIndexT rows, cols;
	mOutlineTable->GetTableSize(rows, cols);
	
	STableCell cell(1, 1);
	for (cell.row = 1; cell.row <= rows; cell.row++) {
		DMElement* element = FindElementForCell(cell);
		if (element != nil) {
			ValidateObject_(element);
			mSelection->SelectAdd(element);
		}
	}
}


// ---------------------------------------------------------------------------
//		* UnselectAllCells
// ---------------------------------------------------------------------------
//	Deselect all cells in the outline.

void
OVSelector::UnselectAllCells() 
{
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	mSelection->SelectNone();
}


// ---------------------------------------------------------------------------
//		* SelectCell
// ---------------------------------------------------------------------------
//	Add the named cell to the selection.

void
OVSelector::SelectCell(
	const STableCell& inCell) 
{
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	DMElement* element = FindElementForCell(inCell);
	if (element != nil) {
		ValidateObject_(element);
		mSelection->SelectAdd(element);
	}
}


// ---------------------------------------------------------------------------
//		* UnselectCell
// ---------------------------------------------------------------------------
//	Remove the named cell from the selection.

void
OVSelector::UnselectCell(
	const STableCell& inCell) 
{
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	DMElement* element = FindElementForCell(inCell);
	if (element != nil) {
		ValidateObject_(element);
		mSelection->SelectRemove(element);
	}
}


// ---------------------------------------------------------------------------
//		* ClickSelect
// ---------------------------------------------------------------------------
//	No-op. In the outline table, cells take care of their own selection.

void
OVSelector::ClickSelect(
	const STableCell&		/* inCell */,
	const SMouseDownEvent&	/* inMouseDown */) 
{
	ValidateThis_();
	UnselectAllCells();
		// cells take care of themselves
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag selection

// ---------------------------------------------------------------------------
//		* DragSelect
// ---------------------------------------------------------------------------
//	Perform Finder-like marquee selection in this table.

Boolean
OVSelector::DragSelect(
	const STableCell&		/* inCell */,
	const SMouseDownEvent&	inMouseDown) 
{

	ValidateThis_();
	ValidateObject_(mOutlineTable);
	ValidateObject_(mSelection.GetObject());

	// If shift-selecting, duplicate current selection.
	
	RecordInitialSelection((inMouseDown.macEvent.modifiers & shiftKey) != 0);
	
	// Make sure display is accurate before we drag.
	
	mOutlineTable->UpdatePort();
	
	// Do the marquee selection.

	try {
		LMarqueeTask marquee(*this, inMouseDown);
		marquee.DoMarquee();
	}
	catch(...) {
	}
	
	return true;
}


// ---------------------------------------------------------------------------
//		* CheckElements
// ---------------------------------------------------------------------------
//	Helper for DragSelect. Test display to see which items have fallen into
//	or out of the selection marqee.

void
OVSelector::CheckElements(
	const Rect& inOldMarqueeRect,
	const Rect& inNewMarqueeRect) 
{

	// Validate input parameters.

	ValidateThis_();
	ValidateObject_(mOutlineTable);
	ValidateObject_(mSelection.GetObject());

	// Hit-test all cells in the junction of the old and new marquee.

	Rect testRect = inOldMarqueeRect;
	::UnionRect(&testRect, &inNewMarqueeRect, &testRect);

	STableCell topLeftCell, botRightCell;
	mOutlineTable->FetchIntersectingCells(testRect, topLeftCell, botRightCell);
	
	TableIndexT rows, cols;
	mOutlineTable->GetTableSize(rows, cols);

	// Iterate over all affected cells.

	STableCell cell(1, 1);
	for (cell.row = topLeftCell.row; cell.row <= botRightCell.row; cell.row++) {

		// Find element at this row.

		DMElement* element = FindElementForCell(cell);
		if (element == nil)
			continue;
		
		ValidateObject_(element);

		// See if the marquee touches this cell's contents.

		Boolean cellIsHitBy = false;
		for (cell.col = 1; cell.col <= cols; cell.col++) {
			if (mOutlineTable->CellHitByMarquee(cell, inNewMarqueeRect))
				cellIsHitBy = true;
		}
		cell.col = 1;

		FoundElement(element, cellIsHitBy);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SelectionChanged										[protected]
// ---------------------------------------------------------------------------
//	The selection has changed. If we're displaying one of the objects in the
//	selection, make sure the selection indication is redrawn.

void
OVSelector::SelectionChanged(
	DM_SelectionChanged* inMessage)
{

	ValidateThis_();
	ValidateObject_(mOutlineTable);
	ValidateObject_(mSelection.GetObject());

	if (inMessage->mSelection != mSelection)
		return;
	
	OVItem* item = mOutlineTable->FindItemForSubElement(inMessage->mElement, true);
	if (item != nil) {

		ValidateObject_(item);

		STableCell cell(1, 1);
		cell.row = mOutlineTable->FindRowForItem(item);
		
		TableIndexT rows, cols;
		mOutlineTable->GetTableSize(rows, cols);
		
		for (cell.col = 1; cell.col <= cols; cell.col++) {
			item->RefreshSelection(cell);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** implementation helper

// ---------------------------------------------------------------------------
//		* FindElementForCell									[protected]
// ---------------------------------------------------------------------------
//	Given a cell, find the data model element that corresponds to that cell.

DMElement*
OVSelector::FindElementForCell(
	const STableCell& inCell) const
{

	ValidateThis_();
	ValidateObject_(const_cast<OVSelector*>(this)->mOutlineTable);
	ValidateObject_(mSelection.GetObject());

	DMElement* element = nil;
	OVItem* item = (dynamic_cast<OVItem*>(mOutlineTable->FindItemForRow(inCell.row)));

	if (item != nil) {
		ValidateObject_(item);
		element = item->GetElement();
	}

	return element;
}
