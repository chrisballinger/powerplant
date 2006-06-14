// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		96.08.28	ebs		Support for locked files
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"
#include "PTOffscreenDragTask.h"

PTDragImageAction::PTDragImageAction( const SPaintAction &inAction, SDragImageInfo *info )
		: PTAction( inAction, str_UndoDrag )
{
	this->ThrowIfFileLocked();
	mDragInfo = *info;
}

void PTDragImageAction::DoIt()
{
	PTPaintView			*thePaintView = mSettings.thePaintView;
	PTPaintSelection	*theSelection = mSettings.theSelection;
	SUOffscreen 		*currentBuffer = mSettings.currentBuffer;
	
	if ( !thePaintView || !theSelection || !currentBuffer ) return;

	thePaintView->CopyToUndo();
	
	switch( mDragInfo.imageType )
	{
		case ImageType_Picture:
			thePaintView->SelectNone();
			thePaintView->ChangeTool( Tool_Selection );
			theSelection->PastePicture( currentBuffer, (PicHandle) mDragInfo.data );
			
			mSettings.theCanvas->Refresh();		// can't redraw now because we'll mess with the drag hilite
			
			this->PostAsAction();
			break;

		case DragFlavor_Offscreen:
			thePaintView->SelectNone();
			thePaintView->ChangeTool( Tool_Selection );
			theSelection->PasteOffscreenBuffer( currentBuffer, (SUOffscreen*) mDragInfo.data );
			
			mSettings.theCanvas->Refresh();		// can't redraw now because we'll mess with the drag hilite
			
			this->PostAsAction();
			break;
	}
}

