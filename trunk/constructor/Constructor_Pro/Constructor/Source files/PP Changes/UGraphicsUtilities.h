// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/* Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>-----------------------------------------

FILE:					UGraphicsUtilities.h

CLASSES:				UGraphicsUtilities & UStringDrawing

AUTHOR:				Robin Mair

CREATION DATE :	95.10.07

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

A collection of useful utilities that are used throughout the grayscale classes.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_UGraphicsUtilities
	#define _H_UGraphicsUtilities
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <PP_Prefix.h>
#include <UGraphicUtils.h>		// PP CHANGE ADDED

// ¥ TOOLBOX HEADERS
#include <TextEdit.h>
#include <Fonts.h>


//==================================================================================== 
//	DEFINES
//==================================================================================== 

#define	SLOW_DRAWING	1
#define	NORMAL_DRAWING	0

//==================================================================================== 
//	TYPES
//==================================================================================== 

enum ERectCorner
{
	rectCorner_None,
	rectCorner_TopLeft,
	rectCorner_TopRight,
	rectCorner_BottomRight,
	rectCorner_BottomLeft
};

enum ERectEdge
{
	rectEdge_None,
	rectEdge_TopLeft,
	rectEdge_BottomRight
};

enum EControlMode
{
	controlMode_None,
	controlMode_Button,
	controlMode_RadioButton,
	controlMode_Switch
};


//====================================================================================
//	GLOBAL VARIABLES
//====================================================================================

extern	Rect					gEmptyRect;				//	Handy emtpy rectangle
extern	Point					gEmptyPoint;			//	Handy empty point
extern	Str255				gEmptyString;			//	Handy empty string

//==================================================================================== 
//	INLINES
//==================================================================================== 

//==================================================================================== 
//	MATH FUNCTIONS

//	¥ Inlines for finding the min or max of two longs
inline long lMax ( long a, long b ) { return a > b ? a : b; }
inline long lMin ( long a, long b ) { return a < b ? a : b; }

// ¥ Our very own abs function so we don't need to depend on the standard library
// for it
inline SInt16 absolute ( SInt16 n )
{
	if ( n < 0 )
		return ( -n );
	else
		return ( n );
}


#if 0	// PP CHANGE: Duplicate defs in UGraphicUtils.h
//==================================================================================== 
//	RGBCOLOR COMPARISON OPERATORS

inline Boolean	operator == ( 	const RGBColor &inColorOne,
										const RGBColor &inColorTwo )
{
	return inColorOne.red == inColorTwo.red && 
				inColorOne.green == inColorTwo.green && 
					inColorOne.blue == inColorTwo.blue;
}

inline Boolean	operator != ( 	const RGBColor &inColorOne,
										const RGBColor &inColorTwo )
{
	return !( inColorOne == inColorTwo );
}


//==================================================================================== 
//	POINT COMPARISON OPERATORS

inline Boolean	operator == ( 	const Point inPointOne,
										const Point inPointTwo )
{
	return inPointOne.h == inPointTwo.h && 
				inPointOne.v == inPointTwo.v;
}


inline Boolean	operator != ( 	const Point inPointOne,
										const Point inPointTwo )
{
	return !( inPointOne.h == inPointTwo.h && 
					inPointOne.v == inPointTwo.v );
}
#endif		// PP CHANGE


//==================================================================================== 
//	CLASS: UGraphicsUtilities
//==================================================================================== 

class	UGraphicsUtilities 
{

protected:

	static	Boolean	sHasAaron;				//	Is Aaron running on the machine
	static	Boolean	sHasMacOS8;				//	Is Mac OS 8 running on the machine
	static	Boolean	sHasAppearance;		//	Is Appearance Manager running on the 
														//		machine
	static	Boolean	sInAppearanceCompatMode; 	//	Is the appearance manager running
																//		in compatibility mode
																	
	
public:

	//----<< ¥ COLOR MANIPULATION ¥ >>-------------------------------------------------

	static	void		 	InitGraphicsUtilities 	();


	//----<< ¥ COLOR MANIPULATION ¥ >>-------------------------------------------------

	static	RGBColor 	Darken 	( RGBColor *inColor );
	static	RGBColor 	Lighten 	( RGBColor *inColor );


	//----<< ¥ GRAPHICAL FUNCTIONS ¥ >>------------------------------------------------

	static	void 			DrawLine 			(  Point inStartLoc,
					            						Point inEndLoc );
	static	void 			ContinueLine 		(  Point inEndLoc );
	static	void 			PaintColorPixel 	( 	SInt16			inHorizLoc,
															SInt16			inVertLoc,
															const RGBColor* 	inColor );


	//----<< ¥ SPECIALIZED GRAPHICAL FUNCTIONS ¥ >>------------------------------------

	static	void			BottomRightSide 	( 	Rect *inRect,					
									          			short inTopOffset,
									           			short inLeftOffset,
									           			short inBottomOffset,
									           			short inRightOffset );
	static	void			BottomRightSideWithNotch ( Rect *inRect,					
											          			short inTopOffset,
											           			short inLeftOffset,
											           			short inBottomOffset,
											           			short inRightOffset,
											           			short	inNotch );
	static	void			TopLeftSide 		( 	Rect *inRect,					
								          				short inTopOffset,
								           				short inLeftOffset,
								           				short inBottomOffset,
								           				short inRightOffset );
     
	static	void			AngledCornerFrame ( 	Rect 		*inRect,
															RGBColor	*inColorArray,
															SInt16		inCornerOffset );
										      									
	static	void			NoCornerPixelFrame	(	Rect	*inRect );

	static	void			BevelledEdge			(	Rect			*inRect,
																ERectEdge 	inRectEdge,
																RGBColor		*inColorArray,
																SInt16			inEdgeWidth = 2 );

	static	void			PaintCornerPixels		(	Rect			*inRect,
																ERectCorner	inCornerToPaint,
																RGBColor		*inCornerColorArray,
																SInt16			inEdgeWidth = 2 );

	static	void			RoundCornerEdges		(	Rect			*inRect,
																ERectEdge	inRectEdge,
																RGBColor		*inEdgeColor );

	//----<< ¥ MISCELLANEOUS ¥ >>-----------------------------------------------------

	static	Boolean		HasAaron					()
								{	return sHasAaron; }

	static	Boolean		HasAppearance			()
								{	return sHasAppearance; }

	static	Boolean		HasAppearanceCompatMode		()
								{	return sInAppearanceCompatMode; }

	static	Boolean		IsAppearanceRunning	()
								{	return HasAppearance () && !HasAppearanceCompatMode (); }

	static	Boolean		HasMacOS8				()
								{	return sHasMacOS8; }

	static	Rect			LocateDeviceRect		(	Point	inMouseLoc );

	static	SInt16			RectWidth				( 	Rect &inRect )
								{	return inRect.right - inRect.left; }
	static	SInt16			RectHeight				(	Rect &inRect )
								{	return inRect.bottom - inRect.top; }
	static	UInt16		RangeRandom 			( 	UInt16 inMin,
																UInt16 inMax );
	static	void			SlowDown 				( 	UInt32 inDelayTicks );		
	
	
	static	GrafPtr		CreateGrafPort 		( Rect 	&inBounds );	
	static	void			DisposeGrafPort		( GrafPtr inDoomedPort );
	
				
};	//	UGraphicsUtilities


//==================================================================================== 
//	CLASS: UStringDrawing
//==================================================================================== 


class	UStringDrawing 
{

public:

	static	 void	DrawJustifiedString		(	const Str255 	&inString,
													  		const Rect 		&inRect,
													  		SInt16			 	inJustification,
															Boolean			inTruncateString = true,
													  		Boolean 			inPreferOutline = false );
											  		
	static 	SInt16	GetActualJustification	( SInt16	inJustification )
		{ return inJustification == teFlushDefault ? ::GetSysDirection () : inJustification; }
																	
};
		

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
