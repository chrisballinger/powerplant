// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLTabControlListTable.cpp	© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================


// ===========================================================================

#include "TCLTabControlListTable.h"
#include "Constructor.menu.h"
#include "DMListAttribute.h"
#include "DMEditAttachment.h"
#include "StDMTransactionBuilder.h"
#include "TCLModelTabControl.h"
#include "DMReanimator.h"

// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT		STR_TCLEditMenuStrings		= 7080;

const TableIndexT	kIndexColumn			= 1;
const TableIndexT	kIconSuiteIDColumn		= 2;
const TableIndexT	kTabNameColumn			= 3;

const SInt16		kIndexColumnWidth		= 48;
const SInt16		kIconSuiteIDColumnWidth	= 48;

const ResIDT		DSPC_TabInfoData	= 5277;



// ===========================================================================


// ---------------------------------------------------------------------------
//		* TCLTabControlListTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

TCLTabControlListTable::TCLTabControlListTable(
	LStream*	inStream)

	: OVTable(inStream, 17, true)
{
	// Establish item factory for table.
	SetItemFactory(new TCLItemFactory);

	// Set edit strings.
	DMDropArea::SetEditMenuStrings(STR_TCLEditMenuStrings);
}


// ---------------------------------------------------------------------------
//		* ~TCLTabControlListTable
// ---------------------------------------------------------------------------
//	Destructor

TCLTabControlListTable::~TCLTabControlListTable()
{
}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Overriden to enable the New String command.

void
TCLTabControlListTable::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	// Validate pointers.
	ValidateThis_();
	
	// Dispatch command.
	switch (inCommand)
	{
		// New dynamic item: Enable iff a dynamic list item is selected.
		case cmd_NewItem:
		case cmd_NewItemDialog:
		{
			ValidateObject_(mElement.GetObject());
			outEnabled = mElement->IsEnabled();
			break;
		}
		
		// Otherwise, let supercommander handle it.
		default:
		{
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
		}
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Respond to the New String command.

Boolean
TCLTabControlListTable::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	// Validate pointers.
	ValidateThis_();
	
	// Dispatch command.
	switch (inCommand)
	{
		// New dynamic list item.
		case cmd_NewItem:
		case cmd_NewItemDialog:
		{
			NewTabInfoItem();
			return true;
		}
		
		// Otherwise: Let supercommander handle it.
		default:
		{
			return OVTable::ObeyCommand(inCommand, ioParam);
		}
	}
}


// ---------------------------------------------------------------------------
//		* NewTabInfoItem
// ---------------------------------------------------------------------------
//	Create a new string in the string list.

void
TCLTabControlListTable::NewTabInfoItem()
{
	// Validate pointers.
	ValidateThis_();

	// See where the dynamic item is.
	TCLModelTabControl* tabControl = dynamic_cast<TCLModelTabControl*>(mElement.GetObject());
	ValidateObject_(tabControl);

	DMListAttribute* tabInfo = tabControl->GetTabInfo();
	ValidateObject_(tabInfo);


	// We have a valid list item. Set up a transaction for the new item.
	StDMTransactionBuilder trans(this, mSelection, cmd_NewItem);

	// Duplicate prototype item.
	DMElementPtr newTabInfoObject = nil;

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->ElementsFromResource('DSPC', DSPC_TabInfoData, newTabInfoObject);
	ValidateObject_(newTabInfoObject.GetObject());
	
	// JCD 980619 - find the location where to insert. If there is
	// no selection, we'll insert at the end. If there is a selection
	// however, we will insert after the selected item. If there is more
	// than one item selected, we will insert after the last item in the
	// selection.

	UInt32 after = LArray::index_Last;
	
	// Insert it into the container.
	tabInfo->InsertElementAt(after + 1, newTabInfoObject, true);
	
	// Set selection to new item.
	mSelection->SelectSimple(newTabInfoObject);

	// Make sure it's visible and start in-place editing.
	OVItem* item = FindItemForSubElement(newTabInfoObject, true);
	ValidateObject_(item);
	
	STableCell cell;
	cell.row = FindRowForItem(item);
	cell.col = kTabNameColumn;
	ScrollCellIntoFrame(cell);

	item->StartInPlaceEdit(cell);	

	// Commit the transaction.
	trans.Commit();

}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Add edit menu handler for edit strings.

void
TCLTabControlListTable::SetSelection(
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
					STR_TCLEditMenuStrings));	// inEditMenuStrings
}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Configure the table display.

void
TCLTabControlListTable::FinishCreateSelf()
{
	// Validate pointers.
	ValidateThis_();
	
	// Do inherited configuration.
	OVTable::FinishCreateSelf();

	// Set up default columns.
	InsertCols(3, 0, nil, 0, false);
	SetColWidth(kIndexColumnWidth, kIndexColumn, kIndexColumn);
	SetColWidth(kIconSuiteIDColumnWidth, kIconSuiteIDColumn, kIconSuiteIDColumn);
	SetColWidth(mFrameSize.width - (kIndexColumnWidth + kIconSuiteIDColumnWidth), kTabNameColumn, kTabNameColumn);
}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Create display items for each named type.

void
TCLTabControlListTable::SetElementSelf(
	DMElement*	inElement)
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have a list attribute.
	TCLModelTabControl* tabControl = dynamic_cast<TCLModelTabControl*>(inElement);
	ValidateObject_(tabControl);
	
	DMListAttribute* tabInfo = tabControl->GetTabInfo();
	ValidateObject_(tabInfo);
	SetContainer(tabInfo);
}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* ResizeFrameBy											[protected]
// ---------------------------------------------------------------------------
//	Make sure the table column follows the window size.

void
TCLTabControlListTable::ResizeFrameBy(
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
		SetColWidth(GetColWidth(kTabNameColumn) + inWidthDelta, kTabNameColumn, kTabNameColumn);
}


// ===========================================================================

#pragma mark -

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------

OVItem*
TCLItemFactory::CreateItemForElement(
	DMElement*	inElement) 
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(inElement);
	
	// See what type of object this is.
	if (dynamic_cast<TCLModelTabInfo*>(inElement) != nil)
		return new TCLTabInfoItem;
	else
		return nil;
}

// ===========================================================================

#pragma mark -

// ---------------------------------------------------------------------------
//		* TCLTabInfoItem()
// ---------------------------------------------------------------------------
//	Constructor

TCLTabInfoItem::TCLTabInfoItem()
{
	mTabInfo = nil;
}


// ---------------------------------------------------------------------------
//		* ~TCLTabInfoItem
// ---------------------------------------------------------------------------
//	Destructor

TCLTabInfoItem::~TCLTabInfoItem()
{
}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Return the attribute key for each column.

DM_AttributeKey
TCLTabInfoItem::GetAttributeKeyForCell(
	TableIndexT		inColumn) const
{
	switch (inColumn)
	{
		case kIconSuiteIDColumn:
		{
			return TCLModelTabInfo::attrKey_IconSuiteID;
		}
		case kTabNameColumn:
		{
			return TCLModelTabInfo::attrKey_TabName;
		}
		default:
		{
			return 0;
		}
	}
}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this element is a VETypeEntry.

void
TCLTabInfoItem::SetElementSelf(
	DMElement*	inElement)
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a DMStringAttribute.
	mTabInfo = dynamic_cast<TCLModelTabInfo*>(inElement);
	ValidateObject_(mTabInfo);
}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	We draw only one column: the class name.

void
TCLTabInfoItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(mTabInfo);
	
	// Return draw contents.
	LStr255 tempStr;

	switch (inCell.col)
	{
		case kIndexColumn:
		{
			DMListAttribute* tabInfoList;
			tabInfoList = dynamic_cast<DMListAttribute*>(mTabInfo->GetSuperElement());
			ValidateObject_(tabInfoList);

			tempStr = (SInt32) tabInfoList->GetSubElements().FetchIndexOf(&mTabInfo);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			ioDrawContents.outTextTraits.justification = teFlushRight;
			ioDrawContents.outTextBaseline++;
			break;
		}
		case kIconSuiteIDColumn:
		{
			tempStr = mTabInfo->GetIconSuiteID();
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			
			ioDrawContents.outCanDoInPlaceEdit = true;
			ioDrawContents.outShowSelection = true;
			ioDrawContents.outMultiLineText = true;
			break;
		}
		case kTabNameColumn:
		{
			mTabInfo->GetTabName(tempStr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			
			ioDrawContents.outCanDoInPlaceEdit = true;
			ioDrawContents.outShowSelection = true;
			ioDrawContents.outMultiLineText = true;
			break;
		}
	}
}


// ===========================================================================


// ---------------------------------------------------------------------------
//		* SingleClick											[protected]
// ---------------------------------------------------------------------------
//	Overridden to make string cell immediately editable.

void
TCLTabInfoItem::SingleClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(mTabInfo);
	
	// Start in-place editing, without typical 1-second delay.
	if ((inCell.col == kIconSuiteIDColumn) && (mTabInfo->GetIconSuiteIDAttribute()->IsEnabled()))
		StartInPlaceEdit(inCell);
	if ((inCell.col == kTabNameColumn) && (mTabInfo->GetTabNameAttribute()->IsEnabled()))
		StartInPlaceEdit(inCell);
}


// ---------------------------------------------------------------------------
//		* TrackEmptyClick										[protected]
// ---------------------------------------------------------------------------
//	Overridden to make entire cell sensitive for IPE.

void
TCLTabInfoItem::TrackEmptyClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		inMouseDown,
	const SOutlineDrawContents&	inDrawContents)
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(mTabInfo);
	
	// Make entire cell sensitive for in-place editing.
	if ((inCell.col == kIconSuiteIDColumn) && (mTabInfo->GetIconSuiteIDAttribute()->IsEnabled()))
	{
		mOVTable->UnselectAllCells();
		mOVTable->SelectCell(inCell);
		StartInPlaceEdit(inCell);
	}
	else if ((inCell.col == kTabNameColumn) && (mTabInfo->GetTabNameAttribute()->IsEnabled()))
	{
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
TCLTabInfoItem::UpdateRowSize()
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Calculate height of attribute text.
	TableIndexT row = FindRowForItem();
	STableCell cell(row, kIconSuiteIDColumn);
	SInt16 height = CalcRowHeightForCell(cell);

	// Set vertical size based on font info.
	if (height < 15)
		height = 15;
	
	if (mOVTable->GetRowHeight(row) != height)
	{
		mOVTable->SetRowHeight(height, row, row);

		STableCell topLeftCell(row, kIndexColumn);
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
TCLTabInfoItem::ValueChanged(
	DM_ValueChange*		/* inMessage */)
{
	// Validate pointers.
	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Check the size of this row.
	UpdateRowSize();
	
	// Trigger a redraw in the value column.
	STableCell topLeftCell(FindRowForItem(), kIconSuiteIDColumn);
	STableCell botRightCell(FindRowForItem(), kTabNameColumn);
	mOVTable->RefreshCellRange(topLeftCell, botRightCell);
}
