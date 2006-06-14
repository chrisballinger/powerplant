// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintPictureView.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTPictureView

DESCRIPTION:		This code is for painting PICT resources
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.06.13

CHANGE HISTORY :

		97.02.13	ebs		Set mPrefersIconColors to false in constructor
		96.11.26	ebs		Add include RFResource.h (CW11)
		96.08.07	ebs		Resizing put back in
		96.08.05	ebs		Better default zoom factors
		96.07.09	ebs		Fixed for constructor integration
		96.06.13	ebs		Started

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTPictureView.h"
#include "SUSaveResFile.h"
#include "PTGraphicConversions.h"
#include "SUFileUtils.h"
#include "RFMap.h"
#include "RFResource.h"

/*===============================================
	OpenPaintWindow
================================================*/
PTPictureView* PTPictureView::OpenPaintWindow( ResIDT inPPobID, RFMap *inMap, ResIDT inResID )
{
	PTPictureView		*theView = nil;

	//Throw_( memFullErr );	// // ###???
	
	try
	{
		theView = (PTPictureView*) PTPictureView::CreatePaintWindow( inPPobID );
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
	CreateFromStream
================================================*/
PTPictureView *PTPictureView::CreateFromStream( LStream *inStream )
{
	return new PTPictureView( inStream );
}

/*===============================================
	Constructor
================================================*/
PTPictureView::PTPictureView( LStream *inStream ) : PTPaintView( inStream )
{
	mAllowImageResizing = true;
	mMaxImageWidth = 1024;				// too big ???
	mMaxImageHeight = 1024;
	mPrefersIconColors = false;
}

/*===============================================
	Destructor
================================================*/
PTPictureView::~PTPictureView()
{
}

/*===============================================
	InitializeFromResource
================================================*/
void PTPictureView::InitializeFromResource( RFMap *inMap, ResIDT inResID )
{
	StSaveGWorld		aSaver;
	StSaveResFile		aSaver2;

	SUOffscreen			*theBuffer = nil;
	PicHandle			thePict = nil;
	
	try
	{
		RFResource *theRes = inMap->FindResource( ImageType_Picture, inResID, false );
		if ( !theRes ) LException::Throw( resNotFound );
		thePict = (PicHandle) theRes->GetResData();
		if ( !thePict ) LException::Throw( resNotFound );
		
		theBuffer = PTGraphicConversions::PictureToOffscreen( thePict );
		this->InitializeFromBuffer( theBuffer );
	}
	catch( ... )
	{
		delete theBuffer;
		SUMiscUtils::DisposeHandle( thePict );
		throw;
	}
	
	delete theBuffer;
	SUMiscUtils::DisposeHandle( thePict );
}

#ifdef NOT_USED_WE_DONT_CLONE
/*===============================================
	CloneWindow
================================================*/
PTPaintView *PTPictureView::CloneWindow( Int32 inNewWidth, Int32 inNewHeight, Boolean inScale )
{
	PTPictureView	*newWindow = nil;
	SUOffscreen		*newBuffer = nil, *oldBuffer = nil;
	Rect			destR;
	
	try
	{
		oldBuffer = this->GetCombinedBuffer();
		ThrowIfNil_( oldBuffer );
		
		newBuffer = oldBuffer->CreateSimilarOffscreen( false, inNewWidth, inNewHeight );
		
		if ( inScale )
			::SetRect( &destR, 0, 0, inNewWidth, inNewHeight );
		else
			::SetRect( &destR, 0, 0, oldBuffer->GetWidth(), oldBuffer->GetHeight() );
	
		newBuffer->CopyFrom( oldBuffer, &destR );
	
		newWindow = (PTPictureView*) PTPictureView::CreatePaintWindow( PPob_PictEditor );
		newWindow->InitializeFromBuffer( newBuffer );
	}
	catch( ... )
	{
		delete newBuffer;
		delete newWindow;
		throw;
	}
	
	delete newBuffer;
	return( newWindow );
}
#endif

/*===============================================
	InitializeFromBuffer
================================================*/
void PTPictureView::InitializeFromBuffer( SUOffscreen *inBuffer )
{	
	this->SetImage( inBuffer, PTResize_All );
}

/*===============================================
	SaveAsResource
================================================*/
void PTPictureView::SaveAsResource( RFMap *inMap, ResIDT inResID )
{
	SUOffscreen	*theBuffer = this->GetCombinedBuffer();
	if ( !theBuffer ) return;
		
	Handle h = nil;
	
	try
	{
		h = (Handle) PTGraphicConversions::OffscreenToPicture( theBuffer );
		RFResource *theResource = inMap->FindResource( ImageType_Picture, inResID, true );
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

/*===============================================
	ChangeImageSize
================================================*/
void PTPictureView::ChangeImageSize( SInt32 inWidth, SInt32 inHeight, Boolean inStretch )
{
	ThrowIf_( (inWidth < mMinImageWidth) || (inHeight < mMinImageHeight) );
	ThrowIf_( (inWidth > mMaxImageWidth) || (inHeight > mMaxImageHeight) );
	ThrowIfNil_( mCurrentImage );
	
	Rect		destR;
	SUOffscreen *newBuffer = mCurrentImage->CreateSimilarOffscreen( false, inWidth, inHeight );
	
	try
	{
		if ( inStretch )
			::SetRect( &destR, 0, 0, inWidth, inHeight );
		else
			::SetRect( &destR, 0, 0, mCurrentImage->GetWidth(), mCurrentImage->GetHeight() );
		
		newBuffer->CopyFrom( mCurrentImage, &destR );
		this->SetImage( newBuffer, PTResize_All, redraw_Later );
	}
	catch( ... )
	{
		delete newBuffer;
		throw;
	}
	
	delete newBuffer;
}

