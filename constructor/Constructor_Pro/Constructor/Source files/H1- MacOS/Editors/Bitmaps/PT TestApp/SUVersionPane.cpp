// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "SUVersionPane.h"
#include "SUFileUtils.h"

/*====================================
	SUVersionPane Constructor
=====================================*/
SUVersionPane::SUVersionPane()
{
}

/*====================================
	SUVersionPane Constructor from stream
=====================================*/
SUVersionPane::SUVersionPane( LStream *inStream ) : LCaption( inStream )
{
}

/*====================================
	SUVersionPane Destructor
=====================================*/
SUVersionPane::~SUVersionPane()
{
}


/*====================================
	SUVersionPane::CreateVersionStream
=====================================*/
SUVersionPane*	SUVersionPane::CreateVersionStream(LStream *inStream)
{
	SUVersionPane	*thePane = new SUVersionPane( inStream );
	return( thePane );
}


/*====================================
	SUVersionPane::FinishCreateSelf
	
	Description:
	Sets the string to the current app's version string.
	Uses the short version number rather than the numeric fields.
	
	Note:
	There have been times when MultiFinder used the version resource,
	   so we'll be safe and not release it if it was in memory already.
=====================================*/
void SUVersionPane::FinishCreateSelf()
{
	Handle			h = nil;
	Boolean			wasLoaded;
	Byte			theFlags;
	
	try
	{
		h = SUFileUtils::GetAppResource( 'vers', 1, &wasLoaded );
	}
	catch( ... )
	{
		mText[0] = 0;
		return;
	}
	
	theFlags = HGetState( h );
	HLock( h );

	LString::AppendPStr( mText, (**(VersRecHndl)h).shortVersion, 32 );

	if ( wasLoaded )
		::HSetState( h, theFlags );
	else
		::ReleaseResource( h );
}

