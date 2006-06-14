// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIFontPopup.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/31/96
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIFontPopup.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 17:56
//	Updated in $/Constructor/Source files/Property inspector
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:29
//	Updated in $/Constructor/Source files/Property inspector
//	Added history information.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 16:28
//	Created
//	Comment: Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIFontPopup.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMFontStringAttribute.h"
#include "DMValueLabel.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Resource mananger
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** PIFontPopup ***

// ---------------------------------------------------------------------------
//		* PIFontPopup()
// ---------------------------------------------------------------------------
//	Constructor

PIFontPopup::PIFontPopup(
	LStream*	inStream)

: LPopupButton(inStream)

{
	mAttribute = nil;
}


// ---------------------------------------------------------------------------
//		* ~PIFontPopup
// ---------------------------------------------------------------------------
//	Destructor

PIFontPopup::~PIFontPopup()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* SetAttributeValue
// ---------------------------------------------------------------------------
//	Takes an value from the attribute and maps it to the popup menu item
// 	which displays that value's label (if any).

void
PIFontPopup::SetValueFromAttribute()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mAttribute);
	
	// See if we can find this font's name in the popup.
	
	LStr255 fontName;
	mAttribute->DMStringAttribute::GetTextValue(fontName);
	if (fontName.Length() == 0)
		mAttribute->GetTextValue(fontName);
	
	// Iterate over menu items until we find this text.
	
	MenuHandle menu = GetMacMenuH();
#if PP_Target_Carbon
	ThrowIf_(menu == nil);
#else
	ValidateHandle_((Handle) menu);
#endif

	SInt16 menuSize = ::CountMenuItems(menu);
	SInt16 menuItem = 0;
	
	while (++menuItem <= menuSize) {
	
		LStr255 menuText;
		::GetMenuItemText(menu, menuItem, menuText);
		
		if (menuText.CompareTo(fontName) == 0) {
		
			// Found it.
			
			StopBroadcasting();
			SetValue(menuItem);
			StartBroadcasting();
			return;
		
		}
	}
	
	// No such menu.
	
	mMinValue = 0;
	SetValue(0);
	
}


// ---------------------------------------------------------------------------
//		* GetFontName
// ---------------------------------------------------------------------------
//	Get the font whose name was selected in the popup menu.

void
PIFontPopup::GetFontName(
	LStr255&	outFontName)
{

	// Validate pointers.

	ValidateThis_();
	
	MenuHandle menu = GetMacMenuH();
#if PP_Target_Carbon
	ThrowIf_(menu == nil);
#else
	ValidateHandle_((Handle) menu);
#endif

	// Get title of menu item. This is the font name.
	
	::GetMenuItemText(menu, GetValue(), outFontName);

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetAttribute
// ---------------------------------------------------------------------------
//	Read the initial value from the attribute.

void
PIFontPopup::SetAttribute(
	DMFontStringAttribute*	inAttribute)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inAttribute);

	// Stash the attribute pointer.

	mAttribute = inAttribute;
	
	// Update popup menu.
	
	SetMacMenuH(LoadPopupMenuH());

	// Make sure the popup value matches the attribute.

	SetValueFromAttribute();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** dynamic popup menu

// ---------------------------------------------------------------------------
//		* LoadPopupMenuH										[protected]
// ---------------------------------------------------------------------------
//	Get the popup menu handle. Add items to the menu to reflect the
//	value labels.

MenuHandle
PIFontPopup::LoadPopupMenuH() const
{

	// Validate pointers.
	
	ValidateThis_();

	// Get the (hopefully) empty menu.

	MenuHandle theMenu = ::GetMenu(GetMenuID());
#if PP_Target_Carbon
	ThrowIf_(theMenu == nil);
#else
	ValidateHandle_((Handle) theMenu);
#endif

	// If no attribute, bail out now.
	
	if (mAttribute == nil)
		return theMenu;
	ValidateObject_((const_cast<PIFontPopup*>(this))->mAttribute);
	
	// If menu isn't empty, yank all the items out of it.

	SInt16 menuItemCount = ::CountMenuItems(theMenu);
	while (menuItemCount--) {
		::DeleteMenuItem(theMenu, 1);
	}
	
	// See if we need to add "System font" and "Application font" items.
	
	AddPredefinedFonts(theMenu);

	// Add names of each font in the system.
	
	AddInstalledFonts(theMenu);

	// Return the newly-formed menu.
	
	return theMenu;

}


// ---------------------------------------------------------------------------
//		* AddPredefinedFonts									[protected]
// ---------------------------------------------------------------------------
//	Add the menu items for system font and application font if the
//	font attribute requests it.

void
PIFontPopup::AddPredefinedFonts(
	MenuHandle	inMenu) const
{
	
	// Validate pointers.

	ValidateThis_();
	ValidateObject_((const_cast<PIFontPopup*>(this))->mAttribute);
#if PP_Target_Carbon
	ThrowIf_(inMenu == nil);
#else
	ValidateHandle_((Handle) inMenu);
#endif
	
	Boolean addedItems = false;
	
	// See if attribute requires "System font".
	
	if (mAttribute->GetIncludeSystemFont()) {
	
		StApplicationContext appContext;
		LStr255 sysFontStr(STR_FontStringAttribute, str_SystemFont);
		
		::AppendMenu(inMenu, (unsigned char*)"\001x");			//! TEMPORARY: magic string
		::SetMenuItemText(inMenu, ::CountMenuItems(inMenu), sysFontStr);
	
		addedItems = true;
	
	}

	// See if attribute requires "Application font".
	
	if (mAttribute->GetIncludeApplicatonFont()) {
	
		StApplicationContext appContext;
		LStr255 appFontStr(STR_FontStringAttribute, str_ApplicationFont);
		
		::AppendMenu(inMenu, (unsigned char*)"\001x");			//! TEMPORARY: magic string
		::SetMenuItemText(inMenu, ::CountMenuItems(inMenu), appFontStr);
	
		addedItems = true;
	
	}
	
	// If we added either of the above items, add a divider line.
	
	if (addedItems)
		::AppendMenu(inMenu, (unsigned char*)"\001-");			//! TEMPORARY: magic string

}


// ---------------------------------------------------------------------------
//		* AddInstalledFonts										[protected]
// ---------------------------------------------------------------------------
//	Add the list of fonts that are currently on the system.

void
PIFontPopup::AddInstalledFonts(
	MenuHandle	inMenu) const
{
	
	// Validate pointers.

	ValidateThis_();
#if PP_Target_Carbon
	ThrowIf_(inMenu == nil);
#else
	ValidateHandle_((Handle) inMenu);
#endif
	
	// Add all fonts to menu.
	
	::AppendResMenu(inMenu, 'FONT');

}