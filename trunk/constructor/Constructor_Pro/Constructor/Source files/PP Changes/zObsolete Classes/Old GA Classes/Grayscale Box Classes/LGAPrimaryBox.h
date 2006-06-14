// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAPrimaryBox.h

CLASSES:				LGAPrimaryBox

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE:		96.06.03

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

Implements a box that can render a title and a border that matches Apple's Grayscale
Appearance specification for a primary box. It renders a border that appears routed
into the surface.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGAPrimaryBox
	#define _H_LGAPrimaryBox
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include	<LGATitleMixin.h>

// ¥ POWERPLANT HEADERS
#include <LView.h>

// ¥ TOOLBOX HEADERS
#include <TextEdit.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

//==================================================================================== 
//	TYPES
//==================================================================================== 

//==================================================================================== 
//	LGAPrimaryBox
//==================================================================================== 
	
class LGAPrimaryBox : 	public LView,
								public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gpbx' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	Rect				mContentOffset;	//	The offsets for the content specified as a rect
														

	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAPrimaryBox 		();							//	¥ Default Constructor
				LGAPrimaryBox 		(	const LGAPrimaryBox &inOriginal );
																		//	¥ Copy Constructor
				LGAPrimaryBox 		(	LStream	*inStream );//	¥ Stream Constructor
				LGAPrimaryBox		(	const SPaneInfo	&inPaneInfo,
											const SViewInfo	&inViewInfo,
											Str255				inTitle,
											ResIDT				inTextTraitsID,
											Rect					&inContentOffset );
																		//	¥ Parameterized Constructor						
	virtual	~LGAPrimaryBox		();							// ¥ Destructor
			

	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	// ¥ GETTERS
	
	virtual	StringPtr		GetDescriptor		(	Str255	outDescriptor) const;
	virtual	Rect				GetContentOffset	() const
													{ return mContentOffset; };
	virtual	RgnHandle		GetBoxBorderRegion	(	const Rect	&inRevealed );
	virtual	RgnHandle		GetBoxContentRegion	(	const Rect	&inRevealed );
	
	virtual	void				CalcTitleRect			( Rect	&outRect );
	virtual	void				CalcContentRect		( Rect	&outRect );
	virtual	void				CalcBorderRect			( Rect	&outRect );
	
	
	// ¥ SETTERS
	
	virtual	void				SetDescriptor 		( ConstStr255Param	inDescriptor );
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
	
	
	//----<< ¥ MISCELLANEOUS ¥ >>---------------------------------------------

protected:
	
	virtual void				CalculateTitleHeightAndWidth 			( 	SInt16 *outHeight,
																						SInt16 *outWidth );
																						
																						
};	// LGAPrimaryBox

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
