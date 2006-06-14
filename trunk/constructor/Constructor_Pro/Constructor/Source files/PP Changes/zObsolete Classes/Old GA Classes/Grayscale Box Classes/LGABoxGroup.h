// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGABoxGroup.h

CLASSES:				LGABoxGroup

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.10.21

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This sub-class of the box adds the ability to manage a set of controls that have been
embedded within it, such that one of the controls is only ever on.  BEcause of the way
this has been built it could also manage a set of LGAIconButtons that were set to
behave as radio buttons.


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGABoxGroup
	#define _H_LGABoxGroup
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGABox.h>

// ¥ POWERPLANT HEADERS
#include <LString.h>
#include <LListener.h>
#include <LBroadcaster.h>
#include <LControl.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LListener;
class LBroadcaster;
class LControl;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGABoxGroup :	public LGABox,
							public LListener,
							public LBroadcaster
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'gbgp' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	LControl*	mCurrentControl;		//	Reference to the current control
	
	
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGABoxGroup 	();								//	¥ Default Constructor
				LGABoxGroup 	(	const LGABoxGroup &inOriginal );
																		//	¥ Copy Constructor
				LGABoxGroup 	(	LStream*	inStream );	
																		// ¥ Stream Constructor
				LGABoxGroup 	(	const SPaneInfo	&inPaneInfo,
										const SViewInfo	&inViewInfo,
										Str255				inTitle,
										ResIDT				inTextTraitsID,
										Boolean				inHasBorder,
										Int16					inBorderStyle,
										Int16					inTitlePosition,
										Rect					&inContentOffset );
																		//	¥ Parameterized Constructor
	virtual	~LGABoxGroup 			();						//	¥ Destructor
									
	virtual	void			FinishCreateSelf		();								//	¥ OVERRIDE
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	virtual	PaneIDT		GetCurrentControlID	();
	
	
	//----<< ¥ MESSAGE LISTENING ¥ >>--------------------------------------------------

	virtual	void			ListenToMessage 		(	MessageT inMessage, 
																void*		ioParam	);		//	¥ OVERRIDE
													
																		
	//----<< ¥ ADDING CONTROLS ¥ >>----------------------------------------------------

	virtual	void			AddControl				(	LControl*	inControl );
	
													
																		
};	// LGABoxGroup

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
