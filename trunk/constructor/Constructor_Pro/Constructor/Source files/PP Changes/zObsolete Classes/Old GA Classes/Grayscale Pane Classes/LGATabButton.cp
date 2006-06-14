// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGATabButton.cp

CLASSES:				LGATabButton

DESCRIPTION:		Implementation file for LGATabButton

AUTHOR:				Robin Mair

CREATION DATE :	96.12.12

CHANGE HISTORY :

		97.11.26		rtm	Fixed a real bad little bug that would never allow a large size
								tab button to be created, oops! The fix is in CalcTabButtonSize
								where the initial tab size had been set to 10 but then in the
								loading of the text trait if the text trait was setup to be
								the System font [0] no handle is loaded as it is built into PP
								and therefore the large size never got selected.  The fix was
								to set the initial value to 12.
		97.04.20		rtm	Removed the CreateFromStream method.
		97.01.27		rtm	Fixed a few cases where drawing could occur when the button
								was not visible.
		97.01.24		rtm	Added a copy constructor.
		97.01.20		rtm	Added a check to make sure the control is visible before we
								call DrawSelf, otherwise the control is drawn even if it is
								not currently visible.
		96.12.17		rtm	Modified HotSpotResult so that it no longer toggles the hilite
								state by calling HotSpotAction, instead it just sets the flag
								directly, thereby saving an additional redraw.  Before the
								normal state would first be drawn and then the On state.
		96.12.17		rtm	Some tweaks to fix the disabling of the tab button. Added the
								overrides for EnableSelf and DisableSelf.
		96.12.17		rtm	Deleted the stream creation function and the stream constructor.
		96.12.16		rtm	Fixed a bug where the button was also broadcasting a control
								clicked message which was unnecessary as it is not used by the
								tab panel.
		96.12.16		rtm	Fixed a bug in the rendering of the dimmed color version of the
								button, that was causing some of the shading from the normal
								view not to be overdrawn.
		96.12.15		rtm	Implemented the rendering of the 12 point version of the button
								which also involved calculating a region for that size button.
		96.12.14		rtm	Modified the hit testing so that it now uses the tab button
								region to test the mouse down, this means the tab will only
								hilite when the mouse is actually in the tab, not just in its
								frame.
		96.12.14		rtm	All rendering for the 10point tab size is now complete, now I
								have to create the 12point versions which add 5 more rows in
								the middle section of the slope.
		96.12.14		rtm	Implemented the shading for the right slope of the tab, really
								compplicated rendering involved on these tabs, I somehow think
								they are going to draw real slow on a slow machine.
		96.12.14		rtm	Fixed the rendering on black & white screens.
		96.12.13		rtm	Implemented the rendering of the slope details for the button.
								Which involved implementing the button region calculations and
								the rendering of the two slopes.  This process also invloved
								some tweaking of the main drawing routines.
		96.12.12		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGATabButton.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LControl.h>
#include <LStream.h>
#include <UTextTraits.h>
#include <UDrawingUtils.h>
#include <UDrawingState.h>
#include <LString.h>
#include <PP_Messages.h>

// е TOOLBOX HEADERS
#include <Quickdraw.h>
#include <TextEdit.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

const	SInt16			tabButton_10TabBottomOffset 			= 6;
const	SInt16			tabButton_12TabBottomOffset 			= 9;
const	SInt16			tabButton_tabButtonSlope	 			= 11;


//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGATabButton

//=====================================================================================
// CLASS:	LGATabButton
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION & DESTRUCTION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGATabButton::LGATabButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGATabButton::LGATabButton ()
{	
	
	mHilited = false;
	
}	//	LGATabButton::LGATabButton


//-------------------------------------------------------------------------------------
// LGATabButton::LGATabButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGATabButton::LGATabButton (	const LGATabButton	&inOriginal )
							:	LControl ( inOriginal ),
								LGATitleMixin ( inOriginal )
{	
	
	mHilited = inOriginal.mHilited;
	
}	//	LGATabButton::LGATabButton


//-------------------------------------------------------------------------------------
// LGATabButton::LGATabButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGATabButton::LGATabButton (	const SPaneInfo 			&inPaneInfo,
										const SControlInfo 		&inControlInfo,
										ConstStringPtr				inTitle,
										Boolean						inHiliteTitle,
										ResIDT						inTextTraitsID )
								: LControl ( 	inPaneInfo,
													inControlInfo.valueMessage,
													inControlInfo.value,
													inControlInfo.minValue,
													inControlInfo.maxValue )
{

	mTitle = inTitle;
	mHiliteTitle = inHiliteTitle;
	mTextTraitsID = inTextTraitsID;
	mHilited = false;
	
}	//	LGATabButton::LGATabButton


//-------------------------------------------------------------------------------------
// LGATabButton::~LGATabButton				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGATabButton::~LGATabButton ()
{
}	//	LGATabButton::~LGATabButton


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGATabButton::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGATabButton::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGATabButton::GetDescriptor


//-------------------------------------------------------------------------------------
// LGATabButton::IsLeftMostButton
//-------------------------------------------------------------------------------------

Boolean
LGATabButton::IsLeftMostButton	()
{
	SPoint32 	buttonLoc;
	GetFrameLocation ( buttonLoc );
	
	return buttonLoc.h == 6;
			
}	//	LGATabButton::IsLeftMostButton


//-------------------------------------------------------------------------------------
// LGATabButton::CalcTitleRect
//-------------------------------------------------------------------------------------

void
LGATabButton::CalcTitleRect	( Rect	&outRect )
{
	
	StTextState			theTextState;

	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();

	// е Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Figure out the height of the text for the selected font
	FontInfo fInfo;
	::GetFontInfo ( &fInfo );
	SInt16		textHeight = fInfo.ascent + fInfo.descent;
		
	// е Figure out the size of the title rect starting from the local frame
	// we have to subtract the width of the side slopes for the button and
	// then setup the correct location for the title based on AGA for tab size
	CalcLocalFrameRect ( outRect );
	outRect.bottom -= CalcTabButtonSize () == 10 ? tabButton_10TabBottomOffset - 
								fInfo.descent : tabButton_12TabBottomOffset - fInfo.descent;
	outRect.left += tabButton_tabButtonSlope;
	outRect.right -= tabButton_tabButtonSlope;
	outRect.top = outRect.bottom - textHeight;
	
}	//	LGATabButton::CalcTitleRect


//-------------------------------------------------------------------------------------
// LGATabButton::CalcTabButtonSize													
//-------------------------------------------------------------------------------------

SInt16
LGATabButton::CalcTabButtonSize () const
{
	
	SInt16 tabSize = 12;
	
	// е Look in the associated text trait to figure out what the
	// point size forthe font is set to be so we can correctly
	// determine the size of the tab which can only be either 10 or 12
	ResIDT	textTID = GetTextTraitsID ();
	TextTraitsH traitsH = UTextTraits::LoadTextTraits ( textTID );
	if ( traitsH ) 
	{
		tabSize =  (**traitsH).size;
		if ( tabSize < 10 )
			tabSize = 10;
		else if ( tabSize > 12 )
			tabSize = 12;
	}
	
	// е Return the tab size which affects the tabs height
	return tabSize;
	
}	//	LGATabButton::CalcTabButtonSize


//-------------------------------------------------------------------------------------
// LGATabButton::CalcTabButtonRegion													
//-------------------------------------------------------------------------------------

RgnHandle
LGATabButton::CalcTabButtonRegion () const
{
	
	//	е Return the approriate region based on the size of the button
	if ( CalcTabButtonSize () == 10 )
		return Calc10PtTabButtonRegion ();
	else
		return Calc12PtTabButtonRegion ();
		
}	//	LGATabButton::CalcTabButtonRegion


//-------------------------------------------------------------------------------------
// LGATabButton::Calc10PtTabButtonRegion													
//-------------------------------------------------------------------------------------

RgnHandle
LGATabButton::Calc10PtTabButtonRegion () const
{
	
	// е TEMPORARY
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е Left Slope
	RgnHandle leftSlopeRgn = ::NewRgn ();
	::OpenRgn ();
		// ROW 3 & 4
		::MoveTo ( localFrame.left, localFrame.bottom - 3 );
		::LineTo ( localFrame.left, localFrame.bottom - 4 );
		// ROW 5 & 6
		::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
		::LineTo ( localFrame.left + 1, localFrame.bottom - 6 );
		// ROW 7 & 8
		::LineTo ( localFrame.left + 2, localFrame.bottom - 7 );
		::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
		// ROW 9 & 10
		::LineTo ( localFrame.left + 3, localFrame.bottom - 9 );
		::LineTo ( localFrame.left + 3, localFrame.bottom - 10 );
		// ROW 11 & 12
		::LineTo ( localFrame.left + 4, localFrame.bottom - 11 );
		::LineTo ( localFrame.left + 4, localFrame.bottom - 12 );
		// ROW 13 & 14
		::LineTo ( localFrame.left + 5, localFrame.bottom - 13 );
		::LineTo ( localFrame.left + 5, localFrame.bottom - 14 );
		// ROW 15 & 16
		::LineTo ( localFrame.left + 6, localFrame.bottom - 15 );
		::LineTo ( localFrame.left + 6, localFrame.bottom - 16 );
		// ROW 17
		::LineTo ( localFrame.left + 7, localFrame.bottom - 17 );
		// ROW 18
		::LineTo ( localFrame.left + 8, localFrame.bottom - 18 );
		::LineTo ( localFrame.left + 9, localFrame.bottom - 18 );
		// ROW 19
		::LineTo ( localFrame.left + 10, localFrame.bottom - 19 );
		::LineTo ( localFrame.left + 11, localFrame.bottom - 19 );
		// BACK TO ROW 1
		::LineTo ( localFrame.left + 11, localFrame.bottom - 3 );
		::LineTo ( localFrame.left, localFrame.bottom - 3 );
	::CloseRgn ( leftSlopeRgn );
	
	// е Right Slope
	RgnHandle rightSlopeRgn = ::NewRgn ();
	::OpenRgn ();
		// ROW 3 & 4
		::MoveTo ( localFrame.right - 1, localFrame.bottom - 3 );
		::LineTo ( localFrame.right - 1, localFrame.bottom - 4 );
		// ROW 5 & 6
		::LineTo ( localFrame.right - 2, localFrame.bottom - 5 );
		::LineTo ( localFrame.right - 2, localFrame.bottom - 6 );
		// ROW 7 & 8
		::LineTo ( localFrame.right - 3, localFrame.bottom - 7 );
		::LineTo ( localFrame.right - 3, localFrame.bottom - 8 );
		// ROW 9 & 10
		::LineTo ( localFrame.right - 4, localFrame.bottom - 9 );
		::LineTo ( localFrame.right - 4, localFrame.bottom - 10 );
		// ROW 11 & 12
		::LineTo ( localFrame.right - 5, localFrame.bottom - 11 );
		::LineTo ( localFrame.right - 5, localFrame.bottom - 12 );
		// ROW 13 & 14
		::LineTo ( localFrame.right - 6, localFrame.bottom - 13 );
		::LineTo ( localFrame.right - 6, localFrame.bottom - 14 );
		// ROW 15 & 16
		::LineTo ( localFrame.right - 7, localFrame.bottom - 15 );
		::LineTo ( localFrame.right - 7, localFrame.bottom - 16 );
		// ROW 17
		::LineTo ( localFrame.right - 8, localFrame.bottom - 17 );
		// ROW 18
		::LineTo ( localFrame.right - 9, localFrame.bottom - 18 );
		::LineTo ( localFrame.right - 10, localFrame.bottom - 18 );
		// ROW 19
		::LineTo ( localFrame.right - 11, localFrame.bottom - 19 );
		::LineTo ( localFrame.right - 12, localFrame.bottom - 19 );
		// BACK TO ROW 1
		::LineTo ( localFrame.right - 12, localFrame.bottom - 3 );
		::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	::CloseRgn ( rightSlopeRgn );
		
	// е Button Region
	RgnHandle buttonRgn = ::NewRgn ();
	::InsetRect ( &localFrame, 11, 0 );
	localFrame.bottom -= 3;
	::RectRgn ( buttonRgn, &localFrame );
	
	// е Now we need to union the regions together to make the
	// complete button region
	::UnionRgn ( leftSlopeRgn, buttonRgn, buttonRgn );
	::UnionRgn ( buttonRgn, rightSlopeRgn, buttonRgn );
	
	// е Get rid of the button Slope regions
	::DisposeRgn ( leftSlopeRgn );
	::DisposeRgn ( rightSlopeRgn );
	
	// е Return the tab size which affects the tabs height
	return buttonRgn;
	
}	//	LGATabButton::Calc10PtTabButtonRegion


//-------------------------------------------------------------------------------------
// LGATabButton::Calc12PtTabButtonRegion													
//-------------------------------------------------------------------------------------

RgnHandle
LGATabButton::Calc12PtTabButtonRegion () const
{
	
	// е TEMPORARY
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е Left Slope
	RgnHandle leftSlopeRgn = ::NewRgn ();
	::OpenRgn ();
		// ROW 3 & 4
		::MoveTo ( localFrame.left, localFrame.bottom - 3 );
		::LineTo ( localFrame.left, localFrame.bottom - 4 );
		// ROW 5 - 7
		::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
		::LineTo ( localFrame.left + 1, localFrame.bottom - 7 );
		// ROW 8 - 10
		::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
		::LineTo ( localFrame.left + 2, localFrame.bottom - 10 );
		// ROW 11 - 13
		::LineTo ( localFrame.left + 3, localFrame.bottom - 11 );
		::LineTo ( localFrame.left + 3, localFrame.bottom - 13 );
		// ROW 14 - 16
		::LineTo ( localFrame.left + 4, localFrame.bottom - 14 );
		::LineTo ( localFrame.left + 4, localFrame.bottom - 16 );
		// ROW 17 - 19
		::LineTo ( localFrame.left + 5, localFrame.bottom - 17 );
		::LineTo ( localFrame.left + 5, localFrame.bottom - 19 );
		// ROW 20 & 21
		::LineTo ( localFrame.left + 6, localFrame.bottom - 20 );
		::LineTo ( localFrame.left + 6, localFrame.bottom - 21 );
		// ROW 22
		::LineTo ( localFrame.left + 7, localFrame.bottom - 22 );
		// ROW 23
		::LineTo ( localFrame.left + 8, localFrame.bottom - 23 );
		::LineTo ( localFrame.left + 9, localFrame.bottom - 23 );
		// ROW 24
		::LineTo ( localFrame.left + 10, localFrame.bottom - 24 );
		::LineTo ( localFrame.left + 11, localFrame.bottom - 24 );
		// BACK TO ROW 1
		::LineTo ( localFrame.left + 11, localFrame.bottom - 3 );
		::LineTo ( localFrame.left, localFrame.bottom - 3 );
	::CloseRgn ( leftSlopeRgn );
	
	// е Right Slope
	RgnHandle rightSlopeRgn = ::NewRgn ();
	::OpenRgn ();
		// ROW 3 & 4
		::MoveTo ( localFrame.right - 1, localFrame.bottom - 3 );
		::LineTo ( localFrame.right - 1, localFrame.bottom - 4 );
		// ROW 5 - 7
		::LineTo ( localFrame.right - 2, localFrame.bottom - 5 );
		::LineTo ( localFrame.right - 2, localFrame.bottom - 7 );
		// ROW 8 - 10
		::LineTo ( localFrame.right - 3, localFrame.bottom - 8 );
		::LineTo ( localFrame.right - 3, localFrame.bottom - 10 );
		// ROW 11 - 13
		::LineTo ( localFrame.right - 4, localFrame.bottom - 11 );
		::LineTo ( localFrame.right - 4, localFrame.bottom - 13 );
		// ROW 14 - 16
		::LineTo ( localFrame.right - 5, localFrame.bottom - 14 );
		::LineTo ( localFrame.right - 5, localFrame.bottom - 16 );
		// ROW 17 - 19
		::LineTo ( localFrame.right - 6, localFrame.bottom - 17 );
		::LineTo ( localFrame.right - 6, localFrame.bottom - 19 );
		// ROW 20 & 21
		::LineTo ( localFrame.right - 7, localFrame.bottom - 20 );
		::LineTo ( localFrame.right - 7, localFrame.bottom - 21 );
		// ROW 22
		::LineTo ( localFrame.right - 8, localFrame.bottom - 22 );
		// ROW 23
		::LineTo ( localFrame.right - 9, localFrame.bottom - 23 );
		::LineTo ( localFrame.right - 10, localFrame.bottom - 23 );
		// ROW 24
		::LineTo ( localFrame.right - 11, localFrame.bottom - 24 );
		::LineTo ( localFrame.right - 12, localFrame.bottom - 24 );
		// BACK TO ROW 1
		::LineTo ( localFrame.right - 12, localFrame.bottom - 3 );
		::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	::CloseRgn ( rightSlopeRgn );
		
	// е Button Region
	RgnHandle buttonRgn = ::NewRgn ();
	::InsetRect ( &localFrame, 11, 0 );
	localFrame.bottom -= 3;
	::RectRgn ( buttonRgn, &localFrame );
	
	// е Now we need to union the regions together to make the
	// complete button region
	::UnionRgn ( leftSlopeRgn, buttonRgn, buttonRgn );
	::UnionRgn ( buttonRgn, rightSlopeRgn, buttonRgn );
	
	// е Get rid of the button Slope regions
	::DisposeRgn ( leftSlopeRgn );
	::DisposeRgn ( rightSlopeRgn );
	
	// е Return the tab size which affects the tabs height
	return buttonRgn;
	
}	//	LGATabButton::Calc12PtTabButtonRegion


//-------------------------------------------------------------------------------------
// LGATabButton::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGATabButton::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGATabButton::SetDescriptor


//-------------------------------------------------------------------------------------
// LGATabButton::SetValue
//-------------------------------------------------------------------------------------
//

void
LGATabButton::SetValue	(	SInt32	inValue )
{
	// е Call our superclass to handle the setting of the value
	LControl::SetValue ( inValue );
		
	// е Get everything redrawn
	if ( IsVisible () && FocusDraw ())
		DrawSelf ();
	
}	//	LGATabButton::SetValue


//-------------------------------------------------------------------------------------
// LGATabButton::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGATabButton::SetHiliteState	(	Boolean	inHiliteState )
{

	// е Only change the hilie state if we have to	
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// е Because the state changed we need to make sure that we 
		// get things redrawn so that the change takes effect
		if ( IsVisible () && FocusDraw ())
			DrawSelf ();
	}
			
}	//	LGATabButton::SetHiliteState


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGATabButton::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGATabButton::EnableSelf ()
{

	// е We don't have attachments on tab buttons so we can simply
	// call DrawSelf
	if ( IsVisible () && FocusDraw ())
		DrawSelf ();				

}	//	LGATabButton::EnableSelf


//-------------------------------------------------------------------------------------
// LGATabButton::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGATabButton::DisableSelf ()
{

	// е We don't have attachments on tab buttons so we can simply
	// call DrawSelf
	if ( IsVisible () && FocusDraw ())
		DrawSelf ();
					
}	//	LGATabButton::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGATabButton::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGATabButton::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGATabButton::ActivateSelf



//-------------------------------------------------------------------------------------
// LGATabButton::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGATabButton::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGATabButton::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGATabButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawSelf	()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	// е Get the frame for the control
	Rect	localFrame;
	
	if ( CalcLocalFrameRect ( localFrame ))
	{
		// е Setup a device loop so that we can handle drawing at the correct bit depth
		StDeviceLoop	theLoop ( localFrame );
		SInt16				depth;
		while ( theLoop.NextDepth ( depth )) 
		{
			// е BLACK & WHITE
			if ( depth < 4 )
			{
				// е Get the control drawn in its various states
				if ( IsHilited ())
					DrawTabButtonHilitedBW ();
				else if ( !IsEnabled ())
					DrawTabButtonDimmedBW ();
				else if ( IsSelected ())
					DrawTabButtonOnBW ();
				else
					DrawTabButtonNormalBW ();
			}
			// е COLOR
			else
			{
				// е Get the control drawn in its various states
				if ( IsHilited ())
					DrawTabButtonHilitedColor ();
				else if ( !IsEnabled () || !IsActive ())
					DrawTabButtonDimmedColor ();
				else if ( IsSelected ())
					DrawTabButtonOnColor ();
				else
					DrawTabButtonNormalColor ();
			}	
		}
	}
	
	// е Call our title drawing routine even though it is non-functional in here
	DrawTabButtonTitle ();
		
}	//	LGATabButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonTitle
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonTitle	()
{
		
	StTextState		theTextState;
	
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect		titleRect;
	
	// е Figure out what the justification is from the text trait and 
	// get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Calculate the title rect
	CalcTitleRect ( titleRect );
	
	// е Get the text color from the fore color
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	{
		StDeviceLoop	theLoop ( titleRect );
		SInt16				depth;
		while ( theLoop.NextDepth ( depth )) 
		{
			// е BLACK & WHITE
			if ( depth < 4 )
			{
				textColor = IsHilited () ? UGAColorRamp::GetWhiteColor () : 
																UGAColorRamp::GetBlackColor();

				// е If the control is dimmed then we use the grayishTextOr 
				// transfer mode to draw the text
				if ( !IsEnabled () )
					::TextMode ( grayishTextOr );
					
			}
			// е COLOR
			else 
			{
				// е If control is selected we always draw the text in the title hilite color
				if ( IsHilited ())
					textColor = UGAColorRamp::GetWhiteColor();
				
				// е If the control is dimmed we lighten the text color before drawing
				if ( !IsActive () || !IsEnabled () )
					textColor = UGraphicsUtilities::Lighten ( &textColor );
			}	
		}
	}
	
	// е Get the text color setup			
	::RGBForeColor ( &textColor );
	
	// е Now get the actual title drawn with all the appropriate settings
	Str255 controlTitle;
	GetDescriptor ( controlTitle );
	UStringDrawing::DrawJustifiedString ( 	controlTitle,
														titleRect,
														teCenter );
		
}	//	LGATabButton::DrawTabButtonTitle


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonNormalBW
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonNormalBW	()
{

	StColorPenState	theColorPenState;
	
	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetWhiteColor ();
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е Render the line along the top edge
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
	
	// е PANEL TOP EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the black line along the bottom edge
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::MoveTo ( localFrame.left, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	
}	//	LGATabButton::DrawTabButtonNormalBW


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonOnBW
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonOnBW	()
{

	StColorPenState	theColorPenState;
	
	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetWhiteColor ();
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е Render the line along the top edge
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
		
	// е PANEL TOP EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the black line along the bottom edge
	theColor = UGAColorRamp::GetWhiteColor();
	::RGBForeColor ( &theColor );
	::MoveTo ( localFrame.left, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	
}	//	LGATabButton::DrawTabButtonOnBW


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonHilitedBW
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonHilitedBW	()
{

	StColorPenState	theColorPenState;

	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetBlackColor ();
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е Render the line along the top edge
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
	
	// е PANEL TOP EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the black line along the bottom edge
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::MoveTo ( localFrame.left, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	
}	//	LGATabButton::DrawTabButtonHilitedBW


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonDimmedBW
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonDimmedBW	()
{

	StColorPenState	theColorPenState;
	
	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetWhiteColor ();
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е Render the line along the top edge
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
	
	// е PANEL TOP EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the black line along the bottom edge
	theColor = IsSelected () ? UGAColorRamp::GetWhiteColor() : UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::MoveTo ( localFrame.left, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	
}	//	LGATabButton::DrawTabButtonDimmedBW


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonNormalColor
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonNormalColor	()
{

	StColorPenState	theColorPenState;
	
	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetColor (colorRamp_Gray3);
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е PANEL TOP EDGE
	// е Render the black line along the top edge
	theColor = UGAColorRamp::GetBlackColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
	
	// е Render the dark gray line along the top edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top + 1 );
	::LineTo ( localFrame.right - 1, localFrame.top + 1 );
	
	// е Render the white line along the top edge
	theColor = UGAColorRamp::GetColor (colorRamp_Gray2);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top + 2 );
	::LineTo ( localFrame.right - 1, localFrame.top + 2 );

	// е PANEL BOTTOM EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the black line along the top edge
	theColor = UGAColorRamp::GetBlackColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	
	// е Render the dark gray line along the top edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.bottom - 2 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 2 );
	
	// е Render the white line along the top edge
	theColor = UGAColorRamp::GetWhiteColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.bottom - 1 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 1 );
		
}	//	LGATabButton::DrawTabButtonNormalColor


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonOnColor
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonOnColor	()
{

	StColorPenState	theColorPenState;
	
	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetColor (colorRamp_Gray1);
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е PANEL TOP EDGE
	// е Render the black line along the top edge
	theColor = UGAColorRamp::GetBlackColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
	
	// е Render the dark gray line along the top edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top + 1 );
	::LineTo ( localFrame.right - 1, localFrame.top + 1 );
	
	// е Render the white line along the top edge
	theColor = UGAColorRamp::GetWhiteColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top + 2 );
	::LineTo ( localFrame.right - 1, localFrame.top + 2 );
		
	// е PANEL BOTTOM EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the first line along the bottom edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray1);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left + 3, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 4, localFrame.bottom - 3 );
	
	// е Render the second line along the bottom edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray1);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left + 2, localFrame.bottom - 2 );
	::LineTo ( localFrame.right - 3, localFrame.bottom - 2 );
	
	// е Render the third line along the bottom edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray1);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left + 1, localFrame.bottom - 1 );
	::LineTo ( localFrame.right - 2, localFrame.bottom - 1 );
		
}	//	LGATabButton::DrawTabButtonOnColor


//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonHilitedColor
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonHilitedColor	()
{

	StColorPenState	theColorPenState;
	
	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetColor (colorRamp_Gray9);
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е PANEL TOP EDGE
	// е Render the black line along the top edge
	theColor = UGAColorRamp::GetBlackColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
	
	// е Render the dark gray line along the top edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top + 1 );
	::LineTo ( localFrame.right - 1, localFrame.top + 1 );
	
	// е Render the white line along the top edge
	theColor = UGAColorRamp::GetColor (colorRamp_Gray10);
	::RGBForeColor ( &theColor );
	::MoveTo ( localFrame.left, localFrame.top + 2 );
	::LineTo ( localFrame.right - 1, localFrame.top + 2 );
		
	// е PANEL BOTTOM EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the black line along the top edge
	theColor = UGAColorRamp::GetBlackColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	
	// е Render the dark gray line along the top edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.bottom - 2 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 2 );
	
	// е Render the white line along the top edge
	theColor = UGAColorRamp::GetWhiteColor ();
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.bottom - 1 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 1 );
		
}	//	LGATabButton::DrawTabButtonHilitedColor

//-------------------------------------------------------------------------------------
// LGATabButton::DrawTabButtonDimmedColor
//-------------------------------------------------------------------------------------

void
LGATabButton::DrawTabButtonDimmedColor	()
{

	StColorPenState	theColorPenState;
	
	// е We start by filling the button region with white
	RgnHandle buttonRgn = CalcTabButtonRegion ();
	RGBColor theColor = UGAColorRamp::GetColor (colorRamp_Gray2);
	::RGBForeColor ( &theColor);
	::PaintRgn ( buttonRgn );
	::DisposeRgn ( buttonRgn );
	
	// е Get the appropriate size drawn for the slope
	if ( CalcTabButtonSize () == 10 )
	{
		Draw10PtLeftTabSlope ();
		Draw10PtRightTabSlope ();
	}
	else
	{
		Draw12PtLeftTabSlope ();
		Draw12PtRightTabSlope ();
	}

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	::InsetRect ( &localFrame, tabButton_tabButtonSlope, 0 );
	
	// е PANEL TOP EDGE
	// е Render the dark gray line along the top edge
	theColor = UGAColorRamp::GetColor(colorRamp_Gray10);
	::RGBForeColor ( &theColor);
	::MoveTo ( localFrame.left, localFrame.top );
	::LineTo ( localFrame.right - 1, localFrame.top );
		
	// е PANEL BOTTOM EDGE
	// е Adjust the rect back to full width
	::InsetRect ( &localFrame, -tabButton_tabButtonSlope, 0 );
		
	// е Render the lines along the bottom edge, the first one is drawn in gray10
	// when the button is not selected and the same as the face when selected
	// the others are drawn in the face color to remove any shading that might
	// have been there
	theColor = IsSelected () ? UGAColorRamp::GetColor (colorRamp_Gray2) :
										UGAColorRamp::GetColor (colorRamp_Gray10);
	::RGBForeColor ( &theColor );
	::MoveTo ( localFrame.left, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 3 );
	
	theColor = UGAColorRamp::GetColor (colorRamp_Gray2);
	::RGBForeColor ( &theColor );
	::MoveTo ( localFrame.left, localFrame.bottom - 2 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 2 );
	
	::MoveTo ( localFrame.left, localFrame.bottom - 1 );
	::LineTo ( localFrame.right - 1, localFrame.bottom - 1 );
	
}	//	LGATabButton::DrawTabButtonDimmedColor


//-------------------------------------------------------------------------------------
// LGATabButton::Draw10PtLeftTabSlope
//-------------------------------------------------------------------------------------

void
LGATabButton::Draw10PtLeftTabSlope	()
{

	StColorPenState	thePenState;
	StColorPenState::Normalize ();

	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е OUTSIDE EDGE
		if ( depth < 4 )
		{
			theColor = UGAColorRamp::GetBlackColor ();
			::RGBForeColor ( &theColor );
		}
		else
		{
			theColor = IsActive () && IsEnabled () ? UGAColorRamp::GetBlackColor () :
										UGAColorRamp::GetColor(colorRamp_Gray10);
			::RGBForeColor ( &theColor );
		}
		
		// ROW 3 & 4
		::MoveTo ( localFrame.left, localFrame.bottom - 3 );
		::LineTo ( localFrame.left, localFrame.bottom - 4 );
		// ROW 5 & 6
		::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
		::LineTo ( localFrame.left + 1, localFrame.bottom - 6 );
		// ROW 7 & 8
		::LineTo ( localFrame.left + 2, localFrame.bottom - 7 );
		::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
		// ROW 9 & 10
		::LineTo ( localFrame.left + 3, localFrame.bottom - 9 );
		::LineTo ( localFrame.left + 3, localFrame.bottom - 10 );
		// ROW 11 & 12
		::LineTo ( localFrame.left + 4, localFrame.bottom - 11 );
		::LineTo ( localFrame.left + 4, localFrame.bottom - 12 );
		// ROW 13 & 14
		::LineTo ( localFrame.left + 5, localFrame.bottom - 13 );
		::LineTo ( localFrame.left + 5, localFrame.bottom - 14 );
		// ROW 15 & 16
		::LineTo ( localFrame.left + 6, localFrame.bottom - 15 );
		::LineTo ( localFrame.left + 6, localFrame.bottom - 16 );
		// ROW 17
		::LineTo ( localFrame.left + 7, localFrame.bottom - 17 );
		// ROW 18
		::LineTo ( localFrame.left + 8, localFrame.bottom - 18 );
		::LineTo ( localFrame.left + 9, localFrame.bottom - 18 );
		// ROW 19
		::LineTo ( localFrame.left + 10, localFrame.bottom - 19 );
		::LineTo ( localFrame.left + 11, localFrame.bottom - 19 );
			
		// е COLOR DETAILS
		if ( depth >= 4 )
		{
			// е SHADOW EDGE
			if ( IsActive () && IsEnabled ())
			{
				if ( IsSelected () || IsHilited ())
				{
					// е Setup the correct color for the state we are drawing
					if ( IsSelected ())
					{
						theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
						::RGBForeColor ( &theColor );
					}
					else if ( IsHilited ())
					{
						theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
						::RGBForeColor ( &theColor );
					}
						
					// е If the button is selected we need to draw a few additional
					// pixels to meet up with the panels edges
					if ( IsSelected ())
					{
						// е ROW 2 & 3
						::MoveTo ( localFrame.left, localFrame.bottom - 2 );
						::LineTo ( localFrame.left, localFrame.bottom - 3 );
					}
					
					// е Inset the rect horizontally
					::InsetRect ( &localFrame, 1, 0 );
					
					// ROW 3 & 4
					::MoveTo ( localFrame.left, localFrame.bottom - 3 );
					::LineTo ( localFrame.left, localFrame.bottom - 4 );
					// ROW 5 & 6
					::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
					::LineTo ( localFrame.left + 1, localFrame.bottom - 6 );
					// ROW 7 & 8
					::LineTo ( localFrame.left + 2, localFrame.bottom - 7 );
					::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
					// ROW 9 & 10
					::LineTo ( localFrame.left + 3, localFrame.bottom - 9 );
					::LineTo ( localFrame.left + 3, localFrame.bottom - 10 );
					// ROW 11 & 12
					::LineTo ( localFrame.left + 4, localFrame.bottom - 11 );
					::LineTo ( localFrame.left + 4, localFrame.bottom - 12 );
					// ROW 13 & 14
					::LineTo ( localFrame.left + 5, localFrame.bottom - 13 );
					::LineTo ( localFrame.left + 5, localFrame.bottom - 14 );
					// ROW 15 & 16
					::LineTo ( localFrame.left + 6, localFrame.bottom - 15 );
					::LineTo ( localFrame.left + 6, localFrame.bottom - 16 );
					// ROW 17
					::LineTo ( localFrame.left + 7, localFrame.bottom - 17 );
					::LineTo ( localFrame.left + 8, localFrame.bottom - 17 );
					// ROW 18
					::LineTo ( localFrame.left + 9, localFrame.bottom - 18 );
					::LineTo ( localFrame.left + 10, localFrame.bottom - 18 );

					// е Inset the rect horizontally
					::InsetRect ( &localFrame, -1, 0 );
				}
				
				// е LIGHT EDGE
				// е Setup the correct color for the state we are drawing
				if ( IsSelected ())
				{
					theColor = UGAColorRamp::GetWhiteColor();
					::RGBForeColor ( &theColor );
				}
				else if ( IsHilited ())
				{
					theColor = UGAColorRamp::GetColor(colorRamp_Gray10);
					::RGBForeColor ( &theColor );
				}
				else
				{
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					::RGBForeColor ( &theColor );
				}
					
				// е If the button is selected we need to draw a few additional
				// pixels to meet up with the panels edges
				if ( IsSelected ())
				{
					// е ROW 1 - 3
					::MoveTo ( localFrame.left, localFrame.bottom - 1 );
					::LineTo ( localFrame.left + 2, localFrame.bottom - 3 );
				}
				
				// е Inset the rect horizontally
				::InsetRect ( &localFrame, 2, 0 );
				
				// ROW 3 & 4
				::MoveTo ( localFrame.left, localFrame.bottom - 3 );
				::LineTo ( localFrame.left, localFrame.bottom - 4 );
				// ROW 5 & 6
				::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
				::LineTo ( localFrame.left + 1, localFrame.bottom - 6 );
				// ROW 7 & 8
				::LineTo ( localFrame.left + 2, localFrame.bottom - 7 );
				::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
				// ROW 9 & 10
				::LineTo ( localFrame.left + 3, localFrame.bottom - 9 );
				::LineTo ( localFrame.left + 3, localFrame.bottom - 10 );
				// ROW 11 & 12
				::LineTo ( localFrame.left + 4, localFrame.bottom - 11 );
				::LineTo ( localFrame.left + 4, localFrame.bottom - 12 );
				// ROW 13 & 14
				::LineTo ( localFrame.left + 5, localFrame.bottom - 13 );
				::LineTo ( localFrame.left + 5, localFrame.bottom - 14 );
				// ROW 15 & 16
				::LineTo ( localFrame.left + 6, localFrame.bottom - 15 );
				::LineTo ( localFrame.left + 6, localFrame.bottom - 16 );
				::LineTo ( localFrame.left + 7, localFrame.bottom - 16 );
				// ROW 17
				::LineTo ( localFrame.left + 8, localFrame.bottom - 17 );
				::LineTo ( localFrame.left + 9, localFrame.bottom - 17 );
			}
		}	
	}

}	//	LGATabButton::Draw10PtLeftTabSlope


//-------------------------------------------------------------------------------------
// LGATabButton::Draw10PtRightTabSlope
//-------------------------------------------------------------------------------------

void
LGATabButton::Draw10PtRightTabSlope	()
{

	StColorPenState	thePenState;
	StColorPenState::Normalize ();

	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е OUTSIDE EDGE
		if ( depth < 4 )
		{
			theColor = UGAColorRamp::GetBlackColor ();
			::RGBForeColor ( &theColor );
		}
		else
		{
			theColor = IsActive () && IsEnabled () ? UGAColorRamp::GetBlackColor () :
										UGAColorRamp::GetColor(colorRamp_Gray10);
			::RGBForeColor ( &theColor );
		}
		
		// ROW 3 & 4
		::MoveTo ( localFrame.right - 1, localFrame.bottom - 3 );
		::LineTo ( localFrame.right - 1, localFrame.bottom - 4 );
		// ROW 5 & 6
		::LineTo ( localFrame.right - 2, localFrame.bottom - 5 );
		::LineTo ( localFrame.right - 2, localFrame.bottom - 6 );
		// ROW 7 & 8
		::LineTo ( localFrame.right - 3, localFrame.bottom - 7 );
		::LineTo ( localFrame.right - 3, localFrame.bottom - 8 );
		// ROW 9 & 10
		::LineTo ( localFrame.right - 4, localFrame.bottom - 9 );
		::LineTo ( localFrame.right - 4, localFrame.bottom - 10 );
		// ROW 11 & 12
		::LineTo ( localFrame.right - 5, localFrame.bottom - 11 );
		::LineTo ( localFrame.right - 5, localFrame.bottom - 12 );
		// ROW 13 & 14
		::LineTo ( localFrame.right - 6, localFrame.bottom - 13 );
		::LineTo ( localFrame.right - 6, localFrame.bottom - 14 );
		// ROW 15 & 16
		::LineTo ( localFrame.right - 7, localFrame.bottom - 15 );
		::LineTo ( localFrame.right - 7, localFrame.bottom - 16 );
		// ROW 17
		::LineTo ( localFrame.right - 8, localFrame.bottom - 17 );
		// ROW 18
		::LineTo ( localFrame.right - 9, localFrame.bottom - 18 );
		::LineTo ( localFrame.right - 10, localFrame.bottom - 18 );
		// ROW 19
		::LineTo ( localFrame.right - 11, localFrame.bottom - 19 );
		::LineTo ( localFrame.right - 12, localFrame.bottom - 19 );
		
		// е COLOR DETAILS
		if ( depth >= 4 )
		{
			// е SHADOWs
			if ( IsActive () && IsEnabled ())
			{
				if ( IsSelected ())
				{
					// е ROW 1
					theColor = UGAColorRamp::GetColor(colorRamp_Gray1);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 1,
															localFrame.bottom - 1,
															&theColor );
					// е ROW 2
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 1,
															localFrame.bottom - 2,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 2,
															&theColor );
					// е ROW 3
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 1,
															localFrame.bottom - 3,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 3,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 3,
															&theColor );
					// е ROW 4
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 4,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 4,
															&theColor );
					// е ROW 5
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 5,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 5,
															&theColor );
					// е ROW 6
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 6,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 6,
															&theColor );
					// е ROW 7
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 7,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 7,
															&theColor );
					// е ROW 8
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 8,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 8,
															&theColor );
					// е ROW 9
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 9,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 9,
															&theColor );
					// е ROW 10
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 10,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 10,
															&theColor );
					// е ROW 11
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 11,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 11,
															&theColor );
					// е ROW 12
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 12,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 12,
															&theColor );
					// е ROW 13
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 13,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 13,
															&theColor );
					// е ROW 14
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 14,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 14,
															&theColor );
					// е ROW 15
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 15,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 15,
															&theColor );
					// е ROW 16
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 16,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 16,
															&theColor );
					// е ROW 17
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 17,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 10,
															localFrame.bottom - 17,
															&theColor );
					theColor = UGAColorRamp::GetWhiteColor();
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 17,
															&theColor );
					// е ROW 18
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 18,
															&theColor );
				}
				else if ( IsHilited ())
				{
					// е ROW 4
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 4,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 4,
															&theColor );
					// е ROW 5
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 5,
															&theColor );
					// е ROW 6
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 6,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 6,
															&theColor );
					// е ROW 7
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 7,
															&theColor );
					// е ROW 8
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 8,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 8,
															&theColor );
					// е ROW 9
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 9,
															&theColor );
					// е ROW 10
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 10,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 10,
															&theColor );
					// е ROW 11
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 11,
															&theColor );
					// е ROW 12
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 12,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 12,
															&theColor );
					// е ROW 13
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 13,
															&theColor );
					// е ROW 14
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 14,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 14,
															&theColor );
					// е ROW 15
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 15,
															&theColor );
					// е ROW 16
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 16,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 16,
															&theColor );
					// е ROW 17
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 17,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 10,
															localFrame.bottom - 17,
															&theColor );
					
					theColor = UGAColorRamp::GetColor(colorRamp_Gray10);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 17,
															&theColor );
					// е ROW 18
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 18,
															&theColor );
				}
				else
				{
					// е ROW 4
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 4,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 4,
															&theColor );
					// е ROW 5
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 5,
															&theColor );
					// е ROW 6
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 6,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 6,
															&theColor );
					// е ROW 7
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 7,
															&theColor );
					// е ROW 8
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 8,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 8,
															&theColor );
					// е ROW 9
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 9,
															&theColor );
					// е ROW 10
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 10,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 10,
															&theColor );
					// е ROW 11
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 11,
															&theColor );
					// е ROW 12
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 12,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 12,
															&theColor );
					// е ROW 13
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 13,
															&theColor );
					// е ROW 14
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 14,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 14,
															&theColor );
					// е ROW 15
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 15,
															&theColor );
					// е ROW 16
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 16,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 16,
															&theColor );
					// е ROW 17
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 17,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 10,
															localFrame.bottom - 17,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 17,
															&theColor );
					// е ROW 18
					theColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 18,
															&theColor );
				}
			}
		}	
	}
		
}	//	LGATabButton::Draw10PtRightTabSlope


//-------------------------------------------------------------------------------------
// LGATabButton::Draw12PtLeftTabSlope
//-------------------------------------------------------------------------------------

void
LGATabButton::Draw12PtLeftTabSlope	()
{

	StColorPenState	thePenState;
	StColorPenState::Normalize ();

	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е OUTSIDE EDGE
		if ( depth < 4 )
		{
			theColor = UGAColorRamp::GetBlackColor ();
			::RGBForeColor ( &theColor );
		}
		else
		{
			theColor = IsActive () && IsEnabled () ? UGAColorRamp::GetBlackColor () :
										UGAColorRamp::GetColor(colorRamp_Gray10);
			::RGBForeColor ( &theColor );
		}
		
		// ROW 3 & 4
		::MoveTo ( localFrame.left, localFrame.bottom - 3 );
		::LineTo ( localFrame.left, localFrame.bottom - 4 );
		// ROW 5 - 7
		::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
		::LineTo ( localFrame.left + 1, localFrame.bottom - 7 );
		// ROW 8 - 10
		::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
		::LineTo ( localFrame.left + 2, localFrame.bottom - 10 );
		// ROW 11 - 13
		::LineTo ( localFrame.left + 3, localFrame.bottom - 11 );
		::LineTo ( localFrame.left + 3, localFrame.bottom - 13 );
		// ROW 14 - 16
		::LineTo ( localFrame.left + 4, localFrame.bottom - 14 );
		::LineTo ( localFrame.left + 4, localFrame.bottom - 16 );
		// ROW 17 - 19
		::LineTo ( localFrame.left + 5, localFrame.bottom - 17 );
		::LineTo ( localFrame.left + 5, localFrame.bottom - 19 );
		// ROW 20 & 21
		::LineTo ( localFrame.left + 6, localFrame.bottom - 20 );
		::LineTo ( localFrame.left + 6, localFrame.bottom - 21 );
		// ROW 22
		::LineTo ( localFrame.left + 7, localFrame.bottom - 22 );
		// ROW 23
		::LineTo ( localFrame.left + 8, localFrame.bottom - 23 );
		::LineTo ( localFrame.left + 9, localFrame.bottom - 23 );
		// ROW 24
		::LineTo ( localFrame.left + 10, localFrame.bottom - 24 );
		::LineTo ( localFrame.left + 11, localFrame.bottom - 24 );
			
		// е COLOR DETAILS
		if ( depth >= 4 )
		{
			// е SHADOW EDGE
			if ( IsActive () && IsEnabled ())
			{
				if ( IsSelected () || IsHilited ())
				{
					// е Setup the correct color for the state we are drawing
					if ( IsSelected ())
					{
						theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
						::RGBForeColor ( &theColor );
					}
					else if ( IsHilited ())
					{
						theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
						::RGBForeColor ( &theColor );
					}
						
					// е If the button is selected we need to draw a few additional
					// pixels to meet up with the panels edges
					if ( IsSelected ())
					{
						// е ROW 2 & 3
						::MoveTo ( localFrame.left, localFrame.bottom - 2 );
						::LineTo ( localFrame.left + 1, localFrame.bottom - 3 );
					}
					
					// е Inset the rect horizontally
					::InsetRect ( &localFrame, 1, 0 );
					
					// ROW 3 & 4
					::MoveTo ( localFrame.left, localFrame.bottom - 3 );
					::LineTo ( localFrame.left, localFrame.bottom - 4 );
					// ROW 5 - 7
					::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
					::LineTo ( localFrame.left + 1, localFrame.bottom - 7 );
					// ROW 8 - 10
					::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
					::LineTo ( localFrame.left + 2, localFrame.bottom - 10 );
					// ROW 11 - 13
					::LineTo ( localFrame.left + 3, localFrame.bottom - 11 );
					::LineTo ( localFrame.left + 3, localFrame.bottom - 13 );
					// ROW 14 - 16
					::LineTo ( localFrame.left + 4, localFrame.bottom - 14 );
					::LineTo ( localFrame.left + 4, localFrame.bottom - 16 );
					// ROW 17 - 19
					::LineTo ( localFrame.left + 5, localFrame.bottom - 17 );
					::LineTo ( localFrame.left + 5, localFrame.bottom - 19 );
					// ROW 20 & 21
					::LineTo ( localFrame.left + 6, localFrame.bottom - 20 );
					::LineTo ( localFrame.left + 6, localFrame.bottom - 21 );
					::LineTo ( localFrame.left + 7, localFrame.bottom - 21 );
					// ROW 22
					::LineTo ( localFrame.left + 7, localFrame.bottom - 22 );
					::LineTo ( localFrame.left + 8, localFrame.bottom - 22 );
					// ROW 23
					::LineTo ( localFrame.left + 9, localFrame.bottom - 23 );
					::LineTo ( localFrame.left + 10, localFrame.bottom - 23 );

					// е Inset the rect horizontally
					::InsetRect ( &localFrame, -1, 0 );
				}
				
				// е LIGHT EDGE
				// е Setup the correct color for the state we are drawing
				if ( IsSelected ())
				{
					theColor = UGAColorRamp::GetWhiteColor();
					::RGBForeColor ( &theColor );
				}
				else if ( IsHilited ())
				{
					theColor = UGAColorRamp::GetColor(colorRamp_Gray10);
					::RGBForeColor ( &theColor );
				}
				else
				{
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					::RGBForeColor ( &theColor );
				}
					
				// е If the button is selected we need to draw a few additional
				// pixels to meet up with the panels edges
				if ( IsSelected ())
				{
					// е ROW 1 - 3
					::MoveTo ( localFrame.left, localFrame.bottom - 1 );
					::LineTo ( localFrame.left + 2, localFrame.bottom - 3 );
				}
				
				// е Inset the rect horizontally
				::InsetRect ( &localFrame, 2, 0 );
				
				// ROW 3 & 4
				::MoveTo ( localFrame.left, localFrame.bottom - 3 );
				::LineTo ( localFrame.left, localFrame.bottom - 4 );
				// ROW 5 - 7
				::LineTo ( localFrame.left + 1, localFrame.bottom - 5 );
				::LineTo ( localFrame.left + 1, localFrame.bottom - 7 );
				// ROW 8 - 10
				::LineTo ( localFrame.left + 2, localFrame.bottom - 8 );
				::LineTo ( localFrame.left + 2, localFrame.bottom - 10 );
				// ROW 11 - 13
				::LineTo ( localFrame.left + 3, localFrame.bottom - 11 );
				::LineTo ( localFrame.left + 3, localFrame.bottom - 13 );
				// ROW 14 - 16
				::LineTo ( localFrame.left + 4, localFrame.bottom - 14 );
				::LineTo ( localFrame.left + 4, localFrame.bottom - 16 );
				// ROW 17 - 19
				::LineTo ( localFrame.left + 5, localFrame.bottom - 17 );
				::LineTo ( localFrame.left + 5, localFrame.bottom - 19 );
				// ROW 20
				::LineTo ( localFrame.left + 6, localFrame.bottom - 20 );
				// ROW 21
				::LineTo ( localFrame.left + 7, localFrame.bottom - 21 );
				// ROW 22 & 23
				::LineTo ( localFrame.left + 8, localFrame.bottom - 22 );
				::LineTo ( localFrame.left + 9, localFrame.bottom - 22 );
			}
		}	
	}

}	//	LGATabButton::Draw12PtLeftTabSlope


//-------------------------------------------------------------------------------------
// LGATabButton::Draw12PtRightTabSlope
//-------------------------------------------------------------------------------------

void
LGATabButton::Draw12PtRightTabSlope	()
{

	StColorPenState	thePenState;
	StColorPenState::Normalize ();

	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е OUTSIDE EDGE
		if ( depth < 4 )
		{
			theColor = UGAColorRamp::GetBlackColor ();
			::RGBForeColor ( &theColor );
		}
		else
		{
			theColor = IsActive () && IsEnabled () ? UGAColorRamp::GetBlackColor () :
										UGAColorRamp::GetColor(colorRamp_Gray10);
			::RGBForeColor ( &theColor );
		}
		
		// ROW 3 & 4
		::MoveTo ( localFrame.right - 1, localFrame.bottom - 3 );
		::LineTo ( localFrame.right - 1, localFrame.bottom - 4 );
		// ROW 5 - 7
		::LineTo ( localFrame.right - 2, localFrame.bottom - 5 );
		::LineTo ( localFrame.right - 2, localFrame.bottom - 7 );
		// ROW 8 - 10
		::LineTo ( localFrame.right - 3, localFrame.bottom - 8 );
		::LineTo ( localFrame.right - 3, localFrame.bottom - 10 );
		// ROW 11 - 13
		::LineTo ( localFrame.right - 4, localFrame.bottom - 11 );
		::LineTo ( localFrame.right - 4, localFrame.bottom - 13 );
		// ROW 14 - 16
		::LineTo ( localFrame.right - 5, localFrame.bottom - 14 );
		::LineTo ( localFrame.right - 5, localFrame.bottom - 16 );
		// ROW 17 - 19
		::LineTo ( localFrame.right - 6, localFrame.bottom - 17 );
		::LineTo ( localFrame.right - 6, localFrame.bottom - 19 );
		// ROW 20 & 21
		::LineTo ( localFrame.right - 7, localFrame.bottom - 20 );
		::LineTo ( localFrame.right - 7, localFrame.bottom - 21 );
		// ROW 22
		::LineTo ( localFrame.right - 8, localFrame.bottom - 22 );
		// ROW 23
		::LineTo ( localFrame.right - 9, localFrame.bottom - 23 );
		::LineTo ( localFrame.right - 10, localFrame.bottom - 23 );
		// ROW 24
		::LineTo ( localFrame.right - 11, localFrame.bottom - 24 );
		::LineTo ( localFrame.right - 12, localFrame.bottom - 24 );
		
		// е COLOR DETAILS
		if ( depth >= 4 )
		{
			// е SHADOWs
			if ( IsActive () && IsEnabled ())
			{
				if ( IsSelected ())
				{
					// е ROW 1
					theColor = UGAColorRamp::GetWhiteColor();
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 1,
															localFrame.bottom - 1,
															&theColor );
					// е ROW 2
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 1,
															localFrame.bottom - 2,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 2,
															&theColor );
					// е ROW 3
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 3,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray1);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 3,
															&theColor );
					// е ROW 4
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 4,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 4,
															&theColor );
					// е ROW 5
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 5,
															&theColor );
					// е ROW 6
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 6,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 6,
															&theColor );
					// е ROW 7
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 7,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 7,
															&theColor );
					// е ROW 8
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 8,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 8,
															&theColor );
					// е ROW 9
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 9,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 9,
															&theColor );
					// е ROW 10
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 10,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 10,
															&theColor );
					// е ROW 11
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 11,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 11,
															&theColor );
					// е ROW 12
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 12,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 12,
															&theColor );
					// е ROW 13
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 13,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 13,
															&theColor );
					// е ROW 14
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 14,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 14,
															&theColor );
					// е ROW 15
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 15,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 15,
															&theColor );
					// е ROW 16
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 16,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 16,
															&theColor );
					// е ROW 17
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 17,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 17,
															&theColor );
					// е ROW 18
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 18,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 18,
															&theColor );
					// е ROW 19
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 19,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 19,
															&theColor );
					// е ROW 20
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 20,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 20,
															&theColor );
					// е ROW 21
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 21,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 21,
															&theColor );
					// е ROW 22
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 22,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 10,
															localFrame.bottom - 22,
															&theColor );
					theColor = UGAColorRamp::GetWhiteColor();
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 22,
															&theColor );
					// е ROW 23
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 23,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 12,
															localFrame.bottom - 23,
															&theColor );
				}
				else if ( IsHilited ())
				{
					// е ROW 4
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 4,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 4,
															&theColor );
					// е ROW 5
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 5,
															&theColor );
					// е ROW 6
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 6,
															&theColor );
					// е ROW 7
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 7,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 7,
															&theColor );
					// е ROW 8
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 8,
															&theColor );
					// е ROW 9
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 9,
															&theColor );
					// е ROW 10
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 10,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 10,
															&theColor );
					// е ROW 11
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 11,
															&theColor );
					// е ROW 12
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 12,
															&theColor );
					// е ROW 13
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 13,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 13,
															&theColor );
					// е ROW 14
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 14,
															&theColor );
					// е ROW 15
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 15,
															&theColor );
					// е ROW 16
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 16,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 16,
															&theColor );
					// е ROW 17
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 17,
															&theColor );
					// е ROW 18
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 18,
															&theColor );
					// е ROW 19
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 19,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 19,
															&theColor );
					// е ROW 20
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 20,
															&theColor );
					// е ROW 21
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 21,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 21,
															&theColor );
					// е ROW 22
					theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 22,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 10,
															localFrame.bottom - 22,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray10);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 22,
															&theColor );
					// е ROW 23
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 23,
															&theColor );
				}
				else
				{
					// е ROW 4
					theColor = UGAColorRamp::GetColor(colorRamp_Gray9);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 2,
															localFrame.bottom - 4,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 4,
															&theColor );
					// е ROW 5
					theColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 5,
															&theColor );
					// е ROW 6
					theColor = UGAColorRamp::GetColor(colorRamp_Gray6);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 6,
															&theColor );
					// е ROW 7
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 3,
															localFrame.bottom - 7,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 7,
															&theColor );
					// е ROW 8
					theColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 8,
															&theColor );
					// е ROW 9
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 9,
															&theColor );
					// е ROW 10
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 4,
															localFrame.bottom - 10,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 10,
															&theColor );
					// е ROW 11
					theColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 11,
															&theColor );
					// е ROW 12
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 12,
															&theColor );
					// е ROW 13
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 5,
															localFrame.bottom - 13,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 13,
															&theColor );
					// е ROW 14
					theColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 14,
															&theColor );
					// е ROW 15
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 15,
															&theColor );
					// е ROW 16
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 6,
															localFrame.bottom - 16,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 16,
															&theColor );
					// е ROW 17
					theColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 17,
															&theColor );
					// е ROW 18
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 18,
															&theColor );
					// е ROW 19
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 7,
															localFrame.bottom - 19,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 19,
															&theColor );
					// е ROW 20
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 20,
															&theColor );
					// е ROW 21
					theColor = UGAColorRamp::GetColor(colorRamp_Gray11);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 8,
															localFrame.bottom - 21,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 21,
															&theColor );
					// е ROW 22
					theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 9,
															localFrame.bottom - 22,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 10,
															localFrame.bottom - 22,
															&theColor );
					theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 22,
															&theColor );
					// е ROW 23
					theColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel 
														( 	localFrame.right - 11,
															localFrame.bottom - 23,
															&theColor );
				}
			}
		}	
	}
		
}	//	LGATabButton::Draw12PtRightTabSlope


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGATabButton::PointIsInFrame
//-------------------------------------------------------------------------------------

Boolean
LGATabButton::PointIsInFrame ( 	SInt32	inHorizPort,
											SInt32	inVertPort ) const
{
	// е If the option to handle the click only in the region occupied
	// by the icon is on then handle that first otherwise we just go
	// to the standard version of this method which handles a click in
	// the pane's frame
	RgnHandle	tabButtonRgn = CalcTabButtonRegion ();
	Point			portPt = { inVertPort, inHorizPort };
	PortToLocalPoint ( portPt );
	Boolean		inTabButton = ::PtInRgn ( portPt, tabButtonRgn );
	::DisposeRgn ( tabButtonRgn );
	
	return inTabButton;
	
}	//	LGATabButton::PointIsInFrame


//-------------------------------------------------------------------------------------
// LGATabButton::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGATabButton::HotSpotAction	(	SInt16		/* inHotSpot */,
											Boolean	inCurrInside,
											Boolean	inPrevInside )
{

	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control which is slightly different for
	// each of the modes supported by the control
	if ( inCurrInside != inPrevInside )
	{
		// е Change the hilite state
		SetHiliteState ( inCurrInside && !IsSelected () );
	}
		
}	//	LGATabButton::HotSpotAction


//-------------------------------------------------------------------------------------
// LGATabButton::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGATabButton::HotSpotResult	(	SInt16 /* inHotSpot */ )
{
	
	
	// е Make sure that by default we turn the hiliting off, all we are going
	// to do is turn the actual flag off as we don't want the button to be 
	// redrawn in its normal state before it gets drawn in the On state, by
	// doing this it will just go from the hilited to the On state.  We can do
	// this because we know we are going to be redrawn when the value gets set.
	mHilited = false;
	
	// е Do the right thing about getting the value set after the tab has been
	// clicked in its hotspot, NOTE: this method assumes the entire control is
	// the hotspot
	if ( !IsSelected ())
	{
		SetValue ( Button_On );
	}
	
}	//	LGATabButton::HotSpotResult


