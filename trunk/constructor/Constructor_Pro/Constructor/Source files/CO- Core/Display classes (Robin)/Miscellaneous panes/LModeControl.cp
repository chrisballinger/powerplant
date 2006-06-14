// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LModeControl.cp

CLASSES:				LModeControl

DESCRIPTION:		Implementation file for LModeControl

AUTHOR:				Robin Mair

CREATION DATE :	96.11.17

CHANGE HISTORY :

		96.11.17		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include "LModeControl.h"

// � POWERPLANT HEADERS
#include <LControl.h>
#include <LStream.h>
#include <UException.h>
#include <PP_Messages.h>

// � TOOLBOX HEADERS

//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LModeControl

//=====================================================================================
// �� STREAM CREATION
//-------------------------------------------------------------------------------------
// LModeControl::CreateLModeControlStream	
//-------------------------------------------------------------------------------------
//

LModeControl*	
LModeControl::CreateLModeControlStream ( LStream *inStream )
{

	return ( new LModeControl ( inStream ));
	
}	// LModeControl::CreateLModeControlStream


//=====================================================================================
// CLASS:	LModeControl
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LModeControl::LModeControl					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LModeControl::LModeControl ()
{	

	mControlMode = controlModes_Button;
	mPushed = false;
	
}	//	LModeControl::LModeControl


//-------------------------------------------------------------------------------------
// LModeControl::LModeControl					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LModeControl::LModeControl (	LStream	*inStream ) : LControl ( inStream )
{
	Int16 controlMode;
	
	// � Read in the data from the stream
	*inStream >> controlMode;
	mControlMode = (EControlModes)controlMode;
	
	// � If the control mode is some form of button then we need to make
	// sure that the max value gets set to at least one
	if ( mControlMode > 0 && GetMaxValue () < 1 )
		mMaxValue = 1;
		
	// � Make sure the hilited flag gets setup correctly from any value
	// that is currently set
	mPushed = GetValue () != 0;	
		
}	//	LModeControl::LModeControl


//-------------------------------------------------------------------------------------
// LModeControl::LModeControl					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LModeControl::LModeControl (	const SPaneInfo 			&inPaneInfo,
										const SControlInfo 		&inControlInfo,
										EControlModes				inControlMode )
								: LControl ( 	inPaneInfo,
													inControlInfo.valueMessage,
													inControlInfo.value,
													inControlInfo.minValue,
													inControlInfo.maxValue )
{	

	// � Setup all the icon fields
	mControlMode = inControlMode;
	
	// � If the control mode is some form of button then we need to make
	// sure that the max value gets set to at least one
	if ( mControlMode > 0 && GetMaxValue () < 1 )
		mMaxValue = 1;
		
	// � Make sure the hilited flag gets setup correctly from any value
	// that is currently set
	mPushed = GetValue () != 0;	
		
}	//	LModeControl::LModeControl


//-------------------------------------------------------------------------------------
// LModeControl::~LModeControl				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LModeControl::~LModeControl ()
{
}	//	LModeControl::~LModeControl


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LModeControl::SetPushedState
//-------------------------------------------------------------------------------------

void			
LModeControl::SetPushedState	(	Boolean	inPushedState )
{
	
	// � We only change the state if we have to
	if ( mPushed != inPushedState )
	{
		mPushed = inPushedState;
		
		// � Because the state has changed we need to make sure 
		// that things get redrawn so that the change can take effect
		Draw ( nil );
	}
			
}	//	LModeControl::SetPushedState


//-------------------------------------------------------------------------------------
// LModeControl::SetValue
//-------------------------------------------------------------------------------------
//

void
LModeControl::SetValue	(	Int32	inValue )
{
	// �  Only change the value if we have too
	if ( mValue != inValue )
	{
		// � Get the value setup first
		LControl::SetValue ( inValue );
		
		// � And make sure we get the hilite state changed
		SetPushedState ( inValue == Button_Off ? false : true );
		
		// � Broadcast the fact that we have turned the button on if that is the case
		if ( inValue == Button_On ) 
			BroadcastMessage ( msg_ControlClicked, (void*) this );
	}
	
}	//	LModeControl::SetValue


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// �� ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LModeControl::EnableSelf
//-------------------------------------------------------------------------------------
//
//	This has been overridden so that we can do the correct thing about getting the
//		transform to match the state of the control

void
LModeControl::EnableSelf ()
{

	// � Get the control redrawn
 	if ( FocusDraw ())
 		DrawSelf ();

}	//	LModeControl::EnableSelf


//-------------------------------------------------------------------------------------
// LModeControl::DisableSelf
//-------------------------------------------------------------------------------------
//
//	This has been overridden so that we can do the correct thing about getting the
//		transform to match the state of the control

void
LModeControl::DisableSelf ()
{

	// � Get the control redrawn
 	if ( FocusDraw ())
 		DrawSelf ();
	
}	//	LModeControl::DisableSelf


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// �� MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LModeControl::HotSpotAction
//-------------------------------------------------------------------------------------

void
LModeControl::HotSpotAction	(	Int16		/* inHotSpot */,
											Boolean	inCurrInside,
											Boolean	inPrevInside )
{

	// � If the mouse moved in or out of the hot spot handle the
	// hiliting of the control which is slightly different for
	// each of the modes supported by the control
	if ( inCurrInside != inPrevInside )
	{
		switch ( GetControlMode ())
		{
			case controlModes_Button:
				// � For a straight button all we have to worry about
				// is switching the hiliting of the button
				SetPushedState ( inCurrInside );
			break;
		
			case controlModes_RadioButton:
				// � Things are a little trickier if we ar a radio button
				// as we only need to toggle the state if the control is
				// not already selected, we also want to avoid doing any
				// unnecessary drawing
				if ( inCurrInside )
				{
					if ( !IsPushed ())
						SetPushedState ( true );
				}
				else if ( !IsSelected ())
					SetPushedState ( false );
			break;
			
			case controlModes_Switch:
				// � If the control is a switch we toggle the hiliting
				// from whatever its current setting is
				SetPushedState ( !IsPushed ());
			break;
			
			default:
			break;
		}
	}
		
}	//	LModeControl::HotSpotAction


//-------------------------------------------------------------------------------------
// LModeControl::HotSpotResult
//-------------------------------------------------------------------------------------

void
LModeControl::HotSpotResult	(	Int16 inHotSpot )
{
	
	// � Make sure that by default we turn the hiliting off
	// if we are in button or radio mode, switch mode is handled
	// in the HotSpotAction method
	if ( mControlMode == controlModes_Button )
		HotSpotAction ( inHotSpot, false, true );
	else if ( mControlMode == controlModes_RadioButton )
		HotSpotAction ( inHotSpot, true, true );
		
	// � Do the right thing about getting the value set after a control has been
	// clicked in its hotspot, NOTE: this method assumes the entire control is
	// the hotspot
	switch ( GetControlMode ())
	{
		case controlModes_Button:
			BroadcastValueMessage ();	
		break;
		
		case controlModes_Switch:
			// � If the control is a switch we toggle
			// the value field of the control to indicate that
			// it is on or off
			SetValue ( 1 - GetValue ());
		break;
		
		case controlModes_RadioButton:
			// � If the control is not currently selected then we need to
			// ensure that the control is 'on', once it is on we never
			// change it back as it is a radio button and they only get
			// turned off by someone else when another radio button in
			// a group is selected
			if ( !IsSelected ())
			{
				SetValue ( Button_On );
			}
//			else if ( GetClickCount() == 2 )
//				BroadcastMessage ( msg_DoubleClick, (void*) this );
		break;
		
		default:
		break;
	}
	
}	//	LModeControl::HotSpotResult


