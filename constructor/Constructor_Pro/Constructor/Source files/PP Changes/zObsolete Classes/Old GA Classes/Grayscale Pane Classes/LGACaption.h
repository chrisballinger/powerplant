// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGACaption.h

CLASSES:				LGACaption

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	97.02.11

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class is a derivative of LCaption that simply adds the ability for rendering its
text in a disabled state when deactivated.  This rendering is achieved by simply
lightening the color used to render the text as specified in the associated text trait.
This class has been provided for use alongside all of the other grayscale classes
which draw themselves in a disabled state when deactivated, so the appearance is more
consistently flat.

NOTE:	This class has not yet been integrated into Constructor so its type is not 
available by default.  A custom CTyp resource has been provided that can be placed in
the same folder as Constructor in order to make this new class available from within
Constructor's catalog.  Additionally, any existing LCaption instance can be changed 
to use this new class by changing it's class ID from 'capt' to 'gcap' the ID of the 
new class.


---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGACaption
	#define _H_LGACaption
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>

// ¥ POWERPLANT HEADERS
#include <LCaption.h>


//==================================================================================== 
//	LGACaption
//==================================================================================== 
	
class	LGACaption : public LCaption 
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gcap' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

					LGACaption 	();								//	¥ Default Constructor
					LGACaption	(	const LGACaption	&inCaption );
																		//	¥	Copy Constructor
					LGACaption 	(	LStream*	inStream );	
																		//	¥ Stream Constructor
					LGACaption 	( 	const SPaneInfo	&inPaneInfo,
										ConstStringPtr	inString,
										ResIDT			inTextTraitsID );			
																		//	¥ Parameterized Constructor
							
	virtual		~LGACaption	();								//	¥ Destructor
	
					
	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf		();								// ¥ OVERRIDE
	virtual 	void				DeactivateSelf		();								// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

protected:

	virtual	void				DrawSelf				();								// ¥ OVERRIDE
	
	
	
};	// LGACaption

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif


