// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintCIconView.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTCIconView

DESCRIPTION:		This code is for painting cicn resources
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.5.23

CHANGE HISTORY :

		96.11.26	ebs		Add include RFResource.h (CW11)
		96.08.08	ebs		Resize fixed for very small cicns
		96.08.07	ebs		Resizing support
		96.08.05	ebs		Support for mSamplePaneList, PTPrefs
		96.08.03	ebs		Support for "unused" flag in target panes
		96.07.31	ebs		Started resizing work
		96.07.22	ebs		New target boxes
		96.07.16	ebs		Support for 32-bit pixmaps
		96.05.23	ebs		Started

---------------------------------------<< ¥ >>-----------------------------------------
*/

#include "PT_Headers.h"
#include "PTDraggableTargetBox.h"
#include "PTCIconView.h"

#include "SUSaveResFile.h"
#include "SUColorUtils.h"
#include "SUFileUtils.h"
#include "SUPaneResizer.h"
#include "SUColorTableBuilder.h"
#include "PTPrefs.h"

#include "RFMap.h"
#include "RFResource.h"

/*====================================
	Constants
=====================================*/
const PaneIDT	PaneID_Mask				= 'MASK';

/*====================================
	OpenPaintWindow
=====================================*/
PTCIconView* PTCIconView::OpenPaintWindow( ResIDT inPPobID, RFMap *inMap, ResIDT inResID )
{
	PTCIconView		*theView = nil;

	try
	{
		theView = (PTCIconView*) PTCIconView::CreatePaintWindow( inPPobID );
		theView->InitializeFromResource( inMap, inResID );

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
	CreatePaintIconStream
================================================*/
PTCIconView *PTCIconView::CreatePaintCIconStream( LStream *inStream )
{
	return new PTCIconView( inStream );
}

/*===============================================
	Constructor
================================================*/
PTCIconView::PTCIconView( LStream *inStream ) : PTPaintView( inStream )
{
	mColorSample = nil;
	mBWSample = nil;
	mMaskSample = nil;
	mAllowImageResizing = true;
}

/*===============================================
	Destructor
================================================*/
PTCIconView::~PTCIconView()
{
}

/*===============================================
	FinishCreateSelf
================================================*/
void PTCIconView::FinishCreateSelf()
{
	PTPaintView::FinishCreateSelf();

	mColorSample = (PTDraggableTargetBox*) this->FindPaneByID( ImageType_ColorIcon );
	mBWSample = (PTDraggableTargetBox*) this->FindPaneByID( ImageType_LargeIcon );
	mMaskSample = (PTDraggableTargetBox*) this->FindPaneByID( PaneID_Mask );
	ThrowIf_( !mColorSample || !mBWSample || !mMaskSample );

	/*
		keep track of the sample panes 
	*/
	mNumSamplePanes = 3;
	mSamplePaneList[ 0 ] = mColorSample;
	mSamplePaneList[ 1 ] = mBWSample;
	mSamplePaneList[ 2 ] = mMaskSample;

	if ( mSampleWell )
		mSampleWell->GetFrameSize( mInitialSampleWellSize );
	else
	{
		mInitialSampleWellSize.width = 0;
		mInitialSampleWellSize.height = 0;
	}

	/*
		we need to listen to these controls (click and/or drop operations)
	*/
	this->BecomeListenerTo( mNumSamplePanes, mSamplePaneList );
}

/*===============================================
	Initialize
================================================*/
void PTCIconView::InitializeFromResource( RFMap *inMap, ResIDT inResID )
{
	StSaveGWorld		aSaver;
	StSaveResFile		aSaver2;
	Boolean				bwIsEmpty;
	
	SUOffscreen			*colorImage = nil, *bwImage = nil, *maskImage = nil;
	
	this->ParseColorIcon( inMap, inResID, &colorImage, &bwImage, &maskImage, &bwIsEmpty );
	this->InitializeFromBuffers( colorImage, bwImage, maskImage, bwIsEmpty );
}

/*===============================================
	InitializeFromBuffers
	
	Note:
	This call takes control of all buffers
================================================*/
void PTCIconView::InitializeFromBuffers( SUOffscreen *colorImage, SUOffscreen *bwImage, 
										 SUOffscreen *maskImage, Boolean bwIsEmpty )
{
	try
	{
		/****************************************************
			resize the sample panes if necessary
		****************************************************/
		SInt32 	iconWidth = colorImage->GetWidth();
		SInt32	iconHeight = colorImage->GetHeight();
		
			// the sample pane PPob's are sized as though the icon was 32x32
		this->ResizeSampleWell( iconWidth, iconHeight );	
		
		/****************************************************
			setup the main image buffers
			this may trigger a resize of the canvas, change of window size, etc
		****************************************************/
		this->SetImage( colorImage, PTResize_All );

		/****************************************************
			setup the sample buffers
			note: we set the buffer ptr to nil because it doesn't belong to us
				  once the call succeeds.
		****************************************************/
		mColorSample->SetBuffer( colorImage, redraw_Dont );
		colorImage = nil;
		mColorSample->SetTarget( true, redraw_Dont );
		mCurrentSamplePane = mColorSample;
		
		mBWSample->SetBuffer( bwImage, redraw_Dont );
		bwImage = nil;
		if ( bwIsEmpty )
			mBWSample->SetUsedFlag( false, redraw_Dont );
		
		mMaskSample->SetBuffer( maskImage, redraw_Dont );
		maskImage = nil;
							
	//	this->RedrawSampleView();
	}
	catch( ... )
	{
		delete colorImage;
		delete bwImage;
		delete maskImage;
	}
}

/*===============================================
	ParseColorIcon
================================================*/
void PTCIconView::ParseColorIcon( RFMap *inMap, ResIDT inResID,
						SUOffscreen **outColor, SUOffscreen **outBW, 
						SUOffscreen **outMask, Boolean *outBWEmpty )
{
	PixMapPtr		cMap;
	BitMapPtr		bwMap, maskMap;
	SInt32			depth, width, height, handleSize;
	UInt8			*sourceData;
	CTabHandle		theTable = nil;
	SUOffscreen		*cBuffer = nil, *bwBuffer = nil, *maskBuffer = nil;
	Handle			h = nil;
	
	try
	{
		/**************************************************
			get the raw resource handle (this isn't the usual way of loading color icons)
		**************************************************/
		RFResource *theRes = inMap->FindResource( ImageType_ColorIcon, inResID, false );
		ThrowIfNil_( theRes );
		h = theRes->GetResData();
		ThrowIfNil_( h );
		::HLock( h );
		
		handleSize = ::GetHandleSize( h );
		if ( handleSize < static_cast<SInt32>(sizeof(PixMap) + 2 * sizeof(BitMap)) )
			Throw_( err_CorruptedResource );
		
		/**************************************************
			get some info about the icon
		**************************************************/
		cMap = (PixMapPtr) *h;
		maskMap = (BitMap*)( *h + sizeof(PixMap) );
		bwMap = (BitMap*)( *h + sizeof(PixMap) + sizeof(BitMap) );
		
		depth = cMap->pixelSize;
		this->ThrowIfInvalidDepth( depth );

		width = cMap->bounds.right - cMap->bounds.left;
		height = cMap->bounds.bottom - cMap->bounds.top;
		if ( (width < this->GetMinImageWidth()) || (width > this->GetMaxImageWidth()) || 
			(height < this->GetMinImageHeight()) || (height > this->GetMaxImageHeight()) ) 
				Throw_( err_InvalidImageSize );

			// this isn't really necessary, but it can't hurt to check
		if ( (width != maskMap->bounds.right - maskMap->bounds.left) ||
			 (height != maskMap->bounds.bottom - maskMap->bounds.top) ||
			 (width != bwMap->bounds.right - bwMap->bounds.left) ||
			 (height != bwMap->bounds.bottom - bwMap->bounds.top) )
			Throw_( err_CorruptedResource );
		
		/**************************************************
			allocate the mask
		**************************************************/
		sourceData = (UInt8*)( *h + sizeof(PixMap) + 2 * sizeof(BitMap) + sizeof(SInt32) );
		maskBuffer = SUOffscreen::CreateBuffer( width, height, 1 );
		if ( maskMap->rowBytes != 0 )
		{
			maskBuffer->CopyFromRawData( sourceData, maskMap->rowBytes );
			sourceData += maskMap->rowBytes * height;
		}
		
		/**************************************************
			allocate the bw bitmap
		**************************************************/
		bwBuffer = SUOffscreen::CreateBuffer( width, height, 1 );
		if ( bwMap->rowBytes != 0 )
		{
			bwBuffer->CopyFromRawData( sourceData, bwMap->rowBytes );
			sourceData += bwMap->rowBytes * height;
			*outBWEmpty = false;
		}
		else
			*outBWEmpty = true;
		
		/**************************************************
			allocate the color table. note that the resource may have a minimal
			color table and QuickDraw doesn't work with anything but a full-sized one.
		**************************************************/
		theTable = SUColorUtils::NewColorTableFromPtr( depth, sourceData );
		sourceData += sizeof(ColorTable) + (((CTabPtr)sourceData)->ctSize) * sizeof(ColorSpec);
		
		/**************************************************
			allocate the color bitmap
		**************************************************/
		cBuffer = SUOffscreen::CreateBuffer( width, height, depth, theTable );
		SInt32	rowBytes = cMap->rowBytes & 0x3FFF;			// clear flag bits
		if ( rowBytes != 0 )
			cBuffer->CopyFromRawData( sourceData, rowBytes );
		
		if ( PTPrefs::Use32BitBuffersForOthers() )
		{
			SUOffscreen *directBuffer = SUOffscreen::CreateBuffer( width, height, 32 );
			directBuffer->CopyFrom( cBuffer );
			
			delete cBuffer;
			cBuffer = directBuffer;
		}
	}
	catch( ... )
	{
		delete maskBuffer;
		delete bwBuffer;
		delete cBuffer;
		if ( theTable ) ::DisposeCTable( theTable );
		SUMiscUtils::DisposeHandle( h );
		throw;
	}
	
		// return buffers to caller
	*outColor = cBuffer;
	*outBW = bwBuffer;
	*outMask = maskBuffer;
	
		// don't need the color table because the offscreen makes a copy of it
	if ( theTable ) ::DisposeCTable( theTable );
	SUMiscUtils::DisposeHandle( h );
}


/*==================================
	SaveAsResource
===================================*/
void PTCIconView::SaveAsResource( RFMap *inMap, ResIDT inResID )
{
	ThrowIf_( !mColorSample || !mBWSample || !mMaskSample );
	
	SUOffscreen	*colorBuffer = mColorSample->GetBuffer();
	SUOffscreen	*bwBuffer = mBWSample->GetBuffer();
	SUOffscreen	*maskBuffer = mMaskSample->GetBuffer();
	ThrowIf_( !colorBuffer || !bwBuffer || !maskBuffer );

	Handle	h = this->CreateColorIcon( 
					colorBuffer, 
					mBWSample->IsUsed() ? bwBuffer : nil, 
					maskBuffer );
	
	try
	{
		RFResource *theResource = inMap->FindResource( ImageType_ColorIcon, inResID, true );
		ThrowIfNil_( theResource );
		theResource->SetResData( h );
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( h );
		throw;
	}
	
	this->SetChangedFlag( false );
	SUMiscUtils::DisposeHandle( h );
}

/*==================================
	CreateColorIcon
	
	Description:
	Given 3 offscreen buffers (color, bw, and mask), this routine creates a
		block of memory in the same form as a 'cicn' resource. The color
		image will be downsampled to 8-bit if necessary.
		
	See IM V-80 for more details on the resource format.
===================================*/
Handle PTCIconView::CreateColorIcon( SUOffscreen *inColor, SUOffscreen *inBW, 
										SUOffscreen *inMask )
{
	Handle			h = nil;						// the handle we're returning
	CTabHandle		theTable = nil;					// the color table for the output cicn
	Boolean			deleteBuffer = false;

	try
	{		
		/**********************************
			The format of a cicn is:
				Color PixMap
				Mask BitMap
				BW BitMap
				4-byte reserved field
				[mask pixel data]
				[bw pixel data (optional)]
				[color table]
				[color pixel data]
		***********************************/	

		/**********************************
			downsample the source buffer to 8 bits or less because cicn's
			can't be 16 or 32-bits.
		**********************************/
		if ( inColor->GetDepth() >= 16 )
		{
			inColor = SUColorTableBuilder::DownSampleBuffer( 
											inColor, 
											8,				// max depth
											false, 			// don't force b&w into table
											true, 			// minimize table
											true );			// use larger inverse table
			ThrowIfNil_( inColor );					// shouldn't happen
			deleteBuffer = true;					// since we created it
		}

		theTable = inColor->GetColorTable();

		SInt32		width = inColor->GetWidth();
		SInt32		height = inColor->GetHeight();
		SInt32		depth = inColor->GetDepth();
		SInt32 		colorTableBytes = ::GetHandleSize( (Handle) theTable );
		
		Rect		r;
		SetRect( &r, 0, 0, width, height );

		/**********************************
			gather up information on the color icon portion. 
			note that the output rowBytes is probably less than the buffer's
			  since ColorQD likes to pad ram buffers by several bytes per row.
		**********************************/
		PixMap		colorMap;
		SInt32		colorRowBytes = SUColorUtils::CalcRowBytes( width, depth );
		SInt32		colorBytes = colorRowBytes * height;		
		
		colorMap.baseAddr = nil;
		colorMap.rowBytes = colorRowBytes | 0x8000;		// hi bit -> pixmap, not bitmap
		colorMap.bounds = r;
		colorMap.pmVersion = 0;
		colorMap.packType = 0;
		colorMap.packSize = 0;
		colorMap.hRes = 0x00480000;						// 72 dpi
		colorMap.vRes = 0x00480000;
		colorMap.pixelType = chunky;
		colorMap.pixelSize = depth;
		colorMap.cmpCount = 1;
		colorMap.cmpSize = depth;
#if OLDPIXMAPSTRUCT
		colorMap.planeBytes = 0;
		colorMap.pmTable = 0;
		colorMap.pmReserved = 0;
#else
		colorMap.pixelFormat = k16LE5551PixelFormat;
		colorMap.pmTable = NULL;
		colorMap.pmExt = NULL;
#endif
		
		/**********************************
			gather up information on the mask portion
		**********************************/
		BitMap	maskMap;	
		SInt32	maskRowBytes = SUColorUtils::CalcRowBytes( width, 1 );
		SInt32	maskBytes = maskRowBytes * height;

		maskMap.rowBytes = maskRowBytes;
		maskMap.bounds = r;
		maskMap.baseAddr = nil;
		
		/**********************************
			gather up information on the BW icon portion
			(if the image is entirely white, save space by not storing this)
		**********************************/
		BitMap	bwMap;	
		SInt32	bwRowBytes = inBW ? maskRowBytes : 0;		// may be deleted
		SInt32	bwBytes = bwRowBytes * height;

		bwMap.baseAddr = nil;
		bwMap.bounds = r;
		bwMap.rowBytes = bwRowBytes;

		
		/**********************************
			Build the cicn resource
		**********************************/
		SInt32 totalBytes = 	sizeof(PixMap) + 2 * sizeof(BitMap) +
							sizeof(SInt32) + maskBytes + bwBytes + 
							colorTableBytes + colorBytes;
		
		h = ::NewHandleClear( totalBytes );
		ThrowIfMemFail_( h );

		::HLock( h );
		UInt8	*p = (UInt8*) *h;
		
		/**********************************************
			put the PixMap, Mask BitMap, and BW BitMap into the block
		**********************************************/
		BlockMoveData( &colorMap, p, sizeof(colorMap) );
		p += sizeof( colorMap );
		
		BlockMoveData( &maskMap, p, sizeof(maskMap) );
		p += sizeof( maskMap );
		
		BlockMoveData( &bwMap, p, sizeof(bwMap) );
		p += sizeof( bwMap ) + sizeof(SInt32);			// past reserved field too
		
		/**********************************************
			put the raw image data from the mask and bw icon into the block
		**********************************************/
		inMask->CopyToRawData( p, maskRowBytes );
		p += maskBytes;
		
		if ( bwBytes != 0 )
		{
			inBW->CopyToRawData( p, bwRowBytes );
			p += bwBytes;
		}

		/**********************************************
			put the color table and raw color pixels into the block
		**********************************************/
		if ( colorTableBytes > 0 )
		{
			BlockMoveData( *theTable, p, colorTableBytes );
			p += colorTableBytes;
		}

		inColor->CopyToRawData( p, colorRowBytes );
			
		/**********************************************
			return the now-completed color icon
		**********************************************/
		::HUnlock( h );
		
	}
	catch( ... )
	{
		if ( deleteBuffer && inColor )
			delete( inColor );
	
		if ( h ) ::DisposeHandle( h );
		throw;
	}
	
	return( h );
}


/*==================================
	ResizeSampleWell
===================================*/
void PTCIconView::ResizeSampleWell( SInt32 inIconWidth, SInt32 inIconHeight )
{
	SDimension16	initialSampleSize;
	SDimension16	minSampleSize;
	SInt32			dh = inIconWidth - 32;		// since samples are sized for 32x32 initially
	SInt32			dv = inIconHeight - 32;

	mColorSample->GetInitialPaneSize( &initialSampleSize );
	mColorSample->GetMinPaneSize( &minSampleSize );

	/************************************************
		sample panes grow in a 1:1 ratio with the icon, but never
		grow smaller than a certain size.
	*************************************************/
	SInt32 			newPaneWidth = MAX( minSampleSize.width, initialSampleSize.width + dh );
	SInt32			newPaneHeight = MAX( minSampleSize.height, initialSampleSize.height + dv );
	
	mColorSample->ResizeFrameTo( newPaneWidth, newPaneHeight, true );
	mBWSample->ResizeFrameTo( newPaneWidth, newPaneHeight, true );
	mMaskSample->ResizeFrameTo( newPaneWidth, newPaneHeight, true );
	
	/************************************************
		the b&w and mask panes must be moved down so things don't overlap
	*************************************************/
	SPoint32		initialLoc, currentLoc;
	
		// otherwise boxes overlap...
	if ( dv < -16 )
		dv = -16;
		
		// otherwise the right side gets cutoff on small cicns
	if ( dh < -16 )
		dh = -16;
	
	mBWSample->GetInitialPaneLocation( &initialLoc );
	mBWSample->GetFrameLocation( currentLoc );
	mBWSample->MoveBy( 0, initialLoc.v + dv - currentLoc.v, true );
	
	mMaskSample->GetInitialPaneLocation( &initialLoc );
	mMaskSample->GetFrameLocation( currentLoc );
	mMaskSample->MoveBy( 0, initialLoc.v + 2 * dv - currentLoc.v, true );

	/************************************************
		the sample well contains all three sample icons (color, bw, mask)
		and needs to be resized accordingly. Never allow it to get tiny
		or things may get cut-off. The numbers should probably be read 
		from a resource???
	*************************************************/
	if ( mSampleWell )
		mSampleWell->ResizeFrameTo( MAX( 50, mInitialSampleWellSize.width + dh ),
									MAX( 140, mInitialSampleWellSize.height + 3 * dv),
									true );
}

/*===============================================
	ChangeImageSize
	
	Note:
	Affects all three buffers: color, bw, mask
================================================*/
void PTCIconView::ChangeImageSize( SInt32 inWidth, SInt32 inHeight, Boolean inStretch )
{
	ThrowIf_( (inWidth < mMinImageWidth) || (inHeight < mMinImageHeight) );
	ThrowIf_( (inWidth > mMaxImageWidth) || (inHeight > mMaxImageHeight) );
	ThrowIf_( !mColorSample || !mBWSample || !mMaskSample );
	ThrowIfNil_( mCurrentImage );
	
	SInt32			oldWidth = mCurrentImage->GetWidth();
	SInt32			oldHeight = mCurrentImage->GetHeight();
	Boolean			changedSize = true;
	Rect			destR;

	SUOffscreen		*newColor = nil, *newBW = nil, *newMask = nil;
		
	try
	{
		/*****************************************
			either stretch the image or put it at the top/left of the destination
		*****************************************/
		if ( inStretch )
			::SetRect( &destR, 0, 0, inWidth, inHeight );
		else
			::SetRect( &destR, 0, 0, mCurrentImage->GetWidth(), mCurrentImage->GetHeight() );
		
		/*****************************************
			create new buffers for the 3 sample panes
		*****************************************/
		newColor = mColorSample->GetBuffer()->CreateSimilarOffscreen( false, inWidth, inHeight );
		newColor->CopyFrom( mColorSample->GetBuffer(), &destR );
			
		newBW = SUOffscreen::CreateBuffer( inWidth, inHeight, 1 );
		newBW->CopyFrom( mBWSample->GetBuffer(), &destR );
		
		newMask = SUOffscreen::CreateBuffer( inWidth, inHeight, 1 );
		newMask->CopyFrom( mMaskSample->GetBuffer(), &destR );

		/*****************************************
			resize the sample panes
		*****************************************/
		this->ResizeSampleWell( inWidth, inHeight );
		changedSize = true;
		
		/*****************************************
			set the current buffer
			 -- this can trigger all sorts of things (canvas resize, etc)
		*****************************************/
		SUOffscreen *newBuffer;
		PTDraggableTargetBox *curTarget = this->GetTargetBox();
		if ( curTarget == mColorSample )
			newBuffer = newColor;
		else if ( curTarget == mBWSample )
			newBuffer = newBW;
		else
			newBuffer = newMask;
			
		this->SetImage( newBuffer, PTResize_All, redraw_Later );
		
		/*****************************************
			change the sample panes' buffers.
			Notes:
			 (1) The sample panes will own the buffer after the call (don't delete em)
			 (2) The previous sample pane buffer will be saved away by the PTResizer object
		*****************************************/
		mColorSample->SetRawBuffer( newColor );
		mBWSample->SetRawBuffer( newBW );
		mMaskSample->SetRawBuffer( newMask );
	}
	catch( ... )
	{
		if ( changedSize )
			this->ResizeSampleWell( oldWidth, oldHeight );
	
		delete newColor;
		delete newBW;
		delete newMask;
		throw;
	}
}
