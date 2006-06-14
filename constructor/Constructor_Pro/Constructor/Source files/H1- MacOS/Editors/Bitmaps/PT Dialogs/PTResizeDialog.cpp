// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTResizeDialog.h"
#include "PT_Errors.h"
#include "UModalDialogs.h"
#include "SUSaveGWorld.h"
#include "SUErrors.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>
#include <LEditText.h>
#include <stdio.h>

const ResIDT	PPob_ResizeDialog	= 9021;
const PaneIDT	PaneID_Width		= 'WIDT';
const PaneIDT	PaneID_Height		= 'HEIG';


/*=================================
	DoResizeDialog
==================================*/
Boolean PTResizeDialog::DoResizeDialog(	SInt32 inWidth, SInt32 inMinWidth, SInt32 inMaxWidth,
										SInt32 inHeight, SInt32 inMinHeight, SInt32 inMaxHeight,
										SInt32 *outWidth, SInt32 *outHeight )
{
	/***************************
		ensure that the window is created properly (Window Mgr bug)
	****************************/
	StSaveGWorld		aSaver;
	::SetGDevice( ::GetMainDevice() );
	
	// If there is a PICT in the clipboard, let's override the inWidth and inHeight
	// parameters and use the size of the PICT in the clipboard.
	try
	{
		if (UScrap::HasData(kScrapFlavorTypePicture))
		{
			StHandleBlock pictHandle(Size_Zero, true, true);
			SInt32 byteSize;
			
			byteSize = UScrap::GetData(kScrapFlavorTypePicture, pictHandle);
			PicHandle picture = (PicHandle) pictHandle.Get();
			inWidth = (*picture)->picFrame.right - (*picture)->picFrame.left;
			inHeight = (*picture)->picFrame.bottom - (*picture)->picFrame.top;
			if (inWidth < inMinWidth)
				inWidth = inMinWidth;
			if (inWidth > inMaxWidth)
				inWidth = inMaxWidth;
			if (inHeight < inMinHeight)
				inHeight = inMinHeight;
			if (inHeight > inMaxHeight)
				inHeight = inMaxHeight;
		}
	}
	catch (...)
	{
		// If an error occurs, we'll just use what was passed to DoResizeDialog
	}
		
	/***************************
		create the dialog
	****************************/
	StApplicationContext 	appContext;				// must load from application file
	StDialogHandler			theHandler( PPob_ResizeDialog, LCommander::GetTopCommander() );
	LWindow	*				theWindow = theHandler.GetDialog();
	LEditText *			widthField =  dynamic_cast<LEditText*> (theWindow->FindPaneByID( PaneID_Width ));
	LEditText *			heightField = dynamic_cast<LEditText*> (theWindow->FindPaneByID( PaneID_Height ));
	
	ThrowIf_( !widthField || !heightField );
	
	/********************************************
		initialize the values & show the window
	********************************************/
	widthField->SetValue( inWidth );
	heightField->SetValue( inHeight );
	theWindow->Show();
	
	/********************************************
		go into an event loop
	********************************************/
	Boolean 	done = false;
	MessageT	theMessage;
	SInt32		newWidth, newHeight;
	
	while( !done )
	{
		theMessage = theHandler.DoDialog();
		
		if ( theMessage == msg_Cancel )
			done = true;
		else if ( theMessage == msg_OK )
		{
			newWidth = widthField->GetValue();
			newHeight = heightField->GetValue();
			
			if ( (newWidth < inMinWidth) || (newWidth > inMaxWidth) )
			{
				theWindow->SwitchTarget( widthField );
				widthField->SelectAll();
				ShowResizeError( inMinWidth, inMaxWidth, inMinHeight, inMaxHeight );
			}
			else if ( (newHeight < inMinHeight) || (newHeight > inMaxHeight) )
			{
				theWindow->SwitchTarget( heightField );
				heightField->SelectAll();
				ShowResizeError( inMinWidth, inMaxWidth, inMinHeight, inMaxHeight );
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
		*outWidth = newWidth;
		*outHeight = newHeight;
		return( true );
	}
	else
		return( false );
}

/*=================================
	ShowResizeError
	
	Note:
	Doesn't deal with case where width & height don't match, but they do
	for all of the image types we handle. 
==================================*/
void PTResizeDialog::ShowResizeError( 	SInt32 inMinWidth, SInt32 inMaxWidth,
										SInt32 /*inMinHeight*/, SInt32 /*inMaxHeight*/ )
{
	char	formatString[ 256 ], combinedString[ 256 ];
	
	SUErrors::ConvertErrorNumberToString( err_ResizeFormatString, formatString, combinedString );
	sprintf( combinedString, formatString, inMinWidth, inMaxWidth );
	SUErrors::DisplayErrorString( combinedString, nil );
}