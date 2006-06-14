// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				TransparentAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			CTransparentAction

DESCRIPTION:		Implementation file for class
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.4.23		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

/*
	The "Make Transparent" action removes every background-colored pixel
	from the current selection. This is especially useful when pasting
	an object into the paint view and you don't want to obscure everything
	beneath it.
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"
#include "SURegionUtils.h"

/*======================================
	CTransparentAction Constructor
=======================================*/
CTransparentAction::CTransparentAction( const SPaintAction &inAction )
					: PTAction( inAction, str_UndoTransparent )
{
	this->ThrowIfFileLocked();
};

/*======================================
	CTransparentAction::DoIt
=======================================*/
void CTransparentAction::DoIt()
{
	StSaveGWorld		aSaver;
	Rect				r;
	RgnHandle			oldSelectionRgn = nil, newSelectionRgn = nil;
	
	SUOffscreen			*selectionBuffer = mSettings.theSelection->GetImageBuffer();
	SUOffscreen			*scratchBuffer = mSettings.scratchBuffer;
	Boolean				deleteScratchBuffer = false;
	
	mSettings.thePaintView->CopyToUndo();

	try
	{
		SInt32	oldWidth = selectionBuffer->GetWidth();
		SInt32	oldHeight = selectionBuffer->GetHeight();
		SetRect( &r, 0, 0, oldWidth, oldHeight );
		
		/*******************************************
			get the background color as an Color32 value -- all compares are in RGB
			space because that's what the user sees as the color (there could be
			duplicate rgb values in the color table).
		********************************************/
		//selectionBuffer->IndexToRGB( mSettings.theBackColor, &theBackRGB );

		/*******************************************
			get the current selection region and make it 0,0 based
		********************************************/
		oldSelectionRgn = mSettings.theSelection->GetCopyOfRegion();
		Rect theRect;
#if PP_Target_Carbon
		::GetRegionBounds(oldSelectionRgn, &theRect);
#else
		theRect = (**oldSelectionRgn).rgnBBox;
#endif
		SInt32 hOffset = theRect.left;
		SInt32 vOffset = theRect.top;
		OffsetRgn( oldSelectionRgn, -hOffset, -vOffset );
	
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
		
		/*******************************************
			set each pixel in the scratch buffer to black or white.
			 white: pixels that are outside the selection or in the background color
			 black: pixels within the selection that are not the background color
		********************************************/
		scratchBuffer->ResetColors();
		scratchBuffer->EraseToWhite();
		
		PixelValue		blackPixelValue = scratchBuffer->RGBToPixelValue( kBlackColor );
		Color32			thePixelColor;
	
		for ( long rowCount = 0; rowCount < oldHeight; rowCount++ )
			for ( long colCount = 0; colCount < oldWidth; colCount++ )
			{
				thePixelColor = selectionBuffer->GetPixelColor( colCount, rowCount );
				
				if ( !SUColorUtils::EqualColor32( thePixelColor, mSettings.theBackColor ) )
				{
					Point	pt;
					::SetPt( &pt, colCount, rowCount );
					if ( ::PtInRgn( pt, oldSelectionRgn ) )		// do this faster way ???
						scratchBuffer->SetPixel( colCount, rowCount, blackPixelValue );
				}
			}
		
		/*******************************************
			convert the bitmap into the new selection region
		********************************************/
		newSelectionRgn = scratchBuffer->ConvertToRegion();
		
		/*******************************************
			need to copy the original image into the scratch buffer
			because selectionBuffer will be deleted by SetSelection
		********************************************/
		scratchBuffer->CopyFrom( selectionBuffer, &r, &r, srcCopy );
		
		mSettings.theSelection->SetSelection( scratchBuffer, newSelectionRgn );
		mSettings.theSelection->Offset( hOffset, vOffset );
		
			// redraw the screen & post this action		
		mSettings.thePaintView->HandleCanvasDraw();
		this->PostAsAction();
	}
	catch( ... )
	{
		if ( oldSelectionRgn ) ::DisposeRgn( oldSelectionRgn );
		if ( newSelectionRgn ) ::DisposeRgn( newSelectionRgn );
		if ( deleteScratchBuffer && scratchBuffer )	delete scratchBuffer;
			// delete "this" ????
		throw;
	}

	if ( oldSelectionRgn ) ::DisposeRgn( oldSelectionRgn );
	if ( newSelectionRgn ) ::DisposeRgn( newSelectionRgn );
	if ( deleteScratchBuffer && scratchBuffer )	delete scratchBuffer;
}


