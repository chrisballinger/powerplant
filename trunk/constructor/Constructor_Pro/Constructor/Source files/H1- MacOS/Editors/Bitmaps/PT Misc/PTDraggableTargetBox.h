// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PTTargetBox.h"
#include "PTImageDragMixin.h"
#include <Drag.h>

class SUOffscreen;
class SUColorCursorCache;
class PTDraggableTargetBox;

const CommandT	cmd_TargetBoxClicked		= 'Tclk';
const CommandT	cmd_ImageDroppedOnTargetBox	= 'IDrp';

/*=================================
	SImageDropOnTargetBox
==================================*/
typedef struct
{
		// one of these two will have the image
	PicHandle				thePicture;
	SUOffscreen				*theBuffer;
	
		// the pane it was dropped on
	PTDraggableTargetBox	*thePane;
	
		// drag info
	DragReference			theDragRef;
	DragAttributes			theDragAttribs;
} SImageDropOnTargetBox;


/*=================================
	PTDraggableTargetBox
==================================*/
class PTDraggableTargetBox : public PTTargetBox, public PTImageDragMixin, public LBroadcaster
{
	public:
		enum { class_ID = 'DRTB' };
		
								PTDraggableTargetBox( LStream * );
		virtual					~PTDraggableTargetBox();
		
		static PTDraggableTargetBox *CreateFromStream( LStream * );
		
		virtual void			AdjustCursorSelf( Point, const EventRecord & );
		virtual void 			ClickSelf( const SMouseDownEvent &inMouseDown );
		virtual void			DrawSelf();
		virtual void			RedrawBufferAsIndicated( ERedrawOptions inRedraw );

		virtual void			SetBuffer( SUOffscreen *, ERedrawOptions );
		virtual void			SetRawBuffer( SUOffscreen * );
		virtual void			CopyBufferFrom( SUOffscreen *, ERedrawOptions );
		virtual SUOffscreen *	GetBuffer();
		
		virtual Boolean			IsMask();
		virtual void			SetMask( Boolean );
		
		virtual Boolean			IsUsed();
		virtual void			SetUsedFlag( Boolean, ERedrawOptions );
		
		virtual Boolean			CanBeDeleted();
		virtual void			SetCanBeDeletedFlag( Boolean );

		virtual Boolean			GetDropEnable();
		virtual void			SetDropEnable( Boolean );
		
			// these help us resize the pane
		virtual void			GetInitialPaneSize( SDimension16 * );
		virtual void			GetMinPaneSize( SDimension16 * );
		virtual void			GetInitialPaneLocation( SPoint32 * );
		
	protected:
		SUOffscreen *			mBuffer;
		SUColorCursorCache *	mCursorCache;
		Boolean					mIsMask;
		Boolean					mIsUsed;
		Boolean					mCanBeDeleted;
		Boolean					mDropEnabled;
		
		SDimension16			mMinPaneSize;
		SDimension16			mInitialPaneSize;
		SPoint32				mInitialPaneLocation;

		virtual void			DrawBuffer();

		virtual Boolean			ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef );
		virtual void			HandleDroppedPicture( PicHandle inPicture, DragReference, DragAttributes );
		virtual void			HandleDroppedOffscreen( SUOffscreen *, DragReference, DragAttributes );
		
		virtual void			GetCenteredBufferArea( 	SInt32 inWidth, SInt32 inHeight, 
													 	Rect *outLocalRect, Rect *outPortRect );
};

