// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMValidationException.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/29/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMValidationException.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:31
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/04/96   Time: 15:03
//	Updated in $/Constructor/Source files/Data model/Exceptions
//	Added ShowValidationFailedAlert method. Inlined constructor and
//	destructor.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : PP headers
#include <PP_Prefix.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LStr255;


// ===========================================================================
//		* DMValidationException									[abstract]
// ===========================================================================
//
//	DMValidationException is an abstract base class for exceptions
//	objects thrown when an unacceptable change to the data model
//	is attempted.
//
// ===========================================================================

class DMValidationException {

public:
							DMValidationException() {}
							virtual ~DMValidationException() {}
	
	// warning messages
	
	virtual void			ShowValidationFailedAlert() const;
	virtual void			GetErrorString(LStr255& outDescriptor) const = 0;

};
