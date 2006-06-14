// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					GAZoomToggle.h

CLASSES:				GAZoomToggle

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.08.14

CHANGE HISTORY :

		96.08.14		rtm	Initial creation of file
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< * >>----------------------------------------
*/

#pragma once



//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LControl;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class GAZoomToggle :	public LControl
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'Gzom' };
	

	//====<< * FIELDS * >>=============================================================

protected:

	Boolean		mHilited;		//	Hilited flag
	
	
private:

	static char sPixelMap[7][9][9];	//	This multidimension array will contain
											//		the indices to the colors that are
											//		used to render the control

	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

							GAZoomToggle 			();							//	* Constructor
							GAZoomToggle 			( 	LStream *inStream );	
																							//	* Constructor
							~GAZoomToggle 		();								//	* Destructor
									
	static	GAZoomToggle*	CreateGAZoomToggleStream 		( 	LStream *inStream );
									
	//----<< * ACCESSORS * >>----------------------------------------------------------
	
	// * GETTERS
	
	virtual	Boolean			IsSelected				() const
															{ 	return GetValue() > 0; };
															
	virtual	void				CalcLocalControlRect	(	Rect	&outRect );
	
	virtual	Boolean			IsHilited				() const
															{ 	return mHilited; };										
	
	// * SETTERS
		
	virtual	void				SetValue					( Int32	inValue );		//	* OVERRIDE
		
	virtual	void				SetHiliteState			(	Boolean	inHiliteState );
	
																																				
	//----<< * STATE CHANGE * >>-------------------------------------------------------

				void				ActivateSelf					();					//	* OVERRIDE
				void				DeactivateSelf					();					//	* OVERRIDE
				
				
	//----<< * DRAWING * >>------------------------------------------------------------

				void				DrawSelf					();							//	* OVERRIDE
				
	virtual	void				DrawMiniPixelButton	();
	
protected:

	virtual	void				DrawPixelsChoice		( 	Int16	inChoice,
																	Rect	&inFrame );
	
																	
	//----<< * MOUSE TRACKING * >>-----------------------------------------------------

				void				HotSpotAction				(	Int16		inHotSpot,
																		Boolean	inCurrInside,
																		Boolean	inPrevInside );	
																								//	* OVERRIDE
				void				HotSpotResult				(	Int16 inHotSpot );											
																								//	* OVERRIDE
																																		
	//----<< * BROADCASTING * >>-------------------------------------------------------

				void				BroadcastValueMessage	();						// * OVERRIDE
				
				

};	// GAZoomToggle
