// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPWindowBackdrop.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPWindowBackdrop.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:01
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:39
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Fixed a commenting error.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:44
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Improved handling of window title changes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 21:37
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added DrawContentArea and window title area helpers.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:26
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added class.
//	
// ===========================================================================

#include "VPWindowBackdrop.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"
#include "DMRGBColorAttribute.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** VPWindowBackdrop ***

// ---------------------------------------------------------------------------
//		* VPWindowBackdrop()
// ---------------------------------------------------------------------------
//	Constructor

VPWindowBackdrop::VPWindowBackdrop()
{
}


// ---------------------------------------------------------------------------
//		* ~VPWindowBackdrop
// ---------------------------------------------------------------------------
//	Destructor

VPWindowBackdrop::~VPWindowBackdrop()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** backdrop drawing

// ---------------------------------------------------------------------------
//		* DrawSelf
// ---------------------------------------------------------------------------
//	Color the content area as specified in the 'wctb' resource.

void
VPWindowBackdrop::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());

	// Draw basic window frame.

	VEBackdrop::DrawSelf();

	// Shade the content area.
	
	DrawContentArea();

}


// ---------------------------------------------------------------------------
//		* DrawContentArea
// ---------------------------------------------------------------------------
//	Color the content area of the window as per the 'wctb' resource.

void
VPWindowBackdrop::DrawContentArea()
{

	// Validate pointers.
	
	ValidateThis_();

	// Shade the window background to match the
	// 'wctb' information.
	
	Rect contentRect;
	if (CalcLocalContentRect(contentRect)) {
	
		// Get window content color.

		RGBColor contentColor;
		GetWindowContentColor(contentColor);
		
		// Erase content area to this color.
		
	 	::RGBBackColor(&contentColor);
	 	::EraseRect(&contentRect);
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** window content area

// ---------------------------------------------------------------------------
//		* CalcLocalContentRect									[protected]
// ---------------------------------------------------------------------------
//	Return the area which is part of the content area of this window.

Boolean
VPWindowBackdrop::CalcLocalContentRect(
	Rect&	outFrameRect)
{
	return CalcLocalFrameForRootObject(outFrameRect);
}


// ---------------------------------------------------------------------------
//		* GetWindowContentColor									[protected]
// ---------------------------------------------------------------------------
//	Look through the root object's data model to find the content color.

void
VPWindowBackdrop::GetWindowContentColor(
	RGBColor&	outContentColor)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());

	// Default to white content color.	

	outContentColor = UGAColorRamp::GetWhiteColor();
	
	// See if window has a 'wctb' resource.
	
	DMIntegerAttribute* hasWctbAttr =
			(dynamic_cast<DMIntegerAttribute*>(mRootObject->FindAttributeByKey(pPaneWindowHasWCTB)));
	
	if (hasWctbAttr == nil)
		return;
	ValidateObject_(hasWctbAttr);

	if (hasWctbAttr->GetIntValue() == 0)
		return;
	
	// It does, look for the content color attribute.
	
	DMRGBColorAttribute* colorAttr =
			(dynamic_cast<DMRGBColorAttribute*>(mRootObject->FindAttributeByKey(pPaneWindowContentColor)));
	
	if (colorAttr == nil)
		return;
	ValidateObject_(colorAttr);

	// Got the color attribute. Return its value.
	
	outContentColor = colorAttr->GetRGBColorValue();	

}


// ---------------------------------------------------------------------------
//		* RootAttributeChanged									[protected]
// ---------------------------------------------------------------------------
//	If the window's content color has changed, trigger a redraw of the
//	content area.

void
VPWindowBackdrop::RootAttributeChanged(
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inAttribute);
	
	// Dispatch by attribute key.

	switch (inAttribute->GetKey()) {
	
		// 'wctb' information: Redraw content area.
	
		case pPaneWindowHasWCTB:
		case pPaneWindowContentColor: {
			FocusDraw();
			Rect contentRect;
			if (CalcLocalContentRect(contentRect))
			{
				CalcPortFrameRect(contentRect);
				InvalPortRect(&contentRect);
			}
			break;
		}
		
		// Window title: Redraw title area.
		
		case pPaneWindowTitle: {
			FocusDraw();
			Rect titleRect;
			if (CalcLocalTitleRect(titleRect))
			{
				CalcPortFrameRect(titleRect);
				InvalPortRect(&titleRect);
			}
			break;
		}
		
		// Otherwise: See if VEBackdrop wants it (not likely).
		
		default:
			VEBackdrop::RootAttributeChanged(inAttribute);
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** window title area

// ---------------------------------------------------------------------------
//		* CalcLocalTitleRect									[protected]
// ---------------------------------------------------------------------------
//	Override hook. Return true and fill in outFrameRect if the window
//	title is visible.

Boolean
VPWindowBackdrop::CalcLocalTitleRect(
	Rect&	/* outFrameRect */)
{
	return false;			// override hook
}


// ---------------------------------------------------------------------------
//		* DrawWindowTitle										[protected]
// ---------------------------------------------------------------------------
//	Draw the window title in a dark gray color.

#if PP_Target_Carbon
pascal void DrawWindowTitle(const Rect *bounds, SInt16 depth, Boolean colorDevice, UInt32 userData)
{
#pragma unused(depth, colorDevice)

	LStr255 *theTitle = (LStr255 *) userData;
	
	short theHeight = bounds->bottom - bounds->top;
	
	FontInfo theFontInfo;
	::GetFontInfo(&theFontInfo);
	
	short theCharHeight = theFontInfo.ascent + theFontInfo.descent;
	short h = bounds->left;
	short v = (theHeight - theCharHeight) / 2 + bounds->top + theFontInfo.ascent;
	
	::MoveTo(h, v);
	::DrawString(*theTitle);
}
#else
void
VPWindowBackdrop::DrawWindowTitle(
	ResIDT		inTxtrID,
	Int16		inHorizInset)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());
	
	// Find the title area.
	
	Rect titleFrame;
	if (!CalcLocalTitleRect(titleFrame))
		return;
	
	// Erase title area to white.
	
	RGBColor theColor = UGAColorRamp::GetWhiteColor();
 	::RGBBackColor(&theColor);
 	::EraseRect(&titleFrame);

	// Now find the window title.
	
	DMAttribute* titleAttr = mRootObject->FindAttributeByKey(pPaneWindowTitle);
	ValidateObject_(titleAttr);
	
	LStr255 title;
	titleAttr->GetTextValue(title);
	
	// Now draw the window title.
	
	::InsetRect(&titleFrame, inHorizInset, 1);

	StApplicationContext appContext;
	UTextTraits::SetPortTextTraits(inTxtrID);

	theColor = UGAColorRamp::GetColor(11);
	::RGBForeColor(&theColor);
	::TETextBox(&title[1], title[0], &titleFrame, teCenter);
	
}
#endif