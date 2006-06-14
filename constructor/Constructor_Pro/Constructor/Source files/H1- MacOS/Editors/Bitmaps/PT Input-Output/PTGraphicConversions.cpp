// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PTGraphicConversions.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTGraphicConversions

DESCRIPTION:		Misc i/o support code
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.11.25	ebs		Support for CW11 (added include RFResource.h)
		96.4.26		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTGraphicConversions.h"
#include "SUOffscreen.h"
#include "PT_Resources.h"		// for resource types
#include "SUSaveGWorld.h"
#include "SURegionUtils.h"
#include "SUMiscUtils.h"
#include "SUSparePort.h"
#include "SUFileUtils.h"
#include "SUSaveResFile.h"
#include "SUColorUtils.h"
#include "SUColorTableBuilder.h"
#include "PTPrefs.h"
#include "SUDisposeOnExit.h"
#include "RFResource.h"
#include "RFMap.h"

#include "PT_Errors.h"

#include <stdio.h>		// temp debug ???

/*=====================================
	OffscreenToPicture
	
	Note:
	The selection region is relative to the painting itself and not the selection
	buffer. This allows us to restore the original image position on a "paste".
======================================*/
PicHandle PTGraphicConversions::OffscreenToPicture( SUOffscreen *inBuffer, RgnHandle inSelectionRgn )
{
	StSaveGWorld		aSaver;					// save/restore gworld
	PicHandle			thePict = nil;
	Rect				sourceR, destR;
	GrafPtr				aSparePort;
	RgnHandle			maskRgn = nil;
	SUOffscreen	*		downSampledBuffer = nil;

	try
	{
		/*
			downsample the buffer if we can
		*/
		downSampledBuffer = SUColorTableBuilder::DownSampleBuffer( inBuffer, 
																	32,			// max depth
																	true, 		// b&w included
																	true, 		// minimize tbl
																	true );		// larger inverse
		if ( downSampledBuffer )
			inBuffer = downSampledBuffer;
		 
		if ( inSelectionRgn )
		{
			maskRgn = SURegionUtils::NewRegionFromRegion( inSelectionRgn );
			Rect theRect;
#if PP_Target_Carbon
			::GetRegionBounds(maskRgn, &theRect);
#else
			theRect = (**maskRgn).rgnBBox;
#endif
			
			::OffsetRgn( maskRgn, -theRect.left, -theRect.top );
			
#if PP_Target_Carbon
			::GetRegionBounds(maskRgn, &sourceR);
#else
			sourceR = (**maskRgn).rgnBBox;
#endif
			
			destR = sourceR;
			//OffsetRect( &destR, -destR.left, -destR.top );
		}
		else		// use the entire buffer
		{
			::SetRect( &sourceR, 0, 0, inBuffer->GetWidth(), inBuffer->GetHeight() );
			destR = sourceR;
			maskRgn = nil;
		}
		
		aSparePort = SUSparePort::GetSparePort();
		SUMiscUtils::SetPort( aSparePort );
		
		thePict = ::OpenPicture( &destR );
		ThrowIfNil_( thePict );

		::ClipRect( &destR );				// work-around clip region bug (see tech notes)
		inBuffer->CopyTo( aSparePort, &destR, &sourceR, srcCopy, maskRgn );
		::ClosePicture();
		
		// qd error sometimes shows errors that don't exist -- should we
		// check it here anyway ???
	}
	catch( ... )
	{
		delete downSampledBuffer;
		if ( maskRgn ) ::DisposeRgn( maskRgn );
		if ( thePict ) ::KillPicture( thePict );
		throw;
	}
	
	if ( maskRgn ) ::DisposeRgn( maskRgn );
	delete downSampledBuffer;
	return( thePict );
}

/*=====================================
	OffscreenToClipboard
======================================*/
void PTGraphicConversions::OffscreenToClipboard( SUOffscreen *inBuffer, RgnHandle inSelection )
{
	PicHandle			thePict = nil;
	LClipboard			*theClipboard = LClipboard::GetClipboard();
	StSaveGWorld		aSaver;				// save/restore gworld
	
		// convert the offscreen buffer to a picture
	thePict = OffscreenToPicture( inBuffer, inSelection );
	
		// delete the picture on exit (handles case when errors are thrown)
	StDisposeHandle		aDeleter( (Handle) thePict );
	
		// write the picture 
	theClipboard->SetData( ImageType_Picture, (Handle) thePict, true /* reset */ );

	if ( inSelection )
		theClipboard->SetData( ResourceType_Region, (Handle) inSelection, false );
}

/*=====================================
	GetPictFromClipboard
	
	Note:
	Throws an error if there's no pict on the clipboard.
======================================*/
PicHandle PTGraphicConversions::GetPictFromClipboard()
{
	PicHandle		thePict = nil;
	SInt32			numBytes;
	LClipboard		*theClipboard = LClipboard::GetClipboard();

	try
	{
		thePict = (PicHandle) ::NewHandle( 0 );
		ThrowIfMemFail_( thePict );
		
		numBytes = theClipboard->GetData( ImageType_Picture, (Handle) thePict );
		if ( numBytes < static_cast<SInt32>(sizeof(Picture)) )
			Throw_( noTypeErr );
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( (Handle) thePict );
		throw;	
	}
	
	return( thePict );
}

/*=====================================
	GetRegionFromClipboard
	
	Note:
	Returns nil if the clipboard doesn't have a region rather
	 than throwing an error.
======================================*/
RgnHandle PTGraphicConversions::GetRegionFromClipboard()
{
	RgnHandle		theRegion = nil;
	SInt32			numBytes;
	LClipboard		*theClipboard = LClipboard::GetClipboard();

	numBytes = theClipboard->GetData( ResourceType_Region, nil );
	if ( numBytes <= 0 ) return( nil );
	
	try
	{
		theRegion = (RgnHandle) ::NewHandle( numBytes );
		ThrowIfMemFail_( theRegion );
		
		numBytes = theClipboard->GetData( ResourceType_Region, (Handle) theRegion );
		if ( numBytes <= 0 )
			Throw_( noTypeErr );
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( (Handle) theRegion );
		throw;	
	}
	
	return( theRegion );
}

/*=====================================
	GetPictFromFile
======================================*/
PicHandle PTGraphicConversions::GetPictFromFile( const FSSpec &inSpec )
{
	PicHandle	thePict = nil;
	long		numBytes;
	OSErr		err = noErr;
	short		fileID = 0;
	
	try
	{
		err = ::FSpOpenDF( &inSpec, fsRdPerm, &fileID );
		if ( err ) 
		{
			fileID = 0;
			Throw_( err );
		}

		err = ::GetEOF( fileID, &numBytes );
		ThrowIfOSErr_( err );
		
		numBytes -= 512;
		if ( numBytes <= 0 ) 
			Throw_( err_CorruptedFile );
			
		thePict = (PicHandle) ::NewHandle( numBytes );
		ThrowIfMemFail_( thePict );
		
		err = ::SetFPos( fileID, fsFromStart, 512 );
		ThrowIfOSErr_( err );
		
		err = ::FSRead( fileID, &numBytes, *thePict );
		ThrowIfOSErr_( err );
	}
	catch( ... )
	{
		if ( fileID )
			::FSClose( fileID );
		throw;
	}	

	if ( fileID )
		FSClose( fileID );	
	
	return( thePict );
}

/*=================================
	SaveOffscreenAsResource
	
	Description:
	Saves the passed offscreen buffer as a raw image resource.
	This is useful for ICON, icl8, icl4, etc.
	
	Notes:
	Throws an error on i/o problem.
	May unlock the resource if it already is present in RAM.
==================================*/
void PTGraphicConversions::SaveOffscreenAsResource( 
								RFMap *inMap, ResType inResType, ResIDT inResID,
								SUOffscreen *inBuffer, SInt32 rowBytes,
								SUOffscreen *inMask, SInt32 maskOffset, SInt32 maskRowBytes )
{
	UInt8				**h = nil;
	SInt32				numBytes;
	
	/*******************************************
		save/restore various states
	*******************************************/
	StSaveResFile		saver1;
	StSaveGWorld		saver2;
	
	/*******************************************
		if the rowBytes (or maskRowBytes) isn't specified, use the
		buffer's rowBytes
	*******************************************/
	if ( rowBytes == -1 )
		rowBytes = inBuffer->GetRowBytes();
	
	if ( inMask && (maskRowBytes == -1) )
	{
		maskRowBytes = inMask->GetRowBytes();
	}

	try
	{
		/*******************************************
			Allocate the buffer to write
		*******************************************/
		numBytes = (rowBytes + maskRowBytes) * inBuffer->GetHeight();
		h = (UInt8**) ::NewHandle( numBytes );
		ThrowIfMemFail_( h );
		::HLock( (Handle) h );
		
		/*******************************************
			Copy the raw image & mask data to the resource
		*******************************************/
		inBuffer->CopyToRawData( *h, rowBytes );
		if ( inMask )
			inMask->CopyToRawData( *h + maskOffset, maskRowBytes );

		/*******************************************
			Write out the resource
		*******************************************/
		RFResource *theResource = inMap->FindResource( inResType, inResID, true );
		ThrowIfNil_( theResource );
		
		theResource->SetResData( (Handle) h );
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( h );
		throw;
	}
	
	SUMiscUtils::DisposeHandle( h );
}

/*=====================================
	PictureToOffscreen
	
	Description:
	Creates an offscreen buffer and draws the picture into it.
======================================*/
SUOffscreen *PTGraphicConversions::PictureToOffscreen( PicHandle inPict )
{
	StSaveGWorld	aSaver;
	
	SInt32	width = (**inPict).picFrame.right - (**inPict).picFrame.left;
	SInt32	height = (**inPict).picFrame.bottom - (**inPict).picFrame.top;
	SInt32	depth;
	
	if ( (width <= 0) || (height <= 0) ) 
		LException::Throw( err_InvalidImageSize );
	
	Rect	r;
	SetRect( &r, 0, 0, width, height );
	
	if ( PTPrefs::Use32BitBuffersForPicts() )
		depth = 32;
	else
		depth = 8;
	
	SUOffscreen *theBuffer = SUOffscreen::CreateBuffer( width, height, depth );
	
	theBuffer->BeginDrawing();
		::DrawPicture( inPict, &r );
	theBuffer->EndDrawing();
	
	return( theBuffer );
}



