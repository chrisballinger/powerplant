// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSearchField.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxSearchField.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

// The system search field is only available on Mac OS 10.3 or later
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

namespace PPx {


// ---------------------------------------------------------------------------
//	SearchField														  [public]
/**
	Default constructor */

SearchField::SearchField()
{
}


// ---------------------------------------------------------------------------
//	~SearchField													  [public]
/**
	Destructor */

SearchField::~SearchField()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from search field creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inAttributes		Initial attributes of the search field
	@param	inSearchMenu		Menu to be associated with this search field
	@param	inDescriptiveText	Text to be displayed in the text field when
								the field does not have focus and contains
								no user entered text						*/

void
SearchField::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	OptionBits		inAttributes,
	MenuRef			inSearchMenu,
	CFStringRef		inDescriptiveText)
{
	HIViewRef	viewRef = SysCreateView::SearchField(inAttributes,
								inSearchMenu, inDescriptiveText);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
SearchField::ClassName() const
{
	return CFSTR("PPx::SearchField");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
SearchField::InitState(
	const DataReader&	inReader)
{
	// ### Not Finished
											// Data for search field
	OptionBits	attributes = kHISearchFieldNoAttributes;
	MenuRef		searchMenu = nil;
	CFStringRef	descriptiveText = nil;

	HIViewRef	viewRef = SysCreateView::SearchField(attributes,
								searchMenu, descriptiveText);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
SearchField::WriteState(
	DataWriter&	ioWriter) const
{
	// ### Not Finished
											// Data for search field

	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetSearchMenu													  [public]
/**
	Sets the menu to be associated with this search field
	
	@param	inSearchMenu	Menu to be associated with this search field	*/

void
SearchField::SetSearchMenu( MenuRef inSearchMenu )
{
	OSStatus	status = ::HISearchFieldSetSearchMenu(
								GetSysView(), inSearchMenu);
									
	PPx_ThrowIfOSError_(status, "HISearchFieldSetSearchMenu failed");
}


// ---------------------------------------------------------------------------
//	GetSearchMenu													  [public]
/**
	Gets the menu associated with this search field
	
	@return	Search Menu of the search field									*/

MenuRef
SearchField::GetSearchMenu() const
{
	MenuRef		searchMenu = nil;
	OSStatus	status = ::HISearchFieldGetSearchMenu(
								GetSysView(), &searchMenu);
									
	PPx_ThrowIfOSError_(status, "HISearchFieldGetSearchMenu failed");
	
	return searchMenu;
}


// ---------------------------------------------------------------------------
//	ChangeAttributes												  [public]
/**
	+++
	
	@param	inAttributesToSet	+++
	@param	inAttributesToClear	+++	*/

void
SearchField::ChangeAttributes(
	OptionBits	inAttributesToSet,
	OptionBits	inAttributesToClear)
{
	OSStatus	status = ::HISearchFieldChangeAttributes(
								GetSysView(), inAttributesToSet,
								inAttributesToClear);
									
	PPx_ThrowIfOSError_(status, "HISearchFieldChangeAttributes failed");
}


// ---------------------------------------------------------------------------
//	GetAttributes													  [public]
/**
	Gets the attributes associated with this search field
	
	@return	+++												*/

OptionBits
SearchField::GetAttributes() const
{
	OptionBits	attributes = kHISearchFieldNoAttributes;
	OSStatus	status = ::HISearchFieldGetAttributes(
								GetSysView(), &attributes);
									
	PPx_ThrowIfOSError_(status, "HISearchFieldGetAttributes failed");
	
	return attributes;
}


// ---------------------------------------------------------------------------
//	SetDescriptiveText												  [public]
/**
	Sets the descriptive text of the search field
	
	@param	inText	Descriptive text of the search field (can be nil)		*/

void
SearchField::SetDescriptiveText( CFStringRef inText )
{
	OSStatus	status = ::HISearchFieldSetDescriptiveText(
								GetSysView(), inText);
									
	PPx_ThrowIfOSError_(status, "HISearchFieldSetDescriptiveText failed");
}


// ---------------------------------------------------------------------------
//	GetDescriptiveText												  [public]
/**
	Returns the descriptive text of the search field
	
	@return	Descriptive text of the search field							*/

CFString
SearchField::GetDescriptiveText() const
{
	CFStringRef	text = nil;
	OSStatus	status = ::HISearchFieldCopyDescriptiveText(
								GetSysView(), &text);
									
	PPx_ThrowIfOSError_(status, "HISearchFieldCopyDescriptiveText failed");
	
	return CFString(text, retain_No);
}


} // namespace PPx

#endif // 10.3 or later
