// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAPrimaryGroup.h

CLASSES:				LGAPrimaryGroup

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.05.26

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This sub-class of the primary box adds the ability to manage a set of radio buttons
that have been embedded within it, such that one of the buttons is only ever on.


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGAPrimaryGroup
	#define _H_LGAPrimaryGroup
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGAPrimaryBox.h>

// ¥ POWERPLANT HEADERS
#include <LListener.h>
#include <LBroadcaster.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LGAPrimaryBox;
class LListener;
class LBroadcaster;
class LControl;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGAPrimaryGroup : public LGAPrimaryBox,
								public LListener,
								public LBroadcaster
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'gpgp' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	LControl*	mCurrentControl;		//	Reference to the current control
	
	
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAPrimaryGroup 	();							//	¥ Default Constructor
				LGAPrimaryGroup 	(	const LGAPrimaryGroup &inOriginal );
																		//	¥ Copy Constructor
				LGAPrimaryGroup 	(	LStream*	inStream );	
																		//	¥ Stream Constructor
				LGAPrimaryGroup	(	const SPaneInfo	&inPaneInfo,
											const SViewInfo	&inViewInfo,
											Str255				inTitle,
											ResIDT				inTextTraitsID,
											Rect					&inContentOffset );
																		//	¥ Parameterized Constructor						
	virtual	~LGAPrimaryGroup 			();					//	¥ Destructor
									
	virtual	void			FinishCreateSelf		();								//	¥ OVERRIDE
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	virtual	PaneIDT		GetCurrentControlID	();
	
	
	//----<< ¥ MESSAGE LISTENING ¥ >>--------------------------------------------------

	virtual	void			ListenToMessage 		(	MessageT inMessage, 
																void*		ioParam	);		//	¥ OVERRIDE
													
																		
	//----<< ¥ ADDING CONTROLS ¥ >>----------------------------------------------------

	virtual	void			AddControl				(	LControl*	inControl );
	
													
																		
};	// LGAPrimaryGroup

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
