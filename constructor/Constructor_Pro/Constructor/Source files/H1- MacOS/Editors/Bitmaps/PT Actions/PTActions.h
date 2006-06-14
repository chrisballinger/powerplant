// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintAction.h

CLASSES:			PTAction, PTTrackingPaintAction, PTSelectionAction,
					plus other paint commands

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		97.02.16	ebs		Removed lasso stuff (into own file)
		96.4.18		ebs		Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	All paint commands are subclasses of PTAction.
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

#include "PT_Headers.h"
#include "PTSnapTo.h"

class PTPaintView;
class PTCanvas;
class SUOffscreen;
class PTPaintSelection;
class SUColorPane;
class PTDraggableTargetBox;

/*==========================================
	SPaintAction 
	
	This is passed to each paint action's constructor and contains the
	current settings for the paint view.
===========================================*/
typedef struct SPaintAction
{
	Color32				theForeColor;
	Color32				theBackColor;
	Pattern				thePattern;
	
	SUOffscreen			*currentBuffer;	
	SUOffscreen			*scratchBuffer;

	PTPaintView *		thePaintView;
	PTCanvas *			theCanvas;
	PTPaintSelection *	theSelection;
} SPaintAction;


/*==========================================
	PTAction 
	
	Superclass to all painting actions.
===========================================*/
class PTAction : public LAction
{
	public:
							PTAction( const SPaintAction &, SInt16 inStringIndex = str_UndoGeneric, Boolean isDone = true );
		virtual				~PTAction();
		
		/*==========================================
			DoIt() is used to perform the paint action, except for tracking
			actions which use HandleMouseDown().
		===========================================*/
		virtual void		DoIt();

		virtual MessageT	GetActionType();
		virtual Boolean		IsPostable() const;
		
	protected:
		SPaintAction		mSettings;
		MessageT			mActionType;			// used for move cmds only
		Boolean				mAffectsUndoState;		// is it undoable ?
		
		virtual void		RedoSelf();	
		virtual void		UndoSelf();
		virtual void		PostAsAction();	

		virtual Boolean		GetColorUnderPoint( Point inLocal, Color32 *outColor );
		virtual void		ThrowIfFileLocked();
};

/*==========================================
	PTTrackingPaintAction 
===========================================*/
class PTTrackingPaintAction : public PTAction
{
	public:
							PTTrackingPaintAction( const SPaintAction &, SInt16 inStringIndex = str_UndoGeneric, Boolean isDone = true );
		virtual				~PTTrackingPaintAction();
		
		virtual void		HandleMouseDown( const SMouseDownEvent & );
		
	protected:
		SInt32				mMouseDownRow, mMouseDownCol;
		PTSnapTo				mSnapTo;
		
		virtual void		MouseInitial( const SMouseDownEvent &, SInt32 newCol, SInt32 newRow );
		
		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
											 		
		virtual Boolean		MouseFinal( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
};


/*==========================================
	PTLassoAction 
===========================================*/
/*==========================================
	CEraserAction 
===========================================*/
class CEraserAction : public PTTrackingPaintAction
{
	public:
							CEraserAction( const SPaintAction & );
		virtual				~CEraserAction();

	protected:
		SInt32				mBufferCols, mBufferRows;
		SInt32				mLastColErased, mLastRowErased;
		SInt32				mEraserWidth;
		
		virtual void		MouseInitial( const SMouseDownEvent &, SInt32 newCol, SInt32 newRow );
		
		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
};

/*==========================================
	PTPenAction 
===========================================*/
class PTPenAction : public PTTrackingPaintAction
{
	public:
							PTPenAction( const SPaintAction & );
		virtual				~PTPenAction();

	protected:
		Color32				mPenColor;
		SInt32				mLastRowPainted, mLastColPainted;
		SInt32				mBufferCols, mBufferRows;
		
		virtual void		MouseInitial( const SMouseDownEvent &, SInt32 newCol, SInt32 newRow );
		
		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
};

/*==========================================
	PTDropperAction 
===========================================*/
class PTDropperAction : public PTTrackingPaintAction
{
	public:
						PTDropperAction( const SPaintAction &, SUColorPane *, SUColorPane * );
		virtual			~PTDropperAction();
		
		virtual void	HandleMouseDown( const SMouseDownEvent & );
		
	protected:
		SUColorPane	 	*mForePane, *mBackPane;
		
		virtual void	ChangeColor( Color32, Boolean isShiftDown = false );
};

/*==========================================
	PTBucketAction 
===========================================*/
class PTBucketAction : public PTTrackingPaintAction
{
	public:
						PTBucketAction( const SPaintAction & );
		virtual			~PTBucketAction();
		void 			HandleMouseDown( const SMouseDownEvent &inEvent );
};

/*==========================================
	CLineAction 
===========================================*/
class CLineAction : public PTTrackingPaintAction
{
	public:
							CLineAction( const SPaintAction & );
		virtual				~CLineAction();

	protected:
		Boolean				mScratchBufferHasImage;
		
		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
											 		
		virtual Boolean		MouseFinal( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
};

/*==========================================
	PTRectAction 
	
	Note:
	This stuff is used for RoundRects and Ovals too.
===========================================*/
class PTRectAction : public PTTrackingPaintAction
{
	public:
						PTRectAction( const SPaintAction &, Boolean isFilled, ResIDT = str_UndoRect );
		virtual			~PTRectAction();
		
	protected:
		Boolean				mFilled;
		Boolean				mScratchBufferHasImage;
		
		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
											 		
		virtual Boolean		MouseFinal( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
											
		virtual void		DrawShape( const Rect &r );
};

/*==========================================
	PTRoundRectAction
===========================================*/
class PTRoundRectAction : public PTRectAction
{
	public:
							PTRoundRectAction( const SPaintAction &, Boolean isFilled );
		virtual				~PTRoundRectAction();

	protected:
		virtual void		DrawShape( const Rect &r );
};


/*==========================================
	COvalAction 
===========================================*/
class COvalAction : public PTRectAction
{
	public:
							COvalAction( const SPaintAction &, Boolean isFilled );
		virtual				~COvalAction();

	protected:
		virtual void		DrawShape( const Rect &r );
};

/*==========================================
	PTEraseAllAction 
===========================================*/
class PTEraseAllAction : public PTAction
{
	public:
						PTEraseAllAction( const SPaintAction & );
		virtual			~PTEraseAllAction();
		virtual void	DoIt();
};

/*==========================================
	CMoveAction 
===========================================*/
class CMoveAction : public PTTrackingPaintAction
{
	public:
							CMoveAction( const SPaintAction & );
		virtual 			~CMoveAction();
		
		virtual void		MoveTo( SInt32 left, SInt32 top );
		virtual void		Move( SInt32 dh, SInt32 dv );
		
	protected:
		SInt32				mHOffsetWithinSelection;
		SInt32				mVOffsetWithinSelection;
		
		Boolean				mHaveMovedSinceMouseDown;
		Boolean				mSnappingHorizontal;

		virtual void		MoveToSelf( SInt32 left, SInt32 top );
		virtual void		MouseInitial( const SMouseDownEvent &, SInt32 newCol, SInt32 newRow );

		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
								
		virtual Boolean		MouseFinal( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
											
		virtual void		PostAsAction();	
};

/*==========================================
	PTRotateAction 
===========================================*/
class PTRotateAction : public PTAction
{
	public:
							PTRotateAction( const SPaintAction &, SInt32 degrees );
		virtual void		DoIt();
		
	protected:
		SInt32				mDegrees;
		
		virtual void		TransformTheBits( SUOffscreen *inSource, SUOffscreen *inDest,
											  SInt32 sourceWidth, SInt32 sourceHeight );
};


/*==========================================
	CTransparentAction 
===========================================*/
class CTransparentAction : public PTAction
{
	public:
							CTransparentAction( const SPaintAction & );
		virtual void		DoIt();
};

/*==========================================
	CTransformAction 
===========================================*/
class CTransformAction : public PTAction
{
	public:
							CTransformAction( const SPaintAction &, SInt16 inStringIndex );
		
		virtual void		DoIt();
		
	protected:
		virtual RgnHandle	CalcNewSelectionRegion();
		virtual void		TransformTheBits( SUOffscreen * ) = 0;
};

/*==========================================
	CFlipVerticalAction 
===========================================*/
class CFlipVerticalAction : public CTransformAction
{
	public:
							CFlipVerticalAction( const SPaintAction & );
		
	protected:
		virtual void		TransformTheBits( SUOffscreen * );
};


/*==========================================
	CFlipHorizontalAction 
===========================================*/
class CFlipHorizontalAction : public CTransformAction
{
	public:
							CFlipHorizontalAction( const SPaintAction & );
		
	protected:
		virtual void		TransformTheBits( SUOffscreen * );
};

/*==========================================
	CDragPictureAction 
===========================================*/
typedef struct
{
	ResType				imageType;		// DragFlavor_Offscreen or ImageType_Picture
	void				*data;			// SUOffscreen * or Picture handle
	DragReference		dragRef;
	DragAttributes		dragAttribs;
} SDragImageInfo;


class PTDragImageAction : public PTAction
{
	public:
							PTDragImageAction( const SPaintAction &, SDragImageInfo * );
		virtual void		DoIt();
		
	protected:
		SDragImageInfo		mDragInfo;
};

/*==========================================
	PTHotspotAction 
===========================================*/
class PTHotSpotAction : public PTTrackingPaintAction
{
	public:
							PTHotSpotAction( const SPaintAction & );
		virtual				~PTHotSpotAction();
	
	protected:
		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
};


/*==========================================
	PTDeleteImageAction 
===========================================*/
class PTDeleteImageAction : public PTAction
{
	public:
								PTDeleteImageAction( const SPaintAction & );
		virtual					~PTDeleteImageAction();
		virtual void			DoIt();
		
	protected:
		PTDraggableTargetBox *	mTargetBox;
		virtual void			RedoSelf();
};

