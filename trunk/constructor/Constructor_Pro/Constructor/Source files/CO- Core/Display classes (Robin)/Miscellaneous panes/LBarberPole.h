// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					LBarberPole.h

CLASSES:				LBarberPole

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.24
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION

This class implements a simple barber pole progress indicator that can be used in
when the progress info is indeterminate.  It works by animate a single PICT either
vertically or horizontally depending on the size of the pane where it is horizontal
if it is wider than it is high.

In order for this to work correctly the PICT that is being used to provide the barber
pole at least twice the dimension of the pole less the insets for the desired 
orientation direction.

In addition to specifying the PICT you can also specify what the animation step will
be, this is simply the amount the PICT will be moved for each cycle.

The barber pole also supports being able to draw an adornment around the pole, there
is a separate field that is used to specify the inset for this adornment, which
is currently only supported as a single inset for all edges.  By default this class
frames the pole with a black frame, therefore the default inset is 1 pixel.

Currently, this class only supports moving the PICT down for the vertical case and
to the right for the horizontal case.


---------------------------------------<< * >>----------------------------------------
*/

#ifndef _H_LBarberPole
	#define _H_LBarberPole
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// * POWERPLANT HEADERS
#include <PP_Types.h>
#include <LPane.h>
#include <LPeriodical.h>

// * TOOLBOX HEADERS
#ifndef __TYPES__
	#include <Types.h>
#endif

#ifndef __QUICKDRAW__
	#include <Quickdraw.h>
#endif

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	TYPES
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class	LPane;
class	LPeriodical;

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	LBarberPole
//==================================================================================== 
	
class LBarberPole : 	public LPane,
							public LPeriodical
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'brbp' };
	
		
	//====<< * FIELDS * >>=============================================================

protected:

	ResIDT			mPictureID;			//	Resource ID for the picture that will be used
												//		render the barber pole
	Int16				mAdornmentInset;	//	This is the amount of inset that is allowed for
												//		the adornment, it is also used to determine
												//		the clipping that will be used for drawing
												//		the picture
	Int16				mAnimationStep;	//	This is the amount of that the picture will be
												//		moved each time we animate it
	Int32				mNextTime;
	Int16				mIndicatorSpeed;	//	The frequency with which the PICTs are moved

private:

	PicHandle		mPictureH;			//	Cached handle to the picture
	Int16				mLastLoc;			//	the last position of the pict when it was last
												//		moved, this value is either the top or left
												//		value that is used to position the PICT
												//		based on whether the pole is vertical or
												//		horizontal
												
																								
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

					LBarberPole 		();								//	* Constructor
					LBarberPole 		(	LStream*	inStream );		//	* Constructor
	virtual		~LBarberPole 		();								//	* Desctructor
									
	static	LBarberPole*	CreateLBarberPoleStream	(	LStream*	inStream );
	
				void		FinishCreateSelf	();										//	* OVERRIDE
				
				
	//----<< * ACCESSORS * >>----------------------------------------------------------

	virtual	Boolean	IsHorizontal		();
	
	virtual	Int16		CalcInitialLocation		();
	
	virtual	void		CalcPictureClippingRect	(	Rect	&outPicRect );
	virtual	void		CalcPictureFrame			(	Rect	&outPicFrame );
	
	
	//----<< * SPEND TIME * >>---------------------------------------------------------

	virtual	void		SpendTime			( const EventRecord &inMacEvent );
											
											
	//----<< * DRAWING * >>------------------------------------------------------------

protected:

	virtual	void		DrawSelf							();							//	* OVERRIDE

	virtual	void		DrawBarberPoleAdornment		();				

	virtual	void		DrawBarberPole					();
	

};	// LBarberPole

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif