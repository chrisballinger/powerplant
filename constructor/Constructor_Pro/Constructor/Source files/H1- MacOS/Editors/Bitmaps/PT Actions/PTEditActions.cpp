// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				EditActions.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTCutAction, PTCopyAction, PTPasteAction, CClearAction

DESCRIPTION:		Implementation file for classes
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.29	ebs		Paste -- commit selection before pasting
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
#include "PTEditActions.h"


	// ------------------ PTCutAction --------------------
/*=====================================
	CutAction Constructor
======================================*/
PTCutAction::PTCutAction( const SPaintAction &inSettings )
				: PTAction( inSettings, str_UndoCut, true )
{
	this->ThrowIfFileLocked();
}

/*=====================================
	CutAction Destructor
======================================*/
PTCutAction::~PTCutAction()
{
}

/*=====================================
	CutAction::DoIt
======================================*/
void PTCutAction::DoIt()
{
	StSaveGWorld		aSaver;
	
	if ( mSettings.theSelection->IsEmpty() )		// shouldn't happen
	{
		delete this;
		return;
	}
	
	mSettings.thePaintView->CopyToUndo();
	
	mSettings.theSelection->CopyToClipboard();
	mSettings.theSelection->SelectNone();		// note: bypassing paint view
	mSettings.thePaintView->HandleCanvasDraw();
	
	this->PostAsAction();
}

	// ------------------ PTCopyAction --------------------
/*=====================================
	CopyAction Constructor
======================================*/
PTCopyAction::PTCopyAction( const SPaintAction &inSettings )
				: PTAction( inSettings, str_UndoGeneric, true )
{
	mAffectsUndoState = false;
}

/*=====================================
	CopyAction Destructor
======================================*/
PTCopyAction::~PTCopyAction()
{
}

/*=====================================
	CopyAction::DoIt
======================================*/
void PTCopyAction::DoIt()
{
	mSettings.theSelection->CopyToClipboard();
	this->PostAsAction();		// will delete this object (mAffectsUndoState == false)
}

	// ------------------ PTPasteAction --------------------
/*=====================================
	PTPasteAction Constructor
======================================*/
PTPasteAction::PTPasteAction( const SPaintAction &inSettings )
				: PTAction( inSettings, str_UndoPaste, true )
{
	this->ThrowIfFileLocked();
}

/*=====================================
	PTPasteAction Destructor
======================================*/
PTPasteAction::~PTPasteAction()
{
}

/*=====================================
	PTPasteAction::DoIt
======================================*/
void PTPasteAction::DoIt()
{
	mSettings.thePaintView->CopyToUndo();
	
	mSettings.thePaintView->CommitSelection();	// 8/29/96 ebs fix
	mSettings.thePaintView->ChangeTool( Tool_Selection );
	mSettings.theSelection->PasteFromClipboard( mSettings.currentBuffer );
	
		// this erases the old marching ants as well as displaying
		// the new selection
	mSettings.thePaintView->HandleCanvasDraw();
	
	this->PostAsAction();
}

	// ------------------ CClearAction --------------------
/*=====================================
	CClearAction Constructor
======================================*/
CClearAction::CClearAction( const SPaintAction &inSettings )
				: PTAction( inSettings, str_UndoClear, true )
{
	this->ThrowIfFileLocked();
}

/*=====================================
	CClearAction Destructor
======================================*/
CClearAction::~CClearAction()
{
}

/*=====================================
	CClearAction::DoIt
======================================*/
void CClearAction::DoIt()
{
	if ( mSettings.theSelection->IsEmpty() )		// shouldn't happen
	{
		delete this;
		return;
	}

	mSettings.thePaintView->CopyToUndo();
	
	mSettings.theSelection->SelectNone();		// note: bypassing paint view
	mSettings.thePaintView->HandleCanvasDraw();	
	this->PostAsAction();
}

