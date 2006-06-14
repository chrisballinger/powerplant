// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMLengthCheckException.h	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/02/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMLengthCheckException.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:31
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:21
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Exceptions'
//	Comment: Fixed CR/LF problem
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

	// Core : Data model : Core objects
#include "DMPointers.h"

	// Core : Data model : Exceptions
#include "DMValidationException.h"


// ===========================================================================
//		* DMLengthCheckException
// ===========================================================================
//
//	A DMLengthCheckException is thrown when a DMStringAttribute
//	receives a request to set its value to string that is too long.
//	This exception object retains the attempted string's length
//	(though not the actual string), and the maximum acceptable length.
//
// ===========================================================================

class DMLengthCheckException : public DMValidationException {

public:
							DMLengthCheckException(
									DMAttribute*	inAttribute,
									SInt32			inMaximumLength,
									SInt32			inAttemptedLength);
	virtual					~DMLengthCheckException();

	virtual void			GetErrorString(LStr255& outDescriptor) const;

protected:
	DMAttributePtr			mAttribute;
	SInt32					mMaximumLength;
	SInt32					mAttemptedLength;

};
