// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PILabelledItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 09/27/96
//     $Date: 2006/01/18 01:33:23 $
//	$History: PILabelledItem.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:21
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/25/96   Time: 19:09
//	Updated in $/Constructor/Source files/Property inspector
//	Improved placement of value label popup. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:38
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for attribute's enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Property inspector
#include "PIStringItem.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMLabelledIntegerAttribute;
class PILabelPopup;


// ===========================================================================
//		* PILabelledItem
// ===========================================================================
//
//	PILabelledItem is an attribute for labelled integer attributes.
//
// ===========================================================================

class PILabelledItem : public PIStringItem {

public:
								PILabelledItem();
	virtual						~PILabelledItem();

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
	DMLabelledIntegerAttribute* mAttributeLab;
	PILabelPopup*				mPopup;

};
