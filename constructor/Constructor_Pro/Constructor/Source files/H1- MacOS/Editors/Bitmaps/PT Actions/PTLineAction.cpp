// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				LineAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			CLineAction

DESCRIPTION:		Implementation file for class
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
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
	CLineAction Constructor
======================================*/
CLineAction::CLineAction( const SPaintAction &inAction ) : 
		PTTrackingPaintAction( inAction, str_UndoLine )
{
	this->ThrowIfFileLocked();
	mScratchBufferHasImage = false;				// no image drawn to scratch buffer yet
	mSnapTo.SetAngle( PTSnapTo::kSnapTo45 );		// snap to 45 deg angle if shift key is down
}

/*=====================================
	CLineAction Destructor
======================================*/
CLineAction::~CLineAction()
{
}

/*=====================================
	Min3
======================================*/
static SInt32 Min3( SInt32 a, SInt32 b, SInt32 c )
{
	if ( a < b )
		return( (a < c) ? a : c );
	else
		return( (b < c) ? b : c );
}

/*=====================================
	Max3
======================================*/
static SInt32 Max3( SInt32 a, SInt32 b, SInt32 c )
{
	if ( a > b )
		return( (a > c) ? a : c );
	else
		return( (b > c) ? b : c );
}


/*=====================================
	CLineAction::MouseStillDown
======================================*/
void CLineAction::MouseStillDown( const SMouseDownEvent &, 
									Point /*prevMousePt*/, Point /*newMousePt*/,
									SInt32 prevCol, SInt32 prevRow,
									SInt32 newCol, SInt32 newRow )
{
	if ( (newCol == prevCol) && (newRow == prevRow) ) 
		return;

	SUOffscreen *theImageBuffer = mSettings.currentBuffer;
	SUOffscreen *theScratchBuffer = mSettings.scratchBuffer;

		// put the virgin image into the scratch buffer
	theScratchBuffer->CopyFrom( theImageBuffer );

		// modify the buffer by drawing into it
	theScratchBuffer->BeginDrawing();
	
	::PenNormal();
	
	theScratchBuffer->SetForeColor( mSettings.theForeColor );
	theScratchBuffer->SetBackColor( mSettings.theBackColor );
	
	::MoveTo( mMouseDownCol, mMouseDownRow );
	::LineTo( newCol, newRow );
	
		// draw the image on the screen -- only the changed area for speed
	Rect	updateR;
	updateR.left = Min3( mMouseDownCol, newCol, prevCol );
	updateR.right = 1 + Max3( mMouseDownCol, newCol, prevCol );
	updateR.top = Min3( mMouseDownRow, newRow, prevRow );
	updateR.bottom = 1 + Max3( mMouseDownRow, newRow, prevRow );
	
	mSettings.theCanvas->DrawFrom( theScratchBuffer, &updateR );
	mScratchBufferHasImage = true;
	
	theScratchBuffer->EndDrawing();
}

/*=====================================
	CLineAction::MouseFinal
======================================*/
Boolean CLineAction::MouseFinal( const SMouseDownEvent &, 
								Point, Point,
								SInt32, SInt32,
								SInt32, SInt32 )
{
	if ( mScratchBufferHasImage )
	{
		mSettings.currentBuffer->CopyFrom( mSettings.scratchBuffer );
		return( true );		// can undo
	}
	
	return( false );		// can't undo since no line was drawn
}


