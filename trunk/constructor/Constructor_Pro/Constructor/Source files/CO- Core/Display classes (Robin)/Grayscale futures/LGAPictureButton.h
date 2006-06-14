// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAPictureButton.h

CLASSES:				LGAPictureButton

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.17

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAPictureButton
	#define _H_LGAPictureButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GENERAL HEADERS
#include	"LModeControl.h"

// ¥ POWERPLANT HEADERS
#include <PP_Types.h>

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

class LControl;

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	LGAPictureButton
//==================================================================================== 
	
class LGAPictureButton :	public LModeControl
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gpic' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	ResIDT		mNormalPictureID;	//	Resource ID for the picture we will be drawing
	ResIDT		mPushedPictureID;	//	Resource ID for the pushed picture
	Boolean		mOffsetPicture;	//	Used to tell if we need to offset the  picture
											//		when the button is being hilited
	Int16			mBevelWidth;		//	Width of the bevels drawn on the edge of the
											//		this is used to control how many shades are
											//		used to draw the edges of the button.  This
											//		will primarily be used by the icon button sub-
											//		class but it has been added in here in order
											//		to make the design of this class a little more
											//		reusable
											
											
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

						LGAPictureButton 		();										//	¥ Constructor
						LGAPictureButton 		( 	LStream *inStream );				//	¥ Constructor
	virtual			~LGAPictureButton 	();										//	¥ Destructor
									
	static	LGAPictureButton*	CreateLGAPictureButtonStream 	( 	LStream *inStream );
								
				void	FinishCreateSelf	();											// ¥ OVERRIDE
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	
	// ¥ GETTERS
	
	virtual	Int16				GetBevelWidth			() const
															{	return mBevelWidth; };

	virtual	void				CalcLocalPictureRect		(	Rect	&outRect );
	
	// ¥ SETTERS
	
	virtual	void				SetBevelWidth			(	Int16		inBevelWidth,
																	Boolean	inRedraw = true );
																	
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

public:

	virtual	void				DrawSelf									();			//	¥ OVERRIDE

	virtual	void				DrawPictureButtonNormal				();						
	virtual	void				DrawPictureButtonPushed			();						
	virtual	void				DrawPictureButtonDimmed				();						
	
protected:

	// ¥ Methods for drawing various states at different bit depths
	virtual	void				DrawPictureButtonNormalBW			();							
	virtual	void				DrawPictureButtonPushedBW			();							
	virtual	void				DrawPictureButtonDimmedBW			();							
	virtual	void				DrawPictureButtonNormalColor		();					
	virtual	void				DrawPictureButtonDimmedColor		();							
	virtual	void				DrawPictureButtonPushedColor		();							


	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

	virtual Point				CalcPictureLocation					();
																		
																		
};	// LGAPictureButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif