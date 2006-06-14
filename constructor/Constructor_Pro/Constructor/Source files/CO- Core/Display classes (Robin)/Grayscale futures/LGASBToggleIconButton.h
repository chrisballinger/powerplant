// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGASBToggleIconButton.h

CLASSES:				LGASBToggleIconButton

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.17
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGASBToggleIconButton
	#define _H_LGASBToggleIconButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include	<LGAIconMixin.h>

// ¥ POWERPLANT HEADERS
#include	<PP_Types.h>
#include	<LControl.h>

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

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGASBToggleIconButton :	public LControl,
										public LGAIconMixin
{		

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gsib' };
	

	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	ResIDT		mNormalIconID;		//	ID for the control in its normal state
	ResIDT		mToggledIconID;	//	ID for the control in its normal state
	Boolean		mHilited;			//	Hilited flag
	Boolean		mHasAdornment;		//	Flag used to tell whether we are going to be 
											//		drawing any adornments to the control
	
private:

	Handle		mNormalIconSuite;		//	Cache of the normal icon suite handle
	Handle		mToggledIconSuite;	//	Cache of the toggled icon suite handle


	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

					LGASBToggleIconButton 		();							//	¥ Constructor
					LGASBToggleIconButton 		( 	LStream *inStream );	
																						//	¥ Constructor
	virtual		~LGASBToggleIconButton 		();							//	¥ Destructor
									
	static	LGASBToggleIconButton*	CreateLGASBToggleIconButtonStream ( 	LStream *inStream );
									
				void				FinishCreateSelf		();							// ¥ OVERRIDE
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
															
	virtual	void				CalcLocalIconRect		(	Rect	&outRect ) const;		
																								// ¥ OVERRIDE
	
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };	
																								
	virtual	Boolean			IsAdorned				() const
															{ 	return mHasAdornment; };										
	
	virtual	void				LoadIconCaches			();
	
	
	// ¥ SETTERS
		
	virtual	void				SetValue					( SInt32	inValue );		//	¥ OVERRIDE
		
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
	virtual	void				AdjustTransform		(	Boolean	inMouseInButton = false,
																	Boolean 	inRedrawIcon = true );
	
	virtual	void				SetHasAdornment		(	Boolean	inIsAdorned );
	
	virtual	void				ToggleIcons				();
	
																																				
	//----<< ¥ STATE CHANGE ¥ >>-------------------------------------------------------

				void				ActivateSelf					();					//	¥ OVERRIDE
				void				DeactivateSelf					();					//	¥ OVERRIDE
				
				
	//----<< ¥ RESIZING ¥ >>-----------------------------------------------------------

	virtual	void				ResizeFrameBy 				( 	SInt16		inWidthDelta,
																		SInt16		inHeightDelta,
																		Boolean		inRefresh );
																								// ¥ OVERRIDE														
	virtual	void				MoveBy 						( 	SInt32		inHorizDelta,
																		SInt32		inVertDelta,
																		Boolean	inRefresh );
																								//	¥ OVERRIDE
																																												
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

				void				DrawSelf					();							//	¥ OVERRIDE
				
	virtual	void				DrawToggleIconButton	();
	virtual	void				DrawButtonIcon			();
	virtual	void				DrawAdornment			();

	virtual	void				DrawAdornmentNormal	();						
	virtual	void				DrawAdornmentHilited	();						
	virtual	void				DrawAdornmentDimmed	();						
	

	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

protected:

				void				HotSpotAction			(	SInt16		inHotSpot,
																	Boolean	inCurrInside,
																	Boolean	inPrevInside );	
																								//	¥ OVERRIDE
				void				HotSpotResult			(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE
				
	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

				void				CalcIconLocation		();							// ¥ OVERRIDE
																											
};	// LGASBToggleIconButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif