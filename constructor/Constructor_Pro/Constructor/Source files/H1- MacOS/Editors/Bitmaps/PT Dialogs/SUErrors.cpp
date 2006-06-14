// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.07.31	ebs		Fixed current res file problem in ConvertErrorNumberToString
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUErrors.h"
#include "SUFileUtils.h"
#include "SUSaveResFile.h"
#include "SUSaveGWorld.h"

#include "PT_Errors.h"

#include <UModalDialogs.h>
#include <LStaticText.h>

#ifdef __MWERKS__
	#include <string.h>
#endif


/*============================================
	The 'Emap' resource maps between Mac error numbers and our own
	internal error numbers.
=============================================*/
typedef struct
{
	SInt32			macError;
	SInt32			localError;
} ErrorMapEntry;

const ResType		ResType_ErrorMap		= 'EMap';


/*============================================
	The error dialog has two caption panes
=============================================*/
const PaneIDT		PaneID_ErrorString1		= 'ERR1';
const PaneIDT		PaneID_ErrorString2		= 'ERR2';


/*============================================
	Error file & id
	
	Note:
	The error PPob, STR#, and EMap must have the same id.
=============================================*/
static ResFileIDT	sErrorFileID 			= -1;			// filled in below
static ResIDT		sErrorResourceID 		= 9020;


/*============================================
	SUPostponedError
	
	Description:
	Used internally to display errors at "bad" times (during drag & drop, etc).
=============================================*/
class SUPostponedError : public LPeriodical
{
	public:
						SUPostponedError( SInt32 inError, const char *inAddString = nil );
		virtual void	SpendTime( const EventRecord & );
		
	protected:
		SInt32			mErrorNum;
		char			mErrorString[ 256 ];
};

/*======================================
	Initialize
=======================================*/
void SUErrors::Initialize( ResFileIDT inFileID, ResIDT inResID )
{
	sErrorFileID = inFileID;
	sErrorResourceID = inResID;
}

/*======================================
	InternalInit
=======================================*/
void SUErrors::InternalInit()
{
	if ( sErrorFileID == -1 )
		sErrorFileID = SUFileUtils::GetMainResourceFile();
}

/*======================================
	DisplayError
=======================================*/
void SUErrors::DisplayError( SInt32 inError, const char *inAddString )
{
	char	string1[ 256 ], string2[ 256 ];
	
	InternalInit();
	
	/*
		convert the error number into a string.
		if this returns false, then don't show the error dialog
		  (noErr, userAbortErr, etc)
	*/
	if ( !SUErrors::ConvertErrorNumberToString( inError, string1, string2) )
		return;
		
	if ( inAddString )
		strcpy( string2, inAddString );
	else
		string2[0] = 0;
		
	SUErrors::DisplayErrorString( string1, string2 );
}

/*======================================
	DisplayErrorString
=======================================*/
void SUErrors::DisplayErrorString( const char *inString, const char *addString )
{
	InternalInit();

		// make sure we get the dialog's PPob resource from the correct file
	StSaveResFile	saver1;
	::UseResFile( sErrorFileID );

		// set the GDevice to the main device to work-around Window Mgr bug
	StSaveGWorld	saver2;
	::SetGDevice( GetMainDevice() );				// bug in Mac toolbox requires this
	LView::OutOfFocus( nil );						// PP needs to know when we changed GDevice
	
	
		// create the dialog
	StDialogHandler		theHandler( sErrorResourceID, LCommander::GetTopCommander() );
	LWindow	*			theWindow = theHandler.GetDialog();
	MessageT			theMessage = 0;
	
		// change the error strings
	SUErrors::ChangeCaption( theWindow, PaneID_ErrorString1, inString );
	SUErrors::ChangeCaption( theWindow, PaneID_ErrorString2, addString );
	
		// show the window & wait for the button to be hit
	theWindow->Show();
	::SysBeep( 5 );
	do
	{
		theMessage = theHandler.DoDialog();
	} while ( (theMessage != msg_OK) && (theMessage != msg_Cancel) );
	
		// the menus will need to be re-enabled
	LCommander::SetUpdateCommandStatus( true );
}

/*======================================
	ChangeCaption
=======================================*/
void SUErrors::ChangeCaption( LWindow *inWindow, PaneIDT inPaneID, const char *inString )
{
	LPane	*thePane = inWindow->FindPaneByID( inPaneID );
	ThrowIfNil_( thePane );
	
	LStaticText *theCaption = dynamic_cast<LStaticText*>( thePane );
	if ( theCaption )
	{
		Str255		pString;
		
		if ( inString )
		{
			strcpy( (char*) pString, inString );
			LString::CToPStr( (char*) pString );
		}
		else
			pString[0] = 0;
		
		theCaption->SetDescriptor( pString );
	}
}

/*======================================
	ConvertErrorNumberToString
	
	Returns true if the error should be displayed.
	
	Note:
	Should we dispose of the error list??? not if we want to handle memFullErr's well.
=======================================*/
Boolean SUErrors::ConvertErrorNumberToString( SInt32 inError, char *outString1, char *outString2 )
{
	InternalInit();

	ErrorMapEntry	**mapH = nil;

	*outString1 = 0;
	*outString2 = 0;

	if ( inError == noErr ) return( false );			// not an error
	
	try
	{
		/*****************************************
			string #2 usually returns with the original error number,
			except in the case of assertion & nilP errors (because
			'nilP' and 'asrt' look silly as integers).
		******************************************/
		if ( inError == err_AssertFailed )
			inError = err_AssertionProxy;
		else if ( inError == err_NilPointer )
			inError = err_NilPointerProxy;
			
		::NumToString( inError, (unsigned char*) outString2 );
		LString::PToCStr( (unsigned char*) outString2 );
	
		/*****************************************
			check the error map  resource to see what local error number the
			error should be mapped to.
		******************************************/
		mapH = (ErrorMapEntry**) SUFileUtils::GetResource( sErrorFileID, ResType_ErrorMap, sErrorResourceID );
	
		/*****************************************
			Scan it for the appropriate error id
		******************************************/
		StHandleLocker	lock1( (Handle) mapH );
		ErrorMapEntry	*p = *mapH;
		
		Boolean			foundInMap = false;
		SInt32			errorForString1 = inError;	

		SInt32	numEntries = ::GetHandleSize( (Handle) mapH ) / sizeof( ErrorMapEntry );
		for ( SInt32 count = 0; (count < numEntries) && !foundInMap; count++ )
			if ( p[ count ].macError == inError )
			{
				errorForString1 = p[ count ].localError;
				foundInMap = true;
			}	

		/*****************************************
			If the error is not within our normal range nor was it in the map,
			just use the generic error message.
		******************************************/
		if ( !foundInMap && ((inError < err_FirstError) || (inError > err_LastError)) )
			errorForString1 = err_Generic;
			
		/*****************************************
			Some Mac errors map to "noErr" (such as userAbort)
		******************************************/
		if ( errorForString1 == noErr )
			return( false );
					
		SUFileUtils::GetIndString( sErrorFileID, sErrorResourceID, 
									errorForString1 - err_FirstError + 1,	// index into STR#
									outString1 );
		return( true );
	}
	catch( ... )
	{
		::SysBeep( 5 );					// not much we can do other than beep here
		return( false );
	}
	
		// if we don't get an error string to display, don't bother putting up dialog
	return( false );
}

/*======================================
	DisplayPostponedError
	
	Note:
	This is useful during drag & drop, when we're not supposed to
	be bringing up dialogs.
=======================================*/
void SUErrors::DisplayPostponedError( SInt32 inError, const char *inAddString )
{
	try
	{
		LPeriodical		*theTask = new SUPostponedError( inError, inAddString );
		ThrowIfMemFail_( theTask );
		theTask->StartIdling();
	}
	catch( ... )
	{
		// sorry -- we're hosed. just don't display an error this time
	}
}

/*======================================
	SUPostponedError Constructor
=======================================*/
SUPostponedError::SUPostponedError( SInt32 inError, const char *inAddString )
{
	mErrorNum = inError;
	if ( inAddString )
		strcpy( mErrorString, inAddString );
	else
		mErrorString[0] = 0;
}

/*======================================
	SUPostponedError::SpendTime
=======================================*/
void SUPostponedError::SpendTime( const EventRecord & )
{	
	/*
		note:
		remove ourself from the queue right away because otherwise
		we will be called recursively from within the error dialog
		handler.
	*/
	this->StopIdling();
	
	if ( mErrorNum )
		SUErrors::DisplayError( mErrorNum, mErrorString );
}