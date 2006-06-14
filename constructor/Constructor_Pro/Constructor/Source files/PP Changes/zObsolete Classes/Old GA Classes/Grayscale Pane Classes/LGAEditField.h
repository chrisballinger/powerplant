// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAEditField.h

CLASSES:				LGAEditField

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.09

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class provides a sub-class of LBroadcastEditField that provides an appearance as 
specified by Apple's Grayscale Appearance specification. Becuase of the features it 
inherits from LBroadcasterEditField it is also able to broadcast a message [typically
just its pane ID] when certain things happen.

The grayscale frame around the edit field is drawn outside the edit fields typical
frame, so this should be taken into consideration when laying out edit fields. The
border is drawn by an attachment LGAEditFieldBorderAttachment, which only handles the
drawing of the grayscale shadow, the rest of the edit field is either drawn by 
LGAEditField or LEditField.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGAEditField
	#define _H_LGAEditField
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <LBroadcasterEditField.h>

//==================================================================================== 
//	LGAEditField
//==================================================================================== 
	
class LGAEditField : public LBroadcasterEditField
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'gedt' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================
	
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAEditField		();							//	¥ Default Constructor
				LGAEditField 		(	const LGAEditField &inOriginal );
																		//	¥ Copy Constructor
				LGAEditField 		(	LStream*	inStream );			
																		//	¥ Stream Constructor
				LGAEditField		(	const SPaneInfo&	inPaneInfo,
											Str255				inString,
											ResIDT				inTextTraitsID,
											SInt16					inMaxChars,
											UInt8					inAttributes,
											TEKeyFilterFunc	inKeyFilter,
											LCommander*			inSuper);
																		//	¥ Parameterized Constructor
	virtual	~LGAEditField 			();						//	¥ Destructor
									

	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf		();									// ¥ OVERRIDE				
	virtual	void				DisableSelf		();									// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf		();								// ¥ OVERRIDE
	virtual 	void				DeactivateSelf		();								// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				Refresh			();									// ¥ OVERRIDE				
	virtual	Boolean			FocusDraw		(	LPane *inSubPane = nil );	// ¥ OVERRIDE
	virtual	void				DrawSelf			();									//	¥ OVERRIDE	
	virtual	void				DrawBox			();									//	¥ OVERRIDE	
				
												

	
};	// LGAEditField

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
