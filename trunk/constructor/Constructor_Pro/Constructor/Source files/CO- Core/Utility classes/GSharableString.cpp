// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GSharableString.cpp			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/18/95
//     $Date: 2006/01/18 01:33:27 $
//	$History: GSharableString.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 03/05/97   Time: 09:36
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Added a workaround for CW Win32 compiler.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "GSharableString.h"


// ===========================================================================

#pragma mark *** GSharableString ***

// ---------------------------------------------------------------------------
//		* GSharableString(Int16, StringPtr)
// ---------------------------------------------------------------------------

GSharableString::GSharableString(
	SInt16		inMaxBytes,
	StringPtr	inStringPtr)

: LString(inMaxBytes, inStringPtr)
{
}


// ===========================================================================

#pragma mark -
#pragma mark *** TSharableString ***

// ---------------------------------------------------------------------------
//		* TSharableString
// ---------------------------------------------------------------------------
//	Default Constructor

template <class T>
TSharableString<T>::TSharableString()

: GSharableString(sizeof(mString), mString)
{
	mString[0] = 0;
}


// ---------------------------------------------------------------------------
//		* TSharableString(TSharableString&)
// ---------------------------------------------------------------------------
//	Copy Constructor

template <class T>
TSharableString<T>::TSharableString(
	const TSharableString& inOriginal)

: GSharableString(sizeof (mString), mString)
{
	LoadFromStringPtr(inOriginal);
}


// ---------------------------------------------------------------------------
//		* TSharableString(LString&)
// ---------------------------------------------------------------------------
//	Copy Constructor

template <class T>
TSharableString<T>::TSharableString(
	const LString&	inOriginal)

: GSharableString(sizeof (mString), mString)
{
	LoadFromStringPtr(inOriginal);
}


// ---------------------------------------------------------------------------
//		* TSharableString(ConstStringPtr&)
// ---------------------------------------------------------------------------
//	Constructor from a pointer to a string

template <class T>
TSharableString<T>::TSharableString(
	ConstStringPtr inStringPtr)

: GSharableString(sizeof (mString), mString)
{
	LoadFromStringPtr(inStringPtr);
}


// ---------------------------------------------------------------------------
//		* TSharableString(Uint8)
// ---------------------------------------------------------------------------

template <class T>
TSharableString<T>::TSharableString(
	UInt8 inChar)

: GSharableString(sizeof (mString), mString)
{
	mString[0] = 1;
	mString[1] = inChar;
}


// ---------------------------------------------------------------------------
//		* TSharableString(char*)
// ---------------------------------------------------------------------------
//	Constructor from a C string (null terminated)

template <class T>
TSharableString<T>::TSharableString(
	const char* inCString)

: GSharableString(sizeof (mString), mString)
{
	Assign(inCString, CStringLength(inCString));
}	


// ---------------------------------------------------------------------------
//		* TSharableString(const Uint8*, Uint8)
// ---------------------------------------------------------------------------
//	Constructor from a pointer and length

template <class T>
TSharableString<T>::TSharableString(
	const void*	inPtr,
	UInt8		inLength)

: GSharableString(sizeof (mString), mString)
{
	LoadFromPtr(inPtr, inLength);
}


// ---------------------------------------------------------------------------
//		* TSharableString(Handle)
// ---------------------------------------------------------------------------
//	Constructor from the data in a Handle

template <class T>
TSharableString<T>::TSharableString(
	Handle inHandle)

: GSharableString(sizeof (mString), mString)
{
	Assign(inHandle);
}


// ---------------------------------------------------------------------------
//		* TSharableString(ResIDT, Int16)
// ---------------------------------------------------------------------------
//	Constructor from a 'STR ' or 'STR#' resource
//
//	The value of inIndex determines whether to use a 'STR ' or 'STR#'.
//	Uses 'STR ' for inIndex <= 0, and 'STR#' for inIndex >= 1. 

template <class T>
TSharableString<T>::TSharableString(
	ResIDT	inResID,
	SInt16	inIndex)

: GSharableString(sizeof (mString), mString)
{
	Assign(inResID, inIndex);
}


// ---------------------------------------------------------------------------
//		* TSharableString(Int32)
// ---------------------------------------------------------------------------
//	Constructor from a long integer

template <class T>
TSharableString<T>::TSharableString(
	SInt32 inNumber)

: GSharableString(sizeof (mString), mString)
{
	Assign(inNumber);
}


// ---------------------------------------------------------------------------
//		* TSharableString(double_t, Int8, Int16)
// ---------------------------------------------------------------------------
//	Assignment from floating point number
//
//	inStyle must be FLOATDECIMAL or FIXEDDECIMAL
//
//	For FLOATDECIMAL, inDigits is the number of significant digits
//		(should be > 0)
//	For FIXEDDECIMAL, inDigits is the number of digits to the right
//		of the decimal point

template <class T>
TSharableString<T>::TSharableString(
	double_t	inNumber,
	SInt8		inStyle,
	SInt16	inDigits)

: GSharableString(sizeof (mString), mString)
{
	Assign(inNumber, inStyle, inDigits);
}


// ---------------------------------------------------------------------------
//		* TSharableString(FourCharCode)
// ---------------------------------------------------------------------------
//	Constructor from a four character code, which is an unsigned long,
//	the same as ResType and OSType

template <class T>
TSharableString<T>::TSharableString(
	FourCharCode inCode)

: GSharableString(sizeof (mString), mString)
{
	Assign(inCode);
}


// ===========================================================================

#pragma mark -
#pragma mark *** template instantiations ***

// ===========================================================================
//	* Template Instantiations
// ===========================================================================
//	Instantiate Template classes for all String types declared in <Types.h>

template class TSharableString<Str255>;
#if !(defined(WINVER) && defined(__MWERKS__))	// work around x86 name-mangling bug
template class TSharableString<Str63>;
template class TSharableString<Str32>;
template class TSharableString<Str31>;
template class TSharableString<Str27>;
template class TSharableString<Str15>;
#endif