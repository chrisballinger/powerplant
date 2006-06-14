// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "SUCloseOnClick.h"


/*========================================
	SUCloseOnClick::CreateSplashWindow
	
	Description:
	Creates a window and adds an attachment that makes it go away the
	next time the mouse or keyboard is hit. 
	
	This function is static, so it can be called without an object
=========================================*/
void SUCloseOnClick::CreateSplashWindow( ResIDT inRezID, LCommander *inSuper )
{
	/*******************************************
		these are volatile because they are set within a try block
		and accessed within a catch block (this *is* important)
	*******************************************/
	LWindow 		*theWindow = nil;
	SUCloseOnClick	*theAttachment = nil;
	
	try
	{
			// create the window
		theWindow = LWindow::CreateWindow( inRezID, inSuper );
	
			// add the attachment
		theAttachment = new SUCloseOnClick( theWindow );
		ThrowIfMemFail_( theAttachment );
		
		LEventDispatcher::GetCurrentEventDispatcher()->AddAttachment( 
									theAttachment, nil, false );
	}
	catch( ... )
	{
		if ( theWindow ) delete( theWindow );
		if ( theAttachment ) delete( theAttachment );
		throw;
	}
	
		// make the window visible
	((LWindow*)theWindow)->Show();
}

/*========================================
	SUCloseOnClick::Execute
	
	Note:
	We override this instead of ExecuteSelf because this can determine
	dynamically whether to return true or false, where ExecuteSelf this
	is determined by the object's creator.
=========================================*/
Boolean SUCloseOnClick::Execute( MessageT inMessage, void *ioParam )
{
	Boolean		handleOutside = true;
	
	switch( inMessage )
	{
		case msg_Event:
			if ( ioParam && mWindowToClose )
			{
				EventRecord		*anEvent = (EventRecord*) ioParam;
				
				if ( (anEvent->what == keyDown) || (anEvent->what == autoKey) ||
						(anEvent->what == mouseDown) )
				{
					mWindowToClose->AttemptClose();		// kill the window
					
						// detach ourselves from the system queue and commit suicide
					LEventDispatcher::GetCurrentEventDispatcher()->RemoveAttachment( this );
					delete this;			
					
					LCommander::SetUpdateCommandStatus( true );
					handleOutside = false;
				}
			}
			break;
			
		case msg_CommandStatus:
			handleOutside = false;				// disable everything
			break;
	}
	
	return( handleOutside );
}

/*========================================
	SUCloseOnClick Constructor
	
	Note:
	Call of LAttachment indicates we want *all* types of messages.
=========================================*/
SUCloseOnClick::SUCloseOnClick( const LWindow *inWindow ) : LAttachment( msg_AnyMessage, true )
{
	mWindowToClose = (LWindow*) inWindow;
}

