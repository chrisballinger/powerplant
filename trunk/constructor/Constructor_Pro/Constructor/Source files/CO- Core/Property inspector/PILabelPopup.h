// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PILabelPopup.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/12/96
//     $Date: 2006/01/18 01:33:23 $
//	$History: PILabelPopup.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/01/97   Time: 14:57
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Isolated access to RF* classes behind Constructor_UseRF macro.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/21/96   Time: 16:12
//	Updated in $/Constructor/Source files/Property inspector
//	Switched from LGAPopup to LGAMiniArrowPopup in PI window. (Bug fix
//	#1132.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/13/96   Time: 14:12
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for resource list popups.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:40
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed current item highlighting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Display classes (Robin) : Grayscale futures
#include "LGAMiniArrowPopup.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMLabelledIntegerAttribute;
class DMResListAttribute;
class RFMap;


// ===========================================================================
//		* PILabelPopup
// ===========================================================================
//
//	PILabelPopup is a variant on the standard grayscale popup that knows
//	about the list of value labels. Used only by PILabelledItem.
//
// ===========================================================================

class PILabelPopup : public LGAMiniArrowPopup {

public:
	enum { class_ID = 'PIdp' };

	static void*				CreateFromStream(LStream* inStream)
										{ return new PILabelPopup(inStream); }

								PILabelPopup(LStream* inStream);
	virtual						~PILabelPopup();

	// value accessors

	inline SInt32				GetAttributeValue() const
										{ return mAttributeValue; }
	virtual void				SetAttributeValue(
										SInt32				inAttributeValue);
	virtual void				SetValue(
										SInt32				inValue);

	// configuration

	virtual void				SetAttribute(
										DMLabelledIntegerAttribute* inAttribute);

	// dynamic popup menu

protected:
	virtual MenuHandle			LoadPopupMenuH() const;
	virtual	void				SetPopupMinMaxValues();

	virtual void				BuildResListPopup(
										MenuHandle			inMenu,
										DMResListAttribute*	inResListAttr) const;
	virtual void				AddResources(
										RFMap*				inMap,
										LArray&				ioResources,
										ResType				inResType) const;


	// data members

protected:
	DMLabelledIntegerAttribute*	mAttribute;
	SInt32						mAttributeValue;

};


// ===========================================================================
//		* PIResListComparator
// ===========================================================================
//
//	Used internally by PILabelPopup to sort list of resources.
//
// ===========================================================================

class PIResListComparator : public LComparator {

public:
							PIResListComparator() {}
	virtual					~PIResListComparator() {}

	virtual SInt32			Compare(
									const void*		inItemOne,
									const void* 	inItemTwo,
									UInt32			inSizeOne,
									UInt32			inSizeTwo) const;

};
