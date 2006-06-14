// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGASeparator.h

CLASSES:				LGASeparator

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.05.26		

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class is used to display a simple separator line as specified by Apple's Grayscale
Appearance specification.  If the size of the separator is specified as wider than it
is tall the line is drawn horizontally or if it is the other way around it is drawn 
vertically.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGASeparator
	#define _H_LGASeparator
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif


// ¥ POWERPLANT HEADERS
#include <LPane.h>


//==================================================================================== 
//	LGASeparator
//==================================================================================== 
	
class LGASeparator : public LPane 
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'gsep' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	Boolean		mHorizontal;		//	True if the separator is horizontal, when it is
											//		false it means we are drawing a vertical line
											
											
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGASeparator 		();							//	¥ Default Constructor
				LGASeparator 		(	const LGASeparator &inOriginal );
																		//	¥ Copy Constructor
				LGASeparator 		( 	LStream*	inStream );	//	¥ Stream Constructor
				LGASeparator		(	const SPaneInfo &inPaneInfo );
																		// ¥ Parameterized Constructor
	virtual	~LGASeparator 		();							//	¥ Destructor
									
	virtual	void				FinishCreateSelf	();								//	¥ OVERRIDE
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	// ¥ GETTERS
	
	virtual	Boolean			IsHorizontal					()
													{	return mHorizontal; };
													
	
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf				();							// ¥ OVERRIDE				
	virtual	void				DisableSelf				();							// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf					();							//	¥ OVERRIDE
	
																		
};	// LGASeparator

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
