// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAIconTextControl.cp

CLASSES:				LGAIconTextControl

DESCRIPTION:		Implementation file for Object

AUTHOR:				Robin Mair

CREATION DATE :	96.12.03

CHANGE HISTORY :

		96.12.03		rtm	Initial creation of file.

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAIconTextControl.h>
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LControl.h>
#include <LStream.h>
#include	<UTextTraits.h>
#include <PP_Messages.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>

// е TOOLBOX HEADERS
#include <Quickdraw.h>
#include <Icons.h>
#include	<LowMem.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

const Int16		iconText_TextOffset = 4;


//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGAIconTextControl
#pragma mark -
#pragma mark === STREAM CREATION

//=====================================================================================
// ее STREAM CREATION
//-------------------------------------------------------------------------------------
// LGAIconTextControl::CreateLGAIconTextControlStream	
//-------------------------------------------------------------------------------------
//

LGAIconTextControl*	
LGAIconTextControl::CreateLGAIconTextControlStream ( LStream *inStream )
{

	return ( new LGAIconTextControl ( inStream ));
	
}	// LGAIconTextControl::CreateLGAIconTextControlStream


//=====================================================================================
// CLASS:	LGAIconTextControl
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAIconTextControl::LGAIconTextControl					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconTextControl::LGAIconTextControl ()
{	

	LString::CopyPStr ( gEmptyString, mTitle );
	mHiliteTitle = false;
	mTitleHiliteColor = UGAColorRamp::GetWhiteColor();
	mTextTraitsID = 0;
	
}	//	LGAIconTextControl::LGAIconTextControl


//-------------------------------------------------------------------------------------
// LGAIconTextControl::LGAIconTextControl					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconTextControl::LGAIconTextControl (	LStream	*inStream ) 
										: LGAIconSuiteControl ( inStream )
{

	// е Setup some default values
	mHiliteTitle = true;
	mTitleHiliteColor = UGAColorRamp::GetWhiteColor();

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

}	//	LGAIconTextControl::LGAIconTextControl


//-------------------------------------------------------------------------------------
// LGAIconTextControl::~LGAIconTextControl				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconTextControl::~LGAIconTextControl ()
{
}	//	LGAIconTextControl::~LGAIconTextControl


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAIconTextControl::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGAIconTextControl::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGAIconTextControl::GetDescriptor


//-------------------------------------------------------------------------------------
// LGAIconTextControl::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGAIconTextControl::SetHiliteState	(	Boolean	inHiliteState )
{
	
	// е Call our parent to hande the changing of the state
	LGAIconSuiteControl::SetHiliteState ( inHiliteState );

	// е When the state is being turned off we want to make sure that
	// we get rid of the background color behind the text
	// NOTE: for now this just paints it white which will not allow this
	// class to work very well against a colored background
	if ( !inHiliteState )
	{
		// е Erase the title rect so that the background gets redrawn
		Rect	titleRect;
		CalcTitleRect ( titleRect );
		::RGBBackColor ( &UGAColorRamp::GetWhiteColor () );
		::EraseRect ( &titleRect );
		
		// е Get the title redrawn
		DrawIconTextControlTitle ();
	}
			
}	//	LGAIconTextControl::SetHiliteState


//-------------------------------------------------------------------------------------
// LGAIconTextControl::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGAIconTextControl::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGAIconTextControl::SetDescriptor


//-------------------------------------------------------------------------------------
// LGAIconTextControl::SetTextTraitsID
//-------------------------------------------------------------------------------------
//

void			
LGAIconTextControl::SetTextTraitsID	( ResIDT inTextTraitID )
{
	//  е only change the ID if it is different
	if ( GetTextTraitsID () != inTextTraitID )
	{
		mTextTraitsID = inTextTraitID;
		Refresh ();
	}
		
}	//	LGAIconTextControl::SetTextTraitsID


//-------------------------------------------------------------------------------------
// LGAIconTextControl::CalcTitleRect
//-------------------------------------------------------------------------------------

void
LGAIconTextControl::CalcTitleRect ( Rect	&outRect )
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
	Int16		textHeight = fi.ascent + fi.descent;
		
	// е Get the local inset frame rectangle
	CalcLocalFrameRect ( outRect );
	
	// е Calculate the icon width based on the size selector
	Int16		iconWidth = mSizeSelector == 12 ? 16 : mSizeSelector;
	
	// е Calculate the mid point based on the font ascent
	Int16		midHeight = (((outRect.bottom - outRect.top) - fi.ascent) / 2);
	
	// е Getthe local icon rect which we want to use for positioning the title
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
			outRect.left += iconWidth + iconText_TextOffset;
		break;
		
		case iconPosition_TopCenter:
			outRect.top = iconRect.bottom + 1;
			outRect.bottom = outRect.top + textHeight;
		break;
		
		case iconPosition_TopRight:
			outRect.bottom = outRect.top + textHeight;
			outRect.right -= iconWidth + iconText_TextOffset;
		break;
		
		case iconPosition_RightCenter:
			outRect.top += midHeight - 1;
			outRect.bottom = outRect.top + textHeight;
			outRect.right -= iconWidth + iconText_TextOffset;
		break;
		
		case iconPosition_BottomRight:
			outRect.top += (outRect.bottom - outRect.top) - textHeight;
			outRect.bottom = outRect.top + textHeight;
			outRect.right -= iconWidth + iconText_TextOffset;
		break;
		
		case iconPosition_BottomCenter:
			outRect.top = iconRect.top - (textHeight + 2);
			outRect.bottom = outRect.top + textHeight;
		break;
		
		case iconPosition_BottomLeft:
			outRect.top += (outRect.bottom - outRect.top) - textHeight;
			outRect.bottom = outRect.top + textHeight;
			outRect.left += iconWidth + iconText_TextOffset;
		break;
		
		case iconPosition_LeftCenter:
			outRect.top += midHeight - 1;
			outRect.bottom = outRect.top + textHeight;
			outRect.left += iconWidth + iconText_TextOffset;
		break;
		
		case iconPosition_Center:
			outRect.top = iconRect.bottom;
			outRect.bottom = outRect.top + textHeight;
		break;
		
	}
	
}	//	LGAIconTextControl::CalcTitleRect


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAIconTextControl::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAIconTextControl::DrawSelf	()
{

	// е Get the control drawn
	LGAIconSuiteControl::DrawSelf ();

	// е If the control is hilited then make sure that we get the
	// background of the title hilited
	if ( IsHilited ())
		DrawIconTextControlHilited ();
		
	// е Draw the control's title
	DrawIconTextControlTitle ();
	
}	//	LGAIconTextControl::DrawSelf


//-------------------------------------------------------------------------------------
// LGAIconTextControl::DrawIconTextControlTitle
//-------------------------------------------------------------------------------------

void
LGAIconTextControl::DrawIconTextControlTitle	()
{

	StColorPenState	theColorPenState;
	StTextState			theTextState;
	
	// е Figure out what the justification is from the text trait
	Int16	titleJust = UTextTraits::SetPortTextTraits ( GetTextTraitsID () );
	
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
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )
		{
			StColorPenState::Normalize ();

			// е If the control is dimmed then we use the grayishTextOr 
			// transfer mode to draw the text
			if ( !IsEnabled ())
			{
				::RGBForeColor ( &UGAColorRamp::GetBlackColor () );
				::TextMode ( grayishTextOr );
			}
			else if ( IsHilited () )
			{
				// е When we are hilited we simply draw the title in white
				::RGBForeColor ( &UGAColorRamp::GetWhiteColor () );
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
			if ( IsHilited () /*&& GetHiliteTitle ()*/ )
				// е Now we can setup the foreground color for the text to draw with
				::RGBForeColor ( &UGAColorRamp::GetWhiteColor() );
				
			// е If the control is dimmed then we have to do our own version of the
			// grayishTextOr as it does not appear to work correctly across
			// multiple devices
			if ( !IsEnabled () && !IsHilited ())
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
	
}	//	LGAIconTextControl::DrawIconTextControlTitle


//-------------------------------------------------------------------------------------
// LGAIconTextControl::DrawIconTextControlHilited
//-------------------------------------------------------------------------------------

void
LGAIconTextControl::DrawIconTextControlHilited	()
{

	Rect		titleRect;
	CalcTitleRect ( titleRect );

	// е When we hilite the control we hilite the title by first
	// painting the background of the title in the hilite color
	// then the text will be drawn over it
	RGBColor	hiliteColor;
	::LMGetHiliteRGB ( &hiliteColor );
	::RGBForeColor ( &hiliteColor );
	::PaintRect ( &titleRect );
	
}	//	LGAIconTextControl::DrawIconTextControlHilited


