// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>-----------------------------------------

FILE:					GAZoomToggle.cp

CLASSES:				GAZoomToggle

DESCRIPTION:		Implementation file for GAZoomToggle which is a control that is
		used to zoom panes within resize borders

AUTHOR:				Robin Mair

CREATION DATE :	96.08.14

CHANGE HISTORY :

		96.12.03		rtm	Moved the color constants defined in the header file for the
								UGraphicsUtilities, to this source file so that they can be
								removed from the header file. Switched to using the new
								constants provided in the color ramp to setup these constants.
		96.08.21		rtm	Changed the zoomed state appearance so that it looks more like 
								a selected button.
		96.08.14		rtm	Initial creation of file

---------------------------------------<< * >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "GAZoomToggle.h"

// * GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

enum
{
	zoom_Choice			=	7,
	zoom_Rows			=	9,
	zoom_Columns		=	9
};

enum
{
	choice_UnzoomedColor				=	0,
	choice_HilitedColor				=	1,
	choice_ZoomedColor				=	2,
	choice_DimmedColor				=	3,
	choice_UnzoomedBW					=	4,
	choice_HilitedBW					=	5,
	choice_ZoomedBW					=	6
};

// * COLOR CONSTANTS

const int 	B	=	colorRamp_Black;		//	Index for black
const int	W	=	colorRamp_White;		//	Index for white
const int	N	=	-1;						//	Index for no color which means draw nothing


//=====================================================================================
// DEFINES
//=====================================================================================

char GAZoomToggle::sPixelMap[zoom_Choice][zoom_Rows][zoom_Columns] = 
{
	//	1 OFF - color	
	{
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B },		//	0
		{ B,	1, 	1,		1, 	B,  	1,  	1, 	4,  	B },		//	1
		{ B,	1, 	10,	7, 	B,  	4,  	4, 	7,  	B },		//	2
		{ B,	1, 	7,		7, 	B,  	4,  	4, 	7,  	B },		//	3
		{ B,	B, 	B,		B, 	B,  	4,  	4, 	7,  	B },		//	4
		{ B,	1, 	4,		4, 	4,  	4,  	4, 	7,  	B },		//	5
		{ B,	1, 	4,		4, 	4,  	4,  	4, 	7,  	B },		//	6
		{ B,	4, 	7,		7, 	7,  	7,  	7, 	7,  	B },		//	7
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B }		//	8
	},
	//	2	HILITED - color		
	{
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B },		//	0
		{ B,	B, 	4,		4, 	B,  	4,  	4, 	B,  	B },		//	1
		{ B,	4, 	B,		4, 	B,  	4,  	B, 	4,  	B },		//	2
		{ B,	4, 	4,		B, 	4,  	B,  	4, 	4,  	B },		//	3
		{ B,	B, 	B,		4, 	4,  	4,  	B, 	B,  	B },		//	4
		{ B,	4, 	4,		B, 	4,  	B,  	4, 	4,  	B },		//	5
		{ B,	4, 	B,		4, 	B,  	4,  	B, 	4,  	B },		//	6
		{ B,	B, 	4,		4, 	B,  	4,  	4, 	B,  	B },		//	7
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B }		//	8
	},
	//	3	ZOOMED - color		
	{
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B },		//	0
		{ B,	9, 	9,		9, 	9,  	9,  	9, 	6,  	B },		//	1
		{ B,	9, 	7,		7, 	7,  	7,  	7, 	4,  	B },		//	2
		{ B,	9, 	7,		7, 	7,  	7,  	7, 	4,  	B },		//	3
		{ B,	9, 	7,		7, 	7,  	7,  	7, 	4,  	B },		//	4
		{ B,	9, 	7,		7, 	7,  	7,  	7, 	4,  	B },		//	5
		{ B,	9, 	7,		7, 	7,  	7,  	7, 	4,  	B },		//	6
		{ B,	7, 	4,		4, 	4,  	4,  	4, 	4,  	B },		//	7
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B }		//	8
	},
	//	4	DIMMED - color		
	{
		{ 7,	7, 	7,		7, 	7,  	7,  	7, 	7,  	7 },		//	0
		{ 7,	1, 	1,		1, 	7,  	1,  	1, 	2,  	7 },		//	1
		{ 7,	1, 	2,		2, 	7,  	2,  	2, 	5,  	7 },		//	2
		{ 7,	1, 	2,		2, 	7,  	2,  	2, 	5,  	7 },		//	3
		{ 7,	7, 	7,		7, 	7,  	2,  	2, 	5,  	7 },		//	4
		{ 7,	1, 	2,		2, 	2,  	2,  	2, 	5,  	7 },		//	5
		{ 7,	1, 	2,		2, 	2,  	2,  	2, 	5,  	7 },		//	6
		{ 7,	2, 	5,		5, 	5,  	5,  	5, 	5,  	7 },		//	7
		{ 7,	7, 	7,		7, 	7,  	7,  	7, 	7,  	7 }		//	8
	},
	//	5	OFF - B&W		
	{
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B },		//	0
		{ B,	W, 	W,		W, 	B,  	W,  	W, 	W,  	B },		//	1
		{ B,	W, 	W,		W, 	B,  	W,  	W, 	W,  	B },		//	2
		{ B,	W, 	W,		W, 	B,  	W,  	W, 	W,  	B },		//	3
		{ B,	B, 	B,		B, 	B,  	W,  	W, 	W,  	B },		//	4
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	5
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	6
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	7
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B }		//	8
	},
	//	6	HILITED - B&W		
	{
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B },		//	0
		{ B,	B, 	W,		W, 	B,  	W,  	W, 	B,  	B },		//	1
		{ B,	W, 	B,		W, 	B,  	W,  	B, 	W,  	B },		//	2
		{ B,	W, 	W,		B, 	W,  	B,  	W, 	W,  	B },		//	3
		{ B,	B, 	B,		W, 	W,  	W,  	B, 	B,  	B },		//	4
		{ B,	W, 	W,		B, 	W,  	B,  	W, 	W,  	B },		//	5
		{ B,	W, 	B,		W, 	B,  	W,  	B, 	W,  	B },		//	6
		{ B,	B, 	W,		W, 	B,  	W,  	W, 	B,  	B },		//	7
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B }		//	8
	},
	//	7	ZOOMED - B&W		
	{
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B },		//	0
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	1
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	2
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	3
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	4
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	5
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	6
		{ B,	W, 	W,		W, 	W,  	W,  	W, 	W,  	B },		//	7
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B }		//	8
	}
};

#pragma mark === CLASS: GAZoomToggle ===

//=====================================================================================
// ** STREAM CREATION
//-------------------------------------------------------------------------------------
// GAZoomToggle::CreateGAZoomToggleStream	
//-------------------------------------------------------------------------------------
//

GAZoomToggle*	
GAZoomToggle::CreateGAZoomToggleStream ( LStream *inStream )
{

	return ( new GAZoomToggle ( inStream ));
	
}	// GAZoomToggle::CreateGAZoomToggleStream


#pragma mark === INITIALIZATION ===

//=====================================================================================
// CLASS:	GAZoomToggle
//=====================================================================================

//=====================================================================================
// ** INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// GAZoomToggle::GAZoomToggle					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAZoomToggle::GAZoomToggle ()
{	

	mHilited = false;
	
}	//	GAZoomToggle::GAZoomToggle


//-------------------------------------------------------------------------------------
// GAZoomToggle::GAZoomToggle					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAZoomToggle::GAZoomToggle (	LStream	*inStream ) 
							: LControl ( inStream )
{

	// * Setup the hilite flag
	mHilited = false;

	// * Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 1 )
		SetMaxValue ( 1 );
		
}	//	GAZoomToggle::GAZoomToggle


//-------------------------------------------------------------------------------------
// GAZoomToggle::~GAZoomToggle				---<<* DESTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAZoomToggle::~GAZoomToggle ()
{
}	//	GAZoomToggle::~GAZoomToggle


#pragma mark === ACCESSORS ===

//=====================================================================================
// ** ACCESSORS
//-------------------------------------------------------------------------------------
// GAZoomToggle::SetHiliteState
//-------------------------------------------------------------------------------------

void			
GAZoomToggle::SetHiliteState	(	Boolean	inHiliteState )
{
	
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
	}
			
}	//	GAZoomToggle::SetHiliteState


//-------------------------------------------------------------------------------------
// GAZoomToggle::SetValue
//-------------------------------------------------------------------------------------

void			
GAZoomToggle::SetValue	( Int32	inValue )
{
	// * Call our parent to handle the setting of the value
	LControl::SetValue ( inValue );
	
	// * And make sure we get the hilite state changed
	if ( IsHilited ())
	{
		SetHiliteState ( false );
		
		// * Invalidate the radio so that it will be redrawn in the
		// correct state for the value setting
		Draw ( nil );
	}
	
}	//	GAZoomToggle::SetValue


//-------------------------------------------------------------------------------------
// GAZoomToggle::CalcLocalControlRect
//-------------------------------------------------------------------------------------

void
GAZoomToggle::CalcLocalControlRect ( Rect &outRect )
{

	// * Figure out the frame rect for the button which is based on the size
	// that was specified for the array that contains the pixel data, basically
	// all we ae doing is creating a rect of the correct size that is centered
	// inside the control.
	CalcLocalFrameRect ( outRect );
	Int16		hDelta = UGraphicsUtilities::RectWidth ( outRect ) - zoom_Columns;
	Int16	 	vDelta = UGraphicsUtilities::RectHeight ( outRect ) - zoom_Rows;
	outRect.top += vDelta / 2;
	outRect.left += hDelta / 2;
	outRect.bottom = outRect.top + zoom_Rows;
	outRect.right = outRect.left + zoom_Columns;
	
}	//	GAZoomToggle::CalcLocalControlRect


#pragma mark === STATE CHANGE ===

//=====================================================================================
// ** STATE CHANGE
//-------------------------------------------------------------------------------------
// GAZoomToggle::ActivateSelf
//-------------------------------------------------------------------------------------

void
GAZoomToggle::ActivateSelf()
{

	Show ();
	
}	//	GAZoomToggle::ActivateSelf


//-------------------------------------------------------------------------------------
// GAZoomToggle::DeactivateSelf
//-------------------------------------------------------------------------------------

void
GAZoomToggle::DeactivateSelf()
{

	Hide ();
	
}	//	GAZoomToggle::DeactivateSelf


#pragma mark === DRAWING ===

//=====================================================================================
// ** DRAWING
//-------------------------------------------------------------------------------------
// GAZoomToggle::DrawSelf
//-------------------------------------------------------------------------------------

void
GAZoomToggle::DrawSelf	()
{

	// * Get the control drawn
	DrawMiniPixelButton ();
	
}	//	GAZoomToggle::DrawSelf


//-------------------------------------------------------------------------------------
// GAZoomToggle::DrawMiniPixelButton
//-------------------------------------------------------------------------------------

void
GAZoomToggle::DrawMiniPixelButton	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// * Setup a device loop so that we can handle drawing at the correct bit depth
	// of the varoius pixel maps
	StDeviceLoop	theLoop ( localFrame );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// * Figure out the reectangle the pixel map will be drawn into
		Rect	pixelRect;
		CalcLocalControlRect ( pixelRect );

		// * In this section we are basically specifying which choice from the
		// pixel array is to be drawn for a given bit depth and control state
		if ( IsHilited ())			// ** BUTTON HILITED
		{
			DrawPixelsChoice ( 	depth >= 4 ? choice_HilitedColor : choice_HilitedBW, 
										pixelRect );
		}
		else if ( !IsEnabled ())	// * BUTTON DISABLED
		{
			if ( depth < 4 )
			{
				::PenPat ( &qd.gray );
				DrawPixelsChoice ( 	choice_UnzoomedBW, pixelRect );
			}
			else
				DrawPixelsChoice ( choice_DimmedColor, pixelRect );
		}
		else								// ** BUTTON NORMAL
		{
			if ( IsSelected ())
				DrawPixelsChoice ( 	(depth < 4 ? choice_ZoomedBW : choice_ZoomedColor),
											pixelRect );
			else
				DrawPixelsChoice ( 	(depth < 4 ? choice_UnzoomedBW : choice_UnzoomedColor), 
											pixelRect );	
		}
	}
	
}	//	GAZoomToggle::DrawMiniPixelButton


//-------------------------------------------------------------------------------------
// GAZoomToggle::DrawPixelsChoice
//-------------------------------------------------------------------------------------

void
GAZoomToggle::DrawPixelsChoice	( Int16	inChoice,
												Rect	&inFrame )
{
	
	// * Given the array specifying the pixel colors go ahead and draw
	// the pixels for the specified choice, where the choice is the
	// version of the pixel map that is being rendered
	Int16 colorindex;
	for ( register int row = 0; row < zoom_Rows; row++ )
		for ( register int column = 0; column < zoom_Columns; column++ )
			{
				colorindex = sPixelMap[inChoice][row][column];
				if ( colorindex != N )
					::SetCPixel ( 	inFrame.left + column, 
										inFrame.top + row, 
										&UGAColorRamp::GetColor(colorindex));
			}

}	//	GAZoomToggle::DrawPixelsChoice


#pragma mark === MOUSE TRACKING ===

//=====================================================================================
// ** MOUSE TRACKING
//-------------------------------------------------------------------------------------
// GAZoomToggle::HotSpotAction
//-------------------------------------------------------------------------------------

void
GAZoomToggle::HotSpotAction	(	Int16		/*inHotSpot*/,
												Boolean	inCurrInside,
												Boolean	inPrevInside )
{

	// * If the mouse moved in or out of the hot spot handle the
	// hiliting of the control
	if ( inCurrInside != inPrevInside )
	{

		// * We behave just like a checkbox which is basically a switch
		// so we simply toggle the state
		SetHiliteState ( !IsHilited ());

		// * Get the control redrawn so that any hiliting changes
		// can take effect
		Draw ( nil );
	}
		
}	//	GAZoomToggle::HotSpotAction


//-------------------------------------------------------------------------------------
// GAZoomToggle::HotSpotResult
//-------------------------------------------------------------------------------------

void
GAZoomToggle::HotSpotResult	(	Int16 /*inHotSpot*/ )
{
	
	// * We behave just like a checkbox which is basically a switch
	// so we simply toggle the state
	SetValue ( 1 - GetValue ());
	
}	//	GAZoomToggle::HotSpotResult


#pragma mark === BROADCASTING ===

//=====================================================================================
// ** BROADCASTING
//-------------------------------------------------------------------------------------
// GAZoomToggle::BroadcastValueMessage
//-------------------------------------------------------------------------------------

void
GAZoomToggle::BroadcastValueMessage ()
{
	if  ( mValueMessage != msg_Nothing ) 
	{
		BroadcastMessage ( mValueMessage, (void*) this );
	}
	
}	//	GAZoomToggle::BroadcastValueMessage


