// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GActiveScroller.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 03/22/96
//     $Date: 2006/01/18 01:32:45 $
//	$History: GActiveScroller.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:43
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Pane classes
//	Improved commenting.
//
//	*****************  Version 4   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:24
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Pane classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 12/07/96   Time: 11:18
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Pane classes'
//	Comment: Updated toRobin's drop 12/06/96.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "GActiveScroller.h"

	// PowerPlant : GA : General utilities
#include <UGraphicsUtilities.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** GActiveScroller ***

// ---------------------------------------------------------------------------
//		* GActiveScroller(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Takes no additional data.

GActiveScroller::GActiveScroller(
	LStream*	inStream)

: LActiveScroller(inStream)
{
}


// ---------------------------------------------------------------------------
//		* ~GActiveScroller
// ---------------------------------------------------------------------------
//	Destructor

GActiveScroller::~GActiveScroller()
{
}


// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Overriden to draw scroller with gray frame when inactive.

void
GActiveScroller::DrawSelf()
{

	// Calculate the border of the frame. The scroller has
	// a one-pixel border.

	Rect frame;
	CalcLocalFrameRect(frame);
	::PenNormal();
	ApplyForeAndBackColors();
	
	RGBColor theColor;
	IsActive() ? theColor = UGAColorRamp::GetBlackColor() : theColor = UGAColorRamp::GetColor(10);
	::RGBForeColor(&theColor);		// ES: quick hack to make scrollers look good
	
	::FrameRect(&frame);
	
	if (mVerticalBar != nil) {
		MoveTo(frame.right - 16, frame.top);
		LineTo(frame.right - 16, frame.bottom - 1);
	}
	
	if (mHorizontalBar != nil) {
		MoveTo(frame.left, frame.bottom - 16);
		LineTo(frame.right - 1, frame.bottom - 16);
	}
		
	// When inactive, ScrollBars are hidden. Just outline
	// the ScrollBar locations with one pixel borders.
	
	if (!IsActive()) {
		if (mVerticalBar != nil) {
			mVerticalBar->CalcPortFrameRect(frame);
			PortToLocalPoint(topLeft(frame));
			PortToLocalPoint(botRight(frame));
			::FrameRect(&frame);
			::InsetRect(&frame, 1, 1);
			::EraseRect(&frame);
		}

		if (mHorizontalBar != nil) {
			mHorizontalBar->CalcPortFrameRect(frame);
			PortToLocalPoint(topLeft(frame));
			PortToLocalPoint(botRight(frame));
			::FrameRect(&frame);
			::InsetRect(&frame, 1, 1);
			::EraseRect(&frame);
		}
	}
	
	// Frame the outside of the scrolled view.		//* 2.2a3
	
	Rect scrollViewFrame;
	if (mScrollingView) {
		mScrollingView->CalcPortFrameRect(scrollViewFrame);
		PortToLocalPoint(topLeft(scrollViewFrame));
		PortToLocalPoint(botRight(scrollViewFrame));
		::InsetRect(&scrollViewFrame, -1, -1);
		::FrameRect(&scrollViewFrame);
	}
	
}



// ---------------------------------------------------------------------------
//		* ActivateSelf											[protected]
// ---------------------------------------------------------------------------
//	Overriden to force the scroll bars to redraw on an activate event.

void
GActiveScroller::ActivateSelf()
{
	if (mVerticalBar != nil) {
		mVerticalBar->Show();
	}
	if (mHorizontalBar != nil) {
		mHorizontalBar->Show();
	}
	
	if (FocusExposed()) {				// Redraw immediately
		Rect frame;
		CalcLocalFrameRect(frame);
		if (ExecuteAttachments(msg_DrawOrPrint, &frame)) {
			DrawSelf();
		}
	}
}
