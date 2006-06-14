// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTModelAttribute.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/10/96
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTModelAttribute.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Custom types
#include "CTModelObject.h"


// ===========================================================================
//		* CTModelAttribute
// ===========================================================================
//	A data model object representing an attribute in a CTYP resource.

class CTModelAttribute : public CTModelObject {

	// data model description
	
	DMClassID_				('ctma', CTModelAttribute, CTModelObject);

	// constructor/destructor

public:
							CTModelAttribute();
							CTModelAttribute(LStream* inStream);
							CTModelAttribute(const CTModelAttribute& inOriginal);
	virtual					~CTModelAttribute();

	virtual	Boolean			CanDeleteThis() const;
};
