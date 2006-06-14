// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventDoer.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxEventDoer.h
	@brief		Classes for handling Carbon Events
*/

#ifndef H_PPxEventDoer
#define H_PPxEventDoer
#pragma once

#include <SysCarbonEvent.h>
#include <SysEventHandler.h>

namespace PPx {

// ===========================================================================
/**	Abstract class for a Carbon Event handler */

class	EventDoer {
public:
	virtual				~EventDoer();
								
	EventHandlerRef		Install(
								EventTargetRef			inTarget,
								UInt32					inNumTypes,
								const EventTypeSpec*	inTypeList);
								
	EventHandlerRef		Install(
								EventTargetRef			inTarget,
								UInt32					inEventClass,
								UInt32					inEventKind);

	OSStatus			Invoke( SysCarbonEvent& ioEvent );
	
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent ) = 0;
};


// ===========================================================================
/**	Template class for a Carbon Event handler that responds to one
	specific type of event.
	
	The template parameters specify the class and kind of the event.		*/

template <EventClassT TEventClass, EventKindT TEventKind>
class	SpecificEventDoer : public EventDoer {
public:

	EventHandlerRef		Install( EventTargetRef inTarget )
						{
							return EventDoer::Install( inTarget,
													   TEventClass,
													   TEventKind );
						}
};


// ===========================================================================
/**	Template class for an EventDoer that calls a member function of an
	object																	*/
	
template <class T>
class	EventDoerCallback : public EventDoer {
public:

typedef	OSStatus	(T:: * CallbackFunction)(SysCarbonEvent&);


// ---------------------------------------------------------------------------

EventDoerCallback()
{
	mObject		= nil;
	mFunction	= nil;
}

// ---------------------------------------------------------------------------

EventHandlerRef
Install(
	T*						inObject,
	CallbackFunction		inFunction,
	EventTargetRef			inTarget,
	UInt32					inNumTypes,
	const EventTypeSpec*	inTypeList)
{
	mObject		= inObject;
	mFunction	= inFunction;
	
	return EventDoer::Install(inTarget, inNumTypes, inTypeList);
}

// ---------------------------------------------------------------------------

EventHandlerRef
Install(
	T*					inObject,
	CallbackFunction	inFunction,
	EventTargetRef		inTarget,
	UInt32				inEventClass,
	UInt32				inEventKind)
{
	mObject		= inObject;
	mFunction	= inFunction;
	
	return EventDoer::Install(inTarget, inEventClass, inEventKind);
}

// ---------------------------------------------------------------------------

void
SetCallback(
	T*					inObject,
	CallbackFunction	inFunction)
{
	mObject		= inObject;
	mFunction	= inFunction;
}
	
// ---------------------------------------------------------------------------

virtual
OSStatus
DoEvent(
	SysCarbonEvent&	ioEvent)
{
	OSStatus	status = eventNotHandledErr;
	
	if ( (mObject != nil)  &&  (mFunction != nil) ) {
		status = (mObject->*mFunction)(ioEvent);
	}
	
	return status;
}


// ---------------------------------------------------------------------------

private:
	T*					mObject;
	CallbackFunction	mFunction;
};


} // namespace PPx

#endif	// H_PPxEventDoer
