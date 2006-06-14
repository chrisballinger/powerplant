// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRetained.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxRetained.h
	@brief		Classes for reference counted objects
*/

#ifndef H_PPxRetained
#define H_PPxRetained
#pragma once

#include <PPxPrefix.h>

namespace PPx {

// ===========================================================================
//	Retained
/**
	Base class for reference counted objects */

class	Retained {
public:
						Retained();
						
	void				Retain() const;
	
	void				Release();
	
	UInt32				GetRetainCount() const;
	
protected:
						Retained( const Retained& inOriginal );
						
	virtual				~Retained();
	
	Retained&			operator = ( const Retained& inSource );
	
private:
	mutable UInt32		mRetainCount;	/**< Number of shared owners */
};


// ===========================================================================
//	AutoRetained
/**
	Template class for automatically retaining and releasing Retained
	objects. TRetained must be a subclass of PPx::Retained (or implement
	its API).																*/

template <class TRetained>
class	AutoRetained {
public:
						AutoRetained();
						
	explicit			AutoRetained( TRetained* inRetained );
								
						AutoRetained( const AutoRetained& inOriginal );
						
	template <class T>	AutoRetained( const AutoRetained<T>& inOriginal );
						
						~AutoRetained();
						
	AutoRetained&		operator = ( const AutoRetained& inSource );
	
	template <class T>
	AutoRetained&		operator = ( const AutoRetained<T>& inSource );
	
	TRetained*			Get() const;
	
	TRetained*			operator -> () const;
	
	TRetained&			operator * () const;
	
	void				Reset();
	
	void				Reset( TRetained* inRetained );
	
	UInt32				GetRetainCount() const;
	
private:
	void				RetainObject();
	
	void				ReleaseObject();
	
private:
	TRetained*		mRetained;		/**< Pointer to shared object */
};


// ---------------------------------------------------------------------------
//	AutoRetained
/**
	Default constructor	 */

template <class TRetained>
AutoRetained<TRetained>::AutoRetained()
{
	mRetained = nil;
}


// ---------------------------------------------------------------------------
//	AutoRetained
/**
	Constructs from a pointer to a Retained object
	
	@param	inRetained		Pointer to object to retain						*/

template <class TRetained>
AutoRetained<TRetained>::AutoRetained(
	TRetained*	inRetained)
{
	mRetained = inRetained;
	RetainObject();
}


// ---------------------------------------------------------------------------
//	AutoRetained
/**
	Copy constructor */

template <class TRetained>
AutoRetained<TRetained>::AutoRetained(
	const AutoRetained&	inOriginal)
{
	mRetained = inOriginal.mRetained;
	RetainObject();
}


// ---------------------------------------------------------------------------
//	AutoRetained
/**
	Member template overload of copy constructor. Allows copy construction
	of AutoRetained<TRetained> from AutoRetained<T>, where T is a subclass
	of TRetained.															*/

template <class TRetained>
template <class T>
AutoRetained<TRetained>::AutoRetained(
	const AutoRetained<T>&	inOriginal)
{
	mRetained = inOriginal.Get();
	RetainObject();
}


// ---------------------------------------------------------------------------
//	~AutoRetained
/**
	Destructor */

template <class TRetained>
AutoRetained<TRetained>::~AutoRetained()
{
	ReleaseObject();
}


// ---------------------------------------------------------------------------
//	operator =
/**
	Assignment operator */

template <class TRetained>
AutoRetained<TRetained>&
AutoRetained<TRetained>::operator = (
	const AutoRetained&	inSource)
{
	Reset(inSource.mRetained);
	return *this;
}


// ---------------------------------------------------------------------------
//	operator =
/**
	Member template overload of assignment operator. Allows assignment of
	AutoRetained<TRetained> from AutoRetained<T>, where T is a subclass
	of TRetained.															*/

template <class TRetained>
template <class T>
AutoRetained<TRetained>&
AutoRetained<TRetained>::operator = (
	const AutoRetained<T>&	inSource)
{
	Reset(inSource.Get());
	return *this;
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns a pointer to the retained object
	
	@return	Pointer to the retained object									*/

template <class TRetained>
TRetained*
AutoRetained<TRetained>::Get() const
{
	return mRetained;
}


// ---------------------------------------------------------------------------
//	operator ->
/**
	Returns a pointer to the retained object
	
	@return	Pointer to the retained object									*/

template <class TRetained>
TRetained*
AutoRetained<TRetained>::operator -> () const
{
	return mRetained;
}


// ---------------------------------------------------------------------------
//	operator *
/**
	Returns a reference to the retained object
	
	@return	Reference to the retained object
	
	@note Behavior is undefined if retained object pointer is nil			*/

template <class TRetained>
TRetained&
AutoRetained<TRetained>::operator * () const
{
	return *mRetained;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Reinitializes the retained object pointer to nil */

template <class TRetained>
void
AutoRetained<TRetained>::Reset()
{
	ReleaseObject();
	mRetained = nil;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Reinitializes the retained object pointer to the input value
	
	@param	inRetained		Pointer to object to retain						*/

template <class TRetained>
void
AutoRetained<TRetained>::Reset(
	TRetained*	inRetained)
{
	if (mRetained != inRetained) {
		ReleaseObject();
		mRetained = inRetained;
		RetainObject();
	}
}


// ---------------------------------------------------------------------------
//	GetRetainCount
/**
	Returns the retain count (number of shared owners) for the retained object
	
	@return Retain count for the retained object							*/

template <class TRetained>
UInt32
AutoRetained<TRetained>::GetRetainCount() const
{
	return (mRetained == nil) ? 0 : mRetained->GetRetainCount();
}


// ---------------------------------------------------------------------------
//	RetainObject													 [private]
/**
	Calls Retain() on the retained object */

template <class TRetained>
void
AutoRetained<TRetained>::RetainObject()
{
	if (mRetained != nil) {
		mRetained->Retain();
	}
}


// ---------------------------------------------------------------------------
//	ReleaseObject													 [private]
/**
	Releases the retained object */

template <class TRetained>
void
AutoRetained<TRetained>::ReleaseObject()
{
		// mRetained is invalid upon return from this function. Caller
		// is responsible for appropriately setting mRetained afterwards.

	if (mRetained != nil) {
		mRetained->Release();
	}
}

#pragma mark -

// ===========================================================================
//	AutoRefCount
/**
	Template class for automatically reference counting objects */

template <class TObject>
class	AutoRefCount {
public:
						AutoRefCount();

	explicit			AutoRefCount( TObject* inObject );
								
						AutoRefCount( const AutoRefCount& inOriginal );
						
	template <class T>	AutoRefCount( const AutoRefCount<T>& inOriginal );
						
						~AutoRefCount();
						
	AutoRefCount&		operator = ( const AutoRefCount& inSource );
	
	template <class T>
	AutoRefCount&		operator = ( const AutoRefCount<T>& inSource );
	
	TObject*			Get() const;
	
	TObject*			operator -> () const;
	
	TObject&			operator * () const;
	
	void				Reset();
	
	void				Reset( TObject* inObject );
	
	UInt32				GetRefCount() const;
	
private:
	template <class T> friend class AutoRefCount;
	
private:
	TObject*			mObject;		/**< Pointer to shared object */
	Retained*			mCounter;		/**< Pointer to shared ref count */
};


// ---------------------------------------------------------------------------
//	AutoRefCount
/**
	Default constructor */

template <class TObject>
AutoRefCount<TObject>::AutoRefCount()
{
	mObject  = nil;
	mCounter = nil;
}


// ---------------------------------------------------------------------------
//	AutoRefCount
/**
	Constructs from a pointer to an object
	
	@param	inObject	Pointer to object to reference count				*/

template <class TObject>
AutoRefCount<TObject>::AutoRefCount(
	TObject*	inObject)
{
	mObject  = inObject;
	mCounter = nil;
	
	if (inObject != nil) {
		mCounter = new Retained;
		mCounter->Retain();
	}
}


// ---------------------------------------------------------------------------
//	AutoRefCount
/**
	Copy constructor */

template <class TObject>
AutoRefCount<TObject>::AutoRefCount(
	const AutoRefCount& inOriginal)
{
	mObject  = inOriginal.mObject;
	mCounter = inOriginal.mCounter;
	
	if (mCounter != nil) {
		mCounter->Retain();
	}
}


// ---------------------------------------------------------------------------
//	AutoRefCount
/**
	Member template overload of copy constructor. Allows copy construction
	of AutoRefCount<TObject> from AutoRefCount<T>, where T is a subclass
	of TObject.																*/

template <class TObject>
template <class T>
AutoRefCount<TObject>::AutoRefCount(
	const AutoRefCount<T>& inOriginal)
{
		// Assignment to mObject will generate a compiler error if
		// T is not a subclass of TObject
		
	mObject  = inOriginal.mObject;
	mCounter = inOriginal.mCounter;
	
	if (mCounter != nil) {
		mCounter->Retain();
	}
}


// ---------------------------------------------------------------------------
//	~AutoRefCount
/**
	Destructor */

template <class TObject>
AutoRefCount<TObject>::~AutoRefCount()
{
	Reset();
}


// ---------------------------------------------------------------------------
//	operator =
/**
	Assignment operator */

template <class TObject>
AutoRefCount<TObject>&
AutoRefCount<TObject>::operator = (
	const AutoRefCount&	inSource)
{
	mObject  = inSource.mObject;
	mCounter = inSource.mCounter;
	
	if (mCounter != nil) {
		mCounter->Retain();
	}
	
	return *this;
}


// ---------------------------------------------------------------------------
//	operator =
/**
	Member template overload of assignment operator. Allows assignment of
	AutoRefCount<TObject> from AutoRefCount<T>, where T is a subclass
	of TObject.																*/

template <class TObject>
template <class T>
AutoRefCount<TObject>&
AutoRefCount<TObject>::operator = (
	const AutoRefCount<T>& inSource)
{
		// Assignment to mObject will generate a compiler error if
		// T is not a subclass of TObject
		
	mObject  = inSource.mObject; 
	mCounter = inSource.mCounter;
	
	if (mCounter != nil) {
		mCounter->Retain();
	}
	
	return *this;
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns a pointer to the reference counted object
	
	@return Pointer to the retained object									*/

template <class TObject>
TObject*
AutoRefCount<TObject>::Get() const
{
	return mObject;
}


// ---------------------------------------------------------------------------
//	operator ->
/**
	Returns a pointer to the reference counted object */

template <class TObject>
TObject*
AutoRefCount<TObject>::operator -> () const
{
	return mObject;
}


// ---------------------------------------------------------------------------
//	operator *
/**
	Returns a reference to the reference counted object
	
	@return	Reference to the reference counted object
	
	@note Behavior is undefined if object pointer is nil					*/

template <class TObject>
TObject&
AutoRefCount<TObject>::operator * () const
{
	return *mObject;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Releases currently retained object and reinitializes the reference
	counted object pointer to nil											*/

template <class TObject>
void
AutoRefCount<TObject>::Reset()
{
	if (mCounter != nil) {
		if (mCounter->GetRetainCount() == 1) {
			delete mObject;
		}
	
		mCounter->Release();

		mObject  = nil;
		mCounter = nil;
	}
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Releases currently retained object and reinitializes the reference
	counted object pointer to the input value
	
	@param	inObject	Pointer to object to reference count				*/

template <class TObject>
void
AutoRefCount<TObject>::Reset(
	TObject*	inObject)
{
	if (inObject != mObject) {
		Reset();
		mObject = inObject;
		if (inObject != nil) {
			mCounter = new Retained;
			mCounter->Retain();
		}
	}
}


// ---------------------------------------------------------------------------
//	GetRefCount
/**
	Returns the reference count for the object
	
	@return	Reference count for the object									*/

template <class TObject>
UInt32
AutoRefCount<TObject>::GetRefCount() const
{
	return (mCounter == nil) ? 0 : mCounter->GetRetainCount();
}


} // namespace PPx

#endif // H_PPxRetained
