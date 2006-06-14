// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMouseEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxMouseEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	MouseDownDoer

OSStatus
MouseDownDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIPoint		mouseLoc;
	
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLoc);
										
	return DoMouseDown(ioEvent, mouseLoc);
}


// ===========================================================================
//	MouseDownDoer

OSStatus
MouseUpDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIPoint		mouseLoc;
	
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLoc);
										
	return DoMouseUp(ioEvent, mouseLoc);
}


// ===========================================================================
//	MouseMovedDoer

OSStatus
MouseMovedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIPoint		mouseLoc;
	
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLoc);
										
	return DoMouseMoved(ioEvent, mouseLoc);
}


// ===========================================================================
//	MouseDraggedDoer

OSStatus
MouseDraggedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIPoint		mouseLoc;
	
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLoc);
										
	return DoMouseDragged(ioEvent, mouseLoc);
}


// ===========================================================================
//	MouseEnteredDoer

OSStatus
MouseEnteredDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MouseTrackingRef	trackingRef;
	WindowRef			window;
	HIPoint				mouseLoc;
	HIPoint				windowMouseLoc;
	UInt32				keyModifiers;
	
	SysEventParam::Get(ioEvent, kEventParamMouseTrackingRef, trackingRef);
	SysEventParam::Get(ioEvent, kEventParamWindowRef, window);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLoc);
	SysEventParam::Get(ioEvent, kEventParamWindowMouseLocation, windowMouseLoc);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
										
	return DoMouseEntered(ioEvent, trackingRef, window, mouseLoc,
							windowMouseLoc, keyModifiers);
}


// ===========================================================================
//	MouseExitedDoer

OSStatus
MouseExitedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MouseTrackingRef	trackingRef;
	WindowRef			window;
	HIPoint				mouseLoc;
	HIPoint				windowMouseLoc;
	UInt32				keyModifiers;
	
	SysEventParam::Get(ioEvent, kEventParamMouseTrackingRef, trackingRef);
	SysEventParam::Get(ioEvent, kEventParamWindowRef, window);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLoc);
	SysEventParam::Get(ioEvent, kEventParamWindowMouseLocation, windowMouseLoc);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
										
	return DoMouseExited(ioEvent, trackingRef, window, mouseLoc,
							windowMouseLoc, keyModifiers);
}


// ===========================================================================
//	MouseWheelMovedDoer

OSStatus
MouseWheelMovedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef				window;
	HIPoint					mouseLoc;
	HIPoint					windowMouseLoc;
	UInt32					keyModifiers;
	EventMouseWheelAxisType	wheelAxis;
	SInt32					wheelDelta;
	
	SysEventParam::Get(ioEvent, kEventParamWindowRef, window);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLoc);
	SysEventParam::Get(ioEvent, kEventParamWindowMouseLocation, windowMouseLoc);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::Get(ioEvent, kEventParamMouseWheelAxis, wheelAxis);
	SysEventParam::Get(ioEvent, kEventParamMouseWheelDelta, wheelDelta);
										
	return DoMouseWheelMoved(ioEvent, window, mouseLoc, windowMouseLoc,
								keyModifiers, wheelAxis, wheelDelta);
}


} // namespace PPx