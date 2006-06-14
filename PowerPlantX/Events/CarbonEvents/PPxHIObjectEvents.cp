// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxHIObjectEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
//
//	Handlers for CarbonEvents of class kEventClassHIObject
//
//	Each handler class implements two functions: Install and DoEvent.
//
//	Install calls the inherited EventDoer::Install function, passing the
//	appropriate event class and kind for the event.
//
//	DoEvent extracts the required and optional parameters from the event,
//	calls the "Do" function, then stores any output parameters back into
//	the event. Subclasses must implement the "Do" function, which is declared
//	as pure virtual.

#include <PPxHIObjectEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	HIObjectConstructDoer

OSStatus
HIObjectConstructDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIObjectRef		objectRef;
	
	SysEventParam::Get(ioEvent, kEventParamHIObjectInstance, objectRef);

	return DoHIObjectConstruct(ioEvent, objectRef);
}


// ===========================================================================
//	HIObjectInitializeDoer

OSStatus
HIObjectInitializeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoHIObjectInitialize(ioEvent);
}


// ===========================================================================
//	HIObjectDestructDoer

OSStatus
HIObjectDestructDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoHIObjectDestruct(ioEvent);
}


// ===========================================================================
//	HIObjectIsEqualDoer

OSStatus
HIObjectIsEqualDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIObjectRef		objectRef;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, objectRef);

	return DoHIObjectIsEqual(ioEvent, objectRef);
}


// ===========================================================================
//	HIObjectPrintDebugInfoDoer

OSStatus
HIObjectPrintDebugInfoDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoHIObjectPrintDebugInfo(ioEvent);
}


} // namespace PPx
