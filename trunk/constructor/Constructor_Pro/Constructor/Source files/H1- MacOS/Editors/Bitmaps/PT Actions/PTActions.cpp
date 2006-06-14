// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintActions.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTAction, PTTrackingPaintAction

DESCRIPTION:		These are the superclasses for all paint actions

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.08.28	ebs		Support for locked files
		96.6.24		ebs		Integrated into Eric Scouten's code
		96.4.18		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"

	// ---------------------------- PTAction ------------------
/*================================
	PTAction Constructor
=================================*/
PTAction::PTAction( const SPaintAction &inSettings, SInt16 inStringIndex, Boolean inDone ) : 
	LAction( STRx_PaintUndo, inStringIndex, inDone )
{
	mSettings = inSettings;
	mActionType = 0;
	mAffectsUndoState = true;					// assume undo-able
}

/*================================
	PTAction Destructor
=================================*/
PTAction::~PTAction()
{
}

/*================================
	PTAction::RedoSelf
=================================*/
void PTAction::RedoSelf()
{
	mSettings.thePaintView->SwapUndoState();
}

/*================================
	PTAction::UndoSelf
=================================*/
void PTAction::UndoSelf()
{
	mSettings.thePaintView->SwapUndoState();
}

/*================================
	PTAction::PostAsAction
=================================*/
void PTAction::PostAsAction()
{
	if ( mSettings.thePaintView )
	{
			// all undo-able actions set the changed flag
		if ( this->IsPostable() )
		{
			mSettings.thePaintView->SetChangedFlag( true );	
			mSettings.thePaintView->RedrawSampleView();
		}
	
			// note: PostAction might delete "this"
		mSettings.thePaintView->PostAction( this );
	}
}

/*===================================
	PTAction::GetColorUnderPoint
====================================*/
Boolean PTAction::GetColorUnderPoint( Point inLocal, Color32 *outColor )
{
	SInt32		h, v;
	
	if ( mSettings.theCanvas && mSettings.theCanvas->MapPointToCell( inLocal, &h, &v ) )
	{
		*outColor = mSettings.currentBuffer->GetPixelColor( h, v );
		return( true );
	}
	
	return( false );
}

/*===================================
	PTAction::DoIt
====================================*/
void PTAction::DoIt()
{
	Throw_( paramErr );			// this should never be called
}

/*==================================
	PTAction::GetActionType
===================================*/
MessageT PTAction::GetActionType()
{
	return( mActionType );
}

/*==================================
	PTAction::IsPostable
===================================*/
Boolean PTAction::IsPostable() const
{
	return( mAffectsUndoState );
}

/*==================================
	PTAction::ThrowIfFileLocked
===================================*/
void PTAction::ThrowIfFileLocked()
{
	if ( mSettings.thePaintView && mSettings.thePaintView->GetLockFlag() )
		Throw_( err_FileLocked );
}

	// ------------------------- PTTrackingPaintAction ----------------------
	
/*==================================
	PTTrackingPaintAction Constructor
===================================*/
PTTrackingPaintAction::PTTrackingPaintAction( const SPaintAction &inAction, SInt16 inStringIndex, Boolean isDone )
									: PTAction( inAction, inStringIndex, isDone )
{
}

/*==================================
	PTTrackingPaintAction Destructor
===================================*/
PTTrackingPaintAction::~PTTrackingPaintAction()
{
}

/*==================================
	PTTrackingPaintAction::HandleMouseDown
===================================*/
void PTTrackingPaintAction::HandleMouseDown( const SMouseDownEvent &inMouseDown )
{
	Point				newMousePt, prevMousePt;
	SInt32				prevRow = -1, prevCol = -1, newRow = -1, newCol = -1;
	StSaveGWorld		aSaver;						// save/restore gdevice & port
	
	/*********************************************
		see where the mouse went down within the canvas
	*********************************************/	
	if ( !mSettings.theCanvas->MapPointToCell( inMouseDown.whereLocal, &mMouseDownCol, &mMouseDownRow ) )
		return;			// missed it
	
	/*********************************************
		prepare for drawing
	*********************************************/	
	mSettings.currentBuffer->ResetPen();
	mSettings.scratchBuffer->ResetPen();
	
	mSettings.theCanvas->FocusDraw();
	prevMousePt = inMouseDown.whereLocal;
	if ( mAffectsUndoState )
		mSettings.thePaintView->CopyToUndo();
	
	/*********************************************
		this lets the subclass know the mouse went down
	*********************************************/	
	this->MouseInitial( inMouseDown, mMouseDownCol, mMouseDownRow );
	
	/*********************************************
		loop until the mouse is released
	*********************************************/	
	while( StillDown() )
	{
		/*********************************************
			Get the current mouse location, relative to the superview of the canvas
		*********************************************/
		SUMiscUtils::GetMouseRelativeToView( mSettings.theCanvas, &newMousePt );

		/*********************************************
			convert this into a col/row within the bitmap
		*********************************************/
		mSettings.theCanvas->MapPointToCell( newMousePt, &newCol, &newRow );

		/*********************************************
			if the shift key is down, snap to the 45 or 90 degrees
		*********************************************/
		mSnapTo.SnapTo( mMouseDownCol, mMouseDownRow, newCol, newRow, &newCol, &newRow );		
		
		/*********************************************
			Call the subclass to handle the tool tracking
		*********************************************/	
		this->MouseStillDown( inMouseDown, prevMousePt, newMousePt, prevCol, prevRow, newCol, newRow );

		/*********************************************
			prepare for next loop
		*********************************************/	
		prevMousePt = newMousePt;
		prevRow = newRow;
		prevCol = newCol;
	}
	
	/*********************************************
		Send a final call to the subclass
	*********************************************/
	mAffectsUndoState = this->MouseFinal( inMouseDown, prevMousePt, newMousePt, prevCol, prevRow, newCol, newRow );

	/*********************************************
		this will post the action if undoable, or just delete the object
	*********************************************/
	this->PostAsAction();
}

/*====================================================
	PTTrackingPaintAction::MouseInitial
=====================================================*/
void PTTrackingPaintAction::MouseInitial( const SMouseDownEvent &, SInt32, SInt32 )
{
}

/*====================================================
	PTTrackingPaintAction::MouseStillDown
=====================================================*/
void PTTrackingPaintAction::MouseStillDown( const SMouseDownEvent &, 
											Point, Point,
											SInt32, SInt32,
											SInt32, SInt32 )
{
}
/*====================================================
	PTTrackingPaintAction::MouseFinal
	
	Returns:
		true -> can undo, false -> can't undo
=====================================================*/
Boolean PTTrackingPaintAction::MouseFinal( const SMouseDownEvent &, 
											Point, Point,
											SInt32, SInt32,
											SInt32, SInt32 )
{
	return( true );				// default to "undoable"
}

