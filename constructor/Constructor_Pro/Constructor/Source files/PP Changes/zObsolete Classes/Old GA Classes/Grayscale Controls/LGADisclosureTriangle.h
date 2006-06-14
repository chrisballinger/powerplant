// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGADisclosureTriangle.h

CLASSES:				LGADisclosureTriangle

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.18

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements the disclosure triangles as specified by AGA.  It provides either
a right or left facing version of the triangle.  In addition, it does the animation
specified by AGA with an intermediate state for the triangle.

This class uses the same approach as the radio and checkbox in that it has all the
data to render the control specified in an array that contains the various states the
control can be in.  The appropriate state is then just drawn from the array.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGADisclosureTriangle
	#define _H_LGADisclosureTriangle
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ POWERPLANT HEADERS
#include <LControl.h>


//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGADisclosureTriangle :	public LControl
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gdcl' };
	

	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	Boolean		mHilited;						//	Flag used to handle hiliting of the
														//		control during mouse tracking
	Boolean		mLeftDisclosure;				//	Flag used to indicate if we are dealing
														//		with a right or left facing triangle
															
private:

	static signed char sDisclosurePixMap[19][12][12];		//	This multidimension array will contain
															//		the indices to the colors that are
															//		used to render the checkbox button

	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGADisclosureTriangle 		();				//	¥ Default Constructor
				LGADisclosureTriangle 		(	const LGADisclosureTriangle &inOriginal );
																		//	¥ Copy Constructor
				LGADisclosureTriangle 		( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGADisclosureTriangle (	const SPaneInfo		&inPaneInfo,
												const SControlInfo	&inControlInfo,
												Boolean					inLeftDisclosure );
																		//	¥ Parameterized Constructor
	virtual	~LGADisclosureTriangle 		();				//	¥ Destructor
									
	
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
															
	virtual	Boolean			IsLeftDisclosure		() const
															{ 	return mLeftDisclosure; };
															
	virtual	void				CalcLocalControlRect	(	Rect	&outRect );
	
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										
	
	// ¥ SETTERS
	
	virtual	void				SetValue					( SInt32	inValue );		//	¥ OVERRIDE
		
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
																																				
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf				();							// ¥ OVERRIDE				
	virtual	void				DisableSelf				();							// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void			ActivateSelf			();								// ¥ OVERRIDE
	virtual 	void			DeactivateSelf			();								// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

				void				DrawSelf						();						//	¥ OVERRIDE
				
	virtual	void				DrawIntermediateState	();
	virtual	void				WipeBackground				();
	

protected:

	virtual	void				PlotPixelMapChoice		( 	SInt16	inChoice );
	
																	
	//----<< ¥ MOuSE TRACKING ¥ >>-----------------------------------------------------

				void				HotSpotAction				(	SInt16		inHotSpot,
																		Boolean	inCurrInside,
																		Boolean	inPrevInside );	
																								//	¥ OVERRIDE
				void				HotSpotResult				(	SInt16 inHotSpot );											
																								//	¥ OVERRIDE
																																		
};	// LGADisclosureTriangle

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
