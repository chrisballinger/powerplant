// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIResourceRefItem.h			   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/30/97
//     $Date: 2006/01/18 01:33:24 $
//	$History: PIResourceRefItem.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:49
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for ValuesOnly.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 02/01/97   Time: 13:44
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed some problems with the RF/RM class refactoring.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 01/31/97   Time: 11:50
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

class RFMap;
class DMResourceRefAttribute;
class RSContainer;
class LPushButton;

// ===========================================================================
//		* PIResourceRefItem
// ===========================================================================
//	PIResourceRefItem is an attribute for DMResourceRefAttributes.

class PIResourceRefItem : public PIStringItem {

public:
								PIResourceRefItem();
	virtual						~PIResourceRefItem() {}

	// configuration

protected:
	virtual void				FinishCreate();
	virtual void				SetElementSelf(
									DMElement*			inElement);

	// drawing & editing

	virtual void				GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);

	virtual void				SetValueFromAttributeSelf();

	virtual void				DisableSelf();
	virtual void				EnableSelf();

	virtual void				ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);

protected:
	virtual RSContainer*		GetContainer();
	virtual void				UpdateButtonTitle();
	virtual void				SetButtonEdit();
	virtual void				SetButtonCreate();
	

	// data members

protected:
	LPushButton*					mButton;
	RFMap*						mMap;
	DMResourceRefAttribute*		mRefAttribute;
	
	enum {
		msg_EditRef = 	'refE',				// message is used internally
		msg_CreateRef = 'refC'				// message is used internally
	};
};