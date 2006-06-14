// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GWindowBorder.cpp			   © 1996 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "GWindowBorder.h"

	// PowerPlant : GA : General utilities
#include "UGraphicsUtilities.h"

	// PowerPlant : GA : Grayscale utilities
#include "UGAColorRamp.h"


// ===========================================================================

#pragma mark *** GWindowBorder ***

// ---------------------------------------------------------------------------
//		¥ GWindowBorder(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[LView data]
//		Int32:			Pane ID of the inset pane (the pane we're framing)
//		Boolean:		True if the outer frame needs a notch for grow box
//		Boolean:		True if the inner frame needs a notch for grow box
//		Boolean:		True if the inset pane should be framed

GWindowBorder::GWindowBorder(
	LStream*	inStream)

: LView(inStream)
{
	inStream->ReadData(&mInsetSubPaneID, sizeof (mInsetSubPaneID));
	inStream->ReadData(&mGrowBoxOuterNotch, sizeof (mGrowBoxOuterNotch));
	inStream->ReadData(&mGrowBoxInnerNotch, sizeof (mGrowBoxInnerNotch));
	inStream->ReadData(&mFrameInsetPane, sizeof (mFrameInsetPane));
	mInsetSubPane = nil;
	mHasBeenShown = false;
	mDrawFrame = true;
}


// ---------------------------------------------------------------------------
//		¥ ~GWindowBorder
// ---------------------------------------------------------------------------
//	Destructor

GWindowBorder::~GWindowBorder()
{
}


// ---------------------------------------------------------------------------
//		¥ Activate
// ---------------------------------------------------------------------------
//	Overriden to force an immediate redraw of the pane.

void
GWindowBorder::Activate()
{
	LView::Activate();
	UpdatePort();
}



// ---------------------------------------------------------------------------
//		¥ Deactivate
// ---------------------------------------------------------------------------
//	Overriden to force an immediate redraw of the pane.

void
GWindowBorder::Deactivate()
{
	LView::Deactivate();
	UpdatePort();
}


// ---------------------------------------------------------------------------
//		¥ DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Draw the inset area.

void
GWindowBorder::DrawSelf()
{
	if (!mDrawFrame)
		return;
	
	::PenNormal();							//¥ 2.2a1: BUG FIX #608

	Rect frame;
	CalcLocalFrameRect(frame);
	StDeviceLoop dev(frame);
	Int16 depth;
	while (dev.NextDepth(depth)) {
		DrawInset(depth);
	}

}


// ---------------------------------------------------------------------------
//		¥ DrawInset												[protected]
// ---------------------------------------------------------------------------
//	Draw the inset border, given a specific pixel depth.
//	Called from the DeviceLoop iterator in DrawSelf.

void
GWindowBorder::DrawInset(
	Int16	inPixelDepth)
{
	// * COLOR CONSTANTS
	const int 	B	=	colorRamp_Black;		//	Index for black
	const int	W	=	colorRamp_White;		//	Index for white
	
	// See if this window is active.
	
	Boolean active = IsActive();
	
	// Find area to be painted in main background shade.
	
	RgnHandle shadeRgn = ::NewRgn();
	ThrowIfNil_(shadeRgn);
	Rect frame;
	CalcLocalFrameRect(frame);
	::RectRgn(shadeRgn, &frame);

	// Chip out grow box if so requested.
	
	if (mGrowBoxOuterNotch) {
		RgnHandle growRgn = ::NewRgn();
		ThrowIfNil_(growRgn);
		Rect growRect = frame;
		growRect.left = growRect.right - 15;
		growRect.top = growRect.bottom - 15;
		::RectRgn(growRgn, &growRect);
		::DiffRgn(shadeRgn, growRgn, shadeRgn);
		::DisposeRgn(growRgn);
	}

	// Chip out inset pane (if one exists).
	
	Rect insetRect;
	if (mFrameInsetPane && mInsetSubPane) {
		RgnHandle insetRgn = ::NewRgn();
		ThrowIfNil_(insetRgn);
		mInsetSubPane->CalcPortFrameRect(insetRect);
		PortToLocalPoint(topLeft(insetRect));
		PortToLocalPoint(botRight(insetRect));
		
		// Draw frame around inside pane.

		if (mFrameInsetPane) {
			::RGBForeColor(active ? &UGAColorRamp::GetColor(B) : &UGAColorRamp::GetColor(10));

			UGraphicsUtilities::TopLeftSide(&insetRect, -1, -1, -1, -1);
			if (mGrowBoxInnerNotch)
				UGraphicsUtilities::BottomRightSideWithNotch(&insetRect, -1, -1, -1, -1, 15);
			else
				UGraphicsUtilities::BottomRightSide(&insetRect, -1, -1, -1, -1);

			::InsetRect(&insetRect, -1, -1);
		}

		// Now we can subtract inset region from shade region.

		::RectRgn(insetRgn, &insetRect);

		// Chip out grow box from inset pane region.
		
		if (mGrowBoxInnerNotch) {
			RgnHandle insetGrowRgn = ::NewRgn();
			ThrowIfNil_(insetGrowRgn);
			Rect insetGrowRect = insetRect;
			insetGrowRect.left = insetGrowRect.right - 15;
			insetGrowRect.top = insetGrowRect.bottom - 15;
			::RectRgn(insetGrowRgn, &insetGrowRect);
			::DiffRgn(insetRgn, insetGrowRgn, insetRgn);
			::DisposeRgn(insetGrowRgn);
		}

		// Now we can subtract inset region from shade region.

		::DiffRgn(shadeRgn, insetRgn, shadeRgn);
		::DisposeRgn(insetRgn);

	}

	// If bit depth is 4 colors or less, just erase the whole thing.
	
	if (inPixelDepth < 4) {
		::RGBBackColor(active ? &UGAColorRamp::GetColor(3) : &UGAColorRamp::GetColor(2));
		::EraseRgn(shadeRgn);
		::DisposeRgn(shadeRgn);
		return;
	}

	// We have at least 16 colors: We can draw 3D effects.
	
	::InsetRgn(shadeRgn, 1, 1);
	::RGBForeColor(active ? &UGAColorRamp::GetColor(3) : &UGAColorRamp::GetColor(2));
	::PaintRgn(shadeRgn);
	::DisposeRgn(shadeRgn);

	// Draw outer borders in 3D.
	
	::RGBForeColor(&UGAColorRamp::GetColor(W));
	UGraphicsUtilities::TopLeftSide(&frame, 0, 0, 1, 1);

	::RGBForeColor(active ? &UGAColorRamp::GetColor(6) : &UGAColorRamp::GetColor(4));
	if (mGrowBoxOuterNotch)
		UGraphicsUtilities::BottomRightSideWithNotch(&frame, 1, 1, 0, 0, 15);
	else
		UGraphicsUtilities::BottomRightSide(&frame, 1, 1, 0, 0);
	
	::SetCPixel(frame.left, frame.bottom-1, active ? &UGAColorRamp::GetColor(3) : &UGAColorRamp::GetColor(2));
	::SetCPixel(frame.right-1, frame.top, active ? &UGAColorRamp::GetColor(3) : &UGAColorRamp::GetColor(2));
	
	// If inside pane is found, draw inner borders also.
	
	if (mFrameInsetPane && mInsetSubPane) {
		::RGBForeColor(active ? &UGAColorRamp::GetColor(6) : &UGAColorRamp::GetColor(4));
		UGraphicsUtilities::TopLeftSide(&insetRect, -1, -1, 0, 0);

		::RGBForeColor(&UGAColorRamp::GetColor(W));
		if (mGrowBoxInnerNotch)
			UGraphicsUtilities::BottomRightSideWithNotch(&insetRect, 0, 0, -1, -1, 15);
		else
			UGraphicsUtilities::BottomRightSide(&insetRect, 0, 0, -1, -1);
		
		::SetCPixel(insetRect.left-1, insetRect.bottom, active ? &UGAColorRamp::GetColor(3) : &UGAColorRamp::GetColor(2));
		::SetCPixel(insetRect.right, insetRect.top-1, active ? &UGAColorRamp::GetColor(3) : &UGAColorRamp::GetColor(2));
		
	}

}


// ---------------------------------------------------------------------------
//		¥ RefreshInset											[protected]
// ---------------------------------------------------------------------------
//	Force a redraw of the frame region.

void
GWindowBorder::RefreshInset()
{

	if (!mDrawFrame)
		return;
	
	// Find area to be painted in main background shade.
	
	FocusDraw();

	RgnHandle shadeRgn = ::NewRgn();
	ThrowIfNil_(shadeRgn);
	Rect frame;
	CalcLocalFrameRect(frame);
	::RectRgn(shadeRgn, &frame);

	// Chip out grow box if so requested.

	if (mGrowBoxOuterNotch) {
		RgnHandle growRgn = ::NewRgn();
		ThrowIfNil_(growRgn);
		Rect growRect = frame;
		growRect.left = growRect.right - 15;
		growRect.top = growRect.bottom - 15;
		::RectRgn(growRgn, &growRect);
		::DiffRgn(shadeRgn, growRgn, shadeRgn);
		::DisposeRgn(growRgn);
	}

	// Chip out inset pane (if one exists).
	
	Rect insetRect;
	if (mInsetSubPane) {
		RgnHandle insetRgn = ::NewRgn();
		ThrowIfNil_(insetRgn);
		mInsetSubPane->CalcPortFrameRect(insetRect);
		PortToLocalPoint(topLeft(insetRect));
		PortToLocalPoint(botRight(insetRect));
		
		// Now we can subtract inset region from shade region.

		::RectRgn(insetRgn, &insetRect);

		// Chip out grow box from inset pane region.
		
		if (mGrowBoxInnerNotch) {
			RgnHandle insetGrowRgn = ::NewRgn();
			ThrowIfNil_(insetGrowRgn);
			Rect insetGrowRect = insetRect;
			insetGrowRect.left = insetGrowRect.right - 15;
			insetGrowRect.top = insetGrowRect.bottom - 15;
			::RectRgn(insetGrowRgn, &insetGrowRect);
			::DiffRgn(insetRgn, insetGrowRgn, insetRgn);
			::DisposeRgn(insetGrowRgn);
		}

		// Now we can subtract inset region from shade region.

		::DiffRgn(shadeRgn, insetRgn, shadeRgn);
		::DisposeRgn(insetRgn);

	}

	// Invalidate the shade region.
	
	InvalPortRgn(shadeRgn);
	::DisposeRgn(shadeRgn);

}


// ---------------------------------------------------------------------------
//		¥ FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Identify the subpane that we'll frame for the inset.

void
GWindowBorder::FinishCreateSelf()
{
	mInsetSubPane = FindPaneByID(mInsetSubPaneID);
	if (mInsetSubPane == this)
		mInsetSubPane = nil;
}


// ---------------------------------------------------------------------------
//		¥ ActivateSelf											[protected]
// ---------------------------------------------------------------------------
//	Redraw the frame on activate.

void
GWindowBorder::ActivateSelf()
{
	RefreshInset();
}



// ---------------------------------------------------------------------------
//		¥ DeactivateSelf										[protected]
// ---------------------------------------------------------------------------
//	Redraw the frame on deactivate.

void
GWindowBorder::DeactivateSelf()
{
	RefreshInset();
}


// ---------------------------------------------------------------------------
//		¥ ShowSelf												[protected]
// ---------------------------------------------------------------------------
//	Remove window borders when Aaron is running. We do this by checking
//	to see if struct & content rects are more than one pixel apart
//	on the left edge, which should also work under Copland.
//
//	We have to wait until the window has been shown for these measurements
//	to be valid, thus the override of Show().

void
GWindowBorder::ShowSelf()
{

	//¥ 2.2a3: SUGGESTION #632: added method

	do {
		
		// Do it only the first time.
	
		if (mHasBeenShown)
			break;
		mHasBeenShown = true;
		
		// If there's no inset pane, then leave the border as is.
		
		if (mInsetSubPane == nil)
			break;
		
		// Find window measurements. If they're normal, then leave border
		// as is.
		
		WindowPeek win = (WindowPeek) GetMacPort();
		Rect structRect = (*(win->strucRgn))->rgnBBox;
		Rect contRect = (*(win->contRgn))->rgnBBox;
		if ((contRect.left - structRect.left) <= 1)
			break;

		// If this pane isn't in the top left corner, don't touch it.
		
		Rect thisFrame;
		CalcPortFrameRect(thisFrame);
		if (thisFrame.left != 0 || thisFrame.top != 0)
			break;
		
		// If the inset pane isn't (more or less) in the top left
		// corner, nothing chnages.
		
		Rect insetFrame;
		mInsetSubPane->CalcPortFrameRect(insetFrame);
		if (insetFrame.left >= 7 || insetFrame.top >= 7)
			break;
	
		// Passed all the tests, disable the frame drawing.
		
		mDrawFrame = false;

		MoveBy(-insetFrame.left, -insetFrame.top, false);
		ResizeFrameBy(
				insetFrame.left + (thisFrame.right - insetFrame.right),
				insetFrame.top << 1,
				false);

	} while (0);

	// We now return you to your regularly scheduled Show.
	
	LView::ShowSelf();

}
