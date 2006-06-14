// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxOwnedPointer.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:11 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxOwnedPointer.h
	@brief		Template class for managing exclusive ownership of a pointer
*/

#ifndef H_PPxOwnedPointer
#define H_PPxOwnedPointer
#pragma once

// No #includes. This file has no dependencies.

namespace PPx {

// ===========================================================================
//	OwnedPointer< T >
/**
	Template class which manages a pointer created via "new". OwnedPointer
	objects own their underlying raw pointer, and delete it upon destruction
	or when adopting a different pointer. The owned object may be nil.

	OwnedPointers maintain exclusive ownership of their underlying pointers.
	You cannot transfer ownership via copy construction or assignment.

	OwnedPointer implements operators * and ->, so you can use it with the
	same syntax as a raw pointer. As with raw pointers, attempts to use
	operator * or -> on a nil owned object results in undefined behavior.
*/

template <class T> class OwnedPointer {
public:
						OwnedPointer();

	explicit			OwnedPointer( T* inPointer );

						~OwnedPointer();
						
	T*					Get() const;
	
	T*					operator -> () const;
						
	T&					operator * () const;
	
	void				Reset();
	
	void				Reset( T* inPointer );
	
private:								// Forbid copy and assignment
						OwnedPointer( const OwnedPointer& );
	OwnedPointer&		operator = ( const OwnedPointer& );

private:
	T*				mPointer;			/**< Pointer to owned object */
};


// ---------------------------------------------------------------------------
//	OwnedPointer
/**
	Default constructor */

template <class T>
OwnedPointer<T>::OwnedPointer()

	: mPointer(0)
{
}


// ---------------------------------------------------------------------------
//	OwnedPointer
/**
	Constructs from a raw pointer, which must have been created via a
	call to "new" or is nil
	
	@param	inPointer	Object takes ownership of this pointer				*/

template <class T>
OwnedPointer<T>::OwnedPointer(
	T*	inPointer)

	: mPointer(inPointer)
{
}


// ---------------------------------------------------------------------------
//	~OwnedPointer
/**
	Destructor */

template <class T>
OwnedPointer<T>::~OwnedPointer()
{
	delete mPointer;
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns a pointer to the owned object
	
	@return	Pointer to the owned object										*/

template <class T>
T*
OwnedPointer<T>::Get() const
{
	return mPointer;
}


// ---------------------------------------------------------------------------
//	operator ->
/**
	Returns a pointer to the owned object
	
	@return	Pointer to the owned object										*/

template <class T>
T*
OwnedPointer<T>::operator -> () const
{
	return mPointer;
}


// ---------------------------------------------------------------------------
//	operator *
/**
	Returns a reference to the owned object
	
	@return Reference to the owned object									*/

template <class T>
T&
OwnedPointer<T>::operator * () const
{
	return *mPointer;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Deletes existing owned object and sets owned object pointer to nil */

template <class T>
void
OwnedPointer<T>::Reset()
{
	delete mPointer;
	mPointer = 0;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Deletes existing owned object and takes ownership of input object pointer
	
	@param	inPointer	Object takes ownership of this pointer				*/

template <class T>
void
OwnedPointer<T>::Reset(
	T*	inPointer)
{
	if (inPointer != mPointer) {
		delete mPointer;
		mPointer = inPointer;
	}
}


} // namespace PPx

#endif // H_PPxOwnedPointer
