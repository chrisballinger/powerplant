// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				Offscreen.cp

CLASSES:			SUOffscreen, StSaveAndDraw

DESCRIPTION:		Implementation file for Object

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.11.18	ebs		RGBToPixelValue - swapped 16/32 bit switch code (was wrong)
		96.07.30	ebs		Leak detection code added
		96.07.18	ebs		Added IncreaseInverseTable
		96.07.11	ebs		Decided to keep pixels locked all the time.
							This allows us speedier access to the pixels and
							less VM thrashing, at the expense of some fragmentation.
		96.07.10	ebs		Rewrote w/support for 16 & 32-bit pixmaps
		96.06.27	ebs		Added CopyFromAndDownSample
		96.06.17	ebs		Support for 2-bit pixmaps
		96.05.09	ebs		Support for 4-bit pixmaps
		96.4.18		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include <PictUtils.h>

#include "SU_Headers.h"

#include "SUOffscreen.h"
#include "SUSaveGWorld.h"
#include "SUBitUtils.h"
#include "SUColorUtils.h"
#include "SUMiscUtils.h"
#include "SUSharedPalette.h"

#include <stdio.h>				// just for leak detection debugging...

	// this clears the flag bits in a pixmap's rowBytes field
#define FixRowBytes( rb )	( (rb) & 0x3FFF )

/*==========================================
	this is used just for leak detection
===========================================*/
SInt32	SUOffscreen::sNumBuffers = 0;

/*==========================================
	SUOffscreen::CreateBuffer
	
	Notes:
	Erases the buffer to white.
	
	Caller may dispose of the color table after this call,
	but not the GDevice (if any).
===========================================*/
SUOffscreen *SUOffscreen::CreateBuffer( SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
										CTabHandle inTable, GDHandle inDevice, 
										Boolean inKeepLocal )
{
	return new SUOffscreen( inWidth, inHeight, inDepth, inTable, inDevice, inKeepLocal );
}

/*==========================================
	SUOffscreen::Constructor
===========================================*/
SUOffscreen::SUOffscreen( SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
										CTabHandle inTable, GDHandle inDevice, 
										Boolean inKeepLocal )
{
	mWorld = nil;
	mWidth = inWidth;
	mHeight = inHeight;
	mDepth = inDepth;
	mRowBytes = 0;						// set below
	mRowArray = nil;
	mPixMap = nil;
	mPalette = nil;

	switch( inDepth )
	{
		case 1:		case 16:	case 32:
			mHasDirectPixels = true;
			break;
		
		case 2:		case 4:		case 8:
			mHasDirectPixels = false;
			break;
			
		default:
			LException::Throw( paramErr );
	}
	
		// find the getpixel/setpixel functions	for this depth
	SUPixelHelper::GetPixelFunctions( inDepth, &mPixelGetter, &mPixelSetter );
	
	try
	{
		mWorld = SUOffscreen::CreateGWorld( inWidth, inHeight, inDepth, inTable, 
											inDevice, inKeepLocal, &mRowBytes );
		ThrowIfNil_( mWorld );		
		mPixMap = ::GetGWorldPixMap( mWorld );
		
		this->SetupRowArray();
	}
	catch( ... )
	{
		if ( mWorld ) ::DisposeGWorld( mWorld );
		if ( mRowArray ) ::DisposePtr( (Ptr) mRowArray );
		throw;
	}
	
	++sNumBuffers;
}

/*==========================================
	SUOffscreen::SetupRowArray
	
	Notes:
	Requires mPixMap, mHeight, and mRowBytes to be correct.
	Pixmap bits must remain locked at all times or these values will be wrong.
===========================================*/
void SUOffscreen::SetupRowArray()
{
	mRowArray = (RawPtr*) ::NewPtr( sizeof(RawPtr) * mHeight );
	ThrowIfMemFail_( mRowArray );
	
	RawPtr	rowPtr = (RawPtr) ::GetPixBaseAddr( mPixMap );
	for ( SInt32 count = 0; count < mHeight; count++ )
	{
		mRowArray[ count ] = rowPtr;
		rowPtr += mRowBytes;
	}
}

/*==========================================
	SUOffscreen::CreateGWorld
	
	Notes:
	Erases the buffer to white. Leaves the pixels locked.
	The ColorTable is copied, so caller can delete it.
===========================================*/
GWorldPtr SUOffscreen::CreateGWorld( SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
							CTabHandle inTable, GDHandle inDevice, Boolean inKeepLocal,
							SInt32 *outRowBytes )
{
	ThrowIf_( (inWidth <= 0) || (inHeight <= 0) );

	StSaveGWorld		aSaver;					// save/restore GWorld
	GWorldPtr			theWorld = nil;
	OSErr				err = noErr;
	Rect				r;
	GWorldFlags			flags;
	
	/***********************************************
		Set up the variables for the NewGWorld call
	***********************************************/
	::SetRect( &r, 0, 0, inWidth, inHeight );
	flags = useTempMem | (inDevice ? noNewDevice : 0) | (inKeepLocal ? keepLocal : 0);

	/***********************************************
		Allocate the GWorld in multifinder memory.
		If memory is full, use the current heap instead.
	***********************************************/
	err = ::NewGWorld( &theWorld, inDepth, &r, inTable, inDevice, flags );
	if ( (err == memFullErr) || (err == cTempMemErr) || (err == cNoMemErr) )
	{
		flags &= ~useTempMem;
		err = ::NewGWorld( &theWorld, inDepth, &r, inTable, inDevice, flags );
	}

	ThrowIfOSErr_( err );
	ThrowIfMemFail_( theWorld );			// shouldn't happen

	/***********************************************
		Force the GWorld to white
	***********************************************/
	PixMapHandle thePixMap = ::GetGWorldPixMap( theWorld );
	::LockPixels( thePixMap );
	::SetGWorld( theWorld, nil );
	::RGBForeColor( &kBlackColor );
	::RGBBackColor( &kWhiteColor );
	Rect theRect;
#if PP_Target_Carbon
	::GetPortBounds(theWorld, &theRect);
#else
	theRect = theWorld->portRect;
#endif
	::EraseRect( &theRect );
	
	if ( outRowBytes )
#if PP_Target_Carbon
		*outRowBytes = ::GetPixRowBytes(thePixMap);
#else
		*outRowBytes = FixRowBytes( (**thePixMap).rowBytes );
#endif

	/***********************************************
		return the GWorld to the caller
	***********************************************/
	return( theWorld );
}


/*==========================================
	SUOffscreen Destructor
===========================================*/
SUOffscreen::~SUOffscreen()
{
	if ( mRowArray )
	{
		::DisposePtr( (Ptr) mRowArray );
		mRowArray = nil;
	}

	if ( mWorld )
	{
		/***********************************************
			Quickdraw isn't smart enough to notice when you destroy the
			current gworld and it crashes if you do so. We'll prevent this.
		***********************************************/
		CGrafPtr	theCurrentPort;
		GDHandle	theCurrentDevice;
		
		::GetGWorld( &theCurrentPort, &theCurrentDevice );
		if ( theCurrentPort == mWorld )
		{
			#ifdef _H_PP_Types
			LView::OutOfFocus( nil );		// flush PP cache of current view
			#endif
			
			::SetGDevice( GetMainDevice() );
#if PP_Target_Carbon
			::SetPortWindowPort( ::FrontWindow() );
#else
			GrafPtr aSparePort;
			::GetWMgrPort( &aSparePort );
			::SetPort( aSparePort );
#endif
		}
	
		/***********************************************
			(finally) dispose of the gworld
		***********************************************/
		::DisposeGWorld( mWorld );
		mWorld = nil;
	}
	
	if ( mPalette )
	{
		mPalette->DecrementRefCount();
		mPalette = nil;
	}
	
	--sNumBuffers;
}

/*==========================================
	SUOffscreen::GetColorTable
===========================================*/
CTabHandle SUOffscreen::GetColorTable()
{
	return( (**this->GetPixMap()).pmTable );
}

/*==========================================
	SUOffscreen::GetPixel
	
	Note:
	The pixel value can be either an index to a color table (2,4,8 bits)
	or a direct color (1,16,32 bits).
===========================================*/
Color32 SUOffscreen::GetPixel( SInt32 h, SInt32 v )
{
	if ( (h < 0) || (h >= mWidth) || (v < 0) || (v >= mHeight) )
		LException::Throw( paramErr );
	
	return( this->RawGetPixel( this->RawGetRowPtr(v), h ) );
}

/*==========================================
	SUOffscreen::SetPixel
===========================================*/
void SUOffscreen::SetPixel( SInt32 h, SInt32 v, PixelValue inPixel )
{
	if ( (h < 0) || (h >= mWidth) || (v < 0) || (v >= mHeight) )
		LException::Throw( paramErr );
	
	this->RawSetPixel( this->RawGetRowPtr(v), h, inPixel );
}


/*==========================================
	SUOffscreen::GetPixelColor
	
	Note:
	This differs from GetPixel() in that GetPixel() returns the value from
	the offscreen, which may be an index to a color, while this function
	converts that index into a real Color32.
===========================================*/
Color32 SUOffscreen::GetPixelColor( SInt32 h, SInt32 v )
{
	return this->PixelValueToColor32( this->GetPixel( h, v ) );
}

/*==========================================
	SUOffscreen::GetPixelRGB
===========================================*/
RGBColor SUOffscreen::GetPixelRGB( SInt32 h, SInt32 v )
{
	return this->PixelValueToRGB( this->GetPixel( h, v ) );
}

/*==========================================
	SUOffscreen::RawGetPixelColor
===========================================*/
Color32 SUOffscreen::RawGetPixelColor( RawPtr inRowPtr, SInt32 h )
{
	PixelValue	thePixel = this->RawGetPixel( inRowPtr, h );
	
	if ( mDepth == 32 )
		return( thePixel );
		
	return( this->PixelValueToColor32( thePixel ) );
}

/*==========================================
	SUOffscreen::RawSetPixelColor
===========================================*/
void SUOffscreen::RawSetPixelColor( RawPtr inRowPtr, SInt32 h, Color32 inColor )
{
	if ( mDepth == 32 )
	{
		this->RawSetPixel( inRowPtr, h, inColor );		// already in 32-bit format
	}
	else
	{
		PixelValue thePixel = this->Color32ToPixelValue( inColor );
		this->RawSetPixel( inRowPtr, h, thePixel );
	}
}

/*==========================================
	SUOffscreen::PixelValueToColor32
===========================================*/
Color32 SUOffscreen::PixelValueToColor32( PixelValue thePixelValue )
{
	if ( mDepth == 32 )
		return( thePixelValue );			// already in the correct format
	
	RGBColor	theColor = this->PixelValueToRGB( thePixelValue );
	return( SUColorUtils::RGBToColor32( theColor ) );
}

/*==========================================
	SUOffscreen::Color32ToPixelValue
===========================================*/
PixelValue SUOffscreen::Color32ToPixelValue( Color32 theColor32 )
{
	if ( mDepth == 32 )
		return( theColor32 );			// already in the correct format
	
	RGBColor	theRGB = SUColorUtils::Color32ToRGB( theColor32 );
	return this->RGBToPixelValue( theRGB );
}




/*==========================================
	SUOffscreen::PixelValueToRGB
	
	Note:
	Doesn't check for out-of-bounds conditions.
===========================================*/
RGBColor SUOffscreen::PixelValueToRGB( PixelValue thePixelValue )
{
	switch( mDepth )
	{
		case 1:
			return thePixelValue ? kBlackColor : kWhiteColor;
		case 16:
			return SUColorUtils::Color16ToRGB( thePixelValue );
		case 32:
			return SUColorUtils::Color32ToRGB( thePixelValue );
		
		default:
			CTabHandle	theTable = this->GetColorTable();
			return( (**theTable).ctTable[ thePixelValue ].rgb );
	}
}

/*==========================================
	SUOffscreen::RGBToPixelValue
	
	Note:
	For b&w, everything that's not white becomes black (ie, no luminance matching).
===========================================*/
PixelValue SUOffscreen::RGBToPixelValue( const RGBColor &inColor )
{
	/*
		the direct buffers are easiest
	*/
	switch( mDepth )
	{
		case 1:
			return( SUColorUtils::EqualRGB(inColor, kWhiteColor) ? 0 : 1 );
			break;
		case 16:
			return SUColorUtils::RGBToColor16( inColor );
			break;
		case 32:
			return SUColorUtils::RGBToColor32( inColor );
			break;
	}

	/*
		for indexed buffers, scan the color table for an exact match
		first. if not found, let Quickdraw use the inverse table and 
		find a close match. (inverse tables aren't very accurate at times)
	
		we'll check specifically for black & white first because it's 
		possible for them to be in the color table in more than one place. 
		we'll return the "preferred" position for each.
	*/
	CTabHandle	theTable = this->GetColorTable();
	SInt32		tableEntries = (**theTable).ctSize + 1;
	
	if ( SUColorUtils::EqualRGB( inColor, kWhiteColor ) )
		return( 0 );
	
	if ( SUColorUtils::EqualRGB( inColor, kBlackColor ) )
		return( tableEntries - 1 );
		
	for ( SInt32 count = 0; count < tableEntries; count++ )
	{
		RGBColor	tempColor = (**theTable).ctTable[ count ].rgb;
		if ( SUColorUtils::EqualRGB( tempColor, inColor ) )
			return( count );
	}
	
		// not in the table -- let Quickdraw search
	{
		StSaveAndDraw		aSaver( this );		// sets the GDevice (where the inverse table is)
		return( ::Color2Index( &inColor ) );
	}
}

/*==========================================
	SUOffscreen::BeginDrawing
===========================================*/
void SUOffscreen::BeginDrawing()
{
	::SetGWorld( mWorld, nil );
	
	#ifdef _H_PP_Types
		LView::OutOfFocus( nil );		// flush PP cache of current view
	#endif
}

/*==========================================
	SUOffscreen::EndDrawing
	
	Note:
	This used to unlock the pixels, but now we leave them locked
	because it's faster and simplifies the code a lot.
===========================================*/
void SUOffscreen::EndDrawing()
{
	//this->Unlock();
}

/*==========================================
	SUOffscreen::CopyTo
	
	Note:
	May change the fore/back color.
	destR is in port-relative coordinates
	For speed, should we leave this locked ???
===========================================*/
void SUOffscreen::CopyTo( GrafPtr inDestPort, const Rect *inDestR, 
						 const Rect *inSourceR, SInt16 inMode, 
						 RgnHandle inMaskRgn )
{
	StSaveGWorld		aSaver;						// save/restore the port & device
	BitMap				*sourceBits, *destBits;
	Rect				sourceR, destR;
	
	ThrowIfNil_( inDestPort );
	
	this->BeginDrawing();
	this->ResetColorsSelf();					// don't want copybits to colorize

	SUMiscUtils::SetPort( inDestPort );			// note: forces origin to 0,0, gdevice to main, unclips
	
#if PP_Target_Carbon
	sourceBits = (BitMap*) *::GetGWorldPixMap(mWorld);
	destBits = (BitMap*) *::GetPortPixMap(inDestPort);
#else
	sourceBits = &((GrafPtr)mWorld)->portBits;
	destBits = &inDestPort->portBits;
#endif

	if ( inSourceR )
		sourceR = *inSourceR;
	else
		SetRect( &sourceR, 0, 0, mWidth, mHeight );
	
	if ( inDestR )
		destR = *inDestR;
	else
#if PP_Target_Carbon
		::GetPortBounds(inDestPort, &destR);
#else
		destR = inDestPort->portRect;
#endif

		// we don't want to colorize the image, so set black & white colors
	RGBForeColor( &kBlackColor );
	RGBBackColor( &kWhiteColor );

	CopyBits( sourceBits, destBits, &sourceR, &destR, inMode, inMaskRgn );
	this->EndDrawing();
}


/*==========================================
	SUOffscreen::CopyTo
	
	Note:
	May change the fore/back color.
	For speed, should we leave this locked ???
===========================================*/
void SUOffscreen::CopyTo( SUOffscreen *inDest, const Rect *inDestR, 
						 const Rect *inSourceR, SInt16 inMode, 
						 RgnHandle inMaskRgn )
{
	StSaveGWorld		aSaver;						// save/restore the port & device
	BitMap				*sourceBits, *destBits;
	Rect				sourceR, destR;
	GWorldPtr			destWorld;
	
	ThrowIfNil_( inDest );
	
		// prevent CopyBits from colorizing
	this->BeginDrawing();
	this->ResetColorsSelf();
	
		// port & device should be the destination when using CopyBits
	inDest->BeginDrawing();	
	inDest->ResetColorsSelf();

	destWorld = inDest->GetGWorld();
	ThrowIfNil_( destWorld );

#if PP_Target_Carbon
	sourceBits = (BitMap*) *::GetGWorldPixMap(mWorld);
	destBits = (BitMap*) *::GetGWorldPixMap(destWorld);
#else
	sourceBits = &((GrafPtr)mWorld)->portBits;
	destBits = &((GrafPtr)destWorld)->portBits;
#endif

	if ( inSourceR )
		sourceR = *inSourceR;
	else
		SetRect( &sourceR, 0, 0, mWidth, mHeight );
	
	if ( inDestR )
		destR = *inDestR;
	else
#if PP_Target_Carbon
		::GetPortBounds(destWorld, &destR);
#else
		destR = destWorld->portRect;
#endif

	::CopyBits( sourceBits, destBits, &sourceR, &destR, inMode, inMaskRgn );
	
	inDest->EndDrawing();
	this->EndDrawing();
}

/*==========================================
	SUOffscreen::CopyFrom
===========================================*/
void SUOffscreen::CopyFrom( SUOffscreen *inSource, const Rect *destR,
							const Rect *sourceR, SInt16 xferMode,
							RgnHandle inMaskRgn )
{
	ThrowIfNil_( inSource );
	
	inSource->CopyTo( this, destR, sourceR, xferMode, inMaskRgn );
}

/*==========================================
	SUOffscreen::ResetColors

	Description:
	Public version of ResetColors
===========================================*/
void SUOffscreen::ResetColors()
{
	StSaveAndDraw		aDraw( this );
	
	this->ResetColorsSelf();
}

/*==========================================
	SUOffscreen::ResetColorsSelf

	Description:
	Sets foreground to black, background to white.
	 should this use Pm calls ???	
	For speed, use this call instead of ResetColors. This one doesn't
	 set the gdevice, etc.
===========================================*/
void SUOffscreen::ResetColorsSelf()
{
	::RGBForeColor( &kBlackColor );
	::RGBBackColor( &kWhiteColor );
}

/*==========================================
	SUOffscreen::CreateSimilarOffscreen
===========================================*/
SUOffscreen * SUOffscreen::CreateSimilarOffscreen( Boolean inUseSameDevice, SInt32 newWidth, SInt32 newHeight )
{
	StSaveGWorld		aSaver;
	SUOffscreen			*theNewBuffer = nil;
	
		// if no width/height specified, make it the exact same size as our buffer
	if ( newWidth < 0 ) newWidth = mWidth;
	if ( newHeight < 0 ) newHeight = mHeight;
	
	try
	{
		GDHandle theDevice = inUseSameDevice ? ::GetGWorldDevice(mWorld) : nil;
		theNewBuffer = SUOffscreen::CreateBuffer( newWidth, newHeight, mDepth, 
												this->GetColorTable(), theDevice );
		
			// if we have a palette, give one to the new guy too
		if ( mPalette )
			theNewBuffer->AssignPalette( mPalette );
			
		if ( mPalette )
			mPalette->AttachToPort( (GrafPtr) theNewBuffer->GetGWorld() );		// let it share our palette
	}
	catch( ... )
	{
		delete theNewBuffer;
		throw;
	}

	return( theNewBuffer );
}

/*==========================================
	SUOffscreen::GetPalette
===========================================*/
SUSharedPalette *SUOffscreen::GetPalette()
{
	return( mPalette );
}

/*==========================================
	SUOffscreen::AssignPalette
	
	Note:
	Caller can destroy the color table once this call returns.
===========================================*/
void SUOffscreen::AssignPalette( CTabHandle inTable )
{
	ThrowIfNil_( inTable );
	ThrowIfNil_( mWorld );

	SUSharedPalette *aNewPalette = new SUSharedPalette( inTable, (GrafPtr) mWorld );
	
		// kill the old palette
	if ( mPalette )		
		mPalette->DecrementRefCount();
	
	mPalette = aNewPalette;
}

/*==========================================
	SUOffscreen::AssignPalette
	
	Note:
	Caller should not destroy the passed palette, but may decrement its reference
	count by one.
===========================================*/
void SUOffscreen::AssignPalette( SUSharedPalette *inPalette )
{
	ThrowIfNil_( mWorld );

		// kill the old palette
	if ( mPalette )		
	{
		mPalette->DecrementRefCount();
		mPalette = nil;
	}
	
	if ( inPalette )
		inPalette->AttachToPort( (GrafPtr) mWorld );
	else
		NSetPalette( GetWindowFromPort((GrafPtr) mWorld), nil, pmNoUpdates );		// is this ok (to remove palette) ???
		
	mPalette = inPalette;
}

/*==========================================
	SUOffscreen::ConvertToRegion
===========================================*/
RgnHandle SUOffscreen::ConvertToRegion()
{
	RgnHandle			theRgn = nil;
	SUOffscreen			*tempBuffer = nil;

	ThrowIfNil_( mWorld );

	try
	{	
		/***************************************************
			Apple's BitmapToRegion only works on a 1-bit buffer, so if we're
			not a 1-bit buffer, we'll create a temporary one and use it.
		****************************************************/
		if ( mDepth == 1 )
		{
			theRgn = ::NewRgn();
			ThrowIfMemFail_( theRgn );
			
			PixMapHandle		thePixMap = this->GetPixMap();
			StHandleLocker		lock2( (Handle) thePixMap );			// can't hurt

			BitMap *thePortPixMap;
#if PP_Target_Carbon
			thePortPixMap = (BitMap*) *::GetGWorldPixMap(mWorld);
#else
			thePortPixMap = (BitMap*) *mWorld->portPixMap;
#endif
			OSErr err = ::BitMapToRegion( theRgn, thePortPixMap );
			ThrowIfOSErr_( err );
		}
		else
		{
			tempBuffer = SUOffscreen::CreateBuffer( mWidth, mHeight, 1 );
			tempBuffer->CopyFrom( this );
			theRgn = tempBuffer->ConvertToRegion();
		}
	}
	catch( ... )
	{
		if ( theRgn ) ::DisposeRgn( theRgn );
		if ( tempBuffer ) delete( tempBuffer );
		throw;
	}
	
	if ( tempBuffer ) delete( tempBuffer );
	return( theRgn );
}


/*==========================================
	SUOffscreen::EraseToWhite
===========================================*/
void SUOffscreen::EraseToWhite()
{
	StSaveAndDraw		saver( this );		// set port, etc
	StColorPenState		aPenState;

	aPenState.Normalize();
	::RGBBackColor( &kWhiteColor );
	Rect theRect;
#if PP_Target_Carbon
	::GetPortBounds(mWorld, &theRect);
#else
	theRect = mWorld->portRect;
#endif
	EraseRect( &theRect );
}

/*==========================================
	SUOffscreen::CopyFromRawData
	
	Copies data row-by-row into this offscreen buffer.
	This is useful when the rowBytes value of the source is not the same
	  as our buffer.
	  
	Note:
	Depth, height, & width should be the same. RowBytes can be different.
===========================================*/
void SUOffscreen::CopyFromRawData( UInt8 *sourceData, SInt32 sourceRowBytes )
{
	sourceRowBytes = FixRowBytes( sourceRowBytes );		// in case the high 2 bits are set
	
		// never overstep our buffer or we'll trash memory
	sourceRowBytes = MIN( sourceRowBytes, mRowBytes );
	
		// move each row individually
	for ( SInt32 count = 0; count < mHeight; count++ )
	{
		::BlockMoveData( sourceData, mRowArray[count], sourceRowBytes );
		sourceData += sourceRowBytes;
	}
}

/*==========================================
	SUOffscreen::CopyToRawData
	
	Copies data row-by-row from this offscreen buffer.
	This is useful when the rowBytes value of the destination is not the 
	  same as our buffer.

	Note:
	Depth, height, & width should be the same. RowBytes can be different.
===========================================*/
void SUOffscreen::CopyToRawData( UInt8 *destData, SInt32 destRowBytes )
{
	destRowBytes = FixRowBytes( destRowBytes );	// in case the high 2 bits are set
	
		// don't overstep caller's buffer bounds if we have a bigger rowBytes
	destRowBytes = MIN( destRowBytes, mRowBytes );
	
	for ( SInt32 count = 0; count < mHeight; count++ )
	{
		::BlockMoveData( mRowArray[count], destData, destRowBytes );
		destData += destRowBytes;
	}
}

/*==========================================
	CopyFromAndDownSample
	
	Description:
	Copies the passed source buffer into our buffer, replacing every pixel that
	doesn't match inMatchColor to inDestColor. (Useful for masks & lassos).
	
	Note:
	This routine can probably be replaced by CopyBits & a custom searchproc ???
===========================================*/
void SUOffscreen::CopyFromAndDownSample( SUOffscreen *inSource, 
										 Color32 inMatchColor,
										 Color32 inDestColor )
{
	StSaveGWorld		save1;
	SUOffscreen *		tempBuffer = nil;
	
	/********************************************
		if the source & dest aren't the same size, we need to shrink/expand the
		bitmap into a temporary buffer before proceeding. We don't want to let
		CopyBits downsample the colors because its default searchProc chooses
		which pixels map to black based on luminence.
	********************************************/
	SInt32	width = this->GetWidth();
	SInt32	height = this->GetHeight();
	
	if ( (width != inSource->GetWidth()) || (height != inSource->GetHeight()) )
	{
		tempBuffer = inSource->CreateSimilarOffscreen( true, width, height );
		tempBuffer->CopyFrom( inSource );		// shrink/expand the pixels -- don't color resample
		inSource = tempBuffer;
	}
	
	/********************************************
		erase the destination to white since we don't set every pixel in the loop below
	********************************************/
	this->EraseToWhite();
	
	/********************************************
		downsample the scratch buffer -- copy from source to dest, setting
		all pixels in dest to black except where the source is the same as the background
	********************************************/
	Color32			sourceColor;	
	PixelValue 		destPixel = this->Color32ToPixelValue( inDestColor );
	
	for ( long rowCount = 0; rowCount < height; rowCount++ )
		for ( long colCount = 0; colCount < width; colCount++ )
		{
			sourceColor = inSource->GetPixelColor( colCount, rowCount );
			if ( !SUColorUtils::EqualColor32( sourceColor, inMatchColor ) )
				this->SetPixel( colCount, rowCount, destPixel );
		}
		
	if ( tempBuffer )
		delete tempBuffer;
}


/*==========================================
	SUOffscreen::ResetPen
===========================================*/
void SUOffscreen::ResetPen()
{
	StSaveAndDraw		aSaver( this );				// setport
	
	this->ResetColorsSelf();			// fore & back color
	::PenNormal();						// pattern, mode, pensize
}

/*==========================================
	SUOffscreen::SetForeColor
===========================================*/
void SUOffscreen::SetForeColor( Color32 inColor )
{
	StSaveAndDraw	save( this );
	
	RGBColor	theRGB = SUColorUtils::Color32ToRGB( inColor );
	::RGBForeColor( &theRGB );
}

/*==========================================
	SUOffscreen::SetBackColor
===========================================*/
void SUOffscreen::SetBackColor( Color32 inColor )
{
	StSaveAndDraw	save( this );
	
	RGBColor	theRGB = SUColorUtils::Color32ToRGB( inColor );
	::RGBBackColor( &theRGB );
}

/*==========================================
	SUOffscreen::SetForePixelValue
===========================================*/
void SUOffscreen::SetForePixelValue( PixelValue inColor )
{
	StSaveAndDraw	save( this );
	
		// this is more exact than using RGBForeColor
	if ( mPalette )
	{
		::PmForeColor( inColor );
		return;
	}
	
	RGBColor	theRGB = this->PixelValueToRGB( inColor );
	::RGBForeColor( &theRGB );
}

/*==========================================
	SUOffscreen::SetBackPixelValue
===========================================*/
void SUOffscreen::SetBackPixelValue( PixelValue inColor )
{
	StSaveAndDraw	save( this );
	
		// this is more exact than using RGBForeColor
	if ( mPalette )
	{
		::PmBackColor( inColor );
		return;
	}
	
	RGBColor	theRGB = this->PixelValueToRGB( inColor );
	::RGBBackColor( &theRGB );
}

/*==========================================
	SUOffscreen::IsErased
===========================================*/
Boolean SUOffscreen::IsErased()
{
	for ( SInt32 rowCount = 0; rowCount < mHeight; rowCount++ )
	{
		for ( SInt32 colCount = 0; colCount < mWidth; colCount++ )
		{
			Color32 thePixelColor = this->GetPixelColor( colCount, rowCount );
			if ( !SUColorUtils::EqualColor32( thePixelColor, kWhiteColor32 ) )
				return( false );
		}
	}
	
	return( true );
}

/*==========================================
	SUOffscreen::IncreaseInverseTableSize
	
	Description:
	Increases the size of the gworld.gdevice's inverse table to 5-bits
	of resolution. This increases the size from 4K to 32K, but improves
	the color matching quality (from RGB -> index value).
===========================================*/
void SUOffscreen::IncreaseInverseTableSize()
{
	ThrowIfNil_( mWorld );

	GDHandle	theDevice = ::GetGWorldDevice( mWorld );
	SInt32		newSeed = ::GetCTSeed();
	
	if ( (**theDevice).gdResPref < 5 )
	{
			// no good way to get error code, so check for enough RAM first
		SInt32	totalMem, contigMem;
		::PurgeSpace( &totalMem, &contigMem );
		if ( (UInt32)contigMem < 35000 ) LException::Throw( memFullErr );
	
		(**theDevice).gdResPref = 5;
		(**(**theDevice).gdITable).iTabSeed = newSeed;
		::GDeviceChanged( theDevice );
	}	
}

/*==========================================
	SUOffscreen::GetBufferCount
===========================================*/
SInt32 SUOffscreen::GetBufferCount()
{
	return( sNumBuffers );
}

/*==========================================
	SUOffscreen::DebugShowInWindow
===========================================*/
void SUOffscreen::DebugShowInWindow()
{
	StSaveGWorld	aSaver;
	Rect			r, tempR;
	WindowPtr		theWindow = nil;
	OSErr			err = noErr;
	GDHandle		maxDevice;
	
	SetRect( &r, 0, 0, this->GetWidth(), this->GetHeight() );
	
	SetGDevice( GetMainDevice() );
	theWindow = NewCWindow( nil, &r, (StringPtr)"", false, plainDBox, (WindowPtr) -1, false, 0);
	if ( !theWindow ) { err = memFullErr; goto DONE; }

	SetRect( &tempR, -20000, -20000, 20000, 20000 );
	maxDevice = GetMaxDevice( &tempR );
	if ( maxDevice )
		MoveWindow( theWindow, (**maxDevice).gdRect.left + 10, (**maxDevice).gdRect.top + 50, false );
	ShowWindow( theWindow );

		// ebs1.1 -- fixed illegal jump past initializer 10/1/96	
	{
		StHandleLocker		lock2( (Handle) mPixMap );			// can't hurt
		
		SetPortWindowPort( theWindow );
		BitMap *thePixMap;
#if PP_Target_Carbon
		thePixMap = (BitMap*) *::GetPortPixMap(GetWindowPort(theWindow));
#else
		thePixMap = &theWindow->portBits;
#endif
		CopyBits( (BitMap*) *mPixMap, thePixMap, &r, &r, srcCopy, nil );
	}
	
	EventRecord		anEvent;
	while( !GetNextEvent( mDownMask, &anEvent ) )
		;
	
	DONE:
	if ( err )
		SysBeep( 5 );
	if ( theWindow )
		DisposeWindow( theWindow );
}

/*==========================================
	StSaveAndDraw Constructor
===========================================*/
StSaveAndDraw::StSaveAndDraw( SUOffscreen *inBuffer )
{
	::GetGWorld( &mOldPort, &mOldDevice );
	inBuffer->BeginDrawing();
}

/*==========================================
	StSaveAndDraw Destructor
===========================================*/
StSaveAndDraw::~StSaveAndDraw()
{
	::SetGWorld( mOldPort, mOldDevice );
}


/*==========================================
	StBufferLeakDetector Constructor
===========================================*/
StBufferLeakDetector::StBufferLeakDetector()
{
	mNumBuffers = SUOffscreen::GetBufferCount();
}

/*==========================================
	StBufferLeakDetector Destructor
===========================================*/
StBufferLeakDetector::~StBufferLeakDetector()
{
	SInt32	finalBuffers = SUOffscreen::GetBufferCount();
	char	buffer[ 256 ];
	
	if ( finalBuffers > mNumBuffers )
	{
		sprintf( buffer, "buffers before=%ld  after=%ld", mNumBuffers, finalBuffers );
		LString::CToPStr( buffer );
		DebugStr( (unsigned char*) buffer );
	}
}
