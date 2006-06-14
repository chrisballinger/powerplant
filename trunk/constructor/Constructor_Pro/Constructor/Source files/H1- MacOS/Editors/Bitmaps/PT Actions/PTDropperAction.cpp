// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				DropperAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTDropperAction

DESCRIPTION:		This code is for the eye dropper paint tool
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.07.12	ebs		Move to Color32
		96.04.23	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/
 
	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"
#include "SUColorPane.h"
 

/*===================================
	Constructor
====================================*/
PTDropperAction::PTDropperAction( const SPaintAction &inAction, SUColorPane *inForePane, SUColorPane *inBackPane ) 
					: PTTrackingPaintAction( inAction )
{
	mForePane = inForePane;
	mBackPane = inBackPane;
	mAffectsUndoState = false;				// since we're not undoable
}

/*===================================
	Destructor
====================================*/
PTDropperAction::~PTDropperAction()
{
}

/*===================================
	HandleMouseDown
====================================*/
void PTDropperAction::HandleMouseDown( const SMouseDownEvent &inEvent )
{
	Point			pt;
	Color32			newColor;
	StSaveGWorld	aGWorldSaver;

		// get the initial color
	if ( this->GetColorUnderPoint( inEvent.whereLocal, &newColor ) )
		this->ChangeColor( newColor, (inEvent.macEvent.modifiers & shiftKey) ? true : false );
		
		// loop until the mouse is released
	while( StillDown() )
	{
		SUMiscUtils::GetMouseRelativeToView( mSettings.theCanvas, &pt );	
		
		if ( this->GetColorUnderPoint( pt, &newColor ) )
			this->ChangeColor( newColor, SUMiscUtils::IsShiftKey() );
	}
	
		// delete this object (since mAffectsUndoState == false)
	this->PostAsAction();	
}

/*===================================
	ChangeColor
====================================*/
void PTDropperAction::ChangeColor( Color32 inColor, Boolean isShiftDown )
{
	if ( !mForePane || !mBackPane ) return;			// shouldn't happen
	
	if ( isShiftDown )
		mBackPane->SetColor( inColor, redraw_Now );
	else if ( mForePane ) 
		mForePane->SetColor( inColor, redraw_Now );
}

#ifdef PREV_VERSION
/*===================================
	ChangeColor
====================================*/
void PTDropperAction::ChangeColor( ColorIndex inColor, Boolean isShiftDown )
{
	if ( isShiftDown )
	{
		if ( mBackPane ) 
			mBackPane->SetColor( inColor, true  );
	}
	else if ( mForePane ) 
		mForePane->SetColor( inColor, true  );
}
#endif

