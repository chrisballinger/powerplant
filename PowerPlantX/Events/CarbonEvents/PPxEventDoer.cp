// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventDoer.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxEventDoer.h>
#include <SysEventHandler.h>

namespace PPx {

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Toolbox callback function for Carbon Events
	//
	//	All EventDoers use this callback function

	pascal OSStatus
	EventHandlerCallback(
		EventHandlerCallRef		inCallRef,
		EventRef				inEventRef,
		void*					inUserData)
	{
			// EventDoer::Install() passes its "this" pointer as
			// the user data
	
		EventDoer*	theDoer = static_cast<EventDoer*>(inUserData);
								
		OSStatus	result = noErr;
		
		try {
			SysCarbonEvent	event(inEventRef, inCallRef);
			result = theDoer->Invoke(event);
		}
		
		catch (...) { }
		
		return result;
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to Carbon Event callback function

	EventHandlerUPP
	GetSysEventHandlerUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysEventHandlerUPP	sEventHandlerUPP(&EventHandlerCallback);
		
		return sEventHandlerUPP.Get();
	}
	

} // unnamed namespace


// ---------------------------------------------------------------------------
//	~EventDoer														  [public]
/**
	Destructor */

EventDoer::~EventDoer()
{
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Installs handler for a list of events
	
	@param	inTarget		Target on which to install handler
	@param	inNumTypes		Number of event types
	@param	inTypeList		Array of event types
	
	@return	EventHandlerRef for the installed event handler					*/

EventHandlerRef
EventDoer::Install(
	EventTargetRef			inTarget,
	UInt32					inNumTypes,
	const EventTypeSpec*	inTypeList)
{
		// We pass the "this" pointer as the user data for the handler
		
	SysEventHandler		handler;
	handler.Install(inTarget, GetSysEventHandlerUPP(), this,
						inNumTypes, inTypeList);
						
	return handler.Detach();
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Installs handler for a single event
	
	@param	inTarget		Target on which to install handler
	@param	inEventClass	Carbon Event class
	@param	inEventKind		Carbon Event kind
	
	@return	EventHandlerRef for the installed event handler					*/

EventHandlerRef
EventDoer::Install(
	EventTargetRef		inTarget,
	UInt32				inEventClass,
	UInt32				inEventKind)
{
	EventTypeSpec	typeSpec = { inEventClass, inEventKind };
	
	return Install(inTarget, 1, &typeSpec);
}


// ---------------------------------------------------------------------------
//	Invoke															  [public]
/**
	Calls function to handle an event
	
	@param	ioEvent		CarbonEvent being handled
	
	@return	OS error code													 */

OSStatus
EventDoer::Invoke(
	SysCarbonEvent&	ioEvent)
{
	return DoEvent(ioEvent);
}


} // namespace PPx