// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:17 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxWindowEvents.h>
#include <PPxEnvironment.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	WindowUpdateDoer

OSStatus
WindowUpdateDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowUpdate(ioEvent, window);
}


// ===========================================================================
//	WindowDrawContentDoer

OSStatus
WindowDrawContentDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDrawContent(ioEvent, window);
}


// ===========================================================================
//	WindowActivatedDoer

OSStatus
WindowActivatedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowActivated(ioEvent, window);
}


// ===========================================================================
//	WindowDeactivatedDoer

OSStatus
WindowDeactivatedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDeactivated(ioEvent, window);
}


// ===========================================================================
//	WindowGetClickActivationDoer

OSStatus
WindowGetClickActivationDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef			window;
	HIPoint				mouseLocation;
	UInt32				keyModifiers;
	WindowPartCode		windowPart;
	ControlRef			controlHit;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLocation);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)
	// Use kEventParamWindowPartCode for Mac OS X 10.3 and later
	if (Environment::GetOSVersion() >= 0x1030)
	{

		SysEventParam::Get(ioEvent, kEventParamWindowPartCode,
			typeWindowPartCode, windowPart);

	}
	else
#endif // Mac OS 10.3
	{

		WindowDefPartCode	windowDefPart;
		
		SysEventParam::Get(ioEvent, kEventParamWindowDefPart,
			typeWindowDefPartCode, windowDefPart);
		
		windowPart = windowDefPart;

	}

	SysEventParam::Get(ioEvent, kEventParamControlRef, controlHit);
	
	ClickActivationResult	result = kActivateAndIgnoreClick;
	
	OSStatus	status = DoWindowGetClickActivation(
								ioEvent, window, mouseLocation, keyModifiers,
								windowPart, controlHit, result );
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamClickActivation, result);
	}
	
	return status;
}


// ===========================================================================
//	WindowShowingDoer

OSStatus
WindowShowingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowShowing(ioEvent, window);
}


// ===========================================================================
//	WindowHidingDoer

OSStatus
WindowHidingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowHiding(ioEvent, window);
}


// ===========================================================================
//	WindowShownDoer

OSStatus
WindowShownDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowShown(ioEvent, window);
}


// ===========================================================================
//	WindowHiddenDoer

OSStatus
WindowHiddenDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowHidden(ioEvent, window);
}


// ===========================================================================
//	WindowCollapseDoer

OSStatus
WindowCollapseDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowCollapse(ioEvent, window);
}


// ===========================================================================
//	WindowCollapseAllDoer

OSStatus
WindowCollapseAllDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowCollapseAll(ioEvent, window);
}


// ===========================================================================
//	WindowCollapsingDoer

OSStatus
WindowCollapsingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowCollapsing(ioEvent, window);
}


// ===========================================================================
//	WindowCollapsedDoer

OSStatus
WindowCollapsedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowCollapsed(ioEvent, window);
}


// ===========================================================================
//	WindowExpandDoer

OSStatus
WindowExpandDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowExpand(ioEvent, window);
}


// ===========================================================================
//	WindowExpandAllDoer

OSStatus
WindowExpandAllDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowExpandAll(ioEvent, window);
}


// ===========================================================================
//	WindowExpandingDoer

OSStatus
WindowExpandingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowExpanding(ioEvent, window);
}


// ===========================================================================
//	WindowExpandedDoer

OSStatus
WindowExpandedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowExpanded(ioEvent, window);
}


// ===========================================================================
//	WindowZoomDoer

OSStatus
WindowZoomDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowZoom(ioEvent, window);
}


// ===========================================================================
//	WindowZoomAllDoer

OSStatus
WindowZoomAllDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowZoomAll(ioEvent, window);
}


// ===========================================================================
//	WindowZoomedDoer

OSStatus
WindowZoomedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowZoomed(ioEvent, window);
}


// ===========================================================================
//	WindowBoundsChangingDoer

OSStatus
WindowBoundsChangingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	UInt32		attributes;
	Rect		originalBounds;
	Rect		previousBounds;
	Rect		currentBounds;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamAttributes, attributes);
	SysEventParam::Get(ioEvent, kEventParamOriginalBounds, originalBounds);
	SysEventParam::Get(ioEvent, kEventParamPreviousBounds, previousBounds);
	SysEventParam::Get(ioEvent, kEventParamCurrentBounds, currentBounds);
	
	OSStatus	status = DoWindowBoundsChanging(
								ioEvent, window, attributes, originalBounds,
								previousBounds, currentBounds);
								
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamCurrentBounds, currentBounds);
	}
	
	return status;
}


// ===========================================================================
//	WindowBoundsChangedDoer

OSStatus
WindowBoundsChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	UInt32		attributes;
	Rect		originalBounds;
	Rect		previousBounds;
	Rect		currentBounds;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamAttributes, attributes);
	SysEventParam::Get(ioEvent, kEventParamOriginalBounds, originalBounds);
	SysEventParam::Get(ioEvent, kEventParamPreviousBounds, previousBounds);
	SysEventParam::Get(ioEvent, kEventParamCurrentBounds, currentBounds);
	
	return DoWindowBoundsChanged(ioEvent, window, attributes, originalBounds,
									previousBounds, currentBounds);
}


// ===========================================================================
//	WindowResizeStartedDoer

OSStatus
WindowResizeStartedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowResizeStarted(ioEvent, window);
}


// ===========================================================================
//	WindowResizeCompletedDoer

OSStatus
WindowResizeCompletedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowResizeCompleted(ioEvent, window);
}


// ===========================================================================
//	WindowDragStartedDoer

OSStatus
WindowDragStartedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDragStarted(ioEvent, window);
}


// ===========================================================================
//	WindowDragCompletedDoer

OSStatus
WindowDragCompletedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDragCompleted(ioEvent, window);
}


// ===========================================================================
//	WindowCloseDoer

OSStatus
WindowCloseDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowClose(ioEvent, window);
}


// ===========================================================================
//	WindowCloseAllDoer

OSStatus
WindowCloseAllDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowCloseAll(ioEvent, window);
}


// ===========================================================================
//	WindowClosedDoer

OSStatus
WindowClosedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowClosed(ioEvent, window);
}


// ===========================================================================
//	WindowCursorChangeDoer

OSStatus
WindowCursorChangeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	Point		mouseLocation;
	UInt32		keyModifiers;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, mouseLocation);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
	
	return DoWindowCursorChange(ioEvent, window, mouseLocation, keyModifiers);
}


// ===========================================================================
//	WindowContextualMenuSelectDoer

OSStatus
WindowContextualMenuSelectDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowContextualMenuSelect(ioEvent, window);
}


// ===========================================================================
//	WindowPathSelectDoer

OSStatus
WindowPathSelectDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowPathSelect(ioEvent, window);
}


// ===========================================================================
//	WindowGetIdealSizeDoer

OSStatus
WindowGetIdealSizeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	Point		idealSize;
	
	OSStatus	status = DoWindowGetIdealSize(ioEvent, window, idealSize);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamDimensions, idealSize);
	}
	
	return status;
}


// ===========================================================================
//	WindowGetMinimumSizeDoer

OSStatus
WindowGetMinimumSizeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	Point		minimumSize;
	
	OSStatus	status = DoWindowGetMinimumSize(ioEvent, window, minimumSize);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamDimensions, minimumSize);
	}
	
	return status;
}


// ===========================================================================
//	WindowGetMaximumSizeDoer

OSStatus
WindowGetMaximumSizeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	Point		maximumSize;
	
	OSStatus	status = DoWindowGetMaximumSize(ioEvent, window, maximumSize);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamDimensions, maximumSize);
	}
	
	return status;
}


// ===========================================================================
//	WindowConstrainDoer

OSStatus
WindowConstrainDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
		// ### Need to get parameters
		//
		//	The explanation in CarbonEvents.h is quite lengthy.
		//	There are many parameters and options.
	
	return DoWindowConstrain(ioEvent, window);
}


// ===========================================================================
//	WindowHandleContentClickDoer

OSStatus
WindowHandleContentClickDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
		// ### Need to get mouse down event parameters
	
	return DoWindowHandleContentClick(ioEvent, window);
}


// ===========================================================================
//	WindowFocusAcquiredDoer

OSStatus
WindowFocusAcquiredDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowFocusAcquired(ioEvent, window);
}


// ===========================================================================
//	WindowFocusRelinquishDoer

OSStatus
WindowFocusRelinquishDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowFocusRelinquish(ioEvent, window);
}


// ===========================================================================
//	WindowFocusContentDoer

OSStatus
WindowFocusContentDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowFocusContent(ioEvent, window);
}


// ===========================================================================
//	WindowFocusToolbarDoer

OSStatus
WindowFocusToolbarDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowFocusToolbar(ioEvent, window);
}


// ===========================================================================
//	WindowDrawerOpeningDoer

OSStatus
WindowDrawerOpeningDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDrawerOpening(ioEvent, window);
}


// ===========================================================================
//	WindowDrawerOpenedDoer

OSStatus
WindowDrawerOpenedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDrawerOpened(ioEvent, window);
}


// ===========================================================================
//	WindowDrawerClosingDoer

OSStatus
WindowDrawerClosingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDrawerClosing(ioEvent, window);
}


// ===========================================================================
//	WindowDrawerClosedDoer

OSStatus
WindowDrawerClosedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDrawerClosed(ioEvent, window);
}


} // namespace PPx