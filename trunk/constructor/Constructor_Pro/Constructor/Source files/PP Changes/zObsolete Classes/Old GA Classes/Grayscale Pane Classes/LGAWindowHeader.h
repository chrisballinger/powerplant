// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAWindowHeader.h

CLASSES:				LGAWindowHeader

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	97.03.03	
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements a view that can be used to create banners or backgrounds.  It is
a view sub-class and can therefore be used to contain a number of other panes to create
more useful elements.

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAWindowHeader
	#define _H_LGAWindowHeader
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <LView.h>


//==================================================================================== 
//	LGAWindowHeader
//==================================================================================== 
	
class LGAWindowHeader : public LView 
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'whdr' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	Boolean			mHasAdornment;			//	Does the banner have an adornment or not
	Boolean			mHasBottomDivider;	// Is there a black line drawn along the bottom
													//		of the header
											
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAWindowHeader 		();						//	¥ Default Constructor
				LGAWindowHeader		(	const	LGAWindowHeader &inOriginal );
																		//	¥ Copy Constructor
				LGAWindowHeader 		( LStream*	inStream );			
																		//	¥ Stream Constructor
				LGAWindowHeader		(	const SPaneInfo	&inPaneInfo,
												const SViewInfo	&inViewInfo,
												Boolean				inHasAdornment = false,
												Boolean				inHasDivider = true );
																		// ¥ Parameterized Constructor
	virtual	~LGAWindowHeader 		();						//	¥ Destructor
									

	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	// ¥ GETTERS
	
	virtual	Boolean			HasAdornment			() const
													{ return mHasAdornment; };
	virtual	Boolean			HasBottomDivider   	() const
													{ return mHasBottomDivider; };
													
	virtual	RgnHandle		GetBorderRegion		(	const Rect	&inRevealed );
	virtual	RgnHandle		GetShadingRegion		(	const Rect	&inRevealed );
	

	// ¥ SETTERS
	
	virtual	void				SetHasAdornment		( 	Boolean inHasBorder,
																	Boolean inRedraw = true );
	virtual	void				SetHasBottomDivider	( 	Boolean inHasBottomDivider,
																	Boolean inRedraw = true );
	
	
	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

				void				DrawSelf					();							// ¥ OVERRIDE
				
	virtual	void				DrawHeaderFace			();
	virtual	void				DrawHeaderBorder		();
	virtual	void				DrawHeaderShading		();
	virtual	void				DrawHeaderAdornment	();

	virtual	void				RefreshHeaderBorder	();
	virtual	void				RefreshHeaderShading	();
	
																		
};	// LGAWindowHeader

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif