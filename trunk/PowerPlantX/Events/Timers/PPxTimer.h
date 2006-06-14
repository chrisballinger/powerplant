// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTimer.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:17 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxTimer.h
	@brief		Base classes for event loop timers and idle timers
*/

#ifndef H_PPxTimer
#define H_PPxTimer
#pragma once

#include <SysEventLoopTimer.h>

namespace PPx {

// ===========================================================================
//	Timer
/**
	Abstract class for an Event Loop Timer. Timers fires at regular
	intervals while the program is running. This includes during
	system calls that may block, such as mouse down tracking.				*/

class	Timer {
public:
						Timer();
						
						Timer(	EventLoopRef		inEventLoop,
								EventTimerInterval	inFireDelay,
								EventTimerInterval	inInterval);
								
	virtual				~Timer();
	
	void				Install(
								EventLoopRef		inEventLoop,
								EventTimerInterval	inFireDelay,
								EventTimerInterval	inInterval);
								
	void				Remove();
	
	bool				IsTimerInstalled() const;
	
	void				SetNextFireTime( EventTimerInterval inNextFire );
	
	void				Invoke();
	
private:
	virtual void		DoTimer() = 0;

										// Forbid copy and assignment
						Timer( const Timer&);
	Timer&				operator = (const Timer&);
	
private:
	SysEventLoopTimer	mEventTimer;	/**< System implementation object
												for an event loop timer */
};


// ---------------------------------------------------------------------------

inline void
Timer::Invoke()
{
	DoTimer();
}


// ===========================================================================
//	IdleTimer
/**
	Abstract class for an Event Loop Idle Timer. Idle timers fire only
	when there is no user activity, such as clicking, typing, and mouse
	down tracking, directed at the program.									*/

class	IdleTimer {
public:
						IdleTimer();
						
						IdleTimer(
								EventLoopRef		inEventLoop,
								EventTimerInterval	inFireDelay,
								EventTimerInterval	inInterval);
								
	virtual				~IdleTimer();
	
	void				Install(
								EventLoopRef		inEventLoop,
								EventTimerInterval	inFireDelay,
								EventTimerInterval	inInterval);
								
	void				Remove();
	
	bool				IsTimerInstalled() const;
	
	void				SetNextFireTime( EventTimerInterval inNextFire );
	
	void				Invoke( EventLoopIdleTimerMessage inMessage );

private:
	virtual void		DoIdleTimer( EventLoopIdleTimerMessage inMessage ) = 0;

						IdleTimer( const IdleTimer& );
	IdleTimer&			operator = (const IdleTimer& );
	
private:
	SysEventLoopIdleTimer	mIdleTimer;		/**< System implementation object
												for an event loop idle timer */
};


// ---------------------------------------------------------------------------

inline void
IdleTimer::Invoke(
	EventLoopIdleTimerMessage	inMessage)
{
	DoIdleTimer(inMessage);
}


// ===========================================================================
/**	Template class for a Timer that calls an object member function */

template <class T>
class	TimerCallback : public Timer {
public:

typedef	void	(T:: * CallbackFunction)();


// ---------------------------------------------------------------------------

TimerCallback()
{
	mObject		= nil;
	mFunction	= nil;
}

// ---------------------------------------------------------------------------

void
Install(
	T*					inObject,
	CallbackFunction	inFunction,
	EventLoopRef		inEventLoop,
	EventTimerInterval	inFireDelay,
	EventTimerInterval	inInterval)
{
	mObject		= inObject;
	mFunction	= inFunction;
	
	Timer::Install(inEventLoop, inFireDelay, inInterval);
}


// ---------------------------------------------------------------------------

virtual
void
DoTimer()
{
	(mObject->*mFunction)();
}


// ---------------------------------------------------------------------------

private:
	T*					mObject;
	CallbackFunction	mFunction;
};


// ===========================================================================
/**	Template class for an IdleTimer that calls an object member function */

template <class T>
class	IdleTimerCallback : public IdleTimer {
public:

typedef	void	(T:: * CallbackFunction)();


// ---------------------------------------------------------------------------

IdleTimerCallback()
{
	mObject		= nil;
	mFunction	= nil;
}

// ---------------------------------------------------------------------------

void
Install(
	T*					inObject,
	CallbackFunction	inFunction,
	EventLoopRef		inEventLoop,
	EventTimerInterval	inFireDelay,
	EventTimerInterval	inInterval)
{
	mObject		= inObject;
	mFunction	= inFunction;
	
	IdleTimer::Install(inEventLoop, inFireDelay, inInterval);
}


// ---------------------------------------------------------------------------

virtual
void
DoIdleTimer(
	EventLoopIdleTimerMessage	inMessage)
{
	(mObject->*mFunction)(inMessage);
}


// ---------------------------------------------------------------------------

private:
	T*					mObject;
	CallbackFunction	mFunction;
};


} // namespace PPx

#endif	// H_PPxTimer
