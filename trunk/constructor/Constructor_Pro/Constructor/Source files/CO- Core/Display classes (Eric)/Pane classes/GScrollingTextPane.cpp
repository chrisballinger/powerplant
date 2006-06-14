// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GScrollingTextPane.cpp		© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/16/95
//     $Date: 2006/04/12 08:47:44 $
//	$History: GScrollingTextPane.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:43
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Pane classes
//	Improved commenting.
//
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "GScrollingTextPane.h"

	// PowerPlant : File & strema
#include <LStream.h>

	// PowerPlant : Utilities
#include <UDrawingUtils.h>


// ===========================================================================
//		* Gray-scale fading table
// ===========================================================================

UInt16 GScrollingTextPane::sGrayLevels[fade_Levels] =
	{ 4369, 8738, 17476, 21845, 30583, 34952, 43690, 48059, 52428, 56979, 61166, 65535 };


// ===========================================================================

#pragma mark *** GScrollingTextPane ***

// ---------------------------------------------------------------------------
//		* GScrollingTextPane(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

GScrollingTextPane::GScrollingTextPane(
	LStream* inStream)

: LPane(inStream)
{
	ResIDT theTextResourceID, theTxtrResourceID;
	*inStream >> theTextResourceID;
	*inStream >> theTxtrResourceID;
	InitScrollingTextPane(theTextResourceID, theTxtrResourceID);
}


// ---------------------------------------------------------------------------
//		* ~GScrollingTextPane
// ---------------------------------------------------------------------------

GScrollingTextPane::~GScrollingTextPane()
{
	if (mTextWorldP)
		::DisposeGWorld(mTextWorldP);
	if (mDrawWorldP)
		::DisposeGWorld(mDrawWorldP);
	if (mBackWorldP)
		::DisposeGWorld(mBackWorldP);
}


// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------

void
GScrollingTextPane::DrawSelf()
{
	FetchBackground();
	DrawCurrent();
	StartRepeating();
}


// ---------------------------------------------------------------------------
//		* SpendTime												[protected]
// ---------------------------------------------------------------------------
//	Continue the animation after the initial drawing.

void
GScrollingTextPane::SpendTime(
	const EventRecord& /* inMacEvent*/)
{

	UInt32 startTicks = ::TickCount();
	
	// Start animations (if appropriate).

	if (mTextFade == animate_Waiting) {
		mTextFade = animate_Active;
		mFadeIndex = 0;
	}
	if ((mTextFade == animate_Active) && (mFadeIndex >= fade_Levels)) {
		UInt32 ticks;
		mTextFade = animate_Done;
		::Delay(90, &ticks);
		mTextScroll = animate_Active;
	}
	if ((mTextScroll == animate_Active) && (mTextBounds.top > mTextHeight)) {
		mTextBounds.top = 1;
		Rect frame;
		CalcPortFrameRect(frame);
		mTextBounds.bottom = frame.bottom - frame.top + 1;
	}

	// Draw the animation as it stands now.

	DrawCurrent();
	
	// Advance animation counters.

	if (mTextFade == animate_Active) {
		UInt32 ticks;
		Delay(3, &ticks);
		mFadeIndex++;
	}
	if (mTextScroll == animate_Active) {
		mTextBounds.top++;
		mTextBounds.bottom++;
	}

	// Make sure at least one tick has elapsed during drawing.
	// This enforces a maximum speed on the animation.

	while (::TickCount() == startTicks)
		;

}


// ---------------------------------------------------------------------------
//		* FetchScrolledText										[protected]
// ---------------------------------------------------------------------------
//	Get the text which will be scrolled for the animation.

void
GScrollingTextPane::FetchScrolledText()
{
	Handle theText;
	TEHandle theTE;
	StScrpHandle theStyle;
	GWorldPtr saveWorld;
	GDHandle saveDevice;
	
	CalcPortFrameRect(mTextBounds);
	mTextBounds.right -= mTextBounds.left;
	mTextBounds.bottom -= mTextBounds.top;
	mTextBounds.top = mTextBounds.left = 0;
	
	// Put the text in a TERecord.
	
//	UTextTraits::SetPortTextTraits(mTextTraitsID);
::TextFont(applFont);	
::TextSize(9);
	theTE = ::TEStyleNew(&mTextBounds, &mTextBounds);
	theText = ::GetResource('TEXT', mTextResourceID);
	theStyle = (StScrpHandle) ::GetResource('styl', mTextResourceID);
	::HLock(theText);
	::HidePen();
	::TEStyleInsert(*theText, ::GetHandleSize(theText), theStyle, theTE);
	::ShowPen();
	::ReleaseResource(theText);
	if (theStyle != nil)
		::ReleaseResource((Handle) theStyle);

	::TESetAlignment(teJustRight, theTE);
	::TECalText(theTE);

	// Determine size of the text.

	mTextBounds.right -= mTextBounds.left;
	mTextBounds.bottom -= mTextBounds.top;
	mTextBounds.left = mTextBounds.top = 0;
	mTextHeight = ::TEGetHeight((**theTE).nLines, 0, theTE);
	
	// Create new GWorld that is the height of the text.
	
	try {
		Rect gWorldBounds = mTextBounds;
		gWorldBounds.bottom = mTextHeight;
		ThrowIfOSErr_(::NewGWorld(&mTextWorldP, 0, &gWorldBounds, nil, nil, 0));
	
		::GetGWorld(&saveWorld, &saveDevice);
		::SetGWorld (mTextWorldP, nil);
		::LockPixels(::GetGWorldPixMap(mTextWorldP));
	
		// Draw text inside GWorld.
	
		::EraseRect(&gWorldBounds);
		(**theTE).viewRect = gWorldBounds;
		(**theTE).destRect = gWorldBounds;
		(**theTE).inPort = (GrafPtr) mTextWorldP;
		::TEUpdate(&gWorldBounds, theTE);
		::TEDispose(theTE);
		
		::InvertRect(&gWorldBounds);			// White letters on black background
	
		::UnlockPixels(GetGWorldPixMap(mTextWorldP));
		::SetGWorld(saveWorld, saveDevice);
	}
	catch(...) {
		::TEDispose(theTE);
		delete this;
	}
}


// ---------------------------------------------------------------------------
//		* FetchBackground										[protected]
// ---------------------------------------------------------------------------
//	Get a copy of the background image. This cache is used to
//	speed the drawing of the animation.

void
GScrollingTextPane::FetchBackground()
{
	if (!UDrawingUtils::IsColorGrafPort(GetMacPort()))
		return;
	
#if PP_Target_Carbon
	GrafPtr theGrafPort = GetMacPort();
	PixMapHandle windowPixMapH = ::GetPortPixMap(theGrafPort);
#else
	CGrafPort* theGrafPort = (CGrafPort*) GetMacPort();
	PixMapHandle windowPixMapH = theGrafPort->portPixMap;
#endif
	::HLockHi((Handle) windowPixMapH);
	PixMapPtr windowPixMapP = *windowPixMapH;

	// Get background from window.

	Rect portFrame;
	CalcPortFrameRect(portFrame);
	
	Rect gWorldFrame = portFrame;
	gWorldFrame.right -= gWorldFrame.left;
	gWorldFrame.bottom -= gWorldFrame.top;
	gWorldFrame.top = gWorldFrame.left = 0;

	::CopyBits((BitMapPtr) windowPixMapP, (BitMapPtr) mBackPixMapP,
			&portFrame, &gWorldFrame, srcCopy, nil);
}


// ---------------------------------------------------------------------------
//		* DrawCurrent											[protected]
// ---------------------------------------------------------------------------
//	Draw the animation as it stands now.

void
GScrollingTextPane::DrawCurrent()
{
	FocusDraw();
	Rect portFrame;
	CalcPortFrameRect(portFrame);
	Rect localFrame;
	CalcLocalFrameRect(localFrame);

	Rect gWorldFrame = portFrame;
	gWorldFrame.right -= gWorldFrame.left;
	gWorldFrame.bottom -= gWorldFrame.top;
	gWorldFrame.top = gWorldFrame.left = 0;

	// Fade-in animation.

	if (mTextFade == animate_Active) {
		PixMapHandle thePixMapH;
		::SetGWorld(mDrawWorldP, nil);

		thePixMapH = ::GetGWorldPixMap(mTextWorldP);
		::LockPixels(thePixMapH);
		::CopyBits((BitMapPtr) (*thePixMapH), (BitMapPtr) mDrawPixMapP,
				 &mTextBounds, &gWorldFrame, srcCopy, nil);
		::UnlockPixels(thePixMapH);

		RGBColor fadeColor;
		fadeColor.red = fadeColor.blue = fadeColor.green = sGrayLevels[mFadeIndex];
		::RGBForeColor(&fadeColor);
		
		::PenMode(adMin);
		::PaintRect(&gWorldFrame);
		::PenMode(patCopy);
		::ForeColor(blackColor);

		::CopyBits((BitMapPtr) mBackPixMapP, (BitMapPtr) mDrawPixMapP,
				 &gWorldFrame, &gWorldFrame, adMax, nil);
	}
	
	// Scrolling text animation.
	
	if (mTextScroll == animate_Active) {
		PixMapHandle thePixMapH;
		short i, j;
		short creditsWidth = portFrame.right - portFrame.left - 1;

		::SetGWorld(mDrawWorldP, nil);

		thePixMapH = ::GetGWorldPixMap(mTextWorldP);
		::LockPixels(thePixMapH);
		if (mTextBounds.bottom <= mTextHeight) {
			::CopyBits((BitMapPtr) (*thePixMapH), (BitMapPtr) mDrawPixMapP,
					 &mTextBounds, &gWorldFrame, srcCopy, nil);
		} else {
			Rect wrapViewRect, wrapPictRect, topPictRect, topViewRect;
			wrapViewRect = gWorldFrame;
			wrapViewRect.top = wrapViewRect.bottom - (mTextBounds.bottom - mTextHeight);
			wrapPictRect = mTextBounds;
			wrapPictRect.top = 0;
			wrapPictRect.bottom = wrapPictRect.top + (wrapViewRect.bottom - wrapViewRect.top);
			
			// JWW - Properly set the bounds for the top area of the scrolling text...
			// ...because OS X doesn't like going past the end of the bitmap
			topPictRect = mTextBounds;
			topPictRect.bottom = mTextHeight;
			topViewRect = gWorldFrame;
			topViewRect.bottom = topPictRect.bottom - topPictRect.top;
			
			::CopyBits((BitMapPtr) (*thePixMapH), (BitMapPtr) mDrawPixMapP,
					 &topPictRect, &topViewRect, srcCopy, nil);
			::CopyBits((BitMapPtr) (*thePixMapH), (BitMapPtr) mDrawPixMapP,
					 &wrapPictRect, &wrapViewRect, srcCopy, nil);
		}
		::UnlockPixels(thePixMapH);

		::PenMode(adMin);					// Fade out at top and fade in at bottom
		j = fade_Levels - 1;
		for (i = 0; i < j; i++) {
			RGBColor fadeColor;
			fadeColor.red = fadeColor.blue = fadeColor.green = sGrayLevels[i];
			::RGBForeColor(&fadeColor);
			::MoveTo(gWorldFrame.left, gWorldFrame.top + i);
			::Line(creditsWidth, 0);
			::MoveTo(gWorldFrame.left, gWorldFrame.bottom - i - 1);
			::Line(creditsWidth, 0);
		}
		::ForeColor(blackColor);
		::PenMode(patCopy);
		::CopyBits((BitMapPtr) mBackPixMapP, (BitMapPtr) mDrawPixMapP,
					&gWorldFrame, &gWorldFrame, adMax /* temp: was srcCopy */, nil);

	}

	if (!UDrawingUtils::IsColorGrafPort(GetMacPort()))
		return;
	
#if PP_Target_Carbon
	GrafPtr theGrafPort = GetMacPort();
	PixMapHandle windowPixMapH = ::GetPortPixMap(theGrafPort);
#else
	CGrafPort* theGrafPort = (CGrafPort*) GetMacPort();
	PixMapHandle windowPixMapH = theGrafPort->portPixMap;
#endif
	::HLockHi((Handle) windowPixMapH);
	PixMapPtr windowPixMapP = *windowPixMapH;

	::SetGWorld((GWorldPtr) GetMacPort(), GetMainDevice()); //! TEMPORARY: GetMacPort()->device);
	::CopyBits((BitMapPtr) mDrawPixMapP, (BitMapPtr) windowPixMapP,
			 &gWorldFrame, &portFrame, srcCopy, nil);

}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------

void
GScrollingTextPane::FinishCreateSelf()			//* 2.1.1: added method
{

	// Make GWorld for offscreen drawing.

	Rect portFrame;
	CalcPortFrameRect(portFrame);
	FocusDraw();

	Rect gWorldFrame = portFrame;
	gWorldFrame.right -= gWorldFrame.left;
	gWorldFrame.bottom -= gWorldFrame.top;
	gWorldFrame.top = gWorldFrame.left = 0;

	::NewGWorld(&mDrawWorldP, 0, &gWorldFrame, nil, nil, 0);
	PixMapHandle thePixMapH = ::GetGWorldPixMap(mDrawWorldP);
	::HLockHi((Handle) thePixMapH);
	::LockPixels(thePixMapH);
	mDrawPixMapP = *thePixMapH;

	// Make GWorld for background.

	::NewGWorld(&mBackWorldP, 0, &gWorldFrame, nil, nil, useTempMem);
	if (mBackWorldP == nil) {
		Hide();
		StopRepeating();
		return;
	}
	
	thePixMapH = ::GetGWorldPixMap(mBackWorldP);
	::HLockHi((Handle) thePixMapH);
	::LockPixels(thePixMapH);
	mBackPixMapP = *thePixMapH;
	
	// Get the scrolled text.
	
	FetchScrolledText();

}


// ---------------------------------------------------------------------------
//		* InitScrollingTextPane									[private]
// ---------------------------------------------------------------------------

void
GScrollingTextPane::InitScrollingTextPane(
	SInt16 inTextResourceID,
	SInt16 inTextTraitsID)
{
	mPixelDepth = 8;
	mTextResourceID = inTextResourceID;
	mTextTraitsID = inTextTraitsID;
	
	mTextFade = animate_Active;
	mFadeIndex = 0;
	mTextScroll = animate_Waiting;

	mBackPixMapP = mDrawPixMapP = nil;
	mBackWorldP = mDrawWorldP =  nil;
											//* 2.1.1: moved some initialization to FinishCreateSelf()
}