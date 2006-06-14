// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCarbonEvent.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCarbonEvent.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysCarbonEvent													  [public]
/**
	Default constructor */

SysCarbonEvent::SysCarbonEvent()
{
	mSysEvent	= nil;
	mCallRef	= nil;
}


// ---------------------------------------------------------------------------
//	SysCarbonEvent													  [public]
/**
	Constructs from an EventRef and EventHandlerCallRef
	
	@param	inEventRef		System EventRef
	@param	inCallRef		System EventHandlerCallRef
	
	This constructor is designed for wrapping EventRefs that you
	receive from the system.												*/

SysCarbonEvent::SysCarbonEvent(
	EventRef			inEventRef,
	EventHandlerCallRef	inCallRef)
{
	mSysEvent	= inEventRef;
	mCallRef	= inCallRef;
	
	::RetainEvent(inEventRef);
}


// ---------------------------------------------------------------------------
//	SysCarbonEvent													  [public]
/**
	Constructs from event creation parameters
	
	@param	inEventClass	CarbonEvent class
	@param	inEventKind		CarbonEvent kind
	@param	inAttrs			Event attributes
	@param	inWhen			Time of event
	@param	inAllocator		CF Allocator									*/

SysCarbonEvent::SysCarbonEvent(
	EventClassT		inEventClass,
	EventKindT		inEventKind,
	EventAttributes	inAttrs,
	EventTime		inWhen,
	CFAllocatorRef	inAllocator)
{
	mSysEvent	= nil;
	mCallRef	= nil;
	
	MakeEvent(inEventClass, inEventKind, inAttrs, inWhen, inAllocator);
}


// ---------------------------------------------------------------------------
//	SysCarbonEvent													  [public]
/**
	Copy constructor */

SysCarbonEvent::SysCarbonEvent(
	const SysCarbonEvent&	inOriginal)
{
	mSysEvent	= ::CopyEvent(inOriginal.mSysEvent);
	mCallRef	= inOriginal.mCallRef;
}


// ---------------------------------------------------------------------------
//	SysCarbonEvent													  [public]
/**
	Assignment operator */

SysCarbonEvent&
SysCarbonEvent::operator = (
	const SysCarbonEvent&	inOriginal)
{
	if (&inOriginal == this) return *this;

	if (mSysEvent != nil) {
		::ReleaseEvent(mSysEvent);
	}
	
	mSysEvent	= ::CopyEvent(inOriginal.mSysEvent);
	mCallRef	= inOriginal.mCallRef;
	
	return *this;
}


// ---------------------------------------------------------------------------
//	~SysCarbonEvent													  [public]
/**
	Destructor */

SysCarbonEvent::~SysCarbonEvent()
{
	if (mSysEvent != nil) {
		::ReleaseEvent(mSysEvent);
	}
}


// ---------------------------------------------------------------------------
//	Adopt															  [public]
/**
	Adopts existing EventRef and EventHnadlerCallRef
	
	@param	inEventRef	System EventRef
	@param	inCallRef	System EventHandlerCallRef
	
	Releases its existing event and retains the input one					*/

void
SysCarbonEvent::Adopt(
	EventRef			inEventRef,
	EventHandlerCallRef	inCallRef)
{
	if (mSysEvent != nil) {
		::ReleaseEvent(mSysEvent);
	}
	
	mSysEvent	= inEventRef;
	mCallRef	= inCallRef;
	
	if (inEventRef != nil) {
		::RetainEvent(inEventRef);
	}
}


// ---------------------------------------------------------------------------
//	MakeEvent														  [public]
/**
	Makes a CarbonEvent from the input creation parametrs
	
	@param	inEventClass	CarbonEvent class
	@param	inEventKind		CarbonEvent kind
	@param	inAttrs			Event attributes
	@param	inWhen			Time of event
	@param	inAllocator		CF Allocator
	
	Releases its existing event and uses the new one created from the
	input parameters															*/

void
SysCarbonEvent::MakeEvent(
	EventClassT		inEventClass,
	EventKindT		inEventKind,
	EventAttributes	inAttrs,
	EventTime		inWhen,
	CFAllocatorRef	inAllocator)
{
	EventRef	newEvent;			// Make new CarbonEvent

	OSStatus	status = ::MacCreateEvent(
									inAllocator, inEventClass, inEventKind,
									inWhen, inAttrs, &newEvent );
										   
	PPx_ThrowIfOSError_(status, "MacCreateEvent failed");


	if (mSysEvent != nil) {			// Release existing event
		::ReleaseEvent(mSysEvent);
	}
	
	mSysEvent = newEvent;			// Store new event
	mCallRef  = nil;
}


// ---------------------------------------------------------------------------
//	GetEventClass													  [public]
/**
	Returns the class of the CarbonEvent
	
	@return Class of the CarbonEvent										*/

EventClassT
SysCarbonEvent::GetEventClass() const
{
	return ::GetEventClass(mSysEvent);
}


// ---------------------------------------------------------------------------
//	GetEventKind													  [public]
/**
	Returns the kind of the CarbonEvent
	
	@return Kind of the CarbonEvent											*/

EventKindT
SysCarbonEvent::GetEventKind() const
{
	return ::GetEventKind(mSysEvent);
}


// ---------------------------------------------------------------------------
//	GetTime															  [public]
/**
	Returns the time of the CarbonEvent
	
	@return Time of the CarbonEvent											*/

EventTime
SysCarbonEvent::GetTime() const
{
	return ::GetEventTime(mSysEvent);
}


// ---------------------------------------------------------------------------
//	SetTime															  [public]
/**
	Sets the time of the CarbonEvent
	
	@param	inTime		Time of the CarbonEvent								*/

void
SysCarbonEvent::SetTime(
	EventTime	inTime)
{
	::SetEventTime(mSysEvent, inTime);
}


// ---------------------------------------------------------------------------
//	GetParameter													  [public]
/**
	Gets a named parameter from the CarbonEvent
	
	@param	inName			Parameter name ID
	@param	inDesiredType	Desired type for the parameter
	@param	outActualType	Actual type of parameter retrieved
	@param	inBufferSize	Size of buffer
	@param	outActualSize	Actual size of data retrieved
	@param	outData			Pointer to buffer for storing parameter value
	
	@return OS error code													*/

OSStatus
SysCarbonEvent::GetParameter(
	EventParamName		inName,
	EventParamType		inDesiredType,
	EventParamType*		outActualType,
	UInt32				inBufferSize,
	UInt32*				outActualSize,
	void*				outData) const
{
	return ::GetEventParameter( mSysEvent, inName, inDesiredType,
								outActualType, inBufferSize, outActualSize,
								outData );
}


// ---------------------------------------------------------------------------
//	SetParameter													  [public]
/**
	Sets a named parameter in the CarbonEvent
	
	@param	inName			Parameter name ID
	@param	inType			Type of the parameter
	@param	inSize			Size of the parameter data
	@param	inData			Buffer containing parameter data
	
	Throws an exception if there's an error setting the parameter			*/

void
SysCarbonEvent::SetParameter(
	EventParamName		inName,
	EventParamType		inType,
	UInt32				inSize,
	const void*			inData)
{
	OSStatus	status = ::SetEventParameter( mSysEvent, inName, inType,
											  inSize, inData );
	
	PPx_ThrowIfOSError_(status, "SetEventParameter failed");
}


// ---------------------------------------------------------------------------
//	PostTo															  [public]
/**
	Post CarbonEvent for a target on an event queue
	
	@param	inTargetRef		Target for event. May be nil.
	@param	inQueueRef		Event queue to which to post the event. May be nil.
	@param	inPriority		Event priority
	
	If the target is nil, system dispatches the event in the standard
	manner (sending to the user focus target).
	
	If the event queue is nil, we post the event to the current queue.
	
	@note Posting an event is asynchronous. Function returns immediately
	after posting the event, which is handled at a later time.				*/

void
SysCarbonEvent::PostTo(
	EventTargetRef		inTargetRef,
	EventQueueRef		inQueueRef,
	EventPriority		inPriority)
{
	OSStatus	status;
	
	if (inTargetRef != nil) {
		status = ::SetEventParameter(
							mSysEvent, kEventParamPostTarget,
							typeEventTargetRef, sizeof(EventTargetRef),
							&inTargetRef);
							
		PPx_ThrowIfOSError_(status, "SetEventParameter failed for typeEventTargetRef");
	}
	
	if (inQueueRef == nil) {
		inQueueRef = ::GetCurrentEventQueue();
	}

	::SetEventTime(mSysEvent, ::GetCurrentEventTime());

	status = ::PostEventToQueue(inQueueRef, mSysEvent, inPriority);
	
	PPx_ThrowIfOSError_(status, "PostEventToQueue failed");
}


// ---------------------------------------------------------------------------
//	SendTo															  [public]
/**
	Send CarbonEvent to a target
	
	@param	inTargetRef		Target for event
	@param	inOptions		Options for sending the event
	
	See <CarbonEventsCore.h> for the list of options. At present, there
	are two options: kEventTargetDontPropagate means to send the event
	to the target only and do not propagate if the event isn't handled.
	CallNextEventHandler does nothing.
	
	kEventTargetSendToAllHandlers means to send the event to all installed
	handlers, not stopping when a handler reports that it has handled the
	event. This is a broadcast or notification style of event.
	
	Normal event dispatching sends the event to the target. If the
	target returns eventNotHandledErr, propagate the event to the next
	handler, and so on. Stop when a handler returns a result other than
	eventNotHandlerErr.														*/

OSStatus
SysCarbonEvent::SendTo(
	EventTargetRef	inTargetRef,
	OptionBits		inOptions)
{
	::SetEventTime(mSysEvent, ::GetCurrentEventTime());
	
	return ::SendEventToEventTargetWithOptions(
									mSysEvent, inTargetRef, inOptions);
}


// ---------------------------------------------------------------------------
//	CallNextHandler													  [public]
/**
	Sends the CarbonEvent to the next event handler
	
	@return	OS error code													*/

OSStatus
SysCarbonEvent::CallNextHandler()
{
	OSStatus	status = eventNotHandledErr;
	
	if (mCallRef != nil) {
		status = ::CallNextEventHandler(mCallRef, mSysEvent);
	}
	
	return status;
}

} // namespace PPx