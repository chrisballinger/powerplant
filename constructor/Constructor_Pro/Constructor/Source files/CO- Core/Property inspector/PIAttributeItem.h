// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIAttributeItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/19/96
//     $Date: 2006/01/18 01:33:20 $
//	$History: PIAttributeItem.h $
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:28
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added ValueChanged method to ensure that attribute display keeps up
//	with attribute changes.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 11/26/96   Time: 09:42
//	Updated in $/Constructor/Source files/Property inspector
//	Moved ValueChanged method to PIStringItem (reduces flicker for other
//	attribute items). Improved commenting.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:16
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for attribute's enabled flag.
//	
//	[Versions 2-9 omitted. Testing history format header.]
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:08
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline tabls
#include "OVItem.h"


// ===========================================================================
//		* PIAttributeItem
// ===========================================================================
//
//	PIAttributeItem is an outline item for a generic attribute. It is not
//	an abstract class, but it is typically subclassed for most data types.
//
// ===========================================================================

class PIAttributeItem : public OVItem {

public:
							PIAttributeItem();
	virtual					~PIAttributeItem();

	// cell accessors

	virtual DMAttribute*	GetAttributeForCell(
									TableIndexT			inColumn) const;

	// update notifications

protected:
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			EnabledChanged(
									DMElement*			inElement);

	// drawing

	virtual void			GetDrawContentsSelf(
									const STableCell&	inCell,
									SOutlineDrawContents& ioDrawContents);
	virtual void			DrawRowAdornments(
									const Rect&			inLocalRowRect);

	virtual LPane*			CreatePaneForCell(
									const STableCell&	inCell,
									ResIDT				inPPobID);

	// mouse-down behaviors

	virtual void			DoubleClick(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									const SOutlineDrawContents&	inDrawContents,
									Boolean					inHitText);

	// display synchronization

	virtual void			SetValueFromAttribute();
	virtual void			SetValueFromAttributeSelf();
	virtual void			SetValueFromDisplay();
	virtual void			SetValueFromDisplaySelf();
	
	virtual void			EnableSelf();
	virtual void			DisableSelf();

	// configuration

	virtual void			SetElementSelf(
									DMElement*			inElement);
	virtual void			CollectChanges(
									DMElement*			inElement);
	virtual void			ValueChanged(
									DM_ValueChange*		inMessage);


	// data member

protected:
	DMAttributePtr			mAttribute;


	// internal value message

	enum {
		msg_DisplayValueChanged = 'Dsp∆'				// message is used internally
	};

};
