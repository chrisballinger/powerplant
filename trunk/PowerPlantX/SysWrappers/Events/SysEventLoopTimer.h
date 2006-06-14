// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysEventLoopTimer.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:34 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysEventLoopTimer.h
	@brief		Wrapper classes for event loop timers and idle timers
*/

#ifndef H_SysEventLoopTimer
#define H_SysEventLoopTimer
#pragma once

#include <PPxPrefix.h>
#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysEventLoopTimer
/**
	Wrapper class for an Event Loop Timer */

class	SysEventLoopTimer {
public:
						SysEventLoopTimer();
						
						~SysEventLoopTimer();
	
	OSStatus			Install(
								EventLoopRef		inEventLoop,
								EventTimerInterval	inFireDelay,
								EventTimerInterval	inInterval,
								EventLoopTimerUPP	inTimerUPP,
								void*				inUserData);
								
	void				Remove();
	
	bool				IsInstalled() const;
	
	OSStatus			SetNextFireTime( EventTimerInterval inNextFire );
	
private:									// Forbid copy and assignment
						SysEventLoopTimer( const SysEventLoopTimer& );
	SysEventLoopTimer&	operator = ( const SysEventLoopTimer& );

private:
	EventLoopTimerRef	mTimerRef;		/**< System reference to installed
												event loop timer */
};


// ===========================================================================
//	SysEventLoopIdleTimer
/**
	Wrapper class for an Event Loop Idle Timer */

class	SysEventLoopIdleTimer {
public:
						SysEventLoopIdleTimer();
						
						~SysEventLoopIdleTimer();
	
	OSStatus			Install(
								EventLoopRef			inEventLoop,
								EventTimerInterval		inFireDelay,
								EventTimerInterval		inInterval,
								EventLoopIdleTimerUPP	inIdleTimerUPP,
								void*					inUserData);
								
	void				Remove();
	
	bool				IsInstalled() const;
	
	OSStatus			SetNextFireTime( EventTimerInterval inNextFire );
	
private:									// Forbid copy and assignment
						SysEventLoopIdleTimer( const SysEventLoopIdleTimer& );
	SysEventLoopIdleTimer&	operator = ( const SysEventLoopIdleTimer& );

private:
	EventLoopTimerRef	mTimerRef;		/**< System reference to installed
												event loop idle timer */
};


// ===========================================================================
//	SysEventLoopTimerUPP
/**
	Wrapper class for an Event Loop Timer callback function UPP */

class	SysEventLoopTimerUPP {
public:
	explicit			SysEventLoopTimerUPP( EventLoopTimerProcPtr inCallbackFunc );
					
						~SysEventLoopTimerUPP();
					
	EventLoopTimerUPP	Get() const;
	
	void				Dispose();
	
private:									// Forbid copy and assignment
						SysEventLoopTimerUPP( const SysEventLoopTimerUPP& );
	SysEventLoopTimerUPP& operator = ( const SysEventLoopTimerUPP& );
	
private:
	EventLoopTimerUPP	mEventLoopTimerUPP;	/**< Callback function UPP */
};


// ---------------------------------------------------------------------------

inline
EventLoopTimerUPP
SysEventLoopTimerUPP::Get() const
{
	return mEventLoopTimerUPP;
}


// ===========================================================================
//	SysEventLoopIdleTimerUPP
/**
	Wrapper class for an Event Loop Idle Timer callback function UPP */

class	SysEventLoopIdleTimerUPP {
public:
	explicit			SysEventLoopIdleTimerUPP(
								EventLoopIdleTimerProcPtr	inCallbackFunc);
					
						~SysEventLoopIdleTimerUPP();
					
	EventLoopIdleTimerUPP	Get() const;
	
	void				Dispose();
	
private:									// Forbid copy and assignment
						SysEventLoopIdleTimerUPP( const SysEventLoopIdleTimerUPP& );
	SysEventLoopIdleTimerUPP& 	operator = ( const SysEventLoopIdleTimerUPP& );
	
private:
	EventLoopIdleTimerUPP	mEventLoopIdleTimerUPP;	/**< Callback function UPP */
};


// ---------------------------------------------------------------------------

inline
EventLoopIdleTimerUPP
SysEventLoopIdleTimerUPP::Get() const
{
	return mEventLoopIdleTimerUPP;
}


} // namespace PPx

#endif // SysEventLoopTimer
