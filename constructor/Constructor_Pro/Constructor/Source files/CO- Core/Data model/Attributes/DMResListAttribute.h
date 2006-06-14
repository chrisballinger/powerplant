// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMResListAttribute.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/12/96
//     $Date: 2006/01/18 01:32:32 $
//  $History: DMResListAttribute.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/13/96   Time: 13:51
//	Created in $/Constructor/Source files/Data model/Attributes
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMLabelledIntegerAttribute.h"


// ===========================================================================
//		* DMResListAttribute
// ===========================================================================
//
//	An integer attribute that holds a resource ID. The property inspector
//	provided a popup of all resources of this type.
//
// ===========================================================================

class DMResListAttribute : public DMLabelledIntegerAttribute {

	// data model description

	DMClassID_				('resl', DMResListAttribute, DMLabelledIntegerAttribute);

	// constructor/destructor

public:
							DMResListAttribute();
							DMResListAttribute(LStream* inStream);
							DMResListAttribute(const DMResListAttribute& inOriginal);
	virtual					~DMResListAttribute();

	// accessors

	inline ResType			GetResType() const
									{ return mResType; }


	// data model

protected:
	ResType					mResType;

};
