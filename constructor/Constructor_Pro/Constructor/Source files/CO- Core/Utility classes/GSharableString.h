// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GSharableString.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/18/95
//     $Date: 2006/01/18 01:33:27 $
//	$History: GSharableString.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:32
//	Checked in '$/Constructor/Source files/CO- Core/Utility classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Feature classes
#include <LSharable.h>

	// PowerPlant : Support classes
#include <LString.h>


// ===========================================================================
//		* GSharableString
// ===========================================================================
//
//	GSharableString is an abstract base class for sharable string
//	objects that are templated (see TSharableString below). Sharable
//	strings are used throughout the data model for memory efficiency.
//
// ===========================================================================

class GSharableString :	public LString,
						public LSharable {

protected:
	GSharableString(SInt16 inMaxBytes, StringPtr inStringPtr);

};


// ===========================================================================
//		* TSharableString
// ===========================================================================
//	Templated implementation of GSharableString.

template <class T>
class TSharableString : public GSharableString {

public:
						TSharableString();
						TSharableString(const TSharableString& inOriginal);
						TSharableString(const LString& inOriginal);
						TSharableString(ConstStringPtr inStringPtr);
						TSharableString(UInt8 inChar);
						TSharableString(const char* inCString);
						TSharableString(const void* inPtr, UInt8 inLength);
						TSharableString(Handle inHandle);
						TSharableString(ResIDT inResID, SInt16 inIndex);
						TSharableString(SInt32 inNumber);
						TSharableString(double_t inNumber, SInt8 inStyle, SInt16 inDigits);
						TSharableString(FourCharCode inCode);

	// assignment operators

	TSharableString&	operator=(const LString& inString)
						 	{
								LString::operator=(inString);
								return *this;
							}
					
	TSharableString&	operator=(ConstStringPtr inStringPtr)
							{
								LString::operator=(inStringPtr);
								return *this;
							}
					
	TSharableString& 	operator=(UInt8 inChar)
							{
								LString::operator=(inChar);
								return *this;
							}
					
	TSharableString&	operator=(const char* inCString)
							{
								LString::operator=(inCString);
								return *this;
							}
					
	TSharableString&	operator=(SInt32 inNumber)
							{
								LString::operator=(inNumber);
								return *this;
							}
					
	TSharableString&	operator=(FourCharCode inCode)
							{
								LString::operator=(inCode);
								return *this;
							}

private:
	T					mString;

};
