// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAMiniArrowPopup.h

CLASSES:				LGAMiniArrowPopup

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.12.14

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAMiniArrowPopup
	#define _H_LGAMiniArrowPopup
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGAPopup.h>

// ¥ POWERPLANT HEADERS
#include <PP_Types.h>
#include <LControl.h>
#include <LStream.h>

// ¥ TOOLBOX HEADERS
#ifndef __MACTYPES__
	#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
	#include <Quickdraw.h>
#endif

#ifndef __MENUS__
	#include <Menus.h>
#endif


//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LGAPopup;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	G3DPOPUP
//==================================================================================== 
	
class LGAMiniArrowPopup : 	public LControl
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gmnp' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	ResIDT			mPopupMenuResID;		//	Resource ID for menu used by the popup
	Boolean			mUseResMenu;			//	Flag that tells us if we are building the
													//		menu from a resource list, typically
													//		used for building FONT menus
	OSType			mResType;				//	Type of resource the menu will be built
													//		from if the above flag is true
	ResIDT			mTextTraitsID;			//	ID for the text trait that will be used for 
													//		the popup menu																	
private:

	MenuHandle		mCachedMenuH;			//	The cached menu handle
	Boolean			mPulldown;				//	Is the menu a pulldown or normal popup
	Boolean			mHilited;				//	Flag used to handle hiliting of the button
													//		during mouse tracking
										
														
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

					LGAMiniArrowPopup 			();								//	¥ Constructor
					LGAMiniArrowPopup 			(	LStream	*inStream );	//	¥ Constructor
							
	virtual		~LGAMiniArrowPopup 			();								//	¥ Destructor
									
	static	LGAMiniArrowPopup*	CreateLGAMiniArrowPopupStream 	( 	LStream *inStream );

				void			FinishCreateSelf			();
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	// ¥ GETTERS
	
	virtual	Boolean		IsHilited					() const
															{ 	return mHilited; }	
																								
	virtual	MenuHandle	GetMacMenuH					()
															{	return mCachedMenuH;	}
	virtual	MenuHandle	LoadPopupMenuH				() const;
	
	virtual	ResIDT		GetPopupMenuResID			() const
															{	return mPopupMenuResID; }
	virtual	Boolean		UseResourceMenu			() const
															{	return mUseResMenu;	}
	virtual	OSType		GetResourceMenuType		() const
															{	return mResType;	}
	virtual	Boolean		IsPulldownMenu				() const
															{	return mPulldown; }
	virtual	SInt16			GetMenuFontSize			() const;
															
	virtual	ResIDT		GetTextTraitsID			() const
															{ return mTextTraitsID; };
	virtual	void			GetCurrentItemTitle		(	Str255	outItemTitle );														
	virtual	void			CalcLocalPopupFrameRect	( Rect	&outRect );
	virtual	void			GetPopupMenuPosition		( Point	&outPopupLoc );
	
										
	// ¥ SETTERS
	
	virtual	void			SetMacMenuH				(	MenuHandle	inMacMenuH );
	virtual	void			SetPopupMenuResID		(	ResIDT	inResID );
	virtual	void			SetHiliteState			(	Boolean	inHiliteState );
	virtual	void			SetValue					(	SInt32		inValue );		// ¥ OVERRIDE
	virtual	void			SetupCurrentMenuItem	(	MenuHandle	inMenuH,
																SInt16			inCurrentItem );
	virtual	void			SetPulldown				(	Boolean	inPulldown );
	virtual	void			SetPopupMinMaxValues	();
																		
																		
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void			EnableSelf				();								// ¥ OVERRIDE				
	virtual	void			DisableSelf				();								// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------
	
	virtual	void			ActivateSelf			();								//	¥ OVERRIDE
	virtual	void			DeactivateSelf			();								//	¥ OVERRIDE
	virtual	void			Activate					();								//	¥ OVERRIDE
	virtual	void			Deactivate				();								//	¥ OVERRIDE
	
	
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void			DrawSelf					();								// ¥ OVERRIDE				
	virtual	void			DrawPopupNormal		();			
	virtual	void			DrawPopupHilited		();				
	virtual	void			DrawPopupArrow			(	Rect		&inRect );
	
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

protected:

				Boolean		TrackHotSpot 			(	SInt16		inHotSpot,
																Point 	inPoint,
																SInt16		inModifiers );		// ¥ OVERRIDE
			 	void			HotSpotAction			(	SInt16 inHotSpot, 
																Boolean inCurrInside, 
																Boolean inPrevInside );
																								// ¥ OVERRIDE
				void			HotSpotResult			(	SInt16 inHotSpot );		// ¥ OVERRIDE
										
																								
	//----<< ¥ POPUP MENU HANDLING ¥ >>------------------------------------------------

	virtual	void			HandlePopupMenuSelect	(	Point		inPopupLoc,
																	SInt16		inCurrentItem,
																	SInt16		&outMenuID,
																	SInt16		&outMenuItem );

	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

	virtual	void			LoadResourceMenu			(	MenuHandle	inMenu ) const;
	
	
	
};	// LGAMiniArrowPopup

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
