// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				DropperAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTDropperAction

DESCRIPTION:		This code is for the eye dropper paint tool
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.12.11	ebs		During paste, shift picture over if it's completely off screen
		96.4.26		ebs		Removed dependence on CSelectionArea since we don't use polys anymore
		96.4.23		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUOffscreen.h"
#include "PTSelection.h"
#include "PTGraphicConversions.h"
#include "SUSaveGWorld.h"
#include "SUMiscUtils.h"
#include "SURegionUtils.h"

/*================================
	Constructor
=================================*/
PTPaintSelection::PTPaintSelection()
{
	mSelectionBuffer = nil;
	mRegion = SURegionUtils::NewRegion();
}

/*================================
	Destructor
=================================*/
PTPaintSelection::~PTPaintSelection()
{
	this->DisposeBuffer();
	SURegionUtils::DisposeRegion( mRegion );
}

/*================================
	DisposeBuffer
=================================*/
void PTPaintSelection::DisposeBuffer()
{
	if ( mSelectionBuffer )
	{
		delete mSelectionBuffer;
		mSelectionBuffer = nil;
	}
}

/*================================
	SelectAll
=================================*/
void PTPaintSelection::SelectAll( SUOffscreen *inBuffer )
{
	Rect	r;
	SetRect( &r, 0, 0, inBuffer->GetWidth(), inBuffer->GetHeight() );
	this->SetSelection( inBuffer, r );
}

/*================================
	SelectNone
=================================*/
void PTPaintSelection::SelectNone()
{
	::SetEmptyRgn( mRegion );
	this->DisposeBuffer();
}

/*================================
	SetSelection
=================================*/
void PTPaintSelection::SetSelection( SUOffscreen *inBuffer, const Rect &inRect )
{
	RgnHandle	theRgn = SURegionUtils::NewRegionFromRect( inRect );
	this->SetSelection( inBuffer, theRgn );
	SURegionUtils::DisposeRegion( theRgn );
}

/*================================
	SetSelection
	
	Note:
	We copy the region and allocate a new selection buffer.
=================================*/
void PTPaintSelection::SetSelection( SUOffscreen *inBuffer, RgnHandle inRegion )
{
	ThrowIfNil_( inBuffer );
	ThrowIfNil_( inRegion );
	
	//StSaveGWorld	aSaver; -- don't do this because the current gworld might be deleted???
	
	Rect	sourceR, destR;
	SInt32	newWidth, newHeight;
	Rect	theRect;
	
#if PP_Target_Carbon
	::GetRegionBounds(inRegion, &theRect);
#else
	theRect = (**inRegion).rgnBBox;
#endif
	newWidth = theRect.right - theRect.left;
	newHeight = theRect.bottom - theRect.top;

	if ( (newWidth <= 0) || (newHeight <= 0) )
	{
		this->SelectNone();
		return;
	}
	
	try
	{
		this->DisposeBuffer();
		
			// make a copy of the region
		this->SetRawSelection( inRegion );
		
			// create a new offscreen buffer and copy the image
#if PP_Target_Carbon
		::GetRegionBounds(inRegion, &sourceR);
#else
		sourceR = (**inRegion).rgnBBox;
#endif
		SetRect( &destR, 0, 0, newWidth, newHeight );
		mSelectionBuffer = inBuffer->CreateSimilarOffscreen( false, newWidth, newHeight );
		
		mSelectionBuffer->CopyFrom( inBuffer, &destR, &sourceR );
	}
	catch( ... )
	{
		this->SelectNone();		// if an error occurs, force no selection or things get weird
		throw;
	}
}

/*================================
	SetSelection
=================================*/
void PTPaintSelection::SetSelection( PTPaintSelection *inSource )
{
	if ( inSource->IsEmpty() )
	{
		this->SelectNone();
		return;
	}
	
	try
	{
		this->DisposeBuffer();
		
		SUOffscreen 		*sourceBuffer = inSource->mSelectionBuffer;
		
		mSelectionBuffer = sourceBuffer->CreateSimilarOffscreen( false );
		mSelectionBuffer->CopyFrom( sourceBuffer );
		
		this->SetRawSelection( inSource->GetRegion() );
	}
	catch( ... )
	{
		this->SelectNone();		// if an error occurs, force no selection or things get weird
		throw;
	}
}


/*================================
	SetRawSelection
	
	Description:
	Copies the region into the internal region structure.
	Does not affect the offscreen bitmap at all.
=================================*/
void PTPaintSelection::SetRawSelection( RgnHandle inRgn )
{
	if ( !inRgn )
		::SetEmptyRgn( mRegion );
	else
	{
		::CopyRgn( inRgn, mRegion );
		ThrowIfOSErr_( QDError() );
	}
}


/*================================
	DrawInto
=================================*/
void PTPaintSelection::DrawInto( SUOffscreen *destBuffer )
{
	if ( !mSelectionBuffer || this->IsEmpty() ) return;
	
	Rect		sourceR, destR;

#if PP_Target_Carbon
	::GetRegionBounds(mRegion, &destR);
#else
	destR = (**mRegion).rgnBBox;
#endif
	sourceR = destR;
	OffsetRect( &sourceR, -sourceR.left, -sourceR.top );	// move offset to 0,0
		
	mSelectionBuffer->CopyTo( destBuffer, &destR, &sourceR, srcCopy, mRegion );
}

/*================================
	GetImageBuffer
=================================*/
SUOffscreen *PTPaintSelection::GetImageBuffer()
{
	return( mSelectionBuffer );
}

/*================================
	CopyToClipboard
=================================*/
void PTPaintSelection::CopyToClipboard()
{
	if ( !mSelectionBuffer || this->IsEmpty() ) return;
	
	PTGraphicConversions::OffscreenToClipboard( mSelectionBuffer, mRegion );
}

/*================================
	PasteFromClipboard
=================================*/
void PTPaintSelection::PasteFromClipboard( SUOffscreen *inParentBuffer )
{
	PicHandle		thePict = nil;
	RgnHandle		theRegion = nil;
	
	try
	{
		/****************************************
			read the picture and (optional) region from the clipboard
		****************************************/
		thePict = PTGraphicConversions::GetPictFromClipboard();
		theRegion = PTGraphicConversions::GetRegionFromClipboard();
		
		/****************************************
			if the region is entirely off-image, move it back
		****************************************/
		Rect r = { 0, 0, inParentBuffer->GetHeight(), inParentBuffer->GetWidth() };
		if ( theRegion && !::RectInRgn( &r, theRegion ) )
		{
			Rect theRect;
#if PP_Target_Carbon
			::GetRegionBounds(theRegion, &theRect);
#else
			theRect = (**theRegion).rgnBBox;
#endif
			::OffsetRgn( theRegion, -theRect.left, -theRect.top );
		}
			
		/****************************************
			paste it
		****************************************/
		this->PastePicture( inParentBuffer, thePict, theRegion );
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( thePict );
		SUMiscUtils::DisposeHandle( theRegion );
		throw;
	}

	SUMiscUtils::DisposeHandle( thePict );
	SUMiscUtils::DisposeHandle( theRegion );
}

/*================================
	PastePicture
=================================*/
void PTPaintSelection::PastePicture( SUOffscreen *inParentBuffer, PicHandle inPict, RgnHandle inRegion )
{
	ThrowIfNil_( inParentBuffer );
	ThrowIfNil_( inPict );
	
	this->SelectNone();

	try
	{
			// note: this is after SelectNone() because it might destroy a gworld
		StSaveGWorld	aSaver;
		Rect			r;
		
		/****************************************
			width & height of our buffer match the picture
		****************************************/
		SInt32	width = (**inPict).picFrame.right - (**inPict).picFrame.left;
		SInt32	height = (**inPict).picFrame.bottom - (**inPict).picFrame.top;

		/****************************************
			create an offscreen gworld using the paint view's gworld as 
			  a base (use same depth, ctable)
		****************************************/
		mSelectionBuffer = inParentBuffer->CreateSimilarOffscreen( false /* device???*/, width, height );

		/****************************************
			image the picture into the offscreen buffer
		****************************************/
		mSelectionBuffer->BeginDrawing();
				// should we erase to the background color instead of white? no.
			SetRect( &r, 0, 0, width, height );
			DrawPicture( inPict, &r );
		mSelectionBuffer->EndDrawing();

		/****************************************
			restore the selection area from the clipboard
			 -- if none, use the entire picture frame
		****************************************/
		if ( inRegion )
			this->SetRawSelection( inRegion );
		else
		{
			RgnHandle tempRgn = SURegionUtils::NewRegionFromRect( r );
			this->SetRawSelection( tempRgn );
			SURegionUtils::DisposeRegion( tempRgn );
		}
	}
	catch( ... )
	{
		this->SelectNone();
		throw;
	}
}

/*================================
	PasteOffscreenBuffer
=================================*/
void PTPaintSelection::PasteOffscreenBuffer( SUOffscreen *inParentBuffer, SUOffscreen *inBuffer, RgnHandle inRegion )
{
	ThrowIfNil_( inParentBuffer );
	ThrowIfNil_( inBuffer );
	
	this->SelectNone();

	try
	{
			// note: this is after SelectNone() because it might destroy a gworld
		StSaveGWorld	aSaver;
		
		/****************************************
			width & height of our buffer match the picture
		****************************************/
		SInt32	width = inBuffer->GetWidth();
		SInt32	height = inBuffer->GetHeight();

		/****************************************
			create an offscreen gworld using the paint view's gworld as 
			  a base (use same depth, ctable)
		****************************************/
		mSelectionBuffer = inParentBuffer->CreateSimilarOffscreen( false /* device???*/, width, height );

		/****************************************
			image the picture into the offscreen buffer
		****************************************/
		mSelectionBuffer->CopyFrom( inBuffer );

		/****************************************
			restore the selection area from the clipboard
			 -- if none, use the entire picture frame
		****************************************/
		if ( inRegion )
			this->SetRawSelection( inRegion );
		else
		{
			RgnHandle tempRgn = SURegionUtils::NewRegionFromRect( 0, 0, width, height );
			this->SetRawSelection( tempRgn );
			SURegionUtils::DisposeRegion( tempRgn );
		}
	}
	catch( ... )
	{
		this->SelectNone();
		throw;
	}
}

/*================================
	IsEmpty
=================================*/
Boolean PTPaintSelection::IsEmpty()
{
#if PP_Target_Carbon
	if ( !mRegion || EmptyRgn(mRegion) )
#else
	if ( !mRegion || !*mRegion || EmptyRgn(mRegion) )
#endif
		return( true );
	else
		return( false );
}

/*================================
	Offset

	Note:
	Does not affect the image.
=================================*/
void PTPaintSelection::Offset( SInt32 dh, SInt32 dv )
{
	::OffsetRgn( mRegion, dh, dv );
}

/*================================
	MoveTo

	Note:
	Does not affect the image.
=================================*/
void PTPaintSelection::MoveTo( SInt32 left, SInt32 top )
{
	Rect theRect;
#if PP_Target_Carbon
	::GetRegionBounds(mRegion, &theRect);
#else
	theRect = (**mRegion).rgnBBox;
#endif
	this->Offset( left - theRect.left, top - theRect.top );
}

/*================================
	GetRegion
=================================*/
RgnHandle PTPaintSelection::GetRegion()
{
	return( mRegion );
}

/*================================
	GetCopyOfRegion
=================================*/
RgnHandle PTPaintSelection::GetCopyOfRegion()
{
	return SURegionUtils::NewRegionFromRegion( mRegion );
}


/*================================
	PointInSelection
=================================*/
Boolean PTPaintSelection::PointInSelection( SInt32 h, SInt32 v )
{
	// if ( this->IsEmpty() ) return( false );
	
	Point	pt;
	SetPt( &pt, h, v );
	return( PtInRgn( pt, mRegion ) );
}

