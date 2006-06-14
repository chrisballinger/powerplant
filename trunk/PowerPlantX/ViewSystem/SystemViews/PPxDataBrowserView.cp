// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataBrowserView.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDataBrowserView.h>
#include <PPxDataBrowserContent.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_DBViewStyle		= CFSTR("data browser view style");
	const CFStringRef	key_DBContent		= CFSTR("data browser content");
}


// ---------------------------------------------------------------------------
//	DataBrowser														  [public]
/**
	Default constructor */

DataBrowserView::DataBrowserView()
{
}


// ---------------------------------------------------------------------------
//	~DataBrowser													  [public]
/**
	Destructor */

DataBrowserView::~DataBrowserView()
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
*/

void
DataBrowserView::Initialize(
	View*					inSuperView,
	const HIRect&			inFrame,
	bool					inVisible,
	bool					inEnabled,
	DataBrowserViewStyle	inStyle,
	DataBrowser::Content*	inContent)
{
	HIViewRef	viewRef = SysCreateView::DataBrowser(inStyle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
	
	mContent.reset(inContent);
	if (inContent != nil) {
		inContent->Install(this);
	}
}


// ---------------------------------------------------------------------------
//	ChangeContent													  [public]
/**
	Changes the Content object used by the DataBrowserView
	
	@param	inNewContent	New Content object. May be nil.
	
	@return	Old content object, which caller is responsible for deleting	*/
	
DataBrowser::Content*
DataBrowserView::ChangeContent(
	DataBrowser::Content*	inNewContent)
{
	DataBrowser::Content*	oldContent = mContent.release();
	
	if (oldContent != nil) {
		oldContent->Remove();
	}
	
	mContent.reset(inNewContent);
	
	if (inNewContent != nil) {
		inNewContent->Install(this);
	}
	
	return oldContent;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
DataBrowserView::ClassName() const
{
	return CFSTR("PPx::DataBrowserView");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
DataBrowserView::InitState(
	const DataReader&	inReader)
{
	DataBrowserViewStyle	viewStyle;		// Data for DataBrowser
	
	inReader.ReadRequired(key_DBViewStyle, viewStyle);
	
	DataBrowser::Content*	content;
	inReader.ReadObjectValue(key_DBContent, content);
	
	mContent.reset(content);

	HIViewRef	viewRef = SysCreateView::DataBrowser(viewStyle);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
	
	if (content != nil) {
		content->Install(this);
	}
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
DataBrowserView::WriteState(
	DataWriter&	ioWriter) const
{
	ioWriter.WriteValue(key_DBViewStyle, GetViewStyle());
	ioWriter.WriteObjectValue(key_DBContent, mContent.get());
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetCallbacks													  [public]

void
DataBrowserView::SetCallbacks(
	const DataBrowserCallbacks&	inCallbacks)
{
	OSStatus	status =
		::SetDataBrowserCallbacks(GetSysView(), &inCallbacks);
			
	PPx_ThrowIfOSError_(status, "SetDataBrowserCallbacks failed");
}


// ---------------------------------------------------------------------------
//	GetCallbacks													  [public]

void
DataBrowserView::GetCallbacks(
	DataBrowserCallbacks&	outCallbacks)
{
	OSStatus	status =
		::GetDataBrowserCallbacks(GetSysView(), &outCallbacks);
			
	PPx_ThrowIfOSError_(status, "GetDataBrowserCallbacks failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetViewStyle													  [public]
/**
	Returns the view style of the data browser
	
	@return	View style of the data browser
	
	@retval	kDataBrowserListView		Multiple column table
	@retval	kDataBrowserColumnView		Fixed navigation columns			*/

DataBrowserViewStyle
DataBrowserView::GetViewStyle() const
{
	DataBrowserViewStyle	viewStyle;
	OSStatus				status =
			::GetDataBrowserViewStyle(GetSysView(), &viewStyle);
			
	PPx_ThrowIfOSError_(status, "GetDataBrowserViewStyle failed");
	
	return viewStyle;
}


// ---------------------------------------------------------------------------
//	SetSortProperty													  [public]

void
DataBrowserView::SetSortProperty(
	DataBrowserPropertyID	inSortProperty)
{
	OSStatus	status =
		::SetDataBrowserSortProperty(GetSysView(), inSortProperty);
			
	PPx_ThrowIfOSError_(status, "SetDataBrowserSortProperty failed");
}


// ---------------------------------------------------------------------------
//	SetSortOrder													  [public]

void
DataBrowserView::SetSortOrder(
	DataBrowserSortOrder	inSortOrder)
{
	OSStatus	status =
		::SetDataBrowserSortOrder(GetSysView(), inSortOrder);
			
	PPx_ThrowIfOSError_(status, "SetDataBrowserSortOrder failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetDefaultRowHeight

void
DataBrowserView::SetDefaultRowHeight(
	UInt16	inHeight)
{
	OSStatus	status =
		::SetDataBrowserTableViewRowHeight(GetSysView(), inHeight);
			
	PPx_ThrowIfOSError_(status, "SetDataBrowserTableViewRowHeight failed");
}


// ---------------------------------------------------------------------------
//	SetDefaultColumnWidth

void
DataBrowserView::SetDefaultColumnWidth(
	UInt16	inWidth)
{
	OSStatus	status =
		::SetDataBrowserTableViewColumnWidth(GetSysView(), inWidth);
			
	PPx_ThrowIfOSError_(status, "SetDataBrowserTableViewColumnWidth failed");
}


// ---------------------------------------------------------------------------
//	AddColumn

void
DataBrowserView::AddColumn(
	const DataBrowserListViewColumnDesc&	inColDesc,
	DataBrowserTableViewColumnIndex			inPosition)
{
		// $$ OS Bug Workaround $$ Current OS (10.3.2)
		//
		// The column desc parameter to AddDataBrowserListViewColumn()
		// should be a const pointer since the function does not
		// change the struct. We use a const_cast as a workaround.

	OSStatus	status =
		::AddDataBrowserListViewColumn(
				GetSysView(),
				const_cast<DataBrowserListViewColumnDesc*>(&inColDesc),
				inPosition);

	PPx_ThrowIfOSError_(status, "AddDataBrowserListViewColumn failed");
}


// ---------------------------------------------------------------------------
//	SetDisclosureColumn

void
DataBrowserView::SetDisclosureColumn(
	DataBrowserTableViewColumnID	inColumnID,
	bool							inExpandableRows)
{
	OSStatus	status =
		::SetDataBrowserListViewDisclosureColumn(GetSysView(), inColumnID,
													inExpandableRows);
			
	PPx_ThrowIfOSError_(status, "SetDataBrowserListViewDisclosureColumn failed");
}


// ---------------------------------------------------------------------------
//	AddItems

void
DataBrowserView::AddItems(
	UInt32						inItemCount,
	const DataBrowserItemID*	inItemIDs,
	DataBrowserItemID			inContainer,
	DataBrowserPropertyID		inPreSortProperty)
{
	OSStatus	status =
		::AddDataBrowserItems(GetSysView(), inContainer, inItemCount,
								inItemIDs, inPreSortProperty);

	PPx_ThrowIfOSError_(status, "AddDataBrowserItems failed");
}


// ---------------------------------------------------------------------------
//	AddOneItem

void
DataBrowserView::AddOneItem(
	DataBrowserItemID		inItemID,
	DataBrowserItemID		inContainer)
{
	AddItems(1, &inItemID, inContainer);
}


// ---------------------------------------------------------------------------
//	RemoveItems

void
DataBrowserView::RemoveItems(
	UInt32						inItemCount,
	const DataBrowserItemID*	inItemIDs,
	DataBrowserItemID			inContainer,
	DataBrowserPropertyID		inPreSortProperty)
{
	OSStatus	status =
		::RemoveDataBrowserItems(GetSysView(), inContainer, inItemCount,
								inItemIDs, inPreSortProperty);

	PPx_ThrowIfOSError_(status, "RemoveDataBrowserItems failed");
}


// ---------------------------------------------------------------------------
//	RemoveOneItem

void
DataBrowserView::RemoveOneItem(
	DataBrowserItemID		inItemID,
	DataBrowserItemID		inContainer)
{
	RemoveItems(1, &inItemID, inContainer);
}


// ---------------------------------------------------------------------------
//	RemoveAllItems

void
DataBrowserView::RemoveAllItems()
{
	RemoveItems(0, nil);
}


// ---------------------------------------------------------------------------
//	UpdateItems

void
DataBrowserView::UpdateItems(
	UInt32						inItemCount,
	const DataBrowserItemID*	inItemIDs,
	DataBrowserPropertyID		inPropertyID,
	DataBrowserItemID			inContainer,
	DataBrowserPropertyID		inPreSortProperty)
{
	OSStatus	status =
		::UpdateDataBrowserItems(GetSysView(), inContainer, inItemCount,
								inItemIDs, inPreSortProperty, inPropertyID);

	PPx_ThrowIfOSError_(status, "UpdateDataBrowserItems failed");
}


// ---------------------------------------------------------------------------
//	UpdateOneItem

void
DataBrowserView::UpdateOneItem(
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inProperty,
	DataBrowserItemID		inContainer)
{
	UpdateItems(1, &inItemID, inProperty, inContainer);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetItemDataText

void
DataBrowserView::SetItemDataText(
	DataBrowserItemDataRef	inItemDataRef,
	CFStringRef				inText)
{
	OSStatus	status =
		::SetDataBrowserItemDataText(inItemDataRef, inText);

	PPx_ThrowIfOSError_(status, "SetDataBrowserItemDataText failed");
}


// ---------------------------------------------------------------------------
//	SetItemDataIcon

void
DataBrowserView::SetItemDataIcon(
	DataBrowserItemDataRef	inItemDataRef,
	IconRef					inIcon)
{
	OSStatus	status =
		::SetDataBrowserItemDataIcon(inItemDataRef, inIcon);

	PPx_ThrowIfOSError_(status, "SetDataBrowserItemDataIcon failed");
}


// ---------------------------------------------------------------------------
//	SetItemDataValue

void
DataBrowserView::SetItemDataValue(
	DataBrowserItemDataRef	inItemDataRef,
	SInt32					inValue)
{
	OSStatus	status =
		::SetDataBrowserItemDataValue(inItemDataRef, inValue);
		
	PPx_ThrowIfOSError_(status, "SetDataBrowserItemDataValue failed");
}


// ---------------------------------------------------------------------------
//	SetItemDataMinimum

void
DataBrowserView::SetItemDataMinimum(
	DataBrowserItemDataRef	inItemDataRef,
	SInt32					inMinimum)
{
	OSStatus	status =
		::SetDataBrowserItemDataMinimum(inItemDataRef, inMinimum);

	PPx_ThrowIfOSError_(status, "SetDataBrowserItemDataMinimum failed");
}


// ---------------------------------------------------------------------------
//	SetItemDataMaximum

void
DataBrowserView::SetItemDataMaximum(
	DataBrowserItemDataRef	inItemDataRef,
	SInt32					inMaximum)
{
	OSStatus	status =
		::SetDataBrowserItemDataMaximum(inItemDataRef, inMaximum);

	PPx_ThrowIfOSError_(status, "SetDataBrowserItemDataMaximum failed");
}


// ---------------------------------------------------------------------------
//	SetItemDataBoolean

void
DataBrowserView::SetItemDataBoolean(
	DataBrowserItemDataRef	inItemDataRef,
	bool					inBoolean)
{
	OSStatus	status =
		::SetDataBrowserItemDataBooleanValue(inItemDataRef, inBoolean);

	PPx_ThrowIfOSError_(status, "SetDataBrowserItemDataBooleanValue failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetItemDataText

CFString
DataBrowserView::GetItemDataText(
	DataBrowserItemDataRef	inItemDataRef) const
{
	CFStringRef		text;

	OSStatus	status =
		::GetDataBrowserItemDataText(inItemDataRef, &text);

	PPx_ThrowIfOSError_(status, "GetDataBrowserItemDataText failed");
	
	return CFString(text, retain_No);
}


// ---------------------------------------------------------------------------
//	GetItemDataValue

SInt32
DataBrowserView::GetItemDataValue(
	DataBrowserItemDataRef	inItemDataRef) const
{
	SInt32		value;

	OSStatus	status =
		::GetDataBrowserItemDataValue(inItemDataRef, &value);

	PPx_ThrowIfOSError_(status, "GetDataBrowserItemDataValue failed");
	
	return value;
}


// ---------------------------------------------------------------------------
//	GetItemDataValue

bool
DataBrowserView::GetItemDataBoolean(
	DataBrowserItemDataRef	inItemDataRef) const
{
	Boolean		value;

	OSStatus	status =
		::GetDataBrowserItemDataBooleanValue(inItemDataRef, &value);

	PPx_ThrowIfOSError_(status, "GetDataBrowserItemDataBooleanValue failed");
	
	return value;
}


#pragma mark -

// ---------------------------------------------------------------------------
//	SetSelectionFlags

void
DataBrowserView::SetSelectionFlags(
	DataBrowserSelectionFlags	inFlags)
{
	OSStatus	status =
		::SetDataBrowserSelectionFlags(GetSysView(), inFlags);

	PPx_ThrowIfOSError_(status, "SetDataBrowserSelectionFlags failed");
}


// ---------------------------------------------------------------------------
//	SetSelectedItems

void
DataBrowserView::SetSelectedItems(
	UInt32						inItemCount,
	const DataBrowserItemID*	inItemIDs,
	DataBrowserSetOption		inOption)
{
	OSStatus	status =
		::SetDataBrowserSelectedItems(GetSysView(), inItemCount,
										inItemIDs, inOption);

	PPx_ThrowIfOSError_(status, "SetDataBrowserSelectedItems failed");
}


// ---------------------------------------------------------------------------
//	SetHiliteStyle

void
DataBrowserView::SetHiliteStyle(
	DataBrowserTableViewHiliteStyle		inStyle)
{
	OSStatus	status =
		::SetDataBrowserTableViewHiliteStyle(GetSysView(), inStyle);

	PPx_ThrowIfOSError_(status, "SetDataBrowserTableViewHiliteStyle failed");
}
	

} // namespace PPx
