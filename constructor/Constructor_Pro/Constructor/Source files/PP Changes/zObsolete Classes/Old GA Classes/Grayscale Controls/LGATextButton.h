// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGATextButton.h

CLASSES:				LGATextButton

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.16

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements a simple text button that dispalys a title and renders with a
one pixel wide 3D border.  It uses the same specification as a mini icon button from 
AGA.  This can be used as an alternative to a regular push button where something a 
little more compact is needed.

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGATextButton
	#define _H_LGATextButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGATextButton : public LControl,
							public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gtxb' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

private:	
	
	Boolean		mHilited;		//	Flag used to handle hiliting of the button
										//		during mouse tracking
										
										
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGATextButton 	();								//	¥ Default Constructor
				LGATextButton 	(	const LGATextButton &inOriginal );
																		//	¥ Copy Constructor
				LGATextButton 	( 	LStream *inStream );		//	¥ Stream Constructor
				LGATextButton	(	const SPaneInfo 			&inPaneInfo,
										const SControlInfo 		&inControlInfo,
										Str255						inTitle,
										ResIDT						inTextTraitsID );
																		//	¥ Parameterized Constructor
	virtual	~LGATextButton ();								//	¥ Destructor
									
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
															
	virtual StringPtr			GetDescriptor			(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
	
	virtual	void				CalcTitleRect			( Rect	&outRect );		// ¥ OVERRIDE

	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										
	
	// ¥ SETTERS
	
	virtual void				SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE
	
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
	virtual	void				SetValue					( SInt32	inValue );		//	¥ OVERRIDE
		

	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf				();							// ¥ OVERRIDE				
	virtual	void				DisableSelf				();							// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf					();							// ¥ OVERRIDE

	virtual	void				DrawTextButtonTitle	();
	
protected:

	virtual	void				DrawTextButtonNormal			();			
	virtual	void				DrawTextButtonHilited		();
	virtual	void				DrawTextButtonDimmed			();
	
	
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

	virtual	void				HotSpotAction			(	SInt16		inHotSpot,
																	Boolean	inCurrInside,
																	Boolean	inPrevInside );	
																								//	¥ OVERRIDE
	virtual	void				HotSpotResult			(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE

																		
};	// LGATextButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
