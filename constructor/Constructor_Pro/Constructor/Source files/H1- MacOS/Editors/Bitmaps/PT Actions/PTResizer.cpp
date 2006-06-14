// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.12.13	ebs		Added "delete mImageBuffer" to remove leak
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTResizer.h"
#include "SUMiscUtils.h"
#include "PTDraggableTargetBox.h"

/*==================================
	Constructor
===================================*/
PTResizer::PTResizer()
{
	mPaintView = nil;
	mImageBuffer = nil;
	mNumSamplePanes = 0;
}

/*==================================
	Initialize
===================================*/
void PTResizer::Initialize( PTPaintView *pv )
{
	/******************************
		keep track of the window size & position
	******************************/
	mPaintView = pv;
	SUMiscUtils::GetWindowGlobalBounds( GetWindowFromPort(pv->GetMacPort()), mWindowBounds );
	
	/******************************
		keep track of the original image buffer
	******************************/
	SUOffscreen *sourceBuffer = pv->GetImageBuffer();
	ThrowIfNil_( sourceBuffer );

	mImageBuffer = sourceBuffer->CreateSimilarOffscreen();
	mImageBuffer->CopyFrom( sourceBuffer );
	
	/******************************
		keep track of the sample well position & size
	******************************/
	this->GetPaneInfoByID( PaneID_SampleWell, false,  &mSampleWellInfo );
	
	/******************************
		keep track of all of the current sample panes
	******************************/
	mNumSamplePanes = pv->GetSamplePaneCount();
	for ( SInt32 count = 1; count <= mNumSamplePanes; count++ )
	{
		PTDraggableTargetBox	*aTarget = pv->GetNthSamplePane( count );
		ThrowIfNil_( aTarget );
		this->GetPaneInfo( (LPane*) aTarget, true, &mSamplePaneInfo[ count - 1 ] );
	}
}

/*==================================
	RestoreWindowState 
	
	Note:
	It is *very* important to allocate all of the buffers first, before
	irrevocably changing the paint view state. We don't want to leave 
	things in a weird state if we run out of memory.
===================================*/
void PTResizer::RestoreWindowState()
{
	if ( !mPaintView ) return;
	
	try
	{
		mPaintView->ImageSizeChangeUndone( mImageBuffer->GetWidth(), mImageBuffer->GetHeight() );
		
		/******************************
			this will change the scratch buffer, undo buffer, image buffer,
			canvas size, etc. everything but the sample pane size.
		******************************/
		mPaintView->SetImage( mImageBuffer, PTResize_Canvas );
		
		/******************************
			restore other panes to the saved state
		******************************/
		this->SetPaneInfo( mSampleWellInfo );

		for ( SInt32 count = 0; count < mNumSamplePanes; count++ )
			this->SetPaneInfo( mSamplePaneInfo[ count ] );

		/******************************
			resize the window itself
			note: user may have moved it, so resize it in its current place,
			not where it was originally
		******************************/
		Rect	r;
		SUMiscUtils::GetWindowGlobalBounds( GetWindowFromPort(mPaintView->GetMacPort()), r );
		r.right = r.left + (mWindowBounds.right - mWindowBounds.left);
		r.bottom = r.top + (mWindowBounds.bottom - mWindowBounds.top);
		mPaintView->DoSetBounds( r );
	}
	catch( ... )
	{
		// ??? anything else we need to do here?
		SUOffscreen *prevBuffer = mPaintView->GetImageBuffer();
		if ( prevBuffer )
			mPaintView->ImageSizeChangeUndone( prevBuffer->GetWidth(), prevBuffer->GetHeight() );
		throw;
	}

	/******************************
		and redraw everything
	******************************/
	mPaintView->Refresh();
}

/*==================================
	Destructor 
===================================*/
PTResizer::~PTResizer()
{
	if ( mImageBuffer )
	{
		delete mImageBuffer;
		mImageBuffer = nil;
	}
}

/*==================================
	DeleteAllBuffers
===================================*/
void PTResizer::DeleteAllBuffers()
{
	if ( mImageBuffer && (mPaintView->GetImageBuffer() != mImageBuffer) )	// can't hurt to check
	{
		delete mImageBuffer;
		mImageBuffer = nil;
	}
	
	for ( SInt32 count = 0; count < mNumSamplePanes; count++ )
	{
		/*
			some sample panes can't be resized, so their buffers may not
			have changed. don't delete the buffers if they are in use.
			
			??? check this for validity and memory leaks
		*/
		PTSavedPaneInfo &info = mSamplePaneInfo[ count ];
		if ( info.buffer )
		{
			PTDraggableTargetBox	*aTarget = dynamic_cast<PTDraggableTargetBox*>( info.thePane );
			if ( aTarget && (aTarget->GetBuffer() != info.buffer) )
				delete info.buffer;
		}
	}
}

/*==================================
	GetPaneInfoByID
===================================*/
void PTResizer::GetPaneInfoByID( PaneIDT inPaneID, Boolean isTargetBox, PTSavedPaneInfo *outInfo )
{
	this->GetPaneInfo( mPaintView->FindPaneByID( inPaneID ), isTargetBox, outInfo );
}

/*==================================
	GetPaneInfo
===================================*/
void PTResizer::GetPaneInfo( LPane *inPane, Boolean isTargetBox, PTSavedPaneInfo *outInfo )
{
	outInfo->thePane = inPane;
	if ( !inPane ) return;				// no sample well in the PICT editor, for example
	
	SDimension16	frameSize;
	SPoint32		frameLocation;
	
	inPane->GetFrameSize( frameSize );
	inPane->GetFrameLocation( frameLocation );
	
	outInfo->top = frameLocation.v;
	outInfo->left = frameLocation.h;
	outInfo->width = frameSize.width;
	outInfo->height = frameSize.height;
	
	if ( isTargetBox )
	{
		PTDraggableTargetBox *theTarget = dynamic_cast<PTDraggableTargetBox*>( inPane );
		ThrowIfNil_( theTarget );
		
		outInfo->buffer = theTarget->GetBuffer();
	}
	else
		outInfo->buffer = nil;
}

/*==================================
	SetPaneInfo
===================================*/
void PTResizer::SetPaneInfo( PTSavedPaneInfo &info )
{
	if ( !info.thePane ) return;		// no sample well in the PICT editor
	
	/********************************************
		resize and move the pane back where it belongs
	********************************************/
	info.thePane->ResizeFrameTo( info.width, info.height, false );
	
	SPoint32	frameLocation;
	info.thePane->GetFrameLocation( frameLocation );
	info.thePane->MoveBy( info.left - frameLocation.h, info.top - frameLocation.v, false );

	/********************************************
		set the buffer if it's a target pane
	********************************************/
	if ( info.buffer )
	{
		PTDraggableTargetBox *theTarget = dynamic_cast<PTDraggableTargetBox*>( info.thePane );
		ThrowIfNil_( theTarget );
		theTarget->SetRawBuffer( info.buffer );
	}
}


