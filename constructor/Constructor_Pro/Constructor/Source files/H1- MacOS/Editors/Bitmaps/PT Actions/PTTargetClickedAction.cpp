// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTTargetClickedAction.h"

PTTargetClickedAction::PTTargetClickedAction( const SPaintAction &inAction, PTDraggableTargetBox *inTargetHit )
	: PTAction( inAction, str_UndoGeneric ) 

{
	mAffectsUndoState = false;					// since we're not undoable
	mTargetBoxHit = inTargetHit;
}

PTTargetClickedAction::~PTTargetClickedAction()
{
}

void PTTargetClickedAction::DoIt()
{
	PTPaintView 			*thePaintView = mSettings.thePaintView;
	PTDraggableTargetBox 	*oldBox = thePaintView->GetTargetBox();
	
	if ( oldBox != mTargetBoxHit )
	{
		//thePaintView->CopyToUndo();
		
		thePaintView->SelectNone();				// commit the current selection

		/*
			clear the current undo action because we can't undo once we
			change buffers to a different sample pane.
		*/
		thePaintView->PostAction( nil );
		
		/*
			change the target box, current image, etc, and return
		*/
		thePaintView->SetTargetBox( mTargetBoxHit, redraw_Later );
		this->PostAsAction();
	}
	else
		delete this;			// just a hoax -- don't screw with the current undo state	
}

