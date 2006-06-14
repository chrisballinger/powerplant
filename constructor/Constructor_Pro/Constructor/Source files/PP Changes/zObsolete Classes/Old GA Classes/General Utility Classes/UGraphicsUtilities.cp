// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/* Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					UGraphicsUtilities.cp

CLASSES:				

DESCRIPTION:		Implementation file for a collection of useful utility functions and
	classes that are used throughout the grayscale classes.

AUTHOR:				Robin Mair

CREATION DATE :	95.10.07

CHANGE HISTORY :

		97.05.06		rtm	Added a couple of additional methods that are going to be used
								for some future buffered drawing.
		97.03.16		rtm	Added a few static fields that are used as flags to indicate
								whether certain capabilities are available at runtime.  These
								flags are for the Appearance Manager, Aaron and MacOS 8. A new
								InitGraphicsUtilities static function has also been added that
								is called in order to initialize these variables. If you want
								to use the associated static function you need to call the
								InitGraphicsUtilities() function somewhere at in the initialization
								process for your application. This was done this way so that
								we are not always calling Gestalt to find out whether these
								capabilities are available, as Gestalt can be slow!  So instead
								we just cahce the results.
								NOTE: anyone that has been using the HasMacOS8 call, it has 
								changed from its original form in that it now only checks for 
								the presence of MacOS8 and not Aaron as well!
		97.03.10		rtm	Changed the SlowDown method so that it now passes a dummy long instead
								of nil as the second parameter to Delay().
		96.10.19		rtm	Converted some functions to inlines. Also removed the 
								TruncateStrings function from UStringDrawing which now just calls
								TruncString instead.
		96.10.10		rtm	Changed PaintColorPixel so that it uses a LineTo call to paint
								the pixel instead of PaintRect, this sped it up a little!
		96.10.10		rtm	Turned UGraphicsUtilities into a class with all the functions
								as static methods on the class.
		96.10.04		rtm	Added a PaintColorPixel function that can be used instead of
								SetCPixel, it uses a PaintRect call which means that it can be
								recorded in a picture and printed.
		96.09.27		rtm	Changed the NoCornerPixelFrame function so that it now simply
								calls FrameRoundRect.
		96.09.04		rtm	PowerPlant conformance changes.
		96.02.03		rtm	Added some additional utitlities in particular a string drawing
								class that can handle drawing truncated justified strings.  Also
								added some operators for comparing RGBColors for equality.
		96.01.29		rtm	Added functions for calculating width and height of rectangles
		96.01.17		rtm	Added an additional routine - RoundCornerEdges - which is used
								to draw edges on the buttons that are patterned after the
								standard toolbox button.  This function is hard coded to deal
								with a very specific round rect with an edge radius of 10.
		96.01.16		rtm	Added a couple of additional drawing routines that are used by
								the various classes that need to draw embossed or recessed
								frames around objects.
		95.11.13		rtm	Changed parameter names to reflect PowerPlant standards
		95.11.15		rtm	Added an additional function to render around a notch on the
								bottom right edges of a rect.
		95.10.07		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include <UGraphicsUtilities.h>

// е POWERPLANT HEADERS
#include <LString.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>

// е TOOLBOX HEADERS
#include <Gestalt.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

/*enum
{
	gestaltAppearanceAttr			=	'appr',
	gestaltAppearanceExists			=	0,
	gestaltAppearanceCompatMode	=	1
};*/


//=====================================================================================
// GLOBAL VARIABLES
//=====================================================================================

Rect					gEmptyRect			=	{ 0, 0, 0, 0 };					//	Empty rectangle
Point					gEmptyPoint 		= 	{ 0, 0 };							//	Empty point
Str255				gEmptyString		=	"\p";									// Empty string


//=====================================================================================
// INITIALIZE STATIC FIELDS
//=====================================================================================

Boolean UGraphicsUtilities::sHasAppearance = false;
Boolean UGraphicsUtilities::sInAppearanceCompatMode = false;
Boolean UGraphicsUtilities::sHasAaron = false;
Boolean UGraphicsUtilities::sHasMacOS8 = false;

//=====================================================================================
// CLASS: UGraphicsUtilities
//=====================================================================================

#pragma mark === CLASS: UGraphicsUtilities
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION
//-------------------------------------------------------------------------------------
// UGraphicsUtilities::InitGraphicsUtilities
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::InitGraphicsUtilities ()
{
	
  // е Check for Appearance Manager
  SInt32	appearanceAttr;
  ::Gestalt ( gestaltAppearanceAttr, &appearanceAttr );
  sHasAppearance = (appearanceAttr & (1 << gestaltAppearanceExists)) != 0;
  sInAppearanceCompatMode = (appearanceAttr & (1 << gestaltAppearanceCompatMode)) != 0;
  
  // е Check for Aaron
  SInt32	hasAaron;
  ::Gestalt ( 'Aarn', &hasAaron );
  sHasAaron = (hasAaron == 0x003);
	
  // е Check for Mac OS 8
  SInt32	systemVersion;
  ::Gestalt ( gestaltSystemVersion, &systemVersion );
  sHasMacOS8 = (systemVersion >= 0x800);
	
}	//	UGraphicsUtilities:: InitGraphicsUtilities


#pragma mark -
#pragma mark === COLOR MANIPULATION

//=====================================================================================
// ее COLOR MANIPULATION
//-------------------------------------------------------------------------------------
// UGraphicsUtilities::Darken
//-------------------------------------------------------------------------------------
//

RGBColor
UGraphicsUtilities::Darken ( RGBColor *inColor )
{
	RGBColor newColor;
	
	newColor.red = inColor->red >> 1;
	newColor.green = inColor->green >> 1;
	newColor.blue = inColor->blue >> 1;
	
	return newColor;
	
}	//	UGraphicsUtilities:: Darken



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::Lighten
//-------------------------------------------------------------------------------------
//

RGBColor
UGraphicsUtilities::Lighten ( RGBColor *inColor )
{
	RGBColor newColor;
	
	newColor.red = inColor->red + 65535 >> 1;
	newColor.green = inColor->green + 65535 >> 1;
	newColor.blue = inColor->blue + 65535 >> 1;
	
	return newColor;
	
}	// UGraphicsUtilities::Lighten


#pragma mark -
#pragma mark === RENDERING EDGES OF RECTANGLES ===

//=====================================================================================
// ее RENDERING EDGES OF RECTANGLES
//-------------------------------------------------------------------------------------
// UGraphicsUtilities::BottomRightSide
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::BottomRightSide ( 	Rect *inRect,					
							          			SInt16 inTopOffset,
							           			SInt16 inLeftOffset,
							           			SInt16 inBottomOffset,
							           			SInt16 inRightOffset )
{

	Point startPoint = { inRect->bottom - 1 - inBottomOffset,  
								inRect->left + inLeftOffset };
	Point endPoint = { 	inRect->bottom - 1 - inBottomOffset, 
								inRect->right - 1 - inRightOffset };
	Point continuePoint = { inRect->top + inTopOffset, 
									inRect->right - 1 - inRightOffset };
	
	// е Draw a line along the bottom of the rectangle
	DrawLine ( startPoint, endPoint);
	
	// е Then continue up the right edge of the rectangle
	ContinueLine ( continuePoint );

}	// UGraphicsUtilities::BottomRightSide



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::BottomRightSideWithNotch
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::BottomRightSideWithNotch ( 	Rect *inRect,					
							          						SInt16 inTopOffset,
							           						SInt16 inLeftOffset,
							           						SInt16 inBottomOffset,
							           						SInt16 inRightOffset,
							           						SInt16 inNotch )
{

	Point startPoint = { inRect->bottom - 1 - inBottomOffset,  
								inRect->left + inLeftOffset };
	Point endPoint = { 	inRect->bottom - 1 - inBottomOffset, 
								inRect->right - 1 - inRightOffset - inNotch };
	Point continuePointUpNotch = { 	inRect->bottom - 1 - inBottomOffset - inNotch, 
												inRect->right - 1 - inRightOffset - inNotch };
	Point continuePointAlongNotch = { 	inRect->bottom - 1 - inBottomOffset - inNotch, 
													inRect->right - 1 - inRightOffset };
	Point continueToEndPoint = { 	inRect->top + inTopOffset, 
										inRect->right - 1 - inRightOffset };
	
	// е Draw a line along the bottom of the rectangle till we reach the notch
	DrawLine ( startPoint, endPoint);
	
	// е Then continue up the right edge of the notch
	ContinueLine ( continuePointUpNotch );

	// е Then continue up the top edge of the notch
	ContinueLine ( continuePointAlongNotch );

	// е Then continue up the right edge of the rectangle
	ContinueLine ( continueToEndPoint );

}	// UGraphicsUtilities::BottomRightSideWithNotch



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::TopLeftSide
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::TopLeftSide ( 	Rect *inRect,					
							          		SInt16 inTopOffset,
							           		SInt16 inLeftOffset,
							           		SInt16 inBottomOffset,
							           		SInt16 inRightOffset )
{

	Point startPoint = { inRect->bottom - 1 - inBottomOffset, 
								inRect->left + inLeftOffset };
	Point endPoint = { 	inRect->top + inTopOffset, 
								inRect->left + inLeftOffset };
	Point continuePoint = { inRect->top + inTopOffset, 
									inRect->right - 1 - inRightOffset };
	
	// е Draw a line along the left edge of the rectangle
	DrawLine ( startPoint, endPoint);
	
	// е Then continue along the top edge of the rectangle
	ContinueLine ( continuePoint );

}	// UGraphicsUtilities::TopLeftSide



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::NoCornerPixelFrame
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::NoCornerPixelFrame ( Rect *inRect )
{

	// е The same thing can be done by calling FrameRoundRect
	// which is probably a lot mor efficient
	::FrameRoundRect ( inRect, 4, 4 );
	
}	// UGraphicsUtilities::NoCornerPixelFrame



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::AngledCornerFrame
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::AngledCornerFrame ( 	Rect 		*inRect,
														RGBColor	*inColorArray,
														SInt16		inCornerOffset )
{

	// е Setup some edge values
	SInt16		left = inRect->left;
	SInt16		top = inRect->top;
	SInt16		right = inRect->right;
	SInt16		bottom = inRect->bottom;
	
	// е Setup some points for drawing routine
	Point 	startPoint;
	Point 	endPoint;
	
	// е DRAW LINES
	::RGBForeColor ( &inColorArray[0] );
	// е Draw a line along the left edge of the rectangle
	startPoint.h = left;
	startPoint.v = bottom - (inCornerOffset + 1);
	endPoint.h = left;
	endPoint.v = top + inCornerOffset;
	DrawLine ( startPoint, endPoint);
	
	// е Draw a line along the top edge
	startPoint.h = left + inCornerOffset;
	startPoint.v = top;
	endPoint.h = right - (inCornerOffset + 1);
	endPoint.v = top;
	DrawLine ( startPoint, endPoint );

	// е Draw a line along the right edge
	startPoint.h = right - 1;
	startPoint.v = top + inCornerOffset;
	endPoint.h = right - 1;
	endPoint.v = bottom - (inCornerOffset + 1);
	DrawLine ( startPoint, endPoint );

	// е Draw a line along the bottom edge
	startPoint.h = left + inCornerOffset;
	startPoint.v = bottom - 1;
	endPoint.h = right - (inCornerOffset + 1);
	endPoint.v = bottom - 1;
	DrawLine ( startPoint, endPoint );

	// е CORNER PIXELS
	SInt16		pixelCount = inCornerOffset + 1;
	for ( SInt16 index = 0; index < pixelCount; index++ )
	{
		// е TOP LEFT CORNER
		PaintColorPixel ( left + index, 
								top + (inCornerOffset - index),
							 	&inColorArray[index + 1] );
		
		// е TOP RIGHT CORNER
		PaintColorPixel ( right - (1 + index), 
								top + (inCornerOffset - index), 
								&inColorArray[index + 1] );
	
		// е BOTTOM RIGHT CORNER
		PaintColorPixel ( right - (1 + index), 
								bottom - (1 + (inCornerOffset - index)), 
								&inColorArray[index + 1] );
							
		// е BOTTOM LEFT CORNER
		PaintColorPixel ( left + index, 
								bottom - (1 + (inCornerOffset - index)), 
								&inColorArray[index + 1] );
	}
	
}	// UGraphicsUtilities::AngledCornerFrame



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::BevelledEdge
//-------------------------------------------------------------------------------------
//
//	This function is used to draw bevelled edges on either the top left or bottom right
//		edges of a rectangle.  These edges can be any width with a color being specified
//		for each edge.

void
UGraphicsUtilities::BevelledEdge	(	Rect			*inRect,
												ERectEdge 	inRectEdge,
												RGBColor		*inColorArray,
												SInt16			inEdgeWidth )
{

	SInt16		counter;

	switch ( inRectEdge )
	{
		case rectEdge_TopLeft:
			for ( counter = 0; counter < inEdgeWidth; counter++ )
			{
				// е Frame the bottom right edge of the rectangle
				::RGBForeColor ( &inColorArray [counter] );
				TopLeftSide ( inRect, 0, 0, 0, 0 );
				
				// е If we are an edge that is wider than one pixel then
				// we need to inset the rect
				if ( inEdgeWidth > 1 )
					::InsetRect ( inRect, 1, 1 );
			}
			
			// е Fix the rect back up if the edge width is greater than one
			if ( inEdgeWidth > 1 )
				::InsetRect ( inRect, -inEdgeWidth, -inEdgeWidth );
				
		break;
		
		case rectEdge_BottomRight:
			for ( counter = 0; counter < inEdgeWidth; counter++ )
			{
				// е Frame the bottom right edge of the rectangle
				::RGBForeColor ( &inColorArray [counter] );
				BottomRightSide ( inRect, 1, 1, 0, 0 );
				
				// е If we are an edge that is wider than one pixel then
				// we need to inset the rect
				if ( inEdgeWidth > 1 )
					::InsetRect ( inRect, 1, 1 );
			}
			
			// е Fix the rect back up if the edge width is greater than one
			if ( inEdgeWidth > 1 )
				::InsetRect ( inRect, -inEdgeWidth, -inEdgeWidth );
				
		break;
		
		default:
		break;
		
	}
	
}	// UGraphicsUtilities::BevelledEdge



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::PaintCornerPixels
//-------------------------------------------------------------------------------------
//
//	This function is used to paint in the corner pixels of a rectangle, the corner to be
//		painted is specified by an enum for each of the corners.  The function supports
//		painting up to two pixels and allows spearate colors to be specifed for each of
//		these pixels.

void
UGraphicsUtilities::PaintCornerPixels	(	Rect			*inRect,
														ERectCorner	inCornerToPaint,
														RGBColor		*inCornerColorArray,
														SInt16			inEdgeWidth )
{

	SInt16		counter;

	switch ( inCornerToPaint )
	{
		// е TOP LEFT CORNER
		case rectCorner_TopLeft:
			for ( counter = 0; counter < inEdgeWidth; counter++ )
			{
				// е Paint the top left pixel
				PaintColorPixel ( 	inRect->left, 
									inRect->top, 
									&inCornerColorArray [counter] );
				
				// е If we are a corner that is wider than one pixel then
				// we need to inset the rect
				if ( inEdgeWidth > 1 )
					::InsetRect ( inRect, 1, 1 );
			}
			
			// е Fix the rect back up if the width is greater than one
			if ( inEdgeWidth > 1 )
				::InsetRect ( inRect, -inEdgeWidth, -inEdgeWidth );
				
		break;
	
		// е TOP RIGHT CORNER
		case rectCorner_TopRight:
			for ( counter = 0; counter < inEdgeWidth; counter++ )
			{
				// е Paint the top right pixel
				PaintColorPixel ( 	inRect->right - 1, 
									inRect->top, 
									&inCornerColorArray [counter] );
				
				// е If we are a corner that is wider than one pixel then
				// we need to inset the rect
				if ( inEdgeWidth > 1 )
					::InsetRect ( inRect, 1, 1 );
			}
			
			// е Fix the rect back up if the width is greater than one
			if ( inEdgeWidth > 1 )
				::InsetRect ( inRect, -inEdgeWidth, -inEdgeWidth );
				
		break;
	
		// е BOTTOM RIGHT CORNER
		case rectCorner_BottomRight:
			for ( counter = 0; counter < inEdgeWidth; counter++ )
			{
				// е Paint the bottom right pixel
				PaintColorPixel ( 	inRect->right - 1, 
									inRect->bottom - 1, 
									&inCornerColorArray [counter] );
				
				// е If we are a corner that is wider than one pixel then
				// we need to inset the rect
				if ( inEdgeWidth > 1 )
					::InsetRect ( inRect, 1, 1 );
			}
			
			// е Fix the rect back up if the width is greater than one
			if ( inEdgeWidth > 1 )
				::InsetRect ( inRect, -inEdgeWidth, -inEdgeWidth );
				
		break;
	
		// е BOTTOM LEFT CORNER
		case rectCorner_BottomLeft:
			for ( counter = 0; counter < inEdgeWidth; counter++ )
			{
				// е Paint the bottom left pixel
				PaintColorPixel ( 	inRect->left, 
									inRect->bottom - 1, 
									&inCornerColorArray [counter] );
				
				// е If we are a corner that is wider than one pixel then
				// we need to inset the rect
				if ( inEdgeWidth > 1 )
					::InsetRect ( inRect, 1, 1 );
			}
			
			// е Fix the rect back up if the width is greater than one
			if ( inEdgeWidth > 1 )
				::InsetRect ( inRect, -inEdgeWidth, -inEdgeWidth );
				
		break;
		
		default:
		break;
	
	}
	
}	// UGraphicsUtilities::PaintCornerPixels



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::RoundCornerEdges
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::RoundCornerEdges	(	Rect			*inRect,
														ERectEdge	inRectEdge,
														RGBColor		*inEdgeColor )
{
	
	Point startPoint, endPoint;

	// е Get the foreground color setup
	::RGBForeColor ( inEdgeColor );
	
	switch ( inRectEdge )
	{
		case rectEdge_TopLeft:
			startPoint.h = inRect->left + 1;
			startPoint.v = inRect->bottom - 5;
			endPoint.h = inRect->left + 1;
			endPoint.v = inRect->top + 3;
       	DrawLine ( startPoint, endPoint );
       	
 			endPoint.h = inRect->left + 2;
			endPoint.v = inRect->top + 3;
       	ContinueLine ( endPoint );
       	
 			endPoint.h = inRect->left + 2;
			endPoint.v = inRect->top + 2;
	      ContinueLine ( endPoint );
	      
 			endPoint.h = inRect->left + 3;
			endPoint.v = inRect->top + 2;
	      ContinueLine ( endPoint );
	      
 			endPoint.h = inRect->left + 3;
			endPoint.v = inRect->top + 1;
       	ContinueLine ( endPoint );
       	
 			endPoint.h = inRect->right - 4;
			endPoint.v = inRect->top + 1;
       	ContinueLine ( endPoint );
		break;
		
		case rectEdge_BottomRight:
			startPoint.h = inRect->left + 3;
			startPoint.v = inRect->bottom - 2;
			endPoint.h = inRect->right - 4;
			endPoint.v = inRect->bottom - 2;
       	DrawLine ( startPoint, endPoint );
       	
 			endPoint.h = inRect->right - 4;
			endPoint.v = inRect->bottom - 3;
       	ContinueLine ( endPoint );
       	
 			endPoint.h = inRect->right - 3;
			endPoint.v = inRect->bottom - 3;
	      ContinueLine ( endPoint );
	      
 			endPoint.h = inRect->right - 3;
			endPoint.v = inRect->bottom - 4;
	      ContinueLine ( endPoint );
	      
 			endPoint.h = inRect->right - 2;
			endPoint.v = inRect->bottom - 4;
       	ContinueLine ( endPoint );
       	
 			endPoint.h = inRect->right - 2;
			endPoint.v = inRect->top + 3;
       	ContinueLine ( endPoint );
		break;
		
		default:
		break;
		
	}
	
}	// UGraphicsUtilities::RoundCornerEdges



#pragma mark -
#pragma mark === GRAPHICS FUNCTIONS

//=====================================================================================
// ее GRAPHICS FUNCTIONS
//-------------------------------------------------------------------------------------
// UGraphicsUtilities::DrawLine
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::DrawLine (  	Point inStartLoc,
            							Point inEndLoc )
{
  // е Move to the starting location then draw the line to
  // the end location
  ::MoveTo ( inStartLoc.h, inStartLoc.v );
  ::LineTo ( inEndLoc.h, inEndLoc.v );
  
}	// UGraphicsUtilities::DrawLine



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::ContinueLine
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::ContinueLine (  Point inEndLoc )
{
  // е Continue drawing the line from the current location of the
  // pen to the end location
  ::LineTo ( inEndLoc.h, inEndLoc.v );

}	// UGraphicsUtilities::ContinueLine



//-------------------------------------------------------------------------------------
// UGraphicsUtilities::PaintColorPixel
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::PaintColorPixel ( 	SInt16			inHorizLoc,
													SInt16			inVertLoc,
													const RGBColor* 	inColor )
{
  
  	// е We willsimply paint the pixel by calling LineTo
	::RGBForeColor ( inColor );
	::MoveTo ( inHorizLoc, inVertLoc );
	::LineTo ( inHorizLoc, inVertLoc );
	
}	// UGraphicsUtilities::PaintColorPixel



#pragma mark -
#pragma mark === MISCELLANEOUS FUNCTIONS

//=====================================================================================
// ее MISCELLANEOUS FUNCTIONS
//-------------------------------------------------------------------------------------
// UGraphicsUtilities::LocateDeviceRect
//-------------------------------------------------------------------------------------
//

Rect
UGraphicsUtilities::LocateDeviceRect (  Point inMouseLoc )
{
  
  GDHandle	aDevice = GetMainDevice ();
  
  while ( aDevice != nil )
  {
		Rect 		deviceRect = (**aDevice).gdRect;
  		if ( PtInRect ( inMouseLoc, &deviceRect ))
  			return deviceRect;
  			
  		aDevice = GetNextDevice ( aDevice );
  }
  
  return gEmptyRect;

}	// UGraphicsUtilities::LocateDeviceRect


//-------------------------------------------------------------------------------------
// UGraphicsUtilities::RangeRandom
//-------------------------------------------------------------------------------------
//

UInt16
UGraphicsUtilities::RangeRandom ( 	UInt16 inMin,
												UInt16 inMax )
{
	
	UInt16	rand;		// Treat the return value as 0 - 65536
	SInt32		range, temp;
	
	rand = ::Random ();
	range = inMax - inMin;
	temp = (rand * range ) / 65536;
	
	return temp + inMin;
	
}	// UGraphicsUtilities::RangeRandom


//-------------------------------------------------------------------------------------
// UGraphicsUtilities::SlowDown
//-------------------------------------------------------------------------------------
//

void
UGraphicsUtilities::SlowDown ( 	UInt32 inDelayTicks )
{

	unsigned long dummy;
	::Delay ( inDelayTicks, &dummy );
	
}	// UGraphicsUtilities::SlowDown


//-------------------------------------------------------------------------------------
// UGraphicsUtilities::CreateGrafPort 
//-------------------------------------------------------------------------------------

GrafPtr 
UGraphicsUtilities::CreateGrafPort ( Rect &inBounds )
{
	GrafPtr	savedPort;		// Saved GrafPtr for later restore
	GrafPtr	newPort;			// New GrafPort
	Rect	localBounds;		// Local copy of bounds

	::GetPort ( &savedPort );

	// е Set the top-left corner of bounds to (0,0)
	localBounds = inBounds;
	::OffsetRect ( &localBounds, -inBounds.left, -inBounds.top );

	// е Allocate a new GrafPort
#if PP_Target_Carbon
	newPort = CreateNewPort();
	
	if ( newPort != nil )
	{
		::SetPortBounds(newPort, &localBounds);
		::EraseRect ( &localBounds );
	}
#else
	newPort = (GrafPtr)::NewPtrClear ( sizeof( GrafPort ) );
	
	if ( newPort != nil )
	{
		// е Initialize the new port and make the current port
		::OpenPort( newPort );

		// е Initialize and allocate the bitmap
		newPort->portBits.bounds = localBounds;
  		newPort->portBits.rowBytes = ((localBounds.right + 15) >> 4) << 1;
		newPort->portBits.baseAddr =  NewPtrClear( newPort->portBits.rowBytes *
													(long)localBounds.bottom );
		if ( newPort->portBits.baseAddr != nil )
		{
			// е Clean up the new port
			newPort->portRect = localBounds;
			::ClipRect ( &localBounds );
			::RectRgn ( newPort->visRgn, &localBounds );
			::EraseRect ( &localBounds );
		}
		else
		{
			// е Allocation failed; deallocate the port
			::ClosePort ( newPort );
			::DisposePtr ( (Ptr)newPort );
			newPort = nil;
		}
	}
#endif
	
	::SetPort ( savedPort );
	return newPort;
	
}	//	UGraphicsUtilities::CreateGrafPort


//-------------------------------------------------------------------------------------
// UGraphicsUtilities::DisposeGrafPort 
//-------------------------------------------------------------------------------------

void 
UGraphicsUtilities::DisposeGrafPort ( GrafPtr inDoomedPort )
{

#if PP_Target_Carbon
	::DisposePort(inDoomedPort);
#else
	::ClosePort ( inDoomedPort );
	::DisposePtr ( inDoomedPort->portBits.baseAddr );
	::DisposePtr ( (Ptr)inDoomedPort );
#endif
	
}	//	UGraphicsUtilities::DisposeGrafPort


#pragma mark -
#pragma mark === CLASS: UStringDrawing

//=====================================================================================
// ее USTRINGDRAWING CLASS
//-------------------------------------------------------------------------------------
// UStringDrawing::DrawJustifiedString 
//-------------------------------------------------------------------------------------

void 
UStringDrawing::DrawJustifiedString	(	const Str255 	&inString,
											  		const Rect 		&inRect,
											  		SInt16			 	inJustification,
													Boolean			inTruncateString,
											  		Boolean 			inPreferOutline )
{

	FontInfo theFontInfo;
	SInt16 	widthOfString;
	SInt16	 	boxWidth;
	Rect 		localBox = inRect;
	Str255	localString;
	
	// е Make a copy of the string we were handed
	LString::CopyPStr ( inString, localString );
	
	// е Setup the width of the rect
	boxWidth = UGraphicsUtilities::RectWidth ( localBox );
	
	// е Setup whether TrueType is preferred over bitmap
	::SetOutlinePreferred ( inPreferOutline );

	// е Setup the font info and then we need to check to see if we need
	// to truncate the string, if we do we call the toolbox function that
	// handles that even though it is slower than our function.
	::GetFontInfo ( &theFontInfo );
	widthOfString = ::StringWidth ( localString );
	if ( inTruncateString && (widthOfString > boxWidth)) 
	{
		// е Get the string truncated which we will do on the end
		::TruncString ( boxWidth, localString, truncEnd );
		
		// е Update the string width to that of the truncated string
		widthOfString = ::StringWidth ( localString );
	}
	
	// е If the width of the string is less than the rect we are drawing
	// into then we proceeed to setupthe justification other wise it will
	// just be drawn at the beginning of the rect
	if ( widthOfString < boxWidth )
	{
		switch ( GetActualJustification ( inJustification ))
		{
			case teCenter:
				localBox.left += (boxWidth - widthOfString) / 2;
				break;

			case teFlushRight:
				localBox.left += boxWidth - widthOfString;
				break;

			case teFlushDefault:
			case teFlushLeft:
				break;
		}
	}

	//	е Do the actual drawing
	::MoveTo ( localBox.left, localBox.top + theFontInfo.ascent );
	::DrawString ( localString );


} // UStringDrawing::DrawJustifiedString 


