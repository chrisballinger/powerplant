// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					GAShadowBorder.h

CLASSES:				GAShadowBorder

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.21

CHANGE HISTORY :

		96.06.21		rtm	Initial creation of file
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION

This class implements a view that renders a border around it edge.  It does NOT do any
kind of filling only the border around the edge which can be one or two pixels wide.
This class is typically used in conjunction with one or more GAFocusBorder classes to
create a 3D effect in windows.

This class uses a coloring scheme based on Apple's Grayscale Appearance specification
including the correct modification of colors as the view moves from being active to 
inactive.

---------------------------------------<< * >>----------------------------------------
*/

#pragma once


	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LView;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	GAShadowBorder
//==================================================================================== 
	
class GAShadowBorder : public LView 
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'Gsbd' };
	
		
	//====<< * FIELDS * >>=============================================================

protected:	
	
	Boolean		mTwoPixelShadow;		//	Are we drawing a two or one pixel wide shadow
	Boolean		mFrameGrowBox;			//	Do we want the shadow around the grow box
	SInt16			mGrowBoxSize;			//	Size of the grow box
	
												
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

							GAShadowBorder 			();							//	* Constructor
							GAShadowBorder 			(	LStream*	inStream );	//	* Constructor
	virtual				~GAShadowBorder 		();								//	* Desctructor
									
	static	GAShadowBorder*	CreateGAShadowBorderStream	(	LStream*	inStream );
	
	//----<< * ACCESSORS * >>----------------------------------------------------------

	//	** GETTERS
	
	virtual	RgnHandle	GetBorderRegion 		();
	
														
	//	** SETTERS
		
	
	//----<< * ACTIVATION * >>---------------------------------------------------------

	virtual	void			ActivateSelf			();								//	* OVERRIDE
	virtual	void			DeactivateSelf			();								//	* OVERRIDE
	virtual	void			Activate					();								//	* OVERRIDE
	virtual	void			Deactivate				();								//	* OVERRIDE
	
	
	//----<< * DRAWING * >>------------------------------------------------------------

	virtual	void			DrawSelf					();
	virtual	void			DrawBorder				(	SInt16		inBitDepth );
	
protected:

	virtual	void			RefreshBorder			();
																	
																		
};	// GAShadowBorder
