// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGATabButton.h

CLASSES:				LGATabButton

AUTHOR:				Robin Mair

CREATION DATE :	96.12.12

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class is used by the LGATabPanel class and is responsible for display a tab
button and handling the tracking and hiliting.

These buttons are created procedurally from within the tab panel and therefore have
no stream conctructor or stream creation function.

There are two supported sizes for the tab button which are based on the point size of
the text trait specified for the tab panel [and hence the tab button].  The two sizes
are 10 pt. and 12 pt. which is used to determine the vertical height of the tab.  The
titles are always drawn centered on the tab button.


---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGATabButton
	#define _H_LGATabButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>
#include <UGAColorRamp.h>

// ¥ POWERPLANT HEADERS
#include <LControl.h>

// ¥ TOOLBOX HEADERS
#ifndef __MACTYPES__
	#include <MacTypes.h>
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
//	LGATabButton
//==================================================================================== 
	
class LGATabButton : public LControl,
							public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gtab' };
	
	//====<< ¥ FIELDS ¥ >>=============================================================
										
private:

	Boolean		mHilited;				//	Flag used to handle hiliting of the button
												//		during mouse tracking
	SInt16			mTabButtonSize;		//	The size of the button which is based on the
												//		point size of the text trait used for the
												//		button it is either 10 or 12
																			
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGATabButton 		();							//	¥ Default Constructor
				LGATabButton 		(	const LGATabButton	&inOriginal );	
																		//	¥ Copy Constructor
				LGATabButton 		(	const SPaneInfo 			&inPaneInfo,
											const SControlInfo 		&inControlInfo,
											ConstStringPtr				inTitle,
											Boolean						inHiliteTitle,
											ResIDT						inTextTraitsID );					
																		//	¥ Parameterized Constructor
	virtual	~LGATabButton 		();							//	¥ Destructor
									
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual StringPtr			GetDescriptor			(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
	
	virtual	void				CalcTitleRect			( Rect	&outRect );		// ¥ OVERRIDE

	virtual	SInt16				CalcTabButtonSize		() const;

	virtual	RgnHandle		CalcTabButtonRegion		() const;
	virtual	RgnHandle		Calc10PtTabButtonRegion	() const;
	virtual	RgnHandle		Calc12PtTabButtonRegion	() const;

	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
															
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										
	
	virtual	Boolean			IsLeftMostButton		();
	
													
	// ¥ SETTERS
	
	virtual void				SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE
	
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
	virtual	void				SetValue					( SInt32	inValue );		//	¥ OVERRIDE
		

	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------
	
	virtual	void		ActivateSelf				();								//	¥ OVERRIDE
	virtual	void		DeactivateSelf				();								//	¥ OVERRIDE
	
	
	//----<< ¥ ENABLING & DISABLING ¥ >>-----------------------------------------------

	virtual 	void		EnableSelf					();								// ¥ OVERRIDE
	virtual 	void		DisableSelf					();								// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf					();							// ¥ OVERRIDE

	virtual	void				DrawTabButtonTitle	();

protected:
	
	// ¥ Methods for drawing various states at different bit depths
	virtual	void				DrawTabButtonNormalBW			();		
	virtual	void				DrawTabButtonOnBW					();		
	virtual	void				DrawTabButtonHilitedBW			();		
	virtual	void				DrawTabButtonDimmedBW			();		
	virtual	void				DrawTabButtonNormalColor		();
	virtual	void				DrawTabButtonOnColor				();
	virtual	void				DrawTabButtonDimmedColor		();		
	virtual	void				DrawTabButtonHilitedColor		();		
	
	virtual	void				Draw10PtLeftTabSlope				();
	virtual	void				Draw10PtRightTabSlope			();
	virtual	void				Draw12PtLeftTabSlope				();
	virtual	void				Draw12PtRightTabSlope			();
	
																								
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

	virtual	Boolean			PointIsInFrame 		( 	SInt32	inHorizPort,
																	SInt32	inVertPort ) const;
																								//	¥ OVERRIDE
	virtual	void				HotSpotAction			(	SInt16		inHotSpot,
																	Boolean	inCurrInside,
																	Boolean	inPrevInside );	
																								//	¥ OVERRIDE
	virtual	void				HotSpotResult			(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE


};	// LGATabButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
