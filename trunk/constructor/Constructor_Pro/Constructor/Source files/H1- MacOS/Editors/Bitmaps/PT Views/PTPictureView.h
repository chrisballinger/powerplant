// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PT_Headers.h"

class PTPictureView : public PTPaintView
{
	public:
		enum { class_ID = 'PTPI' };

		static PTPictureView*		OpenPaintWindow( ResIDT inPPobID, RFMap *, ResIDT );
		static PTPictureView*		CreateFromStream( LStream *inStream );
		
									PTPictureView( LStream * );
		virtual						~PTPictureView();
		
		virtual void				InitializeFromResource( RFMap *, ResIDT );
		virtual void				InitializeFromBuffer( SUOffscreen * );
		
		virtual void				SaveAsResource( RFMap *, ResIDT );
		virtual void				ChangeImageSize( SInt32 inWidth, SInt32 inHeight, Boolean inStretch );
};

