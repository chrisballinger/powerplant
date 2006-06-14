// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				MoveAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			CMoveAction

DESCRIPTION:		Implementation file for class
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.07.08	ebs		Fixed illegal use of instance variable after "this" was disposed
							in MoveTo()
		96.04.23	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

/*
	Notes: 
	
	(1)	This can be called one of two ways:
			HandleMouseDown (MouseInitial, MouseStillDown, MouseFinal)
			Move or MoveTo 
		The former is for the mouse, the latter is for the arrow keys
		
	(2) This is a bit tricky to keep track of, but does what is
	    expected of a move tool in a paint program.
		      
		When a selection is made, the area underneath the selection
		is erased to the background color right away. The selection
		then floats above this area. If the selection is cut, cleared,
		or moved, the backround color shows up. If the selection is
		set to none, it replaces the background color.
		
		This works with moving the selection 1 pixel at a time
		via the keyboard too. Only the initial selection area gets
		erased. 
		
	(3)	Option-drags duplicate the selection. Since we had previously
		erased the area to the background color, we "unerase" it by
		copying the selection buffer back into the image before starting
		the drag.
		
	(4) Unlike most other actions, undoing a move undoes all of the
	    previous moves as well. For example, if the user hits the
	    up arrow key 4 times and then selects Undo, we restore the
	    initial state rather than the most recent one.
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"
#include "PTUndoer.h"					// for cmd_GetLastCommand

/*==================================
	Constructor
===================================*/
CMoveAction::CMoveAction( const SPaintAction &inAction )
				: PTTrackingPaintAction( inAction, str_UndoMove )
{
	this->ThrowIfFileLocked();

	mActionType = cmd_MoveSelection;	// paintview looks at this to see if a move is already in progress
	
	/*********************************************
		if previous action was also a move, this particular one will not
		be undoable because that one will be remembered instead.
	*********************************************/
	LAction		*lastAction = nil;
	mSettings.thePaintView->ProcessCommand( cmd_GetLastCommand, &lastAction );
	
	/*
		NOTE: Constructor LActions don't have the GetActionType() method, so the dynamic
				case below is necessary. If ourLastAction is nil, it isn't a PTAction.
	*/
	PTAction	*ourLastAction = dynamic_cast<PTAction*>( lastAction );
	if ( ourLastAction && (ourLastAction->GetActionType() == cmd_MoveSelection) && ourLastAction->IsDone() )
		mAffectsUndoState = false;					// we're not undoable
}

/*==================================
	Destructor
===================================*/
CMoveAction::~CMoveAction()
{
}

/*==================================
	Move
===================================*/
void CMoveAction::Move( SInt32 dh, SInt32 dv )
{
	if ( mSettings.theSelection->IsEmpty() )
	{
		delete this;
		return;
	}
	
	RgnHandle	selRgn = mSettings.theSelection->GetRegion();
	
	Rect theRect;
#if PP_Target_Carbon
	::GetRegionBounds(selRgn, &theRect);
#else
	theRect = (**selRgn).rgnBBox;
#endif
	
	this->MoveTo( theRect.left + dh, theRect.top + dv );
}

/*==================================
	MoveTo
===================================*/
void CMoveAction::MoveTo( SInt32 left, SInt32 top )
{
	if ( mSettings.theSelection->IsEmpty() )
	{
		delete this;
		return;
	}

	StSaveGWorld	aSaver;
	PTPaintView		*thePaintView = mSettings.thePaintView;		// put in local since "this" may go away

	if ( mAffectsUndoState )
		thePaintView->CopyToUndo();
	
	this->MoveToSelf( left, top );
	this->PostAsAction();		// note: may destroy "this" (so don't use instance vars anymore)
	
	/*
		in the case where the user saves while a move is in progress
		(using the arrow keys), this flag wasn't being set.
	*/
	thePaintView->SetChangedFlag( true );
}

/*==================================
	MoveToSelf
===================================*/
void CMoveAction::MoveToSelf( SInt32 left, SInt32 top )
{
		// move the selection region (internal change -- no visuals involved)
	mSettings.theSelection->MoveTo( left, top );
	
		// copy the "real" image to the scratch buffer
	mSettings.currentBuffer->CopyTo( mSettings.scratchBuffer );
	
		// draw the selection buffer on top
	mSettings.theSelection->DrawInto( mSettings.scratchBuffer );

		// blit the combination to the canvas
	mSettings.theCanvas->DrawFrom( mSettings.scratchBuffer );
}

/*==================================
	MouseInitial
===================================*/
void CMoveAction::MouseInitial( const SMouseDownEvent &inEvent, SInt32 newCol, SInt32 newRow )
{
	RgnHandle	selectionRgn = mSettings.theSelection->GetRegion();
	
	ThrowIfNil_( selectionRgn );			// shouldn't happen
	
	Rect theRect;
#if PP_Target_Carbon
	::GetRegionBounds(selectionRgn, &theRect);
#else
	theRect = (**selectionRgn).rgnBBox;
#endif
	mHOffsetWithinSelection = newCol - theRect.left;
	mVOffsetWithinSelection = newRow - theRect.top;

	if ( inEvent.macEvent.modifiers & shiftKey )
		mSnapTo.SetAngle( PTSnapTo::kSnapTo90 );
	else
		mSnapTo.SetAngle( PTSnapTo::kDontSnap );
	
	/*********************************************
		option-drag duplicates the current selection. the area underneath
		the selection has already been erased, so we'll "unerase" it.
	*********************************************/
	if ( inEvent.macEvent.modifiers & optionKey )
		mSettings.theSelection->DrawInto( mSettings.currentBuffer );	
}

/*==================================
	MouseStillDown
===================================*/
void CMoveAction::MouseStillDown( const SMouseDownEvent &, 
									Point /*prevMousePt*/, Point /*newMousePt*/,
									SInt32 prevCol, SInt32 prevRow,
									SInt32 newCol, SInt32 newRow )
{
	if ( (newCol == prevCol) && (newRow == prevRow) )
		return;

	this->MoveToSelf( newCol - mHOffsetWithinSelection, newRow - mVOffsetWithinSelection );
}

/*==================================
	PostAsAction
===================================*/
void CMoveAction::PostAsAction()
{
	/*
		the move action is a bit different than other actions in that
		it may not be "undoable" but still modifies the image. this is
		because we undo back to the first move rather than the latest
		one. since it still modifies the image, we need to update the
		sample icons. (If we're postable/undoable, the sample view is
		automatically redrawn anyway, do don't bother to do it twice).
	*/	
	PTPaintView *thePaintView = this->IsPostable() ? nil : mSettings.thePaintView;
	
		// note: this call may destroy "this", so don't use instance variables afterwards
	PTTrackingPaintAction::PostAsAction();
	
	if ( thePaintView )
		thePaintView->RedrawSampleView();
}

/*=====================================
	MouseFinal
======================================*/
Boolean CMoveAction::MouseFinal( const SMouseDownEvent &, 
								Point, Point,
								SInt32, SInt32,
								SInt32, SInt32 )
{
	/*******************************************
		whether we can undo depends on whether we copied ourselves into
		the undo buffer when we were created (if the previous action was
		anything but a move action).
	*******************************************/
	return( mAffectsUndoState );
}


