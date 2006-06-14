// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPMovableModalBackdrop.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/18/96
//     $Date: 2006/01/18 01:34:04 $
//  $History: VPMovableModalBackdrop.cpp $
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
//	User: scouten      QDate: 11/20/96   Time: 14:36
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added CalcLocalBackdropFrame method.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/18/96   Time: 20:43
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added class.
//	
// ===========================================================================

#include "VPMovableModalBackdrop.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>
#if PP_Target_Carbon
#include <VPDataModel.h>
#endif

// ===========================================================================

#pragma mark *** VPMovableModalBackdrop ***

// ---------------------------------------------------------------------------
//		* VPMovableModalBackdrop()
// ---------------------------------------------------------------------------
//	Constructor

VPMovableModalBackdrop::VPMovableModalBackdrop()
{
}


// ---------------------------------------------------------------------------
//		* ~VPMovableModalBackdrop
// ---------------------------------------------------------------------------
//	Destructor

VPMovableModalBackdrop::~VPMovableModalBackdrop()
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
VPMovableModalBackdrop::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find the root pane. If none exists, don't draw anything.
	
	Rect rootFrame;
	if (!CalcLocalContentRect(rootFrame))
		return;
	
	// Frame the dialog. A modal dialog has a double frame when inactive.

	Rect outerFrame = rootFrame;
	
#if !PP_Target_Carbon
	RGBColor theColor = UGAColorRamp::GetColor(9);
	::RGBForeColor(&theColor);
	
	::InsetRect(&outerFrame, -5, -5);
	outerFrame.top -= 15;
	::FrameRect(&outerFrame);
	
	Rect innerFrame = rootFrame;
	::InsetRect(&innerFrame, -2, -2);
	::PenSize(2, 2);
	::FrameRect(&innerFrame);
#endif
	
	// Shade the desktop area.

	::InsetRect(&outerFrame, 1, 1);
	ShadeDesktopArea(outerFrame);
	
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
	
	::InsetRect(&outerFrame, -1, -1);
	
	ThemeWindowMetrics theMetrics;
	theMetrics.metricSize = sizeof(theMetrics);
	theMetrics.titleHeight = 15;
	theMetrics.titleWidth = ::StringWidth(title);
	theMetrics.popupTabOffset = 0;
	theMetrics.popupTabWidth = 0;
	theMetrics.popupTabPosition = 0;
	
	ThemeWindowAttributes theAttributes = kThemeWindowHasTitleText;
	
	WindowTitleDrawingUPP theTitleUPP = NewWindowTitleDrawingUPP(DrawWindowTitle);
	::DrawThemeWindowFrame(kThemeMovableDialogWindow, &outerFrame, kThemeStateActive, &theMetrics,
		theAttributes, theTitleUPP, (UInt32) &title);
	DisposeWindowTitleDrawingUPP(theTitleUPP);
#else
	// Draw window title.
	
	DrawWindowTitle(0, 28);
	
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
VPMovableModalBackdrop::CalcLocalBackdropFrame(
	Rect&	outFrameRect)
{

	// Validate pointers.
	
	ValidateThis_();

	// For simple backdrop, the backdrop frame
	// includes only the root object plus the one-pixel
	// border beyond it.

	if (!CalcLocalFrameForRootObject(outFrameRect))
		return false;
	
#if PP_Target_Carbon
	::InsetRect(&outFrameRect, -1, -1);
#else
	::InsetRect(&outFrameRect, -8, -8);
#endif
	outFrameRect.top -= 16;
	return true;
	
}


// ---------------------------------------------------------------------------
//		* GetBackdropOverscan
// ---------------------------------------------------------------------------
//	Leave space for title bar and window shadow.

void
VPMovableModalBackdrop::GetBackdropOverscan(
	SDimension32&	outTopLeft,
	SDimension32&	outBotRight)
{
	outTopLeft.width = 24;
	outTopLeft.height = 16 + 19 + 5;
	outBotRight.width = 24;
	outBotRight.height = 24;
}


// ===========================================================================

#pragma mark -
#pragma mark ** window title area

// ---------------------------------------------------------------------------
//		* CalcLocalTitleRect
// ---------------------------------------------------------------------------
//	Calculate the location of the title bar.

Boolean
VPMovableModalBackdrop::CalcLocalTitleRect(
	Rect&	outFrameRect)
{

	// Validate pointers.
	
	ValidateThis_();

	// Get frame for root object, then modify it.

	if (!CalcLocalContentRect(outFrameRect))
		return false;
	
	outFrameRect.bottom = outFrameRect.top - 2;
	outFrameRect.top -= 19;
	
	return true;

}