// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFData.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:28 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFData.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFData															  [public]
/**
	Default constructor	*/

CFData::CFData()
{
}


// ---------------------------------------------------------------------------
//	CFData															  [public]
/**
	Construct from an immutable CFDataRef */

CFData::CFData(
	CFDataRef	inDataRef,
	bool		inRetain)
	
	: BaseT(inDataRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFData															  [public]
/**
	Constructs from an existing CFMutableDataRef

	@param	inDataRef	CF data to adopt
	@param	inRetain	Whether to retain the CF data
	
	@note Pass false for inRetain if you are transferring ownership of the
	CF data, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF data.
	
	Pase true for inRetain if the caller wants to maintain ownership
	of the CF data. In this case, we make a copy, so that multiple
	clients aren't changing the same data.									*/

CFData::CFData(
	CFMutableDataRef	inDataRef,
	bool				inRetain)
{
	AttachMutableRef(inDataRef, inRetain);
}


// ---------------------------------------------------------------------------
//	CFData															  [public]
/**
	Constructs a CFData with the specified capacity
	
	@param	inCapacity		Size of the CFData
	@param	inAllocator		CF Allocator
	
	Specify 0 for inCapacity to create a CFData that can grow to arbitrary
	size. Otherwise, if you pass a postive value, it creates a fixed-size
	CFData.																	*/

CFData::CFData(
	CFIndex			inCapacity,
	CFAllocatorRef	inAllocator)
{
	CFMutableDataRef	ref = ::CFDataCreateMutable(inAllocator, inCapacity);
	PPx_ThrowIfCFCreateFailed_(ref, "CFDataCreateMutable");
	
	AttachMutableRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFData															  [public]
/**
	Constructs a CFData with the specified data
	
	@param	inBytes			Pointer to buffer of data to store
	@param	inByteCount		Size of buffer
	@param	inAllocator		CF Allocator									*/

CFData::CFData(
	const UInt8*	inBytes,
	CFIndex			inByteCount,
	CFAllocatorRef	inAllocator)
{
	CFDataRef	ref = ::CFDataCreate(inAllocator, inBytes, inByteCount);
	PPx_ThrowIfCFCreateFailed_(ref, "CFDataCreate");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFData															  [public]
/**
	Copy constructor */

CFData::CFData(
	const CFData&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */

CFData&
CFData::operator = (
	const CFData&	inSource)
{
	AssignObject(inSource);
	return *this;
}


// ---------------------------------------------------------------------------
//	GetLength														  [public]
/**
	Returns the Byte length of the data
	
	@return Byte lenght of the data											*/

CFIndex
CFData::GetLength() const
{
	return ::CFDataGetLength( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetBytePtr														  [public]
/**
	Returns a const pointer to the bytes in the CFData object
	
	@return Const pointer to the bytes in the CFData object					*/

const UInt8*
CFData::GetBytePtr() const
{
	return ::CFDataGetBytePtr( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetDataBytes													  [public]
/**
	Copies bytes from the CFData to a supplied buffer
	
	@param	inRange		Range of bytes to copy
	@param	outBuffer	Pointer to a buffer which must be large enough
							to store the requested bytes					*/

void
CFData::GetDataBytes(
	CFRange		inRange,
	UInt8*		outBuffer) const
{
	::CFDataGetBytes(UseRef(), inRange, outBuffer);
}


// ---------------------------------------------------------------------------
//	GetMutableBytePtr												  [public]
/**
	Returns a non-const pointer to the bytes in the CFData
	
	@return Non-const pointer to the bytes in the CFData					*/

UInt8*
CFData::GetMutableBytePtr()
{
	return ::CFDataGetMutableBytePtr( UseMutableRef() );
}


// ---------------------------------------------------------------------------
//	SetLength														  [public]
/**
	Sets the length of the CFData's internal buffer
	
	@param	inNewLength		New length for CFData's buffer
	
	Discards excess bytes if new length is less than current length.
	Zero fills extra bytes if new length is greater than current length.
	Trying to set the length greater than the capacity of a fixed-size
	CFData results in undefined behavior.									*/

void
CFData::SetLength(
	CFIndex	inNewLength)
{
	::CFDataSetLength(UseMutableRef(), inNewLength);
}


// ---------------------------------------------------------------------------
//	IncreaseLength													  [public]
/**
	Increases the size of the CFData's internal buffer
	
	@param	inExtraLength	Amount to increase size of internal buffer
	
	Zero fills extra bytes. Growing the length greater than the capacity
	of the fixed-size CGData results in undefined behavior					*/

void
CFData::IncreaseLength(
	CFIndex	inExtraLength)
{
	::CFDataIncreaseLength(UseMutableRef(), inExtraLength);
}


// ---------------------------------------------------------------------------
//	AppendBytes														  [public]
/**
	Appends bytes from a buffer to a CFData object
	
	@param	inBytes			Pointer to a buffer
	@param	inByteCount		Number of bytes from buffer to append			*/

void
CFData::AppendBytes(
	const UInt8*	inBytes,
	CFIndex			inByteCount)
{
	::CFDataAppendBytes(UseMutableRef(), inBytes, inByteCount);
}


// ---------------------------------------------------------------------------
//	ReplaceBytes													  [public]
/**
	Replaces a range within the CFData with new bytes from a buffer
	
	@param	inRange			Range within CFData to replace
	@param	inNewBytes		Pointer to a buffer
	@param	inByteCount		Number of bytes from buffer to copy				*/

void
CFData::ReplaceBytes(
	CFRange			inRange,
	const UInt8*	inNewBytes,
	CFIndex			inByteCount)
{
	::CFDataReplaceBytes(UseMutableRef(), inRange, inNewBytes, inByteCount);
}


// ---------------------------------------------------------------------------
//	DeleteBytes														  [public]
/**
	Deletes a range of bytes from the CFData
	
	@param	inRange		Range of bytes to delete							*/

void
CFData::DeleteBytes(
	CFRange			inRange)
{
	::CFDataDeleteBytes(UseMutableRef(), inRange);
}


// ---------------------------------------------------------------------------
//	MutableCopy														 [private]
/**
	Returns a mutable reference to a copy of the input CFDataRef
	
	@param	inRefToCopy		CFDataRef to copy
	
	@return	CFMutableDataRef to a copy of the input CFDataRef				*/

CFMutableDataRef
CFData::MutableCopy(
	CFDataRef	inRefToCopy) const
{
	CFMutableDataRef copyRef = ::CFDataCreateMutableCopy(
										::CFGetAllocator(inRefToCopy),
										cfSize_Unlimited, inRefToCopy);
	PPx_ThrowIfCFCreateFailed_(copyRef, "CFDataCreateMutableCopy");

	return copyRef;
}


} // namespace PPx