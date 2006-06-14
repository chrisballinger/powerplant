// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxStaticText.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxStaticText.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_TextColor	= CFSTR("text color");
}


// ---------------------------------------------------------------------------
//	StaticText														  [public]
/**
	Default constructor */

StaticText::StaticText()
{
}


// ---------------------------------------------------------------------------
//	~StaticText														  [public]
/**
	Destructor */

StaticText::~StaticText()
{
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from static text creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inText			Text to display
	@param	inStyle			Text style										*/

void
StaticText::Initialize(
	View*						inSuperView,
	const HIRect&				inFrame,
	bool						inVisible,
	bool						inEnabled,
	CFStringRef					inText,
	const ControlFontStyleRec*	inStyle)
{
	HIViewRef	viewRef = SysCreateView::StaticText(inText, inStyle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
StaticText::ClassName() const
{
	return CFSTR("PPx::StaticText");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
StaticText::InitState(
	const DataReader&	inReader)
{
	CFString			text;				// Data for Static Text
//	ControlFontStyleRec	style;		// ### Read font style
//	RGBColor			textColor;
	
	inReader.ReadOptional(key_TextContent, text);
//	inReader.ReadOptional(key_TextColor, textColor);
	
	HIViewRef	viewRef = SysCreateView::StaticText(text, nil);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
StaticText::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Static Text
	ioWriter.WriteValue(key_TextContent, GetText());
	
		// ### Write font style
	
//	RGBColor	textColor = { 100, 200, 300 };
//	ioWriter.WriteValue(key_TextColor, RGBColorData(textColor));
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetText															  [public]
/**
	Sets the text tp dosplay
	
	@param	inText		Text to display									*/

void
StaticText::SetText(
	CFStringRef	inText)
{
	SetDataTag(kControlEntireControl, kControlStaticTextCFStringTag,
				sizeof(inText), &inText);
}


// ---------------------------------------------------------------------------
//	GetText															  [public]
/**
	Returns the text from the static text view
	
	@return Text from the static text view									*/

CFString
StaticText::GetText() const
{
	CFStringRef	text = nil;

	OSStatus	status =
		GetDataTag(kControlEntireControl, kControlStaticTextCFStringTag,
					sizeof(text), &text);
					
	PPx_ThrowIfOSError_(status, "Failed to copy static text content");
				
	return CFString(text, retain_No);
}


// ---------------------------------------------------------------------------
//	SetThemeFontID													  [public]
/**
	Sets the theme font ID
	
	@param	inFont		Theme font ID to use for text						*/

void
StaticText::SetThemeFontID(
	ThemeFontID		inFont)
{
	ViewUtils::SetControlThemeFontID(this, inFont);
}


// ---------------------------------------------------------------------------
//	SetFontStyle													  [public]
/**
	Sets the font and style for the static text
	
	@param	inStyleRec	Font and style to use for text						*/

void
StaticText::SetFontStyle(
	const ControlFontStyleRec&	inStyleRec)
{
	SetDataTag(kControlEntireControl, kControlStaticTextStyleTag,
				sizeof(ControlFontStyleRec), &inStyleRec);
}


// ---------------------------------------------------------------------------
//	GetFontStyle													  [public]
/**
	Passes back the font and style options
	
	@param	outStyleRec		Font and system information						*/

void
StaticText::GetFontStyle(
	ControlFontStyleRec&	outStyleRec)
{
	GetDataTag(kControlEntireControl, kControlStaticTextStyleTag,
				sizeof(ControlFontStyleRec), &outStyleRec);
}


} // namespace PPx
