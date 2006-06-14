// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SUResourceCache.h"

class SUColorCursorCache : public SUResourceCache
{
	public:
									SUColorCursorCache( SInt32 numElements, SInt32 resFileID = -1 );
		virtual 					~SUColorCursorCache();
	
		static SUColorCursorCache *	GetCurrentCache();
		
		virtual CCrsrHandle			GetColorCursor( ResIDT );
		virtual void				DisposeColorCursor( CCrsrHandle& );
		virtual Boolean				SetColorCursor( ResIDT );
		
	protected:
		virtual Handle				LoadRawResource( ResIDT );
		virtual void				DisposeRawResource( Handle& );
		
		static SUColorCursorCache	*sCurrentCache;
};

