// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMRangeCheckException.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/29/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMRangeCheckException.h $
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
//	User: scouten      QDate: 12/05/96   Time: 12:40
//	Updated in $/Constructor/Source files/Data model/Exceptions
//	Moved error strings to STR# resource. Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMAttribute.h"

	// Core : Data model : Exceptions
#include "DMValidationException.h"


// ===========================================================================
//		* DMRangeCheckException
// ===========================================================================
//
//	A DMRangeCheckException is thrown when a DMIntegerAttribute
//	receives a request to set its value to an unacceptable value.
//	This exception object retains the attempted value, and the
//	minimum and maximum acceptable values.
//
// ===========================================================================

class DMRangeCheckException : public DMValidationException {

public:
							DMRangeCheckException(
									DMAttribute*	inAttribute,
									SInt32			inMinimumValue,
									SInt32			inMaximumValue,
									SInt32			inAttemptedValue);
	virtual					~DMRangeCheckException();
	
	virtual void			GetErrorString(
									LStr255&		outDescriptor) const;


	// data members

protected:
	DMAttributePtr			mAttribute;				// attribute whose value was changed
	SInt32					mMinimumValue;			// minimum acceptable value (inclusive)
	SInt32					mMaximumValue;			// maximum acceptable value (inclusive)
	SInt32					mAttemptedValue;		// value that was attempted

};
