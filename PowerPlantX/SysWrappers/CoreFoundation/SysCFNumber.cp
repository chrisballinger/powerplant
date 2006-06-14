// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFNumber.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:29 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFNumber.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/**	Default constructor

	@note Default construction does not create an underlying tree data
	structure. You must call AttachRef() to associate this object with a
	valid CFNumberRef before you can use it.									*/

CFNumber::CFNumber()
{
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a CFNumberRef.
	
	@note Although CFNumbers are always mutable, the Toolbox does not have a
	function to copy a CFNumber. So both the caller and this object share
	the CFNumberRef. Any change made to the tree will be reflected in all
	trees that share the same CFNumberRef.									*/

CFNumber::CFNumber(
	CFNumberRef	inNumberRef,
	bool		inRetain)
	
	: BaseT(inNumberRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a SInt8.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	SInt8			inValue,
	CFAllocatorRef	inAllocator)
{
	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt8Type, &inValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a UInt8.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	UInt8			inValue,
	CFAllocatorRef	inAllocator)
{
	SInt16		bigValue = inValue;

	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt16Type, &bigValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a SInt16.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	SInt16			inValue,
	CFAllocatorRef	inAllocator)
{
	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt16Type, &inValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a UInt16.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	UInt16			inValue,
	CFAllocatorRef	inAllocator)
{
	SInt32		bigValue = inValue;

	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt32Type, &bigValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a SInt32.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	SInt32			inValue,
	CFAllocatorRef	inAllocator)
{
	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt32Type, &inValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a UInt32.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	UInt32			inValue,
	CFAllocatorRef	inAllocator)
{
	SInt64		bigValue = inValue;

	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt64Type, &bigValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a SInt64.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	SInt64			inValue,
	CFAllocatorRef	inAllocator)
{
	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt64Type, &inValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a UInt64.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	UInt64			inValue,
	CFAllocatorRef	inAllocator)
{
	SInt64		bigValue = (SInt64) inValue;

	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberSInt64Type, &bigValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a float.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	float			inValue,
	CFAllocatorRef	inAllocator)
{
	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberFloatType, &inValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/** Constructs from a double.
	
	@param	inValue			Number to create the CFNumber from
	@param	inAllocator		The CFAllocator object to be used to allocate
							memory for the new object.						*/

CFNumber::CFNumber(
	double			inValue,
	CFAllocatorRef	inAllocator)
{
	CFNumberRef	ref = ::CFNumberCreate(inAllocator,
							kCFNumberDoubleType, &inValue);

	PPx_ThrowIfCFCreateFailed_(ref, "CFNumberCreate");

	AttachRef(ref, retain_No);
}

// ---------------------------------------------------------------------------
//	CFNumber														  [public]

/**
	Copy constructor */

CFNumber::CFNumber(
	const CFNumber&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator=														  [public]

/**
	Assignment operator */

CFNumber&
CFNumber::operator=(
	const CFNumber&	inSource)
{
	AssignObject(inSource);
	return *this;
}

#pragma mark -


// ---------------------------------------------------------------------------
//	GetNumberType													  [public]
/**
	Gets the internal storage type of the number.  It is not guaranteed to
	be the same as what it was created with.

	@return A CFNumberType constant indicating the data 					*/

CFNumberType
CFNumber::GetNumberType() const
{
	return ::CFNumberGetType(UseRef());
}


// ---------------------------------------------------------------------------
//	GetByteSize														  [public]
/**
	Gets the number of bytes used to store the value of the number
	
	@return Returns the number of bytes used to store its value				*/

CFIndex
CFNumber::GetByteSize() const
{
	return ::CFNumberGetByteSize(UseRef());
}


// ---------------------------------------------------------------------------
//	IsFloatType														  [public]
/**
	Determines whether a CFNumber object contains a value stored as one of the
	defined floating point types.
	
	@return True if the CFNumber object's value is one of the defined floating
			point types														*/

bool
CFNumber::IsFloatType() const
{
	return ::CFNumberIsFloatType(UseRef());
}


// ---------------------------------------------------------------------------
//	CompareTo														  [public]
/**
	Compares number to a CFNumberRef
	
	@param	inNumberRef		Number to which to compare
	
	@return Comparison result
	
	@retval	kCFCompareLessThan		Number is less than input number
	@retval	kCFCompareEqualTo		Number is equal to input number
	@retval	kCFCompareGreaterThan	Number is greater than input number		*/

CFComparisonResult
CFNumber::CompareTo( CFNumberRef inNumberRef ) const
{
	return ::CFNumberCompare(UseRef(), inNumberRef, nil);
}


} // namespace PPx