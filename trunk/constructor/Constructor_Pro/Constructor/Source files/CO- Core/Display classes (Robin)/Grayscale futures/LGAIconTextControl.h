// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAIconTextControl.h

CLASSES:				LGAIconTextControl

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.12.03

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAIconTextControl
	#define _H_LGAIconTextControl
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>
#include <LGAIconSuiteControl.h>

// ¥ POWERPLANT HEADERS
#include <PP_Types.h>
#include <LControl.h>

// ¥ TOOLBOX HEADERS
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
//	FORWARD DECLARATIONS
//==================================================================================== 

class LGAIconSuiteControl;
class LGATitleMixin;

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	LGAIconTextControl
//==================================================================================== 
	
class LGAIconTextControl : public LGAIconSuiteControl,
									public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gitc' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================
																							
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAIconTextControl 		();										//	¥ Constructor
				LGAIconTextControl 		( 	LStream *inStream );				//	¥ Constructor
	virtual	~LGAIconTextControl 		();										//	¥ Destructor
									
	static	LGAIconTextControl*	CreateLGAIconTextControlStream 	( 	LStream *inStream );
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	
	// ¥ GETTERS
	
	virtual StringPtr			GetDescriptor			(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
	
	virtual	void				CalcTitleRect			( Rect	&outRect );		// ¥ OVERRIDE

	//	¥ SETTERS
	
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
																								//	¥ OVERRIDE
																								
	virtual 	void				SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE
	
	virtual	void				SetTextTraitsID		( ResIDT inTextTraitID );
																								//	¥ OVERRIDE
	
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf							();					//	¥ OVERRIDE		
	virtual	void				DrawIconTextControlTitle	();
	virtual	void				DrawIconTextControlHilited	();
																		
};	// LGAIconTextControl

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif