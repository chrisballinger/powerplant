// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFObject.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:30 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFObject.h
	@brief		Template base class for Core Foundation wrapper classes
*/

#ifndef H_SysCFObject
#define H_SysCFObject
#pragma once

#include <PPxPrefix.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFObject
/**
	Template base class for Core Foundation wrapper classes

	Template parameter is the underlying system CF reference type. This
	class controls access to the CF reference, automatically retaining
	and releasing it. It also implements the CF Base functions.
	
	This class is the base class for immutable Core Foundation entities.	*/

template <typename TCFRef>
class	CFObject {
public:
									// Accessing Toolbox CF reference
	TCFRef				UseRef() const;
	
	TCFRef				GetRefValue() const;
	
									/** Converts to a type ref */
						operator TCFRef() const		{ return UseRef(); }
	
									// Test functions
	bool				IsValid() const;
	
	bool				HasSameRef( TCFRef inRef ) const;

									// CF Base functions
	CFTypeID			GetTypeID() const;
	
	CFStringRef			CopyTypeIDDescription() const;
	
	CFAllocatorRef		GetAllocator() const;
	
	CFStringRef			CopyDescription() const;
	
	CFHashCode			GetHashCode() const;
	
	CFIndex				GetRetainCount() const;
	
	bool				IsEqualTo( TCFRef inRef ) const;
	
									// Changing ownership of CF reference
	void				AttachRef(
								TCFRef	inRef,
								bool	inRetain);
	
	TCFRef				DetachRef();
	
	void				FreeRef();

protected:
									// Protected constructors and destructor
									//   mean this must only be a base class
						CFObject();
						
						CFObject(
								TCFRef	inRef,
								bool	inRetain);
						
						CFObject( const CFObject& inOriginal );
						
	virtual				~CFObject();
	
									// Utility for subclasses
	void				AssignObject( const CFObject& inOriginal );
	
private:							// Managing the CF reference
	void				Retain();
	
	void				Release();

private:
	TCFRef				mCFRef;		/**< System CF Reference */
};


// ---------------------------------------------------------------------------
//	UseRef															  [public]
/**
	Returns the CF reference for the object. Throws an exception if the
	reference is not valid.
	
	@return CF reference for the object
	
	Call this routine if you are using the Ref in a context where it must
	be valid (not nil)														*/

template <typename TCFRef>
TCFRef
CFObject<TCFRef>::UseRef() const
{
	PPx_ThrowIfNil_(mCFRef, RuntimeError, err_CFNilRef, "Nil CF reference");
	return mCFRef;
}


// ---------------------------------------------------------------------------
//	GetRefValue														  [public]
/**
	Returns the CF reference for the object without checking if it is nil.
	
	@return CF reference for the object
	
	Use this routine if you are prepared for the Ref to be nil.
	
	@note This function is called GetRefValue instead of just GetRef so
	that you are less likely to use it when UseRef is what you really
	should call to guarantee a valid ref.									*/

template <typename TCFRef>
TCFRef
CFObject<TCFRef>::GetRefValue() const
{
	return mCFRef;
}


// ---------------------------------------------------------------------------
//	IsValid															  [public]
/**
	Returns whether the CF reference for the object is valid
	
	@return Whether the CF reference for the object is valid				*/
	

template <typename TCFRef>
bool
CFObject<TCFRef>::IsValid() const
{
	return (mCFRef != nil);
}


// ---------------------------------------------------------------------------
//	HasSameRef														  [public]
/**
	Returns whether the CFObject has the same CF reference as the input one
	
	@param	inRef		CF referce to check for equality
	
	@return Whether the CFObject has the same CF reference as the input one	*/

template <typename TCFRef>
bool
CFObject<TCFRef>::HasSameRef(
	TCFRef	inRef) const
{
	return (inRef == mCFRef);
}


// ---------------------------------------------------------------------------
//	GetTypeID														  [public]
/**
	Returns the CFTypeID for the object's CF reference
	
	@return CFTypeID for the object's CF reference							*/

template <typename TCFRef>
CFTypeID
CFObject<TCFRef>::GetTypeID() const
{
	return ::CFGetTypeID( UseRef() );
}


// ---------------------------------------------------------------------------
//	CopyTypeIDDescription											  [public]
/**
	Returns the type ID description of the object's CF reference
	
	@return Type ID description of the object's CF reference
	
	Caller is responsible for releasing the returned CFStringRef			*/

template <typename TCFRef>
CFStringRef
CFObject<TCFRef>::CopyTypeIDDescription() const
{
	return ::CFCopyTypeIDDescription( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetAllocator													  [public]
/**
	Returns the CF Allocator used by the object's CF reference
	
	@return	CF Allocator used by the object's CF reference					*/

template <typename TCFRef>
CFAllocatorRef
CFObject<TCFRef>::GetAllocator() const
{
	return ::CFGetAllocator( UseRef() );
}


// ---------------------------------------------------------------------------
//	CopyDescription													  [public]
/**
	Returns the description of the object's CF reference
	
	@return Description of the object's CF reference
	
	Caller is responsible for releasing the returned CFStringRef			*/

template <typename TCFRef>
CFStringRef
CFObject<TCFRef>::CopyDescription() const
{
	return ::CFCopyDescription( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetHashCode														  [public]
/**
	Returns the hash code for the object's CF reference
	
	@return Hash code for the object's CF reference							*/

template <typename TCFRef>
CFHashCode
CFObject<TCFRef>::GetHashCode() const
{
	return ::CFHash( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetRetainCount													  [public]
/**
	Returns the retain count for the object's CF reference
	
	@ return Retain count for the object's CF reference						*/

template <typename TCFRef>
CFIndex
CFObject<TCFRef>::GetRetainCount() const
{
	return ::CFGetRetainCount( UseRef() );
}


// ---------------------------------------------------------------------------
//	IsEqualTo														  [public]
/**
	Returns whether the object's CF reference is equalent to another
	CF reference
	
	@param	inRef	CF reference to test for equivalence'
	
	Unlike HasSameRef(), this function tests the equality of the items'
	contents rather than the CF reference values. The meaning of equality
	depends on the actual CF type of the item.								*/

template <typename TCFRef>
bool
CFObject<TCFRef>::IsEqualTo(
	TCFRef	inRef) const
{
	return ::CFEqual(mCFRef, inRef);
}


// ---------------------------------------------------------------------------
//	AttachRef														  [public]
/**
	Takes ownership of another CF reference. Releases the currently owned
	CF reference, and optionally retains the input one.

	@param	inRef		CF reference to adopt
	@param	inRetain	Whether to retain the CF reference
	
	Pass false for inRetain if you are transferring ownership of the
	CF reference, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF reference.
	
	Pase true for inRetain if the caller wants to maintain shared ownership
	of the CF reference.													*/

template <typename TCFRef>
void
CFObject<TCFRef>::AttachRef(
	TCFRef	inRef,
	bool	inRetain)
{
	if (HasSameRef(inRef)) return;
	
	Release();
	mCFRef = inRef;
	
	if (inRetain) {
		Retain();
	}
}


// ---------------------------------------------------------------------------
//	DetachRef														  [public]
/**
	Relinquishes ownership of the object's CF reference. Caller is responsible
	for releasing the CF reference.

	@return	CF reference that object used to own
	
	After detaching, the object does not own any CF reference. Any
	attempt to use the CF reference is an error.							*/

template <typename TCFRef>
TCFRef
CFObject<TCFRef>::DetachRef()
{
	TCFRef	detachedRef = mCFRef;
	mCFRef = nil;
	
	return detachedRef;
}


// ---------------------------------------------------------------------------
//	FreeRef															  [public]
/**
	Releases ownership of the object's CF reference
	
	After freeing, the object does not own any CF reference. Any
	attempt to use the CF reference is an error.							*/

template <typename TCFRef>
void
CFObject<TCFRef>::FreeRef()
{
	Release();
	mCFRef = nil;
}


// ---------------------------------------------------------------------------
//	CFObject													   [protected]
/**
	Default constructor */

template <typename TCFRef>
CFObject<TCFRef>::CFObject()
{
	mCFRef = nil;
}


// ---------------------------------------------------------------------------
//	CFObject													   [protected]
/**
	Constructs from an existing CF Reference.

	@param	inRef		CF reference to adopt
	@param	inRetain	Whether to retain the CF reference
	
	Pass false for inRetain if you are transferring ownership of the
	CF reference, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF reference.
	
	Pase true for inRetain if the caller wants to maintain shared ownership
	of the CF reference.													*/

template <typename TCFRef>
CFObject<TCFRef>::CFObject(
	TCFRef	inRef,
	bool	inRetain)
{
	mCFRef = inRef;
	
	if (inRetain) {
		Retain();
	}
}


// ---------------------------------------------------------------------------
//	CFObject													   [protected]
/**
	Copy constructor */

template <typename TCFRef>
CFObject<TCFRef>::CFObject(
	const CFObject&	inOriginal)
{
	mCFRef = nil;
	AssignObject(inOriginal);
}


// ---------------------------------------------------------------------------
//	~CFObject													   [protected]
/**
	Destructor */

template <typename TCFRef>
CFObject<TCFRef>::~CFObject()
{
	Release();
}


// ---------------------------------------------------------------------------
//	AssignObject												   [protected]
/**
	Share ownership of the CF reference owned by another object
	
	@param	inOriginal	Share CF reference of this CFObject					*/

template <typename TCFRef>
void
CFObject<TCFRef>::AssignObject(
	const CFObject&	inOriginal)
{
	AttachRef(inOriginal.mCFRef, retain_Yes);
}


// ---------------------------------------------------------------------------
//	Retain															 [private]

template <typename TCFRef>
void
CFObject<TCFRef>::Retain()
{
	if (IsValid()) {
		::CFRetain(mCFRef);
	}
}


// ---------------------------------------------------------------------------
//	Release															 [private]

template <typename TCFRef>
void
CFObject<TCFRef>::Release()
{
	if (IsValid()) {
		::CFRelease(mCFRef);
	}
}


// ===========================================================================
/** Template function for retaining a CF reference

	@param	inRef		CF reference to retain
	
	@return	The CF reference that was retained
	
	@note The Toolbox CFRetain() function returns a generic CFTypeRef.
	This template function returns the same CF reference type as the
	input parameter, making it type-safe to use the return value.			*/

template <typename TCFRef>
TCFRef
RetainCFRef(
	TCFRef	inRef)
{
	return (TCFRef) ::CFRetain(inRef);
}


// ===========================================================================
//	PPx_ThrowIfCFCreateFailed_( inCFRef, inFuncName )
/**
	Macro for throwing an exception if a Toolbox function which creates
	a Core Foundation object fails. The CF reference returned is nil
	if creation failed.

	@param	inCFRef		A CF reference
	@param	inFuncName	Literal string name of CF creator function
*/

#define	PPx_ThrowIfCFCreateFailed_(inCFRef, inFuncName)						\
			PPx_ThrowIfNil_(inCFRef, PPx::RuntimeError, PPx::err_CFCreate,	\
							inFuncName " failed")

} // namespace PPx

#endif // H_PPxCFObject
