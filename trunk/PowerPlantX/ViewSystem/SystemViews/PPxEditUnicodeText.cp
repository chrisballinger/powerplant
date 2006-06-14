// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEditUnicodeText.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxEditUnicodeText.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_IsPassword		= CFSTR("is password");
}

// ---------------------------------------------------------------------------
//	EditUnicodeText													  [public]
/**
	Default constructor */

EditUnicodeText::EditUnicodeText()
{
	mIsPassword	= false;
}


// ---------------------------------------------------------------------------
//	~EditUnicodeText												  [public]
/**
	Destructor */

EditUnicodeText::~EditUnicodeText()
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
	@param	inStyle				Text style									*/

void
EditUnicodeText::Initialize(
	View*						inSuperView,
	const HIRect&				inFrame,
	bool						inVisible,
	bool						inEnabled,
	CFStringRef					inText,
	bool						inIsPassword,
	const ControlFontStyleRec*	inStyle)
{
	mIsPassword	= inIsPassword;
	
		// $$ OS Bug Workaround $$ Current OS (10.2.6)
		//
		// Pass nil for the initial text, then set the text after we
		// initialize the base View class. If we don't do this, the text
		// may draw at the wrong location within the frame.
		//
		// This happens because SysCreateView uses 1x1 rectangle when
		// creating the system view, which is then resized to the desired
		// size by View::Initialize(). If there is text in the view during
		// this resize, the system gets confused. Setting the text after
		// the frame is at its desired size avoids the problem.

	HIViewRef	viewRef = SysCreateView::EditUnicodeText(
								nil, inIsPassword, inStyle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
	
	SetText(inText);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
EditUnicodeText::ClassName() const
{
	return CFSTR("PPx::EditUnicodeText");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
EditUnicodeText::InitState(
	const DataReader&	inReader)
{
	CFString			text;				// Data for Edit Unicode Text
	
//	ControlFontStyleRec	style;		// ### Read font style

	inReader.ReadOptional(key_TextContent, text);
	inReader.ReadOptional(key_IsPassword, mIsPassword);
	
		// See discussion for Initialize() for why we set the
		// text after initialiizing the base View.
	
	HIViewRef	viewRef = SysCreateView::EditUnicodeText(
								nil, mIsPassword, nil);
								
	View::InitViewState(viewRef, inReader);	// Data for all Views
	
	SetText(text);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
EditUnicodeText::WriteState(
	DataWriter&	ioWriter) const
{	
											// Data for Edit Unicode Text
	ioWriter.WriteValue(key_TextContent, GetText());
	ioWriter.WriteValue(key_IsPassword, mIsPassword);
	
		// ### Write font style
		
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetText														  [public]
/**
	Sets the text in the edit field
	
	@param	inText		Text to put in edit field							*/

void
EditUnicodeText::SetText(
	CFStringRef	inText)
{
	SetDataTag(kControlEntireControl, kControlEditTextCFStringTag,
				sizeof(inText), &inText);
}


// ---------------------------------------------------------------------------
//	GetText														  [public]
/**
	Returns the text from the edit field
	
	@return Text from the edit field										*/

CFString
EditUnicodeText::GetText() const
{
	CFStringRef	text = nil;

	OSStatus	status =
		GetDataTag(kControlEntireControl, kControlEditTextCFStringTag,
					sizeof(text), &text);
					
	PPx_ThrowIfOSError_(status, "Failed to copy edit text content");
				
	return CFString(text, retain_No);
}


// ---------------------------------------------------------------------------
//	SetThemeFontID												  [public]
/**
	Sets the theme font ID
	
	@param	inFont		Theme font ID to use for text						*/

void
EditUnicodeText::SetThemeFontID(
	ThemeFontID		inFont)
{
	ViewUtils::SetControlThemeFontID(this, inFont);
}


} // namespace PPx
