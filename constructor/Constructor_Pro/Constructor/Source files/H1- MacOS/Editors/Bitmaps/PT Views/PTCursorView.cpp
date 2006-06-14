// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PTCursorView.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTCursorView

DESCRIPTION:		This code is for painting ICON resources
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.07.01

CHANGE HISTORY :

		97.02.13	ebs		Set mPrefersIconColors to false in constructor
		96.11.26	ebs		Add include RFResource.h (CW11)
		96.08.07	ebs		Removed call to GetDecentZoomFactor (routine is gone now)
		96.08.05	ebs		Support for mSamplePaneList
		96.08.04	ebs		Removed oneBitTable -- wasn't used anymore
		96.07.22	ebs		New target boxes
		96.07.18	ebs		Move to 32-bit pixmaps
		96.07.05	ebs		Works now
		96.07.01	ebs		Started

---------------------------------------<< ¥ >>-----------------------------------------
*/

/*
	Cursor structure is:
		BW Image Bits (16 bits per row x 16 rows = 32 bytes)
		Mask Image Bits (same )
		HotSpot (Point)
		
	Color cursor structure: (see also IM V-78):
		CCrsr structure (see Quickdraw.h)
		PixMap
		Color Pixel Data
		Color Table Data
*/

#include "PT_Headers.h"
#include "PTActions.h"
#include "PTCursorView.h"
#include "PTDraggableTargetBox.h"

#include "SUSaveResFile.h"
#include "SUColorUtils.h"
#include "SUFileUtils.h"
#include "SUPaneResizer.h"
#include "SUColorTableBuilder.h"

#include "RFMap.h"
#include "RFResource.h"

/*====================================
	Constants
=====================================*/
const PaneIDT	PaneID_Mask			=	'MASK';

	// cursors are always the same size
const SInt32		Cursor_Width		=	16;
const SInt32		Cursor_Height		=	16;

	// black & white cursors have the following values
const SInt32		BWCursor_RowBytes	=	2;	
const SInt32		BWCursor_Bytes		=	68;				// 2 bw images, 16x16, + hotspot

/*====================================
	OpenPaintWindow
=====================================*/
PTCursorView* PTCursorView::OpenPaintWindow( ResIDT inPPobID, RFMap *inMap, ResType inResType, ResIDT inResID )
{
	PTCursorView		*theView = nil;

	try
	{
		theView = (PTCursorView*) PTCursorView::CreatePaintWindow( inPPobID );
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
PTCursorView *PTCursorView::CreateFromStream( LStream *inStream )
{
	return new PTCursorView( inStream );
}

/*===============================================
	Constructor
================================================*/
PTCursorView::PTCursorView( LStream *inStream ) : PTPaintView( inStream )
{
	mColorSample = nil;
	mBWSample = nil;
	mMaskSample = nil;
	mResourceType = 0;
	mPrefersIconColors = false;
}

/*===============================================
	Destructor
================================================*/
PTCursorView::~PTCursorView()
{
}

/*===============================================
	FinishCreateSelf
================================================*/
void PTCursorView::FinishCreateSelf()
{
	PTPaintView::FinishCreateSelf();

	/*
		note: 
			crsr	- has mColorSample, mBWSample, mMaskSample
			CURS	- has mBWSample & mMaskSample
	*/
	
	mColorSample = (PTDraggableTargetBox*) this->FindPaneByID( ImageType_ColorCursor );
	mBWSample = (PTDraggableTargetBox*) this->FindPaneByID( ImageType_Cursor );
	mMaskSample = (PTDraggableTargetBox*) this->FindPaneByID( PaneID_Mask );

	ThrowIfNil_( mBWSample );
	ThrowIfNil_( mMaskSample );

	/*
		keep track of the sample panes
	*/	
	mNumSamplePanes = 0;
	if ( mColorSample )
		mSamplePaneList[ mNumSamplePanes++ ] = mColorSample;
	mSamplePaneList[ mNumSamplePanes++ ] = mBWSample;
	mSamplePaneList[ mNumSamplePanes++ ] = mMaskSample;

	/*
		we need to listen to the sample panes (click and/or drop operations)
	*/
	this->BecomeListenerTo( mNumSamplePanes, mSamplePaneList );
}

/*===============================================
	Initialize
================================================*/
void PTCursorView::InitializeFromResource( RFMap *inMap, ResType inResType, ResIDT inResID )
{
	StSaveGWorld		aSaver;
	StSaveResFile		aSaver2;

	SUOffscreen			*colorImage = nil, *bwImage = nil, *maskImage = nil;
	Point				theHotSpot;
	
	switch( inResType )
	{
		case ImageType_Cursor:
			this->ParseBWCursor( inMap, inResID, &bwImage, &maskImage, &theHotSpot );
			mResourceType = inResType;
			break;
			
		case ImageType_ColorCursor:
			this->ParseColorCursor( inMap, inResID, &colorImage, &bwImage, &maskImage, &theHotSpot );
			mResourceType = inResType;
			ThrowIfNil_( mColorSample );
			break;
			
		default:
			LException::Throw( err_InvalidImageFormat );
			break;
	}
	
	try
	{
		mCanvas->SetHotSpot( theHotSpot );
	
			// set the image for the paint view
			// if we don't have a color one, use the b&w
		if ( colorImage )
		{
			this->SetImage( colorImage, PTResize_All );
			mCurrentSamplePane = mColorSample;
			mColorSample->SetTarget( true, redraw_Dont );
		}
		else
		{
			this->SetImage( bwImage, PTResize_All );
			mCurrentSamplePane = mBWSample;
			mBWSample->SetTarget( true, redraw_Dont );
			mBWSample->SetUsedFlag( true, redraw_Dont );
		}
	}
	catch( ... )
	{
		delete colorImage;
		delete bwImage;
		delete maskImage;
		throw;
	}

		// give each of the sample panes their image
	if ( mColorSample && colorImage )
		mColorSample->SetBuffer( colorImage, redraw_Dont );
	mBWSample->SetBuffer( bwImage, redraw_Dont );
	mMaskSample->SetBuffer( maskImage, redraw_Dont );	

		// and we'll need to redraw the sample view (if we've already been initted anyway)
	this->RedrawSampleView();
}


/*===============================================
	ParseBWCursor
================================================*/
void PTCursorView::ParseBWCursor( RFMap *inMap, ResIDT inResID,
						SUOffscreen **outBW, SUOffscreen **outMask, 
						Point *outHotSpot )
{
	CursHandle		h = nil;
	SUOffscreen		*bw = nil, *mask = nil;
	
	try
	{
		/**************************************************
			get the raw resource handle
		**************************************************/
		RFResource *theRes = inMap->FindResource( ImageType_Cursor, inResID, false );
		ThrowIfNil_( theRes );
		h = (CursHandle) theRes->GetResData();
		ThrowIfNil_( h );
		::HLock( (Handle) h );

			// 2 rowBytes x 16 rows x 2 images + sizeof(Point)
		if ( ::GetHandleSize( (Handle) h ) != BWCursor_Bytes )
			LException::Throw( err_CorruptedResource );
		
		/**************************************************
			create the bw bitmap
		**************************************************/
		bw = SUOffscreen::CreateBuffer( Cursor_Width, Cursor_Height, 1 );
		ThrowIfMemFail_( bw );
		//bw->AssignPalette( oneBitTable );
		bw->CopyFromRawData( (UInt8*) &(**h).data, BWCursor_RowBytes );

		/**************************************************
			create the mask bitmap
		**************************************************/
		mask = SUOffscreen::CreateBuffer( Cursor_Width, Cursor_Height, 1 );
		ThrowIfMemFail_( bw );
		mask->CopyFromRawData( (UInt8*) &(**h).mask, BWCursor_RowBytes );

		/**************************************************
			and return stuff to the caller
		**************************************************/
		*outBW = bw;
		*outMask = mask;
		*outHotSpot = (**h).hotSpot;
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( h );
		if ( bw ) delete ( bw );
		if ( mask ) delete( mask );
		throw;
	}
	
	SUMiscUtils::DisposeHandle( h );
}


/*===============================================
	ParseColorCursor

	Note:
	See description of CCrsr structure above.
================================================*/
void PTCursorView::ParseColorCursor( RFMap *inMap, ResIDT inResID,
						SUOffscreen **outColor, SUOffscreen **outBW, 
						SUOffscreen **outMask, Point *outHotSpot )
{
	PixMapPtr		cMap;
	SInt32			depth, width, height;
	CTabHandle		theTable = nil;
	SUOffscreen		*cBuffer = nil, *bwBuffer = nil, *maskBuffer = nil, *tempBuffer = nil;
	CCrsrHandle		h = nil;
	UInt8			*p;
	
	try
	{
		/**************************************************
			get the raw resource handle
			 -- this isn't the usual way of loading color cursors, so the fields
			 -- will be raw and not filled in (as when GetCCursor is used)
		**************************************************/
		RFResource *theRes = inMap->FindResource( ImageType_ColorCursor, inResID, false );
		ThrowIfNil_( theRes );
		h = (CCrsrHandle) theRes->GetResData();
		ThrowIfNil_( h );
		::HLock( (Handle) h );
		p = (UInt8*) *h;
		
		/**************************************************
			validate the handle a little bit
		**************************************************/
		if ( (UInt16) (**h).crsrType != (UInt16) 0x8001 )	// cast *is* needed - crsrType is signed
			LException::Throw( err_InvalidImageFormat );
		
		if ( ::GetHandleSize( (Handle) h ) < static_cast<Size>(sizeof(CCrsr)) )
			LException::Throw( err_CorruptedResource );
	
		/**************************************************
			get some info about the cursor
		**************************************************/
		cMap = (PixMapPtr) (p + (SInt32) (**h).crsrMap);

		if ( cMap->pixelType != 0 )
			LException::Throw( err_InvalidImageFormat );

		width = cMap->bounds.right - cMap->bounds.left;
		height = cMap->bounds.bottom - cMap->bounds.top;
		depth = cMap->pixelSize;
		
		if ( (width != Cursor_Width) || (height != Cursor_Height) )
			LException::Throw( err_InvalidImageSize );
		
		if ( (depth != 1) && (depth != 2) && (depth != 4) && (depth != 8) )
			LException::Throw( err_InvalidImageDepth );
		
		/**************************************************
			create the bwBuffer and maskBuffer
		**************************************************/
		bwBuffer = SUOffscreen::CreateBuffer( Cursor_Width, Cursor_Height, 1 );
		ThrowIfMemFail_( bwBuffer );
		bwBuffer->CopyFromRawData( (UInt8*) &(**h).crsr1Data, BWCursor_RowBytes );

		maskBuffer = SUOffscreen::CreateBuffer( Cursor_Width, Cursor_Height, 1 );
		ThrowIfMemFail_( maskBuffer );
		maskBuffer->CopyFromRawData( (UInt8*) &(**h).crsrMask, BWCursor_RowBytes );

		/**************************************************
			allocate the color table. note that the resource may have a minimal
			color table and QuickDraw doesn't work with anything but a full-sized one.
		**************************************************/
		if ( cMap->pmTable == 0 )			// probably won't happen, but can't hurt to check
			theTable = SUColorUtils::NewColorTableByDepth( depth );
		else
			theTable = SUColorUtils::NewColorTableFromPtr( depth, p + (SInt32) cMap->pmTable );
				
		/**************************************************
			allocate the color bitmap
		**************************************************/
		tempBuffer = SUOffscreen::CreateBuffer( width, height, depth, theTable );
		SInt32	rowBytes = cMap->rowBytes & 0x3FFF;			// clear flag bits
		if ( rowBytes != 0 )
			tempBuffer->CopyFromRawData( p + (SInt32) (**h).crsrData, rowBytes );

		/**************************************************
			now switch it to a 32-bit offscreen
		**************************************************/
		cBuffer = SUOffscreen::CreateBuffer( width, height, 32 );
		cBuffer->CopyFrom( tempBuffer );
	}
	catch( ... )
	{
		delete maskBuffer;
		delete bwBuffer;
		delete cBuffer;
		delete tempBuffer;
		if ( theTable ) ::DisposeCTable( theTable );
		SUMiscUtils::DisposeHandle( h );
	}
	
		// return buffers and info to the caller
	*outColor = cBuffer;
	*outBW = bwBuffer;
	*outMask = maskBuffer;
	*outHotSpot = (**h).crsrHotSpot;

		// don't need the color table because the offscreen makes a copy of it
	if ( theTable ) ::DisposeCTable( theTable );
	SUMiscUtils::DisposeHandle( h );
	delete tempBuffer;
}

/*==================================
	SaveAsResource
===================================*/
void PTCursorView::SaveAsResource( RFMap *inMap, ResIDT inResID )
{
	Handle		h = nil;
	
	ThrowIf_( !mBWSample || !mMaskSample );
	SUOffscreen	*bwBuffer = mBWSample->GetBuffer();
	SUOffscreen	*maskBuffer = mMaskSample->GetBuffer();

	Point	theHotSpot = mCanvas->GetHotSpot();
	
	try
	{
		switch( mResourceType )
		{
			case ImageType_Cursor:
				h = this->CreateBWCursor( bwBuffer, maskBuffer, theHotSpot );
				break;
			
			case ImageType_ColorCursor:
				ThrowIfNil_( mColorSample );
				h = this->CreateColorCursor( mColorSample->GetBuffer(), bwBuffer, maskBuffer, theHotSpot );
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
	
	this->SetChangedFlag( false );
	SUMiscUtils::DisposeHandle( h );
}

/*==================================
	CreateBWCursor
===================================*/
Handle PTCursorView::CreateBWCursor( SUOffscreen *inImage, SUOffscreen *inMask, Point inHotSpot )
{
	CursHandle	h = (CursHandle) ::NewHandleClear( BWCursor_Bytes );
	ThrowIfMemFail_( h );
	StHandleLocker	lock1( (Handle) h );
	
	if ( inImage )
		inImage->CopyToRawData( (UInt8*) &(**h).data, BWCursor_RowBytes );
		
	if ( inMask )
		inMask->CopyToRawData( (UInt8*) &(**h).mask, BWCursor_RowBytes );
		
	(**h).hotSpot = inHotSpot;
	return( (Handle) h );
}

/*==================================
	CreateColorCursor
	
	Note:
	See description of CCrsr structure above.
===================================*/
Handle PTCursorView::CreateColorCursor( SUOffscreen *inColor, SUOffscreen *inBW, 
										SUOffscreen *inMask, Point inHotSpot )
{
	Handle			h = nil;
	Boolean			deleteBuffer = false;
	
	ThrowIf_( !inColor || !inBW || !inMask );
	
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
		SInt32		pixelRowBytes = SUColorUtils::CalcRowBytes( Cursor_Width, depth );
		SInt32		pixelBytes = pixelRowBytes * Cursor_Height;
		SInt32		totalBytes = sizeof( CCrsr) + sizeof( PixMap ) + colorTableBytes + pixelBytes;

		SInt32		offsetToPixmap = sizeof(CCrsr);
		SInt32		offsetToPixelData = sizeof(CCrsr) + sizeof(PixMap);
		SInt32		offsetToColorTable = sizeof(CCrsr) + sizeof(PixMap) + pixelBytes;
		
		/**********************************
			Allocate the cursor
		**********************************/
		h = ::NewHandleClear( totalBytes ); 
		ThrowIfMemFail_( h );
		::HLock( h );
		
		CCrsr 	*p = (CCrsr *) *h;
		PixMap 	*theMap = (PixMap*) (*h + offsetToPixmap);
		
		/**********************************
			Fill up the structure (note: fields are already zeroed)
		**********************************/
		p->crsrType = 0x8001;						// magic number for cursors
		p->crsrMap = (PixMapHandle) offsetToPixmap;
		p->crsrData = (Handle) offsetToPixelData;
		
		inBW->CopyToRawData( (UInt8*) &p->crsr1Data, BWCursor_RowBytes );
		inMask->CopyToRawData( (UInt8*) &p->crsrMask, BWCursor_RowBytes );
		p->crsrHotSpot = inHotSpot;
		
		theMap->rowBytes = 0x8000 + pixelRowBytes;
		::SetRect( &theMap->bounds, 0, 0, Cursor_Width, Cursor_Height );
		theMap->hRes = 0x00480000;
		theMap->vRes = 0x00480000;
		theMap->pixelSize = depth;
		theMap->cmpCount = 1;
		theMap->cmpSize = depth;
		theMap->pmTable = (CTabHandle) offsetToColorTable;

		inColor->CopyToRawData( (UInt8*)( *h + offsetToPixelData ), pixelRowBytes );
		
		CTabPtr	destTable = (CTabPtr) (*h + offsetToColorTable);
		BlockMoveData( *sourceTable, destTable, colorTableBytes );
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


/*==================================
	CopyToUndo
===================================*/
void PTCursorView::CopyToUndo()
{
	PTPaintView::CopyToUndo();
	mUndoHotSpot = mCanvas->GetHotSpot();
}

/*==================================
	SwapUndoState
===================================*/
void PTCursorView::SwapUndoState()
{
	Point	tempPt = mUndoHotSpot;
	
	mUndoHotSpot = mCanvas->GetHotSpot();
	mCanvas->SetHotSpot( tempPt );
	
	PTPaintView::SwapUndoState();
}

