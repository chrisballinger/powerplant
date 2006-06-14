// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDisclosureButton.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDisclosureButton.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	DisclosureButton												  [public]
/**
	Default constructor */

DisclosureButton::DisclosureButton()
{
	mAutoToggle = true;
}


// ---------------------------------------------------------------------------
//	~DisclosureButton												  [public]
/**
	Destructor */

DisclosureButton::~DisclosureButton()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from disclosure button creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coordinates of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled	
	@param	inInitialValue		kControlDisclosureButtonClosed or
									kControlDisclosureButtonDisclosed
	@param	inAutoToggle		Whether button automatically toggles
									state when clicked						*/

void
DisclosureButton::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	SInt32			inInitialValue,
	bool			inAutoToggle)
{
	mAutoToggle = inAutoToggle;
	
	HIViewRef	viewRef = SysCreateView::DisclosureButton(
								inInitialValue, inAutoToggle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
DisclosureButton::ClassName() const
{
	return CFSTR("PPx::DisclosureButton");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
DisclosureButton::InitState(
	const DataReader&	inReader)
{
											// Data for Disclosure Button
	SInt32		initialValue = value_Closed;
	
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_AutoToggle, mAutoToggle);

	HIViewRef	viewRef = SysCreateView::DisclosureButton(
								initialValue, mAutoToggle);

	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
DisclosureButton::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Disclosure Button
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_AutoToggle, mAutoToggle);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
