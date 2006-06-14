// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAPopup.h

CLASSES:				LGAPopup

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.01.24

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements a popup menu that has the appearance specified by the Apple
Grayscale Appearance specification.

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


#ifndef _H_LGAPopup
	#define _H_LGAPopup
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>

// ¥ TOOLBOX HEADERS
#include <Menus.h>


//==================================================================================== 
//	LGAPopup
//==================================================================================== 
	
class LGAPopup : 	public LControl,
						public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gpop' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	ResIDT			mPopupMenuResID;		//	Resource ID for menu used by the popup
	SInt16				mLabelWidth;			//	Width to be used when rendering the label
	SInt16				mLabelStyle;			//	Style to be used for the label
	SInt16				mLabelJust;				//	Justification for the label
	Boolean			mUseResMenu;			//	Flag that tells us if we are building the
													//		menu from a resource list, typically
													//		used for building FONT menus
	OSType			mResType;				//	Type of resource the menu will be built
													//		from if the above flag is true
	Boolean			mFixedWidth;			//	Does the user want the popup to always be
													//		the specified size or should it be made
													//		to fit the menu width
	SInt16				mNumCommands;			//	Number of commands associated with menu
	CommandT**		mCommandNums;			//	List of command numbers
																	
private:

	MenuHandle		mCachedMenuH;			//	The cached menu handle
	Boolean			mPulldown;				//	Is the menu a pulldown or normal popup
	Boolean			mArrowOnly;				//	Do we want only the popup arrow to be 
													//		rendered, this will result in only
													//		a 3D framed arrow to be drawn to the
													//		right of the popup's current item				
	Boolean			mHilited;				//	Flag used to handle hiliting of the button
													//		during mouse tracking
										
														
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAPopup 			();							//	¥ Default Constructor
				LGAPopup 			(	const LGAPopup &inOriginal );
																		//	¥ Copy Constructor
				LGAPopup 			(	LStream	*inStream );		
																		//	¥ Stream Constructor
				LGAPopup 			(	const SPaneInfo		&inPaneInfo,
											const SControlInfo	&inControlInfo, 
											SInt32						inRefCon,
											SInt16						inControlKind,
											ResIDT					inTextTraitsID,
											Str255					inLabel,
											SInt16						inCurrentItem );
																		//	¥ Parameterized Constructor
				LGAPopup 			(	const SPaneInfo		&inPaneInfo,
											MessageT					inValueMessage,
											ResIDT					inPopupMenuResID,
											Boolean					inUseResMenu,
											SInt32						inResType,
											Boolean					inFixedWidth,
											Str255					inLabel,
											ResIDT					inTextTraitsID,
											SInt16						inLabelWidth,
											SInt16						inLabelStyle,
											SInt16						inLabelJust,
											SInt16						inCurrentItem );
																		//	¥ Parameterized Constructor
	virtual	~LGAPopup 			();							//	¥ Destructor
									
	virtual	void			FinishCreateSelf			();
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	// ¥ GETTERS
	
	virtual StringPtr		GetDescriptor				(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
	
	virtual	Boolean		IsHilited					() const
															{ 	return mHilited; }	
																								
	virtual	MenuHandle	GetMacMenuH					()
															{	return mCachedMenuH;	}
	virtual	MenuHandle	LoadPopupMenuH				() const;
	
	virtual	ResIDT		GetPopupMenuResID			() const
															{	return mPopupMenuResID; }
	virtual	SInt16			GetLabelWidth				() const
															{	return mLabelWidth; }
	virtual	SInt16			GetLabelStyle				() const
															{	return mLabelStyle;	}
	virtual	SInt16			GetLabelJust				() const
															{	return mLabelJust;	}
	virtual	Boolean		UseResourceMenu			() const
															{	return mUseResMenu;	}
	virtual	OSType		GetResourceMenuType		() const
															{	return mResType;	}
	virtual	Boolean		IsPulldownMenu				() const
															{	return mPulldown; }
	virtual	Boolean		IsArrowOnly					() const
															{	return mArrowOnly; }
	virtual	Boolean		HasLabel						() const;
	virtual	SInt16			GetMenuFontSize			() const;
															
	virtual	void			GetCurrentItemTitle		(	Str255	outItemTitle );	
	
	virtual	void			ReadCommandNumbers			();
	virtual	CommandT		CommandFromIndex				(	SInt16	inMenuItem );														
	virtual	CommandT		SyntheticCommandFromIndex 	( SInt16	inIndex ) const;	
													
	virtual	void			CalcLocalPopupFrameRect	( Rect	&outRect ) const;
	virtual	void			CalcTitleRect				( Rect	&outRect );		// ¥ OVERRIDE
	virtual	void			CalcLabelRect				( Rect	&outRect );
	virtual	void			GetPopupMenuPosition		( Point	&outPopupLoc );
	
										
	// ¥ SETTERS
	
	virtual	void			SetMacMenuH				(	MenuHandle	inMacMenuH );
	virtual	void			SetPopupMenuResID		(	ResIDT	inResID );
	virtual void			SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE
	
	virtual	void			SetHiliteState			(	Boolean	inHiliteState );
	
	virtual	void			SetLabelWidth			(	SInt16		inLabelWidth );
	virtual	void			SetLabelStyle			(	SInt16		inLabelStyle );
	virtual	void			SetLabelJust			(	SInt16		inLabelJust );
	virtual	void			SetValue					(	SInt32		inValue );		// ¥ OVERRIDE
	virtual	void			SetupCurrentMenuItem	(	MenuHandle	inMenuH,
																SInt16			inCurrentItem );
	virtual	void			SetPulldown				(	Boolean	inPulldown );
	virtual	void			SetArrowOnly			(	Boolean	inArrowOnly );
	virtual	void			SetPopupMinMaxValues	();
																		
																		
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void			EnableSelf				();								// ¥ OVERRIDE				
	virtual	void			DisableSelf				();								// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void			ActivateSelf			();								// ¥ OVERRIDE
	virtual 	void			DeactivateSelf			();								// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void			DrawSelf					();								// ¥ OVERRIDE				
	virtual	void			DrawPopupNormal		();			
	virtual	void			DrawPopupHilited		();				
	virtual	void			DrawPopupDimmed		();				
	virtual	void			DrawPopupTitle			();
	virtual	void			DrawPopupLabel			();
	virtual	void			DrawPopupArrow			();
	
protected:

	// ¥ Methods for drawing various states at different bit depths
	virtual	void			DrawPopupNormalBW			();							// ¥ OVERRIDE			
	virtual	void			DrawPopupHilitedBW		();							// ¥ OVERRIDE			
	virtual	void			DrawPopupDimmedBW			();							// ¥ OVERRIDE	
	virtual	void			DrawPopupNormalColor		();							// ¥ OVERRIDE
	virtual	void			DrawPopupDimmedColor		();							// ¥ OVERRIDE	
	virtual	void			DrawPopupHilitedColor	();							// ¥ OVERRIDE
		
	
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

	virtual	Boolean		PointInHotSpot 			(	Point 	inPoint,
																	SInt16		inHotSpot ) const;
																								//	¥ OVERRIDE
	virtual	Boolean		TrackHotSpot 				(	SInt16		inHotSpot,
																	Point 	inPoint,
																	SInt16		inModifiers );
																								//	¥ OVERRIDE
	virtual 	void			HotSpotAction				(	SInt16 inHotSpot, 
																	Boolean inCurrInside, 
																	Boolean inPrevInside );
																								// ¥ OVERRIDE
	virtual	void			HotSpotResult				(	SInt16 inHotSpot );	// ¥ OVERRIDE
										
																								
	//----<< ¥ POPUP MENU HANDLING ¥ >>------------------------------------------------

	virtual	void			HandlePopupMenuSelect	(	Point		inPopupLoc,
																	SInt16		inCurrentItem,
																	SInt16		&outMenuID,
																	SInt16		&outMenuItem );

	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

	virtual	void			LoadResourceMenu			(	MenuHandle	inMenu ) const;
	virtual	void			AdjustPopupWidth			();
	virtual	void			ResizeForArrowOnly		();
	
	
};	// LGAPopup

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
