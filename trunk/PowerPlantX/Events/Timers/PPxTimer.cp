// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTimer.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:17 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxTimer.h>

namespace PPx {

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Toolbox callback function for Event Loop Timers
	//
	//	We use the same callback function for all Timers. The user data
	//	is a pointer to a Timer object.
	
	pascal void
	TimerTBCallback(
		EventLoopTimerRef	/* inTimerRef */,
		void*				inUserData)
	{
		Timer*	theTimer = static_cast<Timer*>(inUserData);
		
		try {
			theTimer->Invoke();
		}
		
		catch (...) { }				// Can't throw out of a Toolbox callback
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to Event Loop Timer callback function
	
	EventLoopTimerUPP
	GetSysEventLoopTimerUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysEventLoopTimerUPP		sEventLoopTimerUPP(TimerTBCallback);
		
		return sEventLoopTimerUPP.Get();
	}

} // unnamed namespace

#pragma mark -

// ---------------------------------------------------------------------------
//	Timer															  [public]
/**
	Default Constructor */

Timer::Timer()
{
}


// ---------------------------------------------------------------------------
//	Timer															  [public]
/**
	Constructs and installs a Timer object
	
	@param	inEventLoop		Event loop on which to install the timer. Call
								::GetMainEventLoop() for the main application
								event loop; call ::GetCurrentEventLoop() for
								the event loop of the current thread.
	@param	inFireDelay		Time, in seconds, to delay before first call
	@param	inInterval		Time, in seconds, between timer calls			*/

Timer::Timer(
	EventLoopRef		inEventLoop,
	EventTimerInterval	inFireDelay,
	EventTimerInterval	inInterval)
{
	Install(inEventLoop, inFireDelay, inInterval);
}


// ---------------------------------------------------------------------------
//	~Timer															  [public]
/**
	Destructor */

Timer::~Timer()
{
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Installs a Timer. Call this function to re-install a timer that
	you have previously removed.
	
	@param	inEventLoop		Event loop on which to install the timer
	@param	inFireDelay		Time, in seconds, to delay before first call
	@param	inInterval		Time, in seconds, between timer calls			*/

void
Timer::Install(
	EventLoopRef		inEventLoop,
	EventTimerInterval	inFireDelay,
	EventTimerInterval	inInterval)
{
	mEventTimer.Install( inEventLoop, inFireDelay, inInterval,
						 GetSysEventLoopTimerUPP(), this );
}


// ---------------------------------------------------------------------------
//	Remove															  [public]
/**
	Uninstalls a Timer. You can call Install() later to re-intall it.		*/

void
Timer::Remove()
{
	mEventTimer.Remove();
}


// ---------------------------------------------------------------------------
//	IsTimerInstalled												  [public]
/**
	Returns whether a Timer is currently installed on an event loop
	
	@return Whether the Timer is currently installed						*/

bool
Timer::IsTimerInstalled() const
{
	return mEventTimer.IsInstalled();
}


// ---------------------------------------------------------------------------
//	SetNextFireTime													  [public]
/**
	Sets time delay until the Timer next fires. This temporarily overrides
	the Timer's interval.
	
	@param	inNextFire		Time, in seconds, until the Timer next fires	*/

void
Timer::SetNextFireTime(
	EventTimerInterval	inNextFire)
{
	PPx_ThrowIfOSError_( mEventTimer.SetNextFireTime(inNextFire),
						 "Failed to set next fire time. Timer is probably "
						 "not installed." );
}

#pragma mark -

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Toolbox callback function for Event Loop Idle Timers
	//
	//	We use the same callback function for all Idle Timers. The user data
	//	is a pointer to an IdleTimer object.
	
	pascal void
	IdleTimerTBCallback(
		EventLoopTimerRef			/* inTimerRef */,
		EventLoopIdleTimerMessage	inMessage,
		void*						inUserData)
	{
		IdleTimer*	theTimer = static_cast<IdleTimer*>(inUserData);
		
		try {
			theTimer->Invoke(inMessage);
		}
		
		catch (...) { }				// Can't throw out of a Toolbox callback
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to Event Loop Idle Timer callback function
	
	EventLoopIdleTimerUPP
	GetSysEventLoopIdleTimerUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysEventLoopIdleTimerUPP		sEventLoopIdleTimerUPP(IdleTimerTBCallback);
		
		return sEventLoopIdleTimerUPP.Get();
	}

} // unnamed namespace

#pragma mark -

// ---------------------------------------------------------------------------
//	IdleTimer														  [public]
/**
	Default constructor */

IdleTimer::IdleTimer()
{
}


// ---------------------------------------------------------------------------
//	IdleTimer														  [public]
/**
	Constructs and installs an Idle Timer
	
	@param	inEventLoop		Event loop on which to install the timer. Call
								::GetMainEventLoop() for the main application
								event loop; call ::GetCurrentEventLoop() for
								the event loop of the current thread.
	@param	inFireDelay		Time, in seconds, to delay before first call
	@param	inInterval		Time, in seconds, between timer calls			*/

IdleTimer::IdleTimer(
	EventLoopRef		inEventLoop,
	EventTimerInterval	inFireDelay,
	EventTimerInterval	inInterval)
{
	Install(inEventLoop, inFireDelay, inInterval);
}


// ---------------------------------------------------------------------------
//	~IdleTimer														  [public]
/**
	Destructor */

IdleTimer::~IdleTimer()
{
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Installs an Idle Timer. Call this function to re-install a timer that
	you have previously removed.
	
	@param	inEventLoop		Event loop on which to install the timer
	@param	inFireDelay		Time, in seconds, to delay before first call
	@param	inInterval		Time, in seconds, between timer calls			*/

void
IdleTimer::Install(
	EventLoopRef		inEventLoop,
	EventTimerInterval	inFireDelay,
	EventTimerInterval	inInterval)
{
	mIdleTimer.Install( inEventLoop, inFireDelay, inInterval,
						 GetSysEventLoopIdleTimerUPP(), this );
}


// ---------------------------------------------------------------------------
//	Remove															  [public]
/**
	Uninstalls an Idle Timer. You can later call Install() to re-install it	*/

void
IdleTimer::Remove()
{
	mIdleTimer.Remove();
}


// ---------------------------------------------------------------------------
//	IsTimerInstalled												  [public]
/**
	Returns whether an Idle Timer is currently installed on an event loop
	
	@return Whether the Idle Timer is currently installed					*/

bool
IdleTimer::IsTimerInstalled() const
{
	return mIdleTimer.IsInstalled();
}


// ---------------------------------------------------------------------------
//	SetNextFireTime													  [public]
/**
	Sets time delay until the Idle Timer next fires. This temporarily
	overrides the Timer's interval.
	
	@param	inNextFire		Time, in seconds, until the Timer next fires	*/

void
IdleTimer::SetNextFireTime(
	EventTimerInterval	inNextFire)
{
	PPx_ThrowIfOSError_( mIdleTimer.SetNextFireTime(inNextFire),
						 "Failed to set next fire time. Idle Timer is "
						 "probably not installed." );
}

} // namespace PPx
