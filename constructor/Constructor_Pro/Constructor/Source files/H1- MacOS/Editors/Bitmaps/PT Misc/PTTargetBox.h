// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "LGABox.h"
#include "SU_Types.h"

class PTTargetBox : public LGABox
{
	public:
		enum { class_ID = 'PTTB' };
		
							PTTargetBox( LStream * );
		virtual				~PTTargetBox();
		static PTTargetBox	*CreateFromStream( LStream * );
		
		virtual void		SetTarget( Boolean, ERedrawOptions );
		
	protected:
		Boolean				mHasTarget;
		
		virtual void		DrawBoxTitle();
		virtual void		DrawColorBorder( const Rect &inBorderRect, EGABorderStyle inStyle );
		virtual void		DrawBlackAndWhiteBorder( const Rect &inBorderRect, EGABorderStyle inStyle );
		virtual void		DrawTargettedFrame( const Rect &inBorderRect, Boolean inColor = true );

		virtual void		GetBackgroundColor( RGBColor * );
};
