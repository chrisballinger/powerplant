// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SU_Constants.h"			// for kWhiteColor, kBlackColor
#include "SUPopupWindow.h"

#pragma options align=mac68k

typedef struct SPatternList
{
	short		numPatterns;
	Pattern		patterns[1];
} SPatternList, *SPatternListP, **SPatternListH;

#pragma options align=reset

class SUPatternPopup : protected SUPopupWindow
{
	public:
		static SInt32		DoPatternPopup( Point inTopLeft, SPatternListH, 
											SInt32 defaultItem = -1, 
											const RGBColor &inForeColor = kBlackColor, 
											const RGBColor &inBackColor = kWhiteColor );
	
	protected:
		SPatternListH		mPatternList;
		RGBColor			mForeColor, mBackColor;
	
							SUPatternPopup( SPatternListH, 
											const RGBColor &inForeColor = kBlackColor, 
											const RGBColor &inBackColor = kWhiteColor );
		virtual				~SUPatternPopup();
		
		virtual void		Prepare( Point inTopLeft );
		virtual void		DrawInitialState();
		virtual void		DrawOneCell( SInt32 inIndex, Boolean inHilited = false );
};
