// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIContainerItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/19/96
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIContainerItem.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:24
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for attribute's enabled flag.
//	
// ===========================================================================

#pragma once

	// Core : Property inspector
#include "PIAttributeItem.h"


// ===========================================================================
//		* PIContainerItem
// ===========================================================================
//
//	PIContainerItem displays an entry in the property inspector for
//	outline items.
//
// ===========================================================================

class PIContainerItem : public PIAttributeItem {

public:
							PIContainerItem();
	virtual					~PIContainerItem();
	
protected:
	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);
	virtual void			DrawRowAdornments(
									const Rect&				inLocalRowRect);

	virtual void			SetElementSelf(
									DMElement*			inElement);
	virtual void			DoubleClick(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									const SOutlineDrawContents&	inDrawContents,
									Boolean					inHitText);

};
