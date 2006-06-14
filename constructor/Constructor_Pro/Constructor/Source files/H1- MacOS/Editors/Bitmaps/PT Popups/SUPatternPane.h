// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SU_Types.h"			// just for ERedrawOptions
#include "SUPatternPopup.h"		// for SPatternListH

class SUPatternPane : public LPane, public LBroadcaster
{
	public:
		enum { class_ID = 'PAT ' };

			// construction/destruction
								SUPatternPane( LStream * );
		virtual					~SUPatternPane();
		static	SUPatternPane *	CreateFromStream( LStream * );
		
			// getting & setting the pattern
		virtual SInt32			GetPatternIndex();
		virtual void			SetPatternIndex( SInt32 inIndex, ERedrawOptions = redraw_Later );
		virtual void			GetCurrentPattern( Pattern * );
		
		virtual void			SetPatternList( SPatternListH inHandle, ERedrawOptions = redraw_Later );
		virtual void 			SetPatternList( ResFileIDT inFileID, ResIDT inResID, ERedrawOptions = redraw_Later );
		virtual void 			SetColors( Color32, Color32, ERedrawOptions = redraw_Later );

			// events
		virtual void			DrawSelf();
		virtual void 			ClickSelf( const SMouseDownEvent & );
	
	protected:
		RGBColor				mForeColor, mBackColor;
		SInt32					mCurrentIndex;
		SPatternListH			mPatternListH;

		virtual void			DrawPopup( Boolean inHilited, Boolean inEnabled );
		virtual void			DrawSwatchSelf( const Rect &swatchR );
		virtual void			DrawSwatch();

};

