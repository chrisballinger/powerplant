// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				LassoAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTLassoAction

DESCRIPTION:		Implementation file for class
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		97.02.16	ebs		Major changes to mousedown handling (marching ants, hit detection)
		96.08.04	ebs		Gray lines instead of black
		96.06.27	ebs		Removed down sample code (inSUOffscreen now)
		96.06.24	ebs		Changed class & file name for Constructor integration	
		96.04.23	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

/*
	Lasso Algorithm:
	 
	(1) User drags around the lasso tool (see below)
	(2) We gather up all of the points into an array
	(3) When the user releases the mouse, we inset this area in order to 
	    locate the object(s) the user has selected.
	    -- that is, we don't select pixels that match the background color
	    -- outside of the object(s) selected
	(4) We set the selection area to this region
   		See the "RestrictRegion" method below.
   		
   	Drag algorithm:
   	
   	Previously (up to CW11), we looked at the cell that was clicked on
   		or dragged over. This wasn't a great UI because the user would
   		click closer to one cell than another and the wrong was hilited.
   		
   	Now we keep track of which "node" (horiz/vert coordinate between the
   		pixels) was clicked on and which node is closest to the mouse at
   		any point in time. The user can "back-off" by tracing the path
   		backwards, so mistakes are easier to correct.
*/

/*
	Note: 2/16/97
	We can improve the look a little by making sure we draw the lasso
		lines on the white gridlines and then we don't have to xor
		except when we're in 1x mode (no gridlines). PTSelectionAction
		does this already.
		
	We can improve the code that traces the mouse backwards to the
		previous point (ie, erases one segment) by allowing any point 
		along the line to be cancelled rather than just to the previous 
		known point. The machines are so fast today, though, I don't think 
		this is necessary.
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "SUMarchingAnts.h"
#include "PTLassoAction.h"
#include "SURegionUtils.h"
#include "SUSparePort.h"

/*==================================
	Constructor
===================================*/
PTLassoAction::PTLassoAction( const SPaintAction &inAction ) : PTTrackingPaintAction( inAction )
{
	mAffectsUndoState = false;					// we're not undoable
	
	mNumPoints = 0;
	mPointArray = (Point *) NewPtr( sizeof( Point ) * (SInt32) kMaxPoints );
	ThrowIfMemFail_( mPointArray );
	
	mSnapTo.SetAngle( PTSnapTo::kDontSnap );		// lasso doesn't snap to angles
	
	mImageWidth = inAction.currentBuffer->GetWidth();
	mImageHeight = inAction.currentBuffer->GetHeight();
}

/*==================================
	Destructor
===================================*/
PTLassoAction::~PTLassoAction()
{
	if ( mPointArray )
		::DisposePtr( (Ptr) mPointArray );
}

/*==================================
	HandleMouseDown
===================================*/
void PTLassoAction::HandleMouseDown( const SMouseDownEvent &inEvent )
{
	SInt32	rowHit, colHit;
	
	PTCanvas *theCanvas = mSettings.theCanvas;
	
	/*****************************************
		if the user clicks within the current selection, we will 
		instantiate a move tool and kill ourselves.
	*****************************************/
	if ( theCanvas->MapPointToCell( inEvent.whereLocal, &colHit, &rowHit ) )
		if ( mSettings.theSelection->PointInSelection( colHit, rowHit ) )
		{
			CMoveAction	*theMover = new CMoveAction( mSettings );
			ThrowIfMemFail_( theMover );
			delete this;
			theMover->HandleMouseDown( inEvent );
			return;
		}
	
	/*****************************************
		otherwise just do the normal mouse tracking stuff
	*****************************************/
	PTTrackingPaintAction::HandleMouseDown( inEvent );
}

/*==================================
	MouseInitial
===================================*/
void PTLassoAction::MouseInitial( const SMouseDownEvent &inEvent, SInt32, SInt32 )
{
	PTCanvas *theCanvas = mSettings.theCanvas;
	
	/*****************************************
		find the cell width & space between cells
	*****************************************/
	theCanvas->GetPixelRect( 0, 0, &mTopLeftPixelRect, false );
	mCellWidth = mTopLeftPixelRect.right - mTopLeftPixelRect.left;
	mSpaceBetweenCells = theCanvas->GetSpaceBetween();
	mTotalCellWidth = mCellWidth + mSpaceBetweenCells;
	mUseXorMode = (mSettings.theCanvas->GetSpaceBetween() == 0);
	if ( mUseXorMode )
		mMarchingAnts.SetDelay( mMarchingAnts.GetDelay() * 2 );	// slow down in 1x mode (less flicker)

	/*****************************************
		prepare for below stuff
	*****************************************/
	mSettings.thePaintView->SelectNone();
	::PenNormal();
	::RGBForeColor( &kBlackColor );
	::RGBBackColor( &kWhiteColor );
	
	/*****************************************
		the first node always goes in the array
	*****************************************/
	SInt32	h, v;
	this->FindClosestNode( inEvent.whereLocal, &h, &v );
	this->AddPointToArray( h, v );
}

/*==================================
	MouseStillDown
===================================*/
void PTLassoAction::MouseStillDown( const SMouseDownEvent &, 
										Point, Point currentPt,
										SInt32, SInt32,
										SInt32, SInt32 )
{
	SInt32				h, v;
	Point				prevNode, prevNode2;
	
	/*****************************************
		prepare the port for drawing
	*****************************************/
	Rect				clipR;
	StColorPenState		aPenState;
	Pattern				thePattern;

	mSettings.theCanvas->GetInsideDrawingRect( &clipR );
	StClipRgnState	saveSetAndRestore( clipR );
	
	aPenState.Normalize();
	if ( mUseXorMode )
		::PenMode( patXor );
	mMarchingAnts.GetPattern( &thePattern );
	::PenPat( &thePattern );
	
	if ( mNumPoints <= 0 ) return;		// user missed the entire cell area on the 1st click
	
	/*****************************************
		find the closest node to where the mouse is
	*****************************************/
	this->FindClosestNode( currentPt, &h, &v );
	
	/*****************************************
		if the node is the same as the previous one, do nothing but animate (haven't moved)
	*****************************************/
	prevNode = mPointArray[ mNumPoints - 1 ];
	if ( (h == prevNode.h) && (v == prevNode.v) )
	{
		this->AnimateArray();
		return;
	}
	
	/*****************************************
		if the node is the same as the 2nd previous one, erase the previous line
	*****************************************/
	if ( mNumPoints > 1 )
	{
		prevNode2 = mPointArray[ mNumPoints - 2 ];
		if ( (h == prevNode2.h) && (v == prevNode2.v) )
		{
				// erase the segment
			if ( !mUseXorMode )
				::RGBForeColor( &kWhiteColor );
			this->DrawSegment( mNumPoints - 2 );
			::RGBForeColor( &kBlackColor );
	
			this->RemovePointFromArray();
			this->AnimateArray();
			return;
		}
	}
	
	/*****************************************
		add the new point to the array and draw the new segment
	*****************************************/
	this->AddPointToArray( h, v );
	this->DrawSegment( mNumPoints - 2 );
	this->AnimateArray();
	
	#ifdef OUTOUTOUT
	/*
		erase & draw here as needed ???
	*/
		if ( mNumPoints > 1 )
		{
			/*******************************************
				draw a single pixel line in the canvas from
				the previous point to the current point.
				The line needs to be between pixels.
			*******************************************/
			Rect	prevRect, newRect;
			
				// canvas is already focused
			if ( mSettings.theCanvas->GetPixelRect( newCol, newRow, &newRect ) &&
				 mSettings.theCanvas->GetPixelRect( prevCol, prevRow, &prevRect) )
			{
				::PenPat( &qd.dkGray );
				::MoveTo( prevRect.left, prevRect.top );
				::LineTo( newRect.left, newRect.top );
			}
		}
	#endif
}

#ifdef OLD_WAY_NOT_USED		// did it this way up until post CW11
/*==================================
	MouseStillDown
===================================*/
void PTLassoAction::MouseStillDown( const SMouseDownEvent &, 
										Point, Point,
										Int32 prevCol, Int32 prevRow,
										Int32 newCol, Int32 newRow )
{
	if ( (newCol == prevCol) && (newRow == prevRow) )
		return;
	
	if ( this->AddPointToArray( newCol, newRow ) )
		if ( mNumPoints > 1 )
		{
			/*******************************************
				draw a single pixel line in the canvas from
				the previous point to the current point.
				The line needs to be between pixels.
			*******************************************/
			Rect	prevRect, newRect;
			
				// canvas is already focused
			if ( mSettings.theCanvas->GetPixelRect( newCol, newRow, &newRect ) &&
				 mSettings.theCanvas->GetPixelRect( prevCol, prevRow, &prevRect) )
			{
				::PenPat( &qd.dkGray );
				::MoveTo( prevRect.left, prevRect.top );
				::LineTo( newRect.left, newRect.top );
			}
		}
}
#endif

/*==================================
	MouseFinal
===================================*/
Boolean PTLassoAction::MouseFinal( const SMouseDownEvent &, 
									Point, Point,
									SInt32, SInt32,
									SInt32, SInt32 )
{	
		// redraw the image (erase our scribblings)
	mSettings.thePaintView->HandleCanvasDraw();

		// convert the scribblings into a polygon
	RgnHandle	aRgn = this->ConvertArrayToRegion();
	if ( aRgn )
	{
			// inset the region to find the enclosed object(s)
		this->RestrictRegion( aRgn );

		if ( !::EmptyRgn( aRgn ) )
			mSettings.thePaintView->SetSelection( aRgn, true );
		::DisposeRgn( aRgn );
	}
	
	::PenNormal();					// prob don't want to leave gray as pattern
	return( false );				// not an undoable action
}

/*==================================
	AddPointToArray
	
	Now maps the point to the image bounds and always returns true
	unless the buffer is full.
===================================*/
Boolean PTLassoAction::AddPointToArray( SInt32 newCol, SInt32 newRow )
{
	if ( mNumPoints >= kMaxPoints - 1 ) return(false);		// sorry, we're full (reallocate ???)
															// note: save 1 slot to close the polygon
	
		// bounds check the incoming values because the mouse tracking 
		// returns #'s outside of the canvas
	newCol = MIN( newCol, mImageWidth );
	newCol = MAX( 0, newCol );
	
	newRow = MIN( newRow, mImageHeight );
	newRow = MAX( 0, newRow );
	
	Point pt;
	SetPt( &pt, newCol, newRow );
	mPointArray[ mNumPoints++ ] = pt;
	
	return( true );
}

/*==================================
	ConvertArrayToRegion
===================================*/
RgnHandle PTLassoAction::ConvertArrayToRegion()
{
	RgnHandle		outRegion = nil;
	StSaveGWorld	aSaver;
	
	/**********************************************************
		the last point in the array is the same as the first (close it)
	**********************************************************/
	this->AddPointToArray( mPointArray[0].h, mPointArray[0].v );

	/**********************************************************
		in order to be valid, we need at least 3 points and they
		may not be linear because there is no enclosed area.
	**********************************************************/
	if ( this->IsArrayLinear() ) return( nil );

	outRegion = SURegionUtils::NewRegion();	
	SUMiscUtils::SetPort( SUSparePort::GetSparePort() );
	
	::OpenRgn();
	::MoveTo( mPointArray[0].h, mPointArray[0].v );

	for ( long count = 1; count < mNumPoints; count++ )
		::LineTo( mPointArray[count].h, mPointArray[count].v );
	
	::CloseRgn( outRegion );		// check for errors ???
	
	return( outRegion );
}

/*==================================
	RestrictRegion
	
	Theory of operation:
	For the lasso to automatically "edge-sense" objects enclosed by it, 
	we do the following:
	
	(1) Copy the image into a scratch buffer, mapping all of the colors to
	    black except the current background color. We use the region the
	    user has sketched as a mask.
	(2) Call CalcCMask with a black seed value to calculate the enclosed
	    area of the bitmap.
	(3) Calling BitMapToRegion to create a region from the selection.
	
	Note that we can't just use CalcCMask with a custom color seach proc
	because we want *every* color to serve as seed value except the background,
	while CalcCMask assumes that only a single color will be the seed value.
===================================*/
void PTLassoAction::RestrictRegion( RgnHandle selectionRgn )
{
	StSaveGWorld	aSaver;					// save/restore gworld
	SInt32			width, height;
	Rect			r;
	SUOffscreen		*oneBitBuffer = nil;
	PixMapHandle	oneBitPixMap, scratchPixMap;
	OSErr			err = noErr;
	BitMap			aBitMap;

	try
	{
		/**********************************************
			Copy the current buffer to the scratch buffer, mapping
			all non-background pixels to black.
		***********************************************/
		this->CopyAndDownSample( mSettings.currentBuffer, mSettings.scratchBuffer, selectionRgn );
		
		/**********************************************
			Allocate a 1-bit buffer for CalcCMask
		***********************************************/
		width = mSettings.currentBuffer->GetWidth();
		height = mSettings.currentBuffer->GetHeight();
		
		oneBitBuffer = SUOffscreen::CreateBuffer( width, height, 1 );
		
			// lock all the pixels and pixmaps down
		oneBitPixMap = oneBitBuffer->GetPixMap();
		StHandleLocker		lock1( (Handle) oneBitPixMap );

		scratchPixMap = mSettings.scratchBuffer->GetPixMap();
		StHandleLocker		lock2( (Handle) scratchPixMap );
		
		/**********************************************
			CalcCMask is too stupid to take a pixmap as the 2nd parameter.
			We'll setup a bitmap structure and use it instead.
			(apparently, the hi bits of rowBytes screw it up 'cause it doesn't work)
		***********************************************/
		aBitMap.bounds = (**oneBitPixMap).bounds;
		aBitMap.baseAddr = (**oneBitPixMap).baseAddr;
		aBitMap.rowBytes = (**oneBitPixMap).rowBytes & 0x3FFF;
		
		/**********************************************
			Call CalcCMask to calculate the interior pixels
		***********************************************/
		RGBColor	seedRGB = { 0, 0, 0 };					// black
		SetRect( &r, 0, 0, width, height );
		::CalcCMask( (BitMap*) *scratchPixMap, &aBitMap,
					&r, &r, &seedRGB, nil, 0 );

		/**********************************************
			Convert this bitmap into a region (and return it to the caller)
		***********************************************/
		err = ::BitMapToRegion( selectionRgn, &aBitMap );
		ThrowIfOSErr_( err );
	}
	catch( ... )
	{
		if ( oneBitBuffer ) delete( oneBitBuffer );
		throw;
	}
	
	if ( oneBitBuffer ) delete( oneBitBuffer );
}

/*=========================================
	PTLassoAction::CopyAndDownSample
	
	Description:
	Copies the source buffer into the destination, but maps all pixels
	to black except where the source pixel is the background color.
==========================================*/
void PTLassoAction::CopyAndDownSample( SUOffscreen *source, SUOffscreen *dest, RgnHandle selectionRgn )
{
	StSaveGWorld		save1;
	
	/********************************************
		Copy the source to the destination, changing all non-background colors
		to black.
	********************************************/
	dest->CopyFromAndDownSample( source, mSettings.theBackColor, kBlackColor32 );

	/********************************************
		erase everything outside the selection region to white
	********************************************/
	SInt32	width = source->GetWidth();
	SInt32	height = source->GetHeight();

	RgnHandle	tempRgn = SURegionUtils::NewRegionFromRect( 0, 0, width, height );
	dest->BeginDrawing();
		dest->ResetColors();
		::DiffRgn( tempRgn, selectionRgn, tempRgn );
		::EraseRgn( tempRgn );
		::DisposeRgn( tempRgn );
	dest->EndDrawing();
}



/*==================================
	IsArrayLinear
	
	Description:
	Returns true if all of the points in the array are on the same line.
	This means that there is no enclosed area.
	
	Is there a better way of doing this??
===================================*/
Boolean PTLassoAction::IsArrayLinear()
{
	if ( mNumPoints < 3 ) 
		return( true );
	
		// get the slope for the first two points
	SInt32		dx, dy;
	double		origSlope, newSlope;
	Boolean		infiniteSlope = false;
	
	#define kCloseEnoughToZero		((double) 0.0005)		// for floating point equality test
	
	for ( long count = 0; count < mNumPoints - 1; count++ )
	{
		dx = mPointArray[ count+1 ].h - mPointArray[ count ].h;
		dy = mPointArray[ count+1 ].v - mPointArray[ count ].v;
		
		if ( count == 0 )
		{
			if ( dx == 0 )
				infiniteSlope = true;
			else
				origSlope = (double) dy / (double) dx;
		}
		else
		{
			if ( dx == 0 )
			{
				if ( !infiniteSlope )
					return( false );
			}
			else
			{
				if ( infiniteSlope )
					return( false );
					
				newSlope = (double) dy / (double) dx;
				if ( ABS( newSlope - origSlope ) > kCloseEnoughToZero )
					return( false );
			}
		}
	}
	
	return( true );
}


/*==================================
	DoIt
	
	Note:
	Normally, the lasso is usually invoked by dragging it around. This 
	routine is only called when the user double-clicks on the lasso tool 
	to "select all".  
	 
	The lasso "select all" is different from the rectangular "select all"
	in that we inset the selection to find the enclosed objects.
===================================*/
void PTLassoAction::DoIt()
{
	SInt32 width = 		mSettings.currentBuffer->GetWidth();
	SInt32 height = 		mSettings.currentBuffer->GetHeight();
	RgnHandle selRgn = 	SURegionUtils::NewRegionFromRect( 0, 0, width, height );
	
	mSettings.thePaintView->SelectNone();
	
	if ( selRgn )
	{
		this->RestrictRegion( selRgn );		// edge-detect the interior
		mSettings.thePaintView->SetSelection( selRgn, true );
		::DisposeRgn( selRgn );	
	}
	
	this->PostAsAction();			// will destroy this object since we're not undo-able
}

/*==================================
	FindClosestNode
	
	Returns the closest node (point between cells).
===================================*/
void PTLassoAction::FindClosestNode( const Point &inLocal, SInt32 *outH, SInt32 *outV )
{
	SInt32 halfWidth = mTotalCellWidth / 2;
	SInt32 dh = inLocal.h - mTopLeftPixelRect.left;
	SInt32 dv = inLocal.v - mTopLeftPixelRect.top;
	
	*outH = (dh + halfWidth) / mTotalCellWidth;
	*outV = (dv + halfWidth) / mTotalCellWidth;
}

/*==================================
	RemovePointFromArray
===================================*/
void PTLassoAction::RemovePointFromArray()
{
	if ( mNumPoints <= 1 ) return;		// never delete the starting point
	--mNumPoints;
}

/*==================================
	DrawSegment
	
	Note:
	Segments are numbered from 0..n-1
	Draws in current pen state, so this can be used to draw, erase, or xor
===================================*/
void PTLassoAction::DrawSegment( SInt32 inSegmentNo )
{
	if ( (inSegmentNo < 0) || (inSegmentNo >= mNumPoints - 1) ) return;
	
	Point	startPt = mPointArray[ inSegmentNo ];
	Point	stopPt = mPointArray[ inSegmentNo + 1 ];

	SInt32	h1 = (mTopLeftPixelRect.left - 1) + mTotalCellWidth * startPt.h;
	SInt32	v1 = (mTopLeftPixelRect.top - 1) + mTotalCellWidth * startPt.v;
	SInt32	h2 = (mTopLeftPixelRect.left - 1) + mTotalCellWidth * stopPt.h;
	SInt32	v2 = (mTopLeftPixelRect.top - 1) + mTotalCellWidth * stopPt.v;
	
	::MoveTo( h1, v1 );
	::LineTo( h2, v2 );
}

/*==================================
	DrawArray
===================================*/
void PTLassoAction::DrawArray()
{
	if ( mNumPoints <= 1 ) return;
	
	SInt32	segsToDraw = mNumPoints - 1;
	for ( SInt32 count = 0; count < segsToDraw; count++ )
		this->DrawSegment( count );
}

/*==================================
	AnimateArray
===================================*/
void PTLassoAction::AnimateArray()
{
	if ( mMarchingAnts.TimeToDraw() )
	{
		if ( mUseXorMode )
			this->DrawArray();				// erase if drawn
	
		mMarchingAnts.IncrementPattern();	
		Pattern	aPat;
		mMarchingAnts.GetPattern( &aPat );
		::PenPat( &aPat );
		this->DrawArray();					// draw the new one
	}
}
