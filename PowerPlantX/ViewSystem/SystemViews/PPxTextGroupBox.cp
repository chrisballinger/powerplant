// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTextGroupBox.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxTextGroupBox.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	TextGroupBox													  [public]
/**
	Default constructor */

TextGroupBox::TextGroupBox()
{
	mIsPrimaryGroup = true;
}


// ---------------------------------------------------------------------------
//	~TextGroupBox													  [public]
/**
	Destructor */

TextGroupBox::~TextGroupBox()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from tab view creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inTitle			Title for text group box
	@param	inIsPrimary		Group box kind (true = primary, false = secondary)
*/

void
TextGroupBox::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inTitle,
	bool			inIsPrimary)
{
	mIsPrimaryGroup = inIsPrimary;
	
	HIViewRef	viewRef = SysCreateView::TextGroupBox(inTitle, inIsPrimary);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
TextGroupBox::ClassName() const
{
	return CFSTR("PPx::TextGroupBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
TextGroupBox::InitState(
	const DataReader&	inReader)
{
	CFString	title;						// Data for Text Group Box

	inReader.ReadOptional(key_ViewTitle,		title);
	inReader.ReadOptional(key_IsPrimaryGroup,	mIsPrimaryGroup);
	
	HIViewRef	viewRef = SysCreateView::TextGroupBox(title, mIsPrimaryGroup);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
TextGroupBox::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Text Group Box
	ioWriter.WriteValue(key_ViewTitle,		GetTitle());
	ioWriter.WriteValue(key_IsPrimaryGroup,	mIsPrimaryGroup);
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetThemeFontID												  [public]
/**
	Sets the theme font ID
	
	@param	inFont		Theme font ID to use for text						*/

void
TextGroupBox::SetThemeFontID(
	ThemeFontID		inFont)
{
	ViewUtils::SetControlThemeFontID(this, inFont);
}


// ---------------------------------------------------------------------------
//	GetTitleRect													  [public]
/**
	Passes back the title rectangle for the text group box
	
	@param	outTitleRect	Title rectangle									*/

void
TextGroupBox::GetTitleRect(
	Rect&	outTitleRect) const
{
	GetDataTag(kControlEntireControl, kControlGroupBoxTitleRectTag,
				sizeof(Rect), &outTitleRect);
}


} // namespace PPx
