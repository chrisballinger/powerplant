// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCheckBox.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxCheckBox.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CheckBox														  [public]
/**
	Default constructor */

CheckBox::CheckBox()
{
	mAutoToggle = true;
}


// ---------------------------------------------------------------------------
//	~CheckBox														  [public]
/**
	Destructor */

CheckBox::~CheckBox()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initializes from check box creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inTitle			Title of check box
	@param	inInitialValue	Initial value for check box
	@param	inAutoToggle	Whether check box toggles automatically when clicked
*/

void
CheckBox::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inTitle,
	SInt32			inInitialValue,
	bool			inAutoToggle)
{
	mAutoToggle = inAutoToggle;
	
	HIViewRef	viewRef = SysCreateView::CheckBox(
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
CheckBox::ClassName() const
{
	return CFSTR("PPx::CheckBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
CheckBox::InitState(
	const DataReader&	inReader)
{
	CFString	title;						// Data for Check Box
	SInt32		initialValue = value_Off;
	
	inReader.ReadOptional(key_ViewTitle, title);
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_AutoToggle, mAutoToggle);
	
	HIViewRef	viewRef = SysCreateView::CheckBox(
								title, initialValue, mAutoToggle);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
CheckBox::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Check Box
	ioWriter.WriteValue(key_ViewTitle, GetTitle());
	
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_AutoToggle, mAutoToggle);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
