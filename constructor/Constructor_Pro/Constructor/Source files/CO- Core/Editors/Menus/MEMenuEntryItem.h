// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEMenuEntryItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/12/96
//	   $Date: 2006/01/18 01:33:09 $
//	$History: MEMenuEntryItem.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:20
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/07/97   Time: 18:10
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Rolled in an update from Clint 01/06/97.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 12/06/96   Time: 23:55
//	Checked in '$/Constructor/Source files/Editors/Menus/+ Generic menu editor'
//	Comment: Updated to Clint's drop 12/06/96.
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

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class MEModelMenuItem;


// ===========================================================================
//		* MEMenuEntryItem
// ===========================================================================
//	MEMenuBarItem is an outline item that draws a single menu item.

class MEMenuEntryItem : public OVItem {

public:
							MEMenuEntryItem();
	virtual					~MEMenuEntryItem();

	// attribute accessor
	
	virtual DM_AttributeKey	GetAttributeKeyForCell(
									TableIndexT				inColumn) const;

	// selection drawing

	virtual void			RefreshSelection(
									const STableCell&		inCell);

	// drawing setup

protected:
	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);
	virtual void			PrepareDrawContents(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);
	virtual void			DrawRowAdornments(
									const Rect&				inLocalRowRect);
	virtual Boolean			CalcModifiedFlagRect(
									Rect&					outModFlagRect);

	virtual void			ConfigureInPlaceEdit(
									const STableCell&		inCell,
									ResIDT&					outTextTraitsID,
									SInt16&					outMaxChars,
									UInt8&					outAttributes,
									TEKeyFilterFunc&		outKeyFilter);

	// click behaviors

	virtual void			TrackEmptyClick(
									const STableCell&			inCell,
									const SMouseDownEvent&		inMouseDown,
									const SOutlineDrawContents&	inDrawContents);
	
	// drag selection helpers

	virtual Boolean			CellHitByMarquee(
									const STableCell&		inCell,
									const Rect&				inMarqueeLocalRect);

	// configuration

	virtual void			SetElementSelf(DMElement*		inElement);

	// attribute notifications

	virtual void			ValueChanged(
									DM_ValueChange*			inMessage);


	// data members

protected:
	MEModelMenuItem*		mMenuItem;
	static Handle			sSubMenuIconSuite;

};
