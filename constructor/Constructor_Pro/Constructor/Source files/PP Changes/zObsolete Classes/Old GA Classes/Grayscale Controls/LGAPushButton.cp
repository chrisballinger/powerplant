// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAPushButton.cp

CLASSES:				LGAPushButton

DESCRIPTION:		Implementation file for LGAPushButton

AUTHOR:				Robin Mair

CREATION DATE :	96.05.25

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.03.26		rtm	Added an override of PointInHotSpot so that we can adjust the
								tracking of the mouse for the default button case so that we
								are don't track over the default outline.
		97.03.26		rtm	Changed the drawing methods so that there is now a DrawPushButton
								method again, as we can then just call it directly when tracking
								the hiliting thereby minimizing the number of times we are drawing
								the default outline if it is present.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		96.12.08		rtm	Changed justification constants to use the new ones defined
								in the universal headers.
		96.10.19		rtm	Fixed the drawing of title across multiple devices for the
								hilited case primarily as there was a situation where the
								title would not get drawn.  In the process cleaned up a lot
								of the drawing code for titles in B&W.
		96.10.19		rtm	Fixed a bug where the default border was not being erased when
								a button was cahnged from having the default to not, this fix
								is in SetDefaultButton.
		96.10.18		rtm	Found and fixed a bug where the dimmed version of the default
								push button was not drawing correctly.
		96.10.12		rtm	Had to tweak the drawing of titles across multiple devices
								disabled.  Using the grayishTextOr text mode transfer was
								resulting in some weirdness that could only be solved by
								doing something similar to what grayishTextOr did, which was
								to basically lighten the text color.
		96.10.09		rtm	Changed the title drawing so that it now will respect a color
								provided in a text trait as  opposed to always drawing the title
								in black.
		96.10.08		rtm	Modified DrawButtonTitle and DrawDefaultOutline so that they
								now take the current screen depth as an argument, these are
								then called from within the device loop in DrawSelf to minimize
								the number of device loops we need to create.
		96.10.08		rtm	Got rid of DrawButton by moving its contents to DrawSelf.
		96.09.27		rtm	Changed the drawing methods to use FrameRoundRect for the
								framing of the button even though it does not fully comply
								with AGA which specifies that two of the corner pixels should
								be a gray 12 color which meant FrameRoundRect could not
								be used.  Decided to sacrifice the spec in favor of efficiency.
		96.09.25		rtm	Changed the SetDefaultButton method so that it does the right
								thing about adjusting the size and location of the button if
								the default state is
		96.09.04		rtm	Changed the title drawing method so that it always draws the
								title centered in the button.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.01		rtm	Changed the rendering of the dimmed state of the control to
								meet Apple's modified spec for grayscale controls.
		96.06.20		rtm	Changed the SetHiliteState method so that it now handles the
								the redraw after a state change instead of all the other 
								places that were trying to take care of it.
		96.05.25		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAPushButton.h>
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

#pragma mark === CLASS: LGAPushButton
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LGAPushButton
//=====================================================================================

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAPushButton::LGAPushButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAPushButton::LGAPushButton ()
{	

	mDefault = false;
	mHilited = false;
	
}	//	LGAPushButton::LGAPushButton


//-------------------------------------------------------------------------------------
// LGAPushButton::LGAPushButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAPushButton::LGAPushButton 	(	const LGAPushButton &inOriginal )
								:	LControl ( inOriginal ),
									LGATitleMixin ( inOriginal )
{	

	mDefault = inOriginal.mDefault;
	mHilited = inOriginal.mHilited;
	
}	//	LGAPushButton::LGAPushButton


//-------------------------------------------------------------------------------------
// LGAPushButton::LGAPushButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAPushButton::LGAPushButton (	LStream	*inStream ) : LControl ( inStream )
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
	
	// е The default flag is set to false by default we will use it in a subclass
	mDefault = false;
	
	// е Make sure we start with the hilte state set to false
	mHilited = false;

}	//	LGAPushButton::LGAPushButton


//-------------------------------------------------------------------------------------
// LGAPushButton::LGAPushButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAPushButton::LGAPushButton (	const SPaneInfo 			&inPaneInfo,
											const SControlInfo 		&inControlInfo,
											Str255						inTitle,
											ResIDT						inTextTraitsID,
											Boolean						inDefault )
									: LControl ( 	inPaneInfo,
														inControlInfo.valueMessage,
														inControlInfo.value,
														inControlInfo.minValue,
														inControlInfo.maxValue )
{	

	// е Setup our fields
	mTitle = inTitle;
	mTextTraitsID = inTextTraitsID;
	mDefault = false;
	if ( inDefault )
		SetDefaultButton ( inDefault, false );
	
	// е Make sure we start with the hilte state set to false
	mHilited = false;

}	//	LGAPushButton::LGAPushButton


//-------------------------------------------------------------------------------------
// LGAPushButton::~LGAPushButton				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAPushButton::~LGAPushButton ()
{
}	//	LGAPushButton::~LGAPushButton


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAPushButton::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGAPushButton::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGAPushButton::GetDescriptor


//-------------------------------------------------------------------------------------
// LGAPushButton::CalcTitleRect
//-------------------------------------------------------------------------------------

void
LGAPushButton::CalcTitleRect	( Rect	&outRect )
{
	
	StTextState			theTextState;
	StColorPenState	thePenState;
	SInt16					bevelWidth = 2;
	
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
	::InsetRect ( &outRect, bevelWidth, bevelWidth );
	
	// е Adjust the title rect to match the height of the font
	outRect.top += (( UGraphicsUtilities::RectHeight ( outRect ) - textBaseline) / 2) - 2;
	outRect.bottom = outRect.top + textHeight;

}	//	LGAPushButton::CalcTitleRect


//-------------------------------------------------------------------------------------
// LGAPushButton::SetValue
//-------------------------------------------------------------------------------------
//

void
LGAPushButton::SetValue	(	SInt32	inValue )
{
	// е Call our superclass to handle the setting of the value
	LControl::SetValue ( inValue );
	
	// е If the button is being turned on or off we need to make sure that
	// its hilite state is set appropriately
	SetHiliteState ( inValue == Button_Off ? false : true );
	
}	//	LGAPushButton::SetValue


//-------------------------------------------------------------------------------------
// LGAPushButton::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGAPushButton::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGAPushButton::SetDescriptor


//-------------------------------------------------------------------------------------
// LGAPushButton::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGAPushButton::SetHiliteState	(	Boolean	inHiliteState )
{
	
	// е We only change the state if we haveo
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// е Get the button redrawn so the changed state can take effect
		if ( IsVisible () && FocusDraw ())
			DrawPushButton ();
	}
			
}	//	LGAPushButton::SetHiliteState


//-------------------------------------------------------------------------------------
// LGAPushButton::SetDefaultButton
//-------------------------------------------------------------------------------------

void
LGAPushButton::SetDefaultButton	(  Boolean inDefault,
												Boolean	inRedraw )
{
	
	// е Only change the default setting if it is different
	if ( mDefault != inDefault )
	{
		mDefault = inDefault;
		
		// е Get the size and location of the host pane
		SDimension16	buttonSize;
		GetFrameSize ( buttonSize );
		
		// е Adjust the size and location of the button depending on whether it
		// is the default or not
		if ( inDefault )
		{
			// е When we become the default button we need to grow by 6 pixels
			ResizeFrameTo ( 	buttonSize.width + 6, 
									buttonSize.height + 6, 
									false );
			
			// е When we become the default button we need to adjust the location by
			// 3 pixels up and left
			MoveBy ( -3, -3, false );

			// е Refresh everything if needed
			if ( inRedraw )
				Refresh ();
		}
		else
		{
		
			// е Refresh everything if needed, her we do it before we shrink the 
			// button so that the default border gets erased
			if ( inRedraw )
				Refresh ();
				
			// е When we are no longer the default button we need to shrink by 6 pixels
			ResizeFrameTo ( 	buttonSize.width - 6, 
									buttonSize.height - 6, 
									false );
			
			// е When we are no longer the default button we need to adjust the 
			// location by 3 pixels down and right
			MoveBy ( 3, 3, false );
		}
	}
		
}	//	LGAPushButton::SetDefaultButton


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAPushButton::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGAPushButton::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAPushButton::EnableSelf


//-------------------------------------------------------------------------------------
// LGAPushButton::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGAPushButton::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAPushButton::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAPushButton::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGAPushButton::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGAPushButton::ActivateSelf


//-------------------------------------------------------------------------------------
// LGAPushButton::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGAPushButton::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGAPushButton::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAPushButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// е Draw the button
	DrawPushButton ();
	
	// е If this is a default button then get the outline drawn
	// around the button in the appropriate bit depth
	if ( mDefault )
	{
		// е Get the frame for the control
		Rect	localFrame;
		CalcLocalFrameRect ( localFrame );

		// е Setup a device loop so that we can handle drawing at the correct bit depth
		StDeviceLoop	theLoop ( localFrame );
		SInt16				depth;
		while ( theLoop.NextDepth ( depth )) 
		{			
			DrawDefaultOutline ( depth );
		}
	}
	
}	//	LGAPushButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawPushButton
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawPushButton	()
{

	StColorPenState::Normalize ();
	
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
			// е Get the control drawn in its various states
			if ( IsEnabled ())
			{
				if ( IsHilited ())
					DrawButtonHilitedBW ();
				else
					DrawButtonNormalBW ();
			}
			else
				DrawButtonDimmedBW ();
		}
		// е COLOR
		else
		{
			// е Get the control drawn in its various states
			if ( IsEnabled () && IsActive ())
			{
				if ( IsHilited ())
						DrawButtonHilitedColor ();
				else
					DrawButtonNormalColor ();
			}
			else
				DrawButtonDimmedColor ();
		}	

		// е Call our title drawing routine to get the title drawn
		DrawButtonTitle ( depth );
			
	}
	
}	//	LGAPushButton::DrawPushButton


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawButtonTitle
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawButtonTitle	( SInt16	inDepth )
{

	StColorPenState	theColorPenState;
	StColorPenState::Normalize ();
	StTextState			theTextState;
	RGBColor				textColor;
	
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect	titleRect;
	
	// е Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Save off a reference to the fore color used to draw the title
	::GetForeColor ( &textColor );
	
	// е We always want to have the title be centered
	SInt16	titleJust = teCenter;
	
	// е Calculate the title rect
	CalcTitleRect ( titleRect );
	
	// е Get the title
	Str255 controlTitle;
	GetDescriptor ( controlTitle );
	RGBColor theColor;

	// е Handle drawing based on the current depth of the device
	if ( inDepth < 4 )
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
	else if ( inDepth >= 4 ) 
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
		if ( !IsEnabled () || !IsActive ())
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
	
}	//	LGAPushButton::DrawButtonTitle


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawDefaultOutline
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawDefaultOutline	( SInt16	inDepth )
{
	
	StColorPenState	theColorPenState;
	StColorPenState::Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е Handle the drawing based on the screen depth passed in
	// е BLACK & WHITE
	RGBColor	colorArray[5];
	RGBColor theColor;
	if ( inDepth < 4 )
	{
		// е Draw the frame for the outline around the outside
		colorArray[0] = UGAColorRamp::GetBlackColor();
		colorArray[1] = UGAColorRamp::GetBlackColor();
		colorArray[2] = UGAColorRamp::GetBlackColor();
		colorArray[3] = UGAColorRamp::GetBlackColor();
		colorArray[4] = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::AngledCornerFrame ( &localFrame, colorArray, 3 );
		::InsetRect ( &localFrame, 1, 1 );
		UGraphicsUtilities::AngledCornerFrame ( &localFrame, colorArray, 2 );
		
		// е Now we also need to paint a couple of pixels in the corners to make 
		// things look right
		// е TOP LEFT
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.top + 1, &theColor);
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 1, localFrame.top + 2, &theColor);
		
		// е TOP RIGHT
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.top + 1, &theColor);
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 2, localFrame.top + 2, &theColor);
		
		// е BOTTOM RIGHT
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.bottom - 2, &theColor);
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 2, localFrame.bottom - 3, &theColor);
		
		// е BOTTOM LEFT
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.bottom - 2, &theColor);
		theColor = UGAColorRamp::GetBlackColor();
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 1, localFrame.bottom - 3, &theColor);

	}
	// е COLOR
	else if ( inDepth >= 4 )
	{
		if ( IsEnabled () && IsActive ())
		{						
		
			// е Draw the frame for the outline around the outside
			colorArray[0] = UGAColorRamp::GetBlackColor();
			colorArray[1] = UGAColorRamp::GetColor(12);
			colorArray[2] = UGAColorRamp::GetBlackColor();
			colorArray[3] = UGAColorRamp::GetBlackColor();
			colorArray[4] = UGAColorRamp::GetColor(12);
			UGraphicsUtilities::AngledCornerFrame ( &localFrame, colorArray, 3 ); 

			//	е Draw light colored ring
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(2);
			::RGBForeColor ( &theColor);
			::FrameRoundRect ( &localFrame, 8, 8 );
			
			// е Draw the dark colored ring
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(5);
			::RGBForeColor ( &theColor );
			::FrameRoundRect ( &localFrame, 8, 8 );
			::InsetRect ( &localFrame, -2, -2 );
			
			// е Fixup some corner pixels
			// е TOP LEFT
			theColor = UGAColorRamp::GetColor(2);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.top + 3, &theColor);
			theColor = UGAColorRamp::GetColor(2);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, localFrame.top + 2, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, localFrame.top + 4, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 4, localFrame.top + 3, &theColor);
			
			// е TOP RIGHT
			theColor = UGAColorRamp::GetColor(3);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, localFrame.top + 1, &theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.top + 2, &theColor);
			theColor = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 2, localFrame.top + 3, &theColor);
			theColor = UGAColorRamp::GetColor(5);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, localFrame.top + 2, &theColor);
			theColor = UGAColorRamp::GetColor(5);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.top + 3, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 5, localFrame.top + 3, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, localFrame.top + 4, &theColor);
			
			// е BOTTOM RIGHT
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.bottom - 3, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 5, localFrame.bottom - 4, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, localFrame.bottom - 5, &theColor);
			theColor = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, localFrame.bottom - 3, &theColor);
			theColor = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.bottom - 4, &theColor);
			
			// е BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(3);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 1, localFrame.bottom - 4, &theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.bottom - 3, &theColor);
			theColor = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, localFrame.bottom - 2, &theColor);
			theColor = UGAColorRamp::GetColor(5);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.bottom - 4, &theColor);
			theColor = UGAColorRamp::GetColor(5);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, localFrame.bottom - 3, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, localFrame.bottom - 5, &theColor);
			theColor = UGAColorRamp::GetColor(8);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 4, localFrame.bottom - 4, &theColor);

			// е Draw bottom left shadows
			theColor = UGAColorRamp::GetColor(8);
			::RGBForeColor ( &theColor);
			::MoveTo ( localFrame.left + 4, localFrame.bottom - 2 );
			::LineTo ( localFrame.right - 4, localFrame.bottom - 2 );
			::MoveTo ( localFrame.right - 2, localFrame.bottom - 4 );
			::LineTo ( localFrame.right - 2, localFrame.top + 4 );
		}
		else
		{
			// е Setup the color array
			RGBColor 			colorArray[5];
		
			// е Draw the frame for the outline around the outside
			colorArray[0] = UGAColorRamp::GetColor(7);
			colorArray[1] = UGAColorRamp::GetColor(7);
			colorArray[2] = UGAColorRamp::GetColor(7);
			colorArray[3] = UGAColorRamp::GetColor(7);
			colorArray[4] = UGAColorRamp::GetColor(7);
			UGraphicsUtilities::AngledCornerFrame ( &localFrame, colorArray, 3 );

			//	е Fill in the default frame
			::InsetRect ( &localFrame, 1, 1 );
			theColor = UGAColorRamp::GetColor(4);
			::RGBForeColor ( &theColor);
			::FrameRoundRect ( &localFrame, 8, 8 );
			::InsetRect ( &localFrame, 1, 1 );
			::FrameRoundRect ( &localFrame, 8, 8 );
			::InsetRect ( &localFrame, -2, -2 );
			
			// е Fixup some corner pixels
			// е TOP LEFT
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, 
								localFrame.top + 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, 
								localFrame.top + 2, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, 
								localFrame.top + 4, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 4, 
								localFrame.top + 3, 
								&theColor);
			
			// е TOP RIGHT
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, 
								localFrame.top + 1, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, 
								localFrame.top + 2, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 2, 
								localFrame.top + 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, 
								localFrame.top + 2, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, 
								localFrame.top + 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 5, 
								localFrame.top + 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, 
								localFrame.top + 4, 
								&theColor);
			
			// е BOTTOM RIGHT
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, 
								localFrame.bottom - 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 5, 
								localFrame.bottom - 4, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, 
								localFrame.bottom - 5, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, 
								localFrame.bottom - 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, 
								localFrame.bottom - 4, 
								&theColor);
			
			// е BOTTOM LEFT
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 1, 
								localFrame.bottom - 4, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, 
								localFrame.bottom - 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, 
								localFrame.bottom - 2, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, 
								localFrame.bottom - 4, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, 
								localFrame.bottom - 3, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, 
								localFrame.bottom - 5, 
								&theColor);
			theColor = UGAColorRamp::GetColor(4);
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 4, 
								localFrame.bottom - 4, 
								&theColor);

		}
	}	
	
}	//	LGAPushButton::DrawDefaultOutline


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawButtonNormalBW
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawButtonNormalBW	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е If the button is set to be a default button then we need to make sure
	// we inset the rectangle
	if ( IsDefaultButton ())
		::InsetRect ( &localFrame, 3, 3 );
	
	// е Draw the frame for the control outside of everything else
	// the frame is draw in the foregorund color
	// е Draw the frame for the control
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
		
	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	theColor = UGAColorRamp::GetWhiteColor();
	::RGBBackColor ( &theColor);
	::InsetRect ( &localFrame, 1, 1 );
	::EraseRoundRect ( &localFrame, 4, 4 );
	
}	//	LGAPushButton::DrawButtonNormalBW


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawButtonHilitedBW
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawButtonHilitedBW	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е If the button is set to be a default button then we need to make sure
	// we inset the rectangle
	if ( IsDefaultButton ())
		::InsetRect ( &localFrame, 3, 3 );
	
	// е Draw the frame for the control outside of everything else
	// the frame is drawn in black
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
		
	// е Now we paint the inside of the button black as well
	::InsetRect ( &localFrame, 1, 1 );
	::PaintRoundRect ( &localFrame, 4, 4 );
	
}	//	LGAPushButton::DrawButtonHilitedBW


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawButtonDimmedBW
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawButtonDimmedBW	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е If the button is set to be a default button then we need to make sure
	// we inset the rectangle
	if ( IsDefaultButton ())
		::InsetRect ( &localFrame, 3, 3 );
	
	// е Draw the frame for the control outside of everything else
	// the frame is drawn in the foregorund color
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
	
	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	theColor = UGAColorRamp::GetWhiteColor();
	::RGBBackColor ( &theColor);
	::InsetRect ( &localFrame, 1, 1 );
	::EraseRoundRect ( &localFrame, 4, 4 );
	
}	//	LGAPushButton::DrawButtonDimmedBW


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawButtonNormalColor
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawButtonNormalColor	()
{
	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
		
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е If the button is set to be a default button then we need to make sure
	// we inset the rectangle
	if ( IsDefaultButton ())
		::InsetRect ( &localFrame, 3, 3 );
	
	// е Draw the frame for the control outside of everything else
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );

	// е Paint the face of the control first
	theColor = UGAColorRamp::GetColor(2);
	::RGBForeColor ( &theColor);
	::InsetRect ( &localFrame, 1, 1 );
	::PaintRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );
	
	// е LIGHT EDGES
	// е Start by rendering the bevelled edges of the sides facing the light
	// source
	theColor = UGAColorRamp::GetWhiteColor();
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::TopLeftSide ( 	&localFrame, 
						2, 					//	TOP
						2, 					//	LEFT
						3, 					// BOTTOM
						3 );					// RIGHT
	theColor = UGAColorRamp::GetWhiteColor();
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, localFrame.top + 3, &theColor);
	theColor = UGAColorRamp::GetColor(4);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 1, localFrame.top + 2, &theColor);
	theColor = UGAColorRamp::GetColor(4);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.top + 1, &theColor);
	theColor = UGAColorRamp::GetColor(4);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 1, localFrame.bottom - 3, &theColor);
	theColor = UGAColorRamp::GetColor(4);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.bottom - 2, &theColor);
	theColor = UGAColorRamp::GetColor(4);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.top + 1, &theColor);
	theColor = UGAColorRamp::GetColor(4);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 2, localFrame.top + 2, &theColor);

	// е SHADOW EDGES
	theColor = UGAColorRamp::GetColor(8);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left + 3, localFrame.bottom - 2 );
	::LineTo ( localFrame.right - 3, localFrame.bottom - 2 );
	::MoveTo ( localFrame.right - 2, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 2, localFrame.top + 3 );
	theColor = UGAColorRamp::GetColor(5);
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::BottomRightSide ( &localFrame, 
							3, 					//	TOP
							3, 					//	LEFT
							2, 					// BOTTOM
							2 );					// RIGHT
	theColor = UGAColorRamp::GetColor(8);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.bottom - 3, &theColor);
	theColor = UGAColorRamp::GetColor(5);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, localFrame.bottom - 4, &theColor);
		

}	//	LGAPushButton::DrawButtonNormalColor


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawButtonHilitedColor
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawButtonHilitedColor	()
{
	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
		
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е If the button is set to be a default button then we need to make sure
	// we inset the rectangle
	if ( IsDefaultButton ())
		::InsetRect ( &localFrame, 3, 3 );
	
	// е Draw the frame for the control outside of everything else
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );

	// е Paint the face of the control first
	theColor = UGAColorRamp::GetColor(9);
	::RGBForeColor ( &theColor);
	::InsetRect ( &localFrame, 1, 1 );
	::PaintRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );

	// е LIGHT EDGES
	// е Start by rendering the bevelled edges of the sides facing the light
	// source
	theColor = UGAColorRamp::GetColor(11);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left + 1, localFrame.bottom - 3 );
	::LineTo ( localFrame.left + 1, localFrame.top + 2 );
	::MoveTo ( localFrame.left + 2, localFrame.top + 1 );
	::LineTo ( localFrame.right - 3, localFrame.top + 1 );
	
	theColor = UGAColorRamp::GetColor(10);
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::TopLeftSide ( 	&localFrame, 
						2, 					//	TOP
						2, 					//	LEFT
						3, 					// BOTTOM
						3 );					// RIGHT
	theColor = UGAColorRamp::GetColor(10);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3, localFrame.top + 3, &theColor);
	theColor = UGAColorRamp::GetColor(11);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.top + 2, &theColor);
	theColor = UGAColorRamp::GetColor(8);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 2, localFrame.bottom - 2, &theColor);
	theColor = UGAColorRamp::GetColor(8);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 2, localFrame.top + 2, &theColor);
	
	// е SHADOW EDGES
	theColor = UGAColorRamp::GetColor(7);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left + 3, localFrame.bottom - 2 );
	::LineTo ( localFrame.right - 3, localFrame.bottom - 2 );
	::MoveTo ( localFrame.right - 2, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 2, localFrame.top + 3 );
	theColor = UGAColorRamp::GetColor(8);
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::BottomRightSide ( &localFrame, 
							3, 					//	TOP
							3, 					//	LEFT
							2, 					// BOTTOM
							2 );					// RIGHT
	theColor = UGAColorRamp::GetColor(7);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.bottom - 3, &theColor);
	theColor = UGAColorRamp::GetColor(8);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 4, localFrame.bottom - 4, &theColor);
		
}	//	LGAPushButton::DrawButtonHilitedColor


//-------------------------------------------------------------------------------------
// LGAPushButton::DrawButtonDimmedColor
//-------------------------------------------------------------------------------------

void
LGAPushButton::DrawButtonDimmedColor	()
{
	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
		
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е If the button is set to be a default button then we need to make sure
	// we inset the rectangle
	if ( IsDefaultButton ())
		::InsetRect ( &localFrame, 3, 3 );
	
	// е Draw the frame for the control outside of everything else
	RGBColor theColor = UGAColorRamp::GetColor(7);
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );

	// е Paint the face of the control
	theColor = UGAColorRamp::GetColor(2);
	::RGBForeColor ( &theColor);
	::InsetRect ( &localFrame, 1, 1 );
	::PaintRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );

}	//	LGAPushButton::DrawButtonDimmedColor


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGAPushButton::PointInHotSpot
//-------------------------------------------------------------------------------------
//

Boolean
LGAPushButton::PointInHotSpot (	Point 	inPoint,
											SInt16		/*inHotSpot*/ ) const
{

	Rect	frameRect;
	CalcLocalFrameRect ( frameRect );
	
	// е Now if the button is a default button we will inset the rect so
	// that we only track the actual button portion of the button
	if ( IsDefaultButton ())
		::InsetRect ( &frameRect, 3, 3 );
		
	// е We only return true if the mouse is in the popup portion of the
	// control not in the label
	return ::PtInRect ( inPoint, &frameRect );
	
}	//	LGAPushButton::PointInHotSpot


//-------------------------------------------------------------------------------------
// LGAPushButton::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGAPushButton::HotSpotAction	(	SInt16		/* inHotSpot */,
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
// LGAPushButton::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGAPushButton::HotSpotResult	(	SInt16 inHotSpot )
{
	
	// е Make sure that by default we turn the hiliting off
	HotSpotAction ( inHotSpot, false, true );
		
	// е Do the right thing about getting the value set after a button has been
	// clicked in its hotspot, NOTE: this method assumes the entire control is
	// the hotspot
	BroadcastValueMessage ();	
	
}	//	LGAPushButton::HotSpotResult



