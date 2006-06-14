// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LIconTextTable.cp				PowerPlant 2.1		©2000 Metrowerks Inc.
// ===========================================================================

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include "LIconTextTable.h"

#include <LStream.h>

#include <LTableView.h>
#include <LTableMonoGeometry.h>
#include <LTableSingleSelector.h>
#include <LTableArrayStorage.h>

#include <PP_Messages.h>
#include <UTextTraits.h>

PP_Begin_Namespace_PowerPlant

// ---------------------------------------------------------------------------
//	¥ LIconTextTable						Stream Constructor		  [public]
// ---------------------------------------------------------------------------

LIconTextTable::LIconTextTable(
	LStream*	inStream)
	
	: LTableView(inStream)
{
	*inStream >> mControllerMessage;
	
	mScroller = NULL;
	
	SetTableGeometry(new LTableMonoGeometry(this, 64, 64));
	SetTableSelector(new LTableSingleSelector(this));
	SetTableStorage(new LTableArrayStorage(this, sizeof(LIconTextItem)));
	SetCustomHilite(true);
	
	LTableView::InsertRows(1, 0, NULL, 0, Refresh_No);
}

// ---------------------------------------------------------------------------
//	¥ LIconTextTable						Destructor				  [public]
// ---------------------------------------------------------------------------

LIconTextTable::~LIconTextTable()
{
}

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ SetScroller
// ---------------------------------------------------------------------------
//	Set the LScrollerView that is in charge of the LIconTextTable.

void
LIconTextTable::SetScroller(LScrollerView *inScroller)
{
	mScroller = inScroller;
}

// ---------------------------------------------------------------------------
//	¥ SetControllerMessage
// ---------------------------------------------------------------------------
//	Set the message to send when the LIconTextTable selection changes.

void
LIconTextTable::SetControllerMessage(MessageT inControllerMessage)
{
	mControllerMessage = inControllerMessage;
}

// ---------------------------------------------------------------------------
//	¥ GetValue
// ---------------------------------------------------------------------------
//	Return the value of a LIconTextTable.

SInt32
LIconTextTable::GetValue() const
{
	SInt32	value = -1;
	
	STableCell firstSelection = GetFirstSelectedCell();
	
	if (!firstSelection.IsNullCell())
	{
		Point thePoint;
		firstSelection.ToPoint(thePoint);
		value = thePoint.h - 1;	// Column number of selection
	}
	
	return value;
}


// ---------------------------------------------------------------------------
//	¥ SetValue
// ---------------------------------------------------------------------------
//	Set the value of a LIconTextTable.

void
LIconTextTable::SetValue(
	SInt32	inValue)
{
	STableCell theCell;
	
	theCell.SetCell(1, inValue + 1);
	SelectCell(theCell);
}


// ---------------------------------------------------------------------------
//	¥ GetDescriptor
// ---------------------------------------------------------------------------

StringPtr
LIconTextTable::GetDescriptor(
	Str255	outDescriptor) const
{
#pragma unused(outDescriptor)
	return NULL;
}


// ---------------------------------------------------------------------------
//	¥ SetDescriptor
// ---------------------------------------------------------------------------

void
LIconTextTable::SetDescriptor(
	ConstStringPtr	inDescriptor)
{
#pragma unused(inDescriptor)
}

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ HiliteCellInactively
// ---------------------------------------------------------------------------
//	Draw or undraw inactive hiliting for a Cell

void
LIconTextTable::HiliteCellInactively(
	const STableCell	&inCell,
	Boolean				/* inHilite */)
{
	Rect	cellFrame;
    if (GetLocalCellRect(inCell, cellFrame) && FocusExposed()) {
        StColorPenState saveColorPen;   // Preserve color & pen state
        StColorPenState::Normalize();
        UDrawingUtils::SetHiliteModeOn();
		::MacInvertRect(&cellFrame);
	}
}


// ---------------------------------------------------------------------------
//	¥ DrawCell
// ---------------------------------------------------------------------------
//	Draw the contents of the specified Cell

void
LIconTextTable::DrawCell(
	const STableCell&	inCell,
	const Rect&			inLocalRect)
{
	LIconTextItem theButtonItem;
	UInt32 theItemSize = sizeof(theButtonItem);
	GetCellData(inCell, &theButtonItem, theItemSize);
	
	Rect theBounds;
	theBounds.top = inLocalRect.top + 4;
	theBounds.bottom = theBounds.top + 32;
	theBounds.left = inLocalRect.left + ((64 /*lRect->right - lRect->left*/) / 2) - 16;
	theBounds.right = theBounds.left + 32;
	
	IconSuiteRef theIcon;
	OSErr theErr = ::GetIconSuite(&theIcon, theButtonItem.theIconID, svAllAvailableData);
	
	if (theErr == noErr)
	{
		::PlotIconSuite(&theBounds, kAlignCenterTop, kTransformNone, theIcon);
		::DisposeIconSuite(theIcon, true);
	}
	
	theBounds = inLocalRect;
	theBounds.top += 32 + 4;
	
#if ACCESSOR_CALLS_ARE_FUNCTIONS
	UTextTraits::SetPortTextTraits(theButtonItem.theTextTraitsID);
#else
	UTextTraits::SetPortTextTraits(theButtonItem.theTextTraitsID);
#endif
	
	short theHeight = 64 /*theBounds.bottom*/ - theBounds.top;
	short theSpace = 64 /*(theBounds.right - theBounds.left)*/ - 6;
	
	FontInfo theFontInfo;
	::GetFontInfo(&theFontInfo);
	
	short theCharHeight = theFontInfo.ascent + theFontInfo.descent;
	short theWidth = ::StringWidth(theButtonItem.theIconLabel);		
	
	short h = (theSpace - theWidth) / 2 + theBounds.left + 3;
	short v = (theHeight - theCharHeight) / 2 + theBounds.top + theFontInfo.ascent;
	
	::MoveTo(h, v);
	::DrawString(theButtonItem.theIconLabel);
}

// ---------------------------------------------------------------------------
//	¥ ClickSelf
// ---------------------------------------------------------------------------
//	Handle a mouse click within a TableView

void
LIconTextTable::ClickSelf(
	const SMouseDownEvent &inMouseDown)
{
	STableCell	hitCell;
	SPoint32	imagePt;
	
	LocalToImagePoint(inMouseDown.whereLocal, imagePt);
	
	if (GetCellHitBy(imagePt, hitCell))
	{
		if (ClickSelect(hitCell, inMouseDown))
		{
			ClickCell(hitCell, inMouseDown);
		}
	}
	else
	{
		hitCell.SetCell(1, GetIconCount());
		ClickCell(hitCell, inMouseDown);
	}
}

void
LIconTextTable::Click(
	SMouseDownEvent	&inMouseDown)
{
	Boolean	saveDelaySelect = inMouseDown.delaySelect;
	inMouseDown.delaySelect = false;
	LTableView::Click(inMouseDown);
	inMouseDown.delaySelect = saveDelaySelect;
}


// ---------------------------------------------------------------------------
//	¥ SelectionChanged
// ---------------------------------------------------------------------------
//	Broadcast message when selected cells change

void
LIconTextTable::SelectionChanged()
{
	if (mControllerMessage != msg_Nothing) {
		SInt32 theValue = GetValue() + 1;
		BroadcastMessage(mControllerMessage, &theValue);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ RevealSelectedIcon
// ---------------------------------------------------------------------------
//	Scroll the selected cell into view

void LIconTextTable::RevealSelectedIcon()
/* JWW - This isn't strictly necessary, so I haven't written it.  Maybe one day... */
{
/*	if (mScroller != NULL)
	{
		STableCell theCell;
		theCell.SetCell(1, GetIconCount());
		
		Rect theRect;
		GetLocalCellRect(theCell, theRect);
		
		SPoint32		scrollUnit;
		SDimension16	scrollFrameSize;
		SDimension32	scrollImageSize;
		SPoint32		scrollPosition;
		GetScrollUnit(scrollUnit);
		GetFrameSize(scrollFrameSize);
		GetImageSize(scrollImageSize);
		GetScrollPosition(scrollPosition);
	}*/
}

// ---------------------------------------------------------------------------
//	¥ GetIconCount
// ---------------------------------------------------------------------------
//	Get the number of items in the table

SInt16 LIconTextTable::GetIconCount() const
{
	TableIndexT theRows;
	TableIndexT theColumns;
	
	GetTableSize(theRows, theColumns);
	
	return theColumns;
}

// ---------------------------------------------------------------------------
//	¥ RemoveIconAt
// ---------------------------------------------------------------------------
//	Remove an item from the table

void LIconTextTable::RemoveIconAt(SInt16 inIndex)
{
	RemoveCols(1, inIndex, Refresh_Yes);
}

// ---------------------------------------------------------------------------
//	¥ AddIcon
// ---------------------------------------------------------------------------
//	Add an item to the table

void LIconTextTable::AddIcon(LIconTextItem *inButtonItem)
{
	TableIndexT theNextPosition = GetIconCount();
	
	InsertCols(1, theNextPosition, inButtonItem, sizeof(*inButtonItem), Refresh_Yes);
}

// ---------------------------------------------------------------------------
//	¥ GetIconAt
// ---------------------------------------------------------------------------
//	Get information on an item in the table

void LIconTextTable::GetIconAt(SInt16 inIndex, LIconTextItem *outButtonItem)
{
	STableCell theCell;
	theCell.SetCell(1, inIndex);
	UInt32 theLength = sizeof(*outButtonItem);
	GetCellData(theCell, outButtonItem, theLength);
}

// ---------------------------------------------------------------------------
//	¥ SetSelectedIconByIndex
// ---------------------------------------------------------------------------
//	Change the current selection

void LIconTextTable::SetSelectedIconByIndex(SInt16 inIndex)
{
	SetValue(inIndex - 1);
}

// ---------------------------------------------------------------------------
//	¥ InsertRows
// ---------------------------------------------------------------------------
//	Only one row allowed, so prevent insertion

void
LIconTextTable::InsertRows(
	UInt32			/* inHowMany */,
	TableIndexT		/* inAfterCol */,
	const void*		/* inDataPtr */,
	UInt32			/* inDataSize	*/,
	Boolean			/* inRefresh */)
{
	SignalStringLiteral_("Inserting rows not allowed");
}


// ---------------------------------------------------------------------------
//	¥ RemoveRows
// ---------------------------------------------------------------------------
//	Only one row allowed, so prevent removal

void
LIconTextTable::RemoveRows(
	UInt32			/* inHowMany */,
	TableIndexT		/* inFromCol */,
	Boolean			/* inRefresh */)

{
	SignalStringLiteral_("Removing rows not allowed");
}

PP_End_Namespace_PowerPlant