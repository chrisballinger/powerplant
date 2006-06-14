// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LBanner.cp

CLASSES:				LBanner

DESCRIPTION:		Implementation file for LBanner

AUTHOR:				Robin Mair

CREATION DATE :	96.11.05

CHANGE HISTORY :

		96.11.05		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include "LBanner.h"

// � GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <LView.h>
#include <LStream.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// �� STATIC STREAM FUNCTION
//-------------------------------------------------------------------------------------
// LBanner::CreateLBannerStream
//-------------------------------------------------------------------------------------

LBanner*
LBanner::CreateLBannerStream ( LStream* inStream )
{

	return ( new LBanner ( inStream ));
	
}	//	LBanner::CreateLBannerStream


//=====================================================================================
// CLASS:	LBanner
//=====================================================================================

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LBanner::LBanner					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LBanner::LBanner ()
{

	// � Get all of our fields initialized
	mHasBorder = true;
	mHasShading = true;
	mHasFace = true;
	mHasAdornment = false;
	mFaceColor = UGAColorRamp::GetColor (3);
	
}	//	LBanner::LBanner


//-------------------------------------------------------------------------------------
// LBanner::LBanner					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LBanner::LBanner (	LStream*	inStream ) : LView ( inStream )
{

	// � Read in the data from the stream
	inStream->ReadData ( &mHasBorder, sizeof ( Boolean ));
	inStream->ReadData ( &mHasShading, sizeof ( Boolean ));
	inStream->ReadData ( &mHasFace, sizeof ( Boolean ));
	inStream->ReadData ( &mHasAdornment, sizeof ( Boolean ));
	inStream->ReadData ( &mFaceColor, sizeof ( RGBColor ));

}	//	LBanner::LBanner


//-------------------------------------------------------------------------------------
// LBanner::~LBanner				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LBanner::~LBanner ()
{
}	//	LBanner::~LBanner


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS - GETTERS
//-------------------------------------------------------------------------------------
// LBanner::GetBorderRegion
//-------------------------------------------------------------------------------------

RgnHandle
LBanner::GetBorderRegion	(	const Rect	&inRevealed )
{
	// � Create a new region
	RgnHandle	borderRgn = ::NewRgn ();
	
	// � Open the region so that we can get it setup as the border
	// portion of our toolbar
	::OpenRgn ();
		Rect	borderRect;
		CalcLocalFrameRect ( borderRect );
		::FrameRect ( &borderRect );					// Outer edge of border
		::InsetRect ( &borderRect, 1, 1 );
		::FrameRect ( &borderRect );					// Inner edge of border
	::CloseRgn ( borderRgn );
	
	// � Clip the region using the revealed rect
	RgnHandle	revealedRgn = ::NewRgn ();
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( borderRgn, revealedRgn, borderRgn );
	::DisposeRgn ( revealedRgn );
	
	// � Return the region forming the border portion of the banner box
	return borderRgn;
	
}	//	LBanner::GetBorderRegion



//-------------------------------------------------------------------------------------
// LBanner::GetShadingRegion
//-------------------------------------------------------------------------------------

RgnHandle
LBanner::GetShadingRegion	(	const Rect	&inRevealed )
{
	// � Create a new region
	RgnHandle	shadingRgn = ::NewRgn ();
	
	// � Open the region so that we can get it setup as the shading
	// portion of our toolbar
	::OpenRgn ();
		Rect	shadingrRect;
		CalcLocalFrameRect ( shadingrRect );
		::InsetRect ( &shadingrRect, 1, 1 );
		::FrameRect ( &shadingrRect );					// Outer edge of border
		::InsetRect ( &shadingrRect, 1, 1 );
		::FrameRect ( &shadingrRect );					// Inner edge of border
	::CloseRgn ( shadingRgn );
	
	// � Clip the region using the revealed rect
	RgnHandle	revealedRgn = ::NewRgn ();
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( shadingRgn, revealedRgn, shadingRgn );
	::DisposeRgn ( revealedRgn );
	
	// � Return the region forming the border portion of the banner box
	return shadingRgn;
	
}	//	LBanner::GetShadingRegion



//-------------------------------------------------------------------------------------
// LBanner::SetHasBorder
//-------------------------------------------------------------------------------------

void
LBanner::SetHasBorder ( 	Boolean inHasBorder,
										Boolean inRedraw )
{
	if ( mHasBorder != inHasBorder )
	{
		mHasBorder = inHasBorder;
		
		// � Then we need to get the border redrawn, if requested
		if ( inRedraw && FocusDraw ())
			RefreshBannerBorder ();
			
	}
	
}	//	LBanner::SetHasBorder


//-------------------------------------------------------------------------------------
// LBanner::SetHasShading
//-------------------------------------------------------------------------------------

void
LBanner::SetHasShading ( 	Boolean inHasShading,
									Boolean inRedraw )
{
	if ( mHasShading != inHasShading )
	{
		mHasShading = inHasShading;
		
		// � Then we need to get the shading redrawn, if requested
		if ( inRedraw && FocusDraw ())
			RefreshBannerShading ();
			
	}
	
}	//	LBanner::SetHasShading


//-------------------------------------------------------------------------------------
// LBanner::SetHasFace
//-------------------------------------------------------------------------------------

void
LBanner::SetHasFace ( 	Boolean inHasFace,
								Boolean inRedraw )
{
	if ( mHasFace != inHasFace )
	{
		mHasFace = inHasFace;
		
		// � Then we need to get the face redrawn, if requested
		if ( inRedraw && FocusDraw ())
			Refresh ();
			
	}
	
}	//	LBanner::SetHasFace


//-------------------------------------------------------------------------------------
// LBanner::SetHasAdornment
//-------------------------------------------------------------------------------------

void
LBanner::SetHasAdornment ( 	Boolean inHasAdornment,
										Boolean inRedraw )
{
	if ( mHasAdornment != inHasAdornment )
	{
		mHasAdornment = inHasAdornment;
		
		// � Then we need to get the adornment redrawn, if requested
		if ( inRedraw && FocusDraw ())
			Refresh ();
			
	}
	
}	//	LBanner::SetHasAdornment


//-------------------------------------------------------------------------------------
// LBanner::SetFaceColor
//-------------------------------------------------------------------------------------

void
LBanner::SetFaceColor ( 	RGBColor inFaceColor,
									Boolean 	inRedraw )
{
	if ( mFaceColor != inFaceColor )
	{
		mFaceColor = inFaceColor;
		
		// � Then we need to get the banner redrawn, if requested
		if ( inRedraw && FocusDraw ())
			Refresh ();
			
	}
	
}	//	LBanner::SetHasFace


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//------------------------------------------------------------------------------------
//	LBanner::DrawSelf
//------------------------------------------------------------------------------------
//

void
LBanner::DrawSelf ()
{
	
	StColorPenState::Normalize ();

	// � Get the border for the banner drawn
	if ( HasFace () )
		DrawBannerFace ();
	
	// � Get the border for the banner drawn
	if ( HasBorder () )
		DrawBannerBorder ();
	
	// � Get the shading for the banner drawn
	if ( HasShading ())
		DrawBannerShading ();
	
	// � Get the adornment for the banner drawn
	if ( HasAdornment ())
		DrawBannerAdornment ();
	
}	//	LBanner::DrawSelf


//------------------------------------------------------------------------------------
//	LBanner::DrawBannerFace
//------------------------------------------------------------------------------------
//

void
LBanner::DrawBannerFace ()
{
	
	StColorPenState	theColorState;
	theColorState.Normalize ();
	Rect	faceRect;
	CalcLocalFrameRect ( faceRect );
	
	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( faceRect );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � The face is painted in white
			::RGBForeColor ( &UGAColorRamp::GetWhiteColor () );
			::PaintRect ( &faceRect );
		}
		// � COLOR
		else
		{
			// � The face is paint with the face color
			::RGBForeColor ( &mFaceColor );
			::PaintRect ( &faceRect );
		}
	}
		
}	//	LBanner::DrawBannerFace


//------------------------------------------------------------------------------------
//	LBanner::DrawBannerBorder
//------------------------------------------------------------------------------------
//

void
LBanner::DrawBannerBorder ()
{
	
	StColorPenState	theColorState;
	theColorState.Normalize ();
	Rect					borderRect;
	CalcLocalFrameRect ( borderRect );
	
	// � The border is always drawn in black
	::RGBForeColor ( &UGAColorRamp::GetBlackColor () );
	::FrameRect ( &borderRect );
	
}	//	LBanner::DrawBannerBorder


//-------------------------------------------------------------------------------------
// LBanner::DrawBannerShading	
//-------------------------------------------------------------------------------------
//

void
LBanner::DrawBannerShading ()
{
	
	StColorPenState	theColorPenState;
	StColorPenState::Normalize ();
	
	Rect		shadingRect;
	CalcLocalFrameRect ( shadingRect );
	::InsetRect ( &shadingRect, 1, 1 );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( shadingRect );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � We only draw the shading is we have 4 bits of screen depth
		if ( depth >= 4 )
		{
			// � Frame the light portion of the shading which is in white
			::RGBForeColor ( &UGAColorRamp::GetWhiteColor() );
			UGraphicsUtilities::TopLeftSide ( &shadingRect, 0, 0, 1, 1 );

			// � Frame the dark portion of the shading which is done by
			// darkening the face color
			::RGBForeColor ( &(UGraphicsUtilities::Darken ( &mFaceColor )) );
			UGraphicsUtilities::BottomRightSide ( &shadingRect, 1, 1, 0, 0 );
		}
	}
		
}	// LBanner::DrawBannerShading


//-------------------------------------------------------------------------------------
// LBanner::DrawBannerAdornment	
//-------------------------------------------------------------------------------------
//

void
LBanner::DrawBannerAdornment ()
{
}	// LBanner::DrawBannerAdornment


//------------------------------------------------------------------------------------
//	LBanner::RefreshBannerBorder
//------------------------------------------------------------------------------------
//
//	Invalidate the border area of the banner so it will 
//	get redrawn during the next update

void
LBanner::RefreshBannerBorder ()
{
	// � We are only going to worry about doing a refresh if the toolbar 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if ( IsVisible ()  && CalcLocalFrameRect ( theFrame )  && ( mSuperView )) 
	{

		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			// � Get the content region figured out so that we can use it for the inval
			RgnHandle	borderRgn = GetBorderRegion ( revealed );
			
			// � Get the content invalidated so that it will be refreshed
			::InvalRgn ( borderRgn );
						
			// � Cleanup
			::DisposeRgn ( borderRgn );
		}
	}
	
}	//	LBanner::RefreshBannerBorder


//------------------------------------------------------------------------------------
//	LBanner::RefreshBannerShading
//------------------------------------------------------------------------------------
//
//	Invalidate the shading area of the banner so it will 
//	get redrawn during the next update

void
LBanner::RefreshBannerShading ()
{
	// � We are only going to worry about doing a refresh if the toolbar 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if ( IsVisible ()  && CalcLocalFrameRect ( theFrame )  && ( mSuperView )) 
	{
		::InsetRect ( &theFrame, 1, 1 );
		
		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			// � Get the shading region invalidated
			RgnHandle	shadingRgn = GetShadingRegion ( revealed );
			::InvalRgn ( shadingRgn );
			::DisposeRgn ( shadingRgn );
		}
	}
	
}	//	LBanner::RefreshBannerShading


