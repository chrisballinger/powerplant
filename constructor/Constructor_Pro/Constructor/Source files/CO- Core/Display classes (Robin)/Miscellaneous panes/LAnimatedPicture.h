// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LAnimatedPicture.h

CLASSES:				LAnimatedPicture

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.06
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION



---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LAnimatedPicture
	#define _H_LAnimatedPicture
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <PP_Types.h>
#include <LPane.h>
#include <LPeriodical.h>

	
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
//	LAnimatedPicture
//==================================================================================== 
	
class LAnimatedPicture : 	public LPane,
									public LPeriodical
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'apic' };
	
		
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	ResIDT			mFirstPictID;		//	Starting PICT ID
	ResIDT			mLastPictID;		//	Ending PICT ID
	ResIDT			mCurrPictID;		//	PICT that is currently being displayed
	Int32				mNextTime;
	Int16				mIndicatorSpeed;	//	The frequency with which the PICTs are moved
	
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

					LAnimatedPicture 		();								//	¥ Constructor
					LAnimatedPicture 		(	LStream*	inStream );		//	¥ Constructor
	virtual		~LAnimatedPicture 		();								//	¥ Desctructor
									
	static	LAnimatedPicture*	CreateLAnimatedPictureStream	(	LStream*	inStream );
	

	//----<< ¥ SPEND TIME ¥ >>---------------------------------------------------------

	virtual	void		SpendTime		( const EventRecord &inMacEvent );
											
											
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

protected:

	virtual	void		DrawSelf			();								//	¥ OVERRIDE


};	// LAnimatedPicture

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif