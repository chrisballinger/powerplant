// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "UModalDialogs.h"
#include "PT_Errors.h"
#include "PTFontSizeDialog.h"
#include "SUSaveGWorld.h"
#include "SUErrors.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>
#include <LEditText.h>

const ResIDT	PPob_TextSizeDialog	= 9022;
const PaneIDT	PaneID_Size			= 'SIZE';

/*=================================
	DoSizeDialog
==================================*/
Boolean PTFontSizeDialog::DoSizeDialog( SInt16 inSize, SInt16 *outSize )
{
	try
	{
		/***************************
			ensure that the window is created properly (Window Mgr bug)
		****************************/
		StSaveGWorld		aSaver;
		::SetGDevice( ::GetMainDevice() );
		
		/***************************
			create the dialog
		****************************/
		StApplicationContext 	appContext;				// must load from application file
		StDialogHandler			theHandler( PPob_TextSizeDialog, LCommander::GetTopCommander() );
		LWindow	*				theWindow = theHandler.GetDialog();
		LEditText *			sizeField =  dynamic_cast<LEditText*> (theWindow->FindPaneByID( PaneID_Size ));
		
		ThrowIf_( !sizeField );
		
		/********************************************
			initialize the values & show the window
		********************************************/
		sizeField->SetValue( inSize );
		theWindow->Show();
		
		/********************************************
			go into an event loop
		********************************************/
		Boolean 	done = false;
		MessageT	theMessage;
		SInt16		newSize;
		
		while( !done )
		{
			theMessage = theHandler.DoDialog();
			
			if ( theMessage == msg_Cancel )
				done = true;
			else if ( theMessage == msg_OK )
			{
				newSize = sizeField->GetValue();

				if ( (newSize < 4) || (newSize > 512) )
				{
					theWindow->SwitchTarget( sizeField );
					sizeField->SelectAll();
					SUErrors::DisplayError( err_BadFontSize );
				}
				else
					done = true;
			}
		};
		
		/********************************************
			return values to the caller
		********************************************/
		if ( theMessage == msg_OK )
		{
			*outSize = newSize;
			return( true );
		}
		else
			return( false );
	}
	catch( ... )
	{
		SUErrors::DisplayError( err_Generic );
		return( false );	
	}
}