// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxComboBox.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxComboBox.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_Attributes	= CFSTR("attributes");
	const CFStringRef	key_ItemList	= CFSTR("item list");
}


// ---------------------------------------------------------------------------
//	ComboBox														  [public]
/**
	Default constructor */

ComboBox::ComboBox()
{
}


// ---------------------------------------------------------------------------
//	~ComboBox														  [public]
/**
	Destructor */

ComboBox::~ComboBox()
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
	@param	inDefaultText	Initial text in the edit field
	@param	inStyle			Text style
	@param	inValueList		List of value to diplay as choices
	@param	inAttributes	Option flags									*/

void
ComboBox::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CFStringRef		inDefaultText,
	const ControlFontStyleRec*	inStyle,
	CFArrayRef		inValueList,
	OptionBits		inAttributes)
{
	HIViewRef	viewRef = SysCreateView::ComboBox(
								inFrame, inDefaultText, inStyle,
								inValueList, inAttributes);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ComboBox::ClassName() const
{
	return CFSTR("PPx::ComboBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ComboBox::InitState(
	const DataReader&	inReader)
{
	CFString			text;				// Data for Combo Box
									
	OptionBits			attributes = attributes_None;
	
	inReader.ReadOptional(key_TextContent, text);
	inReader.ReadOptional(key_Attributes, attributes);
	
											// Read List Items
	CFArray<CFStringRef>	listItems;
	if (inReader.ContainsKey(key_ItemList)) {
											// Read CFStrings into a vector
		std::vector<CFString>	stringList;
		inReader.ReadContainer<CFString>(key_ItemList,
										 std::back_inserter(stringList));
		
											// Put CFStringRef for each item
											//   into a CFArray
		listItems = CFArray<CFStringRef>(stringList.size(), &kCFTypeArrayCallBacks);
										 
		std::vector<CFString>::const_iterator	iter = stringList.begin();
		while  (iter != stringList.end()) {
			listItems.AppendValue((*iter).UseRef());
			++iter;
		}
	}
	
//	ControlFontStyleRec	style;		// ### Read font style
	
	HIRect	frame;
	::CGRectMake(0, 0, 1, 1);
	
	HIViewRef	viewRef = SysCreateView::ComboBox(
								frame, text, nil,
								listItems.GetRefValue(), attributes);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ComboBox::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Combo Box
	ioWriter.WriteValue(key_TextContent, GetText());
	
	ioWriter.WriteValue(key_Attributes, GetAttributes());
	
											// Write List Items
	CFArray<CFStringRef>	listItems = GetListItems();
	CFIndex	itemCount = listItems.GetCount();
	
	if (itemCount > 0) {
											// Store CFString for each item
											//   into a vector
		std::vector<CFString>	stringList;
		stringList.reserve(itemCount);
		
		for (CFIndex i = 0; i < itemCount; ++i) {
		
			stringList.push_back(CFString(listItems[i]));
		}
		
		ioWriter.WriteContainer(key_ItemList, stringList.begin(),
									stringList.end());
	}
	
		// ### Write font style
		
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetText														  [public]
/**
	Sets the text in the edit field of the combo box
	
	@param	inText		Text to put in edit field							*/

void
ComboBox::SetText(
	CFStringRef	inText)
{
	SetDataTag(kControlEntireControl, kControlEditTextCFStringTag,
				sizeof(inText), &inText);
}


// ---------------------------------------------------------------------------
//	GetText														  [public]
/**
	Returns the text from the edit field of the combo box
	
	@return Text from the edit field of the combo box						*/

CFString
ComboBox::GetText() const
{
	CFStringRef	text = nil;

	OSStatus	status =
		GetDataTag(kControlEntireControl, kControlEditTextCFStringTag,
					sizeof(text), &text);
					
	PPx_ThrowIfOSError_(status, "Failed to copy combo box text");
				
	return CFString(text, retain_No);
}


// ---------------------------------------------------------------------------
//	GetListItemsCount												  [public]
/**
	Returns the number of items in the list of the combo box
	
	@return Number of items in the list of the combo box					*/

SInt32
ComboBox::GetListItemsCount() const
{
	return ::HIComboBoxGetItemCount(GetSysView());
}


// ---------------------------------------------------------------------------
//	InsertListItemAt												  [public]
/**
	Inserts an item into the list of the combo box
	
	@param	inIndex		Index in list at which to insert item
	@param	inItemText	Text of item to insert								*/

void
ComboBox::InsertListItemAt(
	CFIndex			inIndex,
	CFStringRef		inItemText)
{
	OSStatus	status = ::HIComboBoxInsertTextItemAtIndex(
								GetSysView(), inIndex, inItemText);
					
	PPx_ThrowIfOSError_(status, "HIComboBoxInsertTextItemAtIndex failed");
}


// ---------------------------------------------------------------------------
//	AppendListItem													  [public]
/**
	Appends an item to the list of the combo box
	
	@param	inItemText	Text of item to append
	
	@return	Index at which item was appended								*/

CFIndex
ComboBox::AppendListItem(
	CFStringRef		inItemText)
{
	CFIndex		indexWhereAppended;
	
	OSStatus	status = ::HIComboBoxAppendTextItem(
								GetSysView(), inItemText, &indexWhereAppended);
					
	PPx_ThrowIfOSError_(status, "HIComboBoxAppendTextItem failed");
	
	return indexWhereAppended;
}


// ---------------------------------------------------------------------------
//	RemoveListItem													  [public]
/**
	Removes an item from the list of the combo box
	
	@param	inIndex		Index of item to remove								*/

void
ComboBox::RemoveListItem(
	CFIndex		inIndex)
{
	OSStatus	status = ::HIComboBoxRemoveItemAtIndex(GetSysView(), inIndex);
					
	PPx_ThrowIfOSError_(status, "HIComboBoxRemoveItemAtIndex failed");
}


// ---------------------------------------------------------------------------
//	GetListItemText													  [public]
/**
	Returns the text of an item in the list of the combo box
	
	@param	inIndex		Index of item whose text to get
	
	@return	String for the specified item									*/

CFString
ComboBox::GetListItemText(
	CFIndex		inIndex) const
{
	CFStringRef	itemText;
	OSStatus	status = ::HIComboBoxCopyTextItemAtIndex(
									GetSysView(), inIndex, &itemText);
					
	PPx_ThrowIfOSError_(status, "HIComboBoxRemoveItemAtIndex failed");
	
	return CFString(itemText, retain_No);
}


// ---------------------------------------------------------------------------
//	SetListItems													  [public]
/**
	Sets the list items of the combo box
	
	@param	inListItems		Array of CFStrings specifying the list items	*/

void
ComboBox::SetListItems(
	CFArrayRef	inListItems)
{
	SetDataTag(kHIComboBoxDisclosurePart, kHIComboBoxListTag,
				sizeof(inListItems), &inListItems);
}


// ---------------------------------------------------------------------------
//	GetListItems													  [public]
/**
	Returns the list items of the combo box as an array of strings
	
	@return	CFArray of CFStrings of the list items							*/

CFArray<CFStringRef>
ComboBox::GetListItems() const
{
	CFArrayRef	listItems;
	
	OSStatus	status =
		GetDataTag(kHIComboBoxDisclosurePart, kHIComboBoxListTag,
					sizeof(listItems), &listItems);

	PPx_ThrowIfOSError_(status, "Failed to copy combo box list items");
	
	return CFArray<CFStringRef>(listItems, retain_No);
}


// ---------------------------------------------------------------------------
//	ChangeAttributes												  [public]
/**
	Changes the attributes of the combo box
	
	@param	inAttributesToSet	Bit mask of attributes to set
	@param	inAttributesToClear	Bit mask of attributes to clear				*/

void
ComboBox::ChangeAttributes(
	OptionBits		inAttributesToSet,
	OptionBits		inAttributesToClear)
{
	OSStatus	status = ::HIComboBoxChangeAttributes(
									GetSysView(), inAttributesToSet,
									inAttributesToClear);
					
	PPx_ThrowIfOSError_(status, "HIComboBoxChangeAttributes failed");
}


// ---------------------------------------------------------------------------
//	GetAttributes													  [public]
/**
	Returns the attributes of the comno box
	
	@return Attributes of the comno box										*/

OptionBits
ComboBox::GetAttributes() const
{
	OptionBits	options;
	OSStatus	status = ::HIComboBoxGetAttributes(GetSysView(), &options);
					
	PPx_ThrowIfOSError_(status, "HIComboBoxGetAttributes failed");
	
	return options;
}


} // namespace PPx
