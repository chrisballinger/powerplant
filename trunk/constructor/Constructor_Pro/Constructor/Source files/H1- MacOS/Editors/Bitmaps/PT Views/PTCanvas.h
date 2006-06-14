// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintCanvas.h

CLASSES:			PTCanvas

DESCRIPTION:		Main header file for PTCanvas

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.25

CHANGE HISTORY :

		96.08.01	ebs		single buffer rather than 1 per canvas
		96.04.25	ebs		Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

class SUOffscreen;
class PTPaintView;

#include "SUOffscreen.h"	
#include "PT_Types.h"
#include "PTImageDragMixin.h"


class PTCanvas : public LView, public PTImageDragMixin
{
	public:
		enum { class_ID = 'CANV' };
		
								PTCanvas();
								PTCanvas( LStream * );
		virtual					~PTCanvas();
								
		static PTCanvas *		CreatePaintCanvasStream( LStream * );
		virtual void			FinishCreateSelf();
		
		virtual void	 		Reinitialize( 
									SUOffscreen *	inOffscreen, 
								 	ERedrawOptions 	inRedraw = redraw_Later,
								 	PTResizeOptions	inResize = PTResize_Canvas );
								 	
		virtual void			DrawSelf();
		virtual void			DrawFrom( SUOffscreen *, const Rect *subArea = nil );
		
		virtual void			AdjustCursorSelf( Point inPortPt, const EventRecord &inMacEvent );
		virtual void			ClickSelf( const SMouseDownEvent & );
		
		virtual Boolean			GetPixelRect( SInt32 inHoriz, SInt32 inVert, Rect *, 
												Boolean inPixelAreaRelative = true );
		virtual void			GetInsideDrawingRect( Rect * );
		virtual SInt32			GetSpaceBetween();
		
		virtual Boolean			MapPointToCell( Point inPoint, SInt32 *outH, SInt32 *outV, 
												Boolean roundNegsDown = false);
		virtual void			DrawOnePixel( SInt32 inHoriz, SInt32 inVert, SUOffscreen * );

		virtual void			MapRectToCanvas( const Rect &, Rect *outRect );
		virtual void			MapRegionToCanvas( RgnHandle inRegion, RgnHandle outRgn );
		
		virtual void			SetHotSpot( Point pt );
		virtual Boolean			HasHotSpot();
		virtual Point			GetHotSpot();
		
		virtual void			ResizeFrameBy( SInt16 dh, SInt16 dv, Boolean inRedraw );
		virtual void			MoveSamplePanes( SInt16 dh, SInt16 dv, Boolean inRedraw );
		
			// from PTImageDragMixin
	//	virtual Boolean 		ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef );
		
	protected:
		PTPaintView *			mPaintView;					// parent view
		
		/*
			mCellWidth:		Cell width & height (in pixels), not including spacing
			mSpaceBetween:	Number of pixels in between rows & columns
			mImageWidth:	Width of source image (pixels)
			mImageHeight:	Height of source image (pixels)
		*/
		SInt32					mCellWidth;
		SInt32					mSpaceBetween;	
		SInt32					mImageWidth, mImageHeight;
		
		/*
			mPixelArea:		in PP "local" coords, relative to the containing view
							this is where we blit pixels to. Does not include top &
								left margins, but does include bottom & right ones
								(so all rows & columns are the same size)
								
			mFirstRect:		in PP local coords
							does not include spacing on any side
		*/
		Rect					mPixelArea;	
		Rect					mFirstRect;
		
			// this is -1,-1 for most images, but used for cursors
		Point					mHotSpot;

			// we keep a single buffer for all of the canvas panes		
		static SUOffscreen *	sBigScratchBuffer;	
		static SInt32			sNumWindows;
		

			// drag & drop
		virtual Boolean			ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef );
		virtual void			HandleDroppedPicture( PicHandle, DragReference, DragAttributes  );
		virtual void			HandleDroppedOffscreen( SUOffscreen *, DragReference, DragAttributes );

		virtual void			DrawHotSpot( SUOffscreen *inSourceBuffer );
		virtual void			DrawHotSpotSelf( Color32 inPixelColor, const Rect &inRect );
		
		virtual void			DrawPortionOfBuffer( 
									SUOffscreen *inSource, const Rect &inSourceR, 
									GrafPtr inDest, const Rect &inDestR );
	private:
		void					ClearInstanceVariables();		// called from within constructor
};


