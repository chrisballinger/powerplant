// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGAIconSuite.h

CLASSES:				LGAIconSuite

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.07

---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

A simple class that can display an icon suite.

---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGAIconSuite
	#define _H_LGAIconSuite
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGAIconMixin.h>

// ¥ POWERPLANT HEADERS
#include <LPane.h>
#include <LString.h>


//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGAIconSuite :	public LPane, 
							public LGAIconMixin
{

	//====<< ¥ CLASS ID ¥ >>===========================================================
public:

	enum { class_ID = 'gicn' };
	

	//====<< ¥ FIELDS ¥ >>=============================================================

	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGAIconSuite 		();							//	¥ Default Constructor
				LGAIconSuite 		(	const LGAIconSuite &inOriginal );
																		//	¥ Copy Constructor
				LGAIconSuite 		( 	LStream *inStream );	
																		//	¥ Stream Constructor
				LGAIconSuite 		(	const SPaneInfo	&inPaneInfo,
											ResIDT				inIconSuiteResID,
											SInt16					inSizeSelector,
											SInt16					inIconPosition );
																		//	¥ Parameterized Constructor
	virtual	~LGAIconSuite 		();							//	¥ Destructor
									
	virtual	void		FinishCreateSelf	();										// ¥ OVERRIDE
				

	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------
	
	// ¥ GETTERS
	
	virtual	void				CalcLocalIconRect			(	Rect	&outRect ) const;	//	¥ OVERRIDE
	
	
	// ¥ SETTERS
	
																																				
	//----<< ¥ ENABLING & DISABLING ¥ >>-----------------------------------------------

	virtual 	void				EnableSelf					();						// ¥ OVERRIDE
	virtual 	void				DisableSelf					();						// ¥ OVERRIDE


	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual 	void				ActivateSelf				();						// ¥ OVERRIDE
	virtual 	void				DeactivateSelf				();						// ¥ OVERRIDE


	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void				DrawSelf					();							// ¥ OVERRIDE

	
	//----<< ¥ RESIZING ¥ >>-----------------------------------------------------------

	virtual	void			ResizeFrameBy 				( 	SInt16		inWidthDelta,
																	SInt16		inHeightDelta,
																	Boolean		inRefresh );
																								// ¥ OVERRIDE														
	virtual	void			MoveBy 						( 	SInt32		inHorizDelta,
																	SInt32		inVertDelta,
																	Boolean	inRefresh );	
																								//	¥ OVERRIDE
																
																
	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

protected:

	virtual	void				CalcIconLocation		();							// ¥ OVERRIDE
																	
																																		
};	// LGAIconSuite

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
