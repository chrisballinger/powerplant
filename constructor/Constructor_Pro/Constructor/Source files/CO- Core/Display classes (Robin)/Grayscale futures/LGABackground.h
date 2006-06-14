// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1995 - 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>----------------------------------------

FILE:					LGABackground.h

CLASSES:				LGABackground

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.19

---------------------------------------<< � >>----------------------------------------

	THEORY OF OPERATION


---------------------------------------<< � >>----------------------------------------
*/


#ifndef _H_LGABackground
	#define _H_LGABackground
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// � GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>

// � POWERPLANT HEADERS
#include <PP_Types.h>
#include <LAttachment.h>

// � TOOLBOX HEADERS
#ifndef __MACTYPES__
	#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
	#include <Quickdraw.h>
#endif


	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LAttachment;


//==================================================================================== 
//	CPIPELINEBACKGROUND
//==================================================================================== 
	
class	LGABackground : public LAttachment 
{

	//====<< � CLASS ID � >>===========================================================

public:

	enum { class_ID = 'gbck' };
	
	
	//====<< � FIELDS � >>=============================================================

protected:
	
	PaneIDT		mInsetSubPaneID;		//	ID of the pane that the border will be framing
												//		if the framing option is turned on
	Boolean		mPaintFace;				//	Should the face of the border be painted or not
												//		this flag allows control over the painting
												//		of the face independent of the drawing of the
												//		inset frame for the inset pane
	Boolean		mNotchFace;				//	Do we want a notch taken out of the lower right
												//		corner of the face of the border in order to
												//		accomodate a growbox
	Boolean		mFrameInsetSubPane;	//	Do we want the specified subpane framed
	Boolean		mNotchInsetFrame;		//	Do we want the lower right corner of the
												//		border to have a notch in it, this is typically
												//		used to draw around the grow box
	SInt16			mNotchWidth;			//	Width for the notch if we are drawing that option
												//		by default this is set to 15

private:

	LPane*		mInsetSubPane;			//	A reference to the pane that we are framing if
												//		that option is turned on


	//====<< � METHODS � >>============================================================
		
	//----<< � INITIALIZATION & DISPOSAL � >>------------------------------------------

public:

							LGABackground 	(	PaneIDT	inInsetSubPane,
													SInt16		inNotchSize = 15,
													Boolean 	inPaintFace = true,
													Boolean 	inFrameSubPane = true,
													Boolean 	inNotchSubPane = false,
													Boolean 	inNotchFace = true,
													Boolean 	inExecuteHost = true );			
																							//	� Constructor
							LGABackground 	(	LStream*	inStream );
							
	static	LGABackground*	CreateLGABackgroundStream	(	LStream*	inStream );
				
	//----<< � ACCESSORS � >>----------------------------------------------------------

	//	�� GETTERS
	
	inline	Boolean		WantsFacePainted		() const
														{	return mPaintFace;	};
	inline	Boolean		WantsFaceNotched					() const
														{	return mNotchFace;	};
	inline	Boolean		WantsInsetSubPaneFramed			() const
														{	return mFrameInsetSubPane;	};
	inline	Boolean		WantsInsetFrameNotched			() const
														{	return mNotchInsetFrame;	};
	inline	LPane*		GetInsetSubPane		();
	inline	SInt16			GetNotchWidth				() const
														{	return mNotchWidth;	};
														
	virtual	RgnHandle	GetFaceRegion				();
	virtual	RgnHandle	GetInsetFrameRegion		();
	
			
	//	�� SETTERS
	
public:
	
	virtual	void			SetPaintFace			(	Boolean	inPaintFace,
																Boolean	inRedraw = true );
	virtual	void			SetInsetSubPane		(	LPane*	inInsetSubPane );
	inline	void			SetFrameInsetSubPane	(	Boolean	inFrameInsetPane )
														{	mFrameInsetSubPane = inFrameInsetPane; };
	inline	void			SetNotchInsetFrame	(	Boolean	inNotchInsetFrame )
														{	mNotchInsetFrame = inNotchInsetFrame; };
	inline	void			SetNotchFace	(	Boolean	inNotchFace )
														{	mNotchFace = inNotchFace; };
	virtual	void			SetNotchWidth			(	SInt16		inNotchWidth,
																Boolean	inRedraw = false );

	//----<< � EXECUTING � >>----------------------------------------------------------

protected:
	
	virtual 	void		ExecuteSelf					(	MessageT inMessage, 
																void*		ioParam );		// � OVERRIDE
																
	//----<< � DRAWING � >>------------------------------------------------------------

	virtual	void		DrawBackground					(	SInt16		inBitDepth );
	
	virtual	void		RefreshFace						();
	virtual	void		RefreshInsetFrame				();
	virtual	void		RefreshInsetSubPane			();
	
	
};	// LGABackground

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif


