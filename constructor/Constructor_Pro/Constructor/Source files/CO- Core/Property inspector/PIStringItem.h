// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIStringItem.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/20/96
//     $Date: 2006/01/18 01:33:24 $
//	$History: PIStringItem.h $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/28/97   Time: 19:56
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Moved multi-line text support to LOutlineItem.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/28/97   Time: 17:46
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Upgraded to handle word-wrapping appropriately. (Bug fix #1211.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/05/96   Time: 17:28
//	Updated in $/Constructor/Source files/Property inspector
//	Added key filter for printing characters and CR.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/05/96   Time: 17:21
//	Updated in $/Constructor/Source files/Property inspector
//	Updated to display and edit multi-line strings.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/04/96   Time: 20:32
//	Updated in $/Constructor/Source files/Property inspector
//	Updated to display PowerPlant pane titles in the style of their text
//	traits.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 09:43
//	Updated in $/Constructor/Source files/Property inspector
//	Moved ValueChanged method from PIAttributeItem (to reduce flicker in
//	other attribute items). Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:42
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
#include "PIAttributeItem.h"


// ===========================================================================
//		* PIStringItem
// ===========================================================================
//
//	PIStringItem is an attribute item for string and integer values.
//	It uses in-place editing to display and change the values.
//
// ===========================================================================

class PIStringItem : public PIAttributeItem {

public:
							PIStringItem();
	virtual					~PIStringItem();
	
	// configuration
	
	virtual void			UpdateRowSize();

	// update notifications

protected:
	virtual void			ValueChanged(
									DM_ValueChange*			inMessage);

	// drawing

	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);

	// clicking

	virtual void			SingleClick(
									const STableCell&			inCell,
									const SMouseDownEvent&		inMouseDown,
									const SOutlineDrawContents&	inDrawContents,
									Boolean						inHitText);
	virtual void			TrackEmptyClick(
									const STableCell&			inCell,
									const SMouseDownEvent&		inMouseDown,
									const SOutlineDrawContents&	inDrawContents);

	// display synchronization

	virtual void			SetValueFromAttributeSelf();

	// in-place editing implementation
	
	virtual void			ConfigureInPlaceEdit(
									const STableCell&		inCell,
									ResIDT&					outTextTraitsID,
									SInt16&					outMaxChars,
									UInt8&					outAttributes,
									TEKeyFilterFunc&		outKeyFilter);
	virtual ResIDT			ConfigureTextTraits(
									TextTraitsRecord&		outTextTraits);

};
