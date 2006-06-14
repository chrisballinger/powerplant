// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>-----------------------------------------

FILE:					GAShadowBorder.cp

CLASSES:				GAShadowBorder

DESCRIPTION:		Implementation file for GAShadowBorder

AUTHOR:				Robin Mair

CREATION DATE :	96.06.21

CHANGE HISTORY :

		96.07.21		rtm	Changed the Activate and Deactivate  methods so that they now
								call RefreshBorder instead of UpdatePort, this was done in an
								attempt to minimize flashing in the windows.  The effects of
								this change will only be seen if the window that the border is
								being used in is setup to NOT do an erase when updated.
		96.07.12		rtm	Added a change that causes the shadow not to be drawn if we are
						running under MacOS 8 or Aaron.
		96.06.24		rtm	Fixed a problem where the border region was not being calculated
								correctly so invalidation was not working correctly.
		96.06.21		rtm	Initial creation of file

---------------------------------------<< * >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
	#include "ctor.h"
#endif

#include "GAShadowBorder.h"
#include "UGraphicsUtilities.h"
#include "UGAColorRamp.h"

//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: GAShadowBorder ===

//=====================================================================================
// ** STREAM CREATION
//-------------------------------------------------------------------------------------
// GAShadowBorder::CreateGAShadowBorderStream	
//-------------------------------------------------------------------------------------
//

GAShadowBorder*	
GAShadowBorder::CreateGAShadowBorderStream ( LStream *inStream )
{

	return ( new GAShadowBorder ( inStream ));
	
}	// GAShadowBorder::CreateGAShadowBorderStream


#pragma mark === INITIALIZATION ===

//=====================================================================================
// CLASS:	GAShadowBorder
//=====================================================================================

//=====================================================================================
// ** INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// GAShadowBorder::GAShadowBorder					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAShadowBorder::GAShadowBorder ()
{

	// * Get all of the fields initialized
	mTwoPixelShadow = false;
	mFrameGrowBox = true;
	mGrowBoxSize = 15;
	
}	//	GAShadowBorder::GAShadowBorder


//-------------------------------------------------------------------------------------
// GAShadowBorder::GAShadowBorder					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAShadowBorder::GAShadowBorder (	LStream*	inStream ) : LView ( inStream )
{

	//	* Setup the data members for the view
	*inStream >> mTwoPixelShadow;
	*inStream >> mFrameGrowBox;
	*inStream >> mGrowBoxSize;

	
}	//	GAShadowBorder::GAShadowBorder


//-------------------------------------------------------------------------------------
// GAShadowBorder::~GAShadowBorder				---<<* DESTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAShadowBorder::~GAShadowBorder ()
{
}	//	GAShadowBorder::~GAShadowBorder


#pragma mark === ACCESSORS ===

//=====================================================================================
// ** ACCESSORS
//-------------------------------------------------------------------------------------
// GAShadowBorder::GetBorderRegion
//-------------------------------------------------------------------------------------

RgnHandle
GAShadowBorder::GetBorderRegion ()
{
	//	* Setup a region that will represent the face of the border
	// the goal being that we want to end up with a region that
	// is the width of the border we are drawing and that has a
	// notch for the growbox if requested
	RgnHandle borderRgn = ::NewRgn ();
	ThrowIfNil_ ( borderRgn );
	
	// * Calculate the border frame rect
	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	::RectRgn ( borderRgn, &localFrame);

	// * Now we need to build the inset portion of the region
	RgnHandle	insetRgn = ::NewRgn ();
	ThrowIfNil_ ( insetRgn );
	if ( mTwoPixelShadow )  
		::InsetRect ( &localFrame, 2, 2 );
	else
		::InsetRect ( &localFrame, 1, 1 );
		
	::RectRgn ( insetRgn, &localFrame );
	
	// * If the growbox is being framed then we need to make
	// sure that we notch the outer and inner regions before
	// we subtract them from each other
	if ( mFrameGrowBox )  
	{
		RgnHandle growRgn = ::NewRgn ();
		ThrowIfNil_ ( growRgn );
		Rect growRect;
		CalcLocalFrameRect ( growRect );
		growRect.left = growRect.right - mGrowBoxSize;
		growRect.top = growRect.bottom - mGrowBoxSize;
		
		// * Setup the region for the grow box and subtract it from
		// the border region
		::RectRgn ( growRgn, &growRect );
		::DiffRgn ( borderRgn, growRgn, borderRgn );
		
		// * Now we need to adjust the grow rect so that we can notch out
		// the inset portion of the view which varies depending on whether
		// we are drawing a two or one pixel border
		if ( mTwoPixelShadow )
			::OffsetRgn ( growRgn, -2, -2 );
		else
			::OffsetRgn ( growRgn, -1, -1 );
		
		// * Get the grow region removed from the inset region
		::DiffRgn ( insetRgn, growRgn, insetRgn );
		
		// * Get rid of the grow region
		::DisposeRgn ( growRgn );
	}

	// * Now we can get the inset region removed from the border region
	::DiffRgn ( borderRgn, insetRgn, borderRgn );
	::DisposeRgn ( insetRgn );
	
	//	* Now that is all done we can return the region to the caller
	// whow will be responsible for disposing of it when done
	return borderRgn;

}	//	GAShadowBorder::GetBorderRegion


#pragma mark === ACTIVATION ===

//=====================================================================================
// ** ACTIVATION
//-------------------------------------------------------------------------------------
// GAShadowBorder::ActivateSelf
//-------------------------------------------------------------------------------------

void
GAShadowBorder::ActivateSelf ()
{

	//	* Get the border redrawn
	RefreshBorder ();
		
}	//	GAShadowBorder::ActivateSelf



//-------------------------------------------------------------------------------------
// GAShadowBorder::DeactivateSelf
//-------------------------------------------------------------------------------------

void
GAShadowBorder::DeactivateSelf ()
{

	//	* Get the border redrawn
	RefreshBorder ();
		
}	//	GAShadowBorder::DeactivateSelf


//-------------------------------------------------------------------------------------
// GAShadowBorder::Activate
//-------------------------------------------------------------------------------------

void
GAShadowBorder::Activate ()
{

	LView::Activate ();
	
	//	* Get the border redrawn
	UpdatePort ();
		
}	//	GAShadowBorder::Activate



//-------------------------------------------------------------------------------------
// GAShadowBorder::Deactivate
//-------------------------------------------------------------------------------------

void
GAShadowBorder::Deactivate ()
{

	LView::Deactivate ();
	
	//	* Get the border redrawn
	UpdatePort ();
		
}	//	GAShadowBorder::Deactivate


#pragma mark === DRAWING ===

//=====================================================================================
// ** DRAWING
//-------------------------------------------------------------------------------------
// GAShadowBorder::DrawSelf
//-------------------------------------------------------------------------------------

void
GAShadowBorder::DrawSelf ()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// * Setup a device loop so that we can pass our drawing routine the
	// correct screen bit depth for the drawing process
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
		DrawBorder ( depth );
		
}	//	GAShadowBorder::DrawSelf



//-------------------------------------------------------------------------------------
// GAShadowBorder::DrawBorder
//-------------------------------------------------------------------------------------

void
GAShadowBorder::DrawBorder (	SInt16		inBitDepth )
{

	StColorPenState	theColorState;

	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
		
	Boolean	active = IsActive ();
	
	// * COLOR
	if ( inBitDepth >= 4 )
	{
		// * EDGE DETAILS
		// * Handle the case for the two pixel shadow
		if ( mTwoPixelShadow )
		{
			// * Draw the outside dark shadow
			// * If Mac OS 8 or Aaron is present then we only paint the edges
			// in the face color which essentially means we don't draw a shadow
			if ( UGraphicsUtilities::HasMacOS8 ())
			{
				RGBColor theColor = UGAColorRamp::GetColor(2);
				::RGBForeColor ( &theColor );
			}
			else
			{
				RGBColor theColor = active ? UGAColorRamp::GetColor(10) : UGAColorRamp::GetColor(7);
				::RGBForeColor ( &theColor );
			}
														
			if ( mFrameGrowBox )
				UGraphicsUtilities::BottomRightSideWithNotch ( &localFrame, 1, 1, 0, 0, mGrowBoxSize );
			else
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
			
			// * Now we draw the inside lighter shadow after insetting the rect
			::InsetRect ( &localFrame, 1, 1 );
			if ( UGraphicsUtilities::HasMacOS8 ())
			{
				RGBColor theColor = UGAColorRamp::GetColor(2);
				::RGBForeColor ( &theColor );
			}
			else
			{
				RGBColor theColor = active ? UGAColorRamp::GetColor(6) : UGAColorRamp::GetColor(4);
				::RGBForeColor ( &theColor );
			}
			if ( mFrameGrowBox )
				UGraphicsUtilities::BottomRightSideWithNotch ( &localFrame, 1, 1, 0, 0, mGrowBoxSize );
			else
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
		
			// * Finally, draw the light edge facing the light source	
			if ( UGraphicsUtilities::HasMacOS8 ())
			{
				RGBColor theColor = UGAColorRamp::GetColor(2);
				::RGBForeColor ( &theColor );
			}
			else
			{
				RGBColor theColor = active ? UGAColorRamp::GetWhiteColor() : UGAColorRamp::GetColor(1);
				::RGBForeColor ( &theColor );
			}
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );
		}
		else // * Handle the case for the one pixel shadow
		{
			// * Draw the outside dark shadow
			if ( UGraphicsUtilities::HasMacOS8 ())
			{
				RGBColor theColor = UGAColorRamp::GetColor(2);
				::RGBForeColor ( &theColor );
			}
			else
			{
				RGBColor theColor = active ? UGAColorRamp::GetColor(6) : UGAColorRamp::GetColor(4);
				::RGBForeColor ( &theColor );
			}
			if ( mFrameGrowBox )
			{
				UGraphicsUtilities::BottomRightSideWithNotch ( &localFrame, 1, 1, 0, 0, mGrowBoxSize );
			}
			else
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
			
			// * Draw the light edge facing the light source	
			if ( UGraphicsUtilities::HasMacOS8 ())
			{
				RGBColor theColor = UGAColorRamp::GetColor(2);
				::RGBForeColor ( &theColor );
			}
			else
			{
				RGBColor theColor = active ? UGAColorRamp::GetWhiteColor() : UGAColorRamp::GetColor(1);
				::RGBForeColor ( &theColor );
			}
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );
		}
		
		// * Fixup a couple of pixels in the corners
		RGBColor theColor = UGAColorRamp::GetColor(2);
		UGraphicsUtilities::PaintColorPixel ( localFrame.left, localFrame.bottom - 1, &theColor);
		theColor = UGAColorRamp::GetColor(2);
		UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, localFrame.top, &theColor);
	}
	
}	//	GAShadowBorder::DrawBorder



//-------------------------------------------------------------------------------------
// GAShadowBorder::RefreshBorder
//-------------------------------------------------------------------------------------

void
GAShadowBorder::RefreshBorder ()
{
	
	// * Get the face region and then invalidate it
	RgnHandle	borderRgn = GetBorderRegion ();
	ThrowIfNil_ ( borderRgn );
	if ( IsVisible () && FocusDraw ())
		InvalPortRgn ( borderRgn );
	
/*	TESTING
	::RGBForeColor ( &UGAColorRamp::GetBlackColor());
	::PaintRgn ( borderRgn );
	SlowDown ( 100 );
*/

	::DisposeRgn ( borderRgn );
	
}	//	GAShadowBorder::RefreshBorder



