// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SnapTo.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTSnapTo

DESCRIPTION:		Implementation file for Object
						
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		97.01.26	ebs		included fp.h instead of math.h (for pi)
		96.07.02	ebs		Added SnapTo() routine
		96.04.18	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTSnapTo.h"
#include "PTActions.h"
#include "SUMiscUtils.h"				// for IsShiftKey()
//#include <math.h>
#include <fp.h>

#define pi 3.141592653589793238462643

/*==================================
	PTSnapTo Constructor
===================================*/
PTSnapTo::PTSnapTo()
{
	mSnappingHorizontal = false;
	mHaveMovedSinceMouseDown = false;
	mSnapAngle = kSnapToRect;
}

/*==================================
	PTSnapTo::SetAngle
===================================*/
void PTSnapTo::SetAngle( ESnapTo inAngle )
{
	mSnapAngle = inAngle;
}

/*==================================
	PTSnapTo::SnapTo
	
	Note:
	This routine maxes out at 32K because we use Point structures in
	the call. We should probably fix this, but since QuickDraw maxes
	out at 32K anyway, this isn't a big deal.
===================================*/
void PTSnapTo::SnapTo( 	SInt32 origH, SInt32 origV, SInt32 newH, SInt32 newV, 
						SInt32 *outH, SInt32 *outV )
{
	Point	origPt = { origV, origH };
	Point	newPt =  { newV, newH };
	
	this->SnapToPoint( origPt, newPt, &newPt );
	*outH = newPt.h;
	*outV = newPt.v;
}

/*==================================
	PTSnapTo::SnapToPoint
===================================*/
void PTSnapTo::SnapToPoint( Point startPt, Point currentPt, Point *outPt )
{
	*outPt = currentPt;
	
	switch( mSnapAngle )
	{
		case kSnapTo45:
			this->SnapTo45( startPt, currentPt, outPt );
			break;
			
		case kSnapTo90:
			this->SnapTo90( startPt, currentPt, outPt );
			break;
			
		case kSnapToRect:
			this->SnapToRect( startPt, currentPt, outPt );
			break;
		
		default:
			break;
	}
}

/*==================================
	PTSnapTo::SnapTo90
	
	This is used by the pen, eraser, and move tools.
	These tools snap to a vertical or horizontal line, but only if the
	  shift key was down at the initial mouseDown event.
===================================*/
void PTSnapTo::SnapTo90( Point startPt, Point currentPt, Point *outPt )
{
	*outPt = currentPt;					// assume no change
	
	//if ( !SUMiscUtils::IsShiftKey() ) return;		// we go by initial click only

	if ( !mHaveMovedSinceMouseDown )
	{
		if ( EqualPt( startPt, currentPt ) )
			return;
			
			// see which direction we've moved in most
		if ( ABS(startPt.h - currentPt.h) >= ABS(startPt.v - currentPt.v) )
			mSnappingHorizontal = true;
		else
			mSnappingHorizontal = false;
						
		mHaveMovedSinceMouseDown = true;
	}
	
	if ( mSnappingHorizontal )
		outPt->v = startPt.v;
	else
		outPt->h = startPt.h;
}


/*==================================
	PTSnapTo::SnapToRect
	
	This is used by the rectangle tools (rect, oval, roundrect) and the
   	  rectangular selection tool.
===================================*/
void PTSnapTo::SnapToRect( Point startPt, Point currentPt, Point *outPt )
{
	/********************************************
		assume no change in point
	********************************************/
	*outPt = currentPt;
	
	if ( !SUMiscUtils::IsShiftKey() ) return;

	/********************************************
		see how far we've moved in each direction
	********************************************/
	long	dh = currentPt.h - startPt.h;
	long	dv = currentPt.v - startPt.v;
	long	absDH = ABS( dh );
	long	absDV = ABS( dv );
	
	/********************************************
		use the bigger of the width or height
	********************************************/
	if ( absDH > absDV )
	{
		if ( dv > 0 )
			outPt->v = startPt.v + absDH;
		else
			outPt->v = startPt.v - absDH;
	}
	else
	{
		if ( dh > 0 )
			outPt->h = startPt.h + absDV;
		else
			outPt->h = startPt.h - absDV;
	}
}

/*==================================
	PTSnapTo::SnapTo45
	
	Snaps to a 45 degree angle if the shift key is down.
	This is used by the Line tool.
===================================*/
void PTSnapTo::SnapTo45( Point startPt, Point currentPt, Point *outPt )
{
	/********************************************
		assume no change in point
	********************************************/
	*outPt = currentPt;
	
	if ( !SUMiscUtils::IsShiftKey() ) return;
	
	/********************************************
		radian/degree conversions (I like degrees, C library likes radians)
	********************************************/
	#define TO_RAD( deg )	( (deg) / 180.0 * pi )
	#define TO_DEG( rad )	( (rad) / pi * 180.0 )

	/********************************************
		see how far we've moved in each direction
	********************************************/
	long	dh = currentPt.h - startPt.h;
	long	dv = startPt.v - currentPt.v;		// opposite since plus is down, not up
	
	/********************************************
		no need for any calculations if we're already at 90 degrees
	********************************************/
	if ( (dh == 0) || (dv == 0) ) 
		return;

	/********************************************
		use whichever coordinate is longer as the h/v length
	********************************************/
	long	maxHV = MAX( ABS(dh), ABS(dv) );
	
	/********************************************
		get the angle between the starting point and the current point
		 -- force it positive if it is less than zero (easier to deal with)
	********************************************/
	double angle = atan2( (double)dv, (double)dh );		// returns value between +- pi
	
	if ( angle < 0 )
		angle += 2 * pi;				// same angle, just 360 degrees bigger
		
	/********************************************
		round angle to nearest 45 degrees
	********************************************/
	const double	k22inRads = TO_RAD( 22.5 );
	const double	k45inRads = TO_RAD( 45.0 );
	
	angle = TO_DEG( k45inRads * floor( (angle + k22inRads) / k45inRads ) );
	
	/********************************************
		fix the coordinates depending on which of 8 angles we are gridding to
	********************************************/
	switch( (long) (angle + 0.5) )
	{
		case 0:
		case 360:
			outPt->v = startPt.v;
			break;
		case 45:
			outPt->v = startPt.v - maxHV;
			outPt->h = startPt.h + maxHV;
			break;
		case 90:
			outPt->h = startPt.h;
			break;
		case 135:
			outPt->v = startPt.v - maxHV;
			outPt->h = startPt.h - maxHV;
			break;
		case 180:
			outPt->v = startPt.v;
			break;
		case 225:
			outPt->v = startPt.v + maxHV;
			outPt->h = startPt.h - maxHV;
			break;
		case 270:
			outPt->h = startPt.h;
			break;
		case 315:
			outPt->v = startPt.v + maxHV;
			outPt->h = startPt.h + maxHV;
			break;
		default:
			DebugStr( LStr255("Bad value in MapTo45") );		// ???
	}
}
