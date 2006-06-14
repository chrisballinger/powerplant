// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUColorTableBuilder.cp

PROJECT:			Constructor Icon Editor

CLASSES:			SUColorTableBuilder

DESCRIPTION:		This code is used for scanning pixmaps and generating color tables
					containing the actual colors used. It only works on 32-bit pixmaps
					right now.
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.07.13

CHANGE HISTORY :

		96.07.29	ebs		Fixed memory leak (deletion of helper in DownSampleBuffer)
		96.07.25	ebs		Support for non-32 bit buffers (icon family)
		96.07.19	ebs		DownSampleBuffer() now takes maxDepth parameter
		96.07.18	ebs		Support for forcing b&w, minimizing tables
		96.07.16	ebs		Changed to new ColorHash method
		96.07.16	ebs		Changed statics to virtual to allow more flexibility
		96.07.14	ebs		Added option of *not* putting black & white in table
		96.07.13	ebs		Created File 

---------------------------------------<< ¥ >>-----------------------------------------
*/

/*
	Strategy:
	Apple's Picture Utilities can build us a ColorTable based on a PixMap. But they
	use only 5 bits of each RGB pixel to generate their histograms and this will
	reduce the color fidelity of our images.
	
	Instead, we will first scan the pixmap and see how many unique colors are used.
	If that number will fit in the color table for the desired buffer depth, then
	we can use a color table full of exact values. SUColorHash will be used to build 
	this table and to convert from Color32 values in the source buffer to indexes
	in the destination.
	
	If the source image uses more colors than the table can handle, we'll use the
	Picture Utilities to generate the table and CopyBits() to downsample.

	Notes:
	This code only works on 32-bit offscreens right now. It would be fairly easy
	to modify for other depths, but it would take a little work do so without
	slowing down the routine.
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SU_Macros.h"
#include "SUOffscreen.h"
#include "SUColorTableBuilder.h"
#include "SUColorUtils.h"
#include "SUColorHash.h"
#include <PictUtils.h>

#include <UMemoryMgr.h>

/*=================================
	DownSampleBuffer (static routine)
	
	Note:
	Returns nil instead of allocating a 32-bit buffer since we would
	just be wasting RAM. This only happens if inMaxDepth is 32.
==================================*/
SUOffscreen *SUColorTableBuilder::DownSampleBuffer( SUOffscreen *inSource, SInt32 inMaxDepth,
													Boolean inIncludeBW, Boolean inMinimizeTable, 
													Boolean inLargerInverse )
{
//	SUOffscreen				*destBuffer = nil;

	StDeleter<SUOffscreen>	destBuffer;
	
	try
	{	
		StDeleter<SUColorTableBuilder>	helper(new SUColorTableBuilder(inIncludeBW) );;
		
		helper->SetBuffer( inSource );
		helper->SetIncludeBW( inIncludeBW );				// prob not necessary anymore
		helper->SetMinimizeTable( inMinimizeTable );
		helper->SetLargerInverseTable( inLargerInverse );
	
		destBuffer.Adopt(helper->CreateDownSampledBuffer( inMaxDepth ));
	}
	catch (const LException& iErr) {
		
		// JCD 980619
		//
		// the call chain of:		
		// CreateDownSampledBuffer
		//  DoScan
		//   AddColorToHash
		//    GetIndex
		//
		//	Has the potential to throw a paramErr if the hash table
		// 	is full. This should never happen, but it has (MW07312).
		//	If we happen to catch this paramErr, we'll say that's ok,
		//	just return a nil destBuffer, and hopefully the bitmap
		//	code will not downsample and just use a 32-bit.
		
		
		if ( iErr.GetErrorCode() != err_HashFull)
			throw;
	}
		
	return( destBuffer.Release() );
}


/*=================================
	Constructor
==================================*/
SUColorTableBuilder::SUColorTableBuilder( Boolean inIncludeBW )
{
	mBuffer = nil;
	mIncludeBW = inIncludeBW;
	mMinimizeTable = true;
	mPictUtilMethod = 0;				// default Apple method
	mScanIsDone = false;
	mLargerInverseTable = false;
	mUseCopyBits = false;

	mColorHash = new SUColorHash( inIncludeBW );
	ThrowIfMemFail_( mColorHash );
}

/*=================================
	Destructor
==================================*/
SUColorTableBuilder::~SUColorTableBuilder()
{
	delete mColorHash;
}

/*=================================
	SetBuffer
==================================*/
void SUColorTableBuilder::SetBuffer( SUOffscreen *inBuffer )
{
	mBuffer = inBuffer;
}

/*=================================
	SetIncludeBW
==================================*/
void SUColorTableBuilder::SetIncludeBW( Boolean inBW )
{
	mIncludeBW = inBW;
}

/*=================================
	SetMinimizeTable
==================================*/
void SUColorTableBuilder::SetMinimizeTable( Boolean inMinimizeTable )
{
	mMinimizeTable = inMinimizeTable;
}

/*=================================
	SetPictUtilMethod
==================================*/
void SUColorTableBuilder::SetPictUtilMethod( short inMethod )
{
	mPictUtilMethod = inMethod;
}

/*=================================
	SetLargerInverseTable
==================================*/
void SUColorTableBuilder::SetLargerInverseTable( Boolean inValue )
{
	mLargerInverseTable = inValue;
}

/*=================================
	CountUniqueColors
==================================*/
SInt32 SUColorTableBuilder::GetColorCount()
{
	this->DoScan();
	return( mColorHash->GetColorCount() );
}

/*=================================
	GetColorTable
	
	Note:
	Caller is responsible for getting rid of the table (using ::DisposeCTable).
==================================*/
CTabHandle SUColorTableBuilder::GetColorTable( SInt32 inMaxDepth, SInt32 *outDepth )
{
	/**************************************************
		find out how much depth we need to display all of the pixels correctly
	**************************************************/
	SInt32 	uniqueColors = this->GetColorCount();
	SInt32	depthForActualColors = SUColorUtils::NumColorsToDepth( uniqueColors );

	/**************************************************
		limit the destination depth depending on inMaxDepth
	**************************************************/
	switch( inMaxDepth )
	{
		case 1:
				// is this correct??? prob doesn't matter since we only call
				// this with depths of 8 or 32-bits
			*outDepth = 1;
			return( SUColorUtils::GetColorTable( 1 ) );		// 1-bit -> no thinking
		
		case 2:
			*outDepth = MIN( 2, depthForActualColors );
			break;

		case 4:
			*outDepth = MIN( 4, depthForActualColors );
			break;

		case 8:		
			*outDepth = MIN( 8, depthForActualColors );
			break;
		
		case 16:
			*outDepth = MIN( 16, depthForActualColors );
			if ( *outDepth == 32 )
				*outDepth = 16;
			break;
					
		case 32:
			*outDepth = MIN( 32, depthForActualColors );
			break;
					
		default:
			LException::Throw( paramErr );
	}
	
	/**************************************************
		for 16 & 32-bit pixmaps, we'll use CopyBits but there's no need
		to build up a color table or anything.
	**************************************************/
	if ( (*outDepth == 16) || (*outDepth == 32) )
	{
		mUseCopyBits = true;
		return( SUColorUtils::GetColorTable( *outDepth ) );
	}
	
	/**************************************************
		if there are too many colors in the image, call Apple's
		Pict Utils to create a color table for us. Otherwise, just
		generate a table from mColorArray.
	**************************************************/
	SInt32 			maxColors = SUColorUtils::DepthToNumColors( *outDepth );
	CTabHandle		theTable = nil;

	if ( uniqueColors > maxColors )
	{
		PictInfo	info;
		short 		flags = returnColorTable | ( mIncludeBW ? 0 : suppressBlackAndWhite );
		
		::GetPixMapInfo( 
						mBuffer->GetPixMap(), 		// the bits
						&info, 						// results go here
						flags, 						// from above
						maxColors, 					// number of colors in output table (incl b&w)				
						mPictUtilMethod, 			// Pict Util method to use (0 = default)
						0 );						// version

		theTable = info.theColorTable;
		
			// we need to use CopyBits since we didn't generate the
			// color table and can't use the hash lookup
		mUseCopyBits = true;
	}
	else
	{
		theTable = mColorHash->BuildColorTable( mMinimizeTable );
		mUseCopyBits = false;
	}
	
	return( theTable );
}

/*=================================
	DoScan
==================================*/
void SUColorTableBuilder::DoScan()
{
	if ( mScanIsDone ) return;
	ThrowIfNil_( mBuffer );

	RawPtr		rowPtr;
	SInt32		height = mBuffer->GetHeight();
	SInt32		width = mBuffer->GetWidth();
	
	if ( mIncludeBW )
	{
		mColorHash->AddColorToHash( kWhiteColor32 );
		mColorHash->AddColorToHash( kBlackColor32 );
	}
	
	for ( SInt32 rowCount = 0; rowCount < height; rowCount++ )
	{
		rowPtr = mBuffer->RawGetRowPtr( rowCount );
		
		for ( SInt32 colCount = 0; colCount < width; colCount++ )
		{
			Color32		theColor = mBuffer->RawGetPixelColor( rowPtr, colCount );
			mColorHash->AddColorToHash( theColor );
		}
	}
	
	mScanIsDone = true;
}

/*=================================
	CreateDownSampledBuffer
	
	Note:
	returns nil instead of a 32-bit buffer. This only happens if inMaxDepth is 32.
==================================*/
SUOffscreen *SUColorTableBuilder::CreateDownSampledBuffer( SInt32 inMaxDepth )
{
	this->DoScan();

	SInt32			depth;	
	SInt32			width = mBuffer->GetWidth();
	SInt32			height = mBuffer->GetHeight();
	RawPtr			sourceRow, destRow;
	SUOffscreen		*destBuffer = nil;
	CTabHandle		theTable = nil;
	
	try
	{
			// get the depth & color table
		theTable = this->GetColorTable( inMaxDepth, &depth );
		
			// no need to allocate 32-bit buffers -- waste of RAM because
			// the source buffer can be used instead of this one
		if ( depth == 32 )
		{
			::DisposeCTable( theTable );
			return( nil );
		}
		
			// allocate the buffer to downsample into
		destBuffer = SUOffscreen::CreateBuffer( width, height, depth, theTable );
		
		/*
			if we had too many colors to do an exact match, use CopyBits
			instead of the color hash.
		*/
		if ( mUseCopyBits )
		{
			if ( mLargerInverseTable && (depth < 16) )
				destBuffer->IncreaseInverseTableSize();
			
			destBuffer->CopyFrom( mBuffer );
		}
		else
		{			
			for ( SInt32 rowCount = 0; rowCount < height; rowCount++ )
			{
				sourceRow = mBuffer->RawGetRowPtr( rowCount );
				destRow = destBuffer->RawGetRowPtr( rowCount );
				
				for ( SInt32 colCount = 0; colCount < width; colCount++ )
				{
					Color32		sourceColor = mBuffer->RawGetPixelColor( sourceRow, colCount );
					PixelValue 	destPixel = mColorHash->LookupAssociatedNumber( sourceColor );
					destBuffer->SetPixel( colCount, rowCount, destPixel );
				}
			}
		}
	}
	catch( ... )
	{
		delete destBuffer;
		if ( theTable ) ::DisposeCTable( theTable );
		throw;
	}
	
	if ( theTable ) ::DisposeCTable( theTable );
	return( destBuffer );
}

