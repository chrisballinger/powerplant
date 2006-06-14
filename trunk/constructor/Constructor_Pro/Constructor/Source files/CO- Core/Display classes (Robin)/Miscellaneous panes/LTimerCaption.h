// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					LTimerCaption.h

CLASSES:				LTimerCaption

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.07

CHANGE HISTORY :

		96.11.07		rtm	Initial creation of file
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION



---------------------------------------<< * >>----------------------------------------
*/

#pragma once

	
// * TOOLBAOX HEADERS
#include	<Packages.h>

//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	TYPES
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class	LCaption;
class	LPeriodical;

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	LTimerCaption
//==================================================================================== 
	
class LTimerCaption : 	public LCaption,
								public LPeriodical
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'time' };
	
		
	//====<< * FIELDS * >>=============================================================

protected:

	Int16				mIndicatorSpeed;	//	The frequency with which the timer is updated
	Int32				mNextTime;
	
	UInt32			mStartTime;			//	Time when the timer was started
	Intl0Hndl		mTimeFormatH;		//	This is the formatting that will be sued for 
												//		the time string
	
	
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

					LTimerCaption 		();											//	* Constructor
					LTimerCaption 		(	LStream*	inStream );					//	* Constructor
	virtual		~LTimerCaption 		();										//	* Desctructor
									
	static	LTimerCaption*	CreateLTimerCaptionStream	(	LStream*	inStream );
	

	//----<< * TIMER CONTROL * >>------------------------------------------------------

	virtual	void		StartTimer			();
	virtual	void		StopTimer			();
											
											
	//----<< * SPEND TIME * >>---------------------------------------------------------

	virtual	void		SpendTime			( const EventRecord &inMacEvent );
											
											
};	// LTimerCaption
