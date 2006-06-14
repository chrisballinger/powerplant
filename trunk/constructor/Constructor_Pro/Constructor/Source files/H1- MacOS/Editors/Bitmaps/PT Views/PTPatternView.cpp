// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PTPatternView.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTPatternView

DESCRIPTION:		This code is for painting Pattern (PAT, ppat) resources
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.07.08

CHANGE HISTORY :

		97.02.13	ebs		Set mPrefersIconColors to false in constructor
		96.11.26	ebs		Add include RFResource.h (CW11)
		96.08.08	ebs		Resizing fixed
		96.08.07	ebs		Resizing put back in
		96.08.05	ebs		Support for mSamplePaneList
		96.07.31	ebs		Started resizing work
		96.07.22	ebs		New target boxes
		96.07.19	ebs		Save code
		96.07.18	ebs		Move to 32-bit buffers
		96.07.08	ebs		Started

---------------------------------------<< ¥ >>-----------------------------------------
*/

#include "PT_Headers.h"
#include "PTPatternView.h"
#include "PTPatternTargetBox.h"
#include "SUSaveResFile.h"
#include "SUPaneResizer.h"
#include "SUColorUtils.h"
#include "SUColorTableBuilder.h"
#include "RFMap.h"
#include "RFResource.h"

/*====================================
	Constants
=====================================*/
const SInt32	DefaultPatternWidth 	= 	8;
const SInt32	DefaultPatternHeight 	= 	8;
const SInt32	BWPatternRowBytes		=	1;
const PaneIDT	PaneID_BWSample			=	'BWPT';		// 'PAT ' is already in use by pattern popup

/*====================================
	OpenPaintWindow
=====================================*/
PTPatternView* PTPatternView::OpenPaintWindow( ResIDT inPPobID, RFMap *inMap, ResType inResType, ResIDT inResID )
{
	PTPatternView		*theView = nil;

	try
	{
		theView = (PTPatternView*) PTPatternView::CreatePaintWindow( inPPobID );
		theView->InitializeFromResource( inMap, inResType, inResID );

		#ifdef ICON_STAND_ALONE
		theView->Show();
		#endif
	}
	catch( ... )
	{
		delete theView;
		throw;
	}
	
	return( theView );
}

/*===============================================
	CreateFromStream
================================================*/
PTPatternView *PTPatternView::CreateFromStream( LStream *inStream )
{
	return new PTPatternView( inStream );
}

/*===============================================
	Constructor
================================================*/
PTPatternView::PTPatternView( LStream *inStream ) : PTPaintView( inStream )
{
	mColorSample = nil;
	mBWSample = nil;
	mResourceType = 0;		// don't have this yet
	mPixPatWidth = 0;
	mPixPatHeight = 0;
	mPrefersIconColors = false;
	
	/*
		note:
		the canvas overlaps the b&w pattern if we set the minimum
		width/height to less than 8x8. but since ResEdit has an
		8x8 minimum anyway and nobody is likely to care, we'll 
		have one too.
	*/
	mMinImageWidth = 8;
	mMinImageHeight = 8;
}

/*===============================================
	Destructor
================================================*/
PTPatternView::~PTPatternView()
{
		// close the file ???
}

/*===============================================
	FinishCreateSelf
================================================*/
void PTPatternView::FinishCreateSelf()
{
	PTPaintView::FinishCreateSelf();

	mBWSample = (PTPatternTargetBox*) this->FindPaneByID( PaneID_BWSample );
	ThrowIfNil_( mBWSample );
	mSamplePaneList[ mNumSamplePanes++ ] = mBWSample;
	
	mColorSample = (PTPatternTargetBox*) this->FindPaneByID( ImageType_PixPat );
	if ( mColorSample )
		mSamplePaneList[ mNumSamplePanes++ ] = mColorSample;

	if ( mSampleWell )
		mSampleWell->GetFrameSize( mInitialSampleWellSize );
	else
	{
		mInitialSampleWellSize.width = 0;
		mInitialSampleWellSize.height = 0;
	}

	this->BecomeListenerTo( mNumSamplePanes, mSamplePaneList );
}

/*===============================================
	InitializeFromResource
================================================*/
void PTPatternView::InitializeFromResource( RFMap *inMap, ResType inResType, ResIDT inResID )
{
	StSaveGWorld		aSaver;
	StSaveResFile		aSaver2;

	SUOffscreen			*colorImage = nil, *bwImage = nil;
	Handle				h = nil;
	
	try
	{
		mResourceType = inResType;
	
		/**************************************************
			get the raw resource handle
		**************************************************/
		RFResource *theRes = inMap->FindResource( inResType, inResID, false );
		ThrowIfNil_( theRes );
		h = theRes->GetResData();
		ThrowIfNil_( h );
		
		/**************************************************
			draw the pattern(s) into bitmaps
		**************************************************/
		switch( inResType )
		{
			case ImageType_PixPat:
				this->ParseColorPattern( h, &colorImage, &bwImage );
				mAllowImageResizing = true;
				break;
			case ImageType_Pattern:
				this->ParseBWPattern( h, &bwImage );
				break;
			default:
				LException::Throw( err_UnknownDataType );
		}
		
		/**************************************************
			resize the sample well if the color pattern is bigger than 8x8
		**************************************************/
		if ( colorImage )
		{
			SInt32	width = colorImage->GetWidth();
			SInt32	height = colorImage->GetHeight();
			
				// save these _now_ because SetImage() will trigger a call to
				// GetZoomFactor()
			mPixPatWidth = width;
			mPixPatHeight = height;
			
			this->ResizeSampleWell( width, height );
			
			this->SetImage( colorImage, PTResize_All, redraw_Later );

			mColorSample->SetBuffer( colorImage, redraw_Dont );
			colorImage = nil;		// because it belongs to the sample pane now
			mColorSample->SetTarget( true, redraw_Dont );
			
			mCurrentSamplePane = mColorSample;
		}
		else
		{
			this->SetImage( bwImage, PTResize_All, redraw_Later );
			mCurrentSamplePane = mBWSample;	
			mBWSample->SetTarget( true, redraw_Dont );
		}
		
		mBWSample->SetBuffer( bwImage, redraw_Dont );
		bwImage = nil;			// because it belongs to the sample pane now
	}
	catch( ... )
	{
		delete( colorImage );
		delete( bwImage );
		SUMiscUtils::DisposeHandle( h );
		throw;
	}
	
	SUMiscUtils::DisposeHandle( h );
}

/*==================================
	ResizeSampleWell
	
	Note:
	Only the color pattern size can change.
===================================*/
void PTPatternView::ResizeSampleWell( SInt32 inPatternWidth, SInt32 inPatternHeight )
{
	if ( mResourceType != ImageType_PixPat ) return;

	SDimension16	initialColorSize;
	SDimension16	minColorSize;
	SInt32			dh = inPatternWidth - 8;		// since samples are sized for 8+8 initially
	SInt32			dv = inPatternHeight - 8;

	mColorSample->GetInitialPaneSize( &initialColorSize );
	mColorSample->GetMinPaneSize( &minColorSize );

	/************************************************
		sample panes grow in a 1:1 ratio with the icon, but never
		grow smaller than a certain size.
	*************************************************/
	SInt32 			newPaneWidth = MAX( minColorSize.width, initialColorSize.width + 2 * dh );
	SInt32			newPaneHeight = MAX( minColorSize.height, initialColorSize.height + 2 * dv );
	
	mColorSample->ResizeFrameTo( newPaneWidth, newPaneHeight, true );
	
		// otherwise boxes overlap...
	if ( dv < 0 )
		dv = 0;
		
		// otherwise the right side gets cutoff on small cicns
	if ( dh < 0 )
		dh = 0;

	/************************************************
		the b&w must be moved down so it doesn't overlap the color one
	*************************************************/
	SPoint32		initialLoc, currentLoc;
	mBWSample->GetInitialPaneLocation( &initialLoc );
	mBWSample->GetFrameLocation( currentLoc );
	mBWSample->MoveBy( 0, initialLoc.v + 2 * dv - currentLoc.v, true );

	/************************************************
		the sample well contains both samples and needs to be resized
		accordingly. Never allow it to get tiny or things may get cut-off. 
		The numbers should probably be read from a resource???
	*************************************************/
	if ( mSampleWell )
		mSampleWell->ResizeFrameTo( MAX( 50, mInitialSampleWellSize.width + 2 * dh ),
									MAX( 140, mInitialSampleWellSize.height + 3 * dv),
									true );
}

/*==================================
	SaveAsResource
===================================*/
void PTPatternView::SaveAsResource( RFMap *inMap, ResIDT inResID  )
{
	Handle		h = nil;
	
	ThrowIfNil_( mBWSample );
	SUOffscreen	*bwBuffer = mBWSample->GetBuffer();
	
	try
	{
		switch( mResourceType )
		{
			case ImageType_Pattern:
				h = this->CreateBWPattern( bwBuffer );
				break;
			
			case ImageType_PixPat:
				ThrowIfNil_( mColorSample );
				h = this->CreateColorPattern( mColorSample->GetBuffer(), bwBuffer );
				break;
			
			default:
				LException::Throw( err_InvalidImageFormat );
		}

		RFResource *theResource = inMap->FindResource( mResourceType, inResID, true );
		ThrowIfNil_( theResource );
		theResource->SetResData( h );
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( h );
		throw;
	}
	
	SUMiscUtils::DisposeHandle( h );
	this->SetChangedFlag( false );
}

/*==================================
	ParseColorPattern
===================================*/
void PTPatternView::ParseColorPattern( Handle inPattern, SUOffscreen **outColor,
												SUOffscreen **outBW )
{
	// See IM V-79 for details

	StSaveGWorld		save1;
	StHandleLocker		save2( inPattern );
	PixPatPtr			p = (PixPatPtr) *inPattern;
	CTabHandle			theTable = nil;
	SUOffscreen			*tempColorBuffer = nil, *colorBuffer = nil, *bwBuffer = nil;

	try
	{	
		if ( p->patType != 1 )
			LException::Throw( err_InvalidImageFormat );
			
		/*******************************************
			first handle the b&w pattern
		********************************************/
		bwBuffer = this->BWPatternToOffscreen( p->pat1Data );

		/*******************************************
			now the color pattern
		********************************************/
		PixMapPtr			theMap = (PixMapPtr)( *inPattern + (SInt32) p->patMap );
		UInt8 *				theData = (UInt8*)( *inPattern + (SInt32) p->patData );
		
		SInt32				depth = theMap->pixelSize;
		SInt32				width = theMap->bounds.right - theMap->bounds.left;
		SInt32				height = theMap->bounds.bottom - theMap->bounds.top;
		SInt32				rowBytes = theMap->rowBytes & 0x3FFF;		// clear flags
		
		if ( (depth != 1) && (depth != 2) && (depth != 4) && (depth != 8) )
			LException::Throw( err_InvalidImageDepth );
		
		if ( (width <= 3) || (width > 64) || (height <= 3) || (height > 64) )
			LException::Throw( err_InvalidImageSize );
		
		theTable = SUColorUtils::NewColorTableFromPtr( depth, (UInt8*)( *inPattern + (SInt32)theMap->pmTable) );
		tempColorBuffer = SUOffscreen::CreateBuffer( width, height, depth, theTable );
		tempColorBuffer->CopyFromRawData( theData, rowBytes );

		// now switch to a 32-bit buffer
		colorBuffer = SUOffscreen::CreateBuffer( width, height, 32 );
		colorBuffer->CopyFrom( tempColorBuffer );
	}
	catch( ... )
	{
		delete colorBuffer;
		delete bwBuffer;
		delete tempColorBuffer;
		if ( theTable )
			::DisposeCTable( theTable );
		throw;
	}
	
	*outColor = colorBuffer;
	*outBW = bwBuffer;

	if ( theTable )
		::DisposeCTable( theTable );
	delete tempColorBuffer;
}

/*==================================
	PatternToOffscreen
===================================*/
SUOffscreen *PTPatternView::BWPatternToOffscreen( const Pattern &inPattern )
{
	StSaveGWorld	save1;
	CTabHandle		theTable = nil;
	SUOffscreen		*theBuffer = nil;
	
	try
	{
		theTable = SUColorUtils::GetColorTable( 1 );
		
		theBuffer = SUOffscreen::CreateBuffer( DefaultPatternWidth, DefaultPatternHeight, 1, theTable );
		//theBuffer->AssignPalette( theTable );		
		
		theBuffer->BeginDrawing();
			Rect		r;
			::SetRect( &r, 0, 0, DefaultPatternWidth, DefaultPatternHeight );
			::PenPat( &inPattern );
			::PaintRect( &r );
			::PenNormal();
		theBuffer->EndDrawing();
	}
	catch( ... )
	{
		if ( theBuffer ) delete( theBuffer );
		if ( theTable ) ::DisposeCTable( theTable );
		throw;
	}
	
	if ( theTable ) ::DisposeCTable( theTable );
	return( theBuffer );
}

/*==================================
	ParseBWPattern
===================================*/
void PTPatternView::ParseBWPattern( Handle inPattern, SUOffscreen **outBW  )
{
	Pattern		thePattern = **(Pattern**) inPattern;
	*outBW = this->BWPatternToOffscreen( thePattern );
}

/*==================================
	CreateBWPattern
===================================*/
Handle PTPatternView::CreateBWPattern( SUOffscreen *inBuffer )
{
	Handle h = ::NewHandle( sizeof( Pattern ) );
	ThrowIfMemFail_( h );
	
	StHandleLocker	lock( h );
	inBuffer->CopyToRawData( (UInt8*) *h, BWPatternRowBytes );
	return( h );
}

/*==================================
	CreateColorPattern
===================================*/
Handle PTPatternView::CreateColorPattern( SUOffscreen *inColor, SUOffscreen *inBW )
{
	Handle			h = nil;
	Boolean			deleteBuffer = false;

	try
	{
		/**********************************
			downsample the color buffer to a more reasonable depth
		**********************************/
		if ( inColor->GetDepth() >= 16 )
		{
			inColor = SUColorTableBuilder::DownSampleBuffer( 
											inColor, 
											8,				// max depth
											false, 			// don't force b&w into table
											true, 			// minimize table
											true );			// use larger inverse table
			ThrowIfNil_( inColor );							// shouldn't happen
			deleteBuffer = true;							// since we created it
		}
		
		/**********************************
			Get the color table from the color buffer
		**********************************/
		CTabHandle	sourceTable = inColor->GetColorTable();
		ThrowIfNil_( sourceTable );
		SInt32		colorTableBytes = ::GetHandleSize( (Handle) sourceTable );

		/**********************************
			do some calculations
		**********************************/
		SInt32		depth = inColor->GetDepth();
		SInt32		width = inColor->GetWidth();
		SInt32		height = inColor->GetHeight();
	
		SInt32		pixelRowBytes = SUColorUtils::CalcRowBytes( width, depth );
		SInt32		pixelBytes = pixelRowBytes * height;
		SInt32		totalBytes = sizeof( PixPat ) + sizeof( PixMap ) + colorTableBytes + pixelBytes;

		SInt32		offsetToPixmap = sizeof(PixPat);
		SInt32		offsetToPixelData = sizeof(PixPat) + sizeof(PixMap);
		SInt32		offsetToColorTable = sizeof(PixPat) + sizeof(PixMap) + pixelBytes;

		/**********************************
			Allocate the pixpat
		**********************************/
		h = ::NewHandleClear( totalBytes ); 
		ThrowIfMemFail_( h );
		::HLock( h );
		
		/**********************************
			Fill up the structure (note: fields are already zeroed)
		**********************************/
		PixPatPtr	p = (PixPatPtr) *h;
		PixMapPtr	theMap = (PixMapPtr)( *h + offsetToPixmap );
		CTabPtr		destTable = (CTabPtr)( *h + offsetToColorTable );
		
		p->patType = 1;
		p->patMap = (PixMapHandle) offsetToPixmap;
		p->patData = (Handle) offsetToPixelData;
		p->patXValid = -1;
		
		theMap->rowBytes = 0x8000 + pixelRowBytes;
		::SetRect( &theMap->bounds, 0, 0, width, height );
		theMap->hRes = 0x00480000;
		theMap->vRes = 0x00480000;
		theMap->pixelSize = depth;
		theMap->cmpCount = 1;
		theMap->cmpSize = depth;
		theMap->pmTable = (CTabHandle) offsetToColorTable;
		
		inBW->CopyToRawData( (UInt8*) &p->pat1Data, BWPatternRowBytes );
		inColor->CopyToRawData( (UInt8*) (*h + offsetToPixelData), pixelRowBytes );
		::BlockMoveData( *sourceTable, destTable, colorTableBytes );
		destTable->ctFlags = 0;					// 0 for pixmaps, 0x8000 for devices
	}
	catch( ... )
	{
		if ( deleteBuffer && inColor )
			delete inColor;
		SUMiscUtils::DisposeHandle( h );
		throw;
	}
	
	if ( deleteBuffer && inColor )
		delete inColor;
	::HUnlock( h );
	return( h );
}



/*====================================
	GetZoomFactor

	Description:
	Patterns are always zoomed for the biggest image -- either the b&w
	  pattern or the color one.
	We don't want to change zoom factors everytime the user clicks on
	  a different target box.
=====================================*/
SInt32 PTPatternView::GetZoomFactor( SInt32, SInt32, Boolean *outShowGrid )
{
	return PTPaintView::GetZoomFactor( MAX(mPixPatWidth, DefaultPatternWidth), 
									   MAX(mPixPatHeight, DefaultPatternHeight), 
									   outShowGrid );
}

/*===============================================
	ChangeImageSize
	
	Note:
	Affects only the pixpat, _not_ the black & white pattern.
================================================*/
void PTPatternView::ChangeImageSize( SInt32 inWidth, SInt32 inHeight, Boolean inStretch )
{
	ThrowIf_( (inWidth < mMinImageWidth) || (inHeight < mMinImageHeight) );
	ThrowIf_( (inWidth > mMaxImageWidth) || (inHeight > mMaxImageHeight) );
	ThrowIf_( mResourceType != ImageType_PixPat );
	ThrowIfNil_( mColorSample );
	ThrowIfNil_( mCurrentImage );
	
	SInt32			savedWidth = mPixPatWidth;		// in case of error...
	SInt32			savedHeight = mPixPatHeight;
	Boolean			changedSize = true;
	SUOffscreen		*newBuffer = nil;
	Rect			destR;
	
	try
	{
		/*****************************************
			create a new buffer -- same depth as current one, but new size
		*****************************************/
		newBuffer = mCurrentImage->CreateSimilarOffscreen( false, inWidth, inHeight );
		
		/*****************************************
			copy the image from the old buffer into the new one
		*****************************************/
		if ( inStretch )
			::SetRect( &destR, 0, 0, inWidth, inHeight );
		else
			::SetRect( &destR, 0, 0, mCurrentImage->GetWidth(), mCurrentImage->GetHeight() );
		
		newBuffer->CopyFrom( mCurrentImage, &destR );
		
		/*****************************************
			resize the pixpat sample pane
		*****************************************/
		this->ResizeSampleWell( inWidth, inHeight );
		changedSize = true;
		
		/*****************************************
			keep track of the changed size. do this before SetImage() because it
			may call GetZoomFactor.
		*****************************************/
		mPixPatWidth = inWidth;
		mPixPatHeight = inHeight;

		/*****************************************
			set the current buffer
			 -- this can trigger all sorts of things (canvas resize, etc)
		*****************************************/
		this->SetImage( newBuffer, PTResize_All, redraw_Later );
		
		/*****************************************
			change the sample pane's buffer.
			Notes:
			 (1) The sample pane will own the buffer after this call (don't delete it)
			 (2) The previous sample pane buffer will be saved away by the PTResizer object
		*****************************************/
		mColorSample->SetRawBuffer( newBuffer );
	}
	catch( ... )
	{
		/*
			restore things to the way they were before we were called
		*/
		mPixPatWidth = savedWidth;
		mPixPatHeight = savedHeight;

		if ( changedSize )
			this->ResizeSampleWell( mPixPatWidth, mPixPatHeight );
	
		delete newBuffer;
		throw;
	}
}

/*===============================================
	ImageSizeChangeUndone
	
	Note:
	The panes and image will be restored automatically, we just need to keep
	track of the image size.
================================================*/
void PTPatternView::ImageSizeChangeUndone( SInt32 inWidth, SInt32 inHeight )
{
	mPixPatWidth = inWidth;
	mPixPatHeight = inHeight;
}

/*=======================================
	FindCommandStatus
	
	Description:
	We override this because we only allow the pixpat to be resized,
	  not the b&w pattern.
=======================================*/
void PTPatternView::FindCommandStatus( CommandT inCommand, Boolean &outEnabled, 
									Boolean &outUsesMark, UInt16 &outMark, 
									Str255 outName )
{
	if ( inCommand != cmd_ResizeImage )
	{
		PTPaintView::FindCommandStatus( inCommand, outEnabled, outUsesMark, outMark, outName );
		return;
	}
	
	outUsesMark = false;

	if ( (mResourceType != ImageType_PixPat) || (this->GetTargetBox() == mBWSample) )
		outEnabled = false;
	else
		outEnabled = true;
}
