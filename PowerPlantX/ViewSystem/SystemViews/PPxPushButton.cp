// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPushButton.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxPushButton.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	PushButton													  [public]
/**
	Default constructor */

PushButton::PushButton()
{
}


// ---------------------------------------------------------------------------
//	~PushButton													  [public]
/**
	Destructor */

PushButton::~PushButton()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from push button creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inTitle			Text title for button							*/

void
PushButton::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inTitle)
{
	HIViewRef	viewRef = SysCreateView::PushButton(inTitle);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
PushButton::ClassName() const
{
	return CFSTR("PPx::PushButton");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
PushButton::InitState(
	const DataReader&	inReader)
{
	CFString	title;						// Data for Push Button
	inReader.ReadOptional(key_ViewTitle, title);
	
	HIViewRef	viewRef = SysCreateView::PushButton(title);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
PushButton::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Push Button
	ioWriter.WriteValue(key_ViewTitle, GetTitle());

	View::WriteState(ioWriter);				// Data for all Views
}


// ---------------------------------------------------------------------------
//	SetDefaultFlag												  [public]
/**
	Sets whether this is the default button
	
	@param	inIsDefault		Whether this is the default button				*/

void
PushButton::SetDefaultFlag(
	bool	inIsDefault)
{
	Boolean	isDefault = inIsDefault;

	SetDataTag(kControlButtonPart, kControlPushButtonDefaultTag,
				sizeof(Boolean), &isDefault);
}


// ---------------------------------------------------------------------------
//	GetDefaultFlag												  [public]
/**
	Returns whether this is the default button
	
	@return Whether this is the default button								*/

bool
PushButton::GetDefaultFlag() const
{
	Boolean	isDefault;

	GetDataTag(kControlButtonPart, kControlPushButtonDefaultTag,
				sizeof(Boolean), &isDefault);
				
	return isDefault;
}


// ---------------------------------------------------------------------------
//	SetCancelFlag													  [public]
/**
	Sets whether this is the cancel button
	
	@param	inIsCancel		Whether this is the cancel button				*/

void
PushButton::SetCancelFlag(
	bool	inIsCancel)
{
	Boolean	isCancel = inIsCancel;

	SetDataTag(kControlButtonPart, kControlPushButtonCancelTag,
				sizeof(Boolean), &isCancel);
}


// ---------------------------------------------------------------------------
//	GetCancelFlag													  [public]
/**
	Returns whether this is the cancel button
	
	@return Whether this is the cancel button								*/

bool
PushButton::GetCancelFlag() const
{
	Boolean	isCancel;

	GetDataTag(kControlButtonPart, kControlPushButtonCancelTag,
				sizeof(Boolean), &isCancel);
				
	return isCancel;
}


} // namespace PPx
