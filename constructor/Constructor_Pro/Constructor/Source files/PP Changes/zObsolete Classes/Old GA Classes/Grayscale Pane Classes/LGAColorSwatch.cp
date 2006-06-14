// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAColorSwatch.cpp

CLASSES:				LGAColorSwatch

DESCRIPTION:		Implementation file for LGAColorSwatch

AUTHOR:				Robin Mair

CREATION DATE :	96.06.09

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.24		rtm	Added a copy constructor.
		97.01.22		rtm	Added a parameterized constructor so that instance can be
								created procedurally.
		96.10.26		rtm	Fixed a problem where the desktop was not being deactivated
								when the color picker was displayed, thanks Matt!
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.02		rtm	Added the ability for a color swatch to render itself in a
								dimmed state.
		96.06.09		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAColorSwatch.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <UDesktop.h>

// е TOOLBOX HEADERS
#include <ColorPicker.h>

//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

//=====================================================================================
// CLASS:	LGAColorSwatch
//=====================================================================================

#pragma mark === CLASS: LGAColorSwatch
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAColorSwatch::LGAColorSwatch					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAColorSwatch::LGAColorSwatch ()
{

	mSwatchColor = UGAColorRamp::GetWhiteColor();
	
}	//	LGAColorSwatch::LGAColorSwatch


//-------------------------------------------------------------------------------------
// LGAColorSwatch::LGAColorSwatch					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAColorSwatch::LGAColorSwatch (	const LGAColorSwatch &inOriginal )
							:	LPane ( inOriginal )
{

	mSwatchColor = inOriginal.mSwatchColor;
	
}	//	LGAColorSwatch::LGAColorSwatch


//-------------------------------------------------------------------------------------
// LGAColorSwatch::LGAColorSwatch					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAColorSwatch::LGAColorSwatch ( LStream* inStream ) : LPane ( inStream )
{

	// е Read in the color to be displayed
	*inStream >> mSwatchColor.red;
	*inStream >> mSwatchColor.green;
	*inStream >> mSwatchColor.blue;
	
}	//	LGAColorSwatch::LGAColorSwatch


//-------------------------------------------------------------------------------------
// LGAColorSwatch::LGAColorSwatch					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAColorSwatch::LGAColorSwatch (	const SPaneInfo	&inPaneInfo,
											RGBColor				&inSwatchColor ) 
									: LPane ( inPaneInfo )
{

	// е Setup the swatch color
	mSwatchColor = inSwatchColor;
	
}	//	LGAColorSwatch::LGAColorSwatch


//-------------------------------------------------------------------------------------
// LGAColorSwatch::~LGAColorSwatch				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAColorSwatch::~LGAColorSwatch ()
{
}	//	LGAColorSwatch::~LGAColorSwatch


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAColorSwatch::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGAColorSwatch::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAColorSwatch::EnableSelf


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGAColorSwatch::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAColorSwatch::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAColorSwatch::ActivateSelf
//-------------------------------------------------------------------------------------
//

void
LGAColorSwatch::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGAColorSwatch::ActivateSelf


//-------------------------------------------------------------------------------------
// LGAEditField::DeactivateSelf
//-------------------------------------------------------------------------------------
//

void
LGAColorSwatch::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGAColorSwatch::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAColorSwatch::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAColorSwatch::DrawSelf ()
{
	
	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	RGBColor	tempColor;
	
	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup adevice loop for handling the drawing on multiple devices
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
	
		::InsetRect ( &localFrame, 1, 1 );
		
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е First we are going to draw a little shadow under the color swatch
			::OffsetRect ( &localFrame, 1, 1 );
			tempColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::PaintRect ( &localFrame );
			::OffsetRect ( &localFrame, -2, -2 );
			
			// е Then we paint the swatch color
			tempColor = GetSwatchColor ();
			::RGBForeColor ( &tempColor );
			::PaintRect ( &localFrame );
			
			// е Finally, we frame the swatch
			tempColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::FrameRect ( &localFrame );
		}
		else
		{
			// е First we are going to draw a little shadow under the color swatch
			::OffsetRect ( &localFrame, 1, 1 );
			tempColor = IsEnabled () && IsActive () ? UGAColorRamp::GetColor(6) :
											UGAColorRamp::GetColor(4);
			::RGBForeColor ( &tempColor );
			::PaintRect ( &localFrame );
			::OffsetRect ( &localFrame, -2, -2 );
			
			// е Then we paint the swatch color
			tempColor = GetSwatchColor ();
			tempColor = IsEnabled () && IsActive () ? tempColor :
											UGraphicsUtilities::Lighten ( &tempColor );
			::RGBForeColor ( &tempColor );
			::PaintRect ( &localFrame );
			
			// е Finally, we frame the color swatch
			tempColor = IsEnabled () && IsActive () ? UGAColorRamp::GetBlackColor() :
											UGAColorRamp::GetColor(7);
			::RGBForeColor ( &tempColor );
			::FrameRect ( &localFrame );
		}
	}
		
}	//	LGAColorSwatch::DrawSelf


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGAColorSwatch::ClickSelf
//-------------------------------------------------------------------------------------

void
LGAColorSwatch::ClickSelf ( const SMouseDownEvent&	/* inMouseDown */ )
{

	// е Display the color picker and allow the user to select a new color
	// if the color changes then we set the swatch to the new color and
	// get it redrawn
	RGBColor	outColor;
	Point		where = { 0, 0 };
	Str255	prompt = "\pSelect swatch color:";
	UDesktop::Deactivate ();
	RGBColor theColor = GetSwatchColor ();
	if ( ::GetColor ( where, prompt, &theColor, &outColor )) 
	{
		SetSwatchColor ( outColor );
		Refresh ();
	}
	UDesktop::Activate ();
}
