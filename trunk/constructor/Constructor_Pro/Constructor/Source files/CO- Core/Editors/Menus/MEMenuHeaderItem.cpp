// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEMenuHeaderItem.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/12/96
//	   $Date: 2006/01/18 01:33:09 $
//	$History: MEMenuHeaderItem.cpp $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:07
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:04p
//	Updated in $/ConstructorWin/Sources
//	Initial merge for MSVC-hosted build
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:20
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 6   *****************
//	User: scouten      Date: 01/24/97   Time: 17:29
//	Checked in '$/Constructor/Source files/CO- Core/Editors/Menus'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 5   *****************
//	User: scouten      Date: 12/07/96   Time: 11:27
//	Checked in '$/Constructor/Source files/Editors/Menus/+ Generic menu editor'
//	Comment: Updated toRobin's drop 12/06/96.
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 12/06/96   Time: 23:55
//	Checked in '$/Constructor/Source files/Editors/Menus/+ Generic menu editor'
//	Comment: Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/24/96   Time: 01:17
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

#include "MEMenuHeaderItem.h"

	// Core : Editors : Menus
#include "MEModelMenu.h"
#include "MEOutlineTable.h"

	// PowerPlant : GA : Grayscale utilities classes
#include "UGAColorRamp.h"


// ===========================================================================

#pragma mark *** MEMenuHeaderItem ***

// ---------------------------------------------------------------------------
//		* MEMenuHeaderItem()
// ---------------------------------------------------------------------------
//	Default constructor

MEMenuHeaderItem::MEMenuHeaderItem()
{
	mExpanded = true;
	mMenuHeader = nil;
}


// ---------------------------------------------------------------------------
//		* ~MEMenuHeaderItem
// ---------------------------------------------------------------------------
//	Destructor

MEMenuHeaderItem::~MEMenuHeaderItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessor

// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Return the attribute that's displayed in each cell.

DM_AttributeKey
MEMenuHeaderItem::GetAttributeKeyForCell(
	TableIndexT		inColumn) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// There's only one attribute/cell link.

	switch (inColumn) {

		case MenuColumn_ItemText:
			return MEModelMenu::attrKey_MenuTitle;
		
		default:
			return 0;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu title drawing behaviors

// ---------------------------------------------------------------------------
//		* UpdateRowSize
// ---------------------------------------------------------------------------
//	Overriden to ensure that the menu bar is 20 pixels high.

void
MEMenuHeaderItem::UpdateRowSize()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);

	TableIndexT row = FindRowForItem();
	
#if PP_Target_Carbon
	SInt16 theHeight;
	GetThemeMenuBarHeight(&theHeight);
	mOVTable->SetRowHeight(theHeight, row, row);
#else
	// The menu bar needs to be 20 pixels tall.
	mOVTable->SetRowHeight(20, row, row);
#endif
}

// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Sets up the drawing for menu titles.

void
MEMenuHeaderItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mMenuHeader);
	
	// It's not a divider. Set up standard drawing behaviors.

	LStr255 tempStr;
	ioDrawContents.outTextTraits.fontNumber = 0;
	ioDrawContents.outTextTraits.size = 12;
	if (mMenuHeader->GetEnabled() == false)
		ioDrawContents.outTextTraits.mode = grayishTextOr;
	
	// See which column we're drawing.
	
	switch (inCell.col) {
	
		case MenuColumn_ItemText:
			ioDrawContents.ioCellBounds.left -= 9;
			ioDrawContents.outCanDoInPlaceEdit = !mMenuHeader->IsFakeMenu();
			ioDrawContents.outShowSelection = true;
			ioDrawContents.outTextTraits.color = UGAColorRamp::GetWhiteColor();
			ioDrawContents.outTextTraits.mode = srcXor;
			mMenuHeader->GetMenuTitle(tempStr);
			
			if (mMenuHeader->IsFakeMenu() || (mMenuHeader->GetEnabled() == 0)) {
				ioDrawContents.outTextTraits.color = UGAColorRamp::GetBlackColor();
				ioDrawContents.outTextTraits.mode = grayishTextOr;
			}
			
			break;

	}
	
	// Put the string in draw contents buffer.
	
	LString::CopyPStr(tempStr, ioDrawContents.outTextString);
	
}


// ---------------------------------------------------------------------------
//		* PrepareDrawContents									[protected]
// ---------------------------------------------------------------------------
//	GHD: Overridden to adjust text frame so that the EditField aligns
//	exactly with the underlying text. For some reason, the default location
//	is too high by one pixel.

void
MEMenuHeaderItem::PrepareDrawContents(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{
	OVItem::PrepareDrawContents(inCell, ioDrawContents);

	ioDrawContents.prTextFrame.top += 1;
	ioDrawContents.prTextFrame.bottom += 1;
}


// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	Overriden to draw the menu title frame in black.

void
MEMenuHeaderItem::DrawRowAdornments(
	const Rect&		inLocalRowRect)
{
	
	// Validate pointers.
	
	ValidateThis_();

	// Draw inherited adornments.

	OVItem::DrawRowAdornments (inLocalRowRect);

#if PP_Target_Carbon
	// Draw menu bar background
	::DrawThemeMenuBarBackground(&inLocalRowRect, kThemeMenuBarNormal, 0);
#endif
	
	// Draw box around menu title.
	
	DrawMenuTitleFrame();

}


// ---------------------------------------------------------------------------
//		* CalcModifiedFlagRect									[protected]
// ---------------------------------------------------------------------------
//	Overriden to disable the modified flag.

Boolean
MEMenuHeaderItem::CalcModifiedFlagRect(
	Rect&	/* outModFlagRect */)
{
	return false;			// we don't want a modified flag
}


// ---------------------------------------------------------------------------
//		* ConfigureInPlaceEdit									[protected]
// ---------------------------------------------------------------------------
//	Set up fonts, etc. for editing menu title.

void
MEMenuHeaderItem::ConfigureInPlaceEdit(
	const STableCell&		inCell,
	ResIDT&					outTextTraitsID,
	SInt16&					/* outMaxChars */,
	UInt8&					outAttributes,
	TEKeyFilterFunc&		/* outKeyFilter */)
{

	// Validate pointers.

	ValidateThis_();
	
	// Turn off word wrapping.
	
	outAttributes &= ~editAttr_WordWrap;
	
	// See which column we're editing.
	
	switch (inCell.col) {
	
		case MenuColumn_ItemText:
			outTextTraitsID = 0;
			break;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu title area behaviors

// ---------------------------------------------------------------------------
//		* DrawMenuTitleFrame									[protected]
// ---------------------------------------------------------------------------
//	Draw the menu title frame in black.

void
MEMenuHeaderItem::DrawMenuTitleFrame()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Get menu frame rectangle.
	
	Rect menuTitleFrame;
	if (!CalcLocalMenuTitleRect(menuTitleFrame))
		return;
	
	// Draw the frame.
	
	RGBColor theColor;
	::RGBForeColor(&theColor);
	::PaintRect(&menuTitleFrame);
	
}


// ---------------------------------------------------------------------------
//		* CalcLocalMenuTitleRect								[protected]
// ---------------------------------------------------------------------------
//	Figure out where the menu title frame should be drawn.

Boolean
MEMenuHeaderItem::CalcLocalMenuTitleRect(
	Rect&	outMenuFrameRect)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOutlineTable);
	ValidateObject_(mMenuHeader);

	// Get coordinates for cell containing menu gap.
	// We'll draw the menu title immediately to the right of the gap.

	STableCell cell;
	cell.row = FindRowForItem();
	cell.col = MenuColumn_LeftGap;

	if (cell.row == 0)
		return false;

	SInt32 left, right, top, bottom;

	mOutlineTable->GetImageCellBounds(cell, left, top, right, bottom);
	outMenuFrameRect.left = left;
	outMenuFrameRect.top = top;
	outMenuFrameRect.right = right;
	outMenuFrameRect.bottom = bottom;
	
	// Got the cell coordinates. Now grab the menu title.
	
	LStr255 tempStr;
	mMenuHeader->GetMenuTitle(tempStr);
	
	// Set up the system's preferred font.
	
	UTextTraits::SetPortTextTraits((TextTraitsPtr) nil);

	// Okay, now we can calculate the menu title frame.
	// It starts at the right edge of the "gap" column and extends
	// far enough right to include the menu title and a little bit
	// of space on either side of it.

	outMenuFrameRect.top++;
	outMenuFrameRect.left = outMenuFrameRect.right;
	outMenuFrameRect.right = outMenuFrameRect.left + ::StringWidth(tempStr) + 12;
	return true;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** disclosure triangle behaviors

// ---------------------------------------------------------------------------
//		* DrawDisclosureTriangle								[protected]
// ---------------------------------------------------------------------------
//	Overriden to disable disclosure triangle.

void
MEMenuHeaderItem::DrawDisclosureTriangle()
{
	// do nothing
}


// ---------------------------------------------------------------------------
//		* CalcLocalDisclosureTriangleRect						[protected]
// ---------------------------------------------------------------------------
//	Overriden to disable disclosure triangle.

Boolean
MEMenuHeaderItem::CalcLocalDisclosureTriangleRect(
	Rect&	outTriangleRect)
{

	// Overriden to disable disclosure triangle.

	outTriangleRect.left = 0;
	outTriangleRect.right = 0;
	
	outTriangleRect.top = 0;
	outTriangleRect.bottom = 0;
	
	return false;
}


// ---------------------------------------------------------------------------
//	¥ TrackDisclosureTriangle									   [protected]
// ---------------------------------------------------------------------------
//	Called by ClickCell whenever a mouse-down is detected within the
//	disclosure triangle. Tracks the mouse movement until mouse-up and
//	expands or collapses this item as appropriate.

void
MEMenuHeaderItem::TrackDisclosureTriangle(
	const SMouseDownEvent& /* inMouseDown */)
{
	// Do nothing
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure we really have an MEModelMenu.

void
MEMenuHeaderItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Make sure we really have a MEModelMenu.

	mMenuHeader = (dynamic_cast<MEModelMenu*>(inElement));
	ValidateObject_(mMenuHeader);

	// Specify container for submodels.

	SetContainer(mMenuHeader->GetMenuList());

}


// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Overridden to redraw menu row when menu title changes.

void
MEMenuHeaderItem::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inMessage->mAttribute);

	// Refresh default cell.

	OVItem::ValueChanged(inMessage);
	
	// Refresh extra cells.

	switch (inMessage->mAttribute->GetKey()) {
	
		case MEModelMenu::attrKey_MenuTitle:
			RefreshRow();
			break;
		
	}
}