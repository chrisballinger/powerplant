// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PenAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTPenAction

DESCRIPTION:		Implementation file for class
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.07.12	ebs		Converted to Color32
		96.04.23	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"

/*==================================
	Constructor
===================================*/
PTPenAction::PTPenAction( const SPaintAction &inAction ) : 
		PTTrackingPaintAction( inAction, str_UndoPen )
{
	this->ThrowIfFileLocked();
	mLastRowPainted = mLastColPainted = -1;			// nothing painted yet
}

/*==================================
	Destructor
===================================*/
PTPenAction::~PTPenAction()
{
}

/*==================================
	MouseInitial
	
	Notes:
	Determines whether we will be drawing in the foreground or background
	  color.
===================================*/
void PTPenAction::MouseInitial( const SMouseDownEvent &inEvent, SInt32 /*newCol*/, SInt32 /*newRow*/ )
{
	if ( inEvent.macEvent.modifiers & shiftKey )
		mSnapTo.SetAngle( PTSnapTo::kSnapTo90 );
	else
		mSnapTo.SetAngle( PTSnapTo::kDontSnap );

	/*
		if pen hits a different color, use the forecolor
		if pen hits the same color, use background color
	*/
	SUOffscreen		*theImageBuffer = mSettings.currentBuffer;
	Color32			colorHit = theImageBuffer->GetPixelColor( mMouseDownCol, mMouseDownRow );

		// do all compares in rgb space since 2 colors in table could be the same
	if ( SUColorUtils::EqualColor32( colorHit, mSettings.theForeColor ) )
		mPenColor = mSettings.theBackColor;
	else
		mPenColor = mSettings.theForeColor;

	mBufferCols = theImageBuffer->GetWidth();
	mBufferRows = theImageBuffer->GetHeight();
}

/*==================================
	MouseStillDown
===================================*/
void PTPenAction::MouseStillDown( const SMouseDownEvent &, 
											Point /*prevMousePt*/, Point /*newMousePt*/,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow )
{
	if ( (newCol == prevCol) && (newRow == prevRow) )
		return;
	
	SUOffscreen	*theImageBuffer = mSettings.currentBuffer;
	
	if ( (newCol < 0) || (newCol >= mBufferCols) ||
	     (newRow < 0) || (newRow >= mBufferRows) )			// offscreen ?
	{
		mLastRowPainted = -1;
		mLastColPainted = -1;
	}
	else
	{
		if ( (mLastRowPainted == -1) || (mLastColPainted == -1) )
		{
			mLastRowPainted = newRow;
			mLastColPainted = newCol;
		}

			// draw the pen stroke into the buffer (as a line)
		theImageBuffer->BeginDrawing();
		theImageBuffer->SetForeColor( mPenColor );
		//	::PmForeColor( mPenColorIndex );
			::MoveTo( mLastColPainted, mLastRowPainted );
			::LineTo( newCol, newRow );
		theImageBuffer->EndDrawing();
		
			// copy the buffer to the screen (only the portion drawn for speed)
		Rect	subRect;
		subRect.left = MIN( mLastColPainted, newCol );
		subRect.right = 1 + MAX( mLastColPainted, newCol );
		subRect.top = MIN( mLastRowPainted, newRow );
		subRect.bottom = 1 + MAX( mLastRowPainted, newRow );
		
		mSettings.theCanvas->DrawFrom( theImageBuffer, &subRect);

		mLastRowPainted = newRow;
		mLastColPainted = newCol;
	}
}

