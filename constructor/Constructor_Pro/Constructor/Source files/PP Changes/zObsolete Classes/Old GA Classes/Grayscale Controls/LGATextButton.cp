// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGATextButton.cp

CLASSES:				LGATextButton

DESCRIPTION:		Implementation file for LGATextButton

AUTHOR:				Robin Mair

CREATION DATE :	96.06.16

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		97.01.05		rtm	Fixed a bug where the corner pixels were not being drawn
								correctly.
		96.12.08		rtm	Changed justification constants to use the new ones defined
								in the universal headers.
		96.12.06		rtm	Tweaked some of the color constants as the indices have changed
								a little in the color ramp. Actually got rid of them and now
								use the contants provided by the enum in the color ramp
		96.12.03		rtm	Added a couple of color constants now that we no longer have
								them in the color ramp header file.
		96.10.19		rtm	Fixed the drawing of title across multiple devices for the
								hilited case primarily as there was a situation where the
								title would not get drawn.  In the process cleaned up a lot
								of the drawing code for titles in B&W.
		96.10.12		rtm	Had to tweak the drawing of titles across multiple devices
								disabled.  Using the grayishTextOr text mode transfer was
								resulting in some weirdness that could only be solved by
								doing something similar to what grayishTextOr did, which was
								to basically lighten the text color.
		96.10.09		rtm	Changed the title drawing so that it now will respect a color
								provided in a text trait as  opposed to always drawing the title
								in black.
		96.10.07		rtm	Changed the rendering so that it is identical to the mini icon
								case from the standard AGA bevel button.
		96.09.19		rtm	Changed the title drawing method so that it always centers
								the title and therefore ignores the justification of any
								associated text trait.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.02		rtm	Changed the rendering of the dimmed state of the control to
								meet Apple's modified spec for grayscale controls.
		96.06.20		rtm	Changed the SetHiliteState method so that it now handles the
								the redraw after a state change instead of all the other 
								places that were trying to take care of it.
		96.06.16		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include <LGATextButton.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <LStream.h>
#include <PP_Messages.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <LString.h>
#include <UTextTraits.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGATextButton
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LGATextButton
//=====================================================================================

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGATextButton::LGATextButton					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGATextButton::LGATextButton ()
{	

	mHilited = false;
	
}	//	LGATextButton::LGATextButton


//-------------------------------------------------------------------------------------
// LGATextButton::LGATextButton					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGATextButton::LGATextButton 	(	const LGATextButton &inOriginal )
								:	LControl ( inOriginal ),
									LGATitleMixin ( inOriginal )
{	

	mHilited = inOriginal.mHilited;
	
}	//	LGATextButton::LGATextButton


//-------------------------------------------------------------------------------------
// LGATextButton::LGATextButton					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGATextButton::LGATextButton (	LStream	*inStream ) : LControl ( inStream )
{
	SInt16	controlKind;
	SInt32	macRefCon;
	
	// � We read this but don't do anything with it
	*inStream >> controlKind;

	// � Read in the text trait ID
	*inStream >> mTextTraitsID;
	
	// � Read in the title string
	inStream->ReadPString ( mTitle );
	
	// � We read this but don't do anything with it
	*inStream >> macRefCon;
	
	// � Make sure we start with the hilte state set to false
	mHilited = false;

}	//	LGATextButton::LGATextButton


//-------------------------------------------------------------------------------------
// LGATextButton::LGATextButton					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGATextButton::LGATextButton (	const SPaneInfo 			&inPaneInfo,
											const SControlInfo 		&inControlInfo,
											Str255						inTitle,
											ResIDT						inTextTraitsID )
										: LControl ( 	inPaneInfo,
															inControlInfo.valueMessage,
															inControlInfo.value,
															inControlInfo.minValue,
															inControlInfo.maxValue )
{	

	// � Setup the clases fields
	mTitle = inTitle;
	mTextTraitsID = inTextTraitsID;
	
	// � Make sure we start with the hilte state set to false
	mHilited = false;

}	//	LGATextButton::LGATextButton


//-------------------------------------------------------------------------------------
// LGATextButton::~LGATextButton				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGATextButton::~LGATextButton ()
{
}	//	LGATextButton::~LGATextButton


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LGATextButton::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGATextButton::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGATextButton::GetDescriptor


//-------------------------------------------------------------------------------------
// LGATextButton::CalcTitleRect
//-------------------------------------------------------------------------------------

void
LGATextButton::CalcTitleRect	( Rect	&outRect )
{
	
	StTextState			theTextState;
	StColorPenState	thePenState;
	
	// � Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();

	// � Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// � Figure out the height of the text for the selected font
	FontInfo fInfo;
	GetFontInfo ( &fInfo );
	SInt16		textHeight = fInfo.ascent + fInfo.descent;
	SInt16		textBaseline = fInfo.ascent;
	
	// � Get the local inset frame rectangle
	CalcLocalFrameRect ( outRect );
	::InsetRect ( &outRect, 6, 2 );
	
	// � Adjust the title rect to match the height of the font
	outRect.top += (( UGraphicsUtilities::RectHeight ( outRect ) - textBaseline) / 2) - 2;
	outRect.bottom = outRect.top + textHeight;
	
}	//	LGATextButton::CalcTitleRect


//-------------------------------------------------------------------------------------
// LGATextButton::SetValue
//-------------------------------------------------------------------------------------
//

void
LGATextButton::SetValue	(	SInt32	inValue )
{
	// � Call our superclass to handle the setting of the value
	LControl::SetValue ( inValue );
	
	// � If the button is being turned on or off we need to make sure that
	// its hilite state is set appropriately
	SetHiliteState ( inValue == Button_Off ? false : true );
	
}	//	LGATextButton::SetValue


//-------------------------------------------------------------------------------------
// LGATextButton::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGATextButton::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGATextButton::SetDescriptor


//-------------------------------------------------------------------------------------
// LGATextButton::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGATextButton::SetHiliteState	(	Boolean	inHiliteState )
{
	
	// � Only change the state if we have too
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// � Make sure we get the button redrawn sothat the state change
		// can take effect
		Draw ( nil );
	}
			
}	//	LGATextButton::SetHiliteState


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// �� ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGATextButton::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGATextButton::EnableSelf ()
{

	// � Get things redrawn so that we can see the state change
	Draw ( nil );
	
}	//	LGATextButton::EnableSelf


//-------------------------------------------------------------------------------------
// LGATextButton::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGATextButton::DisableSelf ()
{

	// � Get things redrawn so that we can see the state change
	Draw ( nil );
	
}	//	LGATextButton::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// �� ACTIVATION
//-------------------------------------------------------------------------------------
// LGATextButton::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGATextButton::ActivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGATextButton::ActivateSelf


//-------------------------------------------------------------------------------------
// LGATextButton::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGATextButton::DeactivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGATextButton::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LGATextButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGATextButton::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// � Get the button drawn in its various states
	if ( IsEnabled () && IsActive ())
	{
		if ( IsHilited ())
			DrawTextButtonHilited ();
		else
			DrawTextButtonNormal ();
	}
	else
		DrawTextButtonDimmed ();

	// � Call our title drawing routine
	DrawTextButtonTitle ();
		
}	//	LGATextButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGATextButton::DrawTextButtonTitle
//-------------------------------------------------------------------------------------

void
LGATextButton::DrawTextButtonTitle	()
{
		
	StTextState			theTextState;
	StColorPenState::Normalize ();
	
	// � Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect	titleRect;
	
	// � Setup the port text style fomr the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// � Setup the justification which is always center justified
	SInt16	titleJust = teCenter;
	
	// � Calculate the title rect
	CalcTitleRect ( titleRect );
	
	// � Save the primary text color
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// � Get the title
	Str255 controlTitle;
	GetDescriptor ( controlTitle );

	// � Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( titleRect );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )
		{
			// � If the control is dimmed then we use the grayishTextOr 
			// transfer mode to draw the text
			if ( !IsEnabled ())
			{
				theColor = UGAColorRamp::GetBlackColor ();
				::RGBForeColor ( &theColor );
				::TextMode ( grayishTextOr );
			}
			else if ( IsEnabled () && IsHilited () )
			{
				// � When we are hilited we simply draw the title in white
				theColor = UGAColorRamp::GetWhiteColor ();
				::RGBForeColor ( &theColor );
			}
			
			// � Now get the actual title drawn with all the appropriate settings
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
		}
		else
		{	
			
			// � If control is selected we always draw the text in the title
			// hilite color, if requested
			if ( IsHilited ())
			{
				theColor = UGAColorRamp::GetWhiteColor();
				::RGBForeColor ( &theColor );
			}
		
			// � If the control is dimmed then we have to do our own version of the
			// grayishTextOr as it does not appear to work correctly across
			// multiple devices
			if ( !IsEnabled () && !IsHilited () || !IsActive ())
			{
				textColor = UGraphicsUtilities::Lighten ( &textColor );
				::TextMode ( srcOr );
				::RGBForeColor ( &textColor );
			}

			// � Now get the actual title drawn with all the appropriate settings
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
		}	
	}
	
}	//	LGATextButton::DrawTextButtonTitle


//-------------------------------------------------------------------------------------
// LGATextButton::DrawTextButtonNormal
//-------------------------------------------------------------------------------------

void
LGATextButton::DrawTextButtonNormal	()
{
	
	StColorPenState::Normalize ();

	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Before we draw anything we need to erase the control in case we
			// we had previously been hilited
			theColor = UGAColorRamp::GetWhiteColor();
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

			// �� FRAME BUTTON
			theColor = UGAColorRamp::GetColor(9);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

			theColor = UGAColorRamp::GetColor(colorRamp_GrayA1);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

			// � TOP RIGHT
			theColor = UGAColorRamp::GetColor(10);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 1, 
																localFrame.top, 
																&theColor );
				
			// � BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(10);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left, 
																localFrame.bottom - 1, 
																&theColor );
		
			// � First make sure the face of the control is drawn
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(3);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// �� LIGHT BEVELS
			// � Setup Colors for top left edges
			colorArray[0] = UGAColorRamp::GetWhiteColor();
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_TopLeft, 
															colorArray, 
															bevelWidth );
			
			// �� SHADOW BEVELS
			// � Setup Colors for bottom right edges
			colorArray[0] = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_BottomRight, 
															colorArray, 
															bevelWidth );

			// � TOP RIGHT
			theColor = UGAColorRamp::GetColor(3);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 1, 
																localFrame.top, 
																&theColor );
				
			// � BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(3);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left, 
																localFrame.bottom - 1, 
																&theColor );
		
		}	
	}
	
}	//	LGATextButton::DrawTextButtonNormal


//-------------------------------------------------------------------------------------
// LGATextButton::DrawTextButtonHilited
//-------------------------------------------------------------------------------------

void
LGATextButton::DrawTextButtonHilited	()
{
	
	StColorPenState::Normalize ();

	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );
			theColor = UGAColorRamp::GetBlackColor ();
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

			// �� FRAME BUTTON
			theColor = UGAColorRamp::GetColor(colorRamp_GrayA2);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

			theColor = UGAColorRamp::GetColor(11);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

			// � TOP RIGHT
			theColor = UGAColorRamp::GetColor(12);
			UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
									localFrame.top, 
									&theColor );
				
			// � BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(12);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left, 
																localFrame.bottom - 1, 
																&theColor );
		
			// � First make sure the face of the control is drawn
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(7);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// �� LIGHT BEVELS
			// � Setup Colors
			colorArray[0] = UGAColorRamp::GetColor(10);
			
			// � Draw top and left edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_TopLeft,
															 colorArray, 
															 bevelWidth );
			
			// �� SHADOW BEVELS
			// � Setup Colors
			colorArray[0] = UGAColorRamp::GetColor(5);
			
			// � Draw bottom and right edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_BottomRight, 
															colorArray, 
															bevelWidth );
			
			// �� CORNER PIXELS
			// � TOP RIGHT
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 1, 
																localFrame.top, 
																&theColor );
				
			// � BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left, 
																localFrame.bottom - 1, 
																&theColor );
		
		}	
	}
	
}	//	LGATextButton::DrawTextButtonHilited


//-------------------------------------------------------------------------------------
// LGATextButton::DrawTextButtonDimmed
//-------------------------------------------------------------------------------------

void
LGATextButton::DrawTextButtonDimmed	()
{

	StColorPenState::Normalize ();

	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Before we draw anything we need to erase the control in case we
			// we had previously been hilited
			theColor = UGAColorRamp::GetWhiteColor();
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
			StColorPenState::Normalize ();
			
			// � Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );

			// � DISABLED NORMAL STATE
			if ( !IsHilited ())
			{
				// �� FRAME BUTTON
				theColor = UGAColorRamp::GetColor(5);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

				
				theColor = UGAColorRamp::GetColor(6);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				
			}
			else	// DISABLED HILITED STATE
			{
			
				StColorPenState::Normalize ();
			
				// �� FRAME BUTTON
				theColor = UGAColorRamp::GetColor(8);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

				theColor = UGAColorRamp::GetColor(6);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

				// � TOP RIGHT
				theColor = UGAColorRamp::GetColor(7);
				UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
										localFrame.top, 
										&theColor );
					
				// � BOTTOM LEFT
				theColor = UGAColorRamp::GetColor(7);
				UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
										localFrame.bottom - 1, 
										&theColor );
			}

			// � Paint the face of the button
			::InsetRect ( &localFrame, 1, 1 );
			theColor =  !IsHilited () ? UGAColorRamp::GetColor(2) : UGAColorRamp::GetColor(4);
			::RGBForeColor ( &theColor );
			::PaintRect ( &localFrame );
		}	
	}
	
}	//	LGATextButton::DrawTextButtonDimmed


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// �� MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGATextButton::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGATextButton::HotSpotAction	(	SInt16		/* inHotSpot */,
											Boolean	inCurrInside,
											Boolean	inPrevInside )
{

	// � If the mouse moved in or out of the hot spot handle the
	// hiliting of the control which is slightly different for
	// each of the modes supported by the control
	if ( inCurrInside != inPrevInside )
	{
		// � For a straight button all we have to worry about
		// is switching the hiliting of the button
		SetHiliteState ( inCurrInside );
	}
		
}	//	GModeControl::HotSpotAction


//-------------------------------------------------------------------------------------
// LGATextButton::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGATextButton::HotSpotResult	(	SInt16 inHotSpot )
{
	
	// � Make sure that by default we turn the hiliting off
	HotSpotAction ( inHotSpot, false, true );
		
	// � Do the right thing about getting the value set after a button has been
	// clicked in its hotspot, NOTE: this method assumes the entire control is
	// the hotspot
	BroadcastValueMessage ();	
	
}	//	LGATextButton::HotSpotResult



