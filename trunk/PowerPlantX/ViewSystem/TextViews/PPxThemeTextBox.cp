// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxThemeTextBox.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxThemeTextBox.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>

namespace PPx {

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_ThemeFontID			= CFSTR("theme font id");
	const CFStringRef	key_OneLineOnly			= CFSTR("one line only");
}

// ---------------------------------------------------------------------------
//	ThemeTextBox													  [public]
/**
	Default constructor */

ThemeTextBox::ThemeTextBox()
{
	mThemeFontID	= kThemeSystemFont;
	mJustification	= teFlushDefault;
	mOneLineOnly	= false;
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initializes from parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inText			Text to draw
	@param	inThemeFontID	Theme font ID for drawing text
	@param	inJustification	Text justification
	@param	inOneLineOnly	Whether text is all on one line or word wraps	*/

void
ThemeTextBox::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inText,
	ThemeFontID		inThemeFontID,
	SInt16			inJustification,
	bool			inOneLineOnly)
{
	mText			= inText;
	mThemeFontID	= inThemeFontID;
	mJustification	= inJustification;
	mOneLineOnly	= inOneLineOnly;
	
	BaseView::Initialize(inSuperView, inFrame, inVisible, inEnabled);
}


// ---------------------------------------------------------------------------
//	FinishInit													   [protected]
/**
	Finishes initialization */

void
ThemeTextBox::FinishInit()
{
	ControlDrawDoer::Install(GetSysEventTarget());
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ThemeTextBox::ClassName() const
{
	return CFSTR("PPx::ThemeTextBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ThemeTextBox::InitState(
	const DataReader&	inReader)
{
											// Data for Theme Text Box	
	inReader.ReadOptional(key_TextContent,			mText);
	inReader.ReadOptional(key_ThemeFontID,			mThemeFontID);
	inReader.ReadOptional(key_TextJustification,	mJustification);
	inReader.ReadOptional(key_OneLineOnly,			mOneLineOnly);
	
	PPx::BaseView::InitState(inReader);		// Data for superclass
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ThemeTextBox::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Theme Text Box
	ioWriter.WriteValue(key_TextContent,		mText);
	ioWriter.WriteValue(key_ThemeFontID,		mThemeFontID);
	ioWriter.WriteValue(key_TextJustification,	mJustification);
	ioWriter.WriteValue(key_OneLineOnly,		mOneLineOnly);
	
	PPx::BaseView::WriteState(ioWriter);	// Data for superclass
	
}


// ---------------------------------------------------------------------------
//	DoControlDraw												   [protected]
/**
	Draws the view
	
	@param	ioEvent			CarbonEvent for control draw
	@param	inControl		ControlRef for the view
	@param	inPartCode		Part of the view to draw
	@param	inClipRgn		Clipping region
	@param	inContext		CGContext for drawing
	
	@return	Status of drawing event. Always returns noErr.					*/

OSStatus
ThemeTextBox::DoControlDraw(
	SysCarbonEvent&		/* ioEvent */,
	ControlRef			/* inControl */,
	ControlPartCode		/* inPartCode */,
	RgnHandle			/* inClipRgn */,
	CGContextRef		inContext)
{
	HIRect	frame;
	GetLocalFrame(frame);
	
	Rect	frameRect;
	ViewUtils::HIToQDRect(frame, frameRect);
	
	ThemeDrawState	drawState = IsActive() ? kThemeStateActive
										   : kThemeStateInactive;
	
	OSStatus	status = ::DrawThemeTextBox( mText, mThemeFontID, drawState,
											 !mOneLineOnly, &frameRect,
											 mJustification, inContext );
											 
	return noErr;
}


// ---------------------------------------------------------------------------
//	SetText															  [public]
/**
	Sets the text to display
	
	@param	inText		Text string											*/

void
ThemeTextBox::SetText(
	CFStringRef	inText)
{
	mText.AttachRef(inText, retain_Yes);
}


// ---------------------------------------------------------------------------
//	GetText															  [public]
/**
	Returns the text string
	
	@return	Text string														*/

CFString
ThemeTextBox::GetText() const
{
	return mText;
}

} // namespace PPx
