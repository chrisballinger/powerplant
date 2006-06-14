// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMemoryUtils.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:11 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxMemoryUtils.h
	@brief		Function and classes for managing objects and data
				stored in memory
*/

#ifndef H_PPxMemoryUtils
#define H_PPxMemoryUtils
#pragma once

#include <PPxPrefix.h>
#include <memory>

namespace PPx {

// ---------------------------------------------------------------------------
//	CreateNew< T >
/**
	Template function that creates a new object of a class */

template <class T>
std::auto_ptr<T>
CreateNew()
{
	return std::auto_ptr<T>( new T );
}


// ---------------------------------------------------------------------------
//	CreateNew< T, TParam1 >
/**
	Template function that creates a new object of a class with 1 parameter */

template <class T, typename TParam1>
std::auto_ptr<T>
CreateNew(
	TParam1	inParam1)
{
	return std::auto_ptr<T>( new T(inParam1) );
}


// ---------------------------------------------------------------------------
//	CreateNew< T, TParam1, TParam2 >
/**
	Template function that creates a new object of a class with 2 parameters */

template <class T, typename TParam1, typename TParam2>
std::auto_ptr<T>
CreateNew(
	TParam1	inParam1,
	TParam2	inParam2)
{
	return std::auto_ptr<T>( new T(inParam1, inParam2) );
}


// ---------------------------------------------------------------------------
//	CreateNew< T, TParam1, TParam2, TParam3 >
/**
	Template function that creates a new object of a class with 3 parameters */

template <class T, typename TParam1, typename TParam2, typename TParam3>
std::auto_ptr<T>
CreateNew(
	TParam1	inParam1,
	TParam2	inParam2,
	TParam3	inParam3)
{
	return std::auto_ptr<T>( new T(inParam1, inParam2, inParam3) );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SafeDynamicCast< TCastTo, TBase >
/**
	Template function for performing a dynamic_cast which throws if the
	cast from TBase to TCastTo is unsuccessful.
	
	@param	inObjectPointer		Pointer of type TBase to dynamically cast
	
	@return	Pointer of type TCastTo											*/

template <class TCastTo, class TBase>
TCastTo
SafeDynamicCast(
	TBase	inObjectPointer)
{
	TCastTo		castResult = nil;
	
	if (inObjectPointer != nil) {
		castResult = dynamic_cast<TCastTo>(inObjectPointer);
		
		PPx_ThrowIfNil_(castResult, LogicError, err_DynamicCast,
						"dynamic_cast unsuccessful");
	}
	
	return castResult;
}

#pragma mark -

// ===========================================================================
//	AutoValueSaver< T >
/**
	Template class for automatically saving and restoring a variable's value */

template <typename T>
class	AutoValueSaver {
public:
	explicit			AutoValueSaver( T& ioValue );
							
						AutoValueSaver(
								T&			ioValue,
								const T&	inNewValue);
							
						~AutoValueSaver();
							
	const T&			Get() const;
							
	void				Restore();
							
	void				Reset();
							
	void				Reset( const T& inNewValue );
	
private:								// Forbid copy and assignment
						AutoValueSaver( const T& );
	T&					operator = ( const T& );
						
private:
	T*		mValuePtr;
	T		mSavedValue;
};


// ---------------------------------------------------------------------------
//	AutoValueSaver
/**
	Constructs from an input variable
	
	@param	ioValue		Variable whose current value we want to save		*/

template <typename T>
AutoValueSaver<T>::AutoValueSaver(
	T&	ioValue)

	: mValuePtr(&ioValue),
	  mSavedValue(ioValue)
{
}


// ---------------------------------------------------------------------------
//	AutoValueSaver
/**
	Constructs from an input variable and a new value to set it to
	
	@param	ioValue		Variable whose current value we want to save
	@param	inNewValue	New vale to which to set variable					*/

template <typename T>
AutoValueSaver<T>::AutoValueSaver(
	T&			ioValue,
	const T&	inNewValue)

	: mValuePtr(&ioValue),
	  mSavedValue(ioValue)
{
	ioValue = inNewValue;
}


// ---------------------------------------------------------------------------
//	~AutoValueSaver
/**
	Destructor. Restores saved value of variable.							*/

template <typename T>
AutoValueSaver<T>::~AutoValueSaver()
{
	Restore();
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns the saved value
	
	@return	Saved value														*/

template <typename T>
const T&
AutoValueSaver<T>::Get() const
{
	return mSavedValue;
}


// ---------------------------------------------------------------------------
//	Restore
/**
	Sets the variable to the saved value */

template <typename T>
void
AutoValueSaver<T>::Restore()
{
	if (mValuePtr != nil) {
		*mValuePtr = mSavedValue;
	}
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Clears identity of the variable to save. Restore() and destructor will
	do nothing, but the saved value is still accessible via Get().			*/

template <typename T>
void
AutoValueSaver<T>::Reset()
{
	mValuePtr = nil;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Sets a new saved value for the variable. Restore() and destructor will
	set the variable to this new value.
	
	@param	inNewValue	New value to which the variable will be restored	*/

template <typename T>
void
AutoValueSaver<T>::Reset(
	const T&	inNewValue)
{
	mSavedValue = inNewValue;
}

#pragma mark -

// ===========================================================================
//	AutoHandle
/**
	Manages ownership of Toolbox Handle data block */

class	AutoHandle {
public:
						AutoHandle();
						
	explicit			AutoHandle( Handle inHandle );
						
						~AutoHandle();
						
	operator			Handle() const;
						
	Handle				Get() const;
	
	void				Reset();
	
	void				Reset( Handle inHandle );
	
private:							// Forbid copy and assignment
						AutoHandle( const AutoHandle& );
	AutoHandle&			operator = ( const AutoHandle& );

private:
	Handle		mHandle;
};


// ---------------------------------------------------------------------------
//	AutoHandle
/**
	Default constructor. Owned Handle is nil. */

inline
AutoHandle::AutoHandle()
{
	mHandle = nil;
}


// ---------------------------------------------------------------------------
//	AutoHandle
/**
	Constructs from a Handle which becomes the owned Handle
	
	@param	inHandle	Object takes ownership of this Handle 				*/

inline
AutoHandle::AutoHandle(
	Handle	inHandle)
{
	mHandle = inHandle;
}


// ---------------------------------------------------------------------------
//	~AutoHandle
/**
	Destructor. Disposes of the owned Handle */

inline
AutoHandle::~AutoHandle()
{
	Reset();
}


// ---------------------------------------------------------------------------
//	operator Handle
/**
	Returns the Handle
	
	@return Handle owned by the AutoHandle									*/

inline
AutoHandle::operator Handle() const
{
	return mHandle;
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns the Handle
	
	@return Handle owned by the AutoHandle									*/

inline Handle
AutoHandle::Get() const
{
	return mHandle;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Disposes existing Handle and sets owned Handle to nil */

inline void
AutoHandle::Reset()
{
	if (mHandle != nil) {
		::DisposeHandle(mHandle);
		mHandle = nil;
	}
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Disposes existing Handle and takes ownership of input Handle
	
	@param	inHandle	Object takes ownership of this Handle				*/

inline void
AutoHandle::Reset(
	Handle	inHandle)
{
	if (inHandle != mHandle) {
		Reset();
		mHandle = inHandle;
	}
}


} // namespace PPx

#endif	// H_PPxMemoryUtils
