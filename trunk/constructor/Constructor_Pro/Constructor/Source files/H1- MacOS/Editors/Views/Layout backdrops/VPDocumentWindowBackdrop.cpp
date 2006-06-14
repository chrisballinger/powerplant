// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPDocumentWindowBackdrop.cpp
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/17/96
//     $Date: 2006/01/18 01:34:03 $
//  $History: VPDocumentWindowBackdrop.cpp $
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
//	User: scouten      QDate: 11/20/96   Time: 14:32
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added CalcLocalBackdropFrame method.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/18/96   Time: 20:40
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Fixed a commenting glitch.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 21:33
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added class.
//	
// ===========================================================================

#include "VPDocumentWindowBackdrop.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>
#if PP_Target_Carbon
#include <VPDataModel.h>
#include "DMIntegerAttribute.h"
#endif

// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	ICN_GrowBoxIcon		= 3060;


// ===========================================================================

#pragma mark *** VPDocumentWindowBackdrop ***

// ---------------------------------------------------------------------------
//		* VPDocumentWindowBackdrop(Int16)
// ---------------------------------------------------------------------------
//	Constructor, specifying the window kind (WDEF #) for this window.

VPDocumentWindowBackdrop::VPDocumentWindowBackdrop(
	SInt16	inWindowKind)
{
	mWindowKind = inWindowKind;
}


// ---------------------------------------------------------------------------
//		* ~VPDocumentWindowBackdrop
// ---------------------------------------------------------------------------
//	Destructor

VPDocumentWindowBackdrop::~VPDocumentWindowBackdrop()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** backdrop drawing

// ---------------------------------------------------------------------------
//		* DrawSelf
// ---------------------------------------------------------------------------
//	Draw a simulation of the standard document window. Color the content
//	area of the window.

void
VPDocumentWindowBackdrop::DrawSelf()
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
	windowFrame.top -= 18;
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
	
	// Now find the window attributes.
	DMAttribute* titleAttr = mRootObject->FindAttributeByKey(pPaneWindowTitle);
	ValidateObject_(titleAttr);
	
	LStr255 title;
	titleAttr->GetTextValue(title);
	
	::InsetRect(&windowFrame, -1, -1);
	
	if (UEnvironment::HasFeature(env_HasAppearance11))
	{
		if (::UseThemeFont(kThemeWindowTitleFont, smCurrentScript) != noErr)
		{
			// Using kThemeWindowTitleFont returns paramErr with CarbonLib 1.2
			// (even though the headers say it is available with 1.1)
			::UseThemeFont(kThemeSystemFont, smCurrentScript);
		}
	}
	
	FontInfo	theFontInfo;
	::GetFontInfo(&theFontInfo);
	
	ThemeWindowMetrics theMetrics;
	theMetrics.metricSize = sizeof(theMetrics);
	theMetrics.titleHeight = theFontInfo.ascent + theFontInfo.descent;
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
	::DrawThemeWindowFrame(kThemeDocumentWindow, &windowFrame, kThemeStateActive, &theMetrics,
		theAttributes, theTitleUPP, (UInt32) &title);
	DisposeWindowTitleDrawingUPP(theTitleUPP);
#else
	// Draw shadow around bottom right of window.
	
	::MoveTo(windowFrame.left, windowFrame.bottom + 1);
	::LineTo(windowFrame.right + 1, windowFrame.bottom + 1);
	::LineTo(windowFrame.right + 1, windowFrame.top);

	// Draw window title.
	
	DrawWindowTitle(0, 28);
	
	// Draw content area.
	
	DrawContentArea();
#endif
}


// ---------------------------------------------------------------------------
//		* DrawContentArea
// ---------------------------------------------------------------------------
//	Color the content area, and draw a size box (if so requested).

void
VPDocumentWindowBackdrop::DrawContentArea()
{

	// Validate pointers.
	
	ValidateThis_();

	// Draw the content area in the appopriate color.

	VPWindowBackdrop::DrawContentArea();

	// Draw size box if needed.
	
	switch (mWindowKind) {

		// document windows

		case documentProc:
		case zoomDocProc: {
		
			Rect contentRect;
			if (CalcLocalContentRect(contentRect)) {
#if !PP_Target_Carbon
				Rect iconRect = contentRect;
				iconRect.left = iconRect.right - 15;
				iconRect.top = iconRect.bottom - 15;
				iconRect.right++;
				iconRect.bottom++;
				
				StApplicationContext appContext;
				::PlotIconID(&iconRect, atNone, ttNone, ICN_GrowBoxIcon);
#endif
			}
			break;
		
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** backdrop location

// ---------------------------------------------------------------------------
//		* GetBackdropOverscan
// ---------------------------------------------------------------------------
//	Leave space for title bar and window shadow.

void
VPDocumentWindowBackdrop::GetBackdropOverscan(
	SDimension32&	outTopLeft,
	SDimension32&	outBotRight)
{
	outTopLeft.width = 16;
	outTopLeft.height = 16 + 19;
	outBotRight.width = 16 + 1;
	outBotRight.height = 16 + 1;
}


// ---------------------------------------------------------------------------
//		* CalcLocalBackdropFrame
// ---------------------------------------------------------------------------
//	Find the frame of the window simulation, in local coordinates of the
//	backdrop pane.

Boolean
VPDocumentWindowBackdrop::CalcLocalBackdropFrame(
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
	outFrameRect.top -= 18;
	return true;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** window title area

// ---------------------------------------------------------------------------
//		* CalcLocalTitleRect									[protected]
// ---------------------------------------------------------------------------
//	Calculate the location of the title bar.

Boolean
VPDocumentWindowBackdrop::CalcLocalTitleRect(
	Rect&	outFrameRect)
{

	// Validate pointers.
	
	ValidateThis_();

	// Get frame for root object, then modify it.

	if (!CalcLocalFrameForRootObject(outFrameRect))
		return false;
	
	outFrameRect.bottom = outFrameRect.top - 1;
	outFrameRect.top -= 18;
	
	return true;

}