// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAIconMixin.h

CLASSES:				LGAIconMixin

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.01.22

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGAIconMixin
	#define _H_LGAIconMixin
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <PP_Prefix.h>

//==================================================================================== 
//	CONSTANTS
//==================================================================================== 


// ¥ Used by the LGAIconMixin class and any of the classes it is mixed into, it
// defines a number of constants for the various positions an icon can be placed
// within its pane

enum EIconPosition
{
	iconPosition_None,
	iconPosition_TopLeft,
	iconPosition_TopCenter,
	iconPosition_TopRight,
	iconPosition_RightCenter,
	iconPosition_BottomRight,
	iconPosition_BottomCenter,
	iconPosition_BottomLeft,
	iconPosition_LeftCenter,
	iconPosition_Center
};

// ¥ Constants for the various size supported by the icon suite class

enum ESizeSelector
{
	sizeSelector_None,
	sizeSelector_LargeIconSize = 32,
	sizeSelector_SmallIconSize = 16,
	sizeSelector_MiniIconSize = 12
};


//==================================================================================== 
//	TYPES
//==================================================================================== 


typedef struct SIconSuiteInfo
{

	ResIDT			iconSuiteID;			
	Int16				transform;				
	ESizeSelector	sizeSelector;			
	EIconPosition	iconPlacement;		
	Int16				edgeOffset;			
	Boolean			hiliteIcon;			
	Boolean			offsetIconOnHilite;	
	Boolean			clickInIcon;	
			
} SIconSuiteInfo;


//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGAIconMixin 
{

	//====<< ¥ CLASS ID ¥ >>===========================================================
public:

	enum { class_ID = 'icnm' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	Handle			mIconSuite;				//	Handle to the actual icon suite
	ResIDT			mIconSuiteID;			//	Resource ID of iconsuite we will be drawing
	Int16				mTransform;				//	Transform to be applied to icon
	ESizeSelector	mSizeSelector;			//	What size should the icon be drawn
	EIconPosition	mIconPlacement;		// Where in the pane will the icon be drawn	
	Point				mIconLocation;			//	Location of the icon within the pane
	Int16				mEdgeOffset;			//	Amount icon is inset from edge
	Boolean			mHiliteIcon;			//	Does icon hilite when button is pressed
	Boolean			mOffsetIconOnHilite;	//	Does icon move down and to right when hilited
	Boolean			mClickInIcon;			//	This flag is used to tell if hit testing
													//		should only be handled in the actual mask
													//		of the icon suite, instead of the entire
													//		pane frame.
										
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

						LGAIconMixin 		();							//	¥ Default Constructor
						LGAIconMixin 		(	const LGAIconMixin &inOriginal );
																				//	¥ Copy Constructor
	virtual			~LGAIconMixin 		();							//	¥ Destructor
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	Handle			GetIconSuiteH				() const
															{	return mIconSuite; }
	virtual	ResIDT			GetIconResourceID			() const
																{ return mIconSuiteID; }
	virtual	Int16				GetIconTransform			() const
																{ return mTransform; }
	virtual	ESizeSelector	GetIconSize					() const
																{ return mSizeSelector; }
	virtual	EIconPosition	GetIconPosition			() const
																{ return mIconPlacement; }
	virtual	Int16				GetEdgeOffset				() const
																{ return mEdgeOffset; }
	virtual	Boolean			GetHiliteIcon				() const
																{ return mHiliteIcon; }
	virtual	Boolean			GetOffsetIconOnHilite	() const
																{ return mOffsetIconOnHilite; }
	virtual	Boolean			GetClickInIcon				() const
																{ return mClickInIcon; }
																
	virtual	void				LoadIconSuiteHandle		();
									
	virtual	void				CalcLocalIconRect			(	Rect	&outRect ) const;
	
	// ¥ SETTERS
	
	virtual	void				SetIconResourceID			(	ResIDT	inResID );
	virtual	void				SetIconTransform			(	Int16		inTransform );
	virtual	void				SetIconSize					(	ESizeSelector	inSizeSelector );
	virtual	void				SetIconPosition			(	EIconPosition	inPosition );
	virtual	void				SetEdgeOffset				(	Int16		inOffset );
	virtual	void				SetHiliteIcon				(	Boolean	inHiliteIcon );
	virtual	void				SetOffsetIconOnHilite	(	Boolean	inOffsetIcon );
	virtual	void				SetClickInIcon				(	Boolean	inClickInIcon );
	
																
	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

protected:

	virtual void				CalcIconLocation			();

																		
};	// LGAIconMixin

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
