// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventTarget.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxEventTarget.h
	@brief		Classes for Carbon Event target objects
*/

#ifndef H_EventTarget
#define H_EventTarget
#pragma once

#include <PPxPersistent.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	EventTarget
/**
	Abstract class for the target of a Carbon Event */

class	EventTarget : public Persistent {
public:
	EventTargetRef			GetSysEventTarget() const;
	
private:
	virtual EventTargetRef	SysEventTarget() const = 0;
};


// ---------------------------------------------------------------------------
//	GetSysEventTarget
/**
	Returns the associated system EventTargetRef */

inline
EventTargetRef
EventTarget::GetSysEventTarget() const
{
	return SysEventTarget();
}


// ===========================================================================
//	ApplicationEventTarget
/**
	The top-level Carbon Event target */

class	ApplicationEventTarget : public EventTarget {
private:
	virtual EventTargetRef	SysEventTarget() const;

	virtual CFStringRef		ClassName() const;
};


// ===========================================================================
//	UserFocusEventTarget
/**
	Carbon Event target for the current user focus */

class	UserFocusEventTarget : public EventTarget {
private:
	virtual EventTargetRef	SysEventTarget() const;
	
	virtual CFStringRef		ClassName() const;
};


// ===========================================================================
//	FrontWindowEventTarget
/**
	Carbon Event target for the front window of a window layer */

class	FrontWindowEventTarget : public EventTarget {
public:
							FrontWindowEventTarget();
							
	explicit				FrontWindowEventTarget( WindowClass inWindowClass );

protected:
	virtual void			InitState( const DataReader& inReader );

	virtual void			WriteState( DataWriter& ioWriter ) const;

private:
	virtual EventTargetRef	SysEventTarget() const;
	
	virtual CFStringRef		ClassName() const;

private:
	WindowClass		mWindowClass;		/**< Kind of system window */
};


// ---------------------------------------------------------------------------
//	EMetaTarget
/**
	Meta targets for events */

enum EMetaTarget {
	metaTarget_Self = 0,
	metaTarget_UserFocus = 1,
	metaTarget_Application = 2
};


} // namespace PPx

#endif	// H_EventTarget
