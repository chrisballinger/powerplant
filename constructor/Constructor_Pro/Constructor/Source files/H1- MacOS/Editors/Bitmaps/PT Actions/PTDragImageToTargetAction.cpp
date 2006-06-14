// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		96.08.28	ebs		Support for locked files
		96.08.08	ebs		Fixed redraw problem when you drag from a large sample (ppat)
							to a small sample (bw pat). The old canvas area wasn't updated.
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTDragImageToTargetAction.h"
#include "PTGraphicConversions.h"

/*=================================
	Constructor
==================================*/
PTDragImageToTargetAction::PTDragImageToTargetAction( 
								const SPaintAction &inAction, 
								SImageDropOnTargetBox *inDropInfo )
	: PTAction( inAction, str_UndoDrag )
{
	mDropInfo = *inDropInfo;
}

/*=================================
	Destructor
==================================*/
PTDragImageToTargetAction::~PTDragImageToTargetAction()
{
}

/*=================================
	DoIt
	
	Note:
	This doesn't fully undo -- undoing would involve a two-step process
	of undoing the new image *and* reverting the previous image as well.
	Maybe fix this eventually?  2.0feature
	
	This will deallocate any pictures passed in, but not offscreen buffers
	because they are probably in use within the editor.
	
	We not delete the picture or passed offscreen buffer -- this is
	handled by the drag & drop code.
==================================*/
void PTDragImageToTargetAction::DoIt()
{
	PTPaintView 			*thePaintView = mSettings.thePaintView;
	PTDraggableTargetBox 	*oldBox = thePaintView->GetTargetBox();
	PTDraggableTargetBox	*newBox = mDropInfo.thePane;
	SUOffscreen				*downSampledBuffer = nil, *sourceBuffer = nil;
	Boolean					deleteSourceBuffer = false;
	
	try
	{
		if ( thePaintView->GetLockFlag() )
			Throw_( err_FileLocked );
			
		/*************************************************
			if a picture was dropped, draw it into a buffer
		**************************************************/
		if ( mDropInfo.theBuffer )
			sourceBuffer = mDropInfo.theBuffer;
		else
		{
			sourceBuffer = PTGraphicConversions::PictureToOffscreen( mDropInfo.thePicture );
			deleteSourceBuffer = true;
		}
		
		/*************************************************
			allocate a buffer with the same depth & color table as
			the pane the image was dropped on. this will map colors
			to the pane's depth & color table, plus scale the image.
		**************************************************/
		SUOffscreen *paneBuffer = mDropInfo.thePane->GetBuffer();
		ThrowIfNil_( paneBuffer );
		downSampledBuffer = paneBuffer->CreateSimilarOffscreen();
		
		if ( newBox->IsMask() )
			this->GenerateMask( sourceBuffer, downSampledBuffer );
		else
			downSampledBuffer->CopyFrom( sourceBuffer );
	
		/*************************************************
			commit the current selection
			note: this can't be undone. is this ok?
		**************************************************/
		thePaintView->SelectNone();				// commit the current selection
		thePaintView->PostAction( nil );		// in case of error -- can't undo anything right now

		/*************************************************
			change the current sample pane
			-- this is needed up here in order to use CopyToUndo
			
			note:
			this may change the current buffer, so don't use mSettings.currentBuffer, etc
			after this point.
		**************************************************/
		if ( newBox != oldBox )
			thePaintView->SetTargetBox( mDropInfo.thePane, redraw_Later );
		
		thePaintView->CopyToUndo();
	
		/*************************************************
			copy to the main image
			note: mSettings.currentBuffer can't be used (see note above).
		**************************************************/
		SUOffscreen *latestImageBuffer = mSettings.thePaintView->GetImageBuffer();
		if ( latestImageBuffer )
			latestImageBuffer->CopyFrom( downSampledBuffer );
		
		#ifdef NOT_NEEDED
		/*************************************************
			this erases the old marching ants (if any) as well 
			as displaying the new selection
		**************************************************/
		thePaintView->HandleCanvasDraw();
		#endif
		
	
		/*************************************************
			do a refresh rather than a draw here because the canvas
			might've changed size and we needed to post an update
			event anyway. this prevents drawing twice.
		**************************************************/
		mSettings.theCanvas->Refresh();
		
		/*************************************************
			and we're done
		**************************************************/
		this->PostAsAction();
	}
	catch (const LException& iErr)
	{
		if ( deleteSourceBuffer ) delete sourceBuffer;
		delete downSampledBuffer;
		
			// we can't display errors during drag & drop, so we'll 
			// postpone it until idle time
		SUErrors::DisplayPostponedError( iErr.GetErrorCode(), nil );
	}
	catch( long inErr )
	{
		if ( deleteSourceBuffer ) delete sourceBuffer;
		delete downSampledBuffer;
		
			// we can't display errors during drag & drop, so we'll 
			// postpone it until idle time
		SUErrors::DisplayPostponedError( inErr, nil );
	}
	catch( ... )
	{
			// somebody threw something we didn't understand...
		SUErrors::DisplayPostponedError( err_Generic, nil );
	}
	
	if ( deleteSourceBuffer ) delete sourceBuffer;
	delete downSampledBuffer;
}


/*=================================
	GenerateMask
==================================*/
void PTDragImageToTargetAction::GenerateMask(	SUOffscreen *inSource, 
												SUOffscreen *inDest )
{
	SInt32			sourceWidth = inSource->GetWidth();
	SInt32			sourceHeight = inSource->GetHeight();
	SInt32			destWidth = inDest->GetWidth();
	SInt32			destHeight = inDest->GetHeight();
	
	Boolean			createdTempBuffer = false;
	SUOffscreen		*sourceBuffer;
	BitMap			aBitMap;
	
	/**********************************************
		if the incoming buffer isn't 1-bit or the correct size, create a temp 
		buffer that is more useful to us.
		
		note that rather than just using CopyBits(), this will map all colors
		other than the background color to black.
	***********************************************/
	if ( (inSource->GetDepth() != 1) || (sourceWidth != destWidth) || (sourceHeight != destHeight) )
	{
		sourceBuffer = SUOffscreen::CreateBuffer( destWidth, destHeight, 1 );
		createdTempBuffer = true;
		sourceBuffer->CopyFromAndDownSample( inSource, kWhiteColor32, kBlackColor32 );
	}
	else
		sourceBuffer = inSource;
	
	/*
		note: the extra braces below are needed because StHandleLocker
		will restore the handle flags and the buffer _may_ be deleted in
		the code just below this block.
	*/
	{
		/**********************************************
			Get the pixmaps and lock them and their bits
		***********************************************/
		PixMapHandle		sourceMap = sourceBuffer->GetPixMap();
		StHandleLocker		lock1( (Handle) sourceMap );

		PixMapHandle		destMap = inDest->GetPixMap();
		StHandleLocker		lock2( (Handle) destMap );

		/**********************************************
			CalcCMask is too stupid to take a pixmap as the 2nd parameter.
			We'll setup a bitmap structure and use it instead.
			(apparently, the hi bits of rowBytes screw it up 'cause it doesn't work)
		***********************************************/
		aBitMap.bounds = (**destMap).bounds;
		aBitMap.baseAddr = ::GetPixBaseAddr( destMap );
		aBitMap.rowBytes = (**destMap).rowBytes & 0x3FFF;

		/**********************************************
			Call CalcCMask to calculate the interior pixels
		***********************************************/
		RGBColor	seedRGB = { 0, 0, 0 };					// black
		Rect		r;
		SetRect( &r, 0, 0, destWidth, destHeight );
		::CalcCMask( (BitMap*) *sourceMap, &aBitMap, &r, &r, &seedRGB, nil, 0 );
	}
	
	/**********************************************
		Cleanup
	***********************************************/
	if ( createdTempBuffer && sourceBuffer )
		delete sourceBuffer;
}

