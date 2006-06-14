// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEditTextControl.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
//
//	Do not use this control. Use PPx::EditUnicodeText instead.
//	This control does not work properly as an HIView in compositing mode.
//	This is a system limitation noted in Apple's "Introducing HIView"
//	documentation.
//	
//	This class is here in case future versions of Mac OS X or PPx add
//	support for this control.

#include <PPxEditTextControl.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCFData.h>
#include <SysCreateView.h>

namespace PPx {

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_IsPassword		= CFSTR("is password");
	const CFStringRef	key_UseInlineInput	= CFSTR("use inline input");
}


// ---------------------------------------------------------------------------
//	EditTextControl													  [public]
/**
	Default constructor */

EditTextControl::EditTextControl()
{
	mIsPassword		= false;
	mUseInlineInput	= false;
}


// ---------------------------------------------------------------------------
//	~EditTextControl												  [public]
/**
	Destructor */

EditTextControl::~EditTextControl()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from edit text creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coordinates of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inText				Initial text in edit field
	@param	inIsPassword		Whether the field is a for password
	@param	inUseInlineInput	Whether to use inline input
	@param	inStyle				Text style									*/

void
EditTextControl::Initialize(
	View*						inSuperView,
	const HIRect&				inFrame,
	bool						inVisible,
	bool						inEnabled,
	CFStringRef					inText,
	bool						inIsPassword,
	bool						inUseInlineInput,
	const ControlFontStyleRec*	inStyle)
{
	mIsPassword		= inIsPassword;
	mUseInlineInput	= inUseInlineInput;
	
	HIViewRef	viewRef = SysCreateView::EditTextControl(
								inText, inIsPassword, inUseInlineInput,
								inStyle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
EditTextControl::ClassName() const
{
	return CFSTR("PPx::EditTextControl");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
EditTextControl::InitState(
	const DataReader&	inReader)
{
	CFString			text;				// Data for Edit Text
	
//	ControlFontStyleRec	style;		// ### Read font style

	inReader.ReadOptional(key_TextContent, text);
	inReader.ReadOptional(key_IsPassword, mIsPassword);
	inReader.ReadOptional(key_UseInlineInput, mUseInlineInput);
	
	HIViewRef	viewRef = SysCreateView::EditTextControl(
								text, mIsPassword, mUseInlineInput,
								nil);
								
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
EditTextControl::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Edit Text
	ioWriter.WriteValue(key_TextContent, GetText());
	
	ioWriter.WriteValue(key_IsPassword, mIsPassword);
	ioWriter.WriteValue(key_UseInlineInput, mUseInlineInput);
	
		// ### Write font style
		
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetText															  [public]
/**
	Sets the text in the edit field
	
	@param	inText		Text to put in edit field							*/

void
EditTextControl::SetText(
	CFStringRef	inText)
{
		// Convert CFString unicode characters to the system encoding
		// used by TextEdit

	CFString	textStr(inText);
	CFIndex		textLength = textStr.GetByteLength();
	CFData		textBuffer(textLength);
	textStr.GetByteRange(textLength, textBuffer.GetMutableBytePtr());
	
	SetDataTag(kControlEntireControl, kControlEditTextTextTag,
				textLength, textBuffer.GetBytePtr());
}


// ---------------------------------------------------------------------------
//	GetText															  [public]
/**
	Returns the text from the edit field
	
	@return Text from the edit field										*/

CFString
EditTextControl::GetText() const
{
	OSStatus	status;
									// Get length of text
	Size	textLength;
	status = GetDataTag(kControlEntireControl, kControlEditTextTextTag,
						0, nil, &textLength);
	PPx_ThrowIfOSError_(status, "Failed to get length of text");

									// Allocate buffer and copy text into it
	CFData	textBuffer(textLength);							
	status = GetDataTag(kControlEntireControl, kControlEditTextTextTag,
						textLength, textBuffer.GetMutableBytePtr(), nil);
	PPx_ThrowIfOSError_(status, "Failed to get text");
	
									// Make CFString from text buffer
	return CFString((void*) textBuffer.GetBytePtr(), textLength);
}


// ---------------------------------------------------------------------------
//	SetThemeFontID													  [public]
/**
	Sets the theme font ID
	
	@param	inFont		Theme font ID to use for text						*/

void
EditTextControl::SetThemeFontID(
	ThemeFontID		inFont)
{
	ViewUtils::SetControlThemeFontID(this, inFont);
}


} // namespace PPx
