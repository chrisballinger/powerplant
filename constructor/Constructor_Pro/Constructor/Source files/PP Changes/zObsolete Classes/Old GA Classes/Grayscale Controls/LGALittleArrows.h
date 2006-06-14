// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGALittleArrows.h

CLASSES:				LGALittleArrows

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.12.08

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements the small arrows as specified by the AGA specification.  This
class supports 3 types of action when clicking on one of the arrows: 

	1) single click - only updates the value on the mouse up and only once for any 
							click.
	2) Delay/Continuous - waits for the specified number of ticks while the mouse is
							down before the value starts changing continuously.
	3)	Continuous - starts changing the value continuously when the mouse goes down in
						one of the arrows. This mode works a little differently than the
						delay continuous mode in that the value is also updated when the
						mouse goes up, this allows the value to also be changed by making
						a single click on the control.  This is likely to be the most
						useful mode.
						
The continuous actions all following the tracking, so if the mouse moves out of a 
control while it is changing it stops till the mouse moves back in.

This control allows the type of click action, the click action delay, and an update 
delay to be specified from within Constructor.  The update delay can be used to
control how fast the value changes, by default it is set to 15 ticks, which is
reasonable on a fast machine, less than that can make the value change so fast that
it is impossible to see.

When the value is being changed the min and max values are used to determine the
limits for the value, when one of the limits is reached the value simply loops around.
The value starts changing from the initial value setup in Constructor.

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGALittleArrows
	#define _H_LGALittleArrows
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <LControl.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

enum ELAClickAction
{
	clickAction_SingleClick 		= 1,
	clickAction_DelayContinuous	= 2,
	clickAction_Continuous			= 3
};

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGALittleArrows :	public LControl
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'glar' };
	

	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	ELAClickAction	mClickAction;		//	What type of response do we handle when we get
												//		a click in one of the arrows
	SInt16			mClickActionDelay;	//	The length of delay when the click action 
												//		option is clickAction_DelayContinuous
	SInt16			mUpdateDelay;			//	The length of delay used when changing values
												//		continuously, this can be used to slowdown
												//		or speed up the amount of time between 
												//		changes in the value
												
private:

	SInt32			mStartTickCount;		//	Used internally to track the tick count for the
												//		purpose of handling the delay
	SInt32			mTickCounter;			//	Used internally to handle slowing down the
												//		calls to HotSpotAction
												
																																		
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGALittleArrows 		();						//	¥ Default Constructor
				LGALittleArrows 		(	const LGALittleArrows &inOriginal );
																		//	¥ Copy Constructor
				LGALittleArrows 		( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGALittleArrows 		(	const SPaneInfo 			&inPaneInfo,
												const SControlInfo 		&inControlInfo,
												SInt16							inClickAction,
												SInt16							inClickActionDelay,
												SInt16							inUpdateDelay );
																		//	¥ Parameterized Constructor
	virtual				~LGALittleArrows 		();			//	¥ Destructor
									
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	void				CalcLocalUpArrowRect			(	Rect	&outRect ) const;
	virtual	void				CalcLocalDownArrowRect		(	Rect	&outRect ) const;
	virtual	void				CalcLocalLittleArrowRect	(	Rect	&outRect ) const;
	

	// ¥ SETTERS
																																					
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf				();							// ¥ OVERRIDE				
	virtual	void				DisableSelf				();							// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf							();					//	¥ OVERRIDE
				
protected:

	virtual	void				DrawLittleArrowsNormal		(	Boolean	inUpArrow );
	virtual	void				DrawLittleArrowsHilited		(	Boolean	inUpArrow );
	virtual	void				DrawActualArrow				(	Rect		&inRect,
																			Boolean	inUpArrow );

		
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------
				
	virtual	void				ClickSelf						( const SMouseDownEvent	&inMouseDown );
																								//	¥ OVERRIDE
	virtual 	SInt16				FindHotSpot						(	Point inPoint ) const;
			 																					//	¥ OVERRIDE
	virtual 	Boolean			PointInHotSpot					(	Point inPoint, 
			 																SInt16 inHotSpot ) const;
																								//	¥ OVERRIDE
	virtual	void				HotSpotAction					(	SInt16		inHotSpot,
																			Boolean	inCurrInside,
																			Boolean	inPrevInside );	
																								//	¥ OVERRIDE
	virtual	void				HotSpotResult					(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE
																																		
	//----<< ¥ ARROW CLICK ACTION ¥ >>-------------------------------------------------

public:

	virtual	void				UpArrowAction					();
	virtual	void				DownArrowAction				();
	

};	// LGALittleArrows

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
