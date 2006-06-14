// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGABox.h

CLASSES:				LGABox

DESCRIPTION:		The LGABox class is a graphics class that is used as a visual grouping
						mechanism.  It can render a title and a border in a number of 
						different arrangements.

AUTHOR:				Robin Mair

CREATION DATE:		96.06.03

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class provides a box that allows a box with a title to be rendered.  The title
can be placed in a number of different locations around the box as specified by the
EGATitlePosition enum.  The box can have a border that can be rendered in a number of
different ways as specified in the EGABorderStyle enum.

We are currently not using this class anywhere as I have created two other versions of
it that directly implement the two styles of group boxes specified in the Apple
Grayscale Appearance specification.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGABox
	#define _H_LGABox
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include	<LGATitleMixin.h>

// ¥ POWERPLANT HEADERS
#include <LView.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

//==================================================================================== 
//	TYPES
//==================================================================================== 

enum EGABorderStyle
{
	borderStyleGA_NoBorder					=	0,
	borderStyleGA_PlainBorder				=	1,
	borderStyleGA_BezelBorder				=	2,
	borderStyleGA_RecessedOneBorder		=	3,
	borderStyleGA_RecessedTwoBorder		=	4,
	borderStyleGA_EmbossedOneBorder		=	5,
	borderStyleGA_EmbossedTwoBorder		=	6
};

enum EGATitlePosition
{
	titlePositionGA_None						=	0,
	titlePositionGA_Default					=	1,
	titlePositionGA_TopRight				=	2,
	titlePositionGA_TopLeftOut				=	3,
	titlePositionGA_TopRightOut			=	4,
	titlePositionGA_CenterTopOut			=	5,
	titlePositionGA_CenterTop				=	6,
	titlePositionGA_CenterInside			=	7,
	titlePositionGA_CenterTopInside		=	8,
	titlePositionGA_CenterBottomInside	=	9,
	titlePositionGA_CenterBottom			=	10,
	titlePositionGA_CenterBottomOut		=	11
};

//==================================================================================== 
//	LGABox
//==================================================================================== 
	
class LGABox : public LView,
					public LGATitleMixin
{					

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gabx' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	Boolean			mHasBorder;			//	Does the box have a border or not
	EGABorderStyle	mBorderStyle;		//	What type of border for the box
	EGATitlePosition	mTitlePosition;	//	Where is the title to be positioned
	Rect				mContentOffset;	//	The offsets for the content specified as a rect
														

	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGABox 		();									//	¥ Default Constructor
				LGABox 		(	const LGABox &inOriginal );
																		//	¥ Copy Constructor
				LGABox 		(	LStream	*inStream );		
																		//	¥ Stream Constructor
				LGABox		(	const SPaneInfo	&inPaneInfo,
									const SViewInfo	&inViewInfo,
									Str255				inTitle,
									ResIDT				inTextTraitsID,
									Boolean				inHasBorder,
									SInt16					inBorderStyle,
									SInt16					inTitlePosition,
									Rect					&inContentOffset );
																		//	¥ Parameterized Constructor
	virtual	~LGABox		();									// ¥ Destructor
			
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	// ¥ GETTERS
	
	virtual	StringPtr		GetDescriptor	(	Str255	outDescriptor) const;
	virtual	Boolean			HasBorder		() const
													{ return mHasBorder; };
	virtual	EGABorderStyle	GetBorderStyle	() const
													{ return mBorderStyle; };
	virtual	EGATitlePosition	GetTitlePosition	() const
													{ return mTitlePosition; };
	virtual	Rect				GetContentOffset	() const
													{ return mContentOffset; };
	virtual	RgnHandle		GetBoxBorderRegion	(	const Rect	&inRevealed );
	virtual	RgnHandle		GetBoxContentRegion	(	const Rect	&inRevealed );
	
	virtual	void				CalcTitleRect			( Rect	&outRect );
	virtual	void				CalcContentRect		( Rect	&outRect );
	virtual	void				CalcBorderRect			( Rect	&outRect );


	// ¥ SETTERS
	
	virtual	void				SetDescriptor 		( ConstStr255Param	inDescriptor );
	virtual	void				SetBoxHasBorder	( 	Boolean inHasBorder,
																Boolean inRedraw = true );
	virtual	void				SetBorderStyle		( 	EGABorderStyle inBorderStyle,
																Boolean inRedraw = true );
	virtual	void				SetTitlePosition	( 	EGATitlePosition inTitlePosition,
																Boolean inRedraw = true );
	virtual	void				SetContentOffset	( 	const Rect &inContentOffset,
																Boolean inRedraw = true );
	virtual	void				SetTextTraitsID	( 	ResIDT 	inTextTraitsID );
																								//	¥ OVERRIDE
	
																		
	//----<< ¥ ENABLING ¥ >>-----------------------------------------------------------

	virtual	void				EnableSelf			();								// ¥ OVERRIDE				
	virtual	void				DisableSelf			();								// ¥ OVERRIDE				


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf		();								// ¥ OVERRIDE
	virtual 	void				DeactivateSelf		();								// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				RefreshBoxBorder	();
	virtual	void				DontRefreshBoxBorder	(	Boolean inOKIfHidden );
	virtual	void				RefreshBoxContent	();
	virtual	void				RefreshBoxTitle	();

	virtual	void				DrawSelf				();								// ¥ OVERRIDE
	virtual	void				DrawBoxTitle		();
	virtual	void				DrawBoxBorder		();
	virtual	void				DrawBlackAndWhiteBorder	(	const Rect &inBorderRect,
																		EGABorderStyle inBorderStyle );
	virtual	void				DrawColorBorder			(	const Rect &inBorderRect,
																		EGABorderStyle inBorderStyle );
																		
	//----<< ¥ RECTANGLE CALCULATIONS ¥ >>---------------------------------------------

protected:

	virtual void				CalculateTitleHeightAndWidth 			( 	SInt16 *outHeight,
																						SInt16 *outWidth );
																						
	virtual	void				CalculateDefaultTitleRect						( Rect	&outRect );
	virtual	void				CalculateTopRightTitleRect						( Rect	&outRect );
	virtual	void				CalculateTopLeftOutsideTitleRect				( Rect	&outRect );
	virtual	void				CalculateTopRightOutsideTitleRect			( Rect	&outRect );
	virtual	void				CalculateCenterTopOutsideTitleRect			( Rect	&outRect );
	virtual	void				CalculateCenterTopTitleRect					( Rect	&outRect );
	virtual	void				CalculateCenterInsideTitleRect				( Rect	&outRect );
	virtual	void				CalculateCenterTopInsideTitleRect			( Rect	&outRect );
	virtual	void				CalculateCenterBottomInsideTitleRect		( Rect	&outRect );
	virtual	void				CalculateCenterBottomTitleRect				( Rect	&outRect );
	virtual	void				CalculateCenterBottomOutsideTitleRect		( Rect	&outRect );

	
};	// LGABox

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
