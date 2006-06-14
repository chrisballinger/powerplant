// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				EraseAllAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTEraseAllAction

DESCRIPTION:		Implementation file for classes
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.4.23		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"

/*=====================================
	Constructor
======================================*/
PTEraseAllAction::PTEraseAllAction( const SPaintAction &inAction ) : PTAction( inAction, str_UndoEraseAll )
{
	this->ThrowIfFileLocked();
}

/*=====================================
	Destructor
======================================*/
PTEraseAllAction::~PTEraseAllAction()
{
}

/*=====================================
	DoIt
======================================*/
void PTEraseAllAction::DoIt()
{
	Rect			r;
	SUOffscreen		*imageBuffer = mSettings.currentBuffer;
	StSaveGWorld	aSaver;
	
		// copy the current image & selection region to the undo area
	mSettings.thePaintView->CopyToUndo();

		// erase the image buffer to the background color
	imageBuffer->BeginDrawing();

	StColorPenState		aPenState;
	aPenState.Normalize();
	
	//PmForeColor( mSettings.theForeColor );
	//PmBackColor( mSettings.theBackColor );
	imageBuffer->SetForeColor( mSettings.theForeColor );
	imageBuffer->SetBackColor( mSettings.theBackColor );
	SetRect( &r, 0, 0, imageBuffer->GetWidth(), imageBuffer->GetHeight() );
	EraseRect( &r );

		// update the screen
	mSettings.theCanvas->DrawFrom( imageBuffer );
	imageBuffer->EndDrawing();
	
		// post the action for undo support
	this->PostAsAction();
}

