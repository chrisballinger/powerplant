// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					LModeControl.h

CLASSES:				LModeControl

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.17

---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< * >>----------------------------------------
*/


#ifndef _H_LModeControl
	#define _H_LModeControl
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// * POWERPLANT HEADERS
#include <PP_Types.h>
#include <LControl.h>

// * TOOLBOX HEADERS
#ifndef __TYPES__
	#include <Types.h>
#endif

#ifndef __QUICKDRAW__
	#include <Quickdraw.h>
#endif

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

// const MessageT		msg_DoubleClick	=	'DClk';			// ebs 


//==================================================================================== 
//	TYPES
//==================================================================================== 

enum EControlModes
{
	controlModes_None,
	controlModes_Button,
	controlModes_RadioButton,
	controlModes_Switch
};

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LControl;

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	LModeControl
//==================================================================================== 
	
class LModeControl :	public LControl
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'mode' };
	
	
	//====<< * FIELDS * >>=============================================================

protected:

	EControlModes	mControlMode;	//	Control mode used to determine how control
											//		responds to a mouse click in its hot spot

	Boolean			mPushed;			//	Flag used to indicate that the control has been
											//		pushed.
											
											
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

						LModeControl 		();										//	* Constructor
						LModeControl 		( 	LStream *inStream );				//	* Constructor
						LModeControl 		(	const SPaneInfo 			&inPaneInfo,
													const SControlInfo 		&inControlInfo,
													EControlModes				inControlMode );
																							//	* Constructor
	virtual			~LModeControl 	();											//	* Destructor
									
	static	LModeControl*	CreateLModeControlStream 	( 	LStream *inStream );
								
				
	//----<< * ACCESSORS * >>----------------------------------------------------------
	
	
	// * GETTERS
	
	virtual	EControlModes	GetControlMode			() const
															{	return mControlMode; };
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
	virtual	Boolean			IsPushed					() const
															{ 	return mPushed; };										
	
	// * SETTERS
	
	virtual	void				SetPushedState			(	Boolean	inPushedState );

	virtual	void				SetValue					(	Int32	inValue );		// * OVERRIDE
	
	virtual	void				SetControlMode			(	EControlModes	inControlMode )
															{	mControlMode = inControlMode;	}
		
	
	//----<< * ENABLING & DISABLING * >>-----------------------------------------------

	virtual 	void				EnableSelf				();							// * OVERRIDE
	virtual 	void				DisableSelf				();							// * OVERRIDE


	//----<< * MOUSE TRACKING * >>-----------------------------------------------------

protected:

	virtual 	void				HotSpotAction				(	Int16 inHotSpot, 
																		Boolean inCurrInside, 
																		Boolean inPrevInside );
																								// * OVERRIDE
	virtual 	void				HotSpotResult				(	Int16 inHotSpot );
																								// * OVERRIDE
																																										
																		
};	// LModeControl

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif