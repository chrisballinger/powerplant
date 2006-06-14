// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

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

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGATextButton.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
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
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGATextButton::LGATextButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGATextButton::LGATextButton ()
{	

	mHilited = false;
	
}	//	LGATextButton::LGATextButton


//-------------------------------------------------------------------------------------
// LGATextButton::LGATextButton					---<<е CONSTRUCTOR е>>---
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
// LGATextButton::LGATextButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGATextButton::LGATextButton (	LStream	*inStream ) : LControl ( inStream )
{
	SInt16	controlKind;
	SInt32	macRefCon;
	
	// е We read this but don't do anything with it
	*inStream >> controlKind;

	// е Read in the text trait ID
	*inStream >> mTextTraitsID;
	
	// е Read in the title string
	inStream->ReadPString ( mTitle );
	
	// е We read this but don't do anything with it
	*inStream >> macRefCon;
	
	// е Make sure we start with the hilte state set to false
	mHilited = false;

}	//	LGATextButton::LGATextButton


//-------------------------------------------------------------------------------------
// LGATextButton::LGATextButton					---<<е CONSTRUCTOR е>>---
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

	// е Setup the clases fields
	mTitle = inTitle;
	mTextTraitsID = inTextTraitsID;
	
	// е Make sure we start with the hilte state set to false
	mHilited = false;

}	//	LGATextButton::LGATextButton


//-------------------------------------------------------------------------------------
// LGATextButton::~LGATextButton				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGATextButton::~LGATextButton ()
{
}	//	LGATextButton::~LGATextButton


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
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
	
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();

	// е Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Figure out the height of the text for the selected font
	FontInfo fInfo;
	GetFontInfo ( &fInfo );
	SInt16		textHeight = fInfo.ascent + fInfo.descent;
	SInt16		textBaseline = fInfo.ascent;
	
	// е Get the local inset frame rectangle
	CalcLocalFrameRect ( outRect );
	::InsetRect ( &outRect, 6, 2 );
	
	// е Adjust the title rect to match the height of the font
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
	// е Call our superclass to handle the setting of the value
	LControl::SetValue ( inValue );
	
	// е If the button is being turned on or off we need to make sure that
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
	
	// е Only change the state if we have too
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// е Make sure we get the button redrawn sothat the state change
		// can take effect
		Draw ( nil );
	}
			
}	//	LGATextButton::SetHiliteState


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGATextButton::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGATextButton::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );
	
}	//	LGATextButton::EnableSelf


//-------------------------------------------------------------------------------------
// LGATextButton::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGATextButton::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );
	
}	//	LGATextButton::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGATextButton::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGATextButton::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGATextButton::ActivateSelf


//-------------------------------------------------------------------------------------
// LGATextButton::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGATextButton::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGATextButton::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGATextButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGATextButton::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// е Get the button drawn in its various states
	if ( IsEnabled () && IsActive ())
	{
		if ( IsHilited ())
			DrawTextButtonHilited ();
		else
			DrawTextButtonNormal ();
	}
	else
		DrawTextButtonDimmed ();

	// е Call our title drawing routine
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
	
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect	titleRect;
	
	// е Setup the port text style fomr the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Setup the justification which is always center justified
	SInt16	titleJust = teCenter;
	
	// е Calculate the title rect
	CalcTitleRect ( titleRect );
	
	// е Save the primary text color
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// е Get the title
	Str255 controlTitle;
	GetDescriptor ( controlTitle );

	// е Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( titleRect );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )
		{
			// е If the control is dimmed then we use the grayishTextOr 
			// transfer mode to draw the text
			if ( !IsEnabled ())
			{
				theColor = UGAColorRamp::GetBlackColor ();
				::RGBForeColor ( &theColor );
				::TextMode ( grayishTextOr );
			}
			else if ( IsEnabled () && IsHilited () )
			{
				// е When we are hilited we simply draw the title in white
				theColor = UGAColorRamp::GetWhiteColor ();
				::RGBForeColor ( &theColor );
			}
			
			// е Now get the actual title drawn with all the appropriate settings
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
		}
		else
		{	
			
			// е If control is selected we always draw the text in the title
			// hilite color, if requested
			if ( IsHilited ())
			{
				theColor = UGAColorRamp::GetWhiteColor();
				::RGBForeColor ( &theColor );
			}
		
			// е If the control is dimmed then we have to do our own version of the
			// grayishTextOr as it does not appear to work correctly across
			// multiple devices
			if ( !IsEnabled () && !IsHilited () || !IsActive ())
			{
				textColor = UGraphicsUtilities::Lighten ( &textColor );
				::TextMode ( srcOr );
				::RGBForeColor ( &textColor );
			}

			// е Now get the actual title drawn with all the appropriate settings
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

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

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
			
			// е Draw the frame for the control outside of everything else
			// the frame is draw in the foreground color
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );
		}
		// е COLOR
		else
		{
			// е Get the bevel width
			SInt16		bevelWidth = 1;

			// е Setup a color array that will handle the number of bevels
			RGBColor colorArray[1];
			
			// е Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );

			// ее FRAME BUTTON
			theColor = UGAColorRamp::GetColor(9);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

			theColor = UGAColorRamp::GetColor(colorRamp_GrayA1);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

			// е TOP RIGHT
			theColor = UGAColorRamp::GetColor(10);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 1, 
																localFrame.top, 
																&theColor );
				
			// е BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(10);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left, 
																localFrame.bottom - 1, 
																&theColor );
		
			// е First make sure the face of the control is drawn
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(3);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// ее LIGHT BEVELS
			// е Setup Colors for top left edges
			colorArray[0] = UGAColorRamp::GetWhiteColor();
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_TopLeft, 
															colorArray, 
															bevelWidth );
			
			// ее SHADOW BEVELS
			// е Setup Colors for bottom right edges
			colorArray[0] = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_BottomRight, 
															colorArray, 
															bevelWidth );

			// е TOP RIGHT
			theColor = UGAColorRamp::GetColor(3);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 1, 
																localFrame.top, 
																&theColor );
				
			// е BOTTOM LEFT
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

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );
			theColor = UGAColorRamp::GetBlackColor ();
			::RGBForeColor ( &theColor );
			::PaintRect ( &localFrame );
		}
		// е COLOR
		else
		{
			// е Get the bevel width
			SInt16		bevelWidth = 1;

			// е Setup a color array that will handle the number of bevels
			RGBColor colorArray[1];
			
			// е Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );

			// ее FRAME BUTTON
			theColor = UGAColorRamp::GetColor(colorRamp_GrayA2);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

			theColor = UGAColorRamp::GetColor(11);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

			// е TOP RIGHT
			theColor = UGAColorRamp::GetColor(12);
			UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
									localFrame.top, 
									&theColor );
				
			// е BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(12);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left, 
																localFrame.bottom - 1, 
																&theColor );
		
			// е First make sure the face of the control is drawn
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(7);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// ее LIGHT BEVELS
			// е Setup Colors
			colorArray[0] = UGAColorRamp::GetColor(10);
			
			// е Draw top and left edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_TopLeft,
															 colorArray, 
															 bevelWidth );
			
			// ее SHADOW BEVELS
			// е Setup Colors
			colorArray[0] = UGAColorRamp::GetColor(5);
			
			// е Draw bottom and right edges
			UGraphicsUtilities::BevelledEdge ( &localFrame, 
															rectEdge_BottomRight, 
															colorArray, 
															bevelWidth );
			
			// ее CORNER PIXELS
			// е TOP RIGHT
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 1, 
																localFrame.top, 
																&theColor );
				
			// е BOTTOM LEFT
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

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

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
			
			// е Draw the frame for the control outside of everything else
			// the frame is draw in the foreground color
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );
		}
		// е COLOR
		else
		{
			StColorPenState::Normalize ();
			
			// е Get the frame for the control
			Rect	localFrame;
			CalcLocalFrameRect ( localFrame );

			// е DISABLED NORMAL STATE
			if ( !IsHilited ())
			{
				// ее FRAME BUTTON
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
			
				// ее FRAME BUTTON
				theColor = UGAColorRamp::GetColor(8);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

				theColor = UGAColorRamp::GetColor(6);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

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

			// е Paint the face of the button
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
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGATextButton::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGATextButton::HotSpotAction	(	SInt16		/* inHotSpot */,
											Boolean	inCurrInside,
											Boolean	inPrevInside )
{

	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control which is slightly different for
	// each of the modes supported by the control
	if ( inCurrInside != inPrevInside )
	{
		// е For a straight button all we have to worry about
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
	
	// е Make sure that by default we turn the hiliting off
	HotSpotAction ( inHotSpot, false, true );
		
	// е Do the right thing about getting the value set after a button has been
	// clicked in its hotspot, NOTE: this method assumes the entire control is
	// the hotspot
	BroadcastValueMessage ();	
	
}	//	LGATextButton::HotSpotResult



