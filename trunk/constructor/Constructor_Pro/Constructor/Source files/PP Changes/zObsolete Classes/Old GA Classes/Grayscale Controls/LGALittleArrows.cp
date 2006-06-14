// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGALittleArrows.cp

CLASSES:				LGALittleArrows

DESCRIPTION:		Implementation file for Object

AUTHOR:				Robin Mair

CREATION DATE :	96.12.08

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.02.06		rtm	Fixed a problem where there were some drawing calls being made
								without checking the visibility and setting the focus.
		97.01.24		rtm	Added a copy constructor.
		96.01.21		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		96.12.19		rtm	Fixed a small bug in the ClickSelf method where the call to
								FocusDraw was in the wrong location, this only showed up when
								drawing into an offscreen view.
		96.12.11		rtm	Changed the HotSpotResult method so that it now also handles an
								update when the mouse goes up in the continuous click action
								case, this effectively allows us to support both single click
								and continuous from the same click action.  What this means is
								that a fast single click on the control will cause an update
								while a click and hold will result in continuous updating of
								the control's value.
		96.12.11		rtm	Tweaked the HotSpotAction method so that the single click action
								case does not go through the delay process.
		96.12.09		rtm	Fixed a bug in the calculation of the up and down arrow rects.
		96.12.09		rtm	Got the delay/continuous and continuous action modes working
								which required the addition of two private fields for tracking
								the tick count.  These are used to manage the delay for the
								start of continuous and for controlling the speed at which the
								controls values are updated.
		96.12.08		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGALittleArrows.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

enum
{
	littleArrows_UpArrowHotSpot		=	1,
	littleArrows_DownArrowHotSpot		=	2,
	littleArrows_ArrowWidth				=	11,
	littleArrows_ArrowHeight			=	10,
	littleArrows_Width					=	13,
	littleArrows_Height					=	23
};



#pragma mark === CLASS: LGALittleArrows

//=====================================================================================
// CLASS:	LGALittleArrows
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGALittleArrows::LGALittleArrows					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGALittleArrows::LGALittleArrows ()
{	

	mClickAction = clickAction_SingleClick;
	mClickActionDelay = 0;
	mUpdateDelay = 0;
	mStartTickCount = 0;
	
}	//	LGALittleArrows::LGALittleArrows


//-------------------------------------------------------------------------------------
// LGALittleArrows::LGALittleArrows					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGALittleArrows::LGALittleArrows (	const LGALittleArrows &inOriginal )
								:	LControl ( inOriginal )
{	

	mClickAction = inOriginal.mClickAction;
	mClickActionDelay = inOriginal.mClickActionDelay;
	mUpdateDelay = inOriginal.mUpdateDelay;
	mStartTickCount = inOriginal.mStartTickCount;
	
}	//	LGALittleArrows::LGALittleArrows


//-------------------------------------------------------------------------------------
// LGALittleArrows::LGALittleArrows					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGALittleArrows::LGALittleArrows (	LStream	*inStream ) 
							: LControl ( inStream )
{

	SInt16		clickAction;
	
	// е Read in the click action
	*inStream >> clickAction;
	mClickAction = (ELAClickAction)clickAction;
	
	// е Read in delay used for the delay comtinuous click action
	*inStream >> mClickActionDelay;
			
	// е Read in delay used for the delay between value changes
	*inStream >> mUpdateDelay;
			
}	//	LGALittleArrows::LGALittleArrows


//-------------------------------------------------------------------------------------
// LGALittleArrows::LGALittleArrows					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGALittleArrows::LGALittleArrows (	const SPaneInfo 			&inPaneInfo,
												const SControlInfo 		&inControlInfo,
												SInt16							inClickAction,
												SInt16							inClickActionDelay,
												SInt16							inUpdateDelay )
										: LControl ( 	inPaneInfo,
															inControlInfo.valueMessage,
															inControlInfo.value,
															inControlInfo.minValue,
															inControlInfo.maxValue )
{

	// е Setup our fields
	mClickAction = (ELAClickAction)inClickAction;
	mClickActionDelay = inClickActionDelay;
	mUpdateDelay = inUpdateDelay;
			
}	//	LGALittleArrows::LGALittleArrows


//-------------------------------------------------------------------------------------
// LGALittleArrows::~LGALittleArrows				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGALittleArrows::~LGALittleArrows ()
{
}	//	LGALittleArrows::~LGALittleArrows


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGALittleArrows::CalcLocalUpArrowRect
//-------------------------------------------------------------------------------------

void
LGALittleArrows::CalcLocalUpArrowRect ( Rect &outRect ) const
{

	// е Figure out the frame rect for the whole control first
	CalcLocalLittleArrowRect ( outRect );

	// е Adjust the rect so that we have a rect that represents the
	// up arrow portion of the control
	outRect.left += 1;
	outRect.top += 1;
	outRect.right = outRect.left + littleArrows_ArrowWidth;
	outRect.bottom = outRect.top + littleArrows_ArrowHeight;
	
}	//	LGALittleArrows::CalcLocalUpArrowRect


//-------------------------------------------------------------------------------------
// LGALittleArrows::CalcLocalDownArrowRect
//-------------------------------------------------------------------------------------

void
LGALittleArrows::CalcLocalDownArrowRect ( Rect &outRect ) const
{

	// е Figure out the frame rect for the whole control first
	CalcLocalLittleArrowRect ( outRect );

	// е Adjust the rect so that we have a rect that represents the
	// down arrow portion of the control
	outRect.left += 1;
	outRect.right = outRect.left + littleArrows_ArrowWidth;
	outRect.top += littleArrows_ArrowHeight + 2;
	outRect.bottom = outRect.top + littleArrows_ArrowHeight;
	
}	//	LGALittleArrows::CalcLocalDownArrowRect


//-------------------------------------------------------------------------------------
// LGALittleArrows::CalcLocalLittleArrowRect
//-------------------------------------------------------------------------------------

void
LGALittleArrows::CalcLocalLittleArrowRect ( Rect &outRect ) const
{
	// е Figure out the frame rect for the little arrows button
	CalcLocalFrameRect ( outRect );
	SInt16		frameHeight = UGraphicsUtilities::RectHeight ( outRect );

	// е Adjust the rect such that it is always centered vertically within
	// the little arrows frame rect
	outRect.right = outRect.left + littleArrows_Width;
	outRect.top += (frameHeight / 2) - (littleArrows_ArrowHeight + 1);
	outRect.bottom = outRect.top + littleArrows_Height;
	
}	//	LGALittleArrows::CalcLocalLittleArrowRect


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGALittleArrows::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGALittleArrows::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );
	
}	//	LGALittleArrows::EnableSelf


//-------------------------------------------------------------------------------------
// LGALittleArrows::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGALittleArrows::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGALittleArrows::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGALittleArrows::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGALittleArrows::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGALittleArrows::ActivateSelf


//-------------------------------------------------------------------------------------
// LGALittleArrows::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGALittleArrows::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGALittleArrows::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGALittleArrows::DrawSelf
//-------------------------------------------------------------------------------------

void
LGALittleArrows::DrawSelf	()
{
	
	StColorPenState theSavedPenState;
	theSavedPenState.Normalize ();
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalLittleArrowRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е Draw the frame around the entire control
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );
			
			// е Now draw the dividing line
			::MoveTo ( localFrame.left, localFrame.top + littleArrows_ArrowHeight + 1 );
			::LineTo ( localFrame.right, localFrame.top + littleArrows_ArrowHeight + 1 );
		}
		else
		{
			// е Draw the frame around the entire control
			IsEnabled () && IsActive () ? theColor = UGAColorRamp::GetBlackColor() :
				theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
			::RGBForeColor (&theColor);
			UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );
			
			// е Now draw the dividing line
			::MoveTo ( localFrame.left, localFrame.top + littleArrows_ArrowHeight + 1 );
			::LineTo ( localFrame.right, localFrame.top + littleArrows_ArrowHeight + 1 );
		}
		
		// е Now get the two arrows drawn
		// е UP ARROW
		DrawLittleArrowsNormal ( true );
					
		// е DOWN ARROW
		DrawLittleArrowsNormal ( false );
	}
	
}	//	LGALittleArrows::DrawSelf


//-------------------------------------------------------------------------------------
// LGALittleArrows::DrawLittleArrowsNormal
//-------------------------------------------------------------------------------------

void
LGALittleArrows::DrawLittleArrowsNormal	(	Boolean	inUpArrow )
{
	
	StColorPenState theSavedPenState;
	theSavedPenState.Normalize ();
	
	// е Get the frame for the control
	Rect	localFrame;
	if ( 	inUpArrow )
		CalcLocalUpArrowRect ( localFrame );
	else
		CalcLocalDownArrowRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е Before we draw anything we need to erase the control in case we
			// we had previously been hilited
			theColor = UGAColorRamp::GetWhiteColor();
			::RGBBackColor ( &theColor);
			::EraseRect ( &localFrame );

			// е╩DRAW ACTUAL ARROW
			// е Always draw in black for B&W
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );

			// е If we are disabled we draw in gray
			if ( !IsEnabled ())
			{
				Pattern thePattern;
				UQDGlobals::GetGrayPat(&thePattern);
				::PenPat ( &thePattern );
			}

			// е Handle the drawing of the actual arrow
			DrawActualArrow ( localFrame, inUpArrow );
		}
		// е COLOR
		else
		{
			// е We need to normail in case we have just drawn a portion in
			// B&W in which case the pen pattern will be off
			StColorPenState::Normalize ();
			
			// е First make sure the face of the control is drawn
			theColor = UGAColorRamp::GetColor(2);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// е We only draw the bevels if we are enabled
			if ( IsEnabled () && IsActive ())
			{
				// ее LIGHT BEVELS
				// е Setup Colors for top left edges
				theColor = UGAColorRamp::GetWhiteColor();
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );

				// ее SHADOW BEVELS
				// е Setup Colors for bottom right edges
				theColor = UGAColorRamp::GetColor(5);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
			}
	
			// е DRAW ACTUAL ARROW
			// е Setup the color for either the enabled or disabled states
			if ( IsEnabled () && IsActive ())
			{
				theColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &theColor);
			}
			else
			{
				theColor = UGAColorRamp::GetColor(7);
				::RGBForeColor ( &theColor);
			}

			// е Handle the drawing of the actual arrow
			DrawActualArrow ( localFrame, inUpArrow );
		}	
	}

}	//	LGALittleArrows::DrawLittleArrowsNormal


//-------------------------------------------------------------------------------------
// LGALittleArrows::DrawLittleArrowsHilited
//-------------------------------------------------------------------------------------

void
LGALittleArrows::DrawLittleArrowsHilited	(	Boolean	inUpArrow )
{
	
	StColorPenState theSavedPenState;
	theSavedPenState.Normalize ();
	
	// е Get the frame for the control
	Rect	localFrame;
	if ( 	inUpArrow )
		CalcLocalUpArrowRect ( localFrame );
	else
		CalcLocalDownArrowRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е First make sure the face of the control is drawn
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );
			::PaintRect ( &localFrame );
			
			// е Draw the actual arrow which we only do this when drawing in color
			theColor = UGAColorRamp::GetWhiteColor();
			::RGBForeColor ( &theColor );
			DrawActualArrow ( localFrame, inUpArrow );
		}
		else
		{
			// е First make sure the face of the control is drawn
			theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// ее LIGHT BEVELS
			// е Setup Colors for top left edges
			theColor = UGAColorRamp::GetColor(colorRamp_Gray10);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );

			// ее SHADOW BEVELS
			// е Setup Colors for bottom right edges
			theColor = UGAColorRamp::GetColor(colorRamp_Gray6);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );

			// е Draw the actual arrow which we only do this when drawing in color
			theColor = UGAColorRamp::GetWhiteColor();
			::RGBForeColor ( &theColor );
			DrawActualArrow ( localFrame, inUpArrow );

		}
	}
	
}	//	LGALittleArrows::DrawLittleArrowsHilited


//-------------------------------------------------------------------------------------
// LGALittleArrows::DrawActualArrow
//-------------------------------------------------------------------------------------

void
LGALittleArrows::DrawActualArrow	(	Rect		&inRect,
												Boolean	inUpArrow )
{

	// е Now we can draw the actual arrows either an up or a down
	// depending on the flag passed in to us
	if ( inUpArrow )
	{
		::MoveTo ( 	inRect.left + 5, inRect.top + 3 );
		::LineTo ( inRect.right - 6, inRect.top + 3 );
		::MoveTo ( 	inRect.left + 4, inRect.top + 4 );
		::LineTo ( inRect.right - 5, inRect.top + 4 );
		::MoveTo ( 	inRect.left + 3, inRect.top + 5 );
		::LineTo ( inRect.right - 4, inRect.top + 5 );
		::MoveTo ( 	inRect.left + 2, inRect.top + 6 );
		::LineTo ( inRect.right - 3, inRect.top + 6 );
	}
	else
	{
		::MoveTo ( 	inRect.left + 2, inRect.top + 3 );
		::LineTo ( inRect.right - 3, inRect.top + 3 );
		::MoveTo ( 	inRect.left + 3, inRect.top + 4 );
		::LineTo ( inRect.right - 4, inRect.top + 4 );
		::MoveTo ( 	inRect.left + 4, inRect.top + 5 );
		::LineTo ( inRect.right - 5, inRect.top + 5 );
		::MoveTo ( 	inRect.left + 5, inRect.top + 6 );
		::LineTo ( inRect.right - 6, inRect.top + 6 );
	}
	
}	//	LGALittleArrows::DrawActualArrow


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGALittleArrows::ClickSelf
//-------------------------------------------------------------------------------------

void
LGALittleArrows::ClickSelf	( const SMouseDownEvent	&inMouseDown )
{
	
	// е Setup our fields used to track the tick count
	mStartTickCount = ::TickCount ();
	
	// е Determine which HotSpot was clicked
	SInt16	theHotSpot = FindHotSpot ( inMouseDown.whereLocal );
	
	// е If we got a valid hot spot then we proceed
	if ( theHotSpot == littleArrows_UpArrowHotSpot || 
						theHotSpot == littleArrows_DownArrowHotSpot ) 
	{

		// е Make sure we are focused before we start any tracking
		FocusDraw ();

		// е If we are handling the case where the click action calls
		// for a delay before we start changing the control's value
		// then we handle that now
		if ( mClickAction == clickAction_DelayContinuous )
		{
			SInt32	currTicks = ::TickCount ();
			
			// е Get the initial state of the button setup
			HotSpotAction ( theHotSpot, true, false );

			// е Now we are going to idle around for awhile to see if the
			// user keeps holding the mouse button down, if it is released
			// before we terminate from here then we restore the state and
			// bail out of here
			while ( (currTicks - mStartTickCount) < mClickActionDelay )
			{
				currTicks = ::TickCount ();
				
				// е If the mouse has gone up while we have been in the
				// delay then we need to get the buttons state changed
				// and bail out of here
				if ( !StillDown ())
				{
					HotSpotResult ( theHotSpot );
					return;
				}
			}
		}
		
		// е If we got to here it means we are going to be calling the
		// HotSpotAction continuously so we need to get the tick counter
		// setup with the starting tick count, we set this to the start
		// count so that the first time into HotSpotAction will get an 
		// immediate update with no increment delay [well that is so long
		// as the click action delay is greater than the increment delay
		mTickCounter = mStartTickCount;

		//	е Start tracking the hot spot
		if ( TrackHotSpot ( theHotSpot, inMouseDown.whereLocal, inMouseDown.macEvent.modifiers ))
			HotSpotResult ( theHotSpot );
	}
	
}	//	LGALittleArrows::ClickSelf


//-------------------------------------------------------------------------------------
// LGALittleArrows::FindHotSpot
//-------------------------------------------------------------------------------------

SInt16
LGALittleArrows::FindHotSpot	(	Point inPoint  ) const
{

	SInt16	hotSpot = 0;
	
	// е UP ARROW
	Rect upArrowRect;
	CalcLocalUpArrowRect ( upArrowRect );
	if ( ::PtInRect ( inPoint, &upArrowRect ) )
		hotSpot = littleArrows_UpArrowHotSpot;
		
	// е DOWN ARROW
	Rect downArrowRect;
	CalcLocalDownArrowRect ( downArrowRect );
	if ( ::PtInRect ( inPoint, &downArrowRect ) )
		hotSpot = littleArrows_DownArrowHotSpot;	
		
	// е Return the value for the hot spot
	return hotSpot;
	
}	//	LGALittleArrows::FindHotSpot


//-------------------------------------------------------------------------------------
// LGALittleArrows::FindHotSpot
//-------------------------------------------------------------------------------------

Boolean
LGALittleArrows::PointInHotSpot	(	Point inPoint, 
			 									SInt16 inHotSpot  ) const
{
	Boolean	hotSpotHit = false;
	
	// е UP ARROW
	if ( inHotSpot == littleArrows_UpArrowHotSpot )
	{
		Rect upArrowRect;
		CalcLocalUpArrowRect ( upArrowRect );
		if ( ::PtInRect ( inPoint, &upArrowRect ) )
			hotSpotHit = true;
	}
	
	// е DOWN ARROW
	if ( inHotSpot == littleArrows_DownArrowHotSpot )
	{
		Rect downArrowRect;
		CalcLocalDownArrowRect ( downArrowRect );
		if ( ::PtInRect ( inPoint, &downArrowRect ) )
			hotSpotHit = true;
	}
	
	// е Return whether the mouse was still in a particular hot spot
	return hotSpotHit;
		
}	//	LGALittleArrows::FindHotSpot


//-------------------------------------------------------------------------------------
// LGALittleArrows::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGALittleArrows::HotSpotAction	(	SInt16		 inHotSpot,
												Boolean	inCurrInside,
												Boolean	inPrevInside )
{
	
	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control based on the hot spot the mouse
	// went down in
	if ( inCurrInside != inPrevInside )
	{
		// е We first need to make sure we focus as the drawing calls do
		// not setup the focus themselves, we don't need to check the
		// visibility as we wouldn't be tracking the mouse if the control
		// was not visible
		if ( FocusDraw ())
		{
			if ( inHotSpot == littleArrows_UpArrowHotSpot )
			{
				if ( inCurrInside )
					DrawLittleArrowsHilited ( true );
				else
					DrawLittleArrowsNormal ( true );
			}
			else if ( inHotSpot == littleArrows_DownArrowHotSpot )
			{
				if ( inCurrInside )
					DrawLittleArrowsHilited ( false );
				else
					DrawLittleArrowsNormal ( false );
			}
		}
	}
	
	// е Here we handle the continuous update of the control's value based on
	// the mousedown status, we only change the value at a certain rate so
	// check the tick count to make sure a certain amount of time has elapsed
	// before we change the value, otherwise we just exit the function
	// NOTE: we only bother doing any of this if it is not the single click
	// action
	if ( mClickAction != clickAction_SingleClick )
	{
		SInt32	currTicks = ::TickCount ();
		if ( (currTicks - mTickCounter) < mUpdateDelay )
		{
			return;
		}
		else
		{
			// е We only handle the continuous mode if one of those click actions
			// has been chosen and if the mouse is inside and has been inside the
			// control, then depending on the hotspot we call our methods to handle
			// the actual changes in values
			if ( inCurrInside && inPrevInside )
			{
				// е Based on the particular hot spot we  call the appropriate 
				// action method
				if ( inHotSpot == littleArrows_UpArrowHotSpot )
					UpArrowAction ();
				else if ( inHotSpot == littleArrows_DownArrowHotSpot )
					DownArrowAction ();
			}
			
			// е Update the tick counter
			mTickCounter = currTicks;
		}
	}
	
}	//	LGALittleArrows::HotSpotAction


//-------------------------------------------------------------------------------------
// LGALittleArrows::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGALittleArrows::HotSpotResult	(	SInt16 inHotSpot )
{

	// е All we do here is make sure that the hiliting is turned off
	if ( IsVisible () && FocusDraw ())
		DrawLittleArrowsNormal ( inHotSpot == littleArrows_UpArrowHotSpot );

	// е We handle the single click action from here as well as the last click
	// for the continuous click action which therefore allows us to also do a
	// single click and get a change
	if ( mClickAction == clickAction_SingleClick || 
					mClickAction == clickAction_Continuous )
	{
		if ( inHotSpot == littleArrows_UpArrowHotSpot )
			UpArrowAction ();
		else
			DownArrowAction ();
	}
	
}	//	LGALittleArrows::HotSpotResult


#pragma mark -
#pragma mark === ARROW CLICK ACTION

//=====================================================================================
// ее ARROW CLICK ACTION
//-------------------------------------------------------------------------------------
//
//	These methods can be overridden if you wish to do something totally different, by
// 	breaking these out from the HotSpotAction method it should be easier to do this
//		without having to also override that method
//
//-------------------------------------------------------------------------------------
// LGALittleArrows::UpArrowAction
//-------------------------------------------------------------------------------------

void
LGALittleArrows::UpArrowAction	()
{
	
	// е By default we simply increment the current value of the control
	// if the new value exceeds the specified max value then we loop
	// around to the min value
	SInt32		newValue = GetValue ();
	newValue++;
	if ( newValue > GetMaxValue ())
		newValue = GetMinValue ();
		
	// е Final step is to get the new value set
	SetValue ( newValue );
		
}	//	LGALittleArrows::UpArrowAction


//-------------------------------------------------------------------------------------
// LGALittleArrows::DownArrowAction
//-------------------------------------------------------------------------------------

void
LGALittleArrows::DownArrowAction	()
{
	
	// е By default we simply decrement the current value of the control
	// if the new value is less than the specified min value then we loop
	// around to the max value
	SInt32		newValue = GetValue ();
	newValue--;
	if ( newValue < GetMinValue ())
		newValue = GetMaxValue ();
		
	// е Final step is to get the new value set
	SetValue ( newValue );

}	//	LGALittleArrows::DownArrowAction


