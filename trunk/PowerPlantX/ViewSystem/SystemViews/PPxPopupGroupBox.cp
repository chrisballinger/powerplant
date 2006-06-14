// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPopupGroupBox.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxPopupGroupBox.h>
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
//	PopupGroupBox												  [public]
/**
	Default constructor */

PopupGroupBox::PopupGroupBox()
{
	mIsPrimaryGroup		= true;
	mMenuID				= -12345;	// Means do not use a MENU resource
	mHasVariableWidth	= true;
	mTitleWidth			= 0;
	mTitleJustification	= teFlushDefault;
	mTitleStyle			= normal;
}


// ---------------------------------------------------------------------------
//	~PopupGroupBox												  [public]
/**
	Destructor */

PopupGroupBox::~PopupGroupBox()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from popup group box creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inTitle				Title for popup
	@param	inIsPrimary			Group box kind (true = primary, false = secondary)
	@param	inMenuID			Menu ID for popup
	@param	inHasVariableWidth	Whether the menu has variable width
	@param	inTitleWidth		Width of title (use -1 for variable width)
	@param	inTitleJust			Justification of title text
	@param	inTitleStyle		Font style for title						*/

void
PopupGroupBox::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inTitle,
	bool			inIsPrimary,
	SInt32			inMenuID,
	bool			inHasVariableWidth,
	SInt16			inTitleWidth,
	SInt16			inTitleJust,
	Style			inTitleStyle)
{
	mIsPrimaryGroup		= inIsPrimary;
	mMenuID				= inMenuID;
	mHasVariableWidth	= inHasVariableWidth;
	mTitleWidth			= inTitleWidth;
	mTitleJustification	= inTitleJust;
	mTitleStyle			= inTitleStyle;

	HIViewRef	viewRef = SysCreateView::PopupGroupBox(
								inTitle, inIsPrimary, inMenuID,
								inHasVariableWidth, inTitleWidth,
								inTitleJust, inTitleStyle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
PopupGroupBox::ClassName() const
{
	return CFSTR("PPx::PopupGroupBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
PopupGroupBox::InitState(
	const DataReader&	inReader)
{
	CFString		title;					// Data for Popup Group Box
	SInt32			initialValue = 0;
	
	inReader.ReadOptional(key_ViewTitle, title);
	inReader.ReadOptional(key_IsPrimaryGroup, mIsPrimaryGroup);
	inReader.ReadOptional(key_MenuID, mMenuID);
	inReader.ReadOptional(key_HasVariableWidth, mHasVariableWidth);
	inReader.ReadOptional(key_TitleWidth, mTitleWidth);
	inReader.ReadOptional(key_TitleJustification, mTitleJustification);
	inReader.ReadOptional(key_TitleStyle, mTitleStyle);
	inReader.ReadOptional(key_InitValue, initialValue);
	
	HIViewRef	viewRef = SysCreateView::PopupGroupBox(
								title, mIsPrimaryGroup, mMenuID,
								mHasVariableWidth, mTitleWidth,
								mTitleJustification, mTitleStyle);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
								
	SetValue(initialValue);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
PopupGroupBox::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Popup Group Box
	ioWriter.WriteValue(key_ViewTitle, GetTitle());
	
	ioWriter.WriteValue(key_IsPrimaryGroup, mIsPrimaryGroup);
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
PopupGroupBox::SetMenuRef(
	MenuRef		inMenu)
{
	SetDataTag(kControlMenuPart, kControlGroupBoxMenuRefTag,
				sizeof(MenuRef), &inMenu);
				
	mMenuID = -12345;						// No longer using MENU resource
}


// ---------------------------------------------------------------------------
//	GetMenuRef													  [public]
/**
	Returns the MenuRef for the popup menu
	
	@return MenuRef															*/

MenuRef
PopupGroupBox::GetMenuRef() const
{
	MenuRef	menu;

	GetDataTag(kControlMenuPart, kControlGroupBoxMenuRefTag,
				sizeof(MenuRef), &menu);
				
	return menu;
}


// ---------------------------------------------------------------------------
//	GetTitleRect													  [public]
/**
	Passes back the title rectangle for the check box group box
	
	@param	outTitleRect	Title rectangle									*/

void
PopupGroupBox::GetTitleRect(
	Rect&	outTitleRect) const
{
	GetDataTag(kControlEntireControl, kControlGroupBoxTitleRectTag,
				sizeof(Rect), &outTitleRect);
}


} // namespace PPx
