// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SU_Types.h"		// just for ERedrawOptions

class SUColorPane : public LPane, public LBroadcaster
{
	public:
		enum { class_ID = 'PTCL' };
		
								SUColorPane( LStream * );
		virtual					~SUColorPane();
		virtual void			FinishCreateSelf();
		static	SUColorPane *	CreateFromStream( LStream * );
		
			// get/set color
		virtual Color32			GetColor();
		virtual void			SetColor( Color32, ERedrawOptions = redraw_Later );

			// get/set color table
		virtual CTabHandle		GetColorTable();
		virtual void			SetColorTable( CTabHandle, Boolean inChangeColorToo = true, ERedrawOptions = redraw_Later );
		virtual void			AllowPickerOption( Boolean inOption );
		
		virtual void			DrawSelf();
		virtual void			ClickSelf( const SMouseDownEvent & );
		virtual Boolean 		PointIsInFrame( SInt32 inHoriz, SInt32 inVert ) const;
		
		virtual void			DrawSwatch();
		
	protected:
		Color32					mCurrentColor;
		CTabHandle				mColorTable;
		RgnHandle				mClippedRgn;
		Boolean					mClipsToSibblings;
		Boolean					mUsePickerOption;
		
		virtual void			CalcClipRegionForOverlap();
		virtual void			DrawPopup( Boolean inHilited, Boolean inEnabled );
		virtual void			DrawSwatchSelf( const Rect &swatchR );
		
		virtual SInt32			GetColorIndex( Color32 );
		virtual Boolean			DoAppleColorPicker( Color32 *outColor );
		virtual void			DisposeCurrentTable();
};

