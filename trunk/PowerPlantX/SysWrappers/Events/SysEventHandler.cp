// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysEventHandler.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:34 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysEventHandler.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysEventHandler													  [public]
/**
	Default constructor */

SysEventHandler::SysEventHandler()
{
	mHandlerRef = nil;
}


// ---------------------------------------------------------------------------
//	~SysEventHandler												  [public]
/**
	Destructor */

SysEventHandler::~SysEventHandler()
{
	Remove();
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Install event handler for a list of event types
	
	@param	inTarget		Target on which to install handler
	@param	inCallback		Callback function UPP for handling the event
	@param	inUserData		User data passed to callback function
	@param	inNumTypes		Number of event types
	@param	inTypeList		Array of event type specifiers
	
	Removes existing handler if already installed							*/

void
SysEventHandler::Install(
	EventTargetRef			inTarget,
	EventHandlerUPP			inCallback,
	void*					inUserData,
	UInt32					inNumTypes,
	const EventTypeSpec*	inTypeList)
{
	Remove();						// Remove if already installed
	
	OSStatus		status = ::InstallEventHandler(inTarget, inCallback,
									inNumTypes, inTypeList, inUserData,
									&mHandlerRef);
								 
	PPx_ThrowIfOSError_(status, "InstallEventHandler failed");
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Install event handler for one event type
	
	@param	inTarget		Target on which to install handler
	@param	inCallback		Callback function UPP for handling the event
	@param	inUserData		User data passed to callback function
	@param	inEventClass	Class of CarbonEvent to handle
	@param	inEventKind		Kind of CarbonEvent to handle
	
	Removes existing handler if already installed							*/

void
SysEventHandler::Install(
	EventTargetRef			inTarget,
	EventHandlerUPP			inCallback,
	void*					inUserData,
	UInt32					inEventClass,
	UInt32					inEventKind)
{
	EventTypeSpec	typeSpec = { inEventClass, inEventKind };
	
	Install(inTarget, inCallback, inUserData, 1, &typeSpec);
}


// ---------------------------------------------------------------------------
//	Remove															  [public]
/**
	Removes the event handler from the system registry						*/

void
SysEventHandler::Remove()
{
	if (mHandlerRef != nil) {
		::RemoveEventHandler(mHandlerRef);
		mHandlerRef = nil;
	}
}


// ---------------------------------------------------------------------------
//	IsInstalled														  [public]
/**
	Returns whether the handler is installed
	
	@return Whether the handler is installed								*/

bool
SysEventHandler::IsInstalled() const
{
	return (mHandlerRef != nil);
}


// ---------------------------------------------------------------------------
//	Adopt															  [public]
/**
	Takes ownership of an existing EventHandlerRef
	
	@param	inHandlerRef	The EventHandlerRef to adopt					*/

void
SysEventHandler::Adopt(
	EventHandlerRef		inHandlerRef)
{
	Remove();
	mHandlerRef = inHandlerRef;
}


// ---------------------------------------------------------------------------
//	Detach															  [public]
/**
	Relinquishes ownership of its EventHanderRef
	
	@return	Formerly owned EventHandlerRef. Caller now owns it.				*/

EventHandlerRef
SysEventHandler::Detach()
{
	EventHandlerRef	handlerRef = mHandlerRef;
	
	mHandlerRef = nil;
	
	return handlerRef;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysEventHandlerUPP					Constructor				  [public]

SysEventHandlerUPP::SysEventHandlerUPP(
	EventHandlerProcPtr	inCallbackFunc)
{
	mEventHandlerUPP = ::NewEventHandlerUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysEventHandlerUPP											  [public]

SysEventHandlerUPP::~SysEventHandlerUPP()
{
	::DisposeEventHandlerUPP(mEventHandlerUPP);
}

} // namespace PPx