// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAIconTextButton.cp

CLASSES:				LGAIconTextButton

DESCRIPTION:		Implementation file for Object

AUTHOR:				Robin Mair

CREATION DATE :	96.09.17

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.27		rtm	Made some changes that allow this class to still work even if
								there is no icon [no icon is specified by passing in 0 or -1
								for the icon resource ID.  An override of ShowSelf was added
								in order to handle some tweaking that is used to correctly set
								up the drawing of the title when a button is created that is
								set to be initially selected.
		97.01.26		rtm	Modified the title rect calculation method to allow a litte more
								space between the title and the icon when the icon is using the
								full space of its bitmap.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		96.12.21		rtm	Fixed a small bug in the AdjustTransform method that was not
								correctly accounting for the state of the control when it was
								embedded in a disabled view.
		96.11.15		rtm	Changed the way the title for the button is rendered when the
								button is on but not being pressed, in that case the text is
								drawn in black, so the only time the text is drawn in white is
								when the button is pushed.
		96.10.22		rtm	Fixed a reported bug in this class where text was being drawn
								cropped. The problem was in CalcTitleRect where a call to
								SetPortTextTraits was missing, before getting the font info,
								this resulted in the wrong info being used for the font. Also
								some of title rect calculations were a little off and needed
								some tweaking.  Also added an override to SetTextTraitsID so
								that we can do a refresh after the ID has been set.
		96.10.19		rtm	Fixed the drawing of title across multiple devices for the
								hilited case primarily as there was a situation where the
								title would not get drawn.  In the process cleaned up a lot
								of the drawing code for titles in B&W.
		96.10.19		rtm	Optimized the CalcTitleRect method so that the icon width
								was only being calculated once. Also removed the call to
								CalcIconLocation from the stream constructor as this will 
								be handled from FinishCreateSelf for LGAIconButton.
		96.10.12		rtm	Had to tweak the drawing of titles across multiple devices
								disabled.  Using the grayishTextOr text mode transfer was
								resulting in some weirdness that could only be solved by
								doing something similar to what grayishTextOr did, which was
								to basically lighten the text color.
		96.10.12		rtm	Fixed a bug where the title was not being drawn when the
								button was pressed on a B&W monitor. Also fixed a problem
								where the icon was not being drawn in the correct inverted
								state when pressed.
		96.10.11		rtm	Fixed a number of bugs in the title rect calculation method
								where the title was not being placed correctly in relationship
								to the icon.
		96.10.09		rtm	Changed the title drawing so that it now will respect a color
								provided in a text trait as  opposed to always drawing the title
								in black.
		96.10.08		rtm	Replaced the override of DrawIconButton with n override of
								DrawSelf as DrawIconButton no longer exists.
		96.10.07		rtm	Fixed the CalcTitleRect function so that it now correctly allows
								for the mini icons rectangular size.
		96.09.17		rtm	Initial creation of file.

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAIconTextButton.h>
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include	<UTextTraits.h>
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

#pragma mark === CLASS: LGAIconTextButton

//=====================================================================================
// CLASS:	LGAIconTextButton
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAIconTextButton::LGAIconTextButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAIconTextButton::LGAIconTextButton ()
{	
}	//	LGAIconTextButton::LGAIconTextButton


//-------------------------------------------------------------------------------------
// LGAIconTextButton::LGAIconTextButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAIconTextButton::LGAIconTextButton (	const LGAIconTextButton &inOriginal )
								:	LGAIconButton ( inOriginal ),
									LGATitleMixin ( inOriginal )
{	
}	//	LGAIconTextButton::LGAIconTextButton


//-------------------------------------------------------------------------------------
// LGAIconTextButton::LGAIconTextButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAIconTextButton::LGAIconTextButton (	LStream	*inStream ) : LGAIconButton ( inStream )
{

	// е Read in the title string
	inStream->ReadPString ( mTitle );
	
	//	е Read in the text traits ID
	*inStream >> mTextTraitsID;

	// е By default we always draw the title in white when hilited
	mTitleHiliteColor = UGAColorRamp::GetWhiteColor();
	
	// е If the control mode is some form of button then we need to make
	// sure that the max value gets set to at least one
	if ( mControlMode > 0 && GetMaxValue () < 1 )
		mMaxValue = 1;

	// е By default we always hilite the title
	mHiliteTitle = true;
	
}	//	LGAIconTextButton::LGAIconTextButton


//-------------------------------------------------------------------------------------
// LGAIconTextButton::LGAIconTextButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAIconTextButton::LGAIconTextButton (	const SPaneInfo			&inPaneInfo,
													const SControlInfo		&inControlInfo,
													const SIconButtonInfo	&inIconButtonInfo,
													Str255						inTitle,
													ResIDT						inTextTraitsID )
											: LGAIconButton ( inPaneInfo,
																	inControlInfo,
																	inIconButtonInfo )
{

	// е Setup our fields from the input parameters
	mTitle = inTitle;
	mTextTraitsID = inTextTraitsID;
			
	// е By default we always draw the title in white when hilited
	mTitleHiliteColor = UGAColorRamp::GetWhiteColor();
	
	// е If the control mode is some form of button then we need to make
	// sure that the max value gets set to at least one
	if ( mControlMode > 0 && GetMaxValue () < 1 )
		mMaxValue = 1;

	// е By default we always hilite the title
	mHiliteTitle = true;
	
}	//	LGAIconTextButton::LGAIconTextButton


//-------------------------------------------------------------------------------------
// LGAIconTextButton::~LGAIconTextButton				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconTextButton::~LGAIconTextButton ()
{
}	//	LGAIconTextButton::~LGAIconTextButton


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAIconTextButton::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGAIconTextButton::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGAIconTextButton::GetDescriptor


//-------------------------------------------------------------------------------------
// LGAIconTextButton::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGAIconTextButton::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGAIconTextButton::SetDescriptor


//-------------------------------------------------------------------------------------
// LGAIconTextButton::SetTextTraitsID
//-------------------------------------------------------------------------------------
//

void			
LGAIconTextButton::SetTextTraitsID	( ResIDT inTextTraitID )
{
	//  е only change the ID if it is different
	if ( GetTextTraitsID () != inTextTraitID )
	{
		mTextTraitsID = inTextTraitID;
		Refresh ();
	}
		
}	//	LGAIconTextButton::SetTextTraitsID


//-------------------------------------------------------------------------------------
// LGAIconTextButton::CalcTitleRect
//-------------------------------------------------------------------------------------

void
LGAIconTextButton::CalcTitleRect ( Rect	&outRect )
{

	StTextState			theTextState;
	StColorPenState	thePenState;

	// е Get the text traits ID for the control
	ResIDT	textTID = GetTextTraitsID ();

	// е Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Figure out the height of the text for the selected font
	FontInfo fi;
	GetFontInfo ( &fi );
	SInt16		textHeight = fi.ascent + fi.descent;
		
	// е Make sure that the edge offset has been correctly setup
	if ( mEdgeOffset < GetBevelWidth () )
		mEdgeOffset = GetBevelWidth () + 2;
		
	// е Get the local inset frame rectangle
	CalcLocalFrameRect ( outRect );
	::InsetRect ( &outRect, mEdgeOffset + 3, mEdgeOffset );
	
	// е Calculate the icon width based on the size selector
	SInt16		iconHeight = mSizeSelector == 12 ? 16 : mSizeSelector;
	SInt16		iconWidth = mSizeSelector == 12 ? 16 : mSizeSelector;
	
	// е If there is no icon handle [we support this] then we set the width
	// to 0 so that the title will still get drawn correctly within the
	// button
	if ( GetIconSuiteH () == nil )
		iconWidth = iconHeight = 0;
		
	// е Calculate the mid point based on the font ascent
	SInt16		midHeight = (((outRect.bottom - outRect.top) - fi.ascent) / 2);
	
	// е Get the local icon rect which we want to use for positioning the title
	// in some of the cases
	Rect	iconRect;
	CalcLocalIconRect ( iconRect );
	
	// е Now we modify the title rect based on the icon placement
	switch ( mIconPlacement )
	{
		case iconPosition_None:
		break;
		
		case iconPosition_TopLeft:
			outRect.bottom = outRect.top + textHeight;
			outRect.left += iconWidth;
		break;
		
		case iconPosition_TopCenter:
			if ( iconHeight > 0 )
				outRect.top = iconRect.bottom;
			outRect.bottom = outRect.top + textHeight;
		break;
		
		case iconPosition_TopRight:
			outRect.bottom = outRect.top + textHeight;
			outRect.right -= iconWidth;
		break;
		
		case iconPosition_RightCenter:
			outRect.top += midHeight - 1;
			outRect.bottom = outRect.top + textHeight;
			outRect.right -= iconWidth;
		break;
		
		case iconPosition_BottomRight:
			outRect.top += (outRect.bottom - outRect.top) - textHeight;
			outRect.bottom = outRect.top + textHeight;
			outRect.right -= iconWidth;
		break;
		
		case iconPosition_BottomCenter:
			if ( iconHeight > 0 )
				outRect.top = iconRect.top - (textHeight + 2);
			else
				outRect.top += (outRect.bottom - outRect.top) - textHeight;
			outRect.bottom = outRect.top + textHeight;
		break;
		
		case iconPosition_BottomLeft:
			outRect.top += (outRect.bottom - outRect.top) - textHeight;
			outRect.bottom = outRect.top + textHeight;
			outRect.left += iconWidth;
		break;
		
		case iconPosition_LeftCenter:
			outRect.top += midHeight - 1;
			outRect.bottom = outRect.top + textHeight;
			outRect.left += iconWidth;
		break;
		
		case iconPosition_Center:
			if ( iconHeight > 0 )
				outRect.top = iconRect.bottom;
			else
				outRect.top += midHeight - 1;
			outRect.bottom = outRect.top + textHeight;
		break;
		
	}

	// е If the button is hilited and the offset flag is set then
	// we need to offset the rect byt one pixel to the bottom and right
	if ( IsHilited () && mOffsetIconOnHilite )
		::OffsetRect ( &outRect, 1, 1 );
	
}	//	LGAIconTextButton::CalcTitleRect


//-------------------------------------------------------------------------------------
// LGAIconTextButton::AdjustTransform
//-------------------------------------------------------------------------------------
//

void			
LGAIconTextButton::AdjustTransform	( 	Boolean	inMouseInButton,
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
	{
		DrawIcon ();
		
		// е We need to do the correct thing with the title, what we are
		// looking for is that the title only be drawn in white when the
		// button is pressed which corresponds with when the transform is
		// set to ttSelected all other times the title is drawn in black
		// we additionally have to call the title to get redrawn so that
		// the changes will show up
		if ( mTransform == ttNone )
		{
			mHiliteTitle = false;
			DrawIconButtonTitle ();
		}
		else if ( mTransform == ttSelected )
		{
			mHiliteTitle = true;
			DrawIconButtonTitle ();
		}
	}
	
}	//	LGAIconTextButton::AdjustTransform


#pragma mark -
#pragma mark === VISIBILITY

//=====================================================================================
// ее VISIBILITY
//-------------------------------------------------------------------------------------
// LGAIconTextButton::ShowSelf
//-------------------------------------------------------------------------------------

void
LGAIconTextButton::ShowSelf ()
{
	
	// е We need to do this because when a button is first created if
	// it is selected the title will not draw correctly, this fixes it
	if ( IsHilited () && IsSelected ())
		mHiliteTitle = false;
	else
		mHiliteTitle = true;
	
}	//	LGAIconTextButton::ShowSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAIconTextButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAIconTextButton::DrawSelf	()
{

	// е Get the control drawn
	LGAIconButton::DrawSelf ();

	// е Draw the button's title
	DrawIconButtonTitle ();
	
}	//	LGAIconTextButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGAIconTextButton::DrawIconButtonTitle
//-------------------------------------------------------------------------------------

void
LGAIconTextButton::DrawIconButtonTitle	()
{

	StColorPenState	theColorPenState;
	StTextState			theTextState;
	
	// е Figure out what the justification is from the text trait
	SInt16	titleJust = UTextTraits::SetPortTextTraits ( GetTextTraitsID () );
	
	// е Calculate the title rect
	Rect	titleRect;
	CalcTitleRect ( titleRect );
	
	// е Setup the text color which starts out initially as black
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// е Get the title
	Str255 controlTitle;
	GetDescriptor ( controlTitle );

	// е Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( titleRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )
		{
			StColorPenState::Normalize ();

			// е If the control is dimmed then we use the grayishTextOr 
			// transfer mode to draw the text
			RGBColor theColor;
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
			StColorPenState::Normalize ();

			// е If the control is hilited and the flag for hiliting the title is
			// true we always hilite in white
			if ( IsHilited () && GetHiliteTitle () )
			{
				RGBColor theColor = UGAColorRamp::GetWhiteColor();
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
			Str255 controlTitle;
			GetDescriptor ( controlTitle );
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
		}	
	}
	
}	//	LGAIconTextButton::DrawIconButtonTitle

