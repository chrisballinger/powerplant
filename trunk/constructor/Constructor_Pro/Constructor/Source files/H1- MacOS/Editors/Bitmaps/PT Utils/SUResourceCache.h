// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PP_Types.h"			// for ResIDT

class SUResourceCache
{
	public:
								SUResourceCache( SInt32 numElements, OSType inResType, SInt32 resFileID = -1 );
		virtual					~SUResourceCache();
							
		virtual Handle			GetResource( ResIDT );
		virtual void			ReleaseResource( Handle& );
		
	protected:
		typedef struct
		{
			Handle				theResource;
			ResIDT				theResID;
		} CacheEntry;		
		
		OSType					mResType;
		SInt32					mResFileID;
		
		SInt32					mNumElements;
		CacheEntry				*mCacheArray;
				
		virtual	SInt32			FindElementByID( ResIDT );
		virtual	SInt32			FindElementByHandle( Handle );
		virtual	void			DisposeElement( SInt32 index );

			// override these for different types of loads (CCursor, CIconHandle, etc)
		virtual	Handle			LoadRawResource( ResIDT );
		virtual	void			DisposeRawResource( Handle& );
};

