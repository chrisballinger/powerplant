// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFArray.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:28 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file	SysCFArray.h
	@brief	Template class wrapper for a Core Foundation Array
*/

#ifndef H_SysCFArray
#define H_SysCFArray
#pragma once

#include <SysCFMutableObject.h>
#include <SysCFUtils.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFArray
/**
	Template class wrapper for a Core Foundation Array

	The template parameter specifies the type of values stored in the
	CFArray. As with all CF containers, the sizeof(TValue) must be 4 bytes.
	Consider using std::vector or std::deque if you need to store values
	that are not 4 bytes.													*/

template <typename TValue>
class	CFArray : public CFMutableObject<CFArrayRef, CFMutableArrayRef> {
public:
									// Constructors
						CFArray();

						CFArray(
							CFArrayRef	inCFArray,
							bool		inRetain);

						CFArray(
							CFMutableArrayRef	inCFArray,
							bool				inRetain);

	explicit			CFArray(
								CFIndex					inCapacity,
								const CFArrayCallBacks*	inCallBacks = nil,
								CFAllocatorRef			inAllocator = nil);

						CFArray(
								const TValue*			inValues,
								CFIndex 				inValueCount,
								const CFArrayCallBacks*	inCallBacks = nil,
								CFAllocatorRef			inAllocator = nil);

						CFArray( const CFArray& inOriginal );

	CFArray&			operator = ( const CFArray& inSource );
	
									// Accessors
	CFIndex				GetCount() const;

	bool				IsEmpty() const;

	CFIndex				GetCountOfValue(
								TValue	inValue,
								CFRange	inRange = cfRange_All) const;

	bool				ContainsValue(
								TValue	inValue,
								CFRange	inRange = cfRange_All) const;

	TValue				GetValueAt( CFIndex inIndex ) const;

	TValue				operator [] ( CFIndex inIndex ) const;

	void				GetValues(
								TValue*	outValues,
								CFRange	inRange = cfRange_All) const;

	CFIndex				GetFirstIndexOf(
								TValue	inValue,
								CFRange	inRange = cfRange_All) const;

	CFIndex				GetLastIndexOf(
								TValue	inValue,
								CFRange	inRange = cfRange_All) const;

	void				AppendValue( TValue inValue );

	void				InsertValueAt(
								CFIndex	inIndex,
								TValue	inValue);

	void				SetValueAt(
								CFIndex	inIndex,
								TValue	inValue);

	void				RemoveValueAt( CFIndex inIndex );

	void				RemoveAllValues();

	void				ReplaceValues(
							CFRange			inRange,
							const TValue*	inValues,
							CFIndex			inValueCount);

	void				ExchangeValuesAt(
								CFIndex	inIndexOne,
								CFIndex	inIndexTwo);

	CFIndex				BinarySearchFor(
								TValue					inValue,
								CFComparatorFunction	inComparator,
								void*					inParam,
								CFRange					inRange = cfRange_All);

	void				Sort(	CFComparatorFunction	inComparator,
								void*					inParam,
								CFRange					inRange = cfRange_All);

	void				ApplyFunction(
								CFArrayApplierFunction	inFunction,
								void*					inParam,
								CFRange					inRange = cfRange_All) const;

private:
	virtual CFMutableArrayRef
						MutableCopy( CFArrayRef inRefToCopy ) const;

private:						
	typedef CFMutableObject<CFArrayRef, CFMutableArrayRef>	BaseT;
};


// ---------------------------------------------------------------------------
//	CFArray															  [public]
/**
	Default constructor */

template <class TValue>
CFArray<TValue>::CFArray()
{
}


// ---------------------------------------------------------------------------
//	CFArray															  [public]
/**
	Constructs from an existing immutable CF array

	@param	inArrayRef		CF array to adopt
	@param	inRetain		Whether to retain the CF array
	
	@note Pass false for inRetain if you are transferring ownership of the
	CF array, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF array.
	
	Pase true for inRetain if the caller wants to maintain shared ownership
	of the CF array.														*/

template <class TValue>
CFArray<TValue>::CFArray(
	CFArrayRef	inArrayRef,
	bool		inRetain)
	
	: BaseT(inArrayRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFArray															  [public]
/**
	Constructs from an existing mutable CF array

	@param	inArrayRef		CF array to adopt
	@param	inRetain		Whether to retain the CF array
	
	@note Pass false for inRetain if you are transferring ownership of the
	CF array, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF array.
	
	Pase true for inRetain if the caller wants to maintain ownership
	of the CF array. In this case, we make a copy, so that multiple
	clients aren't changing the same data.									*/

template <class TValue>
CFArray<TValue>::CFArray(
	CFMutableArrayRef	inArrayRef,
	bool				inRetain)
{
	AttachMutableRef(inArrayRef, inRetain);
}


// ---------------------------------------------------------------------------
//	CFArray															  [public]
/**
	Constructs an array with the specified capacity
		
	@param	inCapacity		Maximum number of items in the array. Use
								cfSize_Unlimited for an unbounded size.
	@param	inCallBacks		Callback functions for managing array values.
								Use &kCFTypeArrayCallBacks if TValue is a
								CF reference type.
	@param	inAllocator		Allocator for memory used by the array			*/

template <class TValue>
CFArray<TValue>::CFArray(
	CFIndex					inCapacity,
	const CFArrayCallBacks*	inCallBacks,
	CFAllocatorRef			inAllocator)
{
	CFMutableArrayRef	ref = ::CFArrayCreateMutable(inAllocator, inCapacity,
														inCallBacks);
	PPx_ThrowIfCFCreateFailed_(ref, "CFArrayCreateMutable");
	
	AttachMutableRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFArray															  [public]
/**
	Constructs from a buffer of values

	@param	inValues		Pointer to buffer of values to store in array
	@param	inValuesCount	Number of values
	@param	inCallBacks		Callback functions for managing array values.
								Use &kCFTypeArrayCallBacks if TValue is a
								CF reference type.
	@param	inAllocator		Allocator for memory used by the array			*/

template <class TValue>
CFArray<TValue>::CFArray(
	const TValue*			inValues,
	CFIndex					inValuesCount,
	const CFArrayCallBacks*	inCallBacks,
	CFAllocatorRef			inAllocator)
{
	CFArrayRef	ref = ::CFArrayCreate(inAllocator, (const void**) inValues,
										inValuesCount, inCallBacks);
	PPx_ThrowIfCFCreateFailed_(ref, "CFArrayCreate");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFArray															  [public]
/**
	Copy constructor */

template <class TValue>
CFArray<TValue>::CFArray(
	const CFArray&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */

template <class TValue>
CFArray<TValue>&
CFArray<TValue>::operator = (
	const CFArray&	inSource)
{
	BaseT::AssignObject(inSource);
	return *this;
}


// ---------------------------------------------------------------------------
//	GetCount														  [public]
/**
	Returns the number of items in the array

	@return	Number of items in the array									*/

template <class TValue>
CFIndex
CFArray<TValue>::GetCount() const
{
	return (::CFArrayGetCount( UseRef() ));
}


// ---------------------------------------------------------------------------
//	IsEmpty															  [public]
/**
	Returns whether the array has no items
	
	@return Whether the array has no items									*/

template <class TValue>
bool
CFArray<TValue>::IsEmpty() const
{
	return (GetCount() == 0);
}


// ---------------------------------------------------------------------------
//	GetCountOfValue													  [public]
/**
	Returns number of items within range that have the specified value
	
	@param	inValue		Value to look for
	@param	inRange		Range which to search
	
	@return Number of itemw within range that have the specified value		*/

template <class TValue>
CFIndex
CFArray<TValue>::GetCountOfValue(
	TValue	inValue,
	CFRange	inRange) const
{
	CFUtils::VerifyRange(GetCount(), inRange);
	return ::CFArrayGetCountOfValue(UseRef(), inRange,
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	ContainsValue													  [public]
/**
	Returns whether the specified value is within a range of the array
	
	@param	inValue		Value to look for
	@param	inRange		Range which to search
	
	@return Whether the specified value is within a range of the array		*/

template <class TValue>
bool
CFArray<TValue>::ContainsValue(
	TValue	inValue,
	CFRange	inRange) const
{
	CFUtils::VerifyRange(GetCount(), inRange);
	return ::CFArrayContainsValue(UseRef(), inRange,
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	GetValueAt														  [public]
/**
	Returns value at a specified index in the array
	
	@param	inIndex		Index into array
	
	@return Value at a specified index in the array							*/

template <class TValue>
TValue
CFArray<TValue>::GetValueAt(
	CFIndex	inIndex) const
{
	CFUtils::VerifyIndex(GetCount(), inIndex);
	return reinterpret_cast<TValue>(::CFArrayGetValueAtIndex(UseRef(), inIndex));
}


// ---------------------------------------------------------------------------
//	operator []														  [public]
/**
	Returns value at a specified index in the array
	
	@param	inIndex		Index into array
	
	@return Value at a specified index in the array							*/

template <class TValue>
TValue
CFArray<TValue>::operator [] (
	CFIndex	inIndex) const
{
	return GetValueAt(inIndex);
}


// ---------------------------------------------------------------------------
//	GetValues														  [public]
/**
	Copies values from a range of the array into a buffer
	
	@param	outValues	Pointer to buffer
	@param	inRange		Range to copy
	
	Buffer must be large enough to hold all values, at least
	sizeof(TValue) * length of range										*/

template <class TValue>
void
CFArray<TValue>::GetValues(
	TValue*	outValues,
	CFRange	inRange) const
{
	PPx_BadParamIfNil_(outValues);
	CFUtils::VerifyRange(GetCount(), inRange);

	::CFArrayGetValues(UseRef(), inRange, (const void**) outValues);
}


// ---------------------------------------------------------------------------
//	GetFirstIndexOf													  [public]
/**
	Returns the first index at which the specified value occurs within a
	range of the array
	
	@param	inValue		Value to look for
	@param	inRange		Range to search
	
	@return	First index at which the values occurs with the range			*/

template <class TValue>
CFIndex
CFArray<TValue>::GetFirstIndexOf(
	TValue	inValue,
	CFRange	inRange) const
{
	CFUtils::VerifyRange(GetCount(), inRange);
	return ::CFArrayGetFirstIndexOfValue(UseRef(), inRange,
									reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	GetLastIndexOf													  [public]
/**
	Returns the last index at which the specified value occurs within a
	range of the array
	
	@param	inValue		Value to look for
	@param	inRange		Range to search
	
	@return	Last index at which the values occurs with the range			*/

template <class TValue>
CFIndex
CFArray<TValue>::GetLastIndexOf(
	TValue	inValue,
	CFRange	inRange) const
{
	CFUtils::VerifyRange(GetCount(), inRange);
	return ::CFArrayGetLastIndexOfValue(UseRef(), inRange,
									reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	AppendValue														  [public]
/**
	Appends a value to the end of the array
	
	@param	inValue		Value to append										*/

template <class TValue>
void
CFArray<TValue>::AppendValue(
	TValue	inValue)
{
	::CFArrayAppendValue(UseMutableRef(),
						 reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	InsertValueAt													  [public]
/**
	Inserts a value into the array at the specified index
	
	@param	inIndex		Index at which to insert value
	@param	inValue		Value to insert										*/

template <class TValue>
void
CFArray<TValue>::InsertValueAt(
	CFIndex	inIndex,
	TValue	inValue)
{
	CFUtils::VerifyInsertIndex(GetCount(), inIndex);
	::CFArrayInsertValueAtIndex(UseMutableRef(), inIndex, 
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	SetValueAt														  [public]
/**
	Assigns a value to the item at the specified index in the array
	
	@param	inIndex		Index of item to set
	@param	inValue		New value for item at the index						*/

template <class TValue>
void
CFArray<TValue>::SetValueAt(
	CFIndex	inIndex,
	TValue	inValue)
{
	CFUtils::VerifyIndex(GetCount(), inIndex);
	::CFArraySetValueAtIndex(UseMutableRef(), inIndex,
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	RemoveValueAt													  [public]
/**
	Removes the value at the specified index from the array
	
	@param	inIndex		Index of item to remove								*/

template <class TValue>
void
CFArray<TValue>::RemoveValueAt(
	CFIndex	inIndex)
{
	CFUtils::VerifyIndex(GetCount(), inIndex);
	::CFArrayRemoveValueAtIndex(UseMutableRef(), inIndex);
}


// ---------------------------------------------------------------------------
//	RemoveAllValues													  [public]
/**
	Makes an array empty by removing all its values */

template <class TValue>
void
CFArray<TValue>::RemoveAllValues()
{
	::CFArrayRemoveAllValues(UseMutableRef());
}


// ---------------------------------------------------------------------------
//	ReplaceValues													  [public]
/**
	Replaces a range of values within the array with values from a buffer
	
	@param	inRange			Range to replace
	@param	inValues		Butter of values
	@param	inValueCount	Number of values, may be zero
	
	The array grows or shrinks if the number of values is different from
	the length of the range													*/

template <class TValue>
void
CFArray<TValue>::ReplaceValues(
	CFRange			inRange,
	const TValue*	inValues,
	CFIndex			inValueCount)
{
	CFUtils::VerifyRange(GetCount(), inRange);

	return ::CFArrayReplaceValues(UseMutableRef(), inRange,
									(const void**) inValues, inValueCount);
}


// ---------------------------------------------------------------------------
//	ExchangeValuesAt												  [public]
/**
	Swaps the values of the items at the specified indexes in the array
	
	@param	inIndexOne	Index of first item
	@param	inIndexTwo	Index of second item								*/

template <class TValue>
void
CFArray<TValue>::ExchangeValuesAt(
	CFIndex	inIndexOne,
	CFIndex	inIndexTwo)
{
	CFIndex valuesCount = GetCount();
	CFUtils::VerifyIndex(valuesCount, inIndexOne);
	CFUtils::VerifyIndex(valuesCount, inIndexTwo);

	::CFArrayExchangeValuesAtIndices(UseMutableRef(), inIndexOne, inIndexTwo);
}


// ---------------------------------------------------------------------------
//	BinarySearchFor													  [public]
/**
	Searches the array for a value using a binary search algorithm
	
	@param	inValue			Value to look for
	@param	inComparator	Comparison function for values
	@param	inParam			User-defined parameter for comparison function
	@param	inRange			Range to search
	
	@return	Index in range where item would be inserted in sorted order
	
	If value exists in the range, return index is that of a matching value.
	If value is larger than all items in range, return index is greater
	than or equal to the end of the range. Otherwise, return index is
	that of the value in the array that is just larger the target value.
	
	Behavior is undefined if the array is not sorted according to the
	comparison function.													*/

template <class TValue>
CFIndex
CFArray<TValue>::BinarySearchFor(
	TValue					inValue,
	CFComparatorFunction	inComparator,
	void*					inParam,
	CFRange					inRange)
{
	CFUtils::VerifyRange(GetCount(), inRange);
	return ::CFArrayBSearchValues(UseRef(), inRange,
		reinterpret_cast<const void *>(inValue), inComparator, inParam);
}


// ---------------------------------------------------------------------------
//	Sort															  [public]
/**
	Sorts the values within a range of the array
	
	@param	inComparator	Comparison function for values
	@param	inParam			User-defineed parameter for comparison function
	@param	inRange			Range to sort									*/

template <class TValue>
void
CFArray<TValue>::Sort(
	CFComparatorFunction	inComparator,
	void*					inParam,
	CFRange					inRange)
{
	CFUtils::VerifyRange(GetCount(), inRange);
	::CFArraySortValues(UseMutableRef(), inRange, inComparator, inParam);
}


// ---------------------------------------------------------------------------
//	ApplyFunction													  [public]
/**
	Calls a function once for each item in a range of the array
	
	@param	inFunction		Function to call
	@param	inParam			User-defined parameter for function
	@param	inRange			Range of items over which to apply function		*/

template <class TValue>
void
CFArray<TValue>::ApplyFunction(
	CFArrayApplierFunction	inFunction,
	void*					inParam,
	CFRange					inRange) const
{
	CFUtils::VerifyRange(GetCount(), inRange);
	::CFArrayApplyFunction(UseRef(), inRange, inFunction, inParam);
}


// ---------------------------------------------------------------------------
//	MutableCopy														 [private]
/**
	Make a copy of the specified CFArrayRef
	
	@param	inRefToCopy		CFArrayRef of values to copy					*/

template <class TValue>
CFMutableArrayRef
CFArray<TValue>::MutableCopy(
	CFArrayRef	inRefToCopy) const
{
	CFMutableArrayRef copyRef = ::CFArrayCreateMutableCopy(
										::CFGetAllocator(inRefToCopy),
										cfSize_Unlimited, inRefToCopy);
	PPx_ThrowIfCFCreateFailed_(copyRef, "CFArrayCreateMutableCopy");

	return copyRef;
}


} // namepsace PPx

#endif // H_SysCFArray
