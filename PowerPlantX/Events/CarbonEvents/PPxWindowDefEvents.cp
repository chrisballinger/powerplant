// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowDefEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:16 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxWindowDefEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	WindowDrawFrameDoer

OSStatus
WindowDrawFrameDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDrawFrame(ioEvent, window);
}


// ===========================================================================
//	WindowDrawPartDoer

OSStatus
WindowDrawPartDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef				window;
	WindowDefPartCodeType	part;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamWindowDefPart, part);
	
	return DoWindowDrawPart(ioEvent, window, part);
}


// ===========================================================================
//	WindowGetRegionDoer

OSStatus
WindowGetRegionDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef				window;
	WindowRegionCodeType	regionCode;
	RgnHandle				region;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamWindowRegionCode, regionCode);
	SysEventParam::Get(ioEvent, kEventParamRgnHandle, region);
	
	return DoWindowGetRegion(ioEvent, window, regionCode, region);
}


// ===========================================================================
//	WindowHitTestDoer

OSStatus
WindowHitTestDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
 	Point		globalPoint;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, globalPoint);
	
	WindowDefPartCodeType	partHit;
	
	OSStatus	status = DoWindowHitTest(ioEvent, window, globalPoint, partHit);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamWindowDefPart, partHit);
	}
	
	return status;
}


// ===========================================================================
//	WindowInitDoer

OSStatus
WindowInitDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	UInt32		features;
	
	OSStatus	status = DoWindowInit(ioEvent, window, features);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamWindowFeatures, features);
	}
	
	return status;
}


// ===========================================================================
//	WindowDisposeDoer

OSStatus
WindowDisposeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDispose(ioEvent, window);
}


// ===========================================================================
//	WindowDragHiliteDoer

OSStatus
WindowDragHiliteDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	Boolean		drawHilite;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamWindowDragHiliteFlag, drawHilite);
	
	return DoWindowDragHilite(ioEvent, window, drawHilite);
}


// ===========================================================================
//	WindowModifiedDoer

OSStatus
WindowModifiedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	Boolean		isModified;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamWindowModifiedFlag, isModified);
	
	return DoWindowModified(ioEvent, window, isModified);
}


// ===========================================================================
//	WindowSetupProxyDragImageDoer

OSStatus
WindowSetupProxyDragImageDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
 	RgnHandle	ImageClipRgn;
 	RgnHandle	dragOutline;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamWindowProxyImageRgn, ImageClipRgn);
	SysEventParam::Get(ioEvent, kEventParamWindowProxyOutlineRgn, dragOutline);
	
	GWorldPtr	image;
	
	OSStatus	status = DoWindowSetupProxyDragImage(ioEvent, window,
											ImageClipRgn, dragOutline, image);
	
	if (status == noErr) {
	
			// A GWorldPtr is the same as a CGrafPtr. The SysEventParam::Set()
			// template function requires unique types. Creating a wrapper
			// for pointer types is cumbersome. This is the only place a
			// GWorldPtr is used for an event parameter, so we just call the
			// Toolbox routine directly.
		
		status = ::SetEventParameter(ioEvent, kEventParamWindowProxyGWorldPtr,
										typeGWorldPtr, sizeof(image), &image);
	
		PPx_ThrowIfOSError_(status, "SetEventParameter failed");
	}
	
	return status;
}


// ===========================================================================
//	WindowStateChangedDoer

OSStatus
WindowStateChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	UInt32		stateFlags;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamWindowStateChangedFlags, stateFlags);
	
	return DoWindowStateChanged(ioEvent, window, stateFlags);
}


// ===========================================================================
//	WindowMeasureTitleDoer

OSStatus
WindowMeasureTitleDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	SInt16		fullWidth;
	SInt16		textWidth;
	
	OSStatus	status = DoWindowMeasureTitle(ioEvent, window,
												fullWidth, textWidth);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamWindowTitleFullWidth, fullWidth);
		SysEventParam::Set(ioEvent, kEventParamWindowTitleTextWidth, textWidth);
	}
	
	return status;
}


// ===========================================================================
//	WindowDrawGrowBoxDoer

OSStatus
WindowDrawGrowBoxDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowDrawGrowBox(ioEvent, window);
}


// ===========================================================================
//	WindowGetGrowImageRegionDoer

OSStatus
WindowGetGrowImageRegionDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	Rect		globalBounds;
	RgnHandle	imageRgn;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	SysEventParam::Get(ioEvent, kEventParamWindowGrowRect, globalBounds);
	SysEventParam::Get(ioEvent, kEventParamRgnHandle, imageRgn);
	
	return DoWindowGetGrowImageRegion(ioEvent, window, globalBounds, imageRgn);
}


// ===========================================================================
//	WindowPaintDoer

OSStatus
WindowPaintDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	window;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, window);
	
	return DoWindowPaint(ioEvent, window);
}


} // namespace PPx