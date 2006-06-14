// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1995 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					GResizeBorderMgr.h

CLASSES:				GResizeBorderMgr

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.08.19

CHANGE HISTORY :

		96.08.19		rtm	Initial creation of file
	
			
---------------------------------------<< * >>----------------------------------------

THEORY OF OPERATION


---------------------------------------<< * >>----------------------------------------
*/

#pragma once


#include <LAttachment.h>

	
//==================================================================================== 
//	COMMAND CONSTANTS
//==================================================================================== 

const	MessageT		cmd_ZoomBorder				=	'zoom';
const	MessageT		cmd_RegisterBorder		=	'regb';
const	MessageT		cmd_RegisterResizeBar	=	'regr';


//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LAttachment;
class	GAResizeBorder;
class	GAResizeBar;

//==================================================================================== 
//	GResizeBorderMgr
//==================================================================================== 
	
class	GResizeBorderMgr : public LAttachment 
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'Grbm' };
	
		
	//====<< * FIELDS * >>=============================================================

protected:

	LView*	mHostView;		//	View too which we are attached and whose borders we
									//		are managing, this view must be the container of
									//		the borders that are being managed
									
	GAResizeBorder*	mZoomedBorder;		//	The currently zoomed border
	GAResizeBorder*	mSurrogateBorder;	//	This is the border that is standing in for
													//		the currently zoomed border in the view
													//		hierarchy
	
private:

	LList		mBorders;		// Borders currently registered with the border manager,
									//		only borders that can be zoomed and are visible are
									//		registered with the manager
	LList		mResizeBars;	//	A list of all the resize bars that are in the window
								
									
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:
			
							GResizeBorderMgr			();							//	* Constructor
							GResizeBorderMgr 			(	LStream*	inStream );	//	* Constructor
	virtual				~GResizeBorderMgr			();							//	* Destructor
	
	static	GResizeBorderMgr*	CreateGResizeBorderMgrStream	(	LStream*	inStream );
	
	//----<< * MESSAGE HANDLING * >>---------------------------------------------------

protected:
	
	virtual 	void	ExecuteSelf						(	MessageT inMessage, 
																void*		ioParam );		// * OVERRIDE
	
	//----<< * BORDER MANAGEMENT * >>--------------------------------------------------

	virtual	void	RegisterBorder					(	GAResizeBorder*	inBorder );
	virtual	void	RemoveBorder					(	GAResizeBorder*	inBorder );
	virtual	void	RegisterResizeBar				(	GAResizeBar*		inBar );
	
	virtual	void	HandleZoomBorder				(	GAResizeBorder*	inBorder,
																Boolean				inZoom );
	virtual	void	DoZoomBorder					(	GAResizeBorder*	inBorder,
																Boolean				inZoom );
	
	
};	// GResizeBorderMgr


