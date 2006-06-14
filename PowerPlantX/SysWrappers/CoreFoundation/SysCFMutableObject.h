// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFMutableObject.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:29 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFMutableObject.h
	@brief		Template base class for Core Foundation wrapper classes
				for mutable objects
*/

#ifndef H_SysCFMutableObject
#define H_SysCFMutableObject
#pragma once

#include <SysCFObject.h>

namespace PPx {

// ===========================================================================
//	CFMutableRef
/**
	Template base class for Core Foundation wrapper classes for
	mutable objects.

	The template parameters are the CF reference type and the CF mutable
	reference type. CFMutableObject implements copy-on-modify behavior.
	Using a mutable reference results in a copy in two cases:
	
		(1)	the object is currently immutable
		(2)	the object is mutable and it is being shared (retain count
			greater than one)												*/

template <typename TCFRef, typename TMutableRef>
class	CFMutableObject : public CFObject<TCFRef> {
public:
									// Accessing Toolbox mutable CF reference
	TMutableRef			UseMutableRef();
									
									/** Converts to a TMutableRef */
						operator TMutableRef()	{ return UseMutableRef(); }
	
									// Changing ownership of CF reference
	void				AttachMutableRef(
								TMutableRef	inMutableRef,
								bool		inRetain);
	
	TMutableRef			DetachMutableRef();
	
protected:
									// Protected constructors and destructor
									//   mean this must only be a base class
						CFMutableObject();
						
						CFMutableObject(
								TCFRef	inRef,
								bool	inRetain);
								
						CFMutableObject( const CFMutableObject& inOriginal );
						
	virtual				~CFMutableObject();

									// Utilities for subclasses
	void				AssignObject( const CFMutableObject& inOriginal );							

private:							// Utility that subclasses must implement
	virtual TMutableRef	MutableCopy( TCFRef inRefToCopy ) const = 0;
						
	void				MakeMutable();	// Private utility
	
	typedef CFObject<TCFRef>	BaseT;
						
private:
	bool			mIsMutable;		/**< Whether CF reference is mutable */
};


// ---------------------------------------------------------------------------
//	UseMutableRef													  [public]
/**
	Returns mutable CF reference
	
	@return	A mutable CF refenrece */

template <typename TCFRef, typename TMutableRef>
TMutableRef
CFMutableObject<TCFRef, TMutableRef>::UseMutableRef()
{
	MakeMutable();
	
	return const_cast<TMutableRef>(BaseT::UseRef());
}


// ---------------------------------------------------------------------------
//	AttachMutableRef												  [public]
/**
	Takes ownership of another CF reference. Releases the currently owned
	CF reference, and optionally retains the input one.

	@param	inMutableRef	CF reference to adopt
	@param	inRetain		Whether to retain the CF reference
	
	Pass false for inRetain if you are transferring ownership of the
	CF reference, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF reference.
	
	Pase true for inRetain if the caller wants to maintain ownership
	of the CF reference. In this case, we make a copy, so that multiple
	clients aren't changing the same data.									*/

template <typename TCFRef, typename TMutableRef>
void
CFMutableObject<TCFRef, TMutableRef>::AttachMutableRef(
	TMutableRef	inMutableRef,
	bool		inRetain)
{
	TMutableRef	refToAttach = inMutableRef;

	if (inRetain and (inMutableRef != nil)) {
		refToAttach = MutableCopy(inMutableRef);
	}
	
	BaseT::AttachRef(refToAttach, retain_No);
	mIsMutable = (refToAttach != nil);
}


// ---------------------------------------------------------------------------
//	DetachMutableRef												  [public]
/**
	Relinquishes ownership of the object's CF reference. Caller is responsible
	for releasing the CF reference.

	@return	CF reference that object used to own
	
	After detaching, the object does not own any CF reference. Any
	attempt to use the CF reference is an error.
*/

template <typename TCFRef, typename TMutableRef>
TMutableRef
CFMutableObject<TCFRef, TMutableRef>::DetachMutableRef()
{
		// Caller is specifically asking for a mutable CF reference.
		// We must make our reference mutable if it isn't already.
		
	MakeMutable();
	return const_cast<TMutableRef>(BaseT::DetachRef());
}


// ---------------------------------------------------------------------------
//	CFMutableObject												   [protected]
/**
	Default constructor */

template <typename TCFRef, typename TMutableRef>
CFMutableObject<TCFRef, TMutableRef>::CFMutableObject()
{
	mIsMutable = false;
}


// ---------------------------------------------------------------------------
//	CFMutableObject												   [protected]
/**
	Constructs from an existing CF Reference.

	@param	inRef		CF reference to adopt
	@param	inRetain	Whether to retain the CF reference
	
	Pass false for inRetain if you are transferring ownership of the
	CF reference, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF reference.
	
	Pase true for inRetain if the caller wants to maintain shared ownership
	of the CF reference.													*/

template <typename TCFRef, typename TMutableRef>
CFMutableObject<TCFRef, TMutableRef>::CFMutableObject(
	TCFRef	inRef,
	bool	inRetain)
	
	: BaseT(inRef, inRetain)
{
	mIsMutable = false;
}


// ---------------------------------------------------------------------------
//	CFMutableObject												   [protected]
/**
	Copy constructor */

template <typename TCFRef, typename TMutableRef>
CFMutableObject<TCFRef, TMutableRef>::CFMutableObject(
	const CFMutableObject&	inOriginal)
	
	: BaseT(inOriginal)
{
	mIsMutable = inOriginal.mIsMutable;
}


// ---------------------------------------------------------------------------
//	~CFMutableObject											   [protected]
/**
	Destructor */

template <typename TCFRef, typename TMutableRef>
CFMutableObject<TCFRef, TMutableRef>::~CFMutableObject()
{
}


// ---------------------------------------------------------------------------
//	AssignObject												   [protected]
/**
	Shares ownership of the CF reference owned by another object
	
	@param	inOriginal		Share the CF refernce of this CFObject			*/

template <typename TCFRef, typename TMutableRef>
void
CFMutableObject<TCFRef, TMutableRef>::AssignObject(
	const CFMutableObject&	inOriginal)
{
	BaseT::AssignObject(inOriginal);
	mIsMutable = inOriginal.mIsMutable;
}


// ---------------------------------------------------------------------------
//	MakeMutable														 [private]
/**
	Makes a mutable copy of the object's CF reference
	
	Subclasses must implement the pure virtual MutableCopy() function
	to perform the actual copying.											*/

template <typename TCFRef, typename TMutableRef>
void
CFMutableObject<TCFRef, TMutableRef>::MakeMutable()
{
	if (not BaseT::IsValid()) return;
	
	if ((not mIsMutable) or (BaseT::GetRetainCount() > 1)) {
	
		TMutableRef	copyRef = MutableCopy( BaseT::UseRef() );
		AttachMutableRef(copyRef, retain_No);
	}
}


} // namespace PPx

#endif // H_SysCFMutableObject
