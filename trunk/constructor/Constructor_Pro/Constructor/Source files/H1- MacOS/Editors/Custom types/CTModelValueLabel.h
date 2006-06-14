// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTModelValueLabel.h		    © 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/10/96
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTModelValueLabel.h $
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
//	User: scouten      QDate: 12/10/96   Time: 14:23
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Custom types
#include "CTModelObject.h"


// ===========================================================================
//		* CTModelValueLabel
// ===========================================================================
//	Data model representation for a value label record in a CTYP resource.

class CTModelValueLabel : public CTModelObject {

	// data model description
	
	DMClassID_				('ctmv', CTModelValueLabel, CTModelObject);

	// constructor/destructor

public:
							CTModelValueLabel();
							CTModelValueLabel(LStream* inStream);
							CTModelValueLabel(const CTModelValueLabel& inOriginal);
	virtual					~CTModelValueLabel();
	
	virtual	Boolean			CanDeleteThis() const;
};
