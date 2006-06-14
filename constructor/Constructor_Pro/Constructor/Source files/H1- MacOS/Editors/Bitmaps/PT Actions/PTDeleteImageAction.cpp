// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTActions.h"
#include "PTDraggableTargetBox.h"

/*======================================
	Constructor
=======================================*/
PTDeleteImageAction::PTDeleteImageAction( const SPaintAction &inAction )
	: PTAction( inAction, str_UndoDeleteImage )
{
	this->ThrowIfFileLocked();
	mTargetBox = inAction.thePaintView->GetTargetBox();
}

/*======================================
	Destructor
=======================================*/
PTDeleteImageAction::~PTDeleteImageAction()
{
}

/*======================================
	DoIt
=======================================*/
void PTDeleteImageAction::DoIt()
{
	if ( !mTargetBox || !mTargetBox->IsUsed() )				// shouldn't happen
	{
		delete this;
		return;
	}
	
	this->RedoSelf();
	
	/*
		note:
		can't call the normal "PostAsAction" here because it redraws
		the sample pane and that triggers it to be used again.
	*/
	mSettings.thePaintView->SetChangedFlag( true );
	mSettings.thePaintView->PostAction( this );			
}

/*======================================
	RedoSelf
	
	Description:
	This deletes the target box.
=======================================*/
void PTDeleteImageAction::RedoSelf()
{
	if ( !mTargetBox ) return;
	
	mSettings.thePaintView->CopyToUndo();
	mSettings.thePaintView->SelectNone();
	
	/*
		erase the image in the main buffer and target box.
		note that mSettings.theImageBuffer may no longer be correct here, so be
		   sure to get the correct one from the paint view.
	*/
	SUOffscreen	*imageBuffer = mSettings.thePaintView->GetImageBuffer();
	imageBuffer->EraseToWhite();
	mTargetBox->CopyBufferFrom( imageBuffer, redraw_Dont );

	/*
		the target is now unused
	*/
	mTargetBox->SetUsedFlag( false, redraw_Now );
	
		// update the screen
	mSettings.thePaintView->HandleCanvasDraw();
}

