// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysMenu.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysMenu.h
	@brief		Wrapper class for a Mac Toolbox MenuRef
*/

#ifndef H_SysMenu
#define H_SysMenu
#pragma once

#include <PPxConstants.h>
#include <PPxExceptions.h>

#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	AutoMenu
/**
	Manages ownership of Toolbox MenuRef */

class	AutoMenu {
public:
						AutoMenu();
						
	explicit			AutoMenu( MenuRef inMenuRef );
						
						AutoMenu( const AutoMenu& inOriginal );

						~AutoMenu();
						
						operator MenuRef() const;
						
	AutoMenu&			operator=( const AutoMenu& inSource );

	MenuRef				Get() const;
	
	MenuRef				Release();
	
	void				Reset();
	
	void				Reset( MenuRef inMenuRef );
	
private:
	void				RetainObject();
	
	void				ReleaseObject();

private:
	MenuRef				mMenuRef;
};


// ---------------------------------------------------------------------------
//	AutoMenu
/**
	Default constructor.  Owned MenuRef is nil. */

inline
AutoMenu::AutoMenu()
{
	mMenuRef = nil;
}


// ---------------------------------------------------------------------------
//	AutoMenu
/**
	Constructs from a MenuRef which becomes the owned MenuRef
	
	@param	inMenuRef	Object takes ownership of this MenuRef 				*/

inline
AutoMenu::AutoMenu( MenuRef inMenuRef )
{
	mMenuRef = inMenuRef;
}


// ---------------------------------------------------------------------------
//	AutoMenu
/**
	Constructs from a AutoMenu
	
	@param	inOriginal	+++ 				*/

inline
AutoMenu::AutoMenu( const AutoMenu& inOriginal )
{
	mMenuRef = inOriginal.mMenuRef;
	RetainObject();
}


// ---------------------------------------------------------------------------
//	~AutoMenu
/**
	Destructor.  Disposes of the owned MenuRef */

inline
AutoMenu::~AutoMenu()
{
	Reset();
}


// ---------------------------------------------------------------------------
//	operator MenuRef
/**
	Returns the MenuRef
	
	@return MenuRef owned by the AutoMenu									*/

inline
AutoMenu::operator MenuRef() const
{
	return mMenuRef;
}


// ---------------------------------------------------------------------------
//	operator=
/**
	Assignment of AutoMenu from another AutoMenu
	
	@param	inSource	+++

	@return A reference to this AutoMenu									*/

inline AutoMenu&
AutoMenu::operator=( const AutoMenu& inSource )
{
	if (inSource == *this)
		return *this;
	
	ReleaseObject();
	mMenuRef = inSource.mMenuRef;
	RetainObject();
	
	return *this;
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns the MenuRef
	
	@return MenuRef owned by the AutoMenu									*/

inline MenuRef
AutoMenu::Get() const
{
	return mMenuRef;
}


// ---------------------------------------------------------------------------
//	Release
/**
	Releases ownership of the MenuRef and sets owned MenuRef to nil */

inline MenuRef
AutoMenu::Release()
{
	MenuRef temp = mMenuRef;
	mMenuRef = nil;
	return temp;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Disposes existing MenuRef and sets owned MenuRef to nil */

inline void
AutoMenu::Reset()
{
	if (mMenuRef != nil) {
		ReleaseObject();
	}
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Disposes existing MenuRef and takes ownership of input MenuRef
	
	@param	inMenuRef	Object takes ownership of this MenuRef				*/

inline void
AutoMenu::Reset( MenuRef inMenuRef )
{
	if (inMenuRef != mMenuRef) {
		Reset();
		mMenuRef = inMenuRef;
	}
}


// ---------------------------------------------------------------------------
//	RetainObject
/**
	Increments the reference count of the owned MenuRef */

inline void
AutoMenu::RetainObject()
{
	if (::IsValidMenu(mMenuRef)) {
		OSStatus	status = ::RetainMenu(mMenuRef);
		PPx_ThrowIfOSError_(status, "RetainMenu failed");
	}
}


// ---------------------------------------------------------------------------
//	ReleaseObject
/**
	Decrements the reference count of the owned MenuRef */

inline void
AutoMenu::ReleaseObject()
{
	if (::IsValidMenu(mMenuRef)) {
		OSStatus	status = ::ReleaseMenu(mMenuRef);
		PPx_ThrowIfOSError_(status, "ReleaseMenu failed");
		mMenuRef = nil;
	}
}


} // namespace PPx

#endif	// H_SysMenu
