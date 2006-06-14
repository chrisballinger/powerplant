// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAIconSuiteControl.h

CLASSES:				LGAIconSuiteControl

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.07

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class provides the ability to display an icon that can either be a static display 
or can act as a button, radio button, or switch.  Uses an icon suite for the icon.

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAIconSuiteControl
	#define _H_LGAIconSuiteControl
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGAIconMixin.h>
#include	<UGraphicsUtilities.h>

// ¥ POWERPLANT HEADERS
#include <LControl.h>

//==================================================================================== 
//	LGAIconSuiteControl
//==================================================================================== 
	
class LGAIconSuiteControl :	public LControl, 
										public LGAIconMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================
public:

	enum { class_ID = 'gict' };
	

	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	EControlMode		mControlMode;	//	Control mode used to determine how control
												//		responds to a mouse click in its hot spot
	Boolean				mHilited;		//	Used internally to handle the hiliting of the 
												//		button
	
	
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAIconSuiteControl 			();				//	¥ Default Constructor
				LGAIconSuiteControl 			(	const LGAIconSuiteControl &inOriginal );
																		//	¥ Copy Constructor
				LGAIconSuiteControl 			( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGAIconSuiteControl			(	const SPaneInfo 			&inPaneInfo,
														const SControlInfo 		&inControlInfo,
														EControlMode				inControlMode,
														ResIDT						inIconSuiteID,		
														ESizeSelector				inSizeSelector,			
														EIconPosition				inIconPlacement,	
														Boolean						inHiliteIcon,	
														Boolean						inClickInIcon );
																		//	¥ Paramterized Constructor
				LGAIconSuiteControl			(	const SPaneInfo 			&inPaneInfo,
														const SControlInfo 		&inControlInfo,
														const SIconSuiteInfo 	&inIconInfo,
														EControlMode				inControlMode );
																		//	¥ Paramterized Constructor
	virtual	~LGAIconSuiteControl 		();				//	¥ Destructor
						
	virtual	void	FinishCreateSelf	();											// ¥ OVERRIDE
				

	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	EControlMode	GetControlMode			() const
															{	return mControlMode; };
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										

	virtual	void				CalcLocalIconRect		(	Rect	&outRect ) const;		//	¥ OVERRIDE
	
	// ¥ SETTERS
	
	virtual	void				SetControlMode			( EControlMode	inControlMode )
												{	mControlMode = inControlMode;	};
												
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
																								// ¥ OVERRIDE
																								
	virtual	void				SetValue					(	SInt32	inValue );		// ¥ OVERRIDE
	
																																				
	//----<< ¥ ENABLING & DISABLING ¥ >>-----------------------------------------------

	virtual 	void				EnableSelf					();						// ¥ OVERRIDE
	virtual 	void				DisableSelf					();						// ¥ OVERRIDE
	

	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void			ActivateSelf					();						// ¥ OVERRIDE
	virtual 	void			DeactivateSelf					();						// ¥ OVERRIDE


	//----<< ¥ RESIZING ¥ >>-----------------------------------------------------------

	virtual	void				ResizeFrameBy 				( 	SInt16		inWidthDelta,
																		SInt16		inHeightDelta,
																		Boolean		inRefresh );
																								// ¥ OVERRIDE														

	virtual	void				MoveBy 						( 	SInt32		inHorizDelta,
																		SInt32		inVertDelta,
																		Boolean	inRefresh );	
																								//	¥ OVERRIDE
																								
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

protected:

	virtual 	void				HotSpotAction				(	SInt16 inHotSpot, 
																		Boolean inCurrInside, 
																		Boolean inPrevInside );
																								// ¥ OVERRIDE
	virtual 	void				HotSpotResult				(	SInt16 inHotSpot );
																								// ¥ OVERRIDE

																																			
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

public:

	virtual	void				DrawSelf									();			//	¥ OVERRIDE


	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

	virtual	void				CalcIconLocation		();							//	¥ OVERRIDE
	
	virtual	Boolean			PointIsInFrame 		( 	SInt32	inHorizPort,
																	SInt32	inVertPort ) const;
																								// ¥ OVERRIDE
																	
																																		
};	// LGAIconSuiteControl

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
