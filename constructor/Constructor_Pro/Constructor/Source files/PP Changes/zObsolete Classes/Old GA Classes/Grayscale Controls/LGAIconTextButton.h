// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAIconTextButton.h

CLASSES:				LGAIconTextButton

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.09.17

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class which is a dervative of LGAIconButton adds the ability to display text
alongside the icon.  The position of the text will vary depending on the location of
the icon as specified by the icon placement field.

---------------------------------------<< ¥ >>----------------------------------------
*/


#ifndef _H_LGAIconTextButton
	#define _H_LGAIconTextButton
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATitleMixin.h>
#include <LGAIconButton.h>


//==================================================================================== 
//	LGAIconTextButton
//==================================================================================== 
	
class LGAIconTextButton : public LGAIconButton,
									public LGATitleMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gitb' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================
																							
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAIconTextButton 		();					//	¥ Default Constructor
				LGAIconTextButton 		(	const LGAIconTextButton &inOriginal );
																		//	¥ Copy Constructor
				LGAIconTextButton 		( 	LStream *inStream );				
																		//	¥ Stream Constructor
				LGAIconTextButton 		(	const SPaneInfo			&inPaneInfo,
													const SControlInfo		&inControlInfo,
													const SIconButtonInfo	&inIconButtonInfo,
													Str255						inTitle,
													ResIDT						inTextTraitsID );
																		//	¥ Parameterized Constructor
	virtual	~LGAIconTextButton 		();					//	¥ Destructor
									
									
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	
	// ¥ GETTERS
	
	virtual StringPtr			GetDescriptor			(	Str255	outDescriptor ) const;
																								// ¥ OVERRIDE
	
	virtual	void				CalcTitleRect			( Rect	&outRect );		// ¥ OVERRIDE

	//	¥ SETTERS
	
	virtual void				SetDescriptor			(	ConstStringPtr	inDescriptor );
																								// ¥ OVERRIDE
	
	virtual	void				SetTextTraitsID		( ResIDT inTextTraitID );
																								//	¥ OVERRIDE
	
	virtual	void				AdjustTransform		(	Boolean	inMouseInButton = false,
																	Boolean 	inRedrawIcon = true );
																								//	¥ OVERRIDE


	//----<< ¥ VISIBILITY ¥ >>---------------------------------------------------------


	virtual	void				ShowSelf					();							//	¥ OVERRIDE
	
																																													
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf							();					//	¥ OVERRIDE		
	virtual	void				DrawIconButtonTitle			();
																															
																		
};	// LGAIconTextButton

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
