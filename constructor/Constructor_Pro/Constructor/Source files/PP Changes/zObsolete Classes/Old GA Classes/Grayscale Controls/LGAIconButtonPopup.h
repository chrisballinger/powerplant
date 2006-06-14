// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAIconButtonPopup.h

CLASSES:				LGAIconButtonPopup

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.04

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements a popup menu that has the appearance specified by the Apple
Grayscale Appearance specification for an icon button, but with the ability to
display a popup menu.  In addition to drawing icon it also displays a small arrow
in the lower righthand corner of the popup.

The popup caches the menu handle for the associated menu resource by detaching the
menu resource after it has been loaded.  This avoids any problems that might occur
when there are multiple popups that have been built from the same menu resource. An
API is provided for setting the menu handle takes care of the detaching and also for
getting the max value for the popup changed as the menu changes - SetMacMenuH.

The menu can also be changed dynamically by changing the menu resource ID which is
done by calling SetPopupMenuResID, it then takes care of all the details of getting
rid of the old menu and getting the new one setup.

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAIconButtonPopup
	#define _H_LGAIconButtonPopup
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGAIconButton.h>

// ¥ TOOLBOX HEADERS
#include <Menus.h>


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
//	LGAIconButtonPopup
//==================================================================================== 
	
class LGAIconButtonPopup : 	public LGAIconButton
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gibp' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	ResIDT			mPopupMenuResID;		//	Resource ID for menu used by the popup
	ResIDT			mTextTraitsID;			//	ID of the text trait that will be used to
													//		determine the font for the popup
	SInt16				mNumCommands;			//	Number of commands associated with menu
	CommandT**		mCommandNums;			//	List of command numbers
											
private:

	MenuHandle		mCachedMenuH;			//	The cached menu handle
	Boolean			mPulldown;				//	Is the menu a pulldown or normal popup

											
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAIconButtonPopup 		();					//	¥ Default Constructor
				LGAIconButtonPopup 		(	const LGAIconButtonPopup &inOriginal );
																		//	¥ Copy Constructor
				LGAIconButtonPopup 		( 	LStream*	inStream );		
																		//	¥ Stream Constructor
				LGAIconButtonPopup 		(	const SPaneInfo			&inPaneInfo,
													const SControlInfo		&inControlInfo,
													const SIconButtonInfo	&inIconButtonInfo,
													ResIDT						inPopupMenuResID,
													SInt16							inCurrentItem,
													ResIDT						inTextTraitsID );
																		//	¥ Parameterized Constructor
	virtual			~LGAIconButtonPopup 		();								//	¥ Destructor
									
	virtual	void				FinishCreateSelf			();

	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	
	// ¥ GETTERS
	
	virtual	Boolean			IsPulldownMenu				() const
															{	return mPulldown; }

	virtual	SInt16				GetMenuFontSize			() const;
	
	virtual	MenuHandle		GetMacMenuH					()
															{	return mCachedMenuH;	}
	virtual	MenuHandle		LoadPopupMenuH				() const;
	
	virtual	ResIDT			GetPopupMenuResID			() const
															{	return mPopupMenuResID;	}
	virtual	void				GetCurrentItemTitle		(	Str255	outItemTitle );														
	
	virtual	void				ReadCommandNumbers			();
	virtual	CommandT			CommandFromIndex				(	SInt16	inMenuItem );														
	virtual	CommandT			SyntheticCommandFromIndex 	( SInt16	inIndex ) const;	
													
	virtual	void				GetPopupMenuPosition		( Point	&outPopupLoc );
	virtual	ResIDT			GetTextTraitsID			() const
															{ return mTextTraitsID; }
	virtual	void				CalcLocalIconRect 		( Rect &outRect ) const;	// ¥ OVERRIDE
	
																								
	// ¥ SETTERS
	
	virtual	void				SetMacMenuH					(	MenuHandle	inMacMenuH );
	virtual	void				SetPopupMenuResID			(	ResIDT	inResID );
	virtual	void				SetValue						(	SInt32		inValue );
																								// ¥ OVERRIDE
	virtual	void				SetPulldown					(	Boolean		inIsPulldown )
															{	mPulldown = inIsPulldown;	}
	virtual	void				SetupCurrentMenuItem		(	MenuHandle	inMenuH,
																		SInt16			inCurrentItem );
	
	virtual	void				SetPopupMinMaxValues		();
	
	virtual	void				SetTextTraitsID			(	ResIDT	inTextTraitID );


	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

protected:

	virtual	Boolean			TrackHotSpot 				(	SInt16		inHotSpot,
																		Point 	inPoint,
																		SInt16		inModifiers );
																								//	¥ OVERRIDE
	virtual 	void				HotSpotAction				(	SInt16 inHotSpot, 
																		Boolean inCurrInside, 
																		Boolean inPrevInside );
																								// ¥ OVERRIDE
	virtual 	void				HotSpotResult				(	SInt16 inHotSpot );
																								// ¥ OVERRIDE
	virtual	void				HandlePopupMenuSelect	(	Point			inPopupLoc,
																		SInt16			inCurrentItem,
																		SInt16			&outMenuID,
																		SInt16			&outMenuItem );


																																			
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

public:

	virtual	void				DrawSelf							();					// ¥ OVERRIDE	
	
protected:

	virtual	void				DrawPopupIndicator			();

	virtual	void				DrawIconButtonDimmedColor	();					//	¥ OVERRIDE
	

	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

	virtual void				CalcIconLocation				();					//	¥ OVERRIDE
																		
																		
};	// LGAIconButtonPopup

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
