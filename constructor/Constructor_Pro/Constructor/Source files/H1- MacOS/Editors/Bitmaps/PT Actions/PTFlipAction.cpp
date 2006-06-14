// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				FlipAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			CTransformAction, CFlipHorizontalAction, CFlipVerticalAction

DESCRIPTION:		Implementation file for classes
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.4.23		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

/*
	Note:
	Flipping a Region structure requires drawing it into a 1-bit buffer,
	flipping the pixels, and then calling BitMapToRegion() on the buffer.
	There isn't a better way to do this because the Region structure is
	not documented. (Remember, the lasso isn't rectangular, so we can't
	assume the region doesn't change).
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"


/*======================================
	CTransformAction Constructor
=======================================*/
CTransformAction::CTransformAction( const SPaintAction &inAction, SInt16 inStringIndex )
					: PTAction( inAction, inStringIndex )
{
	this->ThrowIfFileLocked();
}

/*======================================
	CTransformAction::DoIt
=======================================*/
void CTransformAction::DoIt()
{
	RgnHandle		newSelectionRgn = nil;
	StSaveGWorld	aSaver;
	
	try
	{
			// flip the selection region and store into newSelectionRgn
		newSelectionRgn = this->CalcNewSelectionRegion();
		if ( !newSelectionRgn ) return;
		
			// copy the current image & selection rgn to the undo area
		mSettings.thePaintView->CopyToUndo();
		
			// transform the bits in the image selection buffer
		this->TransformTheBits( mSettings.theSelection->GetImageBuffer() );
	
			// change the selection region
			// note: we are just changing the region and bypassing the paintview
			//       rather than storing new pixels in the selection buffer
		if ( newSelectionRgn )
			mSettings.theSelection->SetRawSelection( newSelectionRgn );
		
			// redraw the canvas (erases old selection box, etc)
		mSettings.thePaintView->HandleCanvasDraw();
		
			// make sure user can undo us
		this->PostAsAction();
	}
	catch(...)
	{
		if ( newSelectionRgn )
			::DisposeRgn( newSelectionRgn );
		throw;
	}
		
	if ( newSelectionRgn )
		::DisposeRgn( newSelectionRgn );
}

/*======================================
	CTransformAction::CalcNewSelectionRegion
=======================================*/
RgnHandle CTransformAction::CalcNewSelectionRegion()
{
	SInt32			width, height;
	SInt32			hOffset, vOffset;
	SUOffscreen		*oneBitBuffer = nil;
	RgnHandle		origRgn = nil, maskRgn = nil;
	
	if ( mSettings.theSelection->IsEmpty() ) return( nil );		// shouldn't happen
	
	origRgn = mSettings.theSelection->GetCopyOfRegion();

	try
	{
		Rect theRect;
		
#if PP_Target_Carbon
		::GetRegionBounds(origRgn, &theRect);
#else
		theRect = (**origRgn).rgnBBox;
#endif
		hOffset = theRect.left;
		vOffset = theRect.top;
		OffsetRgn( origRgn, -hOffset, -vOffset );	// 'cause it's relative to paintview, not selection
	
#if PP_Target_Carbon
		::GetRegionBounds(origRgn, &theRect);
#else
		theRect = (**origRgn).rgnBBox;
#endif
		width = theRect.right;
		height = theRect.bottom;
		
			// draw the selection region into a 1-bit deep buffer
		oneBitBuffer = SUOffscreen::CreateBuffer( width, height, 1 );
		oneBitBuffer->BeginDrawing();
			PaintRgn( origRgn );
		oneBitBuffer->EndDrawing();
		
			// transform the buffer just like it was the image buffer
		this->TransformTheBits( oneBitBuffer );
		
			// convert the bitmap to a region
		maskRgn = oneBitBuffer->ConvertToRegion();
		
			// put the offset back the way it was before the xform
		OffsetRgn( maskRgn, hOffset, vOffset );
	}
	catch( ... )
	{
		if ( oneBitBuffer ) delete( oneBitBuffer );
		if ( origRgn ) ::DisposeRgn( origRgn );
		if ( maskRgn ) ::DisposeRgn( maskRgn );
		throw;
	}
	
	if ( oneBitBuffer ) delete( oneBitBuffer );
	if ( origRgn ) ::DisposeRgn( origRgn );
	return( maskRgn );
}


// ---------------------- Horizontal ------------------------

/*======================================
	CFlipHorizontalAction Constructor
=======================================*/
CFlipHorizontalAction::CFlipHorizontalAction( const SPaintAction &inAction )
						: CTransformAction( inAction, str_UndoFlip )
{
}

/*======================================
	CFlipHorizontalAction::TransformTheBits
=======================================*/
void CFlipHorizontalAction::TransformTheBits( SUOffscreen *theBuffer )
{
	Color32				leftPixel, rightPixel;
	SInt32				colsToSwap;
	SInt32				leftCol, rightCol;
	
	SInt32				bufferWidth = theBuffer->GetWidth();
	SInt32				bufferHeight = theBuffer->GetHeight();
	
		// the column counter only has to loop for 1/2 the pixels since
		// we swap 2 pixels with each loop. If the number is odd, the
		// middle pixel doesn't change
	colsToSwap = bufferWidth / 2;

	for ( long rowCount = 0; rowCount < bufferHeight; rowCount++ )
	{
		for ( 	leftCol = 0, rightCol = bufferWidth - 1; 
				leftCol < colsToSwap; 
				leftCol++, rightCol-- )
		{
			leftPixel = theBuffer->GetPixel( leftCol, rowCount );
			rightPixel = theBuffer->GetPixel( rightCol, rowCount );
			theBuffer->SetPixel( leftCol, rowCount, rightPixel );
			theBuffer->SetPixel( rightCol, rowCount, leftPixel );
		}
	}
}



// ---------------------- Vertical ------------------------

/*======================================
	CFlipVerticalAction Constructor
=======================================*/
CFlipVerticalAction::CFlipVerticalAction( const SPaintAction &inAction )
						: CTransformAction( inAction, str_UndoFlip )
{
}

/*======================================
	CFlipVerticalAction::TransformTheBits
=======================================*/
void CFlipVerticalAction::TransformTheBits( SUOffscreen *theBuffer )
{
	Color32				topPixel, bottomPixel;
	SInt32				rowsToSwap;
	SInt32				topRow, bottomRow;
	
	SInt32				bufferWidth = theBuffer->GetWidth();
	SInt32				bufferHeight = theBuffer->GetHeight();
	
		// the row counter only has to loop for 1/2 the pixels since
		// we swap 2 pixels with each loop. If the number is odd, the
		// middle row doesn't change
	rowsToSwap = bufferHeight / 2;

	for ( 	topRow = 0, bottomRow = bufferHeight - 1;
			topRow < rowsToSwap;
			topRow++, bottomRow-- )
	{
		for ( long colCount = 0; colCount < bufferWidth; colCount++ )
		{
			topPixel = theBuffer->GetPixel( colCount, topRow );
			bottomPixel = theBuffer->GetPixel( colCount, bottomRow );
			theBuffer->SetPixel( colCount, topRow, bottomPixel );
			theBuffer->SetPixel( colCount, bottomRow, topPixel );
		}
	}
}
