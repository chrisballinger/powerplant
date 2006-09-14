// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxViewEvents.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:16 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
//
//	Handlers for CarbonEvents of class kEventClassControl
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

#include <PPxViewEvents.h>
#include <PPxQuickdrawUtils.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	ControlDisposeDoer

OSStatus
ControlDisposeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoControlDispose(ioEvent, control);
}


// ===========================================================================
//	ControlHitDoer

OSStatus
ControlHitDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	ControlPartCodeType	partCode;
	UInt32				keyModifiers;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlPart, partCode);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
						  
	return DoControlHit(ioEvent, control, partCode, keyModifiers);
}


// ===========================================================================
//	ControlSimulateHitDoer

OSStatus
ControlSimulateHitDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	UInt32				keyModifiers;
	ControlPartCodeType	partCode;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::Get(ioEvent, kEventParamControlPart, partCode);
	
	OSStatus	status = DoControlSimulateHit(ioEvent, control,
												keyModifiers, partCode);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlPart, partCode);
	}
	
	return status;
}


// ===========================================================================
//	ControlHitTestDoer

OSStatus
ControlHitTestDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef	control;
	HIPoint		hitPoint;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, hitPoint);
	
	ControlPartCodeType	partCode(kControlEntireControl);
						  
	OSStatus	status = DoControlHitTest(ioEvent, control, hitPoint, partCode);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlPart, partCode);
	}
	
	return status;
}


// ===========================================================================
//	ControlDrawDoer

OSStatus
ControlDrawDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	ControlPartCodeType	partCode = kControlEntireControl;
	RgnHandle			clipRgn  = nil;
	GrafPtr				port	 = nil;
	CGContextRef		context  = nil;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	
	SysEventParam::GetOptional(ioEvent, kEventParamControlPart, partCode);
	SysEventParam::GetOptional(ioEvent, kEventParamRgnHandle, clipRgn);
	SysEventParam::GetOptional(ioEvent, kEventParamGrafPort, port);
	SysEventParam::GetOptional(ioEvent, kEventParamCGContextRef, context);
										
	GrafPortSaver	savePort(port);
						  
	return DoControlDraw(ioEvent, control, partCode, clipRgn, context);
}


// ===========================================================================
//	ControlApplyBackgroundDoer

OSStatus
ControlApplyBackgroundDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	ControlRef		subControl;
	SInt16			drawDepth;
	Boolean			drawInColor;
	GrafPtr			port	 = nil;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlSubControl, subControl);
	SysEventParam::Get(ioEvent, kEventParamControlDrawDepth, drawDepth);
	SysEventParam::Get(ioEvent, kEventParamControlDrawInColor, drawInColor);
	
	SysEventParam::GetOptional(ioEvent, kEventParamGrafPort, port);
										
	GrafPortSaver	savePort(port);
						  
	return DoControlApplyBackground(ioEvent, control, subControl, drawDepth,
									drawInColor);
}


// ===========================================================================
//	ControlApplyTextColorDoer

OSStatus
ControlApplyTextColorDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	ControlRef		subControl;
	SInt16			drawDepth;
	Boolean			drawInColor;
	GrafPtr			port	 = nil;
	CGContextRef	context  = nil;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlSubControl, subControl);
	SysEventParam::Get(ioEvent, kEventParamControlDrawDepth, drawDepth);
	SysEventParam::Get(ioEvent, kEventParamControlDrawInColor, drawInColor);
	
	SysEventParam::GetOptional(ioEvent, kEventParamCGContextRef, context);
	SysEventParam::GetOptional(ioEvent, kEventParamGrafPort, port);
										
	GrafPortSaver	savePort(port);
						  
	return DoControlApplyTextColor(ioEvent, control, subControl, drawDepth,
									drawInColor, context);
}


// ===========================================================================
//	ControlSetFocusPartDoer

OSStatus
ControlSetFocusPartDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	ControlPartCodeType	focusPart;
	Boolean				focusEverything = false;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlPart, focusPart);
	
	SysEventParam::GetOptional(ioEvent, kEventParamControlFocusEverything, focusEverything);

	OSStatus	status = DoControlSetFocusPart(ioEvent, control,
												focusEverything, focusPart);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlPart, focusPart);
	}

	return status;
}


// ===========================================================================
//	ControlGetFocusPartDoer

OSStatus
ControlGetFocusPartDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	ControlPartCodeType	focusPart;
	
	OSStatus	status = DoControlGetFocusPart(ioEvent, control, focusPart);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlPart, focusPart);
	}

	return status;
}


// ===========================================================================
//	ControlActivateDoer

OSStatus
ControlActivateDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoControlActivate(ioEvent, control);
}


// ===========================================================================
//	ControlDeactivateDoer

OSStatus
ControlDeactivateDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoControlDeactivate(ioEvent, control);
}


// ===========================================================================
//	ControlSetCursorDoer

OSStatus
ControlSetCursorDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	HIPoint			mouseLocation;
	UInt32			keyModifiers;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLocation);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
						  
	return DoControlSetCursor(ioEvent, control, mouseLocation, keyModifiers);
}


// ===========================================================================
//	ControlClickDoer

OSStatus
ControlClickDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	HIPoint			mouseLocation;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLocation);
						  
	return DoControlClick(ioEvent, control, mouseLocation);
}


// ===========================================================================
//	ControlTrackDoer

OSStatus
ControlTrackDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	HIPoint				mouseLocation;
	UInt32				keyModifiers;
	ControlActionUPP	actionUPP;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLocation);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::Get(ioEvent, kEventParamControlAction, actionUPP);
	
	ControlPartCodeType		partCode;
						  
	OSStatus	status = DoControlTrack(ioEvent, control, mouseLocation,
										keyModifiers, actionUPP, partCode);
										
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlPart, partCode);
	}
	
	return status;
}


// ===========================================================================
//	ControlDragEnterDoer

OSStatus
ControlDragEnterDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	DragRef			drag;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamDragRef, drag);
						  
	return DoControlDragEnter(ioEvent, control, drag);
}


// ===========================================================================
//	ControlDragWithinDoer

OSStatus
ControlDragWithinDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	DragRef			drag;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamDragRef, drag);
						  
	return DoControlDragWithin(ioEvent, control, drag);
}


// ===========================================================================
//	ControlDragLeaveDoer

OSStatus
ControlDragLeaveDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	DragRef			drag;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamDragRef, drag);
						  
	return DoControlDragLeave(ioEvent, control, drag);
}


// ===========================================================================
//	ControlDragReceiveDoer

OSStatus
ControlDragReceiveDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	DragRef			drag;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamDragRef, drag);
						  
	return DoControlDragReceive(ioEvent, control, drag);
}


// ===========================================================================
//	ControlGetPartRegionDoer

OSStatus
ControlGetPartRegionDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	ControlPartCodeType	partCode;
	RgnHandle			region;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlPart, partCode);
	SysEventParam::Get(ioEvent, kEventParamControlRegion, region);
						  
	return DoControlGetPartRegion(ioEvent, control, partCode, region);
}


// ===========================================================================
//	ControlGetPartBoundsDoer

OSStatus
ControlGetPartBoundsDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef			control;
	ControlPartCodeType	partCode;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlPart, partCode);
	
	HIRect			bounds;
						  
	OSStatus	status = DoControlGetPartBounds(ioEvent, control,
												partCode, bounds);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlPartBounds, bounds);
	}
	
	return status;
}


// ===========================================================================
//	ControlGetOptimalBoundsDoer

OSStatus
ControlGetOptimalBoundsDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	
	HIRect			bounds;
	SInt16			baseline = 0;
						  
	OSStatus	status = DoControlGetOptimalBounds(ioEvent, control,
													bounds, baseline);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlOptimalBounds, bounds);
		
			// Text baseline output parameter is optional. Since it
			// is an offset from the top of the bounds, only positive
			// values are reasonable.
		
		if (baseline > 0) {
			SysEventParam::Set( ioEvent,
								kEventParamControlOptimalBaselineOffset,
								baseline );
		}
	}
	
	return status;
}


// ===========================================================================
//	ControlGetSizeConstraintsDoer

OSStatus
ControlGetSizeConstraintsDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	
	HISize			minSize;
	HISize			maxSize;
						  
	OSStatus	status = DoControlGetSizeConstraints(ioEvent, control,
														minSize, maxSize);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamMinimumSize, minSize);
		SysEventParam::Set(ioEvent, kEventParamMaximumSize, maxSize);
	}
	
	return status;
}


// ===========================================================================
//	ControlValueFieldChangedDoer

OSStatus
ControlValueFieldChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoControlValueFieldChanged(ioEvent, control);
}


// ===========================================================================
//	ControlHiliteChangedDoer

OSStatus
ControlHiliteChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoControlHiliteChanged(ioEvent, control);
}


// ===========================================================================
//	ControlBoundsChangedDoer

OSStatus
ControlBoundsChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
		// NOTE: CarbonEvents.h (Mac OS X 10.2.3) lists a previous bounds
		// parameter. Apple has confirmed that this always has the same
		// value as the original bounds.

	ControlRef		control;
	UInt32			attributes;
	HIRect			originalBounds;
	HIRect			currentBounds;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamAttributes, attributes);
	SysEventParam::Get(ioEvent, kEventParamOriginalBounds, originalBounds);
	SysEventParam::Get(ioEvent, kEventParamCurrentBounds, currentBounds);
	
	return DoControlBoundsChanged(ioEvent, control, attributes,
									originalBounds, currentBounds);
}


// ===========================================================================
//	ControlTitleChangedDoer
// ===========================================================================

OSStatus
ControlTitleChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoControlTitleChanged(ioEvent, control);
}


// ===========================================================================
//	ControlEnabledStateChangedDoer

OSStatus
ControlEnabledStateChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoControlEnabledStateChanged(ioEvent, control);
}


// ===========================================================================
//	ControlOwningWindowChangedDoer

OSStatus
ControlOwningWindowChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	UInt32			attributes;
	WindowRef		formerWindow;
	WindowRef		currentWindow;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamAttributes, attributes);
	SysEventParam::Get(ioEvent, kEventParamControlOriginalOwningWindow, formerWindow);
	SysEventParam::Get(ioEvent, kEventParamControlCurrentOwningWindow, currentWindow);

	return DoControlOwningWindowChanged(ioEvent, control, attributes,
										formerWindow, currentWindow);
}


// ===========================================================================
//	ControlAddedSubControlDoer

OSStatus
ControlAddedSubControlDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	ControlRef		subControl;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlSubControl, subControl);

	return DoControlAddedSubControl(ioEvent, control, subControl);
}


// ===========================================================================
//	ControlRemovingSubControlDoer

OSStatus
ControlRemovingSubControlDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	ControlRef		subControl;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlSubControl, subControl);

	return DoControlRemovingSubControl(ioEvent, control, subControl);
}


// ===========================================================================
//	ControlArbitraryMessageDoer

OSStatus
ControlArbitraryMessageDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	SInt16			message;
	SInt32			parameter;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);
	SysEventParam::Get(ioEvent, kEventParamControlMessage, message);
	SysEventParam::Get(ioEvent, kEventParamControlParam, parameter);
	
	SInt32			result;

	OSStatus	status = DoControlArbitraryMessage(ioEvent, control, message,
													parameter, result);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamControlResult, result);
	}
	
	return status;
}


} // namespace PPx