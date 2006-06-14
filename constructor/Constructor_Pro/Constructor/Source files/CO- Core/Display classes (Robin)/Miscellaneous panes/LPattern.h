// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					LPattern.h

CLASSES:				LPattern

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.08.21

CHANGE HISTORY :

		96.08.21		rtm	Initial creation of file
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION



---------------------------------------<< * >>----------------------------------------
*/

#pragma once

	
//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	TYPES
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class	LPane;


//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	LPattern
//==================================================================================== 
	
class LPattern : public LPane
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'ppat' };
	
		
	//====<< * FIELDS * >>=============================================================

protected:

	ResIDT	mPixPatID;		//	Resource ID for the pixpat resource
	
	
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

					LPattern 		();								//	* Constructor
					LPattern 		(	LStream*	inStream );		//	* Constructor
	virtual		~LPattern 		();								//	* Desctructor
									
	static	LPattern*	CreateLPatternStream	(	LStream*	inStream );
	

	//----<< * ACCESSORS * >>----------------------------------------------------------

	inline	ResIDT	GetPixPatID		() const
											{	return mPixPatID;	}
											
	inline	void		SetPixPatID		(	ResIDT	inPixPatID )
											{	mPixPatID = inPixPatID;	}
											
											
	//----<< * DRAWING * >>------------------------------------------------------------

	virtual	void		DrawSelf		();


};	// LPattern
