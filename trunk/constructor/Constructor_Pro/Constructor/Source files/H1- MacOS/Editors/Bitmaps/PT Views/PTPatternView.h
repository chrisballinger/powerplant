// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PT_Headers.h"

class SUOffscreen;
class PTPatternTargetBox;

class PTPatternView : public PTPaintView
{
	public:
		enum { class_ID = 'PTPA' };

		static PTPatternView*		OpenPaintWindow( ResIDT inPPobID, RFMap *, ResType, ResIDT );
		static PTPatternView*		CreateFromStream( LStream *inStream );
		
									PTPatternView( LStream * );
		virtual						~PTPatternView();
		virtual void				FinishCreateSelf();
		
		virtual void				InitializeFromResource( RFMap *, ResType, ResIDT );
		virtual void				SaveAsResource( RFMap *, ResIDT );
		
		virtual SInt32				GetZoomFactor( SInt32, SInt32, Boolean *outShowGrid );

		virtual void				ChangeImageSize( SInt32 inWidth, SInt32 inHeight, Boolean inStretch );
		virtual void				ImageSizeChangeUndone( SInt32 inWidth, SInt32 inHeight );
		
		virtual void 				FindCommandStatus( CommandT inCommand, Boolean &outEnabled, 
									Boolean &outUsesMark, UInt16 &outMark, 
									Str255 outName );
		
	protected:
		PTPatternTargetBox *		mColorSample;
		PTPatternTargetBox *		mBWSample;
		ResType						mResourceType;
		SInt32						mPixPatWidth, mPixPatHeight;
		
		SDimension16				mInitialSampleWellSize;
		
		void						ParseColorPattern( Handle inPattern, SUOffscreen **outColor,
												SUOffscreen **outBW );
		
		void						ParseBWPattern( Handle inPattern, SUOffscreen **outBW );
		SUOffscreen *				BWPatternToOffscreen( const Pattern & );
		
		Handle						CreateColorPattern( SUOffscreen *inColor, SUOffscreen *inBW );
		Handle						CreateBWPattern( SUOffscreen *inBW );
									
		virtual void				ResizeSampleWell( SInt32 newPatternWidth, SInt32 newPatternHeight );

};

