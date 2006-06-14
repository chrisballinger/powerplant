// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUColorCursorCache.h"
#include "SUSaveResFile.h"
#include "SUMiscUtils.h"			// just for FileHasResource()
#include "SUFileUtils.h"

const OSType	ResType_ColorCursor = 'crsr';

SUColorCursorCache *SUColorCursorCache::sCurrentCache = nil;

SUColorCursorCache::SUColorCursorCache( SInt32 numElements, SInt32 resFileID )
					: SUResourceCache( numElements, ResType_ColorCursor, resFileID )
{
	sCurrentCache = this;
}

SUColorCursorCache::~SUColorCursorCache()
{
		// don't dispose of the active cursor
	UCursor::SetArrow();

	/*
		important to release everything in this destructor rather than
		waiting for the SUResourceCache destructor because it will call
		its own DisposeRawResource() method rather than ours. [blech]
	*/
	for ( SInt32 count = 0; count < mNumElements; count++ )
	{
		if ( mCacheArray[count].theResource )
		{
			this->DisposeElement( count );
			mCacheArray[count].theResource = nil;
		}
	}

	if ( sCurrentCache == this )
		sCurrentCache = nil;
}

CCrsrHandle SUColorCursorCache::GetColorCursor( ResIDT inResID )
{
	return( (CCrsrHandle) this->GetResource( inResID ) );
}

Boolean SUColorCursorCache::SetColorCursor( ResIDT inResID )
{
	CCrsrHandle		theCursor = nil;
	
	/*
		since this is called at idle time, it's best not to put
		up error messages over and over and over, so catch the errors.
	*/
	try
	{
		theCursor = (CCrsrHandle) this->GetResource( inResID );
		ThrowIfNil_( theCursor );
	}
	catch( ... )
	{
		return( false );
	}
	
	::SetCCursor( theCursor );
	return( true );
}

void SUColorCursorCache::DisposeColorCursor( CCrsrHandle& inCursor )
{
	this->ReleaseResource( (Handle&) inCursor );
}

Handle SUColorCursorCache::LoadRawResource( ResIDT inResID )
{
	StSaveResFile		aSaver;
	Handle				h = nil;
	
	if ( mResFileID == -1 )
	{
		h = (Handle) ::GetCCursor( inResID );	
	}
	else
	{
			// note: GetCCursor will use resource chain, so check file first
		if ( SUFileUtils::FileHasResource( mResFileID, ResType_ColorCursor, inResID ) )
		{
			::UseResFile( mResFileID );
			h = (Handle) ::GetCCursor( inResID );	
		}
	}
	
	ThrowIfResFail_( h );
	return( h );
}

void SUColorCursorCache::DisposeRawResource( Handle &h )
{
	if ( !h ) return;
	
	::DisposeCCursor( (CCrsrHandle) h );
	h = nil;
}

SUColorCursorCache *SUColorCursorCache::GetCurrentCache()
{
	ThrowIfNil_( sCurrentCache );
	return( sCurrentCache );
}

