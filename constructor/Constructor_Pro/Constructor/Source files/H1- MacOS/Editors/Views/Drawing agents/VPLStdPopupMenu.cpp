// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLStdPopupMenu.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLStdPopupMenu.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/24/96   Time: 22:40
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Changed VPLStdPopupMenu so it derives from VPLControl instead of
//	VPLView.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:15
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLStdPopupMenu.h"
	
	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** VPLStdPopupMenu ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant display class for LStdPopupMenu. Substitute our
//	VPFStdPopupMenu which doesn't load Constructor's menus and then
//	delete them.

LPane*
VPLStdPopupMenu::CreateFromStream(
	LStream*	inStream)
{
	return new VPFPopupMenu(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLStdPopupMenu::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Basically all parameters for LStdPopupMenu result
		// in a rebuild of the drawing agent.
		
		case pPaneMenuTitleBold:
		case pPaneMenuTitleUnderline:
		case pPaneMenuTitleItalic:
		case pPaneMenuTitleOutline:
		case pPaneMenuTitleShadow:
		case pPaneMenuTitleCondensed:
		case pPaneMenuTitleExtended:
		case pPaneMenuTitleJustification:
		case pPaneMenuTitleWidth:
		case pPaneMenuResourceID:
		case pPaneMenuAddResMenu:
		case pPaneMenuFixedWidth:
		case pPaneMenuInitialMenuItem:
		case pPaneTextTraits:
		case pPaneControlTitle:
			RebuildMe();
			break;
	
		// Otherwise, see if LPane wants it.

		default:
			VPLPane::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFPopupMenu ***

// ---------------------------------------------------------------------------
//		* VPFPopupMenu(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFPopupMenu::VPFPopupMenu(
	LStream*	inStream)

: LPane(inStream)

{

	// Read popup menu's stream data and stash it.

	SInt32 foo32;
	SInt16 foo16;
	UInt8 foo8;

	*inStream >> foo32;										// value message: ignored
	*inStream >> foo16;										// high word of initial value: ignored
	
	*inStream >> foo8;										// title style
	mTitleStyle = foo8;
	
	*inStream >> foo8;										// title alignment
	mTitleAlignment = foo8;
	
	*inStream >> foo16;										// high word of minimum value: ignored
	*inStream >> mMenuID;									// menu ID

	*inStream >> foo16;										// high word of maximum value: ignored
	*inStream >> mTitleWidth;								// menu title width
	
	*inStream >> foo16;										// control kind: read flags
	mUseResMenu = (foo16 & 4) ? true : false;
	mFixedWidth = foo16 & 1;
	
	*inStream >> mTextTraitsID;								// text traits ID
	inStream->ReadPString(mTitleString);					// title string
	*inStream >> mResMenuType;								// resource menu type
	*inStream >> mMenuInitialItem;							// initial menu item

}


// ---------------------------------------------------------------------------
//		* ~VPFPopupMenu
// ---------------------------------------------------------------------------
//	Destructor

VPFPopupMenu::~VPFPopupMenu()
{
}


// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Draw a simulation of the popup menu.

void
VPFPopupMenu::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Reset drawing environment.
	
	StColorPenState::Normalize();

	// Choose the font and get info about it.

	UTextTraits::SetPortTextTraits(mTextTraitsID);
	FontInfo theFontInfo;
	::GetFontInfo(&theFontInfo);
	
	// Decide where to draw the menu.

	Rect frame;
	CalcLocalFrameRect(frame);

	Rect thePopupRect;
	SInt16 menuHeight = theFontInfo.ascent + theFontInfo.descent + theFontInfo.leading + 3;

	thePopupRect.top = (frame.top + frame.bottom - menuHeight) / 2;
	thePopupRect.bottom = (frame.top + frame.bottom + menuHeight) / 2;
	thePopupRect.left = frame.left + mTitleWidth;
	thePopupRect.right = frame.right - 1;				//! TEMPORARY: should measure menu text

	SInt16 baseline = thePopupRect.bottom - theFontInfo.descent - theFontInfo.leading - 2;
	
	// Fetch menu item text.

	Str255 menuText = "\pMenu item";					//! TEMPORARY: will eventually need to fetch menu text

	// Drawing is device dependent since we use gray for disabled items.
	
	StDeviceLoop device(frame);
	SInt16 depth;
	
	while (device.NextDepth(depth)) {
	
		// Change color if disabled.
		
		StColorPenState pen;
		if (!IsEnabled()) {
			if (depth == 1) {
				Pattern thePattern;
				UQDGlobals::GetGrayPat(&thePattern);
				::PenPat(&thePattern);
				::TextMode(grayishTextOr);
			}
			else
			{
				RGBColor theColor = UGAColorRamp::GetColor(8);
				::RGBForeColor(&theColor);
			}
		}
		
		// Draw the popup box.
	
		::FrameRect(&thePopupRect);
		::MoveTo(thePopupRect.left + 4, thePopupRect.bottom);
		::LineTo(thePopupRect.right, thePopupRect.bottom);
		::LineTo(thePopupRect.right, thePopupRect.top + 3);
	
		// Draw menu item text in the box.
		
		if (::StringWidth(menuText) > (thePopupRect.right - thePopupRect.left - theFontInfo.widMax)) {
			if (::TruncString(thePopupRect.right - thePopupRect.left - theFontInfo.widMax, menuText, truncEnd) == -1)
				menuText[0] = '\0';
		}
		
		::MoveTo(thePopupRect.left + theFontInfo.widMax, baseline);
		::DrawString(menuText);
		
		// Place & draw the menu title (if there is one).
		
		if (mTitleWidth > 10 && mTitleString[0] != 0) {
			
			Rect theTitleRect = thePopupRect;
			theTitleRect.top += 1;
			theTitleRect.bottom -= 1;
			theTitleRect.left = frame.left;
			theTitleRect.right = frame.left + mTitleWidth - 4;
			
			::TextFace(mTitleStyle);
			
			LStr255 tempString = mTitleString;
			if (::StringWidth(tempString) > (theTitleRect.right - theTitleRect.left)) {
				::TextFace(mTitleStyle | condense);
				if (::StringWidth(tempString) > (theTitleRect.right - theTitleRect.left)) {
					if (::TruncString(theTitleRect.right - theTitleRect.left, tempString, truncEnd) == -1)
						tempString[0] = '\0';
				}
			}
			
			SInt16 TEalignment;
			switch (mTitleAlignment) {
				case popupTitleCenterJust:
					TEalignment = teCenter;
					break;
				case popupTitleRightJust:
					TEalignment = teFlushRight;
					break;
				default:
					TEalignment = teFlushLeft;
			}
			
			if (tempString[0])
				::TETextBox((StringPtr) tempString+1, tempString.Length(), &theTitleRect, TEalignment);
	
		}
	}
}
