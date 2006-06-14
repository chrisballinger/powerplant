// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGARadioButton.h

CLASSES:				LGARadioButton

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.05.25

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements a standard radio button that has an appearance as specified by
the Apple Grayscale Appearance specificaton.

This class handles the rendering of the radio control portion of the button by setting
the color for each individual pixel that makes up the control's appearance. This data
for this drawing is laid out in a static array of chars which is a private field of the
class. Each pixel either has a color value or 'N' which means no drawing is to be 
performed, the various colors are specified as a value into an array of colors as
provided by the UGAColorRamp class.  This class contains the Apple specified color
ramp for the grayscale appearance specification.

There are a number of different versions of the control specified in the array for 
each of the various states the radio button can be in.  NOTE: the button provides
support for a mixed state although there is currently no implementation for handling
the representation of the state via the controls value field. THE MIXED STATE HAS
NOW BEEN IMPLEMENTED - 97.02.03

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGARadioButton
	#define _H_LGARadioButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>


//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGARadioButton :	public LControl,
								public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'grad' };
	

	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	Boolean		mHilited;						//	Flag used to handle hiliting of the
														//		control during mouse tracking
	
							
private:

	static 	signed char sRadioRendering[15][12][12];	//	This multidimension array will contain
														//		the indices to the colors that are
														//		used to render the radio button

	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGARadioButton 			();					//	¥ Default Constructor
				LGARadioButton 			(	const LGARadioButton &inOriginal );
																		//	¥ Copy Constructor
				LGARadioButton 			( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGARadioButton 			(	const SPaneInfo 			&inPaneInfo,
													const SControlInfo 		&inControlInfo,
													Str255						inTitle,
													ResIDT						inTextTraitsID );
																		//	¥ Parameterized Constructor
	virtual	~LGARadioButton 			();					//	¥ Destructor
									
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() == Button_On; };
															
	virtual StringPtr			GetDescriptor			(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
																
	virtual	void				CalcTitleRect			( Rect	&outRect );
																								// ¥ OVERRIDE
	virtual	void				CalcLocalControlRect	(	Rect	&outRect );
	
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										
	
	virtual	Boolean			IsMixedState			() const
															{ 	return GetValue() == Button_Mixed; }										
	
	// ¥ SETTERS
	
	virtual void				SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE																
	
	virtual	void				SetValue					( SInt32	inValue );		//	¥ OVERRIDE
		
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
																																				
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf				();							// ¥ OVERRIDE				
	virtual	void				DisableSelf				();							// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf					();							//	¥ OVERRIDE
				
	virtual	void				DrawRadioTitle			();

	virtual	void				DrawRadioNormal		();						
	virtual	void				DrawRadioHilited		();						
	virtual	void				DrawRadioDimmed		();						
	
protected:

	virtual	void				DrawRadioChoice		( 	SInt16	inChoice,
																	Rect	&inFrame );
	
																	
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

	virtual	void				HotSpotAction				(	SInt16		inHotSpot,
																		Boolean	inCurrInside,
																		Boolean	inPrevInside );	
																								//	¥ OVERRIDE
	virtual	void				HotSpotResult				(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE
																																		
};	// LGARadioButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
