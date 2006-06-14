// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SUPopupWindow.h"

class SUColorPopup : protected SUPopupWindow
{
	public:
		static SInt32		DoColorPopup( Point inTopLeft, CTabHandle, SInt32 defaultItem = -1 );
	
	protected:
		CTabHandle			mColorTable;
		
							SUColorPopup( CTabHandle inTable );
		virtual				~SUColorPopup();
		
		virtual void		Prepare( Point inTopLeft );
		virtual void		DrawInitialState();
		virtual void		DrawOneCell( SInt32 inIndex, Boolean inHilited = false );
};

