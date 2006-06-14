// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRoundButton.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxRoundButton.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_ButtonSize	= CFSTR("button size");
}


// ---------------------------------------------------------------------------
//	RoundButton													  [public]
/**
	Default constructor */

RoundButton::RoundButton()
{
}


// ---------------------------------------------------------------------------
//	~RoundButton													  [public]
/**
	Destructor */

RoundButton::~RoundButton()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from chasing arrows creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled	
	@param	inButtonSize	Size of round button (large or small)
	@param	inContent		Content of round button							*/

void
RoundButton::Initialize(
	View*							inSuperView,
	const HIRect&					inFrame,
	bool							inVisible,
	bool							inEnabled,
	ControlRoundButtonSize			inButtonSize,
	const ControlButtonContentInfo&	inContent)
{
	HIViewRef	viewRef = SysCreateView::RoundButton(
								inButtonSize, inContent);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
RoundButton::ClassName() const
{
	return CFSTR("PPx::RoundButton");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
RoundButton::InitState(
	const DataReader&	inReader)
{
											// Data for Round Button
	ControlRoundButtonSize		buttonSize = kControlSizeNormal;
	ControlButtonContentInfo	content;
	
	content.contentType = kControlNoContent;
	content.u.iconRef   = nil;
	
	inReader.ReadOptional(key_ButtonSize, buttonSize);
	inReader.ReadOptional(key_ContentType, content.contentType);
	inReader.ReadOptional(key_ContentResID, content.u.resID);
	
	HIViewRef	viewRef = SysCreateView::RoundButton(
								buttonSize, content);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
RoundButton::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Round Button
	ControlButtonContentInfo	content;
	GetContentInfo(content);
	
	ioWriter.WriteValue(key_ButtonSize, GetButtonSize());
	ioWriter.WriteValue(key_ContentType, content.contentType);
	
											// Write ID for resource content
	if ( (content.contentType > kControlNoContent)  &&
		 (content.contentType <= kControlContentICONRes) ) {
		ioWriter.WriteValue(key_ContentResID, content.u.resID);
	}
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetContentInfo												  [public]
/**
	Sets the content inforomation
	
	@param	inContent	Content inforomation								*/

void
RoundButton::SetContentInfo(
	const ControlButtonContentInfo&	inContent)
{
	SetDataTag(kControlButtonPart, kControlRoundButtonContentTag,
				sizeof(ControlButtonContentInfo), &inContent);
}


// ---------------------------------------------------------------------------
//	GetContentInfo												  [public]
/**
	Passes back the content inforomation
	
	@param	outContent	Content inforomation								*/

void
RoundButton::GetContentInfo(
	ControlButtonContentInfo&	outContent) const
{
	GetDataTag(kControlButtonPart, kControlRoundButtonContentTag,
				sizeof(ControlButtonContentInfo), &outContent);
}


// ---------------------------------------------------------------------------
//	SetButtonSize													  [public]
/**
	Sets the button size
	
	@param	inButtonSize		New size for button							*/

void
RoundButton::SetButtonSize(
	ControlRoundButtonSize	inButtonSize)
{
	SetDataTag(kControlButtonPart, kControlRoundButtonSizeTag,
				sizeof(ControlRoundButtonSize), &inButtonSize);
}


// ---------------------------------------------------------------------------
//	GetButtonSize													  [public]
/**
	Returns the button size
	
	@return Button size														*/

ControlRoundButtonSize
RoundButton::GetButtonSize() const
{
	ControlRoundButtonSize	buttonSize;
	
	GetDataTag(kControlButtonPart, kControlRoundButtonSizeTag,
				sizeof(ControlRoundButtonSize), &buttonSize);
				
	return buttonSize;
}


} // namespace PPx
