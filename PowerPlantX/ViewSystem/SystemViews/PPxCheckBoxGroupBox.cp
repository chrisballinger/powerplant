// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCheckBoxGroupBox.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxCheckBoxGroupBox.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CheckBoxGroupBox												  [public]
/**
	Default constructor */

CheckBoxGroupBox::CheckBoxGroupBox()
{
	mAutoToggle		= true;
	mIsPrimaryGroup = true;
}


// ---------------------------------------------------------------------------
//	~CheckBoxGroupBox												  [public]
/**
	Destructor */

CheckBoxGroupBox::~CheckBoxGroupBox()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from check box group box creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inTitle			Title of check box
	@param	inInitialValue	State of check box (0 = unchecked, 1 = checked,
								2 = mixed)
	@param	inIsPrimary		Group box kind (true = primary, false = secondary)
	@param	inAutoToggle	Whether box is checked/unchecked automatically
								when clicked								*/

void
CheckBoxGroupBox::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inTitle,
	SInt32			inInitialValue,
	bool			inIsPrimary,
	bool			inAutoToggle)
{
	mAutoToggle		= inAutoToggle;
	mIsPrimaryGroup = inIsPrimary;

	HIViewRef	viewRef = SysCreateView::CheckBoxGroupBox(
								inTitle, inInitialValue, inIsPrimary,
								inAutoToggle);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
CheckBoxGroupBox::ClassName() const
{
	return CFSTR("PPx::CheckBoxGroupBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
CheckBoxGroupBox::InitState(
	const DataReader&	inReader)
{
	CFString	title;						// Data for Check Box Group Box
	SInt32		initialValue = value_Off;

	inReader.ReadOptional(key_ViewTitle, title);
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_IsPrimaryGroup, mIsPrimaryGroup);
	inReader.ReadOptional(key_AutoToggle, mAutoToggle);

	HIViewRef	viewRef = SysCreateView::CheckBoxGroupBox(
								title, initialValue, mIsPrimaryGroup,
								mAutoToggle);
				
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
CheckBoxGroupBox::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Check Box Group Box
	ioWriter.WriteValue(key_ViewTitle, GetTitle());
	
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_IsPrimaryGroup, mIsPrimaryGroup);
	ioWriter.WriteValue(key_AutoToggle, mAutoToggle);
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetTitleRect													  [public]
/**
	Passes back the title rectangle for the check box group box
	
	@param	outTitleRect	Title rectangle									*/

void
CheckBoxGroupBox::GetTitleRect(
	Rect&	outTitleRect) const
{
	GetDataTag(kControlEntireControl, kControlGroupBoxTitleRectTag,
				sizeof(Rect), &outTitleRect);
}


} // namespace PPx
