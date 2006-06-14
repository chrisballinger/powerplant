// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					GAResizeBorder.h

CLASSES:				GAResizeBorder

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.28

CHANGE HISTORY :

		96.06.28		rtm	Initial creation of file
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION



---------------------------------------<< * >>----------------------------------------
*/

#pragma once

#include	"GAResizeBorder.h"
#include	"GAResizeBar.h"

// * GRAYSCALE HEADERS
#include	<LGAFocusBorder.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

enum
{
	resizeBorder_BorderLayoutType = 'BLAY'
};

//==================================================================================== 
//	TYPES
//==================================================================================== 

enum EAdjustOptions
{
	adjustOption_None,
	adjustOption_Proportional,
	adjustOption_KeepVisible
};

#pragma options align = mac68k

typedef struct SResizeBarInfo
{
	PaneIDT		paneID;
	Int16			position;
	Boolean		horizontal;
} **SResizeBarInfoH;

typedef struct SResizeBorderInfo
{
	PaneIDT				borderID;			// ID of the border we represent
	Boolean				adjustHorizontal;	//	Adjusting horizontally or vertically
	Int16					barCount;			//	Number of resize bars we manage
	SResizeBarInfoH	data[]; 				// Resize bar info for n bars
	
} **SResizeBorderInfoH;
	
// * Struct for the 'BLAY' resource which is used to save the current
// config of a browser window
typedef struct SResizeBorderLayout
{	
	PaneIDT					windowID;
	Int16						borderCount;
	SResizeBorderInfoH	data[];
	
} **SResizeBorderLayoutH;

#pragma options align = reset


//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class GAResizeBar;
class	LListener;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	GAResizeBorder
//==================================================================================== 
	
class GAResizeBorder : public LGAFocusBorder,
								public LListener
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'Grbd' };
	
		
	//====<< * FIELDS * >>=============================================================

protected:	
	
	SDimension16		mMinimumSize;			//	An initial minimum size for the border, 
														//		used when resizing to constrain the 
														//		resizing
	Boolean				mAdjustBarsOnResize;	//	When we change size do we want the bars
														//		adjust such that they are not obscured
	Boolean				mAdjustHorizontally;	//	This flag is used to specify in which
														//		direction the border will handle the
														//		adjusting of resize bars.
	EAdjustOptions		mAdjustOption;			//	This is an enum indicating how we are 
														//		going to manage the resize bars that
														//		are associated with this border, the
														//		current options are proportional &
														//		keep panes visible						
	LList					mResizeBars;			//	List of resize bars we are managing

	// * ZOOMING
	Boolean				mCanZoom;				//	Can this pane be zoomed
	PaneIDT				mZoomWidgetID;			//	Pane ID of the zoom widget
	Boolean				mHideBorderOnZoom;	//	Flag used to indicate whether a border
														//		be hidden when a zoom occurs in a
														//		window
														
private:

	PaneIDT		mZoomCommanderID;	//	ID for the commander to which the border resize
											//		manager has been attached
	
												
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

					GAResizeBorder 		();								//	* Constructor
					GAResizeBorder 		(	const GAResizeBorder	&inOriginal );	
																					//	* Copy Constructor
					GAResizeBorder 		(	LStream*	inStream );		//	* Constructor
	virtual		~GAResizeBorder 		();								//	* Desctructor
									
	static	GAResizeBorder*	CreateGAResizeBorderStream	(	LStream*	inStream );
	
				void		FinishCreateSelf			();								//	* OVERRIDE
				
	
	//----<< * ACCESSORS * >>----------------------------------------------------------

	//	** GETTERS
	
	inline	SDimension16	GetMinimumSize			() const
														{	return mMinimumSize;	};
	inline	Int16				GetMinimumWidth		() const
														{	return mMinimumSize.width;	};
	inline	Int16				GetMinimumHeight		() const
														{	return mMinimumSize.height;	};
	virtual	LList&			GetResizeBars			()
														{	return mResizeBars;	};
	inline	Boolean			WantsBarsAdjusted		() const
														{	return mAdjustBarsOnResize;	};		
	inline	Boolean			IsAdjustHorizontal	() const
														{	return mAdjustHorizontally;	};	
	inline	Boolean			CanZoom					() const
														{	return mCanZoom;	};
	inline	PaneIDT			GetZoomWidgetID		() const
														{	return mZoomWidgetID;	};																		
	inline	Boolean			IsHiddenOnZoom			() const
														{	return mHideBorderOnZoom;	};
																												
	virtual	Int16				CountAdjustingBorders 	();
	virtual	void				GetSubBorders				( LList &outSubBorders );
	
	// * Utility
	
	virtual	LWindow*			GetContainingWindow		();
	
			
	//	** SETTERS
	
	virtual	void		SetMinimumSize				(	SDimension16	inMinSize );
	inline	void		SetAdjustBars				(	Boolean	inAdjustBars )
														{	mAdjustBarsOnResize = inAdjustBars;	};
	virtual	void		SetAdjustHorizontally	(	Boolean	inAdjustHorizontal );
	inline	void		SetCanZoom					(	Boolean	inCanZoom )
														{	mCanZoom = inCanZoom;	};
	inline	void		SetHideBorderOnZoom		(	Boolean	inHideBorder )
														{	mHideBorderOnZoom = inHideBorder;	};
														
	
	//----<< * RESIZE BAR MANAGEMENT * >>----------------------------------------------

	virtual	void		RegisterResizeBar			(	GAResizeBar*	inBarToRegister );
	virtual	void		RemoveResizeBar			(	GAResizeBar*	inBarToRemove );
	virtual	void		RemoveAllResizeBars		(); 	
	virtual	void		RearrangeResizeBars		(	Boolean	inHorizontal );
	
	//----<< * RESIZE BORDER INFO * >>-------------------------------------------------

	virtual 	Handle	GetResizeBorderInfo		();
	virtual 	void		SetResizeBorderInfo		(	Handle	inResizeBorderInfoH );
	virtual	Handle	GetResizeBorderLayout	();
	virtual	void		SetResizeBorderLayout	( 	Handle	inBorderLayoutH );
	

	//----<< * RESIZING * >>-----------------------------------------------------------

	virtual	void		ResizeFrameBy				( 	Int16		inWidthDelta,
																Int16		inHeightDelta,
																Boolean	inRefresh );	// * OVERRIDE
																																							
	virtual	void		AdjustResizeBars			(	Rect	inOldBounds	);

	virtual	void		AdjustBarsProportionally	();	
								
																				
	//----<< * LISTENING * >>----------------------------------------------------------

				void		ListenToMessage 			(	MessageT inMessage, 
																void*		ioParam	);		// * OVERRIDE


};	// GAResizeBorder
