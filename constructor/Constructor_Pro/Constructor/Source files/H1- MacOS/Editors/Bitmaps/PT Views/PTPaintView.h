// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		96.11.11	ebs		Text editing support
*/

#pragma once

class SUOffscreen;
class SUSharedPalette;
class SUColorPane;
class SUPatternPane;
class SUColorCursorCache;

class PTCanvas;
class PTAction;
class PTPaintSelection;
class PTColorTableChoice;
class PTDraggableTargetBox;
class PTTextAction;

class RFMap;

#include "SUOffscreen.h"	
#include "PT_Types.h"
#include "PTImageDragMixin.h"
#include "UTextTraits.h"

const SInt32	PaintWindowMargin_h		= 8;
const SInt32	PaintWindowMargin_v		= 8;


class PTPaintView : public LWindow, public LBroadcaster, public LListener, public LPeriodical
{
	public:
		enum { class_ID = 'PNTG' };
		
			// creation & deletion
									PTPaintView( LStream *inStream );
		virtual void				FinishCreateSelf();
		static PTPaintView*			CreatePaintStream( LStream *inStream );
		virtual						~PTPaintView();

			// image display
		virtual void				SetImage( SUOffscreen *, PTResizeOptions = PTResize_None,
												ERedrawOptions = redraw_Later );
												
		/**********************************
			Getting our buffers
		**********************************/
		virtual SUOffscreen *		GetImageBuffer();
		virtual SUOffscreen *		GetScratchBuffer();
		virtual SUOffscreen *		GetUndoBuffer();
		virtual SUOffscreen *		GetCombinedBuffer();
		
		/**********************************
			Setting our buffers -- these are low-level routines not generally used
		**********************************/
		virtual void				SetRawImageBuffer( SUOffscreen * );
		virtual void				SetRawScratchBuffer( SUOffscreen * );
		virtual void				SetRawUndoBuffer( SUOffscreen * );

		/**********************************
			Getting our sample panes
		**********************************/
		virtual SInt32				GetSamplePaneCount();
		virtual PTDraggableTargetBox *GetNthSamplePane( SInt32 index ); // 1..n
		
		/**********************************
			Misc flags
		**********************************/
		virtual void				SetChangedFlag( Boolean );
		virtual Boolean				GetChangedFlag();
		virtual void				RedrawSampleView( Rect *r = nil );
		
			// events
		virtual Boolean 			HandleKeyPress( const EventRecord &inKeyEvent );
		virtual void				ActivateSelf();
		virtual void				DeactivateSelf();
		
			// events for paint palette
		virtual void				AdjustCursorInCanvas( Point, const EventRecord & );
		virtual void				HandleMouseDownInCanvas( const SMouseDownEvent & );
		virtual void				HandleCanvasDraw();
	
			// undo/redo
		virtual void				CopyToUndo();
		virtual void				SwapUndoState();
		
			// commands & messages
		virtual void				ListenToMessage(MessageT inMessage, void *ioParam);
		virtual Boolean				ObeyCommand( CommandT, void * );
		virtual void 				FindCommandStatus( CommandT inCommand, Boolean &outEnabled, 
										Boolean &outUsesMark, UInt16 &outMark, Str255 outName );
		virtual PTAction *			GetLastAction();
		virtual void				SetTextAction( PTTextAction * = nil );
		virtual void				GetTextTraits( struct TextTraitsRecord * );
		//virtual void				SetTextTraits( struct TextTraitsRecord * );	
		
			// selection
		virtual void				SelectAll();
		virtual void				SelectNone();
		virtual void				CommitSelection();
		virtual void				EraseAreaUnderSelection();
		virtual void				SetSelection( const Rect &r, Boolean toLasso = false );
		virtual void				SetSelection( RgnHandle, Boolean toLasso = false );
		virtual void				SpendTime( const EventRecord &inMacEvent );
		virtual void				NudgeSelection( SInt32 dh, SInt32 dv );
		virtual Boolean				CanvasPointInSelection( Point canvasPt );
		
			// target box
		virtual void				SetTargetBox( PTDraggableTargetBox *, ERedrawOptions = redraw_Later );
		virtual PTDraggableTargetBox *GetTargetBox();
		
			// misc
		virtual void				ChangeTool( OSType toWhat );
		virtual void				SaveAsResource( RFMap *inMap, ResIDT inResID );	// == 0 ???
		//virtual void				DoSave() = 0;
		virtual void				AttemptClose();
		
		virtual Boolean				GetLockFlag();
		virtual void				SetLockFlag( Boolean );
	
			// resizing	
		virtual SInt32				GetMinImageHeight();
		virtual SInt32				GetMaxImageHeight();
		virtual SInt32				GetMinImageWidth();
		virtual SInt32				GetMaxImageWidth();

		virtual void				ChangeImageSize( SInt32 inWidth, SInt32 inHeight, Boolean inStretch );
		virtual void				ImageSizeChangeUndone( SInt32 inWidth, SInt32 inHeight );
	
		virtual SInt32				GetZoomFactor( SInt32 inImageWidth, SInt32 inImageHeight, Boolean *outShowGrid );
		
			// fixes for bugs in PowerPlant when working with GWorlds
		virtual void				UpdatePort();
		virtual Boolean				EstablishPort();
		
	protected:
		Boolean						mChanged;
		Boolean						mLockedFlag;
		RgnHandle					mSpareRgn;
		
		OSType						mCurrentTool;
		OSType						mPreviousTool;		// for reverting on eraser
		
		Color32						mForeColor, mBackColor;
		SUColorPane					*mColorPane, *mBackColorPane;
		Boolean						mPrefersIconColors;	// true for icons, false for PICTs & others
		
		Pattern						mCurrentPattern;
		SUPatternPane				*mPatternPane;
		
		PTCanvas					*mCanvas;
		LPane						*mSampleWell;
		
		SUOffscreen					*mCurrentImage, *mUndoBuffer;
		PTPaintSelection			*mCurrentSelection, *mUndoSelection;
		
		TextTraitsRecord			mTextTraits;
		PTTextAction				*mTextAction;
		
		SUSharedPalette				*mSharedPalette;
	
		PTColorTableChoice			*mColorTableChoice;

		#define kMaxSamplePanes		20
		PTDraggableTargetBox		*mCurrentSamplePane;
		PTDraggableTargetBox		*mSamplePaneList[ kMaxSamplePanes ];
		SInt32						mNumSamplePanes;
				
		#ifdef ICON_STAND_ALONE
		LClipboard					mClipboard;			// only need one of these ???
		#endif
		
		static LMenu				*sOptionsMenu, *sColorsMenu, *sFontMenu, *sStyleMenu;
		Boolean						mWindowIsActive;
		
			// resizing
		Boolean						mAllowImageResizing;
		SInt32						mMinImageWidth, mMaxImageWidth;
		SInt32						mMinImageHeight, mMaxImageHeight;

		static SUColorCursorCache	*mColorCursorCache;
		static SInt32				mNumPaintWindows;
		
		static Str31				sCutString, sCopyString, sPasteString, sClearString;
		
			// note: should this be static since we only need one for all windows ???
		SUOffscreen					*mScratchBuffer;

		static LWindow *			CreatePaintWindow( ResIDT inWindowID );

			// event handling		
		virtual void				HandleControlClick( LPane * );
		virtual void				HandleControlDoubleClick( LPane * );
		virtual void				ForceBlackAndWhite();
		virtual void				SwapColors();
		
		virtual PTAction *			CreateNewAction( OSType inActionType, void *ioParam = nil );
		virtual void				GetActionSettings( struct SPaintAction * );
		virtual void				EraseAll();
		
			// misc
		virtual void				ResetPatternPaneColors( ERedrawOptions = redraw_Later );
		virtual void				PutOnDuty(LCommander *inNewTarget);
		virtual void				TakeOffDuty();
		virtual void				ThrowIfInvalidDepth( SInt32 inDepth );
		virtual void				RemovePaintMenus();
		virtual void				BecomeListenerTo( SInt32 inCount, PTDraggableTargetBox **inList );
};

