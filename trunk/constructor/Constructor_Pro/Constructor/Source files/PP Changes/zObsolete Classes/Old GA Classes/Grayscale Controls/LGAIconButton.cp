// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAIconButton.cp

CLASSES:				LGAIconButton

DESCRIPTION:		Implementation file for Object

AUTHOR:				Robin Mair

CREATION DATE :	96.06.04

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.04.04		rtm	Removed the call to DisposeIconSuite from the destructor as
								this has been moved into the LGAIconMixin class.
		97.02.17		rtm	Added a call to Normalize the pen state to the DrawIcon method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.27		rtm	Fixed a few bugs where the icon could be drawn when the button
								was not visible.
		97.01.27		rtm	Modified the CalcLocalIconRect method so that it returns an
								empty rect if there is no icon suite handle.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added two parameterized constructors so that the pane can be 
								built procedurally. One constructor takes the required input
								parameters, while the other allows these parameters to be
								passed in as a struct.
		97.01.05		rtm	Fixed a bug where a corner pixel in the one pixel bevel button
								was not being drawn correctly.
		96.12.17		rtm	Fixed a problem in the AdjustTransform method where the correct
								transform was not being calculated when the button was set to
								a latent state.
		96.12.11		rtm	Moved the EControlMode enum from the header file into the
								header file for UGraphicsUtilities so that it can be used by
								other classes.  NOTE: this enum will eventually move again as
								I plan on creating an LModeControl class that provides the
								basic mode support, this will be moved to there at that time.
		96.12.10		rtm	Modified the Stream constructor so that it no longer calls any 
								virtual functions.
		96.12.06		rtm	Tweaked some of the color constants as the indices have changed
								a little in the color ramp. Actually got rid of them and now
								use the new ones provided in the enum from the color ramp.
		96.12.03		rtm	Added a couple of color constants now that we no longer have
								them in the color ramp header file.
		96.10.21		rtm	Removed override of AdaptToSuperFrameSize.
		96.10.19		rtm	More messing around with the drawing of the hilited state
								in B&W, simplified the drawing there are now not nearly as
								many calls to DrawIcon.  Changed the drawing of the button
								when hilited in B&W all it does now is paint the entire frame
								black.
		96.10.19		rtm	Optimized the CalcIconLocation method so that we are only
								calculating the icon width once. Also changed the stream
								constructor so that the loading, adjusting of transform and
								calculation of the icons location is now handled in
								FinishCreateSelf.
		96.10.12		rtm	Fixed a bug where the icon was not being drawn correctly when
								pressed on a B & W monitor.
		96.10.09		rtm	Rewrote the handling of the transform for the button so that
								it now does the right thing according to the AGA spec.  The
								spec requires that when the button is pressed that the icon
								transform changes to ttSelected, if the button is behaving as
								either a switch or a radio button the icon then needs to have
								it's transform turned to ttNone when it is just on.  Where the
								On state means that the button appears pressed except the icon
								is not selected.  Confused yet!!  All of the transform adjust-
								ment is now handled from a single method AdjustTransform() that
								is called from any location within the class that might result
								in a transform change.  As part of this process SetHiliteState
								was changed to use this new method as part of the state change.
								NOTE: As part of these changes the icon button no longer uses
								the mHiliteIcon field from the icon mixin class as this is now
								no longer under the users control.  We will still continue to
								support the offset icon feature even though it is not part of
								the AGA specification.
		96.10.08		rtm	Got rid of DrawIconButton by moving its contents into DrawSelf.
		96.10.07		rtm	One last revisit to the appearance for this button in order to
								match it to the latest revision for the appearance from Apple.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.10.04		rtm	Tweaked the calculation of the icon location so that it correctly
								handles the case of the mini icon which is wider than it is high.
		96.10.01		rtm	Added an override to MoveBy so that we can recalulate the icons
								location.
		96.09.27		rtm	Fixed a number of drawing problems where the colors were in-
								correct due to Apple changing the spec and not mentioning that
								the basic frame of this button had changed.
		96.09.25		rtm	Fixed a bug in CalcLocalIconRect where it was not calculating
								rectangle appropriately for a mini icon.
		96.09.17		rtm	Fixed a bug where the edge offset was not being calulated quite 
								correctly, which showed up when creating buttons that had the
								icon in a position other than centered.  The LGAIconTextButton
								class caused this bug to manifest itself.
		96.09.10		rtm	Fixed a bug in SetHiliteState where the trabsform was not being
								set correctly when the state changed, which meant that if the
								hilite icon option was on the icon would never be hilited.
		96.09.05		rtm	Changed the constructors so that they now use the function
								provided by the icon mixin for loading and icon suite handle.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.01		rtm	Changed the rendering of the dimmed state of the button to
								meet Apple's modified spec for grayscale controls.
		96.08.05		rtm	Completed some missing integration problems with StDeviceLoop
								and the drawing of icons which resulted in the icon not being
								drawn when drawing across multiple devices, thanks Eric #2 for
								catching this one.
		96.08.04		rtm	Added the overrides of AdaptToSuperFrameSize and ResizeFrameBy
								which were not in the version that Eric based his changes on.
		96.08.04		rtm	Modified the following methods: DrawIconButton - this has changed
								quite a bit from the version Eric was using, DrawIconButtonHilited
								- also had changed from old code base and was revised to work with
								some of Eric's changes, DrawIconButtonHilited - modified Eric's
								version as it was doing and unnecessary redraw of the border of
								the button which really never needs to be redrawn except when the
								button is enabled or disabled.
		96.08.04		rtm	Merged Eric Shapiro's fixes into the current code base which was
								a lot newer than the codebase he had started with and had many
								of the problems fixed that he had fixed.
		96.08.02		ebs	Fixed 1-bit rendering -- was unhiliting after mouse was released. 
								Fixed rendering across 2+ devices (DrawIconButton & CalcLocalIconRect).
								Changed SetValue() to do nothing if there's no change (less flicker).
		96.07.02		rtm	Fixed a bug where the icon was not being drawn correctly when
								it spanned multiple monitors.  The problem was very obscure in
								that for the case where something spans multiple monitors and
								you have a StDeviceLoop object you have to be very careful
								about doing any drawing after the while loop typically used
								to check the bit depth.  The problem is that the device loop
								object only restores the original clipping when it is destroyed
								so you have to either make sure the device loop object is being
								used within a block statement or don't do any drawing after the
								loop. Very obscure!!  It would actually be nice if there was a
								method in StDeviceLoop that could be called to restore the clip
								so that you could use it without having to place it in an 
								artificial block.
		96.06.27		ebs	Added msg_DoubleClick support in HotSpotResult
		96.06.26		rtm	Added override of ResizeFrameBy so that we can call 
								CalcIconLocation() whenever the zize changes.
		96.06.20		rtm	Changed the SetHiliteState method so that it now handles the
								the redraw after a state change instead of all the other 
								places that were trying to take care of it.
		96.06.17		rtm	Changed the drawing of the icons so that they are now drawn
								from a cached icon suite handle instead of calling PlotIconID
								which was very slow.  By default we are jsut oading all of the
								icon data.
		96.06.17		rtm	Tweaked the drawing of the icon so that we use the transform
								in both the B&W and color cases, the icon was not showing up as
								dimmed on a B&W screen otherwise.
		96.06.04		rtm	Initial creation of file based on G3DButton with superclasses
								all integrated into one.

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAIconButton.h>
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>
#include	<LGAIconMixin.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <PP_Messages.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>

// е TOOLBOX HEADERS
#include <Icons.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

//=====================================================================================
// CLASS:	LGAIconButton
//=====================================================================================

#pragma mark === CLASS: LGAIconButton
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAIconButton::LGAIconButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAIconButton::LGAIconButton ()
{	

	mControlMode = controlMode_None;
	mBevelWidth = 2;
	mHilited = false;
	
}	//	LGAIconButton::LGAIconButton


//-------------------------------------------------------------------------------------
// LGAIconButton::LGAIconButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAIconButton::LGAIconButton 	(	const LGAIconButton &inOriginal )
							:	LControl ( inOriginal ),
								LGAIconMixin ( inOriginal )
{	

	mControlMode = inOriginal.mControlMode;
	mBevelWidth = inOriginal.mBevelWidth;
	mHilited = inOriginal.mHilited;
	
}	//	LGAIconButton::LGAIconButton


//-------------------------------------------------------------------------------------
// LGAIconButton::LGAIconButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAIconButton::LGAIconButton (	LStream	*inStream ) : LControl ( inStream )
{

	// е Setup some initial values
	mHilited = mValue != 0;
	
	// е Read in the data from the stream
	SInt16 sizeSelector;
	SInt16	position;
	SInt16	controlMode;
	
	*inStream >> controlMode;
	mControlMode = (EControlMode)controlMode;
	*inStream >> mIconSuiteID;
	*inStream >> sizeSelector;
	if ( sizeSelector == 0 || sizeSelector > 32 )
		mSizeSelector = sizeSelector_SmallIconSize;
	else
		mSizeSelector = (ESizeSelector)sizeSelector;		
	*inStream >> position;
	if ( position < 0 || position > 9 )
		mIconPlacement = iconPosition_Center;
	else
		mIconPlacement = (EIconPosition)position;
	*inStream >> mHiliteIcon;
	*inStream >> mOffsetIconOnHilite;
	
}	//	LGAIconButton::LGAIconButton


//-------------------------------------------------------------------------------------
// LGAIconButton::LGAIconButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAIconButton::LGAIconButton (	const SPaneInfo		&inPaneInfo,
											const SControlInfo	&inControlInfo,
											EControlMode			inControlMode,
											ResIDT					inIconResID,
											ESizeSelector			inIconSizeSelector,
											EIconPosition			inIconPlacement,
											Boolean					inHiliteIcon,
											Boolean					inOffsetIconOnHilite ) 
									: LControl ( 	inPaneInfo,
														inControlInfo.valueMessage,
														inControlInfo.value,
														inControlInfo.minValue,
														inControlInfo.maxValue )
{

	// е Setup some initial values
	mHilited = mValue != 0;
	
	mControlMode = inControlMode;
	mIconSuiteID = inIconResID;
	mSizeSelector = inIconSizeSelector;
	mIconPlacement = inIconPlacement;
	mHiliteIcon = inHiliteIcon;
	mOffsetIconOnHilite = inOffsetIconOnHilite;
	
}	//	LGAIconButton::LGAIconButton


//-------------------------------------------------------------------------------------
// LGAIconButton::LGAIconButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Paramterized constructor

LGAIconButton::LGAIconButton 	(	const SPaneInfo			&inPaneInfo,
											const SControlInfo		&inControlInfo,
											const SIconButtonInfo	&inIconButtonInfo ) 
									: LControl ( 	inPaneInfo,
														inControlInfo.valueMessage,
														inControlInfo.value,
														inControlInfo.minValue,
														inControlInfo.maxValue )
{

	// е Setup some initial values
	mHilited = mValue != 0;
	
	mControlMode = inIconButtonInfo.controlMode;
	mIconSuiteID = inIconButtonInfo.iconResID;
	mSizeSelector = inIconButtonInfo.sizeSelector;
	mIconPlacement = inIconButtonInfo.iconPosition;
	mHiliteIcon = inIconButtonInfo.hiliteIcon;
	mOffsetIconOnHilite = inIconButtonInfo.offsetIconOnHilite;
	
}	//	LGAIconButton::LGAIconButton


//-------------------------------------------------------------------------------------
// LGAIconButton::~LGAIconButton				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconButton::~LGAIconButton ()
{
}	//	LGAIconButton::~LGAIconButton


//-------------------------------------------------------------------------------------
// LGAIconButton::FinishCreateSelf
//-------------------------------------------------------------------------------------

void LGAIconButton::FinishCreateSelf ()
{

	// е Figure out the correct bevel width for the size icon being displayed
	switch ( mSizeSelector )
	{
		case sizeSelector_LargeIconSize:
			mBevelWidth = 3;
		break;
		
		case sizeSelector_SmallIconSize:
			mBevelWidth = 2;
		break;
		
		case sizeSelector_MiniIconSize:
			mBevelWidth = 1;
		break;
		
		default:
			mBevelWidth = 2;
		break;
		
	}
	
	// е Setup the correct edge offset based on the bevel width
	mEdgeOffset = mBevelWidth + 2;

	// е If the control mode is some form of button then we need to make
	// sure that the max value gets set to at least one
	if ( mControlMode > 0 && GetMaxValue () < 1 )
		mMaxValue = 1;
		
	// е Load and cache the icon suite
	LoadIconSuiteHandle ();
		
	// е Make sure that the transform for the icon is correctly setup
	AdjustTransform ( false, false );
	
	// е Finally, we need to get the icon location calculated
	CalcIconLocation ();	
	
}	//	LGAIconButton::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAIconButton::CalcLocalIconRect
//-------------------------------------------------------------------------------------

void
LGAIconButton::CalcLocalIconRect ( Rect &outRect ) const
{

	// е If we have an icon suite handle we figure out the size
	// of the rectanle it would occupy, otherwise we simply return
	// an empty rectangle.
	if ( GetIconSuiteH () != nil )
	{
		// е Setup the rectangle based on the location and size of the icon
		outRect.top = mIconLocation.v;
		outRect.left = mIconLocation.h;
		outRect.bottom = mIconLocation.v + mSizeSelector;
		outRect.right = outRect.left + (mSizeSelector == 12 ? 16 : mSizeSelector);
		
		// е If we are hilited and the user has requested that the icon be offset
		// then handle it now
		if ( mHilited && mOffsetIconOnHilite )
			::OffsetRect( &outRect, 1, 1 );
	}
	else
	{
		outRect.top = outRect.left = outRect.bottom = outRect.right = 0;
	}
		
}	//	LGAIconButton::CalcLocalIconRect

//-------------------------------------------------------------------------------------
// LGAIconButton::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGAIconButton::SetHiliteState	(	Boolean	inHiliteState )
{
	
	// е We only adjust the state if we have to
	if ( mHilited != inHiliteState )
	{
		// е Update to the new state
		mHilited = inHiliteState;
		
		// е Get the transform adjusted if needed
		AdjustTransform ( inHiliteState );
				
		// е The state changed so we have to redraw the button
		Draw ( nil );
	}
	
}	//	LGAIconButton::SetHiliteState


//-------------------------------------------------------------------------------------
// LGAIconButton::AdjustTransform
//-------------------------------------------------------------------------------------
//
//	This method is called from a number of different locations within this class and
// 	is the one place where all tweaks to the transform are handled.  Before this
//		method we were havig to do adjusting allover the place now it is handled in a
// 	more centralized manner

void			
LGAIconButton::AdjustTransform	( 	Boolean	inMouseInButton,
												Boolean 	inRedrawIcon )
{
	
	// е Adjust the transform as per the new state of the button
	SInt16		oldTransform = mTransform;
	if ( mEnabled == triState_On && mActive == triState_On )
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
	
	// е If the state is being changed just so that the transform
	// will be updated then we only need to get the icon itself redrawn
	if ( oldTransform != mTransform && inRedrawIcon && IsVisible () && FocusDraw () )
		DrawIcon ();
		
}	//	LGAIconButton::AdjustTransform


//-------------------------------------------------------------------------------------
// LGAIconButton::SetValue
//-------------------------------------------------------------------------------------
//

void
LGAIconButton::SetValue	(	SInt32	inValue )
{
	// е  Only change the value if we have too
	if ( mValue != inValue )
	{
		// е Get the value setup first
		LControl::SetValue ( inValue );
		
		// е And make sure we get the hilite state changed
		SetHiliteState ( inValue == Button_Off ? false : true );
		
		// е Make sure we get the transform adjusted for the current state
		// of the button
		AdjustTransform ();

		// е Broadcast the fact that we have turned the button on if that is the case
		if ( inValue == Button_On ) 
			BroadcastMessage ( msg_ControlClicked, (void*) this );
	}
	
}	//	LGAIconButton::SetValue


//-------------------------------------------------------------------------------------
// LGAIconButton::SetBevelWidth
//-------------------------------------------------------------------------------------

void
LGAIconButton::SetBevelWidth ( SInt16		inBevelWidth,
										Boolean	inRedraw )
{
	// е Only change the bevel width if we have to
	if ( mBevelWidth != inBevelWidth )
	{
		mBevelWidth = inBevelWidth;
		
		// е Get everything redrawn if requested
		if ( inRedraw )
			Draw ( nil );
	}
	
}	//	LGAIconButton::SetBevelWidth



#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAIconButton::EnableSelf
//-------------------------------------------------------------------------------------
//
//	This has been overridden so that we can do the correct thing about getting the
//		to match the state of the control

void
LGAIconButton::EnableSelf ()
{

 	if ( mEnabled == triState_On )
	{
		// е Get the transform adjusted if needed
		AdjustTransform ( false, false );
		
		// е Redraw the buttons so the changes show up
		Draw ( nil );
	}

}	//	LGAIconButton::EnableSelf


//-------------------------------------------------------------------------------------
// LGAIconButton::DisableSelf
//-------------------------------------------------------------------------------------
//
//	This has been overridden so that we can do the correct thing about getting the
//		transform to match the state of the control

void
LGAIconButton::DisableSelf ()
{

	// е Always just set the transform to be disabled
	if ( mEnabled == triState_Off || mEnabled == triState_Latent )
	{
		// е Get the transform for the icon adjusted to reflect the
		// new state
		AdjustTransform ( false, false );
		
		// е Get everything redrawn so the changes take effect
		Draw ( nil );
	}
	
}	//	LGAIconButton::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAIconButton::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGAIconButton::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
	{
		// е Get the transform for the icon adjusted to reflect the
		// new state
		AdjustTransform ( false, false );
	
		// е Get things redrawn
		Refresh ();
	}
				
}	//	LGAIconButton::ActivateSelf



//-------------------------------------------------------------------------------------
// LGAIconButton::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGAIconButton::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
	{				
		// е Get the transform for the icon adjusted to reflect the
		// new state
		AdjustTransform ( false, false );
		
		// е Get everything redrawn so the changes take effect
		Refresh ();
	}
	
}	//	LGAIconButton::DeactivateSelf


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGAIconButton::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGAIconButton::HotSpotAction	(	SInt16		/* inHotSpot */,
											Boolean	inCurrInside,
											Boolean	inPrevInside )
{

	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control which is slightly different for
	// each of the modes supported by the control
	if ( inCurrInside != inPrevInside )
	{
		switch ( GetControlMode ())
		{
			case controlMode_Button:
				// е For a straight button all we have to worry about
				// is switching the hiliting of the button
				SetHiliteState ( inCurrInside );
			break;
		
			case controlMode_RadioButton:
				// е Things are a little trickier if we are a radio button
				// as we only need to toggle the state if the control is
				// not already selected, we also want to avoid doing any
				// unnecessary drawing
				if ( inCurrInside )
				{
					if ( !IsHilited ())
						SetHiliteState ( true );
				}
				else if ( !IsSelected ())
					SetHiliteState ( false );
				
				// е Make sure we get the transform for the icon adjusted
				// to reflect the current state of the button
				AdjustTransform ( inCurrInside );
			break;
			
			case controlMode_Switch:
				// е If the control is a switch and it is currently selected
				// then we only need to mess with the transform of the icon.
				// If we are not currently selected then we just go ahead and 
				// call SetHiliteState
				if ( IsSelected ())
					AdjustTransform ( inCurrInside );
				else
					SetHiliteState ( !IsHilited ());
			break;
			
			default:
			break;
		}
	}
		
}	//	LGAIconButton::HotSpotAction


//-------------------------------------------------------------------------------------
// LGAIconButton::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGAIconButton::HotSpotResult	(	SInt16 inHotSpot )
{
	
	// е Make sure that by default we turn the hiliting off
	// if we are in button or radio mode, switch mode is handled
	// in the HotSpotAction method
	if ( mControlMode == controlMode_Button )
		HotSpotAction ( inHotSpot, false, true );
	else if ( mControlMode == controlMode_RadioButton )
		HotSpotAction ( inHotSpot, true, true );
		
	// е Do the right thing about getting the value set after a control has been
	// clicked in its hotspot, NOTE: this method assumes the entire control is
	// the hotspot
	switch ( GetControlMode ())
	{
		case controlMode_Button:
			BroadcastValueMessage ();	
		break;
		
		case controlMode_Switch:
			// е If the control is a switch we toggle
			// the value field of the control to indicate that
			// it is on or off
			SetValue ( 1 - GetValue ());
		break;
		
		case controlMode_RadioButton:
			// е If the control is not currently selected then we need to
			// ensure that the control is 'on', once it is on we never
			// change it back as it is a radio button and they only get
			// turned off by someone else when another radio button in
			// a group is selected
			if ( !IsSelected ())
			{
				// е Turn the button on
				SetValue ( Button_On );
			}
			else if ( GetClickCount() == 2 )
				BroadcastMessage ( msg_DoubleClick, (void*) this );
				
			// е Make sure we get the transform setup for the new state
			AdjustTransform ();
		break;
		
		default:
		break;
	}
	
}	//	LGAIconButton::HotSpotResult


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAIconButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// е Get the control drawn in its various states
	if ( IsEnabled () && IsActive ())
	{
		if ( IsHilited ())
			DrawIconButtonHilited ();
		else
			DrawIconButtonNormal ();
	}
	else
		DrawIconButtonDimmed ();

	// е Now we finally draw the icon after everything else has been drawn
	DrawIcon ();

}	//	LGAIconButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawControlNormal
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonNormal	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawIconButtonNormalBW ();
		}
		// е COLOR
		else
		{
			DrawIconButtonNormalColor ();
		}	
	}
	
}	//	LGAIconButton::DrawIconButtonNormal


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonHilited
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonHilited	()
{
	
	// е Get the frame for the control
	Rect	localFrame;	
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawIconButtonHilitedBW ();
		}
		else	// е COLOR
		{
			DrawIconButtonHilitedColor ();
		}
	}
	
}	//	LGAIconButton::DrawIconButtonHilited


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonDimmed
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonDimmed	()
{
	
	// е Get the frame for the control
	Rect	localFrame;	
	CalcLocalFrameRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawIconButtonDimmedBW ();
		}
		// е COLOR
		else
		{
			DrawIconButtonDimmedColor ();
		}	
	}
	
}	//	LGAIconButton::DrawIconButtonDimmed


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIcon
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIcon	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();

	// е Get the icon rectangle
	Rect iconRect;
	CalcLocalIconRect ( iconRect );
	
	// е  Now we plot the icon by calling the toolbox passing it our cached
	// handle to the icon suite
	if ( GetIconSuiteH ())
		::PlotIconSuite ( &iconRect, atNone, mTransform, GetIconSuiteH () );
		
}	//	LGAIconButton::DrawIcon


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonNormalBW
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonNormalBW	()
{

	StColorPenState::Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	RGBColor theColor = UGAColorRamp::GetWhiteColor();
	::RGBBackColor ( &theColor);
	::EraseRect ( &localFrame );
	
	// е Draw the frame for the control outside of everything else
	// the frame is draw in the foreground color
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor);
	::FrameRect ( &localFrame );
	
}	//	LGAIconButton::DrawIconButtonNormalBW


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonHilitedBW
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonHilitedBW	()
{

	StColorPenState::Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е We need to erase the old position of the icon if we had the
	// Offset Icon on Hilite option set
	RGBColor theColor = UGAColorRamp::GetBlackColor ();
	::RGBForeColor ( &theColor );
	::PaintRect ( &localFrame );

}	//	LGAIconButton::DrawIconButtonHilitedBW


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonDimmedBW
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonDimmedBW	()
{

	StColorPenState::Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	RGBColor theColor = UGAColorRamp::GetWhiteColor();
	::RGBBackColor ( &theColor);
	::EraseRect ( &localFrame );
	
	// е Draw the frame for the control outside of everything else
	// the frame is draw in the foreground color
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor);
	::FrameRect ( &localFrame );
	
}	//	LGAIconButton::DrawIconButtonDimmedBW


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonNormalColor
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonNormalColor	()
{

	StColorPenState::Normalize ();

	// е Get the bevel width
	SInt16		bevelWidth = GetBevelWidth ();

	// е Setup a color array that will handle the number of bevels
	RGBColor colorArray[3];
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// ее FRAME BUTTON
	RGBColor theColor = UGAColorRamp::GetColor(9);
	::RGBForeColor ( &theColor );
	UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

	theColor = UGAColorRamp::GetColor(colorRamp_GrayA1);
	::RGBForeColor ( &theColor );
	UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

	// е TOP RIGHT
	bevelWidth == 2 ? theColor = UGAColorRamp::GetColor(9) : theColor = UGAColorRamp::GetColor(10);
	UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
							localFrame.top, 
							&theColor);
		
	// е BOTTOM LEFT
	bevelWidth == 2 ? theColor = UGAColorRamp::GetColor(9) : theColor = UGAColorRamp::GetColor(10);
	UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
							localFrame.bottom - 1, 
							&theColor);
		
	// е FACE COLOR
	::InsetRect ( &localFrame, 1, 1 );
	theColor = UGAColorRamp::GetColor(3);
	::RGBForeColor ( &theColor);
	::PaintRect ( &localFrame );
	
	// ее LIGHT BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetWhiteColor();
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(3);
			colorArray[1] = UGAColorRamp::GetWhiteColor();
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(3);
			colorArray[1] = UGAColorRamp::GetColor(1);
			colorArray[2] = UGAColorRamp::GetWhiteColor();
		break;
		
	}
	
	// е Draw top and left edges
	UGraphicsUtilities::BevelledEdge ( 	&localFrame, 
													rectEdge_TopLeft, 
													colorArray, 
													bevelWidth );
	
	// ее SHADOW BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(7);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(8);
			colorArray[1] = UGAColorRamp::GetColor(6);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(10);
			colorArray[1] = UGAColorRamp::GetColor(8);
			colorArray[2] = UGAColorRamp::GetColor(6);
		break;
		
	}
	
	// е Draw bottom and right edges
	UGraphicsUtilities::BevelledEdge ( 	&localFrame, 
													rectEdge_BottomRight, 
													colorArray, 
													bevelWidth );
	
	// ее CORNER PIXELS
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(3);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(5);
			colorArray[1] = UGAColorRamp::GetColor(3);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(5);
			colorArray[1] = UGAColorRamp::GetColor(4);
			colorArray[2] = UGAColorRamp::GetColor(3);
		break;
	}
	
	// е Paint corner pixels
	// е TOP RIGHT
	UGraphicsUtilities::PaintCornerPixels ( 	&localFrame, 
															rectCorner_TopRight, 
															colorArray, 
															bevelWidth );
	
	// е BOTTOM LEFT
	UGraphicsUtilities::PaintCornerPixels ( 	&localFrame, 
															rectCorner_BottomLeft, 
															colorArray, 
															bevelWidth );
	
}	//	LGAIconButton::DrawIconButtonNormalColor


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonHilitedColor
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonHilitedColor	()
{

	StColorPenState::Normalize ();

	// е Get the bevel width
	SInt16		bevelWidth = GetBevelWidth ();

	// е Setup a color array that will handle the number of bevels
	RGBColor colorArray[3];
	
	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// ее FRAME BUTTON
	RGBColor theColor = UGAColorRamp::GetColor(colorRamp_GrayA2);
	::RGBForeColor ( &theColor );
	UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

	theColor = UGAColorRamp::GetColor(11);
	::RGBForeColor ( &theColor );
	UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

	// е TOP RIGHT
	bevelWidth == 2 ? theColor = UGAColorRamp::GetColor(colorRamp_GrayA2) : theColor = UGAColorRamp::GetColor(12);
	UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
							localFrame.top, 
							&theColor);
		
	// е BOTTOM LEFT
	bevelWidth == 2 ? theColor = UGAColorRamp::GetColor(colorRamp_GrayA2) : theColor = UGAColorRamp::GetColor(12);
	UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
							localFrame.bottom - 1, 
							&theColor);
		
	// е FACE COLOR
	::InsetRect ( &localFrame, 1, 1 );
	theColor = UGAColorRamp::GetColor(7);
	::RGBForeColor ( &theColor);
	::PaintRect ( &localFrame );
	
	// ее LIGHT BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(10);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(11);
			colorArray[1] = UGAColorRamp::GetColor(9);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(colorRamp_GrayA1);
			colorArray[1] = UGAColorRamp::GetColor(11);
			colorArray[2] = UGAColorRamp::GetColor(9);
		break;
		
	}
			
	// е Draw top left edges
	UGraphicsUtilities::BevelledEdge ( &localFrame, 
													rectEdge_TopLeft, 
													colorArray, 
													bevelWidth );
	
	// ее SHADOW BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(5);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(4);
			colorArray[1] = UGAColorRamp::GetColor(6);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(4);
			colorArray[1] = UGAColorRamp::GetColor(5);
			colorArray[2] = UGAColorRamp::GetColor(6);
		break;
		
	}
	
	// е Draw bottom right edges
	UGraphicsUtilities::BevelledEdge ( &localFrame, 
													rectEdge_BottomRight, 
													colorArray, 
													bevelWidth );
	
	// ее CORNER PIXELS
	// е TOP RIGHT
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(8);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(8);
			colorArray[1] = UGAColorRamp::GetColor(7);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(11);
			colorArray[1] = UGAColorRamp::GetColor(8);
			colorArray[2] = UGAColorRamp::GetColor(7);
		break;
	}
	
	// ее CORNER PIXELS
	// е TOP RIGHT
	UGraphicsUtilities::PaintCornerPixels ( 	&localFrame, 
															rectCorner_TopRight, 
															colorArray, 
															bevelWidth );
	
	// е BOTTOM LEFT
	UGraphicsUtilities::PaintCornerPixels ( 	&localFrame, 
															rectCorner_BottomLeft, 
															colorArray, 
															bevelWidth );

}	//	LGAIconButton::DrawIconButtonHilitedColor


//-------------------------------------------------------------------------------------
// LGAIconButton::DrawIconButtonDimmedColor
//-------------------------------------------------------------------------------------

void
LGAIconButton::DrawIconButtonDimmedColor	()
{

	StColorPenState::Normalize ();

	SInt16		bevelWidth = GetBevelWidth ();
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е DISABLED NORMAL STATE
	if ( !IsHilited ())
	{
		// ее FRAME BUTTON
		RGBColor theColor = UGAColorRamp::GetColor(5);
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

		
		bevelWidth == 3 ? theColor = UGAColorRamp::GetColor(7) : theColor = UGAColorRamp::GetColor(6);
 		::RGBForeColor (&theColor);
		UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				


		// е TOP RIGHT
		if ( bevelWidth == 3 )
		{
			theColor = UGAColorRamp::GetColor(6);
			UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
									localFrame.top, 
									&theColor );
				
			// е BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(6);
			UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
									localFrame.bottom - 1, 
									&theColor );
		}
	}
	else	// DISABLED HILITED STATE
	{
		// ее FRAME BUTTON
		RGBColor theColor = UGAColorRamp::GetColor(8);
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

		theColor = UGAColorRamp::GetColor(6);
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

		if ( bevelWidth != 2 )
		{
			// е TOP RIGHT
			theColor = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
									localFrame.top, 
									&theColor );
				
			// е BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
									localFrame.bottom - 1, 
									&theColor );
		}
	}


	// е Paint the face of the button
	::InsetRect ( &localFrame, 1, 1 );
	RGBColor theColor =  !IsHilited () ? UGAColorRamp::GetColor(2) : UGAColorRamp::GetColor(4);
	::RGBForeColor ( &theColor );
	::PaintRect ( &localFrame );

}	//	LGAIconButton::DrawIconButtonDimmedColor


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGAIconButton::CalcIconLocation
//-------------------------------------------------------------------------------------

void
LGAIconButton::CalcIconLocation ()
{

	Rect		localFrame;
	SInt16		width, height;
	SInt16		bevelWidth = GetBevelWidth ();

	// е We need to make sure that if the edge offset has been set
	// to be less than the bevel width we adjust it accordingly
	// which in this case means that we always want the offset to be
	// thebevel width plus a 2 pixel buffer
	if ( mEdgeOffset < bevelWidth )
		mEdgeOffset =  bevelWidth + 2;
	
	// е Get the local inset frame rect
	CalcLocalFrameRect ( localFrame );
	mIconLocation.h = localFrame.left;
	mIconLocation.v = localFrame.top;
	
	// е Setup the width and height values
	width = UGraphicsUtilities::RectWidth ( localFrame );
	height = UGraphicsUtilities::RectHeight ( localFrame );
	SInt16	iconWidth = mSizeSelector == 12 ? 16 : mSizeSelector;
	
	// е Now we calculate the location of the icon based on the icon placement enum
	switch ( mIconPlacement )
	{
		case iconPosition_None:
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
	
}	//	LGAIconButton::CalcIconLocation


#pragma mark -
#pragma mark === RESIZING

//=====================================================================================
// ее RESIZING
//-------------------------------------------------------------------------------------
// LGAIconButton::ResizeFrameBy
//-------------------------------------------------------------------------------------

void
LGAIconButton::ResizeFrameBy ( 	SInt16		inWidthDelta,
										SInt16		inHeightDelta,
										Boolean		inRefresh )
{
	
	// е Pass the call on to our superclass
	LPane::ResizeFrameBy ( inWidthDelta, inHeightDelta, inRefresh );
	
	// е Any time we might undergo a size change we need to make sure that
	// we re-calculate the icon location
	CalcIconLocation ();
	
}	//	LGAIconButton::ResizeFrameBy


//-------------------------------------------------------------------------------------
// LGAIconButton::MoveBy
//-------------------------------------------------------------------------------------

void
LGAIconButton::MoveBy ( 	SInt32		inHorizDelta,
									SInt32		inVertDelta,
									Boolean	inRefresh )
{
	
	// е Pass the call on to our superclass
	LControl::MoveBy ( inHorizDelta, inVertDelta, inRefresh );
	
	// е Any time we might undergo a size change we need to make sure that
	// we re-calculate the icon location
	CalcIconLocation ();
	
}	//	LGAIconButton::MoveBy


