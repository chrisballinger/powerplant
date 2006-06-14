// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEMenuHeaderItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/12/96
//	   $Date: 2006/01/18 01:33:09 $
//	$History: MEMenuHeaderItem.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:20
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
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

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class MEModelMenu;


// ===========================================================================
//		* MEMenuHeaderItem
// ===========================================================================
//	MEMenuBarItem is an outline item that draws the menu title.

class MEMenuHeaderItem : public OVItem {

public:
							MEMenuHeaderItem();
	virtual					~MEMenuHeaderItem();

	// attribute accessor

	virtual DM_AttributeKey	GetAttributeKeyForCell(
									TableIndexT				inColumn) const;

	// menu title drawing behaviors
	
	virtual void			UpdateRowSize();

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

	// menu title area behaviors
	
	virtual void			DrawMenuTitleFrame();
	virtual Boolean			CalcLocalMenuTitleRect(Rect&	outMenuFrameRect);

	// disclosure triangle behaviors

	virtual void			DrawDisclosureTriangle();
	virtual Boolean			CalcLocalDisclosureTriangleRect(
									Rect&					outTriangleRect);
	virtual void			TrackDisclosureTriangle(
									const SMouseDownEvent&		inMouseDown);

	// configuration
	
	virtual void			SetElementSelf(
									DMElement*				inElement);
	virtual void			ValueChanged(
									DM_ValueChange*			inMessage);


	// data members

protected:
	MEModelMenu*			mMenuHeader;

};
