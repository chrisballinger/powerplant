// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PT_Headers.h"

class SUOffscreen;
class PTDraggableTargetBox;

class PTCIconView : public PTPaintView
{
	public:
		enum { class_ID = 'PTCI' };

		static PTCIconView*			OpenPaintWindow( ResIDT inPPobID, RFMap *, ResIDT );
		static PTCIconView*			CreatePaintCIconStream( LStream *inStream );
		
									PTCIconView( LStream * );
		virtual						~PTCIconView();
		virtual void				FinishCreateSelf();
		
		virtual void				InitializeFromResource( RFMap *, ResIDT );
		virtual void				InitializeFromBuffers( 
											SUOffscreen *inColor, SUOffscreen *inBW, 
											SUOffscreen *inMask, Boolean bwIsEmpty );
		
		virtual void				SaveAsResource( RFMap *, ResIDT );
		
		virtual void				ChangeImageSize( SInt32 inWidth, SInt32 inHeight, Boolean inStretch );
		
												
	protected:
		PTDraggableTargetBox *		mColorSample;
		PTDraggableTargetBox *		mBWSample;
		PTDraggableTargetBox *		mMaskSample;
		
		SDimension16				mInitialSampleWellSize;

		void						ParseColorIcon( RFMap *, ResIDT, SUOffscreen **outColor,
												SUOffscreen **outBW, SUOffscreen **outMask,
												Boolean *outBWEmpty );
										
		Handle 						CreateColorIcon( SUOffscreen *inColor, SUOffscreen *inBW, 
										SUOffscreen *inMask );
										
		virtual void				ResizeSampleWell( SInt32 inIconWidth, SInt32 inIconHeight );
};

