// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIFontItem.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/31/96
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIFontItem.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:21
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/25/96   Time: 19:04
//	Updated in $/Constructor/Source files/Property inspector
//	Improved placement of font name popup. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:26
//	Updated in $/Constructor/Source files/Property inspector
//	Added history information.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 16:24
//	Created
//	Comment: Added class.
//	
// ===========================================================================

#pragma once

	// Core : Property inspector
#include "PIStringItem.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMFontStringAttribute;
class PIFontPopup;


// ===========================================================================
//		* PIFontItem
// ===========================================================================
//
//	PIFontItem is an attribute for font string attributes.
//
// ===========================================================================

class PIFontItem : public PIStringItem {

public:
								PIFontItem();
	virtual						~PIFontItem();

	// configuration

protected:
	virtual void				FinishCreate();
	virtual void				SetElementSelf(DMElement*		inElement);

	// drawing & editing

	virtual void				GetDrawContentsSelf(
										const STableCell&		inCell,
										SOutlineDrawContents&	ioDrawContents);
	virtual void				ConfigureInPlaceEdit(
										const STableCell&		inCell,
										ResIDT&					outTextTraitsID,
										SInt16&					outMaxChars,
										UInt8&					outAttributes,
										TEKeyFilterFunc&		outKeyFilter);

	virtual void				SetValueFromAttributeSelf();
	virtual void				SetValueFromDisplaySelf();
	virtual void				DisableSelf();
	virtual void				EnableSelf();

	
	// data members

protected:
	DMFontStringAttribute*		mAttributeFont;					// the font attribute we're displaying
	PIFontPopup*				mPopup;							// font menu popup

};
