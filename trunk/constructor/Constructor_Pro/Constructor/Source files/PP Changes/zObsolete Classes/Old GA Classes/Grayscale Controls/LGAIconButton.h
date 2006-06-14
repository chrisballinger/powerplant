// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAIconButton.h

CLASSES:				LGAIconButton

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.04

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class provides a button that has an appearance as specified by Apple's Grayscale
Appearance specification, as well as displaying an icon within the button.  The button
renders itself differently depending on the size speciifed for the icon, as follows;

	32 x 32 icon		-	the button renders three pixel wide shadows
	16 x 16 icon		-	two pixel wide shadows
	16 x 12 icon		-	one pixel wide shadow
	
this class also has the ability to display the icon in a number of different locations
within the button, these are enumerated in the EIconPosition enum which can be found
in the file: LGAIconMixin.h.

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAIconButton
	#define _H_LGAIconButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGAIconMixin.h>
#include <UGraphicsUtilities.h>

// ¥ POWERPLANT HEADERS
#include <LControl.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

const MessageT		msg_DoubleClick	=	'DClk';			// ebs 


//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LControl;
class LGAIconMixin;

//==================================================================================== 
//	TYPEDEFS
//==================================================================================== 

#pragma options align=mac68k

typedef struct	SIconButtonInfo 
{
	EControlMode	controlMode;
	ResIDT			iconResID;
	ESizeSelector	sizeSelector;
	EIconPosition	iconPosition;
	Boolean			hiliteIcon;
	Boolean			offsetIconOnHilite;
} SIconButtonInfo;

#pragma options align=reset

//==================================================================================== 
//	LGAIconButton
//==================================================================================== 
	
class LGAIconButton :	public LControl,
								public LGAIconMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gibt' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:

	EControlMode mControlMode;		//	Control mode used to determine how control
											//		responds to a mouse click in its hot spot

private:

	SInt16		mBevelWidth;			//	Width of the bevels drawn on the edge of the
											//		this is used to control how many shades are
											//		used to draw the edges of the button.  This
											//		will primarily be used by the icon button sub-
											//		class but it has been added in here in order
											//		to make the design of this class a little more
											//		reusable
	Boolean	mHilited;				//	Flag used during hiliting of a control, used
											//		internally for now.
											
											
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAIconButton 		();							//	¥ Default Constructor
				LGAIconButton 		(	const LGAIconButton &inOriginal );
																		//	¥ Copy Constructor
				LGAIconButton 		( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGAIconButton 		(	const SPaneInfo		&inPaneInfo,
											const SControlInfo	&inControlInfo,
											EControlMode			inControlMode,
											ResIDT					inIconResID,
											ESizeSelector			inIconSizeSelector,
											EIconPosition			inIconPlacement,
											Boolean					inHiliteIcon = true,
											Boolean					inOffsetIconOnHilite = true );
																		//	¥ Parameterized Constructor
				LGAIconButton 		(	const SPaneInfo			&inPaneInfo,
											const SControlInfo		&inControlInfo,
											const SIconButtonInfo	&inIconButtonInfo );
																		//	¥ Parameterized Constructor
	virtual	~LGAIconButton 	();							//	¥ Destructor
									
	virtual	void	FinishCreateSelf	();											// ¥ OVERRIDE
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	
	// ¥ GETTERS
	
	virtual	EControlMode	GetControlMode			() const
															{	return mControlMode; };
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										
	virtual	SInt16				GetBevelWidth			() const
															{	return mBevelWidth; };

	virtual	void				CalcLocalIconRect		(	Rect	&outRect ) const;		// ¥ OVERRIDE
	
	// ¥ SETTERS
	
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );

	virtual	void				SetValue					(	SInt32	inValue );		// ¥ OVERRIDE
	
	virtual	void				SetBevelWidth			(	SInt16		inBevelWidth,
																	Boolean	inRedraw = true );
																	
	virtual	void				AdjustTransform		(	Boolean	inMouseInButton = false,
																	Boolean 	inRedrawIcon = true );
	
	
	//----<< ¥ ENABLING & DISABLING ¥ >>-----------------------------------------------

	virtual 	void				EnableSelf				();							// ¥ OVERRIDE
	virtual 	void				DisableSelf				();							// ¥ OVERRIDE


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf			();							// ¥ OVERRIDE
	virtual 	void				DeactivateSelf			();							// ¥ OVERRIDE


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

	virtual	void				DrawSelf							();					//	¥ OVERRIDE

	virtual	void				DrawIconButtonNormal			();						
	virtual	void				DrawIconButtonHilited		();						
	virtual	void				DrawIconButtonDimmed			();						
	
protected:

	virtual	void				DrawIcon					();

	// ¥ Methods for drawing various states at different bit depths
	virtual	void				DrawIconButtonNormalBW			();							
	virtual	void				DrawIconButtonHilitedBW			();							
	virtual	void				DrawIconButtonDimmedBW			();							
	virtual	void				DrawIconButtonNormalColor		();					
	virtual	void				DrawIconButtonDimmedColor		();							
	virtual	void				DrawIconButtonHilitedColor		();							


	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

	virtual void				CalcIconLocation					();				//	¥ OVERRIDE
																		
																		
};	// LGAIconButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
