// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPFloatSideBackdrop.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/18/96
//     $Date: 2006/01/18 01:34:03 $
//  $History: VPFloatSideBackdrop.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:34
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added CalcLocalBackdropFrame method.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/18/96   Time: 20:38
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added class.
//	
// ===========================================================================

#include "VPFloatSideBackdrop.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>
#if PP_Target_Carbon
#include <VPDataModel.h>
#include "DMIntegerAttribute.h"
#endif

// ===========================================================================

#pragma mark *** VPFloatSideBackdrop ***

// ---------------------------------------------------------------------------
//		* VPFloatSideBackdrop()
// ---------------------------------------------------------------------------
//	Constructor

VPFloatSideBackdrop::VPFloatSideBackdrop()
{
}


// ---------------------------------------------------------------------------
//		* ~VPFloatSideBackdrop
// ---------------------------------------------------------------------------
//	Destructor

VPFloatSideBackdrop::~VPFloatSideBackdrop()
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
VPFloatSideBackdrop::DrawSelf()
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
	windowFrame.left -= 13;
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
	theMetrics.titleHeight = 13;
	theMetrics.titleWidth = ::StringWidth(title);
	theMetrics.popupTabOffset = 0;
	theMetrics.popupTabWidth = 0;
	theMetrics.popupTabPosition = 0;
	
	ThemeWindowAttributes theAttributes = /* kThemeWindowHasTitleText + */ kThemeWindowHasCollapseBox;
	
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
	::DrawThemeWindowFrame(kThemeUtilitySideWindow, &windowFrame, kThemeStateActive, &theMetrics,
		theAttributes, theTitleUPP, (UInt32) &title);
	DisposeWindowTitleDrawingUPP(theTitleUPP);
#else
	// Draw shadow around bottom right of window.
	
	::MoveTo(windowFrame.left, windowFrame.bottom + 1);
	::LineTo(windowFrame.right + 1, windowFrame.bottom + 1);
	::LineTo(windowFrame.right + 1, windowFrame.top);

	// Draw window title.
	
	DrawWindowTitle(128, 0);

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
VPFloatSideBackdrop::CalcLocalBackdropFrame(
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
	outFrameRect.left -= 12;
	return true;
	
}


// ---------------------------------------------------------------------------
//		* GetBackdropOverscan
// ---------------------------------------------------------------------------
//	Leave space for title bar on left side and window shadow.

void
VPFloatSideBackdrop::GetBackdropOverscan(
	SDimension32&	outTopLeft,
	SDimension32&	outBotRight)
{
	outTopLeft.width = 16 + 13;
	outTopLeft.height = 16;
	outBotRight.width = 16 + 1;
	outBotRight.height = 16 + 1;
}