// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		96.08.28	ebs		Support for locked files
		96.08.27	ebs		Fixed crash when "cancel" hit -- added mCancelledResize 
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTResizeImageAction.h"
#include "PTResizeDialog.h"

/*=========================================
	Constructor
==========================================*/
PTResizeImageAction::PTResizeImageAction( const SPaintAction &inSettings ) :
	PTAction( inSettings, str_UndoResize )
{
	this->ThrowIfFileLocked();
	mCancelledResize = true;
}

/*=========================================
	Destructor
==========================================*/
PTResizeImageAction::~PTResizeImageAction()
{
	if ( mCancelledResize )
		return;
	
	if ( this->IsDone() )
		mBeforeState.DeleteAllBuffers();
	else
		mAfterState.DeleteAllBuffers();
}

/*=========================================
	DoIt
==========================================*/
void PTResizeImageAction::DoIt()
{
	PTPaintView		*pv = mSettings.thePaintView;
	SUOffscreen		*cb = mSettings.currentBuffer;
	SInt32			newWidth, newHeight;
	
	/**********************************************
		ask the user for a new height & width 
	**********************************************/
	Boolean okHit = PTResizeDialog::DoResizeDialog(
						cb->GetWidth(), pv->GetMinImageWidth(), pv->GetMaxImageWidth(),
						cb->GetHeight(), pv->GetMinImageHeight(), pv->GetMaxImageHeight(),
						&newWidth, &newHeight );

	if ( !okHit || ((newWidth == cb->GetWidth()) && (newHeight == cb->GetHeight())) )
	{
		delete this;
		return;
	}
	
	/**********************************************
		we're memory hungry enough -- clear the selection so we don't
		have to save it. 
		
		clear the current action as well.
	**********************************************/
	pv->SelectNone();
	pv->PostAction( nil );
	
	/**********************************************
		save off the current state of the window
		(this includes sample pane location, canvas size, etc, etc)
	**********************************************/
	mCancelledResize = false;
	mBeforeState.Initialize( pv );
	
	/**********************************************
		do the resize operation
	**********************************************/
	pv->ChangeImageSize( newWidth, newHeight, false );
	
	/**********************************************
		save off the current state of the window
	**********************************************/
	mAfterState.Initialize( pv );
	
	/**********************************************
		and we're undoable
	**********************************************/
	this->PostAsAction();
}

/*=========================================
	UndoSelf
==========================================*/
void PTResizeImageAction::UndoSelf()
{
	mBeforeState.RestoreWindowState();
}

/*=========================================
	RedoSelf
==========================================*/
void PTResizeImageAction::RedoSelf()
{
	mAfterState.RestoreWindowState();
}

