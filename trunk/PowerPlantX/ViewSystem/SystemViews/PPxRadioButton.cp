// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRadioButton.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxRadioButton.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	RadioButton													  [public]
/**
	Default constructor */

RadioButton::RadioButton()
{
	mAutoToggle = true;
}


// ---------------------------------------------------------------------------
//	~RadioButton													  [public]
/**
	Destructor */

RadioButton::~RadioButton()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initializes from redio button creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inTitle			Title of radio button
	@param	inInitialValue	Initial value for radio button
	@param	inAutoToggle	Whether button toggles automatically when clicked
*/

void
RadioButton::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inTitle,
	SInt32			inInitialValue,
	bool			inAutoToggle)
{
	mAutoToggle = inAutoToggle;
	
	HIViewRef	viewRef = SysCreateView::RadioButton(
								inTitle, inInitialValue, inAutoToggle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
RadioButton::ClassName() const
{
	return CFSTR("PPx::RadioButton");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
RadioButton::InitState(
	const DataReader&	inReader)
{
	CFString	title;						// Data for Radio Button
	SInt32		initialValue = value_Off;

	inReader.ReadOptional(key_ViewTitle, title);
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_AutoToggle, mAutoToggle);
	
	
	HIViewRef	viewRef = SysCreateView::RadioButton(
								title, initialValue, mAutoToggle);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
RadioButton::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Check Box
	ioWriter.WriteValue(key_ViewTitle, GetTitle());
	
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_AutoToggle, mAutoToggle);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
