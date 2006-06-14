// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAPictureButton.cp

CLASSES:				LGAPictureButton

DESCRIPTION:		Implementation file for LGAPictureButton

AUTHOR:				Robin Mair

CREATION DATE :	96.11.17

CHANGE HISTORY :

		96.12.09		rtm	Removed the color constants an switched to using the new
								one's provided by the color ramp.
		96.12.03		rtm	Added a couple of color constants now that we no longer have
								them in the color ramp header file.
		96.11.17		rtm	Initial creation of file.

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAPictureButton.h>
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <PP_Messages.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>

// е TOOLBOX HEADERS
#include <Quickdraw.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGAPictureButton

//=====================================================================================
// ее STREAM CREATION
//-------------------------------------------------------------------------------------
// LGAPictureButton::CreateLGAPictureButtonStream	
//-------------------------------------------------------------------------------------
//

LGAPictureButton*	
LGAPictureButton::CreateLGAPictureButtonStream ( LStream *inStream )
{

	return ( new LGAPictureButton ( inStream ));
	
}	// LGAPictureButton::CreateLGAPictureButtonStream


//=====================================================================================
// CLASS:	LGAPictureButton
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAPictureButton::LGAPictureButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAPictureButton::LGAPictureButton ()
{	

	mBevelWidth = 2;
	mNormalPictureID = 0;
	mOffsetPicture = true;
	
}	//	LGAPictureButton::LGAPictureButton


//-------------------------------------------------------------------------------------
// LGAPictureButton::LGAPictureButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAPictureButton::LGAPictureButton (	LStream	*inStream ) 
															: LModeControl ( inStream )
{

	// е Get the normal picture resource ID
	*inStream >> mNormalPictureID;
		
	// е Get the pushed picture resource ID
	*inStream >> mPushedPictureID;
		
	// е Get the offset flag for the picture
	*inStream >> mBevelWidth;
		
	// е Get the offset flag for the picture
	*inStream >> mOffsetPicture;
		
	// е Make sure the hilited flag gets setup correctly from any value
	// that is currently set
	mPushed = GetValue () != 0;	

}	//	LGAPictureButton::LGAPictureButton


//-------------------------------------------------------------------------------------
// LGAPictureButton::~LGAPictureButton				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAPictureButton::~LGAPictureButton ()
{
}	//	LGAPictureButton::~LGAPictureButton


//-------------------------------------------------------------------------------------
// LGAPictureButton::FinishCreateSelf
//-------------------------------------------------------------------------------------

void LGAPictureButton::FinishCreateSelf ()
{
}	//	LGAPictureButton::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAPictureButton::CalcLocalPictureRect
//-------------------------------------------------------------------------------------

void
LGAPictureButton::CalcLocalPictureRect ( Rect &outRect )
{

	if ( mNormalPictureID > 0 )
	{
		// е Get the location of the picture
		Point	pictLoc = CalcPictureLocation ();
		
		// е Figure out the width and height of the picture
		PicHandle	macPictureH = ::GetPicture ( mNormalPictureID );
		Rect	pictRect = (*macPictureH)->picFrame;
		Int16	width = UGraphicsUtilities::RectWidth ( pictRect );
		Int16 height = UGraphicsUtilities::RectHeight ( pictRect );;
		
		// е Build the picture rect
		outRect.top = pictLoc.v;
		outRect.left = pictLoc.h;
		outRect.bottom = outRect.top + height;
		outRect.right = outRect.left + width;
		
		// е If we are offsetting the picture when hiliting then do it now
		if ( IsPushed () && mOffsetPicture )
			::OffsetRect ( &outRect, 1, 1 );
	}
	else
		outRect = gEmptyRect;
		
}	//	LGAPictureButton::CalcLocalPictureRect


//-------------------------------------------------------------------------------------
// LGAPictureButton::SetBevelWidth
//-------------------------------------------------------------------------------------

void
LGAPictureButton::SetBevelWidth ( Int16		inBevelWidth,
										Boolean	inRedraw )
{
	// е Only change the bevel width if we have to
	if ( mBevelWidth != inBevelWidth )
	{
		mBevelWidth = inBevelWidth;
		
		// е Get everything redrawn if requested
		if ( inRedraw && FocusDraw ())
			DrawSelf ();
	}
	
}	//	LGAPictureButton::SetBevelWidth



#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawSelf	()
{
	StColorPenState	theColorPenState;
	StColorPenState::Normalize ();
	
	// е Get the control drawn in its various states
	if ( IsEnabled ())
	{
		if ( IsPushed ())
			DrawPictureButtonPushed ();
		else
			DrawPictureButtonNormal ();
	}
	else
		DrawPictureButtonDimmed ();

	// е Get the picture rectangle
	Rect pictureRect;
	CalcLocalPictureRect ( pictureRect );
	
	// е Now we finally draw the picture after everything else has been drawn
	if ( mPushedPictureID == 0 )
		mPushedPictureID = mNormalPictureID;
		
	// е Pick the appropriate picture ID and then get the picture drawn
	ResIDT	pictureID = !IsPushed () ? mNormalPictureID : mPushedPictureID;
	if ( pictureID > 0 )
	{
		PicHandle	macPictureH = ::GetPicture ( pictureID );
		ThrowIfNil_ ( macPictureH );
		::DrawPicture ( macPictureH, &pictureRect);
	}
	
}	//	LGAPictureButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawControlNormal
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonNormal	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawPictureButtonNormalBW ();
		}
		// е COLOR
		else
		{
			DrawPictureButtonNormalColor ();
		}	
	}
	
}	//	LGAPictureButton::DrawPictureButtonNormal


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonPushed
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonPushed	()
{
	
	// е Get the frame for the control
	Rect	localFrame;	
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawPictureButtonPushedBW ();
		}
		else	// е COLOR
		{
			DrawPictureButtonPushedColor ();
		}
	}
	
}	//	LGAPictureButton::DrawPictureButtonPushed


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonDimmed
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonDimmed	()
{
	
	// е Get the frame for the control
	Rect	localFrame;	
	CalcLocalFrameRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawPictureButtonDimmedBW ();
		}
		// е COLOR
		else 
		{
			DrawPictureButtonDimmedColor ();
		}	
	}
	
}	//	LGAPictureButton::DrawPictureButtonDimmed


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonNormalBW
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonNormalBW	()
{

	StColorPenState::Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	::RGBBackColor ( &UGAColorRamp::GetWhiteColor());
	::EraseRect ( &localFrame );
	
	// е Draw the frame for the control outside of everything else
	// the frame is draw in the foreground color
	::RGBForeColor ( &UGAColorRamp::GetBlackColor());
	::FrameRect ( &localFrame );
	
}	//	LGAPictureButton::DrawPictureButtonNormalBW


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonPushedBW
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonPushedBW	()
{

	StColorPenState::Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е We need to erase the old position of the icon if we had the
	// Offset Picture on Hilite option set
	::RGBForeColor ( &UGAColorRamp::GetBlackColor () );
	::PaintRect ( &localFrame );

}	//	LGAPictureButton::DrawPictureButtonPushedBW


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonDimmedBW
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonDimmedBW	()
{

	StColorPenState::Normalize ();

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	::RGBBackColor ( &UGAColorRamp::GetWhiteColor());
	::EraseRect ( &localFrame );
	
	// е Draw the frame for the control outside of everything else
	// the frame is draw in the foreground color
	::RGBForeColor ( &UGAColorRamp::GetBlackColor());
	::FrameRect ( &localFrame );
	
}	//	LGAPictureButton::DrawPictureButtonDimmedBW


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonNormalColor
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonNormalColor	()
{

	StColorPenState::Normalize ();

	// е Get the bevel width
	Int16		bevelWidth = GetBevelWidth ();

	// е Setup a color array that will handle the number of bevels
	RGBColor colorArray[3];
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// ее FRAME BUTTON
	::RGBForeColor ( &UGAColorRamp::GetColor(9) );
	UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

	::RGBForeColor ( &UGAColorRamp::GetColor(colorRamp_GrayA1) );
	UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

	// е TOP RIGHT
	UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
							localFrame.top, 
							(bevelWidth == 2 ? &UGAColorRamp::GetColor(9) :
															&UGAColorRamp::GetColor(10)) );
		
	// е BOTTOM LEFT
	UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
							localFrame.bottom - 1, 
							(bevelWidth == 2 ? &UGAColorRamp::GetColor(9) :
															&UGAColorRamp::GetColor(10)) );
		
	// е FACE COLOR
	::InsetRect ( &localFrame, 1, 1 );
	::RGBForeColor ( &UGAColorRamp::GetColor(3));
	::PaintRect ( &localFrame );
	
	// ее LIGHT BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetWhiteColor();
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(3);
			colorArray[1] = UGAColorRamp::GetWhiteColor();
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(3);
			colorArray[1] = UGAColorRamp::GetColor(1);
			colorArray[2] = UGAColorRamp::GetWhiteColor();
		break;
		
	}
	
	// е Draw top and left edges
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, bevelWidth );
	
	// ее SHADOW BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(7);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(8);
			colorArray[1] = UGAColorRamp::GetColor(6);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(10);
			colorArray[1] = UGAColorRamp::GetColor(8);
			colorArray[2] = UGAColorRamp::GetColor(6);
		break;
		
	}
	
	// е Draw bottom and right edges
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, bevelWidth );
	
	// ее CORNER PIXELS
	switch ( bevelWidth )
	{
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(5);
			colorArray[1] = UGAColorRamp::GetColor(3);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(5);
			colorArray[1] = UGAColorRamp::GetColor(4);
			colorArray[2] = UGAColorRamp::GetColor(3);
		break;
	}
	
	// е Paint corner pixels
	// е TOP RIGHT
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, bevelWidth );
	
	// е BOTTOM LEFT
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, bevelWidth );

	
}	//	LGAPictureButton::DrawPictureButtonNormalColor


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonPushedColor
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonPushedColor	()
{

	StColorPenState::Normalize ();

	// е Get the bevel width
	Int16		bevelWidth = GetBevelWidth ();

	// е Setup a color array that will handle the number of bevels
	RGBColor colorArray[3];
	
	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// ее FRAME BUTTON
	::RGBForeColor ( &UGAColorRamp::GetColor(colorRamp_GrayA2) );
	UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );				

	::RGBForeColor ( &UGAColorRamp::GetColor(11) );
	UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

	// е TOP RIGHT
	UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
							localFrame.top, 
							(bevelWidth == 2 ? &UGAColorRamp::GetColor(colorRamp_GrayA2) :
															&UGAColorRamp::GetColor(12)) );
		
	// е BOTTOM LEFT
	UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
							localFrame.bottom - 1, 
							(bevelWidth == 2 ? &UGAColorRamp::GetColor(colorRamp_GrayA2) :
															&UGAColorRamp::GetColor(12)) );
		
	// е FACE COLOR
	::InsetRect ( &localFrame, 1, 1 );
	::RGBForeColor ( &UGAColorRamp::GetColor(7));
	::PaintRect ( &localFrame );
	
	// ее LIGHT BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(10);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(11);
			colorArray[1] = UGAColorRamp::GetColor(9);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(colorRamp_GrayA1);
			colorArray[1] = UGAColorRamp::GetColor(11);
			colorArray[2] = UGAColorRamp::GetColor(9);
		break;
		
	}
			
	// е Draw top left edges
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, bevelWidth );
	
	// ее SHADOW BEVELS
	// е Setup Colors
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(5);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(4);
			colorArray[1] = UGAColorRamp::GetColor(6);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(4);
			colorArray[1] = UGAColorRamp::GetColor(5);
			colorArray[2] = UGAColorRamp::GetColor(6);
		break;
		
	}
	
	// е Draw bottom right edges
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, bevelWidth );
	
	// ее CORNER PIXELS
	// е TOP RIGHT
	switch ( bevelWidth )
	{
		case 1:
			colorArray[0] = UGAColorRamp::GetColor(8);
		break;
		
		case 2:
			colorArray[0] = UGAColorRamp::GetColor(8);
			colorArray[1] = UGAColorRamp::GetColor(7);
		break;
		
		case 3:
			colorArray[0] = UGAColorRamp::GetColor(11);
			colorArray[1] = UGAColorRamp::GetColor(8);
			colorArray[2] = UGAColorRamp::GetColor(7);
		break;
	}
	
	// ее CORNER PIXELS
	// е TOP RIGHT
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, bevelWidth );
	
	// е BOTTOM LEFT
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, bevelWidth );

}	//	LGAPictureButton::DrawPictureButtonPushedColor


//-------------------------------------------------------------------------------------
// LGAPictureButton::DrawPictureButtonDimmedColor
//-------------------------------------------------------------------------------------

void
LGAPictureButton::DrawPictureButtonDimmedColor	()
{

	StColorPenState::Normalize ();

	Int16		bevelWidth = GetBevelWidth ();
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е DISABLED NORMAL STATE
	if ( !IsPushed ())
	{
		// ее FRAME BUTTON
		::RGBForeColor ( &UGAColorRamp::GetColor(5) );
		UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

		
		::RGBForeColor ((bevelWidth == 3 ? &UGAColorRamp::GetColor(7) :
													&UGAColorRamp::GetColor(6)) );
		UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				


		// е TOP RIGHT
		if ( bevelWidth == 3 )
		{
			UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
									localFrame.top, 
									&UGAColorRamp::GetColor(6) );
				
			// е BOTTOM LEFT
			UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
									localFrame.bottom - 1, 
									&UGAColorRamp::GetColor(6) );
		}
	}
	else	// DISABLED HILITED STATE
	{
		// ее FRAME BUTTON
		::RGBForeColor ( &UGAColorRamp::GetColor(8) );
		UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

		::RGBForeColor ( &UGAColorRamp::GetColor(6) );
		UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				

		if ( bevelWidth != 2 )
		{
			// е TOP RIGHT
			UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
									localFrame.top, 
									&UGAColorRamp::GetColor(7) );
				
			// е BOTTOM LEFT
			UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
									localFrame.bottom - 1, 
									&UGAColorRamp::GetColor(7) );
		}
	}


	// е Paint the face of the button
	::InsetRect ( &localFrame, 1, 1 );
	::RGBForeColor ( &( !IsPushed () ? UGAColorRamp::GetColor(2) :
										UGAColorRamp::GetColor(4)));
	::PaintRect ( &localFrame );

}	//	LGAPictureButton::DrawPictureButtonDimmedColor


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGAPictureButton::CalcPictureLocation
//-------------------------------------------------------------------------------------

Point
LGAPictureButton::CalcPictureLocation ()
{
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	Int16 pictOffset = GetBevelWidth () + 2;
	Point pictLoc = { localFrame.top + pictOffset, localFrame.left + pictOffset };
	
	return pictLoc;
	
}	//	LGAPictureButton::CalcIconLocation


