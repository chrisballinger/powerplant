// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGACheckbox.h

CLASSES:				LGACheckbox

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.05.26

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements a standard checkbox that has an appearance as specified by
the Apple Grayscale Appearance specificaton.

This class handles the rendering of the checkbox control portion of the button by setting
the color for each individual pixel that makes up the control's appearance. The data
for this drawing is laid out in a static array of chars which is a private field of the
class. Each pixel either has a color value or 'N' which means no drawing is to be 
performed, the various colors are specified as a value into an array of colors as
provided by the UGAColorRamp class.  This class contains the Apple specified color
ramp for the grayscale appearance specification.

There a re a number of different versions of the control specified in the array for 
each of the various states the checkbox can be in.  NOTE: the checkbox provides
support for a mixed state although there is currently no implementation for handling
the representation of the state via the controls value field. THIS HAS NOW BEEN
IMPLEMENTED USING A NEW CONSTANT THAT HAS BEEN ADDED TO PP_Constants.h CALLED
Button_Mixed - 97.02.03


---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGACheckbox
	#define _H_LGACheckbox
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>


//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGACheckbox :	public LControl,
							public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gchk' };
	

	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	Boolean		mHilited;						//	Flag used to handle hiliting of the
														//		control during mouse tracking

	Boolean		mClassicCheckMark;			//	True if the checkmark is the classic
														//		'x' style mark
														
																					
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGACheckbox 		();							//	¥ Default Constructor
				LGACheckbox 		(	const LGACheckbox &inOriginal );
																		//	¥ Copy Constructor
				LGACheckbox 		( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGACheckbox 		(	const SPaneInfo		&inPaneInfo,
											const SControlInfo	&inControlInfo,
											Str255			inTitle,
											ResIDT			inTextTraitID );
																		//	¥ Parameterized Constructor
	virtual	~LGACheckbox 		();							//	¥ Destructor
									
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() == Button_On; }
															
	virtual 	StringPtr		GetDescriptor		(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
																
	virtual	void				CalcTitleRect			( Rect	&outRect );		// ¥ OVERRIDE
																								
	virtual	void				CalcLocalControlRect	(	Rect	&outRect );
	
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; }								
	
	virtual	Boolean			IsMixedState			() const
															{ 	return GetValue() == Button_Mixed; }										
	
	// ¥ SETTERS
	
	virtual 	void				SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE																
	
	virtual	void				SetValue					( SInt32	inValue );		//	¥ OVERRIDE
		
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
	virtual	void				SetSetClassicCheckMark	(	Boolean	inClassicMark );
	
																																				
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf				();							// ¥ OVERRIDE				
	virtual	void				DisableSelf				();							// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

				void				DrawSelf						();						//	¥ OVERRIDE
				
	virtual	void				DrawCheckboxTitle			();

	virtual	void				DrawCheckboxNormal		();						
	virtual	void				DrawCheckboxHilited		();						
	virtual	void				DrawCheckboxDimmed		();						
	
	
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

protected:

				void				HotSpotAction				(	SInt16		inHotSpot,
																		Boolean	inCurrInside,
																		Boolean	inPrevInside );	
																								//	¥ OVERRIDE
				void				HotSpotResult				(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE
																																		
	//----<< ¥ MOUSE TRACKING ¥ >>-----------------------------------------------------

	virtual	void				RefreshControl				();
	

};	// LGACheckbox

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
