// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFNumber.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 05:04:51 $
//	$Revision: 1.2 $
// ===========================================================================
/**
	@file		SysCFNumber.h
	@brief		Wrapper class for Core Foundation Number
*/

#ifndef H_SysCFNumber
#define H_SysCFNumber
#pragma once

#include <SysCFObject.h>
#include <CoreFoundation/CFNumber.h>

namespace PPx {

// ===========================================================================
//	CFNumber
/**
	Wrapper class for Core Foundation Number */

class	CFNumber : public CFObject<CFNumberRef> {
public:
									// Constructors
						CFNumber();
						
						CFNumber(
								CFNumberRef	inNumberRef,
								bool		inRetain);
						
									// Signed integer constructors
	explicit			CFNumber(
								SInt8			inValue,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFNumber(
								SInt16			inValue,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFNumber(
								SInt32			inValue,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFNumber(
								SInt64			inValue,
								CFAllocatorRef	inAllocator = nil);
								
									// Unsigned integer constructors
	explicit			CFNumber(
								UInt8			inValue,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFNumber(
								UInt16			inValue,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFNumber(
								UInt32			inValue,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFNumber(
								UInt64			inValue,
								CFAllocatorRef	inAllocator = nil);
								
									// Floating point constructors
	explicit			CFNumber(
								float			inValue,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFNumber(
								double			inValue,
								CFAllocatorRef	inAllocator = nil);
								
						CFNumber( const CFNumber& inOriginal );

	CFNumber&			operator=( const CFNumber& inSource );

	CFNumberType		GetNumberType() const;
	
	CFIndex				GetByteSize() const;
	
	bool				IsFloatType() const;
	
	template <typename TNumber>
	TNumber				GetValue() const;

									// Comparison Functions
	CFComparisonResult	CompareTo( CFNumberRef inNumberRef ) const;		

private:
	typedef CFObject<CFNumberRef>	BaseT;
};


// ---------------------------------------------------------------------------
template <typename TNumber>
TNumber
CFNumber::GetValue() const
{
	TNumber	number = 0;;
	return number;
}

// ---------------------------------------------------------------------------
template <> inline
SInt8
CFNumber::GetValue() const
{
	SInt8	value;
	::CFNumberGetValue(UseRef(), kCFNumberSInt8Type, &value);
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
SInt16
CFNumber::GetValue() const
{
	SInt16	value;
	::CFNumberGetValue(UseRef(), kCFNumberSInt16Type, &value);
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
SInt32
CFNumber::GetValue() const
{
	SInt32	value;
	::CFNumberGetValue(UseRef(), kCFNumberSInt32Type, &value);
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
SInt64
CFNumber::GetValue() const
{
	SInt64	value;
	::CFNumberGetValue(UseRef(), kCFNumberSInt64Type, &value);
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
UInt8
CFNumber::GetValue() const
{
	UInt8	value;
	SInt16	bigValue;
	::CFNumberGetValue(UseRef(), kCFNumberSInt16Type, &bigValue);
	value = (UInt8) bigValue;
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
UInt16
CFNumber::GetValue() const
{
	UInt16	value;
	SInt32	bigValue;
	::CFNumberGetValue(UseRef(), kCFNumberSInt32Type, &bigValue);
	value = (UInt16) bigValue;
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
UInt32
CFNumber::GetValue() const
{
	UInt32	value;
	SInt64	bigValue;
	::CFNumberGetValue(UseRef(), kCFNumberSInt64Type, &bigValue);
	value = (UInt32) bigValue;
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
UInt64
CFNumber::GetValue() const
{
	UInt64	value;
	SInt64	bigValue;
	::CFNumberGetValue(UseRef(), kCFNumberSInt64Type, &bigValue);
	value = (UInt64) bigValue;
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
float
CFNumber::GetValue() const
{
	float	value;
	::CFNumberGetValue(UseRef(), kCFNumberFloatType, &value);
	return value;
}

// ---------------------------------------------------------------------------
template <> inline
double
CFNumber::GetValue() const
{
	double	value;
	::CFNumberGetValue(UseRef(), kCFNumberDoubleType, &value);
	return value;
}


// ===========================================================================
//	Global number operator<

inline bool operator<(const CFNumber& inLeft, const CFNumber& inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareLessThan);
}


inline bool operator<(const CFNumber& inLeft, CFNumberRef inRight)
{
	return (inLeft.CompareTo(inRight) == kCFCompareLessThan);
}


inline bool operator<(CFNumberRef inLeft, const CFNumber&inRight)
{
	return (inRight.CompareTo(inLeft) == kCFCompareLessThan);
}


} // namespace PPx

#endif // H_SysCFNumber
