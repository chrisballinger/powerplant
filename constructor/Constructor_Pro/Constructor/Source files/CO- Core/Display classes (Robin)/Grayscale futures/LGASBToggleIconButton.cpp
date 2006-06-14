// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGASBToggleIconButton.cp

CLASSES:				LGASBToggleIconButton

DESCRIPTION:		Implementation file for LGASBToggleIconButton which is a control that is
		used inline with a scrollbar and handles toggling between two icons.

AUTHOR:				Robin Mair

CREATION DATE :	96.11.17

CHANGE HISTORY :

		96.12.10		rtm	Fixed a bug in the destructor where the were a couple of typos
								that could result in the same icon suite being disposed a couple 
								of times, thanks Clint!
		96.11.17		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include "LGASBToggleIconButton.h"
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <LControl.h>
#include <LStream.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <UException.h>

// � TOOLBOX HEADERS
#include <Quickdraw.h>
#include <TextEdit.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

#pragma mark === CLASS: LGASBToggleIconButton

//=====================================================================================
// �� STREAM CREATION
//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::CreateLGASBToggleIconButtonStream	
//-------------------------------------------------------------------------------------
//

LGASBToggleIconButton*	
LGASBToggleIconButton::CreateLGASBToggleIconButtonStream ( LStream *inStream )
{

	return ( new LGASBToggleIconButton ( inStream ));
	
}	// LGASBToggleIconButton::CreateLGASBToggleIconButtonStream


#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LGASBToggleIconButton
//=====================================================================================

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::LGASBToggleIconButton					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGASBToggleIconButton::LGASBToggleIconButton ()
{	

	mHilited = false;
	mNormalIconID = 0;
	mToggledIconID = 0;
	mHasAdornment = true;
	mNormalIconSuite = nil;
	mToggledIconSuite = nil;
	
}	//	LGASBToggleIconButton::LGASBToggleIconButton


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::LGASBToggleIconButton					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGASBToggleIconButton::LGASBToggleIconButton (	LStream	*inStream ) 
							: LControl ( inStream )
{

	// � Setup some initial stuff
	mNormalIconSuite = nil;
	mToggledIconSuite = nil;

	// � read in our data from the stream
	*inStream >> mIconSuiteID;
	mNormalIconID = mIconSuiteID;
	*inStream >> mToggledIconID;
	*inStream >> mHiliteIcon;
	*inStream >> mOffsetIconOnHilite;
	*inStream >> mHasAdornment;

	//	� Setup some additional field values which are hard coded based
	// on the way this class is intended to be used
	mIconSuite = nil;
	mHilited = false;
	mEdgeOffset = 2;
	mSizeSelector = sizeSelector_MiniIconSize;
	mIconPlacement = iconPosition_LeftCenter;
	mTransform = mEnabled == triState_Off ? ttDisabled : ttNone;
	
	// � Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 1 )
		SetMaxValue ( 1 );
		
}	//	LGASBToggleIconButton::LGASBToggleIconButton


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::~LGASBToggleIconButton				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGASBToggleIconButton::~LGASBToggleIconButton ()
{

	// � Get rid of the cached icon suites
	if ( mIconSuite )
		mIconSuite = nil;

	if ( mNormalIconSuite )
		::DisposeIconSuite ( mNormalIconSuite, true );

	if ( mToggledIconSuite )
		::DisposeIconSuite ( mToggledIconSuite, true );

}	//	LGASBToggleIconButton::~LGASBToggleIconButton


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::FinishCreateSelf ()
{

	// � Load and cache the icon suites for the two icons
	LoadIconSuiteHandle ();		//	This call is part of the GIconMixin class
	LoadIconCaches ();			//	This basically setups the toggle icon
		
	// � Check to see if the value is greater than zero which means the
	// control has been toggled so we need to toggle the icon IDs, if we 
	// don't need to toggle then we simply set the cached icon suite to be
	// the normal one
	if ( GetValue () > 0 )
		ToggleIcons ();
	
	// � Finally make sure we get the icon location calculated
	CalcIconLocation ();
	
}	//	LGASBToggleIconButton::FinishCreateSelf

	
#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGASBToggleIconButton::SetHiliteState	(	Boolean	inHiliteState )
{
	
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;

		// � Get the transform adjusted if needed
		AdjustTransform ( inHiliteState );
				
		// � We need to make sure that we get the button redrawn so that
		// the state change can be seen
		Draw ( nil );
	}
			
}	//	LGASBToggleIconButton::SetHiliteState


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::SetValue
//-------------------------------------------------------------------------------------

void			
LGASBToggleIconButton::SetValue	( SInt32	inValue )
{

	// � We are only going to do something if the value actually changed
	if ( GetValue () != inValue )
	{
		// � Call our parent to handle the setting of the value
		LControl::SetValue ( inValue );
		
		// � Make sure we get the icons toggled
		ToggleIcons ();
		
		// � Make sure we get the transform adjusted for the current state
		// of the button
		AdjustTransform ();

	}
	
}	//	LGASBToggleIconButton::SetValue


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::AdjustTransform
//-------------------------------------------------------------------------------------
//
//	This method is called from a number of different locations within this class and
// 	is the one place where all tweaks to the transform are handled.  Before this
//		method we were havig to do adjusting allover the place now it is handled in a
// 	more centralized manner

void			
LGASBToggleIconButton::AdjustTransform	( 	Boolean	inMouseInButton,
														Boolean 	inRedrawIcon )
{
	
	// � Adjust the transform as per the new state of the button
	SInt16		oldTransform = mTransform;
	if ( mEnabled != triState_Off )
	{
		if ( IsHilited () && !IsSelected ())
			mTransform = ttSelected;
		else if ( IsSelected ())
			mTransform = inMouseInButton ? ttSelected : ttNone;
		else
			mTransform = ttNone;
	}
	else
		mTransform = ttDisabled;
	
	// � If the state is being changed just so that the transform
	// will be updated then we only need to get the icon itself redrawn
	if ( oldTransform != mTransform && inRedrawIcon && FocusDraw () )
		DrawButtonIcon ();
		
}	//	LGASBToggleIconButton::AdjustTransform


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::SetHasAdornment
//-------------------------------------------------------------------------------------

void			
LGASBToggleIconButton::SetHasAdornment	(	Boolean	inIsAdorned )
{

	// � We are only going to do something if the value actually changed
	if ( mHasAdornment != inIsAdorned )
	{
		// � Change the flag
		mHasAdornment = inIsAdorned;
		
		// � Get the control redrawn so that the adornment is visible
		Draw ( nil );
	}
	
}	//	LGASBToggleIconButton::SetHasAdornment


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::ToggleIcons
//-------------------------------------------------------------------------------------

void			
LGASBToggleIconButton::ToggleIcons	()
{

	// � Toggle the icon ID and cache between the two choices
	if ( mIconSuiteID == mNormalIconID )
	{
		// � Change the flag
		mIconSuiteID = mToggledIconID;
		
		// � Now we also need to switch the icon handle that is cached
		// so that the correct one gets drawn
		mIconSuite = mToggledIconSuite;
	}
	else
	{
		// � Change the flag
		mIconSuiteID = mNormalIconID;

		// � Now we also need to switch the icon handle that is cached
		// so that the correct one gets drawn
		mIconSuite = mNormalIconSuite;
	}

}	//	LGASBToggleIconButton::ToggleIcons



//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::LoadIconCaches
//-------------------------------------------------------------------------------------

void			
LGASBToggleIconButton::LoadIconCaches	()
{

	// � The normal icon suite is loaded as part of the initialization 
	// process so all we need to do here is make a copy into the cache
	if ( mIconSuite != nil )
	{
		// � Load and cache the icon suite
		mNormalIconSuite = mIconSuite;
	}

	// � Load the toggle icon suite handle into our cache so that we don't
	// have to do it when we toggle
	if ( mToggledIconID >= 0 )
	{
	
		// � Load and cache the icon suite
		Handle				toggledIconSuite;
		IconSelectorValue	selector = svAllMiniData;
		
		// � Now handle the actual loading of the icon suite
		::GetIconSuite ( &toggledIconSuite, mToggledIconID, selector );
		ThrowIfNil_ ( toggledIconSuite );
		if ( toggledIconSuite )
			mToggledIconSuite = toggledIconSuite;
	}
	
}	//	LGASBToggleIconButton::LoadIconCaches


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::CalcLocalIconRect
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::CalcLocalIconRect ( Rect &outRect ) const
{

	// � Setup the rectangle based on the location and size of the icon
	outRect.top = mIconLocation.v;
	outRect.left = mIconLocation.h;
	outRect.bottom = mIconLocation.v + mSizeSelector;
	outRect.right = mIconLocation.h + 16;
	
	// � If we are hilited and the user has requested that the icon be offset
	// then handle it now
	if ( mHilited && mOffsetIconOnHilite )
		::OffsetRect( &outRect, 1, 1 );
		
}	//	LGASBToggleIconButton::CalcLocalIconRect


#pragma mark -
#pragma mark === STATE CHANGE

//=====================================================================================
// �� STATE CHANGE
//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::ActivateSelf()
{

	Refresh ();
	
}	//	LGASBToggleIconButton::ActivateSelf


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DeactivateSelf()
{

	Refresh ();
	
}	//	LGASBToggleIconButton::DeactivateSelf


#pragma mark -
#pragma mark === RESIZING

//=====================================================================================
// �� RESIZING
// ------------------------------------------------------------------------------------
//	LGASBToggleIconButton::ResizeFrameBy
// ------------------------------------------------------------------------------------
//

void
LGASBToggleIconButton::ResizeFrameBy ( 	SInt16		inWidthDelta,
													SInt16		inHeightDelta,
													Boolean		inRefresh )
{
	
	// � Pass the call on to our superclass
	LControl::ResizeFrameBy ( inWidthDelta, inHeightDelta, inRefresh );
	
	// � Any time we might undergo a size change we need to make sure that
	// we re-calculate the icon location
	CalcIconLocation ();
	
}	//	LGASBToggleIconButton::ResizeFrameBy


// ------------------------------------------------------------------------------------
//	LGASBToggleIconButton::MoveBy
// ------------------------------------------------------------------------------------
//

void
LGASBToggleIconButton::MoveBy ( SInt32		inHorizDelta,
								SInt32		inVertDelta,
								Boolean	inRefresh )
{

	// � Pass the call on to our superclass
	LControl::MoveBy ( inHorizDelta, inVertDelta, inRefresh );
	
	// � Get the icon location recalculated just in case
	CalcIconLocation ();
	
}	//	LGASBToggleIconButton::MoveBy


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DrawSelf	()
{

	// � Get the control drawn
	DrawToggleIconButton ();
	
}	//	LGASBToggleIconButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::DrawToggleIconButton
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DrawToggleIconButton	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// � We only draw the icon and any adornments if the control is active
	if ( IsActive ())
	{
		// � If the control needs to be adorned then handle that now
		if ( IsAdorned ())
			DrawAdornment ();
		
		// � Then draw the icon
		DrawButtonIcon ();
	}
	else
	{
		// � Setup a device loop so that we can handle drawing at the correct bit depth
		// of the varoius pixel maps
		StDeviceLoop	theLoop ( localFrame );
		SInt16				depth;
		while ( theLoop.NextDepth ( depth )) 
		{
			// � Fill the contents of the control with a light gray if we are drawing
			// in color otherwise it is simply white, and frame it in black
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );
			::FrameRect ( &localFrame );
			::InsetRect ( &localFrame, 1, 1 );
			theColor = depth < 4 ? UGAColorRamp::GetWhiteColor() : UGAColorRamp::GetColor(1);
			::RGBForeColor ( &theColor );
			::PaintRect ( &localFrame );
		}
	}
	
}	//	LGASBToggleIconButton::DrawToggleIconButton


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::DrawButtonIcon
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DrawButtonIcon	()
{

	StColorPenState	theColorPenState;

	// � Get the icon rectangle
	Rect iconRect;
	CalcLocalIconRect ( iconRect );
	
	// �  Now we plot the icon by calling the toolbox passing it our cached
	// handle to the icon suite
	if ( GetIconSuiteH ())
		::PlotIconSuite ( &iconRect, atNone, mTransform, GetIconSuiteH () );
	
}	//	LGASBToggleIconButton::DrawButtonIcon


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::DrawAdornment
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DrawAdornment	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	// � Get the adornment for the control drawn in its various states, this
	// allows us to adorn the basic pixel map with an adornment that meets
	// our needs, in this case it is a 3D button appearance as specified by
	// the AGA
	if ( IsHilited ())
	{
		if ( IsEnabled ())
			DrawAdornmentHilited ();
		else
			DrawAdornmentDimmed ();
	}
	else if ( !IsEnabled ())
		DrawAdornmentDimmed ();
	else
		DrawAdornmentNormal ();

}	//	LGASBToggleIconButton::DrawAdornment


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::DrawAdornmentNormal
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DrawAdornmentNormal	()
{
	
	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Before we draw anything we need to erase the control in case we
			// we had previously been hilited
			RGBColor theColor = UGAColorRamp::GetWhiteColor();
			::RGBBackColor ( &theColor);
			::EraseRect ( &localFrame );
			
			// � Draw the frame for the control outside of everything else
			// the frame is draw in the foreground color
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );
		}
		// � COLOR
		else
		{
			// � Get the bevel width
			SInt16		bevelWidth = 1;

			// � Setup a color array that will handle the number of bevels
			RGBColor colorArray[1];
			
			// � Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );

			// � Draw the frame for the control outside of everything else
			// the frame is draw in the foregorund color
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );

			// � First make sure the face of the control is drawn
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(3);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// �� LIGHT BEVELS
			// � Setup Colors
			colorArray[0] = UGAColorRamp::GetWhiteColor();
			
			// � Draw top and left edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, bevelWidth );
			
			// �� SHADOW BEVELS
			// � Setup Colors
			colorArray[0] = UGAColorRamp::GetColor(6);
			
			// � Draw bottom and right edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, bevelWidth );
			
			// �� CORNER PIXELS
			// � TOP RIGHT
			colorArray[0] = UGAColorRamp::GetColor(3);
			UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, bevelWidth );
			
			// � BOTTOM LEFT
			UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, bevelWidth );
		}	
	}
	
}	//	LGASBToggleIconButton::DrawAdornmentNormal


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::DrawAdornmentHilited
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DrawAdornmentHilited	()
{
	
	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );
			RGBColor theColor = UGAColorRamp::GetBlackColor ();
			::RGBForeColor ( &theColor );
			::PaintRect ( &localFrame );
		}
		// � COLOR
		else
		{
			// � Get the bevel width
			SInt16		bevelWidth = 1;

			// � Setup a color array that will handle the number of bevels
			RGBColor colorArray[1];
			
			// � Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );

			// � Draw the frame for the control outside of everything else
			// the frame is draw in the foregorund color
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );

			// � First make sure the face of the control is drawn
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(7);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// �� LIGHT BEVELS
			// � Setup Colors
			colorArray[0] = UGAColorRamp::GetColor(10);
			
			// � Draw top and left edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, bevelWidth );
			
			// �� SHADOW BEVELS
			// � Setup Colors
			colorArray[0] = UGAColorRamp::GetColor(5);
			
			// � Draw bottom and right edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, bevelWidth );
			
			// �� CORNER PIXELS
			// � TOP RIGHT
			colorArray[0] = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, bevelWidth );
			
			// � BOTTOM LEFT
			UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, bevelWidth );
		}	
	}
	
}	//	LGASBToggleIconButton::DrawAdornmentHilited


//-------------------------------------------------------------------------------------
// G3DButton::DrawAdornmentDimmed
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::DrawAdornmentDimmed	()
{
	
	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Before we draw anything we need to erase the control in case we
			// we had previously been hilited
			RGBColor theColor = UGAColorRamp::GetWhiteColor();
			::RGBBackColor ( &theColor);
			::EraseRect ( &localFrame );
			
			// � Draw the frame for the control outside of everything else
			// the frame is draw in the foreground color
			Pattern thePattern;
			UQDGlobals::GetGrayPat(&thePattern);
			::PenPat ( &thePattern );
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );
		}
		// � COLOR
		else 
		{
			// � Get the bevel width
			SInt16		bevelWidth = 1;

			// � Setup a color array that will handle the number of bevels
			RGBColor colorArray[1];
			
			// � Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );

			// � Draw the frame for the control outside of everything else
			// the frame is draw in the foregorund color
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );

			// � The first case is drawing a dimmed button in its normal state
			if ( !IsHilited ())
			{
				// � First make sure the face of the control is drawn
				::InsetRect ( &localFrame, 1, 1 );
				theColor = UGAColorRamp::GetColor(3);
				::RGBForeColor ( &theColor);
				::PaintRect ( &localFrame );
				
				// �� LIGHT BEVELS
				// � Setup Colors
				colorArray[0] = UGAColorRamp::GetColor(1);
				
				// � Draw top and left edges
				UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, bevelWidth );
				
				// �� SHADOW BEVELS
				// � Setup Colors
				colorArray[0] = UGAColorRamp::GetColor(5);
				
				// � Draw bottom and right edges
				UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, bevelWidth );
				
				// �� CORNER PIXELS
				// � TOP RIGHT
				colorArray[0] = UGAColorRamp::GetColor(3);
				UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, bevelWidth );
				
				// � BOTTOM LEFT
				UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, bevelWidth );
			}
			else
			{
				// � First make sure the face of the control is drawn
				::InsetRect ( &localFrame, 1, 1 );
				theColor = UGAColorRamp::GetColor(4);
				::RGBForeColor ( &theColor);
				::PaintRect ( &localFrame );
				
				// �� LIGHT BEVELS
				// � Setup Colors
				colorArray[0] = UGAColorRamp::GetColor(6);
				
				// � Draw top and left edges
				UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, bevelWidth );
				
				// �� SHADOW BEVELS
				// � Setup Colors
				colorArray[0] = UGAColorRamp::GetColor(2);
				
				// � Draw bottom and right edges
				UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, bevelWidth );
				
				// �� CORNER PIXELS
				// � TOP RIGHT
				colorArray[0] = UGAColorRamp::GetColor(4);
				UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, bevelWidth );
				
				// � BOTTOM LEFT
				UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, bevelWidth );
			}
		}	
	}
	
}	//	LGASBToggleIconButton::DrawAdornmentDimmed


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// �� MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::HotSpotAction	(	SInt16		/*inHotSpot*/,
													Boolean	inCurrInside,
													Boolean	inPrevInside )
{

	// � If the mouse moved in or out of the hot spot handle the
	// hiliting of the control
	if ( inCurrInside != inPrevInside )
	{
		// � Make sure the transform gets adjusted and then turn the
		// hiliting off based on whether we are inside the control
		// or not
		AdjustTransform ( inCurrInside );
		SetHiliteState ( inCurrInside );
	}
		
}	//	LGASBToggleIconButton::HotSpotAction


//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::HotSpotResult	(	SInt16 /*inHotSpot*/ )
{
	
	// � We behave just like a checkbox which is basically a switch
	// so we simply toggle the state, and we always turn the hiliting off
	SetValue ( 1 - GetValue ());
	SetHiliteState ( false );
	
}	//	LGASBToggleIconButton::HotSpotResult


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// �� MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGASBToggleIconButton::CalcIconLocation
//-------------------------------------------------------------------------------------

void
LGASBToggleIconButton::CalcIconLocation ()
{

	Rect		localFrame;
	SInt16		width, height, iconWidth;

	// � Get the local inset frame rect
	CalcLocalFrameRect ( localFrame );
	mIconLocation.h = localFrame.left;
	mIconLocation.v = localFrame.top;
	
	// � Setup the width and height values
	width = UGraphicsUtilities::RectWidth ( localFrame );
	height = UGraphicsUtilities::RectHeight ( localFrame );
	iconWidth = mSizeSelector == 12 ? 16 : mSizeSelector;
	
	// � Now we calculate the location of the icon based on the icon placement enum
	switch ( mIconPlacement )
	{
		case iconPosition_None:
		break;
		
		case iconPosition_TopLeft:
			mIconLocation.h += mEdgeOffset;
			mIconLocation.v += mEdgeOffset;
		break;
		
		case iconPosition_TopCenter:
			mIconLocation.h += ( width / 2 ) - ( iconWidth / 2 );
			mIconLocation.v += mEdgeOffset;
		break;
		
		case iconPosition_TopRight:
			mIconLocation.h += width - ( iconWidth + mEdgeOffset );
			mIconLocation.v += mEdgeOffset;
		break;
		
		case iconPosition_RightCenter:
			mIconLocation.h += width - ( iconWidth + mEdgeOffset );
			mIconLocation.v += ( height / 2 ) - ( mSizeSelector / 2 );
		break;
		
		case iconPosition_BottomRight:
			mIconLocation.h += width - ( iconWidth + mEdgeOffset );
			mIconLocation.v += height - ( mSizeSelector + mEdgeOffset );
		break;
		
		case iconPosition_BottomCenter:
			mIconLocation.h += ( width / 2 ) - ( iconWidth / 2 );
			mIconLocation.v += height - ( mSizeSelector + mEdgeOffset );
		break;
		
		case iconPosition_BottomLeft:
			mIconLocation.h += mEdgeOffset;
			mIconLocation.v += height - ( mSizeSelector + mEdgeOffset );
		break;
		
		case iconPosition_LeftCenter:
			mIconLocation.h += mEdgeOffset;
			mIconLocation.v += ( height / 2 ) - ( mSizeSelector / 2 );
		break;
		
		case iconPosition_Center:
			mIconLocation.h += ( width / 2 ) - ( iconWidth / 2 );
			mIconLocation.v += ( height / 2 ) - ( mSizeSelector / 2 );
		break;
		
	};
	
}	//	LGASBToggleIconButton::CalcIconLocation



