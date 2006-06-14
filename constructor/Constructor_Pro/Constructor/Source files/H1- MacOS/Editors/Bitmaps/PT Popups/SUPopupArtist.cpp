// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.11.26	ebs		Previous global calls are now within UGraphicsUtilities
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUPopupArtist.h"
#include "SU_Constants.h"
#include "UGAColorRamp.h"
#include "UGraphicsUtilities.h"		// for various drawing calls

/*==================================================
	Constants (from Robin's code)
===================================================*/
const SInt16 	gsPopup_ArrowHeight			= 	3;		// 4 Actual height of the arrow
const SInt16 	gsPopup_ArrowWidth			= 	5;		//	7 Actual width of the arrow at widest


/*==================================================
	DrawPopup
===================================================*/
void SUPopupArtist::DrawPopup( const Rect &inBounds, Boolean inHilited, Boolean inEnabled )
{
	SInt16				theDepth;
	StDeviceLoop		theLoop( inBounds );
	
	#ifdef OUT_FOR_NOW		// ??? needed?
	/*************************************************
		Don't draw on the swatch area 'cause it flickers
	*************************************************/
	Rect				swatchR;
	CalculateSwatchArea( inBounds, &swatchR );
	StClipRgnState		savedClip( swatchR );
	#endif
	
	StColorPenState		savedPenState;	
	savedPenState.Normalize();

	while( theLoop.NextDepth( theDepth ) )
	{
		if ( theDepth < 4 )
		{
			if ( inHilited )
				DrawPopupHilitedBW( inBounds );
			else if ( inEnabled )
				DrawPopupNormalBW( inBounds );
			else
				DrawPopupDimmedBW( inBounds );
		}
		else
		{
			if ( inHilited )
				DrawPopupHilitedColor( inBounds );
			else if ( inEnabled )
				DrawPopupNormalColor( inBounds );
			else
				DrawPopupDimmedColor( inBounds );
		}
	}
}

/*==================================================
	DrawPopupNormalBW
===================================================*/
void SUPopupArtist::DrawPopupNormalBW( const Rect &inBounds )
{
	Rect	localFrame = inBounds;
	
		// needed to unhilite us (when we switch from hilited -> not)
	::EraseRect( &localFrame );
	
		// draw the frame
	RGBColor 	colorArray[4];
	colorArray[0] = UGAColorRamp::GetBlackColor();
	colorArray[1] = UGAColorRamp::GetBlackColor();
	colorArray[2] = UGAColorRamp::GetBlackColor();
	colorArray[3] = UGAColorRamp::GetBlackColor();
	UGraphicsUtilities::AngledCornerFrame ( &localFrame, colorArray, 2 );

		// and the arrow
	DrawPopupArrow( inBounds, 1, false, true );
}

/*==================================================
	DrawPopupNormalColor
===================================================*/
void SUPopupArtist::DrawPopupNormalColor( const Rect &inBounds )
{
	// ¥ Get the frame for the control
	Rect	localFrame = inBounds;

	// ¥ Draw the frame for the control outside of everything else
	// the frame is draw in the foregorund color
	UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );

	// ¥ First make sure the face of the control is drawn
	::InsetRect ( &localFrame, 1, 1 );
	RGBColor theColor = UGAColorRamp::GetColor(2);
	::RGBForeColor ( &theColor );
	::PaintRect ( &localFrame );
	
	// ¥ Start by rendering the bevelled edges of the sides facing the light
	// source
	RGBColor colorArray[3];
	colorArray[0] = UGAColorRamp::GetColor(2);
	colorArray[1] = kWhiteColor;
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, 2 );
	
	// ¥ Render the shadow bevels
	colorArray[0] = UGAColorRamp::GetColor(8);
	colorArray[1] = UGAColorRamp::GetColor(6);
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, 2 );
	
	// ¥ Take care of the corner details 
	// ¥ TOP LEFT
	colorArray[0] = kWhiteColor;
	colorArray[1] = kWhiteColor;
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopLeft, colorArray, 2 );
	
	// ¥ TOP RIGHT
	colorArray[0] = UGAColorRamp::GetColor(2);
	colorArray[1] = UGAColorRamp::GetColor(2);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, 2 );
	
	// ¥ BOTTOM RIGHT
	colorArray[0] = kBlackColor;
	colorArray[1] = UGAColorRamp::GetColor(8);
	colorArray[2] = UGAColorRamp::GetColor(6);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomRight, colorArray, 3 );
	
	// ¥ BOTTOM LEFT
	colorArray[0] = UGAColorRamp::GetColor(2);
	colorArray[1] = UGAColorRamp::GetColor(2);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, 2 );

	//	¥ Draw the frame around the color swatch
	Rect		swatchR;
	CalculateSwatchArea ( inBounds, &swatchR );
	::InsetRect ( &swatchR, -1, -1 );
	
	// ¥ Dark Shadow top and left
	theColor = UGAColorRamp::GetColor(8);
	::RGBForeColor ( &theColor );
	::MoveTo ( swatchR.left, swatchR.bottom - 2 );
	::LineTo ( swatchR.left, swatchR.top );
	::LineTo ( swatchR.right - 2, swatchR.top );
	
	// ¥ Light shadow bottom and Right
	theColor = UGAColorRamp::GetWhiteColor();
	::RGBForeColor ( &theColor );
	::MoveTo ( swatchR.left + 1, swatchR.bottom - 1 );
	::LineTo ( swatchR.right - 1, swatchR.bottom - 1 );
	::LineTo ( swatchR.right - 1, swatchR.top + 1 );

		// finally, draw the arrow
	DrawPopupArrow( inBounds, 8, false, true );
}

/*==================================================
	DrawPopupHilitedBW
===================================================*/
void SUPopupArtist::DrawPopupHilitedBW( const Rect &inBounds )
{
	Rect		localFrame = inBounds;
	
	::InsetRect ( &localFrame, 1, 1 );
	::InvertRect ( &localFrame );

	DrawPopupArrow( inBounds, 1, true, true );
}

/*==================================================
	DrawPopupHilitedColor
===================================================*/
void SUPopupArtist::DrawPopupHilitedColor( const Rect &inBounds )
{
	Rect localFrame = inBounds;
	
	// ¥ Draw the frame for the control outside of everything else
	// the frame is draw in the foregorund color
	UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );

	// ¥ Paint the face of the button first
	::InsetRect ( &localFrame, 1, 1 );
	RGBColor theColor = UGAColorRamp::GetColor(5);
	::RGBForeColor ( &theColor );
	::PaintRect ( &localFrame );
	
	// ¥ Start by rendering the bevelled edges of the sides facing the light
	// source
	RGBColor colorArray[2];
	colorArray[0] = UGAColorRamp::GetColor(9);
	colorArray[1] = UGAColorRamp::GetColor(7);
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, 2 );
	
	// ¥ Render the shadow bevels
	colorArray[0] = UGAColorRamp::GetColor(1);
	colorArray[1] = UGAColorRamp::GetColor(3);
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, 2 );
	
	// ¥ Take care of the corener details 
	// ¥ TOP LEFT
	::InsetRect ( &localFrame, 1, 1 );
	colorArray[0] = UGAColorRamp::GetColor(9);
	colorArray[1] = UGAColorRamp::GetColor(7);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopLeft, colorArray, 2 );
	::InsetRect ( &localFrame, -1, -1 );
	
	// ¥ TOP RIGHT
	colorArray[0] = UGAColorRamp::GetColor(5);
	colorArray[1] = UGAColorRamp::GetColor(5);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, 2 );
	
	// ¥ BOTTOM RIGHT
	::InsetRect ( &localFrame, 1, 1 );
	colorArray[0] = UGAColorRamp::GetColor(1);
	colorArray[1] = UGAColorRamp::GetColor(3);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomRight, colorArray, 2 );
	::InsetRect ( &localFrame, -1, -1 );
	
	// ¥ BOTTOM LEFT
	colorArray[0] = UGAColorRamp::GetColor(5);
	colorArray[1] = UGAColorRamp::GetColor(5);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, 2 );

	//	¥ Draw the frame around the color swatch
	Rect		swatchR;
	CalculateSwatchArea ( inBounds, &swatchR );
	::InsetRect ( &swatchR, -1, -1 );
	
	// ¥ Dark Shadow top and left
	theColor = UGAColorRamp::GetColor(9);
	::RGBForeColor ( &theColor );
	::MoveTo ( swatchR.left, swatchR.bottom - 2 );
	::LineTo ( swatchR.left, swatchR.top );
	::LineTo ( swatchR.right - 2, swatchR.top );
	
	// ¥ Light shadow bottom and Right
	theColor = UGAColorRamp::GetColor(3);
	::RGBForeColor ( &theColor );
	::MoveTo ( swatchR.left + 1, swatchR.bottom - 1 );
	::LineTo ( swatchR.right - 1, swatchR.bottom - 1 );
	::LineTo ( swatchR.right - 1, swatchR.top + 1 );
	
	// ¥ Finally, the popup arrow
	DrawPopupArrow( inBounds, 8, true, true );
}

/*==================================================
	DrawPopupDimmedBW
===================================================*/
void SUPopupArtist::DrawPopupDimmedBW( const Rect &inBounds )
{
	// ¥ Get the frame for the control
	Rect	localFrame = inBounds;
	
	// ¥ Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	::RGBBackColor ( &kWhiteColor );
	::EraseRect ( &localFrame );
		
	// ¥ Draw the frame for the control outside of everything else
	// the frame is draw in the foreground color
	Pattern thePattern;
	UQDGlobals::GetGrayPat(&thePattern);
	::PenPat ( &thePattern );
	UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );

	// ¥ and the arrow
	DrawPopupArrow( inBounds, 1, false, false );
}

/*==================================================
	DrawPopupDimmedColor
===================================================*/
void SUPopupArtist::DrawPopupDimmedColor( const Rect &inBounds )
{
	// ¥ Get the frame for the control
	Rect	localFrame = inBounds;

	// ¥ Draw the frame for the control outside of everything else
	// the frame is draw in the foregorund color
	RGBColor theColor = UGAColorRamp::GetColor(5);
	::RGBForeColor ( &theColor );
	UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );

	// ¥ First make sure the face of thecontrol is drawn
	::InsetRect ( &localFrame, 1, 1 );
	theColor = UGAColorRamp::GetColor(1);
	::RGBForeColor ( &theColor );
	::PaintRect ( &localFrame );
	
	// ¥ Start by rendering the bevelled edges of the sides facing the light
	// source
	RGBColor colorArray[2];
	colorArray[0] = kWhiteColor;
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_TopLeft, colorArray, 1 );
	
	// ¥ Render the shadow bevels
	colorArray[0] = UGAColorRamp::GetColor(3);
	UGraphicsUtilities::BevelledEdge ( &localFrame, rectEdge_BottomRight, colorArray, 1 );
	
	// ¥ Take care of the corner details 
	// ¥ TOP LEFT
	colorArray[0] = kWhiteColor;
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopLeft, colorArray, 1 );
	
	// ¥ TOP RIGHT
	colorArray[0] = UGAColorRamp::GetColor(1);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_TopRight, colorArray, 1 );
	
	// ¥ BOTTOM RIGHT
	colorArray[0] = UGAColorRamp::GetColor(5);
	colorArray[1] = UGAColorRamp::GetColor(3);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomRight, colorArray, 2 );
	
	// ¥ BOTTOM LEFT
	colorArray[0] = UGAColorRamp::GetColor(1);
	UGraphicsUtilities::PaintCornerPixels ( &localFrame, rectCorner_BottomLeft, colorArray, 1 );

	//	¥ Draw the frame around the color swatch
	Rect		swatchR;
	CalculateSwatchArea ( inBounds, &swatchR );
	::InsetRect ( &swatchR, -1, -1 );
	
	// ¥ Dark Shadow top and left
	theColor = UGAColorRamp::GetColor(3);
	::RGBForeColor ( &theColor );
	::MoveTo ( swatchR.left, swatchR.bottom - 2 );
	::LineTo ( swatchR.left, swatchR.top );
	::LineTo ( swatchR.right - 2, swatchR.top );
	
	// ¥ Light shadow bottom and Right
	theColor = UGAColorRamp::GetWhiteColor();
	::RGBForeColor ( &theColor );
	::MoveTo ( swatchR.left + 1, swatchR.bottom - 1 );
	::LineTo ( swatchR.right - 1, swatchR.bottom - 1 );
	::LineTo ( swatchR.right - 1, swatchR.top + 1 );
	
	// ¥ And the arrow
	DrawPopupArrow( inBounds, 8, false, false );
}


/*==================================================
	DrawPopupArrow
===================================================*/
void SUPopupArtist::DrawPopupArrow( const Rect &inBounds, SInt16 inDepth, 
									Boolean inHilited, Boolean inEnabled )
{
	/*****************************************
		find the color to draw the arrow in
	*****************************************/
	RGBColor	theColor;
	GetArrowColor( inDepth, inHilited, inEnabled, &theColor );
	
	/*****************************************
		figure out where to draw the lines
	*****************************************/
	SInt16		height = inBounds.bottom - inBounds.top;	
	SInt16		startV = inBounds.top + ((height - gsPopup_ArrowHeight) / 2) + 2; // 1
	SInt16		leftEdge = inBounds.right - gsPopup_ArrowWidth - 6;
	SInt16		rightEdge = leftEdge + gsPopup_ArrowWidth - 1;
	
	/*****************************************
		draw the arrow
	*****************************************/
	::RGBForeColor( &theColor );
	for ( SInt16 count = 0; count < gsPopup_ArrowHeight; count++ )
	{
		::MoveTo( leftEdge + count, startV + count );
		::LineTo( rightEdge - count, startV + count );
	}
}

/*==================================================
	GetArrowColor
===================================================*/
void SUPopupArtist::GetArrowColor( SInt16 inDepth, 
									Boolean inHilited, Boolean inEnabled,
									RGBColor *outColor )
{
	if ( inDepth < 4 )							// bw
	{
		*outColor = inHilited ? kWhiteColor : kBlackColor;
	}
	else										// color
	{
		if ( inHilited )
			*outColor = kWhiteColor;
		else
		{
			if (inEnabled)
				*outColor = kBlackColor;
			else
			{
				RGBColor theColor = UGAColorRamp::GetColor ( 7 );
				*outColor = theColor;
			}
		}
	}
}

/*==================================================
	CalculateSwatchArea
===================================================*/
void SUPopupArtist::CalculateSwatchArea( const Rect &inBounds, Rect *outBounds )
{
	*outBounds = inBounds;
	outBounds->right -= (gsPopup_ArrowWidth + 6 + 4); // was + 3
	outBounds->left += 5;
	outBounds->top += 5;
	outBounds->bottom -= 5;
}

