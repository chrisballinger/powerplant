// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysIcon.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:36 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysIcon.h
	@brief		Wrapper class for a Mac Toolbox IconRef
*/

#ifndef H_SysIcon
#define H_SysIcon
#pragma once

#include <PPxConstants.h>
#include <PPxExceptions.h>

#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	AutoIcon
/**
	Manages ownership of Toolbox IconRef */

class	AutoIcon {
public:
						AutoIcon();
						
	explicit			AutoIcon( IconRef inIconRef );
						
						AutoIcon( const AutoIcon& inOriginal );

						~AutoIcon();
						
						operator IconRef() const;
						
	AutoIcon&			operator=( const AutoIcon& inSource );

	IconRef				Get() const;
	
	IconRef				Release();
	
	void				Reset();
	
	void				Reset( IconRef inIconRef );
	
private:
	void				RetainObject();
	
	void				ReleaseObject();

private:
	IconRef				mIconRef;
};


// ---------------------------------------------------------------------------
//	AutoIcon
/**
	Default constructor.  Owned IconRef is nil. */

inline
AutoIcon::AutoIcon()
{
	mIconRef = nil;
}


// ---------------------------------------------------------------------------
//	AutoIcon
/**
	Constructs from a IconRef which becomes the owned IconRef
	
	@param	inIconRef	Object takes ownership of this IconRef 				*/

inline
AutoIcon::AutoIcon( IconRef inIconRef )
{
	mIconRef = inIconRef;
}


// ---------------------------------------------------------------------------
//	AutoIcon
/**
	Constructs from a AutoIcon
	
	@param	inOriginal	+++ 				*/

inline
AutoIcon::AutoIcon( const AutoIcon& inOriginal )
{
	mIconRef = inOriginal.mIconRef;
	RetainObject();
}


// ---------------------------------------------------------------------------
//	~AutoIcon
/**
	Destructor.  Disposes of the owned IconRef */

inline
AutoIcon::~AutoIcon()
{
	Reset();
}


// ---------------------------------------------------------------------------
//	operator IconRef
/**
	Returns the IconRef
	
	@return IconRef owned by the AutoIcon									*/

inline
AutoIcon::operator IconRef() const
{
	return mIconRef;
}


// ---------------------------------------------------------------------------
//	operator=
/**
	Assignment of AutoIcon from another AutoIcon
	
	@param	inSource	+++

	@return A reference to this AutoIcon									*/

inline AutoIcon&
AutoIcon::operator=( const AutoIcon& inSource )
{
	if (inSource == *this)
		return *this;
	
	ReleaseObject();
	mIconRef = inSource.mIconRef;
	RetainObject();
	
	return *this;
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns the IconRef
	
	@return IconRef owned by the AutoIcon									*/

inline IconRef
AutoIcon::Get() const
{
	return mIconRef;
}


// ---------------------------------------------------------------------------
//	Release
/**
	Releases ownership of the IconRef and sets owned IconRef to nil */

inline IconRef
AutoIcon::Release()
{
	IconRef temp = mIconRef;
	mIconRef = nil;
	return temp;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Disposes existing IconRef and sets owned IconRef to nil */

inline void
AutoIcon::Reset()
{
	if (mIconRef != nil) {
		ReleaseObject();
	}
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Disposes existing IconRef and takes ownership of input IconRef
	
	@param	inIconRef	Object takes ownership of this IconRef				*/

inline void
AutoIcon::Reset( IconRef inIconRef )
{
	if (inIconRef != mIconRef) {
		Reset();
		mIconRef = inIconRef;
	}
}


// ---------------------------------------------------------------------------
//	RetainObject
/**
	Increments the reference count of the owned IconRef */

inline void
AutoIcon::RetainObject()
{
	if (::IsValidIconRef(mIconRef)) {
		OSStatus	status = ::AcquireIconRef(mIconRef);
		PPx_ThrowIfOSError_(status, "AcquireIconRef failed");
	}
}


// ---------------------------------------------------------------------------
//	ReleaseObject
/**
	Decrements the reference count of the owned IconRef */

inline void
AutoIcon::ReleaseObject()
{
	if (::IsValidIconRef(mIconRef)) {
		OSStatus	status = ::ReleaseIconRef(mIconRef);
		PPx_ThrowIfOSError_(status, "ReleaseIconRef failed");
		mIconRef = nil;
	}
}


} // namespace PPx

#endif	// H_SysIcon
