// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUResourceCache.h"
#include "SUSaveResFile.h"
#include "SUMiscUtils.h"

/*====================================
	SUResourceCache::Constructor
=====================================*/
SUResourceCache::SUResourceCache( SInt32 numElements, OSType inResType, SInt32 resFileID )
{
	mResType = inResType;
	mResFileID = resFileID;
	mNumElements = numElements;
	mCacheArray = (CacheEntry*) ::NewPtrClear( sizeof( CacheEntry ) * numElements );
	ThrowIfMemFail_( mCacheArray );
}

/*====================================
	SUResourceCache::Destructor
=====================================*/
SUResourceCache::~SUResourceCache()
{
	for ( SInt32 count = 0; count < mNumElements; count++ )
	{
		if ( mCacheArray[count].theResource )
			this->DisposeElement( count );
	}
}
 
/*====================================
	SUResourceCache::GetResource
=====================================*/
Handle SUResourceCache::GetResource( ResIDT inResID )
{
	/******************************************
		see if it's already in the cache
	******************************************/
	SInt32	index = this->FindElementByID( inResID );
	if ( index != -1 )
		return( mCacheArray[index].theResource );
	
	/******************************************
		not in cache, so read it into memory
	******************************************/
	Handle	h = this->LoadRawResource( inResID );
		
	/******************************************
		find an opening in the cache and save the icon there
		
		Note: if the cache is full, we'll just forget about saving it
	******************************************/
	for ( SInt32 count = 0; count < mNumElements; count++ )
		if ( mCacheArray[ count ].theResource == nil )
		{
			mCacheArray[ count ].theResource = h;
			mCacheArray[ count ].theResID = inResID;
			break;
		}
		
	return( h );
}

/*====================================
	SUResourceCache::ReleaseResource
=====================================*/
void SUResourceCache::ReleaseResource( Handle& h )
{
	if ( !h ) return;

	SInt32	index = this->FindElementByHandle( h );
	if ( index != -1 )
		this->DisposeElement( index );
	else
		this->DisposeRawResource( h );		// still delete it even if not cached

	h = nil;
}

/*====================================
	SUResourceCache::FindElementByID
=====================================*/
SInt32 SUResourceCache::FindElementByID( ResIDT theID )
{
	for ( SInt32 count = 0; count < mNumElements; count++ )
	{
		if ( mCacheArray[count].theResID == theID )
			return( count );
	}
	
	return( -1 );
}

/*====================================
	SUResourceCache::FindElementByHandle
=====================================*/
SInt32 SUResourceCache::FindElementByHandle( Handle h )
{
	for ( SInt32 count = 0; count < mNumElements; count++ )
	{
		if ( mCacheArray[count].theResource == h )
			return( count );
	}
	
	return( -1 );
}

/*====================================
	SUResourceCache::DisposeElement
=====================================*/
void SUResourceCache::DisposeElement( SInt32 index )
{
	if ( mCacheArray[index].theResource )
	{
		this->DisposeRawResource( mCacheArray[index].theResource );
		mCacheArray[index].theResID = 0;
	}
}


/*====================================
	SUResourceCache::LoadRawResource
=====================================*/
Handle SUResourceCache::LoadRawResource( ResIDT inResID )
{
	StSaveResFile		aSaver;
	Handle				h = nil;
	
	if ( mResFileID == -1 )
	{
		h = ::GetResource( mResType, inResID );
	}
	else
	{
		UseResFile( mResFileID );
		h = ::Get1Resource( mResType, inResID );
	}
	
	ThrowIfResFail_( h );
	return( h );
}

/*====================================
	SUResourceCache::DisposeRawResource
=====================================*/
void SUResourceCache::DisposeRawResource( Handle &h )
{
	if ( h )
	{
		SUMiscUtils::DisposeHandle( h );	// does a smart releaseresource/disposehandle
		h = nil;
	}
}

