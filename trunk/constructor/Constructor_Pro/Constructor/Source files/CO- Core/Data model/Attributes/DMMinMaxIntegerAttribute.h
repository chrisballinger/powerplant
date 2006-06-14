// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMMinMaxIntegerAttribute.h	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/12/96
//     $Date: 2006/01/18 01:32:32 $
//  $History: DMMinMaxIntegerAttribute.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/07/97   Time: 14:11
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Rolled in Clint's fixes 02/07/97.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/13/96   Time: 13:48
//	Created in $/Constructor/Source files/Data model/Attributes
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"


// ===========================================================================
//		* DMMinMaxIntegerAttribute
// ===========================================================================
//
//	An integer attribute with explicit minimum and maximum values.
//
// ===========================================================================

class DMMinMaxIntegerAttribute : public DMIntegerAttribute {

	DMClassID_				('mimx', DMMinMaxIntegerAttribute, DMIntegerAttribute);

public:
							DMMinMaxIntegerAttribute();
							DMMinMaxIntegerAttribute(LStream* inStream);
							DMMinMaxIntegerAttribute(const DMMinMaxIntegerAttribute& inOriginal);
	virtual					~DMMinMaxIntegerAttribute();

	// value accessors
	
	virtual SInt32			GetMaxValue() const;
	virtual SInt32			GetMinValue() const;
	virtual void			SetMaxValue(SInt32 inValue)
									{ mMaximumValue = inValue; }
	virtual void			SetMinValue(SInt32 inValue)
									{ mMinimumValue = inValue; }
	// data members

protected:
	SInt32					mMinimumValue;				// minimum acceptable value
	SInt32					mMaximumValue;				// maximum acceptable value (inclusive)

};
