// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSDuplicateResIDException.h	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:32:59 $
//  $History: RSDuplicateResIDException.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:27
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:53
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Exceptions
#include "DMValidationException.h"


// ===========================================================================
//		* RSDuplicateResIDException
// ===========================================================================
//
//	PRDuplicateResIDException is a validation exception that is
//	thrown whenever the user attempts to renumber one PRResource
//	such that its resource ID conflicts with an existing resource.
//
// ===========================================================================

class RSDuplicateResIDException : public DMValidationException {

public:
							RSDuplicateResIDException() {}
	virtual					~RSDuplicateResIDException() {}

	virtual void			GetErrorString(LStr255& outDescriptor) const;

};
