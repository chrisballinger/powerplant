// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTSampleCaption.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTSampleCaption.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:18
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/07/96   Time: 12:35
//	Updated in $/Constructor/Source files/Editors/Text traits
//	Changed DrawSelf() to use luminance calculation (provided by Eric
//	Shapiro). Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/30/96   Time: 18:36
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#include "TTSampleCaption.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// MacOS : Editors : Bitmaps : PT utils
#include "SU_Macros.h"	

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** TTSampleCaption ***

// ---------------------------------------------------------------------------
//		* TTSampleCaption()
// ---------------------------------------------------------------------------
//	Constructor

TTSampleCaption::TTSampleCaption(
	LStream*	inStream)

: LStaticText(inStream)

{
	mTxtrObject = nil;
}


// ---------------------------------------------------------------------------
//		* ~TTSampleCaption
// ---------------------------------------------------------------------------
//	Destructor

TTSampleCaption::~TTSampleCaption()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetObject
// ---------------------------------------------------------------------------
//	Link the caption to the Txtr data model object that's being portrayed.
//	The caption will make itself a listener to the object, and redraw itself
//	whenever the data model's attributes change.

void
TTSampleCaption::SetObject(
	DMObject*	inObject)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);
	
	if (mTxtrObject != nil)
		SignalCStr_("Can't change txtr object.");
	
	// Set up listener relationship.
	
	mTxtrObject = inObject;
	mTxtrObject->AddListener(this);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Draw the caption text using the text traits attributes specified by
//	the data model (even though it may not yet be written to a resource).
//	Erase the background to either white or black, depending on the text
//	color.

void
TTSampleCaption::DrawSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mTxtrObject.GetObject());

	// Set up text traits.
	
	LHandleStream txtrData;
	mTxtrObject->WriteStreamData(&txtrData);

	SInt16 just;
	{
		StHandleLocker lockTxtr((Handle) txtrData.GetDataHandle());
		just = UTextTraits::SetPortTextTraits((TextTraitsPtr) *txtrData.GetDataHandle());
	}
		
	RGBColor textColor;
	::GetForeColor(&textColor);
	
	ApplyForeAndBackColors();
	::RGBForeColor(&textColor);

	// Erase the text frame to white, unless the text color is very light
	// then erase text frame to black.

	Rect frame;
	CalcLocalFrameRect(frame);

	// Luminance calculation contributed by Eric Shapiro.
	// See Foley & Van Dam 2nd edition, p. 595.
	
	RGBColor backColor;
	UInt32 maxColor = MAX( textColor.red, MAX(textColor.green, textColor.blue) );
	UInt32 minColor = MIN( textColor.red, MIN(textColor.green, textColor.blue) );
	if ( maxColor + minColor > 85195L )					// 65%
		backColor = UGAColorRamp::GetBlackColor();
	else
		backColor = UGAColorRamp::GetWhiteColor();
	
	::RGBBackColor(&backColor);
	::EraseRect(&frame);

	// Draw the text. Inset the frame by a couple of pixels
	// so the white border doesn't look too garish.
	
	::InsetRect(&frame, 4, 2);
//	UTextDrawing::DrawWithJustification((Ptr)&mText[1], mText[0], frame, just);
	LStr255 theText;
	GetText(theText);
	UTextDrawing::DrawWithJustification(theText.TextPtr(), theText.LongLength(), frame, just);

}


// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Whenever an attribute of the Txtr changes, we trigger a redraw.

void
TTSampleCaption::ValueChanged(
	DM_ValueChange*		inMessage)
{
	
	// Validate input parameters.
	
	ValidateThis_();
	ValidateObject_(inMessage->mObject);
	
	// If it's our object, redraw the pane.
	
	if (inMessage->mObject == mTxtrObject)
		Refresh();

}