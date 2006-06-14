// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGASeparator.cp

CLASSES:				LGASeparator

DESCRIPTION:		Implementation file for LGASeparator

AUTHOR:				Robin Mair

CREATION DATE :	96.05.26

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.24		rtm	Added a copy constructor.
		97.01.22		rtm	Added a parameterized constructor that will allow instances
								to be created procedurally.
		97.01.20		rtm	Added a check to make sure the pane is visible before we
								call DrawSelf, otherwise the pane is drawn even if it is
								not currently visible.
		96.10.24		rtm	Added a normalize call in the DrawSelf function.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Changed to reflect the latest changes to AGA, also added
								overrides to EnableSelf and DisableSelf to handle redrawing
								the pane.
		96.05.26		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include <LGASeparator.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <LStream.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>

//=====================================================================================
// CONSTANTS
//=====================================================================================

#pragma mark === CLASS: LGASeparator

//=====================================================================================
// CLASS:	LGASeparator
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGASeparator::LGASeparator					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGASeparator::LGASeparator ()
{

	mHorizontal = false;
	
}	//	LGASeparator::LGASeparator


//-------------------------------------------------------------------------------------
// LGASeparator::LGASeparator					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGASeparator::LGASeparator (	const LGASeparator &inOriginal )
							:	LPane ( inOriginal )
{

	mHorizontal = inOriginal.mHorizontal;
	
}	//	LGASeparator::LGASeparator


//-------------------------------------------------------------------------------------
// LGASeparator::LGASeparator					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGASeparator::LGASeparator (	LStream*	inStream ) : LPane ( inStream )
{

	mHorizontal = false;
	
}	//	LGASeparator::LGASeparator


//-------------------------------------------------------------------------------------
// LGASeparator::LGASeparator					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGASeparator::LGASeparator ( const SPaneInfo	&inPaneInfo )
							: LPane ( inPaneInfo )
{

	mHorizontal = false;
	
}


//-------------------------------------------------------------------------------------
// LGASeparator::~LGASeparator				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGASeparator::~LGASeparator ()
{
}	//	LGASeparator::~LGASeparator


//-------------------------------------------------------------------------------------
// LGASeparator::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGASeparator::FinishCreateSelf ()
{

	// � Figure out if we are drawing a vertical or horizontal
	// separator, we set the horizontal flag to true if its
	// horizontal false if its vertical
	SDimension16	frameSize;
	GetFrameSize ( frameSize );
	mHorizontal = frameSize.width > frameSize.height;
		
}	//	LGASeparator::FinishCreateSelf


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// �� ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGASeparator::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGASeparator::EnableSelf ()
{

	// � Get things redrawn so that we can see the state change
	if ( IsVisible () && FocusDraw ())
		DrawSelf ();
	
}	//	LGASeparator::EnableSelf


//-------------------------------------------------------------------------------------
// LGASeparator::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGASeparator::DisableSelf ()
{

	// � Get things redrawn so that we can see the state change
	if ( IsVisible () && FocusDraw ())
		DrawSelf ();

}	//	LGASeparator::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// �� ACTIVATION
//-------------------------------------------------------------------------------------
// LGASeparator::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGASeparator::ActivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
		
}	//	LGASeparator::ActivateSelf


//-------------------------------------------------------------------------------------
// LGASeparator::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGASeparator::DeactivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGASeparator::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LGASeparator::DrawSelf
//-------------------------------------------------------------------------------------

void
LGASeparator::DrawSelf ()
{

	StColorPenState	theColorState;
	
	RGBColor	tempColor;
	
	// � Get the frame rect and calculate the width and height of the pane
	Rect	frameRect;
	CalcLocalFrameRect ( frameRect );
	SInt16		width = UGraphicsUtilities::RectWidth ( frameRect );
	SInt16		height = UGraphicsUtilities::RectHeight ( frameRect );
	
	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( frameRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		StColorPenState::Normalize ();
		
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			if ( IsHorizontal ())
			{
				tempColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &tempColor );
				::MoveTo ( frameRect.left, frameRect.top + (height / 2));
				::LineTo ( frameRect.right - 1, frameRect.top + (height / 2));
			}
			else
			{
				tempColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &tempColor );
				::MoveTo ( frameRect.left + (width / 2), frameRect.top );
				::LineTo ( frameRect.left + (width / 2), frameRect.bottom - 1 );
			}
		}
		// � COLOR
		else
		{
			// � HORIZONTAL
			if ( IsHorizontal ())
			{
				// � Dark line
				tempColor = IsEnabled () && IsActive () ? UGAColorRamp::GetColor(colorRamp_Gray7) 
													: UGAColorRamp::GetColor(colorRamp_Gray4);
				::RGBForeColor ( &tempColor );
				::MoveTo ( frameRect.left, frameRect.top + ((height / 2) - 1));
				::LineTo ( frameRect.right - 2, frameRect.top + ((height / 2) - 1) );
				
				// � Light line
				tempColor = IsEnabled () && IsActive () ? UGAColorRamp::GetWhiteColor() :
														UGAColorRamp::GetColor(2);
				::RGBForeColor ( &tempColor );
				::MoveTo ( frameRect.left + 1, frameRect.top + ((height / 2) - 1) + 1 );
				::LineTo ( frameRect.right - 1, frameRect.top + ((height / 2) - 1) + 1 );
			}
			else // � VERTICAL
			{
				// � Dark line
				tempColor = IsEnabled () && IsActive () ? UGAColorRamp::GetColor(colorRamp_Gray7) 
															: UGAColorRamp::GetColor(colorRamp_Gray4);
				::RGBForeColor ( &tempColor );
				::MoveTo ( frameRect.left + ((width / 2) - 1), frameRect.top );
				::LineTo ( frameRect.left + ((width / 2) - 1), frameRect.bottom - 2 );
				
				// � Light line
				tempColor = IsEnabled () && IsActive () ? UGAColorRamp::GetWhiteColor() :
														UGAColorRamp::GetColor(2);
				::RGBForeColor ( &tempColor );
				::MoveTo ( frameRect.left + ((width / 2) - 1) + 1, frameRect.top + 1 );
				::LineTo ( frameRect.left + ((width / 2) - 1) + 1, frameRect.bottom - 1 );
			}
		}
	}
	
}	//	LGASeparator::DrawSelf

