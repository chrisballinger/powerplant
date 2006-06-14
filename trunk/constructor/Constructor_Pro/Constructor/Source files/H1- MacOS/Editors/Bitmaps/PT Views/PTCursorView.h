// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PT_Headers.h"

class SUOffscreen;
class PTDraggableTargetBox;

class PTCursorView : public PTPaintView
{
	public:
		enum { class_ID = 'PTCU' };

		static PTCursorView*		OpenPaintWindow( ResIDT inPPobID, RFMap *, ResType, ResIDT );
		static PTCursorView*		CreateFromStream( LStream *inStream );
		
									PTCursorView( LStream * );
		virtual						~PTCursorView();
		virtual void				FinishCreateSelf();
		
		virtual void				InitializeFromResource( RFMap *, ResType, ResIDT );		
		virtual void				SaveAsResource( RFMap *, ResIDT );

	protected:
		PTDraggableTargetBox 		*mColorSample, *mBWSample, *mMaskSample;
		ResType						mResourceType;
		Point						mUndoHotSpot;
		
		void 						ParseBWCursor( RFMap *, ResIDT, SUOffscreen **outBW,
													SUOffscreen **outMask, Point *outHotSpot );

		void						ParseColorCursor( RFMap *, ResIDT, SUOffscreen **outColor,
												SUOffscreen **outBW, SUOffscreen **outMask,
												Point *outHotSpot);

		Handle						CreateBWCursor( SUOffscreen *inBW, SUOffscreen *inMask,
										Point inHotSpot );

		Handle 						CreateColorCursor( SUOffscreen *inColor, SUOffscreen *inBW, 
										SUOffscreen *inMask, Point inHotSpot );
										
		virtual void				SwapUndoState();
		virtual void				CopyToUndo();
};

