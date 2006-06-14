// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPopupButton.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxPopupButton.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_MenuID				= CFSTR("menu id");
	const CFStringRef	key_HasVariableWidth	= CFSTR("has variable width");
	const CFStringRef	key_TitleWidth			= CFSTR("title width");
	const CFStringRef	key_TitleJustification	= CFSTR("title justification");
	const CFStringRef	key_TitleStyle			= CFSTR("title style");
}


// ---------------------------------------------------------------------------
//	PopupButton													  [public]
/**
	Default constructor */

PopupButton::PopupButton()
{
	mMenuID				= -12345;	// Means do not use a MENU resource
	mHasVariableWidth	= true;
	mTitleWidth			= 0;
	mTitleJustification	= teFlushDefault;
	mTitleStyle			= normal;
}


// ---------------------------------------------------------------------------
//	~PopupButton													  [public]
/**
	Destructor */

PopupButton::~PopupButton()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from popup button creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inTitle				Title for popup
	@param	inMenuID			Menu ID for popup
	@param	inHasVariableWidth	Whether the menu has variable width
	@param	inTitleWidth		Width of title (use -1 for variable width)
	@param	inTitleJust			Justification of title text
	@param	inTitleStyle		Font style for title						*/

void
PopupButton::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inTitle,
	SInt32			inMenuID,
	bool			inHasVariableWidth,
	SInt16			inTitleWidth,
	SInt16			inTitleJust,
	Style			inTitleStyle)
{
	mMenuID				= inMenuID;
	mHasVariableWidth	= inHasVariableWidth;
	mTitleWidth			= inTitleWidth;
	mTitleJustification	= inTitleJust;
	mTitleStyle			= inTitleStyle;

	HIViewRef	viewRef = SysCreateView::PopupButton(
								inTitle, inMenuID, inHasVariableWidth,
								inTitleWidth, inTitleJust,
								inTitleStyle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
PopupButton::ClassName() const
{
	return CFSTR("PPx::PopupButton");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
PopupButton::InitState(
	const DataReader&	inReader)
{
	CFString		title;					// Data for Popup Button
	SInt32			initialValue = 0;
	
	inReader.ReadOptional(key_ViewTitle, title);
	inReader.ReadOptional(key_MenuID, mMenuID);
	inReader.ReadOptional(key_HasVariableWidth, mHasVariableWidth);
	inReader.ReadOptional(key_TitleWidth, mTitleWidth);
	inReader.ReadOptional(key_TitleJustification, mTitleJustification);
	inReader.ReadOptional(key_TitleStyle, mTitleStyle);
	inReader.ReadOptional(key_InitValue, initialValue);
	
	HIViewRef	viewRef = SysCreateView::PopupButton(
								title, mMenuID, mHasVariableWidth,
								mTitleWidth, mTitleJustification,
								mTitleStyle);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
								
	SetValue(initialValue);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
PopupButton::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Popup Button
	ioWriter.WriteValue(key_ViewTitle, GetTitle());
	
	ioWriter.WriteValue(key_MenuID, mMenuID);
	ioWriter.WriteValue(key_HasVariableWidth, mHasVariableWidth);
	ioWriter.WriteValue(key_TitleWidth, mTitleWidth);
	ioWriter.WriteValue(key_TitleJustification, mTitleJustification);
	ioWriter.WriteValue(key_TitleStyle, mTitleStyle);
	ioWriter.WriteValue(key_InitValue, GetValue());
		
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetMenuRef													  [public]
/**
	Sets the MenuRef for the popup menu
	
	@param	inMenu		MenuRef												*/

void
PopupButton::SetMenuRef(
	MenuRef		inMenu)
{
	SetDataTag(kControlMenuPart, kControlPopupButtonMenuRefTag,
				sizeof(MenuRef), &inMenu);
				
	mMenuID = -12345;				// No longer using MENU resource
}


// ---------------------------------------------------------------------------
//	GetMenuRef													  [public]
/**
	Returns the MenuRef for the popup menu
	
	@return MenuRef															*/

MenuRef
PopupButton::GetMenuRef() const
{
	MenuRef	menu;

	GetDataTag(kControlMenuPart, kControlPopupButtonMenuRefTag,
				sizeof(MenuRef), &menu);
				
	return menu;
}


// ---------------------------------------------------------------------------
//	SetOwnedMenuRef												  [public]
/**
	Sets the MenuRef that the popup button owns
	
	@param	inMenu		MenuRef												*/

void
PopupButton::SetOwnedMenuRef(
	MenuRef		inMenu)
{
	SetDataTag(kControlMenuPart, kControlPopupButtonOwnedMenuRefTag,
				sizeof(MenuRef), &inMenu);
				
	mMenuID = -12345;				// No longer using MENU resource
}


// ---------------------------------------------------------------------------
//	GetOwnedMenuRef												  [public]
/**
	Returns the MenuRef that the popup button owns
	
	@return MenuRef															*/

MenuRef
PopupButton::GetOwnedMenuRef() const
{
	MenuRef	menu;

	GetDataTag(kControlMenuPart, kControlPopupButtonOwnedMenuRefTag,
				sizeof(MenuRef), &menu);
				
	return menu;
}


// ---------------------------------------------------------------------------
//	SetMenuID														  [public]
/**
	Sets the Menu ID for the popup menu
	
	@param	inMenuID		Menu Id											*/

void
PopupButton::SetMenuID(
	SInt16		inMenuID)
{
	SetDataTag(kControlMenuPart, kControlPopupButtonMenuIDTag,
				sizeof(SInt16), &inMenuID);
				
	mMenuID = inMenuID;
}


// ---------------------------------------------------------------------------
//	GetMenuID														  [public]
/**
	Returns the Menu ID for the popup menu
	
	@return Menu ID															*/

SInt16
PopupButton::GetMenuID() const
{
	return mMenuID;
}


// ---------------------------------------------------------------------------
//	SetExtraHeight													  [public]
/**
	Sets the extra height for te popup button
	
	@param	inExtraHeight		Extra height for te popup button			*/

void
PopupButton::SetExtraHeight(
	SInt16		inExtraHeight)
{
	SetDataTag(kControlMenuPart, kControlPopupButtonExtraHeightTag,
				sizeof(SInt16), &inExtraHeight);
}


// ---------------------------------------------------------------------------
//	GetExtraHeight													  [public]
/**
	Returns the extra height for te popup button
	
	@return Extra height for te popup button								*/

SInt16
PopupButton::GetExtraHeight() const
{
	SInt16	extraHeight;

	GetDataTag(kControlMenuPart, kControlPopupButtonExtraHeightTag,
				sizeof(SInt16), &extraHeight);
				
	return extraHeight;
}


// ---------------------------------------------------------------------------
//	SetCheckCurrentItemFlag											  [public]
/**
	Sets whether to check the current item in the menu
	
	@param	inCheckIt	Whether check the current menu item					*/

void
PopupButton::SetCheckCurrentItemFlag(
	bool		inCheckIt)
{
	Boolean		checkIt = inCheckIt;
	
	SetDataTag(kControlMenuPart, kControlPopupButtonCheckCurrentTag,
				sizeof(Boolean), &checkIt);
}


// ---------------------------------------------------------------------------
//	GetCheckCurrentItemFlag											  [public]
/**
	Returns whether to check the current item in the menu
	
	@return Whether check the current menu item								*/

bool
PopupButton::GetCheckCurrentItemFlag() const
{
	Boolean		checkIt;

	GetDataTag(kControlMenuPart, kControlPopupButtonCheckCurrentTag,
				sizeof(Boolean), &checkIt);
				
	return checkIt;
}


} // namespace PPx
