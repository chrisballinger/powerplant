// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysEventLoopTimer.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:34 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysEventLoopTimer.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysEventLoopTimer												  [public]
/**
	Default constructor */

SysEventLoopTimer::SysEventLoopTimer()
{
	mTimerRef = nil;
}


// ---------------------------------------------------------------------------
//	~SysEventLoopTimer												  [public]
/**
	Destructor */

SysEventLoopTimer::~SysEventLoopTimer()
{
	Remove();				// Remove timer from System event loop
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Install timer onto a System event loop
	
	@param	inEventLoop		System event loop. Usually you will pass in the
								return value from ::GetMainEventLoop()
								or ::GetCurrentEventLoop().
	@param	inFireDelay		Seconds before first firing of the timer
	@param	inInterval		Seconds between successive firings of the timer.
								If zero, the timer fires once, but is still
								installed. To fire it again, you can call
								SetNextFireTime().
	@param	inTimerUPP		Timer callback function
	@param	inUserData		User data passed to callback function
									
	@return	System error code

	@note This function removes the timer if it is already installed, then
	re-installs it. Thus, you may call Install() more than once. You will
	need to do this in order to change the Event Loop or the Interval.		*/

OSStatus
SysEventLoopTimer::Install(
	EventLoopRef		inEventLoop,
	EventTimerInterval	inFireDelay,
	EventTimerInterval	inInterval,
	EventLoopTimerUPP	inTimerUPP,
	void*				inUserData)
{
	if (mTimerRef != nil) {			// Already installed. Remove it.
		Remove();
	}

	return ::InstallEventLoopTimer( inEventLoop,	inFireDelay,
									inInterval,		inTimerUPP,
									inUserData,		&mTimerRef );
}


// ---------------------------------------------------------------------------
//	Remove															  [public]
/**
	Remove timer from its event loop. You can re-install the timer later
	by calling Install().													*/

void
SysEventLoopTimer::Remove()
{
	if (mTimerRef != nil) {
		::RemoveEventLoopTimer(mTimerRef);
		mTimerRef = nil;
	}
}


// ---------------------------------------------------------------------------
//	IsInstalled														  [public]
/**
	Returns whether the timer is installed onto an event loop
	
	@return Whether the timer is installed									*/

bool
SysEventLoopTimer::IsInstalled() const
{
	return (mTimerRef != nil);		// Timer is installed if Ref exists
}


// ---------------------------------------------------------------------------
//	SetNextFireTime													  [public]
/**
	Specify the next firing time
	
	@param	inNextFire		Next firing time, in seconds

	@note Firing time temporarily overrides the interval until the next
	firing. For example, if you installed the timer with a 1 second interval,
	and then call SetNextFireTime(5), the timer will cease firing for 5
	seconds, fire, and then fire again at 1 second intervals.				*/

OSStatus
SysEventLoopTimer::SetNextFireTime(
	EventTimerInterval	inNextFire)
{
	return ::SetEventLoopTimerNextFireTime(mTimerRef, inNextFire);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysEventLoopIdleTimer											  [public]
/**
	Default constructor */

SysEventLoopIdleTimer::SysEventLoopIdleTimer()
{
	mTimerRef = nil;
}


// ---------------------------------------------------------------------------
//	~SysEventLoopIdleTimer											  [public]
/**
	Destructor */

SysEventLoopIdleTimer::~SysEventLoopIdleTimer()
{
	Remove();				// Remove timer from System event loop
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Install idle timer onto a System event loop
	
	@param	inEventLoop		System event loop. Usually you will pass in the
								return value from ::GetMainEventLoop()
								or ::GetCurrentEventLoop().
	@param	inFireDelay		Seconds before first firing of the timer
	@param	inInterval		Seconds between successive firings of the timer.
								If zero, the timer fires once, but is still
								installed. To fire it again, you can call
								SetNextFireTime().
	@param	inIdleTimerUPP	Timer callback function
	@param	inUserData		User data passed to callback function
									
	@return	System error code

	@note This function removes the timer if it is already installed, then
	re-installs it. Thus, you may call Install() more than once. You will
	need to do this in order to change the Event Loop or the Interval.		*/

OSStatus
SysEventLoopIdleTimer::Install(
	EventLoopRef			inEventLoop,
	EventTimerInterval		inFireDelay,
	EventTimerInterval		inInterval,
	EventLoopIdleTimerUPP	inIdleTimerUPP,
	void*					inUserData)
{
	if (mTimerRef != nil) {			// Already installed. Remove it.
		Remove();
	}

	return ::InstallEventLoopIdleTimer( inEventLoop,	inFireDelay,
										inInterval,		inIdleTimerUPP,
										inUserData,		&mTimerRef );
}


// ---------------------------------------------------------------------------
//	Remove															  [public]
/**
	Remove idle timer from its event loop. You can re-install the idle timer
	later by calling Install().												*/

void
SysEventLoopIdleTimer::Remove()
{
	if (mTimerRef != nil) {
		::RemoveEventLoopTimer(mTimerRef);
		mTimerRef = nil;
	}
}


// ---------------------------------------------------------------------------
//	IsInstalled														  [public]
/**
	Returns whether the idle imer is installed onto an event loop
	
	@return Whether the idle timer is installed								*/

bool
SysEventLoopIdleTimer::IsInstalled() const
{
	return (mTimerRef != nil);		// Timer is installed if Ref exists
}


// ---------------------------------------------------------------------------
//	SetNextFireTime													  [public]
/**
	Specify the next firing time
	
	@param	inNextFire		Next firing time, in seconds

	@note Firing time temporarily overrides the interval until the next
	firing. For example, if you installed the timer with a 1 second interval,
	and then call SetNextFireTime(5), the timer will cease firing for 5
	seconds, fire, and then fire again at 1 second intervals.				*/

OSStatus
SysEventLoopIdleTimer::SetNextFireTime(
	EventTimerInterval	inNextFire)
{
	return ::SetEventLoopTimerNextFireTime(mTimerRef, inNextFire);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysEventLoopTimerUPP											  [public]
/**
	Constructs from a pointer to an event loop timer callback function */

SysEventLoopTimerUPP::SysEventLoopTimerUPP(
	EventLoopTimerProcPtr	inCallbackFunc)
{
	mEventLoopTimerUPP = ::NewEventLoopTimerUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysEventLoopTimerUPP											  [public]
/**
	Destructor */

SysEventLoopTimerUPP::~SysEventLoopTimerUPP()
{
	::DisposeEventLoopTimerUPP(mEventLoopTimerUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysEventLoopIdleTimerUPP										  [public]
/**
	Constructs from a pointer to an event loop idle timer callback function */

SysEventLoopIdleTimerUPP::SysEventLoopIdleTimerUPP(
	EventLoopIdleTimerProcPtr	inCallbackFunc)
{
	mEventLoopIdleTimerUPP = ::NewEventLoopIdleTimerUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysEventLoopIdleTimerUPP										  [public]
/**
	Destructor */

SysEventLoopIdleTimerUPP::~SysEventLoopIdleTimerUPP()
{
	::DisposeEventLoopIdleTimerUPP(mEventLoopIdleTimerUPP);
}


} // namespace PPx