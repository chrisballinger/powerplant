// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< * >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< * >>----------------------------------------

FILE:				PaintCanvas.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTCanvas

DESCRIPTION:		Implementation file for class
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.25

CHANGE HISTORY :

		97.01.28	ebs		Finished marquee bug fix from yesterday (MARQUEE_FIX #define)
		97.01.27	ebs		Fixed marquee bug (select all in 1x mode didn't show right/bottom)
		96.12.13	ebs		MapPointToCell handles negative cells better (rounds down)
		96.08.28	ebs		Support for locked files
		96.08.08	ebs		Added inWidthOverride & inHeightOverride for resize support (ppat)
		96.08.05	ebs		Resize canvas smaller now, just just bigger
		96.08.04	ebs		Added GetSpaceBetween. 
							MapRectToCanvas & MapRgnToCanvas are more accurate
		96.08.02	ebs		Fixed redraw of right/bottom area of canvas (single pixel lines)
		96.08.01	ebs		Single buffer for all canvases
		96.07.08	ebs		Changed the way resizing works (no more broadcasting)
		96.07.02	ebs		Re-worked internal coordinates (1 pixel off problems). HotSpots.
		96.04.25	ebs		Integrated into Robin's sources

---------------------------------------<< * >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTOffscreenDragTask.h"
#include "PTActions.h"
#include "SUPaneResizer.h"
#include "SUColorUtils.h"

#define MARQUEE_FIX
#undef NEW_HOTSPOT_CODE	

const SInt32	kSideMargin 			= 1;	// !these need to remain identical or change the calculations
const SInt32	kSpaceBetween 			= 1;

/*===================================
	statics
====================================*/
SInt32 			PTCanvas::sNumWindows = 0;
SUOffscreen *	PTCanvas::sBigScratchBuffer = nil;

/*===================================
	Constructor
====================================*/
PTCanvas::PTCanvas( LStream *inStream ) : 
			LView( inStream ), PTImageDragMixin( this->GetMacPort(), this ) 
{
	this->ClearInstanceVariables();

	if ( sNumWindows == 0 )
		sBigScratchBuffer = SUOffscreen::CreateBuffer( 320, 320, 32 );
	
	++sNumWindows;
}

/*===================================
	ClearInstanceVariables
====================================*/
void PTCanvas::ClearInstanceVariables()
{
	mPaintView = nil;
	mCellWidth = 6;
	mSpaceBetween = kSpaceBetween;
	mImageHeight = mImageWidth = 0;
	SetRect( &mFirstRect, 0, 0, 0, 0 );
	mHotSpot.h = -1;
	mHotSpot.v = -1;
}

/*===================================
	Destructor
====================================*/
PTCanvas::~PTCanvas()
{
	if ( (--sNumWindows == 0) && sBigScratchBuffer )
	{
		delete sBigScratchBuffer;
		sBigScratchBuffer = nil;
	}
}

/*===================================
	CreatePaintCanvasFromStream
====================================*/
PTCanvas *PTCanvas::CreatePaintCanvasStream( LStream *inStream )
{
	return new PTCanvas( inStream );
}

/*===================================
	FinishCreateSelf
====================================*/
void PTCanvas::FinishCreateSelf()
{
	LView::FinishCreateSelf();
	mPaintView = (PTPaintView*) this->GetSuperView();
}

/*===================================
	AdjustCursorSelf
====================================*/
void PTCanvas::AdjustCursorSelf( Point inPortPt, const EventRecord &inMacEvent )
{
	if ( !mPaintView ) return;
	
	PortToLocalPoint( inPortPt );
	SInt32	totalCellWidth = mCellWidth + mSpaceBetween;
	SInt32	rightSide = mFirstRect.left + mImageWidth * totalCellWidth;
	SInt32	bottomSide = mFirstRect.top + mImageHeight * totalCellWidth;
	
	if ( (inPortPt.h >= mFirstRect.left) && (inPortPt.v >= mFirstRect.top) 
			&& (inPortPt.h < rightSide) && (inPortPt.v < bottomSide) )
	{
		mPaintView->AdjustCursorInCanvas( inPortPt, inMacEvent );
	}
	else
		UCursor::SetArrow();
}

/*===================================
	ClickSelf
====================================*/
void PTCanvas::ClickSelf(const SMouseDownEvent &inMouseDown)
{
	if ( mPaintView )
		mPaintView->HandleMouseDownInCanvas( inMouseDown );
}

/*===================================
	DrawSelf
====================================*/
void PTCanvas::DrawSelf()
{
	if ( mPaintView )
		mPaintView->HandleCanvasDraw();
}




/*===================================
	HasHotSpot
====================================*/
Boolean PTCanvas::HasHotSpot()
{
	if ( (mHotSpot.h == -1) || (mHotSpot.v == -1) )
		return( false );
	return( true );
}

/*===================================
	GetHotSpot
====================================*/
Point PTCanvas::GetHotSpot()
{
	return( mHotSpot );
}

/*===================================
	SetHotSpot
	
	Note:
	Set to -1,-1 to remove it -- this is never done, though.
	Does not redraw.
====================================*/
void PTCanvas::SetHotSpot( Point inHotSpot )
{
	if ( EqualPt( inHotSpot, mHotSpot ) )
		return;
	
	mHotSpot = inHotSpot;
}

/*===================================
	DrawHotSpot
====================================*/
void PTCanvas::DrawHotSpot( SUOffscreen *inSourceBuffer )
{
	StSaveGWorld		aSaver;
	Rect				r;
	
	if ( this->GetPixelRect( mHotSpot.h, mHotSpot.v, &r, false ) )
	{
		this->FocusDraw();
		Color32 theColor = inSourceBuffer->GetPixelColor( mHotSpot.h, mHotSpot.v );
		this->DrawHotSpotSelf( theColor, r );
	}
}

/*===================================
	DrawHotSpotSelf
====================================*/
void PTCanvas::DrawHotSpotSelf( Color32 inPixelColor, const Rect &inRect )
{
	/*************************************************
		make sure the pattern & mode are correct
	*************************************************/
	StColorPenState		thePenState;
	thePenState.Normalize();
	
	/*************************************************
		if the source pixel is light, draw the hot spot in black.
		if the source pixel is dark, draw the hot spot in white.
	*************************************************/
	HSLColor	pixelHSL;
	RGBColor	pixelRGB = SUColorUtils::Color32ToRGB( inPixelColor );
	::RGB2HSL( &pixelRGB, &pixelHSL );
	if ( pixelHSL.lightness > 0x8000 )
	{
		::RGBForeColor( &kBlackColor );
		::RGBBackColor( &kWhiteColor );
	}
	else
	{
		::RGBForeColor( &kWhiteColor );
		::RGBBackColor( &kBlackColor );
	}

	/*************************************************
		Draw the target
	*************************************************/
	Rect r = inRect;		// since we modify it
	
	if ( (r.right - r.left < 12) || (r.bottom - r.top < 12) )
	{
		::PenSize( 1, 1 );
		::FrameOval( &r );
		::InsetRect( &r, 2, 2 );
		::FrameOval( &r );
	}
	else
	{
		::PenSize( 2, 2 );
		::FrameOval( &r );
		::InsetRect( &r, 4, 4 );
		::FrameOval( &r );
	}
}

/*===================================
	GetPixelRect

	Description:
	Returns the rectangle for a given pixel, relative to either the superview
	or the pixelArea.  Does not include any boundary whitespace.	
====================================*/
Boolean PTCanvas::GetPixelRect( SInt32 inHoriz, SInt32 inVert, Rect *outRect, 
								Boolean inPixelAreaRelative )
{
	SetRect( outRect, 0, 0, 0, 0 );			// in case of error
	
	if ( (inHoriz < 0) || (inHoriz >= mImageWidth) ) return( false );
	if ( (inVert < 0 ) || (inVert >= mImageHeight) ) return( false );
	
	SInt32	totalCellWidth = mCellWidth + mSpaceBetween;
	
	*outRect = mFirstRect;
	::OffsetRect( outRect, inHoriz * totalCellWidth, inVert * totalCellWidth );
	
	if ( inPixelAreaRelative )
		::OffsetRect( outRect, kSideMargin-mPixelArea.left, kSideMargin-mPixelArea.top );
	
	return( true );
}

/*===================================
	MapPointToCell
	
	Parameters:
	inPoint is in local coordinates (0,0 at top/left of superview).
	
	Returns:
	true if point is within the canvas

	Notes:
	if point is outside the canvas, outH & outV will contain values outside
	  the canvas. this is useful for continuing to track the mouse while it's down.
====================================*/
Boolean PTCanvas::MapPointToCell( Point inPoint, SInt32 *outH, SInt32 *outV, Boolean roundNegsDown )
{
	SInt32	dh = inPoint.h - mFirstRect.left;
	SInt32	dv = inPoint.v - mFirstRect.top;
	SInt32	totalCellWidth = mCellWidth + mSpaceBetween;
	
	*outH = dh / totalCellWidth;
	*outV = dv / totalCellWidth;

	#ifdef MARQUEE_FIX_NO_GOOD_HERE
	if ( mSpaceBetween == 0 )
	{
		++*outH;
		++*outV;
	}
	#endif
	
	if ( roundNegsDown )
	{
			// negative pixels should round to the left, not to zero
			// (needed for PTSelectionAction to select the zero'th pixel)
		if ( dh < 0 )
			--*outH;
			
		if ( dv < 0 )
			--*outV;
	}
	
	return ( 	(inPoint.h >= mPixelArea.left) &&
				(inPoint.h <= mPixelArea.right) &&
				(inPoint.v >= mPixelArea.top) &&
				(inPoint.v <= mPixelArea.bottom) &&
				(*outH >= 0) && (*outH < mImageWidth) &&
				(*outV >= 0) && (*outV < mImageHeight) 
			);
}

/*===================================
	MapRectToCanvas

	Description:
	Given a rectangle in the source image (0,0-32,32 for icons), this
	  function returns a rectangle in the canvas that encloses those pixels.
	The area is in the white space between pixels (if there is any).
	
	Parameters:
	inRect	- relative to pixmap (0,0 is top left)
	outrect	- in PP local coords (relative to superview).
====================================*/
void PTCanvas::MapRectToCanvas( const Rect &inRect, Rect *outRect )
{
	ThrowIfNil_( outRect );

	if ( ::EmptyRect( &inRect ) )
	{
		::SetRect( outRect, 0, 0, 0, 0 );
		return;
	}
	
	SInt32	totalCellWidth = mCellWidth + mSpaceBetween;
	
	outRect->left = mFirstRect.left + inRect.left * totalCellWidth;
	outRect->right = mFirstRect.left + inRect.right * totalCellWidth;
	outRect->top = mFirstRect.top + inRect.top * totalCellWidth;
	outRect->bottom = mFirstRect.top + inRect.bottom * totalCellWidth;
	
		// we want the returned rect to be _outside_ the pixels
	--outRect->left;
	--outRect->top;
}

/*===================================
	MapRegionToCanvas

	Description:
	Returns a fat-bitted region mapped from the passed selection region.
	 
	Parameters:
	inRgn 	- relative to pixmap (0,0 is top left)
	outRgn	- in PP local coords (relative to the canvas since we're an LView).
====================================*/
void PTCanvas::MapRegionToCanvas( RgnHandle inRgn, RgnHandle outRgn )
{
	ThrowIfNil_( inRgn );
	ThrowIfNil_( outRgn );				// should never happen
	
	/**********************************************
		this is a hack, but MapRgn() always seemed to be one pixel
		off when we selected a rectangle using the PTSelectionAction
		class. It prevents flicker by *not* haveing the PTSelectionAction
		class erase it's final rectangle, but then the selection region
		must line up exactly and we were one pixel off.
	***********************************************/
#if PP_Target_Carbon
	if ( ::IsRegionRectangular(inRgn) )
#else
	if ( ::GetHandleSize( (Handle)inRgn ) == sizeof(Region) )		// just a rect?
#endif
	{
		Rect	inRect, outRect;
		
#if PP_Target_Carbon
		::GetRegionBounds(inRgn, &inRect);
#else
		inRect = (**inRgn).rgnBBox;
#endif
		this->MapRectToCanvas( inRect, &outRect );
		::RectRgn( outRgn, &outRect );
		return;
	}
	
	Rect		sourceR;
	::SetRect( &sourceR, 0, 0, mImageWidth, mImageHeight );

	::CopyRgn( inRgn, outRgn );
	::MapRgn( outRgn, &sourceR, &mPixelArea );
}



/*===================================
	DrawOnePixel
====================================*/
void PTCanvas::DrawOnePixel( SInt32 inHoriz, SInt32 inVert, SUOffscreen *inBuffer )
{
	Rect	r;
	
	if ( this->GetPixelRect( inHoriz, inVert, &r, false ) )
	{
		Color32		thePixelColor = inBuffer->GetPixelColor( inHoriz, inVert );
		
		this->FocusDraw();
		SUColorUtils::SetForeColor32( thePixelColor );
		::PaintRect( &r );	

		if ( (mHotSpot.h == inHoriz) && (mHotSpot.v == inVert) )
			this->DrawHotSpot( inBuffer );
	}
}

/*===================================
	GetInsideDrawingRect
	
	Used by the paint view to clip when drawing a selection area.
====================================*/
void PTCanvas::GetInsideDrawingRect( Rect *outRect )
{
	*outRect = mPixelArea;

	#ifdef MARQUEE_FIX
	/*
		1/27/97 ebs bug fix w/selection marquee in 1x mode
	*/
	if ( this->GetSpaceBetween() == 0 )
	{
		++outRect->right;
		++outRect->bottom;
	}
	#endif
}


/*===================================
	ItemIsAcceptable
====================================*/
Boolean PTCanvas::ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef )
{
	if ( mPaintView->GetLockFlag() )
		return( false );
	
	return( PTImageDragMixin::ItemIsAcceptable( inDragRef, inItemRef ) );
}

/*=====================================
	HandleDroppedPicture
======================================*/
void PTCanvas::HandleDroppedPicture( PicHandle inPict, DragReference inDragRef, DragAttributes inAttribs )
{
	if ( mPaintView )
	{
		SDragImageInfo	dragInfo;
		dragInfo.imageType = ImageType_Picture;
		dragInfo.data = (void*) inPict;
		dragInfo.dragRef = inDragRef;
		dragInfo.dragAttribs = inAttribs;
		
		mPaintView->ObeyCommand( cmd_DragImage, &dragInfo );	
	}
}

/*=====================================
	HandleDroppedOffscreen
======================================*/
void PTCanvas::HandleDroppedOffscreen( SUOffscreen *inOffscreen, DragReference inDragRef, DragAttributes inAttribs )
{
	if ( mPaintView )
	{
		SDragImageInfo	dragInfo;
		dragInfo.imageType = DragFlavor_Offscreen;
		dragInfo.data = (void*) inOffscreen;
		dragInfo.dragRef = inDragRef;
		dragInfo.dragAttribs = inAttribs;
		
		mPaintView->ObeyCommand( cmd_DragImage, &dragInfo );	
	}
}

/*=====================================
	Reinitialize
	
	Description:
	Use this method to change canvas settings.
======================================*/
void PTCanvas::Reinitialize( 	SUOffscreen *	inOffscreen, 
								ERedrawOptions 	inRedraw,
								PTResizeOptions	inResize )
{
	ThrowIfNil_( inOffscreen );
	ThrowIfNil_( mPaintView );
	
	Rect		localFrame;
	SInt32		numHGridlines, numVGridlines;
	SInt32		imageWidth = inOffscreen->GetWidth();
	SInt32		imageHeight = inOffscreen->GetHeight();

	/**************************************************
		these are new values for our instance variables
		
		(We change the real instance variables only if everything was successful
		 so we don't get caught in a funny state)
	**************************************************/
	SInt32			cellWidth, spaceBetween;
	Rect			pixelArea, firstRect;
	Boolean			showGridlines;
	
	/**************************************************
		find the cell width & space between rows/columns
	**************************************************/
	cellWidth = mPaintView->GetZoomFactor( imageWidth, imageHeight, &showGridlines );
	spaceBetween = showGridlines ? kSpaceBetween : 0;
	
	/**************************************************
		find the number of rows of white lines we need
	**************************************************/
	if ( spaceBetween == 0 )
	{
		numHGridlines = 0;
		numVGridlines = 0;
	}
	else
	{
		numHGridlines = imageHeight + 1;
		numVGridlines = imageWidth + 1;
	}
	
	CalcLocalFrameRect( localFrame );
	
	/**************************************************
		resize the pane if needed
	**************************************************/
	if ( inResize & PTResize_Canvas )
	{
		SInt32	neededWidth = (numVGridlines * spaceBetween) + (cellWidth * imageWidth);
		SInt32	neededHeight = (numHGridlines * spaceBetween) + (cellWidth * imageHeight);
		SInt32	currentWidth = localFrame.right - localFrame.left;
		SInt32	currentHeight = localFrame.bottom - localFrame.top;

	#ifdef MARQUEE_FIX
			// no gridline mode still requires one pixel of border space on each side
		if ( spaceBetween == 0 )
		{
			neededWidth += 2;
			neededHeight += 2;
		}
	#endif
	
		SInt32	dh = neededWidth - currentWidth;
		SInt32	dv = neededHeight - currentHeight;
		if ( (dh != 0) || (dv != 0) )
		{
			this->ResizeFrameBy( dh, dv, inRedraw == redraw_Later );

			if ( inResize & PTResize_MoveSampleBox )
				this->MoveSamplePanes( dh, dv, inRedraw == redraw_Later );

			if ( inResize & PTResize_Window )
			{
				LWindow *theWindow = dynamic_cast< LWindow * >( SUMiscUtils::GetTopView( this ) );
				if ( theWindow )
					SUPaneResizer::ResizeWindowIfNeeded( theWindow, PaintWindowMargin_h, PaintWindowMargin_v );
			}
			
			CalcLocalFrameRect( localFrame );			// this may have been changed
		}
	
	}
	
	
	/**************************************************
		calculate the new pixel location information
	**************************************************/
	pixelArea.left = localFrame.left;
	pixelArea.right = pixelArea.left + (numVGridlines * spaceBetween) + (cellWidth * imageWidth);
	pixelArea.top = localFrame.top;
	pixelArea.bottom = pixelArea.top + (numHGridlines * spaceBetween) + (cellWidth * imageHeight);
	
	/**************************************************
		the first pixel is drawn here
	**************************************************/
	firstRect.left = pixelArea.left + kSideMargin;
	firstRect.right = firstRect.left + cellWidth;
	firstRect.top = pixelArea.top + kSideMargin;
	firstRect.bottom = firstRect.top + cellWidth;

	/**************************************************
		everything was successful, so update our instance variables
	**************************************************/
	mCellWidth = cellWidth;
	mSpaceBetween = spaceBetween;
	mPixelArea = pixelArea;
	mFirstRect = firstRect;
	mImageWidth = imageWidth;
	mImageHeight = imageHeight;
	
	/**************************************************
		redraw as desired
	**************************************************/
	SUMiscUtils::RedrawPaneAsIndicated( this, inRedraw );
}

/*=====================================
	ResizeFrameBy
======================================*/
void PTCanvas::ResizeFrameBy( SInt16 dh, SInt16 dv, Boolean inRedraw )
{
		// call our parent class to resize the frame
	LView::ResizeFrameBy( dh, dv, inRedraw );

		// resize the GBox surrounding us
	LPane *theBox = SUMiscUtils::FindSibblingPaneByID( this, PaneID_BoxAroundCanvas );
	if ( theBox )
		theBox->ResizeFrameBy( dh, dv, inRedraw );
}

/*===================================
	MoveSamplePanes
====================================*/
void PTCanvas::MoveSamplePanes( SInt16 dh, SInt16 /*dv*/, Boolean inRedraw )
{
	LPane *samplePane = SUMiscUtils::FindSibblingPaneByID( this, PaneID_SampleWell );
	if ( samplePane )
		samplePane->MoveBy( dh, 0, inRedraw );

	#ifdef NOW_DONE_ABOVE
		// our window is probably too small now, so resize it
	LWindow *ourWindow = dynamic_cast< LWindow * >( SUMiscUtils::GetTopView( this ) );
	if ( ourWindow )
		SUPaneResizer::ResizeWindowIfNeeded( ourWindow, PaintWindowMargin_h, PaintWindowMargin_v );
	#endif
	
}

/*===================================
	DrawFrom
====================================*/
void PTCanvas::DrawFrom( SUOffscreen *inBuffer, const Rect *inSourceRect )
{
	StSaveGWorld		aSaver;
	Rect				sourceR, offscreenR, windowR, tempDestR;
	
	if ( !inBuffer ) return;
	
	/*******************************************
		get ready to draw
	********************************************/
	SInt32 	totalCellWidth = mCellWidth + mSpaceBetween;
	SInt32 	totalCellHeight = totalCellWidth;
	
	/*******************************************
		figure out where everything comes from and goes
	********************************************/
	
		// the source is 0,0 relative and _not_ fatbits
	if ( inSourceRect )
	{
			// make sure we pin sourceRect to the buffer (damn callers)
		sourceR.left = MAX( 0, inSourceRect->left );
		sourceR.right = MIN( mImageWidth, inSourceRect->right );
		sourceR.top = MAX( 0, inSourceRect->top );
		sourceR.bottom = MIN( mImageHeight, inSourceRect->bottom );
	}
	else
		::SetRect( &sourceR, 0, 0, mImageWidth, mImageHeight );
		
		// the offscreen is 0,0-relative and fatbits
	offscreenR.left = sourceR.left * totalCellWidth;
	offscreenR.right = sourceR.right * totalCellWidth + kSideMargin;
	offscreenR.top = sourceR.top * totalCellHeight;
	offscreenR.bottom = sourceR.bottom * totalCellHeight + kSideMargin;
	
		// the window area is port relative and fatbits
	windowR = offscreenR;
	::OffsetRect( &windowR, mPixelArea.left, mPixelArea.top );
	LocalToPortPoint( topLeft( windowR ) );
	LocalToPortPoint( botRight( windowR ) );
	

	/***********************************************
		If we're not drawing gridlines, we can just CopyBits directly
		to the screen. This is much faster than copying twice (once to
		our offscreen buffer, once to the screen).
	***********************************************/
	if ( mSpaceBetween == 0 )
	{
	#ifdef MARQUEE_FIX
			// make sure when we draw we overwrite the right/bottom pixel
			// since the drawing code is often used to overwrite the selection
			// marquee (1/27/97 ebs bug fix dealing with 1x mode and marquee)
		this->FocusDraw();
		::RGBForeColor( &kWhiteColor );
		::MoveTo( mPixelArea.right, mPixelArea.top );
		::Line( 0, mPixelArea.bottom - mPixelArea.top );
		::MoveTo( 0, mPixelArea.bottom );
		::Line( mPixelArea.right - mPixelArea.left, 0 );
	#endif
	
			// the window rect is one pixel too big (it includes the side margin)
		tempDestR = windowR;
		tempDestR.right -= kSideMargin;
		tempDestR.bottom -= kSideMargin;

		inBuffer->CopyTo( this->GetMacPort(), &tempDestR, &sourceR );

		if ( this->HasHotSpot() && ::PtInRect( mHotSpot, &sourceR ) )
			this->DrawHotSpot( inBuffer );
		return;
	}
	
	/***********************************************
		We are drawing gridlines, so we must copy the image into the
		big scratch buffer first. This stretches the image into fatbits.

		it's possible that our scratch buffer isn't big enough, so we
		might have to call CopyBits more than once.
	***********************************************/
	SInt32		scratchWidth = sBigScratchBuffer->GetWidth();
	SInt32		scratchHeight = sBigScratchBuffer->GetHeight();
	
	SInt32		sourceWidth = sourceR.right - sourceR.left;
	SInt32		sourceHeight = sourceR.bottom - sourceR.top;
	
	SInt32		destWidth = sourceWidth * totalCellWidth + kSideMargin;
	SInt32		destHeight = sourceHeight * totalCellHeight + kSideMargin;
	
	SInt32		hLoops = 1 + (destWidth - 1) / scratchWidth;
	SInt32		vLoops = 1 + (destHeight - 1) / scratchHeight;
	
	SInt32		destIncH = (destWidth - kSideMargin) / hLoops;
	SInt32		destIncV = (destHeight - kSideMargin) / vLoops;
	
	SInt32		sourceIncH = sourceWidth / hLoops;
	SInt32		sourceIncV = sourceHeight / vLoops;
	
	GrafPtr		macPort = this->GetMacPort();
	
	Rect		sourcePortion, destPortion;
	
	for ( SInt32 vCount = 0; vCount < vLoops; vCount++ )
	{
		sourcePortion.top = sourceR.top + vCount * sourceIncV;
		sourcePortion.bottom = MIN( sourceR.bottom, sourcePortion.top + sourceIncV );

		destPortion.top = windowR.top + vCount * destIncV;
		destPortion.bottom = MIN( windowR.bottom, destPortion.top + destIncV );
		
		for ( SInt32 hCount = 0; hCount < hLoops; hCount++ )
		{
			sourcePortion.left = sourceR.left + hCount * sourceIncH;
			sourcePortion.right = MIN( sourceR.right, sourcePortion.left + sourceIncH );

			destPortion.left = windowR.left + hCount * destIncH;
			destPortion.right = MIN( windowR.right, destPortion.left + destIncH );
			
			this->DrawPortionOfBuffer( inBuffer, sourcePortion, macPort, destPortion );
		}
	}

	#ifndef NEW_HOTSPOT_CODE
	/***********************************************
		Draw the hotspot.
	***********************************************/
	if ( this->HasHotSpot() && ::PtInRect( mHotSpot, &sourceR ) )
		this->DrawHotSpot( inBuffer );
	#endif
}

/*===================================
	DrawPortionOfBuffer
	
	Note: we actually draw 1 pixel of border outside of the inDestR parameter (right/bottom).
====================================*/
void PTCanvas::DrawPortionOfBuffer( SUOffscreen *inSource, const Rect &inSourceR, 
									GrafPtr inDest, const Rect &inDestR )
{
	SInt32 	totalCellWidth = mCellWidth + mSpaceBetween;
	SInt32 	totalCellHeight = totalCellWidth;
	SInt32	sourceWidth = inSourceR.right - inSourceR.left;
	SInt32	sourceHeight = inSourceR.bottom - inSourceR.top;
	
	Rect	bufferR;
	
	/***********************************************
		Expand the source image into the offscreen buffer
	***********************************************/
	bufferR.left = 0;
	bufferR.right = totalCellWidth * sourceWidth;
	bufferR.top = 0;
	bufferR.bottom = totalCellHeight * sourceHeight;
	
	sBigScratchBuffer->CopyFrom( inSource, &bufferR, &inSourceR );

	/***********************************************
		Draw the horizontal gridlines.
		notes:
		 (1) we are drawing into an offscreen, so draw relative to 0,0
		 (2) we draw one more gridline than pixel
	***********************************************/
	sBigScratchBuffer->BeginDrawing();
	
	RGBForeColor( &kWhiteColor );
	
	SInt32 v = bufferR.top;
	for ( SInt32 rowCount = inSourceR.top; rowCount <= inSourceR.bottom; rowCount++ )
	{
		::MoveTo( bufferR.left, v );
		::LineTo( bufferR.right, v );
		v += totalCellWidth;
	}
	
	/***********************************************
		Draw the vertical gridlines.
	***********************************************/
	SInt32 h = bufferR.left;
	for ( SInt32 colCount = inSourceR.left; colCount <= inSourceR.right; colCount++ )
	{
		::MoveTo( h, bufferR.top );
		::LineTo( h, bufferR.bottom );
		h += totalCellWidth;
	}

	/***********************************************
		Overlay the hotspot (if any)
	***********************************************/
	#ifdef NEW_HOTSPOT_CODE
	if ( (mHotSpot.h >= inSourceR.left) && (mHotSpot.h < inSourceR.right) &&
		 (mHotSpot.v >= inSourceR.top)  && (mHotSpot.v < inSourceR.bottom) )
	{
		Rect	hotSpotR;
		
			// get the rectangle and offset it to it hits our offscreen
			// buffer in the correct spot
		this->GetPixelRect( mHotSpot.h, mHotSpot.v, &hotSpotR, false );
		//::OffsetRect( &hotSpotR, -inDestR.left, -inDestR.top );
		
		Color32 theColor = inSource->GetPixelColor( mHotSpot.h, mHotSpot.v );
		this->DrawHotSpotSelf( theColor, hotSpotR );
	}
	#endif
			
	sBigScratchBuffer->EndDrawing();

	/***********************************************
		Copy the gridlined image to the screen.
		The one extra pixel in the source & destination is for the right/bottom gridline.
	***********************************************/
	++bufferR.right;
	++bufferR.bottom;
	Rect	tempDestR = inDestR;
	++tempDestR.right;
	++tempDestR.bottom;
	
	sBigScratchBuffer->CopyTo( inDest, &tempDestR, &bufferR );
}

/*===================================
	GetSpaceBetween
====================================*/
SInt32 PTCanvas::GetSpaceBetween()
{
	return( mSpaceBetween );
}

