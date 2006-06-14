// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFString.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:30 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFString.h
	@brief		Wrapper class for Core Foundation String
*/

#ifndef H_SysCFString
#define H_SysCFString
#pragma once

#include <SysCFMutableObject.h>

#include <CoreFoundation/CoreFoundation.h>

#include <string>
#include <sstream>

namespace PPx {

// ===========================================================================
//	CFString
/**
	Wrapper class for Core Foundation String */

class	CFString : public CFMutableObject<CFStringRef, CFMutableStringRef> {
public:			
									// Constructors
						CFString();
						
						CFString(
								CFStringRef	inStringRef,
								bool		inRetain = true);
						
						CFString(
								CFMutableStringRef	inStringRef,
								bool				inRetain);
						
	explicit			CFString(
								ConstStringPtr		inPascalString,
								CFStringEncoding	inEncoding = encoding_System,
								CFAllocatorRef		inAllocator = nil);
						
	explicit			CFString(
								const char*			inCString,
								CFStringEncoding	inEncoding = encoding_System,
								CFAllocatorRef		inAllocator = nil);
								
						CFString(
								const UniChar*		inUniChars,
								CFIndex				inCharCount,
								CFAllocatorRef		inAllocator = nil);
								
	explicit			CFString(
								const HFSUniStr255&	inHFSUniStr,
								CFAllocatorRef		inAllocator = nil);
								
						CFString(
								const void*			inBuffer,
								CFIndex				inByteCount,
								CFStringEncoding	inEncoding = encoding_System,
								bool				inIsExternalRep = false,
								CFAllocatorRef		inAllocator = nil);
								
	explicit			CFString(
								const std::string&	inString,
								CFStringEncoding	inEncoding = encoding_System,
								CFAllocatorRef		inAllocator = nil);
								
						CFString( const CFString& inOriginal );
						
	CFString&			operator = ( const CFString& inSource );
	
									// Accessors
	CFIndex				GetLength() const;
	
	CFIndex				GetByteLength(
								CFRange				inRange = cfRange_All,
								CFStringEncoding	inEncoding = encoding_System,
								UInt8				inLossByte = 0,
								bool				inIsExternalRep = false) const;
								
	CFIndex				GetByteRange(
								CFIndex				inBufferSize,
								UInt8*				outBuffer,
								CFRange				inRange = cfRange_All,
								CFStringEncoding	inEncoding = encoding_System,
								UInt8				inLossByte = 0,
								bool				inIsExternalRep = false) const;
								
	UniChar				GetCharacterAt( CFIndex inIndex ) const;
	
	UniChar				operator [] ( CFIndex inIndex ) const;
	
	void				GetSubstring(
								CFRange		inRange,
								UniChar*	outBuffer) const;
								
	const UniChar*		GetUniStringPtr() const;

	bool				GetPascalString(
								StringPtr			outBuffer,
								CFIndex				inBufferSize,
								CFStringEncoding	inEncoding = encoding_System) const;
								
	ConstStringPtr		GetPascalStringPtr( CFStringEncoding inEncoding = encoding_System ) const;
	
	bool				GetCString(
								char*				outBuffer,
								CFIndex				inBufferSize,
								CFStringEncoding	inEncoding = encoding_System) const;
								
	const char*			GetCStringPtr( CFStringEncoding inEncoding = encoding_System ) const;
								
	void				GetString(
								std::string&		outString,
								CFStringEncoding	inEncoding = encoding_System) const;
								
	FourCharCode		Get4CharCodeValue() const;
								
	template <typename TNumber>
	TNumber				GetNumericValue() const;
	
									// Mutators
	void				Append( CFStringRef inString );

	void				Append(	const UniChar*	inChars,
								CFIndex			inCharCount);

	void				Append(	ConstStringPtr		inPascalString,
								CFStringEncoding	inEncoding = encoding_System);

	void				Append(	const char*			inCString,
								CFStringEncoding	inEncoding = encoding_System);

	void				Insert(	CFIndex		inIndex,
								CFStringRef	inString);
								
	void				Pad(	CFStringRef	inPadString,
								CFIndex		inLength,
								CFIndex		inPadPosition);

	void				Delete(	CFRange inRange );

	void				Replace(
								CFRange		inRange,
								CFStringRef	inReplacement);

	void				ReplaceAll( CFStringRef inReplacement );
	
	template <typename TNumber>
	void				AssignNumericValue(
								TNumber			inNumber,
								CFAllocatorRef	inAllocator = nil);
								
	void				AssignNumericValue(
								SInt8			inNumber,
								CFAllocatorRef	inAllocator = nil);
								
	void				AssignNumericValue(
								UInt8			inNumber,
								CFAllocatorRef	inAllocator = nil);
								
	void				Assign4CharCode(
								FourCharCode	inCode,
								CFAllocatorRef	inAllocator = nil);

									// Comparison Functions
	CFComparisonResult	CompareTo(
								CFStringRef		inStringRef,
								CFOptionFlags	inOptions = 0) const;		
								
									// Searching
	CFRange				FindInRange(
								CFRange			inRange,
								CFStringRef		inSearchStr,
								CFOptionFlags	inOptions = 0) const;
	
private:
	virtual CFMutableStringRef
						MutableCopy( CFStringRef inRefToCopy ) const;

private:	
	typedef CFMutableObject<CFStringRef, CFMutableStringRef>	BaseT;
};


// ---------------------------------------------------------------------------
//	GetNumericValue
/**
	Template function for returning a numeric value from a string.
	Templater parameter is the numeric type.
	
	@return	Numeric value represented by the string							*/

template <typename TNumber>
TNumber
CFString::GetNumericValue() const
{
	std::string streamString;
	GetString(streamString, kCFStringEncodingASCII);
	std::istringstream stringToValue(streamString);

	TNumber number;
	stringToValue.operator >> (number);

	return number;
}


// ---------------------------------------------------------------------------
//	GetNumericValue < SInt8 >			Template Specialization

template <> inline
SInt8
CFString::GetNumericValue() const
{
	return ::CFStringGetIntValue( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetNumericValue < UInt8 >			Template Specialization

template <> inline
UInt8
CFString::GetNumericValue() const
{
	return ::CFStringGetIntValue( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetNumericValue < SInt16 >			Template Specialization

template <> inline
SInt16
CFString::GetNumericValue() const
{
	return ::CFStringGetIntValue( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetNumericValue < UInt16 >			Template Specialization

template <> inline
UInt16
CFString::GetNumericValue() const
{
	return ::CFStringGetIntValue( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetNumericValue < SInt32 >			Template Specialization

template <> inline
SInt32
CFString::GetNumericValue() const
{
	return ::CFStringGetIntValue( UseRef() );
}


// ---------------------------------------------------------------------------
//	AssignNumericValue
/**
	Template function for setting a string from a numeric value.
	Template parameter is the numeric type.
	
	@param	inNumber		Number value
	@param	inAllocator		CF Allocator									*/

template <typename TNumber>
void
CFString::AssignNumericValue(
	TNumber			inNumber,
	CFAllocatorRef	inAllocator)
{
	std::ostringstream numberStream;
	numberStream << inNumber;
	
	AssignObject( CFString( numberStream.str(),
							kCFStringEncodingMacRoman,
							inAllocator ) );
}


// ---------------------------------------------------------------------------
//	AssignNumericValue < SInt8 >

inline void
CFString::AssignNumericValue(
	SInt8			inNumber,
	CFAllocatorRef	inAllocator)
{
		// We need an overload since std::ostream doesn't support
		// 8-bit numeric types
		
	AssignNumericValue(SInt16(inNumber), inAllocator);
}


// ---------------------------------------------------------------------------
//	AssignNumericValue < UInt8 >

inline void
CFString::AssignNumericValue(
	UInt8			inNumber,
	CFAllocatorRef	inAllocator)
{
		// We need an overload since std::ostream doesn't support
		// 8-bit numeric types
		
	AssignNumericValue(UInt16(inNumber), inAllocator);
}


// ===========================================================================
//	Global string operator ==

inline bool operator == (const CFString& inLeft, const CFString& inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareEqualTo);
}


inline bool operator == (const CFString& inLeft, CFStringRef inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareEqualTo);
}


inline bool operator == (CFStringRef inLeft, const CFString&inRight)
{
	return (inRight.CompareTo(inLeft) == kCFCompareEqualTo);
}


// ===========================================================================
//	Global string operator !=

inline bool operator != (const CFString& inLeft, const CFString& inRight)
{
	return (inLeft.CompareTo(inRight) != kCFCompareEqualTo);
}


inline bool operator != (const CFString& inLeft, CFStringRef inRight)
{
	return (inLeft.CompareTo(inRight) != kCFCompareEqualTo);
}


inline bool operator != (CFStringRef inLeft, const CFString&inRight)
{
	return (inRight.CompareTo(inLeft) != kCFCompareEqualTo);
}


// ===========================================================================
//	Global string operator <

inline bool operator < (const CFString& inLeft, const CFString& inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareLessThan);
}


inline bool operator < (const CFString& inLeft, CFStringRef inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareLessThan);
}


inline bool operator < (CFStringRef inLeft, const CFString&inRight)
{
	return (inRight.CompareTo(inLeft) == kCFCompareLessThan);
}


// ===========================================================================
//	Global string operator <=

inline bool operator <= (const CFString& inLeft, const CFString& inRight)
{
	return (inLeft.CompareTo(inRight) != kCFCompareGreaterThan);
}


inline bool operator <= (const CFString& inLeft, CFStringRef inRight)
{
	return (inLeft.CompareTo(inRight) != kCFCompareGreaterThan);
}


inline bool operator <= (CFStringRef inLeft, const CFString&inRight)
{
	return (inRight.CompareTo(inLeft) != kCFCompareGreaterThan);
}


// ===========================================================================
//	Global string operator >

inline bool operator > (const CFString& inLeft, const CFString& inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareGreaterThan);
}


inline bool operator > (const CFString& inLeft, CFStringRef inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareGreaterThan);
}


inline bool operator > (CFStringRef inLeft, const CFString&inRight)
{
	return (inRight.CompareTo(inLeft) == kCFCompareGreaterThan);
}


// ===========================================================================
//	Global string operator >=

inline bool operator >= (const CFString& inLeft, const CFString& inRight)
{
	return (inLeft.CompareTo(inRight) != kCFCompareLessThan);
}


inline bool operator >= (const CFString& inLeft, CFStringRef inRight)
{
	return (inLeft.CompareTo(inRight) != kCFCompareLessThan);
}


inline bool operator >= (CFStringRef inLeft, const CFString&inRight)
{
	return (inRight.CompareTo(inLeft) != kCFCompareLessThan);
}


// ===========================================================================
//	Global string operator +

CFString	operator + (const CFString& inLeft, const CFString& inRignt);

CFString	operator + (const CFString& inLeft, CFStringRef inRignt);
CFString	operator + (CFStringRef inLeft, const CFString& inRignt);

CFString	operator + (const CFString& inLeft, ConstStringPtr inRignt);
CFString	operator + (ConstStringPtr inLeft, const CFString& inRignt);

CFString	operator + (const CFString& inLeft, const char* inRignt);
CFString	operator + (const char* inLeft, const CFString& inRignt);


} // namespace PPx

#endif // H_SysCFString
