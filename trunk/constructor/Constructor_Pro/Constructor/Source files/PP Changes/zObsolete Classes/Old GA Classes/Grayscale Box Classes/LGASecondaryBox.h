// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGASecondaryBox.h

CLASSES:				LGASecondaryBox

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE:		96.06.03

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

Implements a box that can render a title and a border that matches Apple's Grayscale
Appearance specification for a secondary box, this is a sub-class of the primary box
class.  It renders a border that provides a recessed appearance.


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGASecondaryBox
	#define _H_LGASecondaryBox
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGAPrimaryBox.h>

	
class LGASecondaryBox : public LGAPrimaryBox 
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gsbx' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================

	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

				LGASecondaryBox 		();						//	¥ Default Constructor
				LGASecondaryBox 		(	const LGASecondaryBox &inOriginal );
																		//	¥ Copy Constructor
				LGASecondaryBox 		(	LStream	*inStream );	
																		//	¥ Stream Constructor
				LGASecondaryBox		(	const SPaneInfo	&inPaneInfo,
												const SViewInfo	&inViewInfo,
												Str255				inTitle,
												ResIDT				inTextTraitsID,
												Rect					&inContentOffset );
																		//	¥ Parameterized Constructor						
	virtual	~LGASecondaryBox		();						// ¥ Destructor
			

	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	virtual	void				CalcBorderRect		(	Rect	&outRect );			//	¥ OVERRIDE
	
	
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawBoxBorder		();								//	¥ OVERRIDE
	
	
};	// LGASecondaryBox

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
