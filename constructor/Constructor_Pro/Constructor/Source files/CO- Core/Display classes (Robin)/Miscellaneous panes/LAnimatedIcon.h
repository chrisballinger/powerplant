// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					LAnimatedIcon.h

CLASSES:				LAnimatedIcon

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.04

CHANGE HISTORY :

		96.11.04		rtm	Initial creation of file
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION



---------------------------------------<< * >>----------------------------------------
*/

#pragma once

	
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
//	LAnimatedIcon
//==================================================================================== 
	
class LAnimatedIcon : 	public LPane,
								public LPeriodical
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'anmi' };
	
		
	//====<< * FIELDS * >>=============================================================

protected:

	ResIDT			mFirstIconID;		//	Starting icon suite ID
	ResIDT			mLastIconID;		//	Ending icon suite ID
	ResIDT			mCurrIconID;		//	Icon that is currently being displayed
	Int32				mNextTime;
	Int16				mIndicatorSpeed;	//	The frequency with which the icons are moved
	
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

					LAnimatedIcon 		();											//	* Constructor
					LAnimatedIcon 		(	LStream*	inStream );					//	* Constructor
	virtual		~LAnimatedIcon 		();										//	* Desctructor
									
	static	LAnimatedIcon*	CreateLAnimatedIconStream	(	LStream*	inStream );
	

	//----<< * SPEND TIME * >>---------------------------------------------------------

	virtual	void		SpendTime		( const EventRecord &inMacEvent );
											
											
	//----<< * DRAWING * >>------------------------------------------------------------

protected:

	virtual	void		DrawSelf					();									//	* OVERRIDE
	virtual	void		RefreshToIconMask		();

};	// LAnimatedIcon
