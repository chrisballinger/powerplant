// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SelectionAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTSelectionAction

DESCRIPTION:		Implementation file for classes
					See also LassoAction.cp
										
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.12.13	ebs		Fixed shift key (broke yesterday). Made easier 0,0 selection.
		96.12.12	ebs		Selection criteria better (Koziol bug)
		96.08.27	ebs		Selection of last row/col fixed
		96.08.03	ebs		Marching ants during selection
		96.07.25	ebs		Fixed visual glitch - leftover pixels during tracking sometimes
		96.04.23	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTSelectionAction.h"
#include "SUMarchingAnts.h"

/*==================================
	Constructor
===================================*/
PTSelectionAction::PTSelectionAction( const SPaintAction &inAction ) : PTTrackingPaintAction( inAction )
{
	SetRect( &mPrevDrawnRect, 0, 0, 0, 0 );
	SetRect( &mCurrentSelection, 0, 0, 0, 0 );
	mAffectsUndoState = false;					// since we're not undoable
}

/*==================================
	Destructor
===================================*/
PTSelectionAction::~PTSelectionAction()
{
}

/*==================================
	HandleMouseDown
===================================*/
void PTSelectionAction::HandleMouseDown( const SMouseDownEvent &inEvent )
{
	SInt32	rowHit, colHit;
	
	/*****************************************
		if the user clicks within the current selection, we will 
		instantiate a move tool and kill ourselves.
	*****************************************/
	if ( mSettings.theCanvas->MapPointToCell( inEvent.whereLocal, &colHit, &rowHit ) )
		if ( mSettings.theSelection->PointInSelection( colHit, rowHit ) )
		{
			CMoveAction	*theMover = new CMoveAction( mSettings );
			ThrowIfMemFail_( theMover );
			delete this;
			theMover->HandleMouseDown( inEvent );
			return;
		}
	
	/*****************************************
		we need to draw in xor mode if there's no space between pixels
		on the canvas. otherwise, we'll draw in patCopy mode because
		this flickers less.
	*****************************************/
	mUseXorMode = (mSettings.theCanvas->GetSpaceBetween() == 0);

	/*****************************************
		just do the normal mouse tracking stuff
	*****************************************/
	PTTrackingPaintAction::HandleMouseDown( inEvent );
}

/*==================================
	MouseInitial
===================================*/
void PTSelectionAction::MouseInitial( const SMouseDownEvent &inEvent, SInt32 /*newCol*/, SInt32 /*newRow*/ )
{
	mMouseDownPt = inEvent.whereLocal;
	mSettings.thePaintView->SelectNone();
}

/*
	Selection rules (according to tests & conversation with Dave Koziol):
	
	The direction the mouse has moved relative to the first pixel
	  is important when determining if a pixel gets selected or not.
	  
	A pixel doesn't get selected if just clicked on, the mouse must move
	  past one of its boundaries.
	  
	The initial pixel clicked on is tricker. It gets selected only if
	  more than 1/2 of it has been enclosed by the selection rect.
*/

/*==================================
	MouseStillDown
===================================*/
void PTSelectionAction::MouseStillDown( const SMouseDownEvent &, 
										Point, Point currentPt,
										SInt32 /*prevCol*/, SInt32 /*prevRow*/,
										SInt32 /*newCol*/, SInt32 /*newRow*/ )
{
	Rect			r, rectToDraw, clipR;
	PTCanvas		*theCanvas = mSettings.theCanvas;
	SInt32			h1, h2, v1, v2;
	Boolean			movingDown, movingRight;
	
	/*****************************************************
		1/2 of the cell area is a good amount of slop
	*****************************************************/
	Rect tempR;
	theCanvas->GetPixelRect( 0, 0, &tempR );
	SInt16 slop = MAX( 0, (tempR.right - tempR.left) / 2 );

	/*****************************************************
		find the cell range that has been hit
	*****************************************************/
	SInt32 	initialCellH, initialCellV;
	SInt32	currentCellH, currentCellV;
	Rect	initialCellR, currentCellR;
	
	theCanvas->MapPointToCell( mMouseDownPt, &initialCellH, &initialCellV, true );
	theCanvas->MapPointToCell( currentPt, &currentCellH, &currentCellV );
	theCanvas->GetPixelRect( initialCellH, initialCellV, &initialCellR, false );
	theCanvas->GetPixelRect( currentCellH, currentCellV, &currentCellR, false );
	
	/*****************************************************
		find the cell range that should be hilited
	*****************************************************/
	if ( (initialCellH == currentCellH) || (initialCellV == currentCellV) )
	{
		h1 = 0; h2 = 0;			// mouse has to move outside cell to select anything
		v1 = 0; v2 = 0;
	}
	else
	{
		if ( currentCellH > initialCellH )		// moving right
		{
			movingRight = true;
			h1 = initialCellH;
			h2 = currentCellH;
			
			if ( mMouseDownPt.h + slop > initialCellR.right )
				++h1;
		}
		else									// left
		{
			movingRight = false;
			h1 = currentCellH + 1;
			h2 = initialCellH + 1;
			
			if ( mMouseDownPt.h - slop < initialCellR.left )
				--h2;
		}
		
		if ( currentCellV > initialCellV )		// moving down
		{
			movingDown = true;
			v1 = initialCellV;
			v2 = currentCellV;
			
			if ( mMouseDownPt.v + slop > initialCellR.bottom )
				++v1;
		}
		else									// up
		{
			movingDown = false;
			v1 = currentCellV + 1;
			v2 = initialCellV + 1;
			
			if ( mMouseDownPt.v - slop < initialCellR.top )
				--v2;
		}
	}
	
	/*****************************************************
		snap to a rectangle if the shift key is down
		(need to handle all 4 combinations of left/right/up/down)
	*****************************************************/
	SInt32 &tempH1 = movingRight ? h1 : h2;
	SInt32 &tempH2 = movingRight ? h2 : h1;
	SInt32 &tempV1 = movingDown ? v1 : v2;
	SInt32 &tempV2 = movingDown ? v2 : v1;
	
	mSnapTo.SnapTo( tempH1, tempV1, tempH2, tempV2, &tempH2, &tempV2 );
	
	/*****************************************************
		never extend a selection beyond the buffer (except by 1 pixel since
		we need to enclose the selection)
	*****************************************************/
	r.left = MAX( 0, h1 );
	r.right = MIN( 1 + mSettings.currentBuffer->GetWidth(), h2 );
	r.top = MAX( 0, v1 );
	r.bottom = MIN( 1 + mSettings.currentBuffer->GetHeight(), v2 );

	/*****************************************************
		prepare to draw the selection
	*****************************************************/
	theCanvas->MapRectToCanvas( r, &rectToDraw );

	theCanvas->FocusDraw();
	StColorPenState		aSaver;
	aSaver.Normalize();
	
	theCanvas->GetInsideDrawingRect( &clipR );
	StClipRgnState	saveSetAndRestore( clipR );

	/*****************************************************
		even if the rect hasn't changed, we may have to march the ants
	*****************************************************/
	if ( ::EqualRect( &rectToDraw, &mPrevDrawnRect ) ) 
	{
		if ( mMarchingAnts.TimeToDraw() )
		{
			if ( mUseXorMode )				// less flicker if we don't erase (drawing on top anyway)
				this->EraseOldRect();
			this->DrawNewRect( rectToDraw );
		}

		mCurrentSelection = r;
		return;
	}
	
	/*****************************************************
		mouse has moved, so erase the old rect and draw the new one
	*****************************************************/
	this->EraseOldRect();
	this->DrawNewRect( rectToDraw );
	
	mCurrentSelection = r;					// in source coords -- not canvas
}
#ifdef NOTUSED_XXX
/*==================================
	MouseStillDown
===================================*/
void PTSelectionAction::MouseStillDown( const SMouseDownEvent &, 
										Point, Point currentPt,
										Int32 /*prevCol*/, Int32 /*prevRow*/,
										Int32 /*newCol*/, Int32 /*newRow*/ )
{
	SUOffscreen		*theImageBuffer = mSettings.currentBuffer;
	SUOffscreen		*theScratchBuffer = mSettings.scratchBuffer;
	Rect			r, rectToDraw, clipR;
	PTCanvas		*theCanvas = mSettings.theCanvas;
	Boolean			goingRight, goingDown;
	Point			rawTopLeft, rawBotRight;
	
	/*****************************************************
		find the raw top-left and bottom-right of the selection area
	*****************************************************/
	if ( currentPt.h > mMouseDownPt.h )
	{
		goingRight = true;
		rawTopLeft.h = mMouseDownPt.h;
		rawBotRight.h = currentPt.h;
	}
	else
	{
		goingRight = false;
		rawTopLeft.h = currentPt.h;
		rawBotRight.h = mMouseDownPt.h;
	}
	
	if ( currentPt.v > mMouseDownPt.v )
	{
		goingDown = true;
		rawTopLeft.v = mMouseDownPt.v;
		rawBotRight.v = currentPt.v;
	}
	else
	{
		goingDown = false;
		rawTopLeft.v = currentPt.v;
		rawBotRight.v = mMouseDownPt.v;
	}
	
	/*****************************************************
		find the cell range that has been hit
	*****************************************************/
	Int32 h, h2, v, v2;
	theCanvas->MapPointToCell( rawTopLeft, &h, &v );
	theCanvas->MapPointToCell( rawBotRight, &h2, &v2 );
	
	/*****************************************************
		do the tricky boundary fixes
	*****************************************************/
	Rect	topLeftCellR, botRightCellR;
	Int16	slop;
	
	theCanvas->GetPixelRect( h, v, &topLeftCellR, false );
	theCanvas->GetPixelRect( h2, v2, &botRightCellR, false );
	
	Rect tempR;
	theCanvas->GetPixelRect( 0, 0, &tempR );		// note: above rects may be empty
	slop = MAX( 0, (tempR.right - tempR.left) / 3 );
	
	if ( goingRight )
	{
		if ( rawTopLeft.h + slop > topLeftCellR.right )
			++h;
	}
	else	// left
	{
		if ( ::PtInRect( currentPt, &botRightCellR ) )
		{
			h = 0;
			h2 = 0;
		}
	}
	
	if ( goingDown )
	{
		if ( rawTopLeft.v + slop > topLeftCellR.bottom )
			++v;
	}
	else	// up
	{
		if ( ::PtInRect( currentPt, &botRightCellR ) )
		{
			v = 0;
			v2 = 0;
		}
	}
	
	/*****************************************************
		never extend a selection beyond the buffer (except by 1 pixel since
		we need to enclose the selection)
	*****************************************************/
	r.left = MAX( 0, h );
	r.right = MIN( 1 + mSettings.currentBuffer->GetWidth(), h2 );
	r.top = MAX( 0, v );
	r.bottom = MIN( 1 + mSettings.currentBuffer->GetHeight(), v2 );

	theCanvas->MapRectToCanvas( r, &rectToDraw );

	theCanvas->FocusDraw();
	StColorPenState		aSaver;
	aSaver.Normalize();
	
	theCanvas->GetInsideDrawingRect( &clipR );
	StClipRgnState	saveSetAndRestore( clipR );

	/*****************************************************
		even if the rect hasn't changed, we may have to march the ants
	*****************************************************/
	if ( ::EqualRect( &rectToDraw, &mPrevDrawnRect ) ) 
	{
		if ( mMarchingAnts.TimeToDraw() )
		{
			if ( mUseXorMode )				// less flicker if we don't erase (drawing on top anyway)
				this->EraseOldRect();
			this->DrawNewRect( rectToDraw );
		}

		mCurrentSelection = r;
		return;
	}
	
	/*****************************************************
		mouse has moved, so erase the old rect and draw the new one
	*****************************************************/
	this->EraseOldRect();
	this->DrawNewRect( rectToDraw );
	
	mCurrentSelection = r;					// in source coords -- not canvas
}
#endif

/*==================================
	EraseOldRect
===================================*/
void PTSelectionAction::EraseOldRect()
{
	StColorPenState		aPenState;
	aPenState.Normalize();
	
	if ( EmptyRect( &mPrevDrawnRect ) )
		return;
	
	if ( mUseXorMode )
	{
		Pattern		thePattern;
		mMarchingAnts.GetPattern( &thePattern );
		::PenPat( &thePattern );
		::PenMode( patXor );
		::RGBForeColor( &kBlackColor );
		::RGBForeColor( &kWhiteColor );
		::FrameRect( &mPrevDrawnRect );
	}
	else
	{
		::RGBForeColor( &kWhiteColor );
		::FrameRect( &mPrevDrawnRect );
	}		
}

/*==================================
	DrawNewRect
===================================*/
void PTSelectionAction::DrawNewRect( const Rect &inRect )
{
	StColorPenState		aPenState;
	aPenState.Normalize();

	if ( mMarchingAnts.TimeToDraw() )
		mMarchingAnts.IncrementPattern();
	
	Pattern		thePattern;
	mMarchingAnts.GetPattern( &thePattern );
	::PenPat( &thePattern );

	if ( mUseXorMode )
		::PenMode( patXor );
	::FrameRect( &inRect );
	
	mPrevDrawnRect = inRect;
}

/*==================================
	MouseFinal
===================================*/
Boolean PTSelectionAction::MouseFinal( const SMouseDownEvent &, 
									Point, Point,
									SInt32, SInt32,
									SInt32, SInt32 )
{
		// don't select anything outside the current image
	mCurrentSelection.top = MAX( 0, mCurrentSelection.top );
	mCurrentSelection.bottom = MIN( mSettings.currentBuffer->GetHeight(), mCurrentSelection.bottom );
	mCurrentSelection.left = MAX( 0, mCurrentSelection.left );
	mCurrentSelection.right = MIN( mSettings.currentBuffer->GetWidth(), mCurrentSelection.right );
	
	if ( !EmptyRect( &mCurrentSelection ) )
	{
			// set the current selection
		mSettings.thePaintView->SetSelection( mCurrentSelection );
	}
	
	return( false );				// not an undoable action
}
