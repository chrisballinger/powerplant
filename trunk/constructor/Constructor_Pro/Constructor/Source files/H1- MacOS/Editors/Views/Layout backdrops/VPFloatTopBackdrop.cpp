// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPFloatTopBackdrop.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/18/96
//     $Date: 2006/01/18 01:34:04 $
//  $History: VPFloatTopBackdrop.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/04/96   Time: 21:02
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Fixed floating window drawing to once again use Geneva 9.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:35
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added CalcLocalBackdropFrame method.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/18/96   Time: 20:40
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added class.
//	
// ===========================================================================

#include "VPFloatTopBackdrop.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>
#if PP_Target_Carbon
#include <VPDataModel.h>
#include "DMIntegerAttribute.h"
#endif

// ===========================================================================

#pragma mark *** VPFloatTopBackdrop ***

// ---------------------------------------------------------------------------
//		* VPFloatTopBackdrop()
// ---------------------------------------------------------------------------
//	Constructor

VPFloatTopBackdrop::VPFloatTopBackdrop()
{
}


// ---------------------------------------------------------------------------
//		* ~VPFloatTopBackdrop
// ---------------------------------------------------------------------------
//	Destructor

VPFloatTopBackdrop::~VPFloatTopBackdrop()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** backdrop drawing

// ---------------------------------------------------------------------------
//		* DrawSelf
// ---------------------------------------------------------------------------
//	Draw the window simulation.

void
VPFloatTopBackdrop::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find the root pane. If none exists, don't draw anything.
	
	Rect rootFrame;
	if (!CalcLocalContentRect(rootFrame))
		return;
	
	// Frame the root pane.

#if !PP_Target_Carbon
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor(&theColor);
	::InsetRect(&rootFrame, -1, -1);
	::FrameRect(&rootFrame);
#endif

	// Frame the window.
	
	Rect windowFrame = rootFrame;
#if !PP_Target_Carbon
	windowFrame.top -= 15;
	::FrameRect(&windowFrame);
#endif
	
	// Shade the desktop area.

	::InsetRect(&windowFrame, 1, 1);
	ShadeDesktopArea(windowFrame);
	
#if PP_Target_Carbon
	// Draw content area.
	
	DrawContentArea();

	// Validate pointers.
	
	ValidateObject_(mRootObject.GetObject());
	
	// Now find the window title.
	
	DMAttribute* titleAttr = mRootObject->FindAttributeByKey(pPaneWindowTitle);
	ValidateObject_(titleAttr);
	
	LStr255 title;
	titleAttr->GetTextValue(title);
	
	::InsetRect(&windowFrame, -1, -1);
	
	ThemeWindowMetrics theMetrics;
	theMetrics.metricSize = sizeof(theMetrics);
	theMetrics.titleHeight = 15;
	theMetrics.titleWidth = ::StringWidth(title);
	theMetrics.popupTabOffset = 0;
	theMetrics.popupTabWidth = 0;
	theMetrics.popupTabPosition = 0;
	
	ThemeWindowAttributes theAttributes = kThemeWindowHasTitleText + kThemeWindowHasCollapseBox;
	
	DMIntegerAttribute* closeAttr = mRootObject->FindIntegerAttribute(pPaneWindowCloseBox);
	if (closeAttr->GetIntValue())
		theAttributes += kThemeWindowHasCloseBox;
	
	DMIntegerAttribute* sizeBoxAttr = mRootObject->FindIntegerAttribute(pPaneWindowSizeBox);
	if (sizeBoxAttr->GetIntValue())
		theAttributes += kThemeWindowHasGrow;
	
	DMIntegerAttribute* zoomAttr = mRootObject->FindIntegerAttribute(pPaneWindowZoomable);
	if (zoomAttr->GetIntValue())
		theAttributes += kThemeWindowHasFullZoom;
	
	WindowTitleDrawingUPP theTitleUPP = NewWindowTitleDrawingUPP(DrawWindowTitle);
	::DrawThemeWindowFrame(kThemeUtilityWindow, &windowFrame, kThemeStateActive, &theMetrics,
		theAttributes, theTitleUPP, (UInt32) &title);
	DisposeWindowTitleDrawingUPP(theTitleUPP);
#else
	// Draw shadow around bottom right of window.
	
	::MoveTo(windowFrame.left, windowFrame.bottom + 1);
	::LineTo(windowFrame.right + 1, windowFrame.bottom + 1);
	::LineTo(windowFrame.right + 1, windowFrame.top);

	// Draw window title.
	
	DrawWindowTitle(3050, 0);
	
	// Draw content area.
	
	DrawContentArea();

#endif
}


// ===========================================================================

#pragma mark -
#pragma mark ** backdrop location

// ---------------------------------------------------------------------------
//		* CalcLocalBackdropFrame
// ---------------------------------------------------------------------------
//	Find the frame of the window simulation, in local coordinates of the
//	backdrop pane.

Boolean
VPFloatTopBackdrop::CalcLocalBackdropFrame(
	Rect&	outFrameRect)
{

	// Validate pointers.
	
	ValidateThis_();

	// For simple backdrop, the backdrop frame
	// includes only the root object plus the one-pixel
	// border beyond it.

	if (!CalcLocalFrameForRootObject(outFrameRect))
		return false;
	
	::InsetRect(&outFrameRect, -1, -1);
	outFrameRect.top -= 14;
	return true;
	
}


// ---------------------------------------------------------------------------
//		* GetBackdropOverscan
// ---------------------------------------------------------------------------
//	Leave space for title bar on left side and window shadow.

void
VPFloatTopBackdrop::GetBackdropOverscan(
	SDimension32&	outTopLeft,
	SDimension32&	outBotRight)
{
	outTopLeft.width = 16;
	outTopLeft.height = 16 + 15;
	outBotRight.width = 16 + 1;
	outBotRight.height = 16 + 1;
}


// ===========================================================================

#pragma mark -
#pragma mark ** window title area

// ---------------------------------------------------------------------------
//		* CalcLocalTitleRect									[protected]
// ---------------------------------------------------------------------------
//	Calculate the location of the title bar.

Boolean
VPFloatTopBackdrop::CalcLocalTitleRect(
	Rect&	outFrameRect)
{

	// Validate pointers.
	
	ValidateThis_();

	// Get frame for root object, then modify it.

	if (!CalcLocalFrameForRootObject(outFrameRect))
		return false;
	
	outFrameRect.bottom = outFrameRect.top - 1;
	outFrameRect.top -= 15;
	
	return true;

}