// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					LBanner.h

CLASSES:				LBanner

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.05	
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION

This class implements a view that can be used to create banners or backgrounds.  It is
a view sub-class and can therefore be used to contain a number of other panes to create
more useful elements.

---------------------------------------<< * >>----------------------------------------
*/


#ifndef _H_LBanner
	#define _H_LBanner
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// * POWERPLANT HEADERS
#include <PP_Types.h>
#include <LView.h>



//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	LBanner
//==================================================================================== 
	
class LBanner : public LView 
{
	
	//====<< * CLASS ID * >>===========================================================

public:

	enum	{ class_ID = 'bann' };
	
	
	//====<< * FIELDS * >>=============================================================

protected:

	Boolean			mHasBorder;			//	Does the banner have a border or not
	Boolean			mHasShading;		//	Does the banner have shading or not
	Boolean			mHasFace;			//	Does the banner have its face painted or not
	Boolean			mHasAdornment;		//	Does the banner have an adornment or not
	RGBColor			mFaceColor;			//	Color for the banner's face
											
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

							LBanner 			();									//	* Constructor
							LBanner 			( LStream*	inStream );			//	* Constructor
							~LBanner 		();									//	* Destructor
									
	static	LBanner*	CreateLBannerStream ( LStream* inStream );
	

	//----<< * ACCESSORS * >>----------------------------------------------------------

	// * GETTERS
	
	virtual	Boolean			HasBorder		() const
													{ return mHasBorder; };
	virtual	Boolean			HasShading		() const
													{ return mHasShading; };
	virtual	Boolean			HasFace		() const
													{ return mHasFace; };
	virtual	Boolean			HasAdornment() const
													{ return mHasAdornment; };
													
	virtual	RgnHandle		GetBorderRegion		(	const Rect	&inRevealed );
	virtual	RgnHandle		GetShadingRegion		(	const Rect	&inRevealed );
	

	// * SETTERS
	
	virtual	void				SetHasBorder			( 	Boolean inHasBorder,
																	Boolean inRedraw = true );
	virtual	void				SetHasShading			( 	Boolean inHasShading,
																	Boolean inRedraw = true );
	virtual	void				SetHasFace				( 	Boolean inHasFace,
																	Boolean inRedraw = true );
	virtual	void				SetHasAdornment		( 	Boolean inHasBorder,
																	Boolean inRedraw = true );
	virtual	void				SetFaceColor			( 	RGBColor inFaceColor,
																	Boolean inRedraw = true );
	
	
	//----<< * ACTIVATION * >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// * OVERRIDE
	virtual 	void				DeactivateSelf			();							// * OVERRIDE


	//----<< * DRAWING * >>------------------------------------------------------------

				void				DrawSelf					();							// * OVERRIDE
				
	virtual	void				DrawBannerFace			();
	virtual	void				DrawBannerBorder		();
	virtual	void				DrawBannerShading		();
	virtual	void				DrawBannerAdornment	();

	virtual	void				RefreshBannerBorder	();
	virtual	void				RefreshBannerShading	();
	
																		
};	// LBanner

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif