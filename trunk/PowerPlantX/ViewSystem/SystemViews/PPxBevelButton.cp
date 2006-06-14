// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxBevelButton.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxBevelButton.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_Thickness		= CFSTR("thickness");
	const CFStringRef	key_ButtonBehavior	= CFSTR("button behavior");
	const CFStringRef	key_TextAlignment	= CFSTR("text alignment");
	const CFStringRef	key_TextOffset		= CFSTR("text offset");
	const CFStringRef	key_TextPlacement	= CFSTR("text placement");
	const CFStringRef	key_IconTransform	= CFSTR("icon transform");
	const CFStringRef	key_GraphicAlignment = CFSTR("graphic alignment");
	const CFStringRef	key_GraphicOffset	= CFSTR("graphic offset");
	const CFStringRef	key_MenuID			= CFSTR("menu id");
	const CFStringRef	key_MenuBehavior	= CFSTR("menu behavior");
	const CFStringRef	key_MenuPlacement	= CFSTR("menu placement");
	const CFStringRef	key_MenuValue		= CFSTR("menu value");
	const CFStringRef	key_CenterPopupGlyph = CFSTR("center popup glyph");
	
	const ResIDT		menuID_Unspecified	= -12345;
}


// ---------------------------------------------------------------------------
//	BevelButton														  [public]
/**
	Default constructor */

BevelButton::BevelButton()
{
	mThickness		= kControlBevelButtonNormalBevel;
	mMenuID			= menuID_Unspecified;
	mButtonBehavior	= kControlBehaviorPushbutton;
	mMenuBehavior	= kControlBehaviorSingleValueMenu;
	mMenuPlacement	= kControlBevelButtonMenuOnBottom;
}


// ---------------------------------------------------------------------------
//	~BevelButton													  [public]
/**
	Destructor */

BevelButton::~BevelButton()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from bevel button creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coordinates of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inTitle				Text title for button
	@param	inThickness			Thickness of the beveled edges
	@param	inButtonBehavior	How button behaves when clicked
	@param	inButtonContent		Kind of button content
	@param	inMenuID			Menu ID for popup menu
	@param	inMenuBehavior		How menu behaves when item is selected
	@param	inMenuPlacement		Placement of menu glyph within button		*/

void
BevelButton::Initialize(
	View*							inSuperView,
	const HIRect&					inFrame,
	bool							inVisible,
	bool							inEnabled,
	CFStringRef						inTitle,
	ControlBevelThickness			inThickness,
	ControlBevelButtonBehavior		inButtonBehavior,
	const ControlButtonContentInfo&	inButtonContent,
	SInt16							inMenuID,
	ControlBevelButtonMenuBehavior	inMenuBehavior,
	ControlBevelButtonMenuPlacement	inMenuPlacement)
{
	mThickness		= inThickness;
	mMenuID			= inMenuID;
	mButtonBehavior	= inButtonBehavior;
	mMenuBehavior	= inMenuBehavior;
	
	HIViewRef	viewRef = SysCreateView::BevelButton(
								inTitle, inThickness, inButtonBehavior,
								inButtonContent, inMenuID, inMenuBehavior,
								inMenuPlacement);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
BevelButton::ClassName() const
{
	return CFSTR("PPx::BevelButton");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
BevelButton::InitState(
	const DataReader&	inReader)
{
	CFString					title;		// Data for Bevel Button
	ControlButtonContentInfo	content;
	
	content.contentType = kControlNoContent;
	content.u.picture = nil;
	
	inReader.ReadOptional(key_ViewTitle, title);
	
	inReader.ReadOptional(key_Thickness, mThickness);
	inReader.ReadOptional(key_ButtonBehavior, mButtonBehavior);
	
	inReader.ReadOptional(key_MenuID, mMenuID);
	inReader.ReadOptional(key_MenuBehavior, mMenuBehavior);
	inReader.ReadOptional(key_MenuPlacement, mMenuPlacement);
	
	inReader.ReadOptional(key_ContentType, content.contentType);
	inReader.ReadOptional(key_ContentResID, content.u.resID);

	HIViewRef	viewRef = SysCreateView::BevelButton(
								title, mThickness, mButtonBehavior, content,
								mMenuID, mMenuBehavior, mMenuPlacement);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
	
											// Extra options for Bevel Button
	ControlButtonTextAlignment		textAlignment;
	if (inReader.ReadOptional(key_TextAlignment, textAlignment)) {
		SetTextAlignment(textAlignment);
	}
	
	SInt16							textOffset;
	if (inReader.ReadOptional(key_TextOffset, textOffset)) {
		SetTextOffset(textOffset);
	}
	
	ControlButtonTextPlacement		textPlacement;
	if (inReader.ReadOptional(key_TextPlacement, textPlacement)) {
		SetTextPlacement(textPlacement);
	}
	
	IconTransformType				iconTransform;
	if (inReader.ReadOptional(key_IconTransform, iconTransform)) {
		SetIconTransform(iconTransform);
	}
	
	ControlButtonGraphicAlignment	graphicAlignment;
	if (inReader.ReadOptional(key_GraphicAlignment, graphicAlignment)) {
		SetGraphicAlignment(graphicAlignment);
	}
	
	Point							graphicOffset;
	if (inReader.ReadOptional(key_GraphicOffset, graphicOffset)) {
		SetGraphicOffset(graphicOffset);
	}
	
	SInt16							menuValue;
	if (inReader.ReadOptional(key_MenuValue, menuValue)) {
		SetMenuValue(menuValue);
	}
	
	bool							centerPopupGlyph;
	if (inReader.ReadOptional(key_CenterPopupGlyph, centerPopupGlyph)) {
		SetCenterPopupGlyph(centerPopupGlyph);
	}
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
BevelButton::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Bevel Button
	ioWriter.WriteValue(key_ViewTitle,			GetTitle());
	
	ioWriter.WriteValue(key_Thickness,			mThickness);
	ioWriter.WriteValue(key_ButtonBehavior,		mButtonBehavior);
	ioWriter.WriteValue(key_MenuID,				mMenuID);
	ioWriter.WriteValue(key_MenuBehavior,		mMenuBehavior);
	ioWriter.WriteValue(key_MenuPlacement,		mMenuPlacement);
	
	ioWriter.WriteValue(key_TextAlignment,		GetTextAlignment());
	ioWriter.WriteValue(key_TextOffset,			GetTextOffset());
	ioWriter.WriteValue(key_TextPlacement,		GetTextPlacement());
	ioWriter.WriteValue(key_IconTransform,		GetIconTransform());
	ioWriter.WriteValue(key_GraphicAlignment,	GetGraphicAlignment());
	ioWriter.WriteValue(key_GraphicOffset,		GetGraphicOffset());
	
	if (mMenuID != menuID_Unspecified) {
		ioWriter.WriteValue(key_MenuValue, GetMenuValue());
	}
	
	ioWriter.WriteValue(key_CenterPopupGlyph,	GetCenterPopupGlyph());

	ControlButtonContentInfo	content;
	GetContentInfo(content);
	
	ioWriter.WriteValue(key_ContentType,		content.contentType);
	
											// Write ID for resource content
	if ( (content.contentType > kControlNoContent)  &&
		 (content.contentType <= kControlContentICONRes) ) {
		ioWriter.WriteValue(key_ContentResID, content.u.resID);
	}
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetTextAlignment												  [public]
/**
	Sets the text alignment for the bevel button's title
	
	@param	inAlignment		Text alignment for title						*/

void
BevelButton::SetTextAlignment(
	ControlButtonTextAlignment	inAlignment)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonTextAlignTag,
				sizeof(ControlButtonTextAlignment), &inAlignment);
}


// ---------------------------------------------------------------------------
//	GetTextAlignment												  [public]
/**
	Returns the text alignment for the bevel button's title
	
	@return Text alignment for the bevel button's title						*/

ControlButtonTextAlignment
BevelButton::GetTextAlignment() const
{
	ControlButtonTextAlignment	alignment;

	GetDataTag(kControlButtonPart, kControlBevelButtonTextAlignTag,
				sizeof(ControlButtonTextAlignment), &alignment);
				
	return alignment;
}


// ---------------------------------------------------------------------------
//	SetTextOffset													  [public]
/**
	Sets the text offset for the bevel button's title
	
	@param	inOffset		Text offset for title							*/

void
BevelButton::SetTextOffset(
	SInt16	inOffset)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonTextOffsetTag,
				sizeof(SInt16), &inOffset);
}


// ---------------------------------------------------------------------------
//	GetTextOffset													  [public]
/**
	Returns the text offset for the bevel button's title
	
	@return Text offset for the bevel button's title						*/

SInt16
BevelButton::GetTextOffset() const
{
	SInt16	offset;

	GetDataTag(kControlButtonPart, kControlBevelButtonTextOffsetTag,
				sizeof(SInt16), &offset);
				
	return offset;
}


// ---------------------------------------------------------------------------
//	SetTextPlacement												  [public]
/**
	Sets the text placement for the bevel button's title
	
	@param	inPlacement		Text placement for title						*/

void
BevelButton::SetTextPlacement(
	ControlButtonTextPlacement	inPlacement)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonTextPlaceTag,
				sizeof(ControlButtonTextPlacement), &inPlacement);
}


// ---------------------------------------------------------------------------
//	GetTextPlacement												  [public]
/**
	Returns the text placement for the bevel button's title
	
	@return Text placement for the bevel button's title						*/

ControlButtonTextPlacement
BevelButton::GetTextPlacement() const
{
	ControlButtonTextPlacement	placement;

	GetDataTag(kControlButtonPart, kControlBevelButtonTextPlaceTag,
				sizeof(ControlButtonTextPlacement), &placement);
				
	return placement;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetIconTransform												  [public]
/**
	Sets the icon transform for the bevel button's icon
	
	@param	inTransform		Icon transform									*/

void
BevelButton::SetIconTransform(
	IconTransformType	inTransform)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonTransformTag,
				sizeof(IconTransformType), &inTransform);
}


// ---------------------------------------------------------------------------
//	GetIconTransform												  [public]
/**
	Returns the icon transform for the bevel button's icon
	
	@return Icon transform													*/

IconTransformType
BevelButton::GetIconTransform() const
{
	IconTransformType	transform;

	GetDataTag(kControlButtonPart, kControlBevelButtonTransformTag,
				sizeof(IconTransformType), &transform);
				
	return transform;
}


// ---------------------------------------------------------------------------
//	SetGraphicAlignment												  [public]
/**
	Sets the graphic alignment for the bevel button's content
	
	@param	inAlignment		Graphic alignemnt								*/

void
BevelButton::SetGraphicAlignment(
	ControlButtonGraphicAlignment	inAlignment)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonGraphicAlignTag,
				sizeof(ControlButtonGraphicAlignment), &inAlignment);
}


// ---------------------------------------------------------------------------
//	GetGraphicAlignment												  [public]
/**
	Returns the graphic alignment for the bevel button's content
	
	@return Graphic alignment												*/

ControlButtonGraphicAlignment
BevelButton::GetGraphicAlignment() const
{
	ControlButtonGraphicAlignment	alignment;

	GetDataTag(kControlButtonPart, kControlBevelButtonGraphicAlignTag,
				sizeof(ControlButtonGraphicAlignment), &alignment);
				
	return alignment;
}


// ---------------------------------------------------------------------------
//	SetGraphicOffset												  [public]
/**
	Sets the graphic offset for the bevel button's content
	
	@param	inOffset		Graphic offset									*/

void
BevelButton::SetGraphicOffset(
	const Point&	inOffset)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonGraphicOffsetTag,
				sizeof(Point), &inOffset);
}


// ---------------------------------------------------------------------------
//	GetGraphicOffset												  [public]
/**
	Returns the graphic offset for the bevel button's content
	
	@return Graphic offset													*/

Point
BevelButton::GetGraphicOffset() const
{
	Point	offset;

	GetDataTag(kControlButtonPart, kControlBevelButtonGraphicOffsetTag,
				sizeof(Point), &offset);
				
	return offset;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetMenuValue													  [public]
/**
	Sets the value for the bevel button's popup menu
	
	@param	inValue		Menu value											*/

void
BevelButton::SetMenuValue(
	SInt16	inValue)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonMenuValueTag,
				sizeof(SInt16), &inValue);
}


// ---------------------------------------------------------------------------
//	GetMenuValue													  [public]
/**
	Returns the value for the bevel button's popup menu
	
	@return Menu value														*/

SInt16
BevelButton::GetMenuValue() const
{
	SInt16	value;

	GetDataTag(kControlButtonPart, kControlBevelButtonMenuValueTag,
				sizeof(SInt16), &value);
				
	return value;
}


// ---------------------------------------------------------------------------
//	SetMenuRef														  [public]
/**
	Sets the MenuRef for the bevel button's popup menu
	
	@param	inMenu		MenuRef												*/

void
BevelButton::SetMenuRef(
	MenuRef	inMenu)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonMenuRefTag,
				sizeof(MenuRef), &inMenu);
				
	mMenuID = menuID_Unspecified;			// No longer using MENU resource
}


// ---------------------------------------------------------------------------
//	GetMenuRef														  [public]
/**
	Returns the MenuRef for the bevel button's popup menu
	
	@return MenuRef															*/

MenuRef
BevelButton::GetMenuRef() const
{
	MenuRef	menu;

	GetDataTag(kControlButtonPart, kControlBevelButtonMenuRefTag,
				sizeof(MenuRef), &menu);
				
	return menu;
}


// ---------------------------------------------------------------------------
//	SetCenterPopupGlyph												  [public]
/**
	Sets the center popup glyph option for the bevel button
	
	@param	inCenter	Whether to center the popup glyph					*/

void
BevelButton::SetCenterPopupGlyph(
	bool	inCenter)
{
	Boolean	centerIt = inCenter;

	SetDataTag(kControlButtonPart, kControlBevelButtonCenterPopupGlyphTag,
				sizeof(Boolean), &centerIt);
}


// ---------------------------------------------------------------------------
//	GetCenterPopupGlyph												  [public]
/**
	Returns the center popup glyph option for the bevel button
	
	@return Whether to center the popup glyph								*/

bool
BevelButton::GetCenterPopupGlyph() const
{
	Boolean	centerIt;

	GetDataTag(kControlButtonPart, kControlBevelButtonCenterPopupGlyphTag,
				sizeof(Boolean), &centerIt);
				
	return centerIt;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetContentInfo													  [public]
/**
	Sets the content inforomation for the bevel button
	
	@param	inContent	Content inforomation								*/

void
BevelButton::SetContentInfo(
	const ControlButtonContentInfo&	inContent)
{
	SetDataTag(kControlButtonPart, kControlBevelButtonContentTag,
				sizeof(ControlButtonContentInfo), &inContent);
}


// ---------------------------------------------------------------------------
//	GetContentInfo													  [public]
/**
	Passes back the content inforomation for the bevel button
	
	@param	outContent	Content inforomation								*/

void
BevelButton::GetContentInfo(
	ControlButtonContentInfo&	outContent) const
{
	GetDataTag(kControlButtonPart, kControlBevelButtonContentTag,
				sizeof(ControlButtonContentInfo), &outContent);
}


} // namespace PPx