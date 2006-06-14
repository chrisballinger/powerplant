// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:					PTTargetBox.cp

CLASSES:				PTTargetBox

DESCRIPTION:			Implementation file for Object

AUTHOR:					Eric Shapiro

CREATION DATE :			96.07.21

CHANGE HISTORY :

		96.11.26	ebs		mTitleRect & mBorderRect are gone from parent, so use routines now
		96.07.21	ebs		Created from new & old code
	
---------------------------------------<< ¥ >>----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "LGABox.h"
#include "PTTargetBox.h"
#include "UGAColorRamp.h"
#include "UGraphicsUtilities.h"
#include "SUMiscUtils.h"

/*===============================
	Constructor
================================*/
PTTargetBox::PTTargetBox( LStream *inStream ) : LGABox( inStream )
{
	mHasTarget = false;
}

/*===============================
	Destructor
================================*/
PTTargetBox::~PTTargetBox()
{
}

/*===============================
	CreateFromStream
================================*/
PTTargetBox * PTTargetBox::CreateFromStream( LStream *inStream )
{
	return new PTTargetBox( inStream );
}

/*===============================
	SetTarget
================================*/
void PTTargetBox::SetTarget( Boolean inTarget, ERedrawOptions inRedraw )
{
	if ( mHasTarget == inTarget ) return;
	
	mHasTarget = inTarget;
	SUMiscUtils::RedrawPaneAsIndicated( this, inRedraw );
}

/*===============================
	DrawBoxTitle
================================*/
void PTTargetBox::DrawBoxTitle()
{

	StColorPenState		theColorPenState;
	StTextState			theTextState;
	Rect 				titleRect, borderRect;
	RGBColor			textColor, backColor;
	
	this->CalcTitleRect( titleRect );
	this->CalcBorderRect( borderRect );
	
	theColorPenState.Normalize();
	SInt16	just = UTextTraits::SetPortTextTraits ( GetTextTraitsID ());
	
	/****************************************************
		erase the area behind the title and get the appropriate
		color for the title depending on the device depth.
	****************************************************/
	if ( mHasBorder )
	{
		StDeviceLoop	theLoop ( titleRect );
		SInt16			depth;

		while ( theLoop.NextDepth ( depth )) 
		{
			if ( depth < 4 )				// Black & White
			{
				if ( mHasTarget )
				{
					backColor = UGAColorRamp::GetBlackColor();
					textColor = UGAColorRamp::GetWhiteColor();
				}
				else
				{
					backColor = UGAColorRamp::GetWhiteColor();
					textColor = UGAColorRamp::GetBlackColor();
				}
			}
			else							// Color
			{
				if ( mHasTarget )									// targetted
				{
					textColor = UGAColorRamp::GetWhiteColor();
					backColor = UGAColorRamp::GetBlackColor();
				}
				else if ( IsEnabled() )								// enabled
				{
					textColor = UGAColorRamp::GetBlackColor();
					this->GetBackgroundColor( &backColor );
				}
				else												// disabled
				{
					textColor = UGAColorRamp::GetColor(7);
					this->GetBackgroundColor( &backColor );
				}
			}
		}
	
		::RGBBackColor ( &backColor );
		::EraseRect ( &titleRect );
	}
	
	// ¥ If the control is dimmed then we use the grayishTextOr 
	// transfer mode to draw the text
	if ( !IsEnabled() )
		::TextMode ( grayishTextOr );
					
	// ¥ Get the actual text drawn with the appropriate justification
	::RGBForeColor ( &textColor );
	Str255 boxTitle;
	GetDescriptor ( boxTitle );
	UStringDrawing::DrawJustifiedString ( boxTitle, titleRect, just );
}

/*===============================
	GetBackgroundColor
	
	Decsription:
	Returns the superview's background color.
	
	Notes:
	For some reason, there is no call to get the current background color.
	Robin's code used "GetCPixel" to read a single pixel from the window,
	but that may fail if a window is partially overlapped. We'll just use
	the "ApplyForeAndBackColors" method and then read the background color
	out of the port.
================================*/
void PTTargetBox::GetBackgroundColor( RGBColor *outColor )
{
	StColorPenState		thePenState;		// save/restore pen state
	
	this->ApplyForeAndBackColors();
	::GetBackColor( outColor );
}

/*===============================
	DrawBlackAndWhiteBorder
================================*/
void PTTargetBox::DrawBlackAndWhiteBorder( const Rect &inBorderRect, EGABorderStyle inStyle )
{
	if ( !mHasTarget )
		this->DrawTargettedFrame( inBorderRect, false );	// erase old target frame (if any)
		
	LGABox::DrawBlackAndWhiteBorder( inBorderRect, inStyle );
	
	if ( mHasTarget )
		this->DrawTargettedFrame( inBorderRect, false );	// draw new target frame (if any)
}

/*===============================
	DrawBlackAndWhiteBorder
================================*/
void PTTargetBox::DrawColorBorder( const Rect &inBorderRect, EGABorderStyle inStyle )
{
	if ( !mHasTarget )
		this->DrawTargettedFrame( inBorderRect );			// erase old target frame (if any)
		
	LGABox::DrawColorBorder( inBorderRect, inStyle );
	
	if ( mHasTarget )
		this->DrawTargettedFrame( inBorderRect );			// draw new target frame (if any)
}

/*===============================
	DrawTargettedFrame
================================*/
void PTTargetBox::DrawTargettedFrame( const Rect &inBorderRect, Boolean inColor )
{
	StColorPenState		thePenState;
	thePenState.Normalize();

	if ( !mHasTarget )
	{
		RGBColor	backColor;
		if ( inColor )
			this->GetBackgroundColor( &backColor );
		else
			backColor = UGAColorRamp::GetWhiteColor();
		::RGBForeColor( &backColor );
	}
	
	::PenSize( 2, 2 );
	::FrameRect( &inBorderRect );
}

