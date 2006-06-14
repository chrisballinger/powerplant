// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				RectAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTRectAction

DESCRIPTION:		The rectangle drawing tool code is here.
 					See also OvalAction.cp and RoundRectAction.cp -- they use
 					  most of this code too.
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.08.27	ebs		Clicking in last row/col now works correctly
		96.08.02	ebs		Speed enhancement -- update only the screen area necessary
		96.04.23	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"

/*=====================================
	Constructor
======================================*/
PTRectAction::PTRectAction( const SPaintAction &inAction, Boolean inFill, ResIDT inUndoIndex ) : 
		PTTrackingPaintAction( inAction, inUndoIndex )
{
	this->ThrowIfFileLocked();

	mFilled = inFill;
	mScratchBufferHasImage = false;			// nothing drawn into scratch buffer yet
}

/*=====================================
	Destructor
======================================*/
PTRectAction::~PTRectAction()
{
}

/*=====================================
	PTRectAction::MouseStillDown
======================================*/
void PTRectAction::MouseStillDown( const SMouseDownEvent &, 
									Point /*prevMousePt*/, Point /*newMousePt*/,
									SInt32 prevCol, SInt32 prevRow,
									SInt32 newCol, SInt32 newRow )
{
	if ( (newCol == prevCol) && (newRow == prevRow) ) 
		return;

	SUOffscreen 	*theImageBuffer = mSettings.currentBuffer;
	SUOffscreen 	*theScratchBuffer = mSettings.scratchBuffer;
	Rect			r;
	
		// put the virgin image into the scratch buffer
	theScratchBuffer->CopyFrom( theImageBuffer );

		// modify the image buffer by drawing into it
	theScratchBuffer->BeginDrawing();
	
	theScratchBuffer->SetForeColor( mSettings.theForeColor );
	theScratchBuffer->SetBackColor( mSettings.theBackColor );
	
	r.top = MIN( mMouseDownRow, newRow );
	r.bottom = 1 + MAX( mMouseDownRow, newRow );
	r.left = MIN( mMouseDownCol, newCol );
	r.right = 1 + MAX( mMouseDownCol, newCol );
	
		// this will draw the rectangle, oval, or round-rect into the buffer
	this->DrawShape( r );
	
		// indicate that we've drawn something into the buffer
	mScratchBufferHasImage = true;	
	
		// draw the image on the screen
		// for speed, only update the required portion -- the union of the
		// new rectangle and the old one
	Rect	unionR;
	
	unionR.top = MIN( r.top, prevRow );
	unionR.bottom = MAX( r.bottom, prevRow + 1 );
	unionR.left = MIN( r.left, prevCol );
	unionR.right = MAX( r.right, prevCol + 1 );

	mSettings.theCanvas->DrawFrom( theScratchBuffer, &unionR );
	theScratchBuffer->EndDrawing();
}

/*=====================================
	PTRectAction::DrawShape
======================================*/
void PTRectAction::DrawShape( const Rect &r )
{
	if ( mFilled )
	{
		PenPat( &mSettings.thePattern );
		PaintRect( &r );
	}
	else
		FrameRect( &r );
}

/*=====================================
	PTRectAction::MouseFinal
======================================*/
Boolean PTRectAction::MouseFinal( const SMouseDownEvent &, 
								Point, Point,
								SInt32, SInt32,
								SInt32, SInt32 )
{
	if ( mScratchBufferHasImage )
	{
		mSettings.currentBuffer->CopyFrom( mSettings.scratchBuffer );
		return( true );				// can undo
	}
	
	return( false );				// can't undo
}

