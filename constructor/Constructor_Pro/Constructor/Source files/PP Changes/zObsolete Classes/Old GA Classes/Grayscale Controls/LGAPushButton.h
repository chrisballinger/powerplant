// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAPushButton.h

CLASSES:				LGAPushButton

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.05.25

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements the standard pushbutton as specified by Apple's Grayscale
Appearance specification.  This button is used in place of LStdButton which is the
standard toolbox provided button.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGAPushButton
	#define _H_LGAPushButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>


//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGAPushButton :	public LControl,
								public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gpsh' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	Boolean		mDefault;		//	This flag is used to indicate whether the control is
										//		the default which means that it would then possibly
										//		do additional drawing of some form of outline
	Boolean		mHilited;		//	Flag used to handle hiliting of the button
										//		during mouse tracking
										
										
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAPushButton 	();								//	¥ Default Constructor
				LGAPushButton 	(	const LGAPushButton &inOriginal );
																		//	¥ Copy Constructor
				LGAPushButton 	( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGAPushButton	(	const SPaneInfo 			&inPaneInfo,
										const SControlInfo 		&inControlInfo,
										Str255						inTitle,
										ResIDT						inTextTraitsID,
										Boolean						inDefault = false );
																		//	¥ Parameterized Constructor
	virtual	~LGAPushButton ();								//	¥ Destructor
									
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual StringPtr			GetDescriptor			(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
	
	virtual	void				CalcTitleRect			( Rect	&outRect );		// ¥ OVERRIDE

	virtual	Boolean			IsDefaultButton		() const
															{ return mDefault; };
															
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										
	
	// ¥ SETTERS
	
	virtual void				SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE
	
	virtual	void				SetDefaultButton		( 	Boolean inDefault,
																	Boolean	inRedraw = true );

	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
	virtual	void				SetValue					( SInt32	inValue );		//	¥ OVERRIDE
		

	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void			EnableSelf				();								// ¥ OVERRIDE				
	virtual	void			DisableSelf				();								// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf					();							// ¥ OVERRIDE

	virtual	void				DrawPushButton			();
	virtual	void				DrawButtonTitle		( SInt16	inDepth );
	virtual	void				DrawDefaultOutline	( SInt16	inDepth );							
	
protected:

	// ¥ Methods for drawing various states at different bit depths
	virtual	void				DrawButtonNormalBW			();			
	virtual	void				DrawButtonHilitedBW			();
	virtual	void				DrawButtonDimmedBW			();
	virtual	void				DrawButtonNormalColor		();
	virtual	void				DrawButtonDimmedColor		();
	virtual	void				DrawButtonHilitedColor		();
	
	
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

	virtual	Boolean			PointInHotSpot 		(	Point 	inPoint,
																	SInt16		inHotSpot ) const;
																								//	¥ OVERRIDE
	virtual	void				HotSpotAction			(	SInt16		inHotSpot,
																	Boolean	inCurrInside,
																	Boolean	inPrevInside );	
																								//	¥ OVERRIDE
	virtual	void				HotSpotResult			(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE

																		
};	// LGAPushButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
