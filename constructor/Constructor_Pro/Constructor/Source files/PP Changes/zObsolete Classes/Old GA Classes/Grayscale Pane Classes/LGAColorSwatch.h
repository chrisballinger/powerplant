// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAColorSwatch.h

CLASSES:				LGAColorSwatch

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.09

CHANGE HISTORY :

		96.06.09		rtm	Initial creation of file
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class provides a simple mechanism for display a color that the user can alter by
calling up the Color Picker.  The initial color can be specified from a custom pane
type in Constructor.  Then when the user clicks on the swatch the color picker is
displayed allowing the user to change the color.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGAColorSwatch
	#define _H_LGAColorSwatch
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <LPane.h>


//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGAColorSwatch : public LPane 
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'gswt' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	RGBColor		mSwatchColor;		//	The color being displayed by the swatch
	
	
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAColorSwatch 		();						//	¥ Default Constructor
				LGAColorSwatch 		(	const LGAColorSwatch &inOriginal );
																		//	¥ Copy Constructor
				LGAColorSwatch 		(	LStream*	inStream );		
																		//	¥ Stream	Constructor
				LGAColorSwatch			(	const SPaneInfo	&inPaneInfo,
												RGBColor				&inSwatchColor );
																		//	¥ Parameterized Constructor
	virtual	~LGAColorSwatch 		();						//	¥ Destructor
	
								
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	//	¥ GETTERS
	
	virtual	RGBColor		GetSwatchColor		()
										{	return mSwatchColor;	};
										
										
	//	¥ SETTERS
	
	virtual	void			SetSwatchColor		(	RGBColor		&inSwatchColor )
										{	mSwatchColor = inSwatchColor;	};
										
	
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void			EnableSelf			();									// ¥ OVERRIDE				
	virtual	void			DisableSelf			();									// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void			ActivateSelf		();									// ¥ OVERRIDE
	virtual 	void			DeactivateSelf		();									// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void			DrawSelf				();									// ¥ OVERRIDE
	
																		
	//----<< ¥ CLICK HANDLING ¥ >>-----------------------------------------------------

	virtual	void		ClickSelf				( const SMouseDownEvent&	inMouseDown );
																								// ¥ OVERRIDE
	
																		
};	// LGAColorSwatch

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
