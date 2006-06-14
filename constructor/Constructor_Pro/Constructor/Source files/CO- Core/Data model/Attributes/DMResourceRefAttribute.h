// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMResourceRefAttribute.h	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 01/30/97
//     $Date: 2006/01/18 01:32:32 $
//  $History: DMResourceRefAttribute.h $
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 01/31/97   Time: 11:55
//	Created
//	Comment: Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"


// ===========================================================================
//		* DMResourceRefAttribute
// ===========================================================================
//
//	DMResourceRefAttribute implements an integer attribute with an associated
//	resource type.  The integer thus represents a resource ID, and the whole
//	attribute can be used to point to a resource.
//
// ===========================================================================

class DMResourceRefAttribute : 	public DMIntegerAttribute {

	// data model description

	DMClassID_				('rerf', DMResourceRefAttribute, DMIntegerAttribute);

	// constructor/destructor

public:
							DMResourceRefAttribute();
							DMResourceRefAttribute(LStream* inStream);
							DMResourceRefAttribute(const DMResourceRefAttribute& inOriginal);
	virtual					~DMResourceRefAttribute() { }

	// value accessors

	ResType					GetResourceType() const
									{ return mResourceType; }

	// data members

protected:
	ResType					mResourceType;

};
