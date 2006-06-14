// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGASecondaryGroup.h

CLASSES:				LGASecondaryGroup

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.03

CHANGE HISTORY :

		96.06.03		rtm	Initial creation of file
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This sub-class of the secondary box adds the ability to manage a set of radio buttons
that have been embedded within it, such that one of the buttons is only ever on.


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGASecondaryGroup
	#define _H_LGASecondaryGroup
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGASecondaryBox.h>

// ¥ POWERPLANT HEADERS
#include <LListener.h>
#include <LBroadcaster.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LGASecondaryBox;
class LListener;
class LBroadcaster;
class LControl;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGASecondaryGroup : 	public LGASecondaryBox,
									public LListener,
									public LBroadcaster
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'gsgp' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	LControl*	mCurrentControl;		//	Reference to the current control
	
	
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGASecondaryGroup ();							//	¥ Default Constructor
				LGASecondaryGroup (	const LGASecondaryGroup &inOriginal );
																		//	¥ Copy Constructor
				LGASecondaryGroup (	LStream*	inStream );	
																		//	¥ Stream Constructor
				LGASecondaryGroup	(	const SPaneInfo	&inPaneInfo,
											const SViewInfo	&inViewInfo,
											Str255				inTitle,
											ResIDT				inTextTraitsID,
											Rect					&inContentOffset );
																		//	¥ Parameterized Constructor						
	virtual	~LGASecondaryGroup 			();				//	¥ Destructor
									
	virtual	void			FinishCreateSelf		();								//	¥ OVERRIDE
			
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	virtual	PaneIDT		GetCurrentControlID	();
	
	
	//----<< ¥ MESSAGE LISTENING ¥ >>--------------------------------------------------

	virtual	void			ListenToMessage 		(	MessageT inMessage, 
																void*		ioParam	);		//	¥ OVERRIDE
													
																		
	//----<< ¥ ADDING CONTROLS ¥ >>----------------------------------------------------

	virtual	void			AddControl				(	LControl*	inControl );
	
													
																		
};	// LGASecondaryGroup

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
