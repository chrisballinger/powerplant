// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					GAZoomView.h

CLASSES:				GAZoomView

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.11.23
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION



---------------------------------------<< * >>----------------------------------------
*/

#pragma once

// *ÊPOWERPLANT HEADERS
#include	<LView.h>
#include	<LCommander.h>

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	TYPES
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	GAZoomView
//==================================================================================== 
	
class GAZoomView : 	public LView,
							public LCommander
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'zoom' };
	
		
	//====<< * FIELDS * >>=============================================================

	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

					GAZoomView 		();										//	* Constructor
					GAZoomView 		(	const GAZoomView	&inOriginal );	
																					//	* Copy Constructor
					GAZoomView 		(	LStream*	inStream );				//	* Constructor
	virtual		~GAZoomView 		();									//	* Desctructor
									
	static	GAZoomView*	CreateGAZoomViewStream	(	LStream*	inStream );
	
	
};	// GAZoomView
