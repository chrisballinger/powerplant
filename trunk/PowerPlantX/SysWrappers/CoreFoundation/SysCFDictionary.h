// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFDictionary.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:29 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFDictionary.h
*/

#ifndef H_SysCFDictionary
#define H_SysCFDictionary
#pragma once

#include <SysCFMutableObject.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
/**	Template wrapper class for Core Foundation Dictionary

	The template parameters specify the types of the key and the value. As
	with all CF containers, sizeof(TKey) and sizeof(TValue) must be 4 bytes.
*/

template <class TKey, class TValue>
class	CFDictionary : public CFMutableObject<CFDictionaryRef, CFMutableDictionaryRef> {
public:
									// Constructors
						CFDictionary();

						CFDictionary(
								CFDictionaryRef	inCFDictionary,
								bool			inRetain);

						CFDictionary(
								CFMutableDictionaryRef	inCFDictionary,
								bool					inRetain);

	explicit			CFDictionary(
								CFIndex								inCapacity,
								const CFDictionaryKeyCallBacks*		inKeyCallBacks = nil,
								const CFDictionaryValueCallBacks*	inValueCallBacks = nil,
								CFAllocatorRef						inAllocator = nil);

						CFDictionary(
								const TKey*							inKeys,
								const TValue*						inValues,
								CFIndex								inItemCount,
								const CFDictionaryKeyCallBacks*		inKeyCallBacks = nil,
								const CFDictionaryValueCallBacks*	inValueCallBacks = nil,
								CFAllocatorRef						inAllocator = nil);

						CFDictionary( const CFDictionary& inOriginal );
						
	CFDictionary&		operator = ( const CFDictionary& inSource );
	
									// Accessors
	CFIndex				GetCount() const;

	bool				IsEmpty() const;

	CFIndex				GetCountOfKey( TKey inKey ) const;

	bool				ContainsKey( TKey inKey ) const;

	CFIndex				GetCountOfValue( TValue inValue ) const;

	bool				ContainsValue( TValue inValue ) const;

	TValue				GetValue( TKey inKey ) const;
	
	TValue				operator [] ( TKey inKey ) const;

	bool				GetValueIfPresent(
								TKey	inKey,
								TValue&	outValue) const;

	void				GetKeysAndValues(
								TKey*	outKeys,
								TValue*	outValues) const;

	void				AddValue(
								TKey	inKey,
								TValue	inValue);

	void				SetValue(
								TKey	inKey,
								TValue	inValue);

	void				ReplaceValue(
								TKey	inKey,
								TValue	inValue);

	void				RemoveValue( TKey inKey );

	void				RemoveAllValues();

	void				ApplyFunction(
								CFDictionaryApplierFunction	inFunction,
								void*						inParam) const;

private:
	virtual CFMutableDictionaryRef
						MutableCopy( CFDictionaryRef inRefToCopy ) const;

private:	
	typedef CFMutableObject<CFDictionaryRef, CFMutableDictionaryRef>	BaseT;
};


// ---------------------------------------------------------------------------
//	CFDictionary													  [public]
/**
	Default constructor */

template <class TKey, class TValue>
CFDictionary<TKey, TValue>::CFDictionary()
{
}


// ---------------------------------------------------------------------------
//	CFDictionary													  [public]
/**
	Constructs from an existing immutable CFDictionaryRef */

template <class TKey, class TValue>
CFDictionary<TKey, TValue>::CFDictionary(
	CFDictionaryRef	inDictRef,
	bool			inRetain)
	
	: BaseT(inDictRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFDictionary													  [public]
/**
	Constructs from an existing CFMutableDictionaryRef

	@param	inDictRef	CF dictionary to adopt
	@param	inRetain	Whether to retain the CF dictionary
	
	@note Pass false for inRetain if you are transferring ownership of the
	CF dictionary, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF dictionary.
	
	Pase true for inRetain if the caller wants to maintain ownership
	of the CF dictionary. In this case, we make a copy, so that multiple
	clients aren't changing the same data.									*/

template <class TKey, class TValue>
CFDictionary<TKey, TValue>::CFDictionary(
	CFMutableDictionaryRef	inDictRef,
	bool					inRetain)
{
	AttachMutableRef(inDictRef, inRetain);
}


// ---------------------------------------------------------------------------
//	CFDictionary													  [public]
/**
	Constructs a dictionary with the specified capacity
	
	@param	inCapacity			Size of the dictionary
	@param	inKeyCallBacks		Callbacks for the dictionary keys
	@param	inValueCallBacks	Callbacks for the dictionary values
	@param	inAllocator			CF Allocator
	
	If the keys are CFTypeRefs, use kCFTypeDictionaryKeyCallBacks for
	inKeyCallBacks. If the values are CFTypeRefs, use
	kCFTypeDictionaryValueCallBacks for inValueCallBacks.					*/

template <class TKey, class TValue>
CFDictionary<TKey, TValue>::CFDictionary(
	CFIndex								inCapacity,
	const CFDictionaryKeyCallBacks*		inKeyCallBacks,
	const CFDictionaryValueCallBacks*	inValueCallBacks,
	CFAllocatorRef						inAllocator)
{
	CFMutableDictionaryRef	ref = ::CFDictionaryCreateMutable(
														inAllocator,
														inCapacity,
														inKeyCallBacks,
														inValueCallBacks);
	PPx_ThrowIfCFCreateFailed_(ref, "CFDictionaryCreateMutable");
	
	AttachMutableRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFDictionary													  [public]
/**
	Constructs from buffers of keys and values
	
	@param	inKeys				Array of keys
	@param	inValues			Array of values
	@param	inItemCount			Number of key/value pairs
	@param	inKeyCallBacks		Callbacks for the keys
	@param	inValueCallBacks	Callbacks for the values
	@param	inAllocator			CF Allocator
	
	If the keys are CFTypeRefs, use kCFTypeDictionaryKeyCallBacks for
	inKeyCallBacks. If the values are CFTypeRefs, use
	kCFTypeDictionaryValueCallBacks for inValueCallBacks.					*/

template <class TKey, class TValue>
CFDictionary<TKey, TValue>::CFDictionary(
	const TKey*							inKeys,
	const TValue*						inValues,
	CFIndex								inItemCount,
	const CFDictionaryKeyCallBacks*		inKeyCallBacks,
	const CFDictionaryValueCallBacks*	inValueCallBacks,
	CFAllocatorRef						inAllocator)
{
	CFDictionaryRef	ref = ::CFDictionaryCreate(
								inAllocator,
								(const void**) inKeys,
								(const void**) inValues,
								inItemCount,
								inKeyCallBacks,
								inValueCallBacks);
	PPx_ThrowIfCFCreateFailed_(ref, "CFDictionaryCreate");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFDictionary													  [public]
/**
	Copy constructor */

template <class TKey, class TValue>
CFDictionary<TKey, TValue>::CFDictionary(
	const CFDictionary&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */

template <class TKey, class TValue>
CFDictionary<TKey, TValue>&
CFDictionary<TKey, TValue>::operator = (
	const CFDictionary&	inSource)
{
	AssignObject(inSource);
	return *this;
}


// ---------------------------------------------------------------------------
//	GetCount														  [public]
/**
	Returns the number of items in the dictionary

	@return	Number of items in the dictionary								*/

template <class TKey, class TValue>
CFIndex
CFDictionary<TKey, TValue>::GetCount() const
{
	return ::CFDictionaryGetCount( UseRef() );
}


// ---------------------------------------------------------------------------
//	IsEmpty															  [public]
/**
	Returns whether the dictionary has no items
	
	@return Whether the dictionar has no items */

template <class TKey, class TValue>
bool
CFDictionary<TKey, TValue>::IsEmpty() const
{
	return (GetCount() == 0);
}


// ---------------------------------------------------------------------------
//	GetCountOfKey													  [public]
/**
	Returns number of items with the given key that are in the dictionary
	
	@param	inKey	Item key
	
	@return Number of items with the given key that are in the dictionary	*/

template <class TKey, class TValue>
CFIndex
CFDictionary<TKey, TValue>::GetCountOfKey(
	TKey	inKey) const
{
	return ::CFDictionaryGetCountOfKey(UseRef(),
								reinterpret_cast<const void *>(inKey));
}


// ---------------------------------------------------------------------------
//	ContainsKey														  [public]
/**
	Returns whether an item with the given key is in the dictionary
	
	@param	inKey	Item key
	
	@return Whether an item with the given key is in the dictionary			*/

template <class TKey, class TValue>
bool
CFDictionary<TKey, TValue>::ContainsKey(
	TKey	inKey) const
{
	return ::CFDictionaryContainsKey(UseRef(),
								reinterpret_cast<const void *>(inKey));
}


// ---------------------------------------------------------------------------
//	GetCountOfValue													  [public]
/**
	Returns number of items with the given value that are in the dictionary
	
	@param	inValue		Item value
	
	@return Number of items with the given value that are in the dictionary	*/

template <class TKey, class TValue>
CFIndex
CFDictionary<TKey, TValue>::GetCountOfValue(
	TValue	inValue) const
{
	return ::CFDictionaryGetCountOfValue(UseRef(),
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	ContainsValue													  [public]
/**
	Returns whether an item with the given value is in the dictionary
	
	@param	inValue		Item value
	
	@return Whether an item with the given value is in the dictionary		*/

template <class TKey, class TValue>
bool
CFDictionary<TKey, TValue>::ContainsValue(
	TValue	inValue) const
{
	return ::CFDictionaryContainsValue(UseRef(),
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	GetValue														  [public]
/**
	Returns value of item with the given key
	
	@param	inKey	Item key
	
	@return Value of item with the given key
	
	Returns zero if the key is not in the dictionary. Call
	GetValueIfPresent() if zero may be a valid item value.					*/

template <class TKey, class TValue>
TValue
CFDictionary<TKey, TValue>::GetValue(
	TKey	inKey) const
{
	return reinterpret_cast<TValue>(::CFDictionaryGetValue(UseRef(),
								reinterpret_cast<const void *>(inKey)));
}


// ---------------------------------------------------------------------------
//	operator []														  [public]
/**
	Returns value of item with the given key
	
	@param	inKey	Item key
	
	@return Value of item with the given key
	
	Returns zero if the key is not in the dictionary. Call
	GetValueIfPresent() if zero may be a valid item value.					*/

template <class TKey, class TValue>
TValue
CFDictionary<TKey, TValue>::operator [] (
	TKey	inKey) const
{
	return GetValue(inKey);
}


// ---------------------------------------------------------------------------
//	GetValueIfPresent												  [public]
/**
	Passes back value of item with the given key and returns whether an
	item was found
	
	@param	inKey		Item key
	@param	outValue	Item value
	
	@return	Whether an item with the given key was found					*/

template <class TKey, class TValue>
bool
CFDictionary<TKey, TValue>::GetValueIfPresent(
	TKey	inKey,
	TValue&	outValue) const
{
	return ::CFDictionaryGetValueIfPresent(UseRef(),
								reinterpret_cast<const void *>(inKey),
								reinterpret_cast<const void **>(&outValue));
}


// ---------------------------------------------------------------------------
//	GetKeysAndValues												  [public]
/**
	Passes back all keys and values to supplied buffers
	
	@param	outKeys		Buffer to which to copy the keys
	@param	outValues	Buffer to which to copy the values
	
	Buffers must be large enough to hold all the items						*/

template <class TKey, class TValue>
void
CFDictionary<TKey, TValue>::GetKeysAndValues(
	TKey*	outKeys,
	TValue*	outValues) const
{
	::CFDictionaryGetKeysAndValues(UseRef(),
								reinterpret_cast<const void **>(outKeys),
								reinterpret_cast<const void **>(outValues));
}


// ---------------------------------------------------------------------------
//	AddValue														  [public]
/**
	Adds a key/value pair to the dictionary
	
	@param	inKey		Item key
	@param	inValue		Item value											*/

template <class TKey, class TValue>
void
CFDictionary<TKey, TValue>::AddValue(
	TKey	inKey,
	TValue	inValue)
{
	::CFDictionaryAddValue(UseMutableRef(),
								reinterpret_cast<const void *>(inKey),
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	SetValue														  [public]
/**
	Sets the value for the item with the specified key
	
	@param	inKey		Item key
	@param	inValue		Item value
	
	If an item with the key is present, changes the value of that item
	to the input value. If an item with the key is not present, adds
	the key/value pair as a new item.										*/

template <class TKey, class TValue>
void
CFDictionary<TKey, TValue>::SetValue(
	TKey	inKey,
	TValue	inValue)
{
	::CFDictionarySetValue(UseMutableRef(),
								reinterpret_cast<const void *>(inKey),
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	ReplaceValue													  [public]
/**
	Replaces the value for the item with the specified key
	
	@param	inKey		Item key
	@param	inValue		Item value
	
	If an item with the key is present, changes the value of that item
	to the input value. If an item with the key is not present, does
	nothing.																*/
	

template <class TKey, class TValue>
void
CFDictionary<TKey, TValue>::ReplaceValue(
	TKey	inKey,
	TValue	inValue)
{
	::CFDictionaryReplaceValue(UseMutableRef(),
								reinterpret_cast<const void *>(inKey),
								reinterpret_cast<const void *>(inValue));
}


// ---------------------------------------------------------------------------
//	RemoveValue														  [public]
/**
	Removes the item with the given key from the dictionary
	
	@param	inKey	Item key
	
	Does nothng if there is no item with the given key in the dictionary	*/

template <class TKey, class TValue>
void
CFDictionary<TKey, TValue>::RemoveValue(
	TKey	inKey)
{
	::CFDictionaryRemoveValue(UseMutableRef(),
								reinterpret_cast<const void *>(inKey));
}


// ---------------------------------------------------------------------------
//	RemoveAllValues													  [public]
/**
	Removes all items from the dictionary, making it empty					*/

template <class TKey, class TValue>
void
CFDictionary<TKey, TValue>::RemoveAllValues()
{
	::CFDictionaryRemoveAllValues( UseMutableRef() );
}


// ---------------------------------------------------------------------------
//	ApplyFunction													  [public]
/**
	Call a function for each item in the dictionary
	
	@param	inFunction		Function to call
	@param	inParam			User-defined parameter to the function			*/

template <class TKey, class TValue>
void
CFDictionary<TKey, TValue>::ApplyFunction(
	CFDictionaryApplierFunction inFunction,
	void*						inParam) const
{
	::CFDictionaryApplyFunction(UseRef(), inFunction, inParam);
}


// ---------------------------------------------------------------------------
//	MutableCopy														 [private]
/**
	Returns a mutable reference to a copy of the input CFDictionaryRef
	
	@param	inRefToCopy		CFDictionaryRef to copy
	
	@return	CFMutableDictionaryRef to a copy of the input CFDictionaryRef	*/

template <class TKey, class TValue>
CFMutableDictionaryRef
CFDictionary<TKey, TValue>::MutableCopy(
	CFDictionaryRef	inRefToCopy) const
{
	CFMutableDictionaryRef copyRef = ::CFDictionaryCreateMutableCopy(
											::CFGetAllocator(inRefToCopy),
											cfSize_Unlimited,
											inRefToCopy);
	PPx_ThrowIfCFCreateFailed_(copyRef, "CFDictionaryCreateMutableCopy");

	return copyRef;
}


} // namespace PPx

#endif // H_SysCFDictionary
