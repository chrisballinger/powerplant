// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGADialogBox.h

CLASSES:				LGADialogBox

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.03.21

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGADialogBox
	#define _H_LGADialogBox
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <LWindow.h>
#include <LListener.h>


//==================================================================================== 
//	LGADialogBox
//==================================================================================== 
	
class LGADialogBox : 	public LWindow,
					 			public LListener 
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'gdlb' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	PaneIDT			mDefaultButtonID;		// Pane ID for the default button
	PaneIDT			mCancelButtonID;		//	Pane ID for the cancel button


	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGADialogBox		();							//	¥ Default Constructor
				LGADialogBox 		(	const LGADialogBox &inOriginal );
																		//	¥ Copy Constructor
				LGADialogBox		(	LStream *inStream );	//	¥ Stream Constructor
				LGADialogBox		(	SWindowInfo &inWindowInfo );
																		//	¥ Parameterized Constructor
				LGADialogBox		(	ResIDT inWINDid, 
											UInt32 inAttributes,
											LCommander* inSuperCommander );
																		//	¥ Parameterized Constructor
	virtual	~LGADialogBox		();							//	¥ Destructor
					
	virtual	void	FinishCreateSelf 		();
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	virtual	 void		SetDefaultButton	(	PaneIDT inButtonID );
	virtual	 void		SetCancelButton	(	PaneIDT inButtonID );


	//----<< ¥ KEY PRESS ¥ >>----------------------------------------------------------

	virtual 	Boolean	HandleKeyPress		(	const EventRecord &inKeyEvent );
																								// ¥ OVERRIDE

	//----<< ¥ LISTENING ¥ >>----------------------------------------------------------

	virtual 	void		ListenToMessage	(	MessageT inMessage, 
														void*		ioParam );				// ¥ OVERRIDE
	

};	// LGADialogBox


//==================================================================================== 
//	TYPES
//==================================================================================== 

struct SLGADialogResponse 
{
	LGADialogBox*	dialogBox;
	void*				messageParam;
};


#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
