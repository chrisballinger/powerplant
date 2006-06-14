// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GScrollingTextPane.h		© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/16/95
//     $Date: 2006/01/18 01:32:46 $
//	$History: GScrollingTextPane.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:43
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Pane classes
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:24
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Pane classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Feature classes
#include <LPeriodical.h>

	// PowerPlant : Pane classes
#include <LPane.h>


// ===========================================================================
//		* GScrollingTextPane
// ===========================================================================
//
//	GScrollingTextPane does the scrolling credits line in the about box.
//	It's a rather ugly hack, so it's not for public distribution.
//
// ===========================================================================

class GScrollingTextPane :	public LPane,
							public LPeriodical {

public:
	enum { class_ID = 'gstp' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new GScrollingTextPane(inStream); }

							GScrollingTextPane(LStream* inStream);
	virtual					~GScrollingTextPane();

protected:
	virtual void			DrawSelf();
	virtual void			SpendTime(const EventRecord& inMacEvent);
	
	void					FetchScrolledText();
	void					FetchBackground();
	void					DrawCurrent();

	virtual void			FinishCreateSelf();					//* 2.1.1


	enum AnimateState  {
		animate_Waiting,
		animate_Active,
		animate_Done
	};
	enum {
		fade_Levels = 12
	};
	

	ResIDT					mTextResourceID;				// 'TEXT'/'styl' resources for scrolled text
	ResIDT					mTextTraitsID;					// default text traits for scrolled text

	GWorldPtr				mDrawWorldP;					// offscreen world for scratch drawing
	PixMapPtr				mDrawPixMapP;
	GWorldPtr				mBackWorldP;					// offscreen world for saving background image
	PixMapPtr				mBackPixMapP;

	GWorldPtr				mTextWorldP;					// offscreen world for scrolled text
	Rect					mTextBounds;					// location of scrolled text image
	SInt16					mTextHeight;					// height of scrolled text image
	
	SInt16					mPixelDepth;					// depth of the port we're drawing into
	AnimateState			mTextFade;
	AnimateState			mTextScroll;
	SInt16					mFadeIndex;
	
	static UInt16			sGrayLevels[fade_Levels];

	// private initialization

private:
	void					InitScrollingTextPane(
									SInt16 inTextResourceID,
									SInt16 inTextTraitsID);

};
