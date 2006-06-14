// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEMenuEntryItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/12/96
//	   $Date: 2006/01/18 01:33:08 $
//	$History: MEMenuEntryItem.cpp $
//	
//	*****************  Version 13  *****************
//	User: Andrew       Date: 3/05/97    Time: 4:36p
//	Updated in $/ConstructorWin/Core/Source
//	Fixed array index for Pilot target under MSVC
//	
//	*****************  Version 12  *****************
//	User: scouten      Date: 3/03/97    Time: 1:11p
//	Updated in $/ConstructorWin/Core/Source
//	Fixed a VC++ build error.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:36
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Added #ifs for Pilot appearance.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:07
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:04p
//	Updated in $/ConstructorWin/Sources
//	Initial merge for MSVC-hosted build
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:20
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/22/97   Time: 20:10
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed Constructor_ForJava #ifdefs.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/07/97   Time: 18:10
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Rolled in an update from Clint 01/06/97.
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

#include "MEMenuEntryItem.h"

	// Core : Editors : Menus 
#include "MEModelMenuItem.h"
#include "MEOutlineTable.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Core : Utilities
#include "UIconUtilities.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

Handle MEMenuEntryItem::sSubMenuIconSuite = nil;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	ICON_SubMenuIcon		= 7001;


// ===========================================================================

#pragma mark *** MEMenuEntryItem ***

// ---------------------------------------------------------------------------
//		* MEMenuEntryItem()
// ---------------------------------------------------------------------------
//	Constructor

MEMenuEntryItem::MEMenuEntryItem()
{
	mMenuItem = nil;
	
	if (sSubMenuIconSuite == nil) {
		StApplicationContext appContext;
		sSubMenuIconSuite = UIconUtilities::Get1DetachedIconSuite(ICON_SubMenuIcon, svAllSmallData);
	}
}


// ---------------------------------------------------------------------------
//		* ~MEMenuEntryItem
// ---------------------------------------------------------------------------
//	Destructor

MEMenuEntryItem::~MEMenuEntryItem()
{
	ValidateThis_();
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessor

// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Return the attribute key for each cell.

DM_AttributeKey
MEMenuEntryItem::GetAttributeKeyForCell(
	TableIndexT		inColumn) const
{
	switch (inColumn) {
	
		case MenuColumn_ItemText:
			return MEModelMenuItem::attrKey_MenuText;

		case MenuColumn_MarkChar:
			return MEModelMenuItem::attrKey_MarkCharacter;

		case MenuColumn_CmdKey:
			return MEModelMenuItem::attrKey_CommandKey;
			
		case MenuColumn_CmdID:
			return MEModelMenuItem::attrKey_CommandNumber;

		default:
			return 0;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection drawing

// ---------------------------------------------------------------------------
//		* RefreshSelection
// ---------------------------------------------------------------------------
//	Overriden to ensure that the entire menu frame is redrawn if this is
//	a divider line.

void
MEMenuEntryItem::RefreshSelection(
	const STableCell&	inCell)
{

	// Sanity check: Make sure menu item is valid.

	ValidateThis_();
	ValidateObject_(mMenuItem);
	ValidateObject_(mOutlineTable);
	
	// If it's a divider line, we have to refresh everything.
	// Otherwise the inherited method will do the right thing.

	if (mMenuItem->GetDividerLine()) {
	
		// Divider line, refresh everything between left gap
		// and right gap.
		
		if ((inCell.col > MenuColumn_LeftGap) && (inCell.col < MenuColumn_RightGap))
			mOutlineTable->RefreshCell(inCell);
	
	}
	else {
	
		// Not a divider line, let inherited do the work.
	
		LOutlineItem::RefreshSelection(inCell);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing setup

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Configures the drawing for menu items.

void
MEMenuEntryItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Sanity check: Make sure menu item is valid.

	ValidateThis_();
	ValidateObject_(mMenuItem);
	
	// See if this is a divider item. If so, all drawing is done by
	// DrawRowAdornments.
	
	if (mMenuItem->GetDividerLine()) {
		if (inCell.col == MenuColumn_ItemText)
			ioDrawContents.outShowSelection = true;
		return;
	}

	// It's not a divider. Set up standard drawing behaviors.

	LStr255 tempStr;
	ioDrawContents.outTextTraits.fontNumber = 0;
	ioDrawContents.outTextTraits.size = 12;
	if (mMenuItem->GetEnabled() == false)
		ioDrawContents.outTextTraits.mode = grayishTextOr;
	
	// See which column we're drawing.
	
	switch (inCell.col) {
	
		case MenuColumn_MarkChar: {
#if !Constructor_ForJava
			SInt32 markChar = mMenuItem->GetMarkCharacter();
			if (markChar != 0) {
				tempStr[(UInt8) 0] = 1;
				tempStr[(UInt8) 1] = markChar;
			}
#endif			
			ioDrawContents.ioCellBounds.left -= 1;
			ioDrawContents.ioCellBounds.right += 4;
			break;
		}
		
		case MenuColumn_ItemText:
			ioDrawContents.outTextTraits.style = mMenuItem->GetMenuStyle();
			ioDrawContents.outCanDoInPlaceEdit = true;
			ioDrawContents.outShowSelection = true;
			mMenuItem->GetMenuText(tempStr);
			break;

		case MenuColumn_CloverChar:
			if (mMenuItem->GetSubMenuID() != 0)
			{
				ioDrawContents.outHasIcon = true;
				ioDrawContents.outIconSuite = sSubMenuIconSuite;
				ioDrawContents.ioCellBounds.left += 5;
				ioDrawContents.ioCellBounds.right += 5;
			}
			else
			{
				const UInt8	charShift = kMenuShiftGlyph;
				const UInt8	charControl = kMenuControlGlyph;
				const UInt8	charOption = kMenuOptionGlyph;
				const UInt8	charCommand = kMenuCommandGlyph;
				
				tempStr = "";
				if (mMenuItem->GetCommandKey() != 0)
				{
					if (mMenuItem->GetModShiftKey())
						tempStr.Append(&charShift, 1);
					if (mMenuItem->GetModControlKey())
						tempStr.Append(&charControl, 1);
					if (mMenuItem->GetModOptionKey())
						tempStr.Append(&charOption, 1);
					if (mMenuItem->GetModCommandKey())
						tempStr.Append(&charCommand, 1);
				}
				ioDrawContents.outTextTraits.justification = teJustRight;
				ioDrawContents.ioCellBounds.left -= 3;
				ioDrawContents.ioCellBounds.right += 1;
			}
			break;
		
		case MenuColumn_CmdKey:
		{
			if (mMenuItem->GetSubMenuID() == 0)
			{
				SInt16	keyboardGlyph = mMenuItem->GetKeyboardGlyph();
				if (keyboardGlyph != kMenuNullGlyph)
				{
					// A keyboard glyph will override the key shortcut
					ioDrawContents.outCanDoInPlaceEdit = false;

					short fontNum = 0;
					GetFNum("\p.Keyboard", &fontNum);
					if (fontNum != 0)
						ioDrawContents.outTextTraits.fontNumber = fontNum;

					//+++ Some of the key glyphs are not drawing properly
					if (keyboardGlyph < 255)
					{
						unsigned char keyboardGlyphChar = keyboardGlyph;
						tempStr.Assign(&keyboardGlyphChar, 1);
					}
					else
						tempStr.Assign(&keyboardGlyph, 2);
				}
				else
				{
					SInt32 cmdKey = mMenuItem->GetCommandKey();
					if (cmdKey != 0)
					{
						tempStr[(UInt8) 0] = 1;
						tempStr[(UInt8) 1] = cmdKey;
					}
					ioDrawContents.outCanDoInPlaceEdit = true;
				}
				ioDrawContents.ioCellBounds.left -= 4;
				ioDrawContents.ioCellBounds.right += 3;
			}
			break;
		}
			
		case MenuColumn_CmdID: {
			DMAttribute* cmdNumber = mMenuItem->GetCommandNumberAttribute();
			ValidateObject_(cmdNumber);
			cmdNumber->GetTextValue(tempStr);
			ioDrawContents.outTextTraits.fontNumber = 1;
			ioDrawContents.outTextTraits.size = 9;
			ioDrawContents.outTextTraits.mode = srcOr;
			ioDrawContents.outCanDoInPlaceEdit = true;
			break;
		}
	}
	
	// Put the string in draw contents buffer.
	
	LString::CopyPStr(tempStr, ioDrawContents.outTextString);
	
}


// ---------------------------------------------------------------------------
//		* PrepareDrawContents									[protected]
// ---------------------------------------------------------------------------
//	GHD: Overridden to adjust text frame so that the EditField aligns
//	exactly with the underlying text. For some reason, the default location
//	to too high by one pixel.

void
MEMenuEntryItem::PrepareDrawContents(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{
	OVItem::PrepareDrawContents(inCell, ioDrawContents);

	switch (inCell.col) {
	
#if !Constructor_ForJava
		case MenuColumn_CmdKey:
		case MenuColumn_MarkChar:
			ioDrawContents.prTextFrame.top += 1;
			ioDrawContents.prTextFrame.bottom += 1;
			break;
#endif

		case MenuColumn_ItemText:
			ioDrawContents.prTextFrame.top += 1;
			ioDrawContents.prTextFrame.bottom += 1;
			break;
	}
			
}


// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	Overriden to draw the divider line item.

void
MEMenuEntryItem::DrawRowAdornments(
	const Rect&		inLocalRowRect)
{

	ValidateThis_();
	ValidateObject_(mMenuItem);
	ValidateObject_(mOutlineTable);
	
	// See if this is a divider line item.

	if (mMenuItem->GetDividerLine()) {
	
		// It is, draw divider line instead of menu text.
		// Get location of left and right gap cells.
	
		STableCell cell;
		Rect leftCellFrame, rightCellFrame;
		SInt32 left, right, top, bottom;

		cell.row = FindRowForItem();
		cell.col = MenuColumn_LeftGap;

		mOutlineTable->GetImageCellBounds(cell, left, top, right, bottom);
		leftCellFrame.left = left;
		leftCellFrame.top = top;
		leftCellFrame.right = right;
		leftCellFrame.bottom = bottom;

		cell.col = MenuColumn_RightGap;
		mOutlineTable->GetImageCellBounds(cell, left, top, right, bottom);
		rightCellFrame.left = left;
		rightCellFrame.top = top;
		rightCellFrame.right = right;
		rightCellFrame.bottom = bottom;
		
		// Calculate divider line position.
		
		Rect lineFrame;
		lineFrame.left = leftCellFrame.right + 1;
		lineFrame.right = rightCellFrame.left;
#if PP_Target_Carbon
		lineFrame.top = leftCellFrame.top;
		SInt16 theHeight;
		::GetThemeMenuSeparatorHeight(&theHeight);
		lineFrame.bottom = lineFrame.top + theHeight;
		::DrawThemeMenuSeparator(&lineFrame);
#else
		lineFrame.top = (leftCellFrame.top + leftCellFrame.bottom >> 1) - 1;
		lineFrame.bottom = lineFrame.top + 1;
		// Need to adjust line drawing for monitor depth. Argh.
		
		{
			StDeviceLoop devices(lineFrame);
			Int16 depth;
			
			while (devices.NextDepth(depth)) {
				::PenNormal();
				if (depth > 2)
				{
					RGBColor theColor = UGAColorRamp::GetColor(7);
					::RGBForeColor(&theColor);
				}
				else
				{
					Pattern thePattern;
					UQDGlobals::GetGrayPat(&thePattern);
					::PenPat(&thePattern);
				}
				
				::MoveTo(lineFrame.left, lineFrame.top);
				::LineTo(lineFrame.right - 1, lineFrame.top);
			}
		}
#endif
		
		// If this item is selected, draw selection indication.
		
		if (mOutlineTable->CellIsSelected(cell)) {
		
			// Calculate the selection area.
			
			Rect selectRect = lineFrame;
			selectRect.top -= 4;
			selectRect.bottom += 4;
			selectRect.left += 8;
			selectRect.right -= 8;
		
			// Highlight this area.
		
			StColorPenState::Normalize();
			::LMSetHiliteMode(::LMGetHiliteMode() & 0x7F);
			::InvertRect(&selectRect);
			if (!mOutlineTable->IsActive()) {
				::LMSetHiliteMode(::LMGetHiliteMode() & 0x7F);
				::InsetRect(&selectRect, 1, 1);
				::InvertRect(&selectRect);
			}
		}
	}

	// Draw any other adornments.

	OVItem::DrawRowAdornments(inLocalRowRect);

}


// ---------------------------------------------------------------------------
//		* CalcModifiedFlagRect									[protected]
// ---------------------------------------------------------------------------
//	Overriden to place the modified flag at the left edge of the window.

Boolean
MEMenuEntryItem::CalcModifiedFlagRect(
	Rect&	outModFlagRect)
{
	ValidateThis_();
	ValidateObject_(mOutlineTable);

	STableCell cell;
	cell.row = FindRowForItem();
	cell.col = 1;
	
	if (cell.row == 0)
		return false;

	if (!mOutlineTable->GetLocalCellRect(cell, outModFlagRect))
		return false;
	
	SInt16 mid = (outModFlagRect.top + outModFlagRect.bottom) >> 1;
	outModFlagRect.top = mid - 1;
	outModFlagRect.bottom = mid + 3;
	outModFlagRect.left = outModFlagRect.right - 10;
	outModFlagRect.right = outModFlagRect.left + 4;
	return true;
	
}


// ---------------------------------------------------------------------------
//		* ConfigureInPlaceEdit									[protected]
// ---------------------------------------------------------------------------
//	Set up correct fonts for in-place editing.

void
MEMenuEntryItem::ConfigureInPlaceEdit(
	const STableCell&		inCell,
	ResIDT&					outTextTraitsID,
	SInt16&					outMaxChars,
	UInt8&					outAttributes,
	TEKeyFilterFunc&		/* outKeyFilter */)
{

	// Sanity check: Make sure menu item is valid.

	ValidateThis_();
	ValidateObject_(mMenuItem);
	
	// See if this is a divider item. If so, all drawing is done by
	// DrawRowAdornments.
	
	if (mMenuItem->GetDividerLine())
		return;

	// Turn off word wrapping.
	
	outAttributes &= ~editAttr_WordWrap;

	// See which column we're editing.
	
	switch (inCell.col) {
	
#if !Constructor_ForJava						//* EAS 970122: fixed conditional error
		case MenuColumn_CmdKey:
		case MenuColumn_MarkChar:
			outMaxChars = 1;
			outTextTraitsID = 0;
			break;
#endif
		case MenuColumn_ItemText:
			outTextTraitsID = 0;
			break;

	}

}


// ===========================================================================

#pragma mark -
#pragma mark ** click behaviors

// ---------------------------------------------------------------------------
//		* TrackEmptyClick										[protected]
// ---------------------------------------------------------------------------
//	Overriden to ensure that menu item is selected regardless of what
//	part of the menu frame is hit.

void
MEMenuEntryItem::TrackEmptyClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		inMouseDown,
	const SOutlineDrawContents&	inDrawContents)
{


	// Sanity check: Make sure menu item is valid.

	ValidateThis_();
	ValidateObject_(mMenuItem);
	ValidateObject_(mOutlineTable);
	
	// Unlike all other outline tables, we'll select this cell whenever
	// anything between the left gap and right gap is selected.

	if ((inCell.col > MenuColumn_LeftGap) && (inCell.col < MenuColumn_RightGap)) {

		// Change the selection.

		if (!(inMouseDown.macEvent.modifiers & shiftKey))
			mOutlineTable->UnselectAllCells();
		mOutlineTable->SelectCell(inCell);
		mOutlineTable->UpdatePort();
		
		// If this is a divider line, see if user wants to drag.
		
		if (mMenuItem->GetDividerLine()) {
		
			// It is a divider line. See if mouse is going to move.
			
			if (::StillDown() && ::WaitMouseMoved(inMouseDown.macEvent.where))
				TrackDrag(inCell, inMouseDown, inDrawContents);
				
		}

	}
	else {
	
		// Not a select click, let inherited do the work.
	
		LOutlineItem::TrackEmptyClick(inCell, inMouseDown, inDrawContents);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag selection helpers

// ---------------------------------------------------------------------------
//		* CellHitByMarquee										[protected]
// ---------------------------------------------------------------------------
//	Overriden to allow drag-selection of divider lines.

Boolean
MEMenuEntryItem::CellHitByMarquee(
	const STableCell&	inCell,
	const Rect&			inMarqueeLocalRect)
{

	// Validate input parameters.

	ValidateThis_();
	ValidateObject_(mMenuItem);
	
	// See if this is a divider line. If so, select any
	// space between left gap and right gap.
	
	if (mMenuItem->GetDividerLine()) {
	
		if ((inCell.col > MenuColumn_LeftGap) || (inCell.col < MenuColumn_RightGap)) {
		
			// Calculcate cell bounds.
		
			Rect cellFrame;
			mOutlineTable->FocusDraw();
			mOutlineTable->GetLocalCellRect(inCell, cellFrame);
		
			// See if marquee rect touches any part of the cell.
		
			Rect scratchRect;
			if (::SectRect(&inMarqueeLocalRect, &cellFrame, &scratchRect))
				return true;
				
		}
	}
	
	// Not divider line, or outside area. Use regular hit testing.

	return OVItem::CellHitByMarquee(inCell, inMarqueeLocalRect);

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure that we are given an MEModelMenuItem.

void
MEMenuEntryItem::SetElementSelf(
	DMElement*	inElement)
{
	
	// Sanity checks.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a MEModelMenuitem.

	mMenuItem = (dynamic_cast<MEModelMenuItem*>(inElement));
	ValidateObject_(mMenuItem);

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Make sure that display gets updated properly. Some attributes don't
//	appear automatically, so some special handling is required.

void
MEMenuEntryItem::ValueChanged(
	DM_ValueChange*		inMessage)
{

	ValidateThis_();
	ValidateObject_(inMessage->mAttribute);

	// Refresh default cell.

	OVItem::ValueChanged(inMessage);
	
	// Refresh extra cells.

	switch (inMessage->mAttribute->GetKey()) {
	
		case MEModelMenuItem::attrKey_DividerLine:
			RefreshRow();
			break;
		
		case MEModelMenuItem::attrKey_ModCommandKey:
		case MEModelMenuItem::attrKey_ModShiftKey:
		case MEModelMenuItem::attrKey_ModOptionKey:
		case MEModelMenuItem::attrKey_ModControlKey:
		case MEModelMenuItem::attrKey_CommandKey:
			RefreshCell(MenuColumn_CloverChar);
			break;
		
		case MEModelMenuItem::attrKey_SubMenuID:
			RefreshCell(MenuColumn_CloverChar);
			RefreshCell(MenuColumn_CmdKey);
			break;
		
		case MEModelMenuItem::attrKey_Enabled:
			RefreshCell(MenuColumn_ItemText);
			RefreshCell(MenuColumn_MarkChar);
			RefreshCell(MenuColumn_CloverChar);
			RefreshCell(MenuColumn_CmdKey);
			break;
		
		case MEModelMenuItem::attrKey_MenuStyle:
			RefreshCell(MenuColumn_ItemText);
			break;

		case MEModelMenuItem::attrKey_KeyboardGlyph:
			RefreshCell(MenuColumn_CmdKey);
			break;
	}
}
