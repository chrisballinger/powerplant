// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCarbonEvent.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCarbonEvent.h
	@brief		Classes for managing Carbon Events
*/

#ifndef H_SysCarbonEvent
#define H_SysCarbonEvent
#pragma once

#include <PPxPrefix.h>
#include <PPxConstants.h>
#include <Carbon/Carbon.h>

namespace PPx {

// ---------------------------------------------------------------------------
/**	Indicates "now" as the time stamp for a Carbon Event */

const EventTime		eventTime_Now = EventTime(0);


// ===========================================================================
//	SysEventSpec
/**
	Struct describing the type of a Carbon Event. The system identifies
	a Carbon Event by its class and kind, both 4-byte values. This struct
	groups the two identifiers so that we can initialize and compare them
	as a single unit.														*/

struct	SysEventSpec {
	EventClassT		eventClass;
	EventKindT		eventKind;
	
	SysEventSpec();
	
	SysEventSpec(
			EventClassT		inClass,
			EventKindT		inKind);
};


// ---------------------------------------------------------------------------

inline
SysEventSpec::SysEventSpec()

	: eventClass(0),
	  eventKind(0)
{
}

// ---------------------------------------------------------------------------

inline
SysEventSpec::SysEventSpec(
	EventClassT		inClass,
	EventKindT		inKind)

	: eventClass(inClass),
	  eventKind(inKind)
{
}

// ---------------------------------------------------------------------------

inline bool operator == (
	const SysEventSpec&	inLeft,
	const SysEventSpec&	inRight)
{
	return ( (inLeft.eventClass == inRight.eventClass)  &&
			 (inLeft.eventKind == inRight.eventKind) );
}


// ===========================================================================
//	SysCarbonEvent
/**
	Wrapper class for a Carbon Event */

class SysCarbonEvent {
public:
						SysCarbonEvent();
						
						SysCarbonEvent(
								EventRef			inEventRef,
								EventHandlerCallRef	inCallRef = nil );

						SysCarbonEvent(
								EventClassT		inEventClass,
								EventKindT		inEventKind,
								EventAttributes	inAttrs = kEventAttributeNone,
								EventTime		inWhen = eventTime_Now,
								CFAllocatorRef	inAllocator = nil);
								
						SysCarbonEvent( const SysCarbonEvent& inOriginal );
						
	SysCarbonEvent&		operator = ( const SysCarbonEvent& inOriginal );
						
						~SysCarbonEvent();
		
	void				Adopt(	EventRef			inEventRef,
								EventHandlerCallRef	inCallRef = nil );
	
	void				MakeEvent(
								EventClassT		inEventClass,
								EventKindT		inEventKind,
								EventAttributes	inAttrs = kEventAttributeNone,
								EventTime		inWhen = eventTime_Now,
								CFAllocatorRef	inAllocator = nil);
								
	operator			EventRef() const;
								
	EventRef			GetEventRef() const;
	
	EventClassT			GetEventClass() const;
	
	EventKindT			GetEventKind() const;
	
	EventTime			GetTime() const;
	
	void				SetTime( EventTime inTime );
	
	OSStatus			GetParameter(
								EventParamName		inName,
								EventParamType		inDesiredType,
								EventParamType*		outActualType,
								UInt32				inBufferSize,
								UInt32*				outActualSize,
								void*				outData) const;
							
	void				SetParameter(
								EventParamName		inName,
								EventParamType		inType,
								UInt32				inSize,
								const void*			inData);
								
	void				PostTo(	EventTargetRef		inTargetRef,
								EventQueueRef		inQueueRef = nil,
								EventPriority		inPriority = kEventPriorityStandard);
								
	OSStatus			SendTo(	EventTargetRef		inTargetRef,
								OptionBits			inOptions = options_None);
								
	OSStatus			CallNextHandler();
		
private:
	EventRef			mSysEvent;		/**< System event reference */
	EventHandlerCallRef	mCallRef;		/**< System callback reference */
};


// ---------------------------------------------------------------------------

inline
SysCarbonEvent::operator EventRef() const
{
	return mSysEvent;
}


inline
EventRef
SysCarbonEvent::GetEventRef() const
{
	return mSysEvent;
}

} // namespace PPx

#endif	// H_SysCarbonEvent
