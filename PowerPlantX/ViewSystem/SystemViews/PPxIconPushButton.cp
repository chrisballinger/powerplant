// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxIconPushButton.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxIconPushButton.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_IconAlignment	= CFSTR("icon alignment");
}


// ---------------------------------------------------------------------------
//	IconPushButton												  [public]
/**
	Default constructor */

IconPushButton::IconPushButton()
{
	mContent.contentType = kControlNoContent;
	mContent.u.iconRef   = nil;
	mIconAlignment		 = kControlPushButtonIconOnLeft;
}


// ---------------------------------------------------------------------------
//	~IconPushButton												  [public]
/**
	Destructor */

IconPushButton::~IconPushButton()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from icon push button creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inTitle			Title for button
	@param	inContent		Content of icon push button
	@param	inAlignment		Alignement of icon in the button				*/

void
IconPushButton::Initialize(
	View*							inSuperView,
	const HIRect&					inFrame,
	bool							inVisible,
	bool							inEnabled,
	CFStringRef						inTitle,
	const ControlButtonContentInfo&	inContent,
	ControlPushButtonIconAlignment	inAlignment)
{
	mContent	   = inContent;
	mIconAlignment = inAlignment;
	
	HIViewRef	viewRef = SysCreateView::IconPushButton(
								inTitle, inContent, inAlignment);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
IconPushButton::ClassName() const
{
	return CFSTR("PPx::IconPushButton");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
IconPushButton::InitState(
	const DataReader&	inReader)
{
	CFString	title;						// Data for Icon Push Button
	
	inReader.ReadOptional(key_ViewTitle, title);
	inReader.ReadOptional(key_ContentType, mContent.contentType);
	inReader.ReadOptional(key_ContentResID, mContent.u.resID);
	inReader.ReadOptional(key_IconAlignment, mIconAlignment);

	HIViewRef	viewRef = SysCreateView::IconPushButton(
								title, mContent, mIconAlignment);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
IconPushButton::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Push Button
	ioWriter.WriteValue(key_ViewTitle, GetTitle());
	
	ioWriter.WriteValue(key_ContentType, mContent.contentType);
	
											// Write ID for resource content
	if ( (mContent.contentType > kControlNoContent)  &&
		 (mContent.contentType <= kControlContentICONRes) ) {
		ioWriter.WriteValue(key_ContentResID, mContent.u.resID);
	}
	ioWriter.WriteValue(key_IconAlignment, mIconAlignment);
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetDefaultFlag												  [public]
/**
	Sets whether this is the default button
	
	@param	inIsDefault		Whether this is the default button				*/

void
IconPushButton::SetDefaultFlag(
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
IconPushButton::GetDefaultFlag() const
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
IconPushButton::SetCancelFlag(
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
IconPushButton::GetCancelFlag() const
{
	Boolean	isCancel;

	GetDataTag(kControlButtonPart, kControlPushButtonCancelTag,
				sizeof(Boolean), &isCancel);
				
	return isCancel;
}


} // namespace PPx
