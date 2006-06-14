// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAWindowHeader.cp

CLASSES:				LGAWindowHeader

DESCRIPTION:		Implementation file for LGAWindowHeader

AUTHOR:				Robin Mair

CREATION DATE :	97.03.03

CHANGE HISTORY :

		97.03.26		rtm	Added support for having a divider at the bottom of the header
								or not, this was done to match features of the window header
								provided by Apple with the Appearance manager.
		97.03.07		rtm	Added some missing constructors.
		97.03.03		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include <LGAWindowHeader.h>

// е GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>

// е TOOLBOX HEADERS
#include <MacWindows.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// CLASS:	LGAWindowHeader
//=====================================================================================

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAWindowHeader::LGAWindowHeader					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default Constructor

LGAWindowHeader::LGAWindowHeader ()
{

	// е Get all of our fields initialized
	mHasAdornment = false;
	mHasBottomDivider = false;
	
}	//	LGAWindowHeader::LGAWindowHeader


//-------------------------------------------------------------------------------------
// LGAWindowHeader::LGAWindowHeader					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy Constructor

LGAWindowHeader::LGAWindowHeader (	const	LGAWindowHeader &inOriginal )
								:	LView ( inOriginal )
{		

	// е Get all of our fields initialized
	mHasAdornment = inOriginal.mHasAdornment;
	mHasBottomDivider = inOriginal.mHasBottomDivider;
	
}	//	LGAWindowHeader::LGAWindowHeader


//-------------------------------------------------------------------------------------
// LGAWindowHeader::LGAWindowHeader					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream Constructor

LGAWindowHeader::LGAWindowHeader (	LStream*	inStream ) : LView ( inStream )
{

	// е Read in the data from the stream
	*inStream >> mHasAdornment;
	*inStream >> mHasBottomDivider;

}	//	LGAWindowHeader::LGAWindowHeader


//-------------------------------------------------------------------------------------
// LGAWindowHeader::LGAWindowHeader					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized Constructor

LGAWindowHeader::LGAWindowHeader (	const SPaneInfo	&inPaneInfo,
												const SViewInfo	&inViewInfo,
												Boolean				inHasAdornment,
												Boolean				inHasDivider ) 
								: LView ( 	inPaneInfo,
												inViewInfo )
{

	// е Setup the adornment field
	mHasAdornment = inHasAdornment;
	mHasBottomDivider = inHasDivider;
	
}	//	LGAWindowHeader::LGAWindowHeader


//-------------------------------------------------------------------------------------
// LGAWindowHeader::~LGAWindowHeader				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAWindowHeader::~LGAWindowHeader ()
{
}	//	LGAWindowHeader::~LGAWindowHeader


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS - GETTERS
//-------------------------------------------------------------------------------------
// LGAWindowHeader::GetBorderRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGAWindowHeader::GetBorderRegion	(	const Rect	&inRevealed )
{
	// е Create a new region
	RgnHandle	borderRgn = ::NewRgn ();
	
	// е Open the region so that we can get it setup as the border
	// portion of our toolbar
	::OpenRgn ();
		Rect	borderRect;
		CalcLocalFrameRect ( borderRect );
		::FrameRect ( &borderRect );					// Outer edge of border
		::InsetRect ( &borderRect, 1, 1 );
		::FrameRect ( &borderRect );					// Inner edge of border
	::CloseRgn ( borderRgn );
	
	// е Clip the region using the revealed rect
	RgnHandle	revealedRgn = ::NewRgn ();
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( borderRgn, revealedRgn, borderRgn );
	::DisposeRgn ( revealedRgn );
	
	// е Return the region forming the border portion of the banner box
	return borderRgn;
	
}	//	LGAWindowHeader::GetBorderRegion



//-------------------------------------------------------------------------------------
// LGAWindowHeader::GetShadingRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGAWindowHeader::GetShadingRegion	(	const Rect	&inRevealed )
{
	// е Create a new region
	RgnHandle	shadingRgn = ::NewRgn ();
	
	// е Open the region so that we can get it setup as the shading
	// portion of our toolbar
	::OpenRgn ();
		Rect	shadingrRect;
		CalcLocalFrameRect ( shadingrRect );
		::InsetRect ( &shadingrRect, 1, 1 );
		::FrameRect ( &shadingrRect );					// Outer edge of border
		::InsetRect ( &shadingrRect, 1, 1 );
		::FrameRect ( &shadingrRect );					// Inner edge of border
	::CloseRgn ( shadingRgn );
	
	// е Clip the region using the revealed rect
	RgnHandle	revealedRgn = ::NewRgn ();
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( shadingRgn, revealedRgn, shadingRgn );
	::DisposeRgn ( revealedRgn );
	
	// е Return the region forming the border portion of the banner box
	return shadingRgn;
	
}	//	LGAWindowHeader::GetShadingRegion



//-------------------------------------------------------------------------------------
// LGAWindowHeader::SetHasAdornment
//-------------------------------------------------------------------------------------

void
LGAWindowHeader::SetHasAdornment ( 	Boolean inHasAdornment,
										Boolean inRedraw )
{
	if ( mHasAdornment != inHasAdornment )
	{
		mHasAdornment = inHasAdornment;
		
		// е Then we need to get the adornment redrawn, if requested
		if ( inRedraw && FocusDraw ())
			Refresh ();
			
	}
	
}	//	LGAWindowHeader::SetHasAdornment


//-------------------------------------------------------------------------------------
// LGAWindowHeader::SetHasBottomDivider
//-------------------------------------------------------------------------------------

void
LGAWindowHeader::SetHasBottomDivider ( Boolean inHasBottomDivider,
													Boolean inRedraw )
{
	if ( mHasBottomDivider != inHasBottomDivider )
	{
		mHasBottomDivider = inHasBottomDivider;
		
		// е Then we need to get the adornment redrawn, if requested
		if ( inRedraw && FocusDraw ())
			Refresh ();
			
	}
	
}	//	LGAWindowHeader::SetHasAdornment


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAWindowHeader::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGAWindowHeader::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGAWindowHeader::ActivateSelf



//-------------------------------------------------------------------------------------
// LGAWindowHeader::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGAWindowHeader::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGAWindowHeader::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//------------------------------------------------------------------------------------
//	LGAWindowHeader::DrawSelf
//------------------------------------------------------------------------------------
//

void
LGAWindowHeader::DrawSelf ()
{
	
	StColorPenState::Normalize ();

	// е Get the border for the banner drawn
	DrawHeaderFace ();
	
	// е Get the border for the banner drawn
	DrawHeaderBorder ();
	
	// е Get the shading for the banner drawn
	DrawHeaderShading ();
	
	// е Get the adornment for the banner drawn
	if ( HasAdornment ())
		DrawHeaderAdornment ();
	
}	//	LGAWindowHeader::DrawSelf


//------------------------------------------------------------------------------------
//	LGAWindowHeader::DrawHeaderFace
//------------------------------------------------------------------------------------
//

void
LGAWindowHeader::DrawHeaderFace ()
{
	
	StColorPenState	theColorState;
	theColorState.Normalize ();
	Rect	faceRect;
	CalcLocalFrameRect ( faceRect );
	
	RGBColor	tempColor;
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( faceRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е The face is painted in white
			tempColor = UGAColorRamp::GetWhiteColor ();
			::RGBForeColor ( &tempColor );
			::PaintRect ( &faceRect );
		}
		// е COLOR
		else
		{
			// е The face is painted with the face color if enabled and active
			// otherwise it is painted with the standard AGA color gray3
			tempColor = UGAColorRamp::GetColor(colorRamp_Gray2);
			::RGBForeColor ( &tempColor );
			::PaintRect ( &faceRect );
		}
	}
		
}	//	LGAWindowHeader::DrawHeaderFace


//------------------------------------------------------------------------------------
//	LGAWindowHeader::DrawHeaderBorder
//------------------------------------------------------------------------------------
//

void
LGAWindowHeader::DrawHeaderBorder ()
{
	
	StColorPenState	theColorState;
	theColorState.Normalize ();
	Rect					borderRect;
	CalcLocalFrameRect ( borderRect );
	
	RGBColor	tempColor;
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( borderRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е The border is always drawn in black
		tempColor = depth < 4 ? UGAColorRamp::GetBlackColor() : IsEnabled () &&
								IsActive () ? UGAColorRamp::GetBlackColor() :
										UGAColorRamp::GetColor(colorRamp_Gray7);
		::RGBForeColor ( &tempColor );
		
		// е If we have a bottom divider then we simply frame the whole thing
		// if not then we need to be a little smarter about drawing the border
		if ( HasBottomDivider ())
		{
			::FrameRect ( &borderRect );
		}
		else
		{
			::MoveTo ( borderRect.left, borderRect.bottom );
			::LineTo ( borderRect.left, borderRect.top );
			::LineTo ( borderRect.right - 1, borderRect.top );
			::LineTo ( borderRect.right - 1, borderRect.bottom );
		}
	}
	
}	//	LGAWindowHeader::DrawHeaderBorder


//-------------------------------------------------------------------------------------
// LGAWindowHeader::DrawHeaderShading	
//-------------------------------------------------------------------------------------
//

void
LGAWindowHeader::DrawHeaderShading ()
{
	
	StColorPenState	theColorPenState;
	StColorPenState::Normalize ();
	
	RGBColor	tempColor;
	
	Rect		shadingRect;
	CalcLocalFrameRect ( shadingRect );
	::InsetRect ( &shadingRect, 1, 1 );
	
	// е If there is no bottom divider then we need to adjust the bottom 
	// such that the shading in drawn a little lower at the bottom
	if ( !HasBottomDivider ())
		shadingRect.bottom++;

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( shadingRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е We only draw the shading is we have 4 bits of screen depth
		if ( depth >= 4 && IsEnabled () && IsActive ())
		{
			// е Frame the light portion of the shading which is in white
			tempColor = UGAColorRamp::GetWhiteColor();
			::RGBForeColor ( &tempColor );
			UGraphicsUtilities::TopLeftSide ( &shadingRect, 0, 0, 1, 1 );

			// е Frame the dark portion of the shading which is done with
			// a gray 5 shade
			tempColor = UGAColorRamp::GetColor(5);
			::RGBForeColor ( &tempColor );
			UGraphicsUtilities::BottomRightSide ( &shadingRect, 1, 1, 0, 0 );
		}
	}
		
}	// LGAWindowHeader::DrawHeaderShading


//-------------------------------------------------------------------------------------
// LGAWindowHeader::DrawHeaderAdornment	
//-------------------------------------------------------------------------------------
//

void
LGAWindowHeader::DrawHeaderAdornment ()
{
}	// LGAWindowHeader::DrawHeaderAdornment


//------------------------------------------------------------------------------------
//	LGAWindowHeader::RefreshHeaderBorder
//------------------------------------------------------------------------------------
//
//	Invalidate the border area of the banner so it will 
//	get redrawn during the next update

void
LGAWindowHeader::RefreshHeaderBorder ()
{
	// е We are only going to worry about doing a refresh if the toolbar 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if ( IsVisible ()  && CalcLocalFrameRect ( theFrame )  && ( mSuperView )) 
	{

		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			// е Get the content region figured out so that we can use it for the inval
			RgnHandle	borderRgn = GetBorderRegion ( revealed );
			
			// е Get the content invalidated so that it will be refreshed
			InvalPortRgn(borderRgn);
//			::InvalRgn ( borderRgn );
						
			// е Cleanup
			::DisposeRgn ( borderRgn );
		}
	}
	
}	//	LGAWindowHeader::RefreshHeaderBorder


//------------------------------------------------------------------------------------
//	LGAWindowHeader::RefreshHeaderShading
//------------------------------------------------------------------------------------
//
//	Invalidate the shading area of the banner so it will 
//	get redrawn during the next update

void
LGAWindowHeader::RefreshHeaderShading ()
{
	// е We are only going to worry about doing a refresh if the toolbar 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if ( IsVisible ()  && CalcLocalFrameRect ( theFrame )  && ( mSuperView )) 
	{
		::InsetRect ( &theFrame, 1, 1 );
		
		// е If there is no bottom divider then we need to adjust the bottom 
		// such that the shading in drawn a little lower at the bottom
		if ( !HasBottomDivider ())
			theFrame.bottom++;

		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			// е Get the shading region invalidated
			RgnHandle	shadingRgn = GetShadingRegion ( revealed );
			InvalPortRgn(shadingRgn);
//			::InvalRgn ( shadingRgn );
			::DisposeRgn ( shadingRgn );
		}
	}
	
}	//	LGAWindowHeader::RefreshHeaderShading