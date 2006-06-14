// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				RotateAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTRotateAction

DESCRIPTION:		Implementation file for class
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.07.12	ebs		Change to Color32
		96.6.18		ebs		Fixed problem with new BitUtils. We downsample to 1-bit
							from higher depths properly using RGB values.
		96.4.23		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

/*
	Note:
	This code does only +/- 90 degree rotation. I started work on arbitrary
	rotation code but didn't finish it.
	
	Rotating a Region structure requires drawing it into a 1-bit buffer,
	rotating the pixels, and then calling BitMapToRegion() on the buffer.
	There isn't a better way to do this because the Region structure is
	not documented.  (Remember, the lasso selects non-rectangular areas)
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"
#include "SUColorUtils.h"


/*=====================================
	Constructor
======================================*/
PTRotateAction::PTRotateAction( const SPaintAction &inAction, SInt32 degrees )
				: PTAction( inAction, str_UndoRotate )
{
	this->ThrowIfFileLocked();
	mDegrees = degrees;
}

/*=====================================
	DoIt
======================================*/
void PTRotateAction::DoIt()
{
	SInt32			oldWidth, oldHeight;
	SInt32			newWidth, newHeight;
	Boolean			deleteScratchBuffer = false;
	StSaveGWorld	aSaver;
	
	SUOffscreen		*oldBuffer = mSettings.theSelection->GetImageBuffer();
	SUOffscreen		*scratchBuffer = mSettings.scratchBuffer;
	SUOffscreen		*newBuffer = nil, *maskBuffer = nil;
	
		// get a copy of the selection region, offset to 0,0
	RgnHandle		selectionRgn = mSettings.theSelection->GetCopyOfRegion();
	Rect			oldSelRect;
#if PP_Target_Carbon
	::GetRegionBounds(selectionRgn, &oldSelRect);
#else
	oldSelRect = (**selectionRgn).rgnBBox;
#endif
	::OffsetRgn( selectionRgn, -oldSelRect.left, -oldSelRect.top );		// 'cause it's relative to the paint view

		// copy the current image & selection area to the undo buffer
	mSettings.thePaintView->CopyToUndo();
	
	/************************************************
		Do the rotation
	************************************************/
	try
	{
		oldWidth = oldBuffer->GetWidth();
		oldHeight = oldBuffer->GetHeight();
		
			// since we only do 90 degree rotations...this calculation is easy
		newWidth = oldHeight;
		newHeight = oldWidth;
		
		/************************************************
			it's possible for someone to have pasted a selection that is larger
			than the image size. this means that our scratch buffer is too small
			for the rotate operations. we'll allocate a bigger scratch buffer if needed.
		************************************************/
		if ( (oldWidth > scratchBuffer->GetWidth()) || (oldHeight > scratchBuffer->GetHeight()) )
		{
			scratchBuffer = scratchBuffer->CreateSimilarOffscreen( false /* ??? */, oldWidth, oldHeight );
			deleteScratchBuffer = true;
		}
		
		/************************************************
			draw the selection region into the scratch buffer so we can rotate it
		************************************************/
		scratchBuffer->BeginDrawing();
			scratchBuffer->EraseToWhite();
			::PaintRgn( selectionRgn );
		scratchBuffer->EndDrawing();

			// don't need this one anymore
		::DisposeRgn( selectionRgn );
		selectionRgn = nil;
		
		/************************************************
			allocate a 1-bit deep mask buffer and rotate the selection into it
			(BitMapToRegion requires a 1-bit buffer)
		************************************************/
		maskBuffer = SUOffscreen::CreateBuffer( newWidth, newHeight, 1 );
		this->TransformTheBits( scratchBuffer, maskBuffer, oldWidth, oldHeight );
		
		/************************************************
			we no longer need our temporary scratch buffer, so get rid of it before we run out of ram
		************************************************/
		if ( deleteScratchBuffer && scratchBuffer )
		{
			delete scratchBuffer;
			scratchBuffer = nil;
		}
		
		/************************************************
			create the new selection region based on this mask
		************************************************/
		selectionRgn = maskBuffer->ConvertToRegion();
		
			// don't need this anymore -- we might need the memory below
		delete maskBuffer;
		maskBuffer = nil;

		/************************************************
			allocate a new selection buffer
		************************************************/
		newBuffer = oldBuffer->CreateSimilarOffscreen( false /* ??? */, newWidth, newHeight );
		this->TransformTheBits( oldBuffer, newBuffer, oldWidth, oldHeight );

		/************************************************
			change the current selection
			 keep the center point where it was before
		************************************************/
		mSettings.theSelection->SetSelection( newBuffer, selectionRgn );	// it will duplicate the offscreen here
		
		SInt32	oldSelCenterH = (oldSelRect.right + oldSelRect.left) / 2;
		SInt32	oldSelCenterV = (oldSelRect.top + oldSelRect.bottom) / 2;
		mSettings.theSelection->Offset( oldSelCenterH - newWidth/2, oldSelCenterV - newHeight / 2 );
		
			// if it's offscreen -- move it on????
	}
	catch( ... )
	{
		if ( newBuffer ) delete newBuffer;
		if ( maskBuffer ) delete maskBuffer;
		if ( deleteScratchBuffer && scratchBuffer )
			delete scratchBuffer;
		SUMiscUtils::DisposeHandle( (Handle) selectionRgn );
			// dispose of "this" here ???
		throw;
	}

	if ( newBuffer ) delete newBuffer;
	if ( maskBuffer ) delete maskBuffer;
	if ( deleteScratchBuffer && scratchBuffer )
		delete scratchBuffer;
	SUMiscUtils::DisposeHandle( (Handle) selectionRgn );

	mSettings.thePaintView->HandleCanvasDraw();
	this->PostAsAction();
}



/*=====================================
	TransformTheBits
	
	Note:
	This won't work if the destination is a different depth than the
	source since we use pixel indices rather than RGB values. We *will*
	work with 1-bit destinations, however, since we need this ability.
======================================*/
void PTRotateAction::TransformTheBits( SUOffscreen *source, SUOffscreen *dest, 
										SInt32 sourceWidth, SInt32 sourceHeight )
{
	SInt32				sourceRow, destRow, sourceCol, destCol;
	SInt32				destWidth = sourceHeight;
	SInt32				destHeight = sourceWidth;
	
	if ( source->GetDepth() != dest->GetDepth() )		// 1-bit case only
	{
		Color32		thePixelColor;
		
		if ( mDegrees == 90 )
		{
			for ( destRow = 0; destRow < destHeight; destRow++ )
			{
				sourceCol = sourceWidth - destRow - 1;
			
				for ( destCol = 0; destCol < destWidth; destCol++ )
				{
					sourceRow = destCol;
					thePixelColor = source->GetPixelColor( sourceCol, sourceRow );
					if ( !SUColorUtils::EqualColor32( thePixelColor, kWhiteColor32 ) )
						dest->SetPixel( destCol, destRow, 1 );
				}
			}
		}
		else if ( mDegrees == -90 )
		{
			for ( destRow = 0; destRow < destHeight; destRow++ )
			{
				sourceCol = destRow;
			
				for ( destCol = 0; destCol < destWidth; destCol++ )
				{
					sourceRow = sourceHeight - destCol - 1;
	
					thePixelColor = source->GetPixelColor( sourceCol, sourceRow );
					if ( !SUColorUtils::EqualColor32( thePixelColor, kWhiteColor32 ) )
						dest->SetPixel( destCol, destRow, 1 );
				}
			}
		}
	}
	else
	{
		PixelValue		thePixel;
	
		if ( mDegrees == 90 )
		{
			for ( destRow = 0; destRow < destHeight; destRow++ )
			{
				sourceCol = sourceWidth - destRow - 1;
			
				for ( destCol = 0; destCol < destWidth; destCol++ )
				{
					sourceRow = destCol;
					thePixel = source->GetPixel( sourceCol, sourceRow );
					dest->SetPixel( destCol, destRow, thePixel );
				}
			}
		}
		else if ( mDegrees == -90 )
		{
			for ( destRow = 0; destRow < destHeight; destRow++ )
			{
				sourceCol = destRow;
			
				for ( destCol = 0; destCol < destWidth; destCol++ )
				{
					sourceRow = sourceHeight - destCol - 1;
					thePixel = source->GetPixel( sourceCol, sourceRow );
					dest->SetPixel( destCol, destRow, thePixel );
				}
			}
		}
	}
}

