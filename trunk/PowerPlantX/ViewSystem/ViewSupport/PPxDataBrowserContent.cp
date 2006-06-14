// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataBrowserContent.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:47 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDataBrowserContent.h>
#include <SysDataBrowser.h>

namespace PPx {

namespace { // unnamed

const	OSType		property_PPxDataBrowserContent = 'DBCo';


// ---------------------------------------------------------------------------
//	GetContentObject

DataBrowser::Content*
GetContentObject(
	ControlRef	inBrowserRef)
{
	DataBrowser::Content*	theContent;
	
	OSStatus status = ::GetControlProperty(inBrowserRef, Signature::Get(),
								property_PPxDataBrowserContent,
								sizeof(theContent), nil, &theContent);
								
	PPx_ThrowIfOSError_(status, "Failed to get PPx content object from "
								"data browser control");
	
	return theContent;
}


// ---------------------------------------------------------------------------

template <typename TCallback, typename T1, typename T2>
void
InvokeCallback(
	ControlRef		inBrowserRef,
	T1				param1,
	T2				param2)
{
	DataBrowser::Content*	theContent = GetContentObject(inBrowserRef);
	
	TCallback*				theCallback = dynamic_cast<TCallback*>(theContent);
										
	theCallback->Invoke(param1, param2);
}


// ---------------------------------------------------------------------------

template <typename TCallback, typename T1, typename T2, typename T3>
void
InvokeCallback(
	ControlRef		inBrowserRef,
	T1				param1,
	T2				param2,
	T3				param3)
{
	DataBrowser::Content*	theContent = GetContentObject(inBrowserRef);
	
	TCallback*				theCallback = dynamic_cast<TCallback*>(theContent);
	
	theCallback->Invoke(param1, param2, param3);
}


// ---------------------------------------------------------------------------

template <typename TCallback, typename T1, typename T2, typename T3,
							  typename T4>
void
InvokeCallback(
	ControlRef		inBrowserRef,
	T1				param1,
	T2				param2,
	T3				param3,
	T4				param4)
{
	DataBrowser::Content*	theContent = GetContentObject(inBrowserRef);
	
	TCallback*				theCallback = dynamic_cast<TCallback*>(theContent);
										
	theCallback->Invoke(param1, param2, param3, param4);
}


// ---------------------------------------------------------------------------

template <typename TCallback, typename T1, typename T2, typename T3,
							  typename T4, typename T5>
void
InvokeCallback(
	ControlRef		inBrowserRef,
	T1				param1,
	T2				param2,
	T3				param3,
	T4				param4,
	T5				param5)
{
	DataBrowser::Content*	theContent = GetContentObject(inBrowserRef);
	
	TCallback*				theCallback = dynamic_cast<TCallback*>(theContent);
										
	theCallback->Invoke(param1, param2, param3, param4, param5);
}


// ---------------------------------------------------------------------------

template <typename TCallback, typename T1, typename T2, typename T3,
							  typename T4, typename T5, typename T6>
void
InvokeCallback(
	ControlRef		inBrowserRef,
	T1				param1,
	T2				param2,
	T3				param3,
	T4				param4,
	T5				param5,
	T6				param6)
{
	DataBrowser::Content*	theContent = GetContentObject(inBrowserRef);
	
	TCallback*				theCallback = dynamic_cast<TCallback*>(theContent);
										
	theCallback->Invoke(param1, param2, param3, param4, param5, param6);
}

} // unnamed namespace

#pragma mark -

namespace DataBrowser {

namespace {	// unnamed

// ---------------------------------------------------------------------------
//	Data Browser item-data callback function

pascal OSStatus
ItemDataCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	DataBrowserItemDataRef	inItemDataRef,
	Boolean					inSetValue)
{
	OSStatus	status = errDataBrowserPropertyNotSupported;

	try {
		DataBrowser::Content*	theContent = GetContentObject(inBrowserRef);
		
		status = theContent->Invoke(inItemID, inPropertyID, inItemDataRef,
										inSetValue);
	}
		
	catch ( const OSError& inError ) {
		status = inError.GetOSErrorCode();
	}
	
	catch (...) { }
	
	return status;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser item-data callback function

DataBrowserItemDataUPP
GetItemDataUPP()
{
	static SysDataBrowser::ItemDataUPP	sItemDataUPP(&ItemDataCallback);
	
	return sItemDataUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Content

Content::Content()
{
	mBrowserView = nil;
}


// ---------------------------------------------------------------------------
//	~Content

Content::~Content()
{
	Remove();
}


// ---------------------------------------------------------------------------
//	Install

void
Content::Install(
	DataBrowserView*	inBrowserView)
{
	mBrowserView	= inBrowserView;

	DataBrowserCallbacks	theCallbacks;
	theCallbacks.version = kDataBrowserLatestCallbacks;

	::InitDataBrowserCallbacks(&theCallbacks);
	
	theCallbacks.u.v1.itemDataCallback = GetItemDataUPP();
	
	LoadItemCallbacks(theCallbacks);
	
	::SetDataBrowserCallbacks(GetSysDataBrowser(), &theCallbacks);
	
	// Store pointer to "this" object as a property in the control ref
	
	Content*	thisContent = this;

	::SetControlProperty(GetSysDataBrowser(), Signature::Get(),
						property_PPxDataBrowserContent,
						sizeof(thisContent), &thisContent);
}


// ---------------------------------------------------------------------------
//	Remove

void
Content::Remove()
{
	if (mBrowserView != nil) {			// Reset callbacks to defaults
		DataBrowserCallbacks	theCallbacks;
		theCallbacks.version = kDataBrowserLatestCallbacks;

		::InitDataBrowserCallbacks(&theCallbacks);
		::SetDataBrowserCallbacks(GetSysDataBrowser(), &theCallbacks);
	}
}


// ---------------------------------------------------------------------------
//	Invoke															  [public]
/**
	Calls virtual functions to change/supply item data
*/

OSStatus
Content::Invoke(
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	DataBrowserItemDataRef	inItemDataRef,
	bool					inSetValue)
{
	OSStatus	status;

	if (inSetValue) {
		status = ChangeItemData(inItemID, inPropertyID, inItemDataRef);
	} else {
		status = SupplyItemData(inItemID, inPropertyID, inItemDataRef);
	}
	
	return status;
}


// ---------------------------------------------------------------------------
//	LoadItemCallbacks											   [protected]
/**
	Loads UPPs to data browser callback functions

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must override to load the callbacks which they implement		*/

void
Content::LoadItemCallbacks(
	DataBrowserCallbacks&	/* outCallbacks */)
{
}


// ---------------------------------------------------------------------------
//	ChangeItemData													 [private]

OSStatus
Content::ChangeItemData(
	DataBrowserItemID		/* inItemID */,
	DataBrowserPropertyID	/* inPropertyID */,
	DataBrowserItemDataRef	/* inItemDataRef */)
{
	return errDataBrowserPropertyNotSupported;
}

#pragma mark -

namespace {	// unnamed

// ---------------------------------------------------------------------------
//	Data Browser item-notification callback function

pascal void
ItemNotificationCallback(
	ControlRef					inBrowserRef,
	DataBrowserItemID			inItemID,
	DataBrowserItemNotification	inMessage,
	DataBrowserItemDataRef		inItemDataRef)
{
	try {
		InvokeCallback<DataBrowser::ItemNotification>(
			inBrowserRef, inItemID, inMessage, inItemDataRef);
	}
	
	catch (...) { }
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser item-notification callback function

DataBrowserItemNotificationWithItemUPP
GetItemNotificationUPP()
{
	static SysDataBrowser::ItemNotificationUPP	sItemNotificationUPP(&ItemNotificationCallback);
	
	return sItemNotificationUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
ItemNotification::Invoke(
	DataBrowserItemID			inItemID,
	DataBrowserItemNotification	inMessage,
	DataBrowserItemDataRef		inItemDataRef)
{
	NotifyAboutItem(inItemID, inMessage, inItemDataRef);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for item-notificaton callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
ItemNotification::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
		// Explain need to cast. WithItem vs. no item.

	outCallbacks.u.v1.itemNotificationCallback =
		(DataBrowserItemNotificationProcPtr) GetItemNotificationUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser item-compare callback function

pascal Boolean
ItemCompareCallback(
	ControlRef					inBrowserRef,
	DataBrowserItemID			inItemOneID,
	DataBrowserItemID			inItemTwoID,
	DataBrowserPropertyID		inSortPropertyID)
{
	bool	result = false;

	try {
		InvokeCallback<DataBrowser::ItemCompare>(
			inBrowserRef, inItemOneID, inItemTwoID, inSortPropertyID, &result);
	}
	
	catch (...) { }
	
	return result;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser item-compare callback function

DataBrowserItemCompareUPP
GetItemCompareUPP()
{
	static SysDataBrowser::ItemCompareUPP	sItemCompareUPP(&ItemCompareCallback);
	
	return sItemCompareUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
ItemCompare::Invoke(
	DataBrowserItemID		inItemIDOne,
	DataBrowserItemID		inItemIDTwo,
	DataBrowserPropertyID	inSortPropertyID,
	bool*					outResult)
{
	*outResult = CompareItems(inItemIDOne, inItemIDTwo, inSortPropertyID);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for item-compare callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
ItemCompare::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.itemCompareCallback = GetItemCompareUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser add-drag-item callback function

pascal Boolean
AddDragItemCallback(
	ControlRef			inBrowserRef,
	DragRef				inDragRef,
	DataBrowserItemID	inItemID,
	ItemReference*		outDragItemRef)
{
	bool	itemIsInDrag = false;

	try {
		InvokeCallback<DataBrowser::AddDragItem>(
			inBrowserRef, inDragRef, inItemID, outDragItemRef, &itemIsInDrag);
	}
	
	catch (...) { }
	
	return itemIsInDrag;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser add-drag-item callback function

DataBrowserAddDragItemUPP
GetAddDragItemUPP()
{
	static SysDataBrowser::AddDragItemUPP	sAddDragItemUPP(&AddDragItemCallback);
	
	return sAddDragItemUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
AddDragItem::Invoke(
	DragRef				inDragRef,
	DataBrowserItemID	inItemID,
	ItemReference*		outDragItemRef,
	bool*				outItemIsInDrag)
{
	*outItemIsInDrag = AddItemToDrag(inDragRef, inItemID, *outDragItemRef);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for add-drag-item callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
AddDragItem::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.addDragItemCallback = GetAddDragItemUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser accept-drag callback function

pascal Boolean
AcceptDragCallback(
	ControlRef			inBrowserRef,
	DragRef				inDragRef,
	DataBrowserItemID	inItemID)
{
	bool	isAcceptable = false;

	try {
		InvokeCallback<DataBrowser::AcceptDrag>(
			inBrowserRef, inDragRef, inItemID, &isAcceptable);
	}
	
	catch (...) { }
	
	return isAcceptable;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser accept-drag callback function

DataBrowserAcceptDragUPP
GetAcceptDragUPP()
{
	static SysDataBrowser::AcceptDragUPP	sAcceptDragUPP(&AcceptDragCallback);
	
	return sAcceptDragUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
AcceptDrag::Invoke(
	DragRef				inDragRef,
	DataBrowserItemID	inItemID,
	bool*				outDragIsAcceptable)
{
	*outDragIsAcceptable = DragIsAcceptable(inDragRef, inItemID);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for accept-drag callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
AcceptDrag::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.acceptDragCallback = GetAcceptDragUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser receive-drag callback function

pascal Boolean
ReceiveDragCallback(
	ControlRef			inBrowserRef,
	DragRef				inDragRef,
	DataBrowserItemID	inItemID)
{
	bool	dragSucceeded = false;

	try {
		InvokeCallback<DataBrowser::ReceiveDrag>(
			inBrowserRef, inDragRef, inItemID, &dragSucceeded);
	}
	
	catch (...) { }
	
	return dragSucceeded;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser accept-drag callback function

DataBrowserReceiveDragUPP
GetReceiveDragUPP()
{
	static SysDataBrowser::ReceiveDragUPP	sReceiveDragUPP(&ReceiveDragCallback);
	
	return sReceiveDragUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
ReceiveDrag::Invoke(
	DragRef				inDragRef,
	DataBrowserItemID	inItemID,
	bool*				outDragSucceeded)
{
	*outDragSucceeded = ReceiveDraggedItems(inDragRef, inItemID);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for receive-drag callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
ReceiveDrag::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.receiveDragCallback = GetReceiveDragUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser post-process-drag callback function

pascal void
PostProcessDragCallback(
	ControlRef			inBrowserRef,
	DragRef				inDragRef,
	OSStatus			inTrackDragResult)
{
	try {
		InvokeCallback<DataBrowser::PostProcessDrag>(
			inBrowserRef, inDragRef, inTrackDragResult);
	}
	
	catch (...) { }
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser post-process-drag callback function

DataBrowserPostProcessDragUPP
GetPostProcessDragUPP()
{
	static SysDataBrowser::PostProcessDragUPP	sPostProcessDragUPP(&PostProcessDragCallback);
	
	return sPostProcessDragUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
PostProcessDrag::Invoke(
	DragRef				inDragRef,
	OSStatus			inTrackDragResult)
{
	PostProcessDragFromBrowser(inDragRef, inTrackDragResult);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for post-process-drag callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
PostProcessDrag::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.postProcessDragCallback = GetPostProcessDragUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser item-help-content callback function

pascal void
ItemHelpContentCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	HMContentRequest		inRequest,
	HMContentProvidedType*	outProvidedType,
	HMHelpContentPtr		ioHelpContent)
{
	try {
		InvokeCallback<DataBrowser::ItemHelpContent>(
			inBrowserRef, inItemID, inPropertyID, inRequest,
			outProvidedType, ioHelpContent);
	}
	
	catch (...) { }
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser item-help-content callback function

DataBrowserItemHelpContentUPP
GetItemHelpContentUPP()
{
	static SysDataBrowser::ItemHelpContentUPP	sItemHelpContentUPP(&ItemHelpContentCallback);
	
	return sItemHelpContentUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
ItemHelpContent::Invoke(
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	HMContentRequest		inRequest,
	HMContentProvidedType*	outProvidedType,
	HMHelpContentPtr		ioHelpContent)
{
	if (inRequest == kHMSupplyContent) {
		SupplyHelpContent(inItemID, inPropertyID, *outProvidedType, ioHelpContent);
							
	} else if (inRequest == kHMDisposeContent) {
		DisposeHelpContent(inItemID, inPropertyID, *outProvidedType, ioHelpContent);
	}
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for post-process-drag callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
ItemHelpContent::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.itemHelpContentCallback = GetItemHelpContentUPP();
}


// ---------------------------------------------------------------------------
//	DisposeHelpContent												 [private]

void
ItemHelpContent::DisposeHelpContent(
	DataBrowserItemID		/* inItemID */,
	DataBrowserPropertyID	/* inPropertyID */,
	HMContentProvidedType&	/* outProvidedType */,
	HMHelpContentPtr		/* ioHelpContent */)
{
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser get-contextual-menu callback function

pascal void
GetContextualMenuCallback(
	ControlRef				inBrowserRef,
	MenuRef*				outMenu,
	UInt32*					outHelpType,
	CFStringRef*			outHelpItemString,
	AEDesc*					outSelection)
{
	try {
		InvokeCallback<DataBrowser::GetContextualMenu>(
			inBrowserRef, outMenu, outHelpType, outHelpItemString,
			outSelection);
	}
	
	catch (...) { }
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser get-contextual-menu callback function

DataBrowserGetContextualMenuUPP
GetGetContextualMenuUPP()
{
	static SysDataBrowser::GetContextualMenuUPP	sGetContextualMenuUPP(&GetContextualMenuCallback);
	
	return sGetContextualMenuUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
GetContextualMenu::Invoke(
	MenuRef*			outMenu,
	UInt32*				outHelpType,
	CFStringRef*		outHelpItemString,
	AEDesc*				outSelection)
{
	SupplyContextualMenu(*outMenu, *outHelpType, *outHelpItemString, *outSelection);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for get-contextual-menu callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
GetContextualMenu::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.getContextualMenuCallback = GetGetContextualMenuUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser select-contextual-menu callback function

pascal void
SelectContextualMenuCallback(
	ControlRef			inBrowserRef,
	MenuRef				inMenu,
	UInt32				inSelectionType,
	SInt16				inMenuID,
	MenuItemIndex		inMenuItem)
{
	try {
		InvokeCallback<DataBrowser::SelectContextualMenu>(
			inBrowserRef, inMenu, inSelectionType, inMenuID, inMenuItem);
	}
	
	catch (...) { }
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser select-contextual-menu callback function

DataBrowserSelectContextualMenuUPP
GetSelectContextualMenuUPP()
{
	static SysDataBrowser::SelectContextualMenuUPP	sSelectContextualMenuUPP(&SelectContextualMenuCallback);
	
	return sSelectContextualMenuUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke															  [public]

void
SelectContextualMenu::Invoke(
	MenuRef				inMenu,
	UInt32				inSelectionType,
	SInt16				inMenuID,
	MenuItemIndex		inMenuItem)
{
	ProcessContextualMenuSelection(inMenu, inSelectionType, inMenuID, inMenuItem);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for select-contextual-menu callback function

	@param	outCallbacks	Struct of UPPs to data browser callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::Content::LoadItemCallbacks()							*/
	
void
SelectContextualMenu::LoadCallback(
	DataBrowserCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.selectContextualMenuCallback = GetSelectContextualMenuUPP();
}

#pragma mark -

namespace {	// unnamed

// ---------------------------------------------------------------------------
//	Data Browser draw-item callback function

pascal void
DrawItemCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	DataBrowserItemState	inItemState,
	const Rect*				inItemBounds,
	SInt16					inBitDepth,
	Boolean					inIsColorDevice)
{
	try {
		InvokeCallback<DataBrowser::CustomContent>(
			inBrowserRef, inItemID, inPropertyID, inItemState, *inItemBounds,
			inBitDepth, inIsColorDevice);
	}
	
	catch (...) { }
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser draw-item callback function

DataBrowserDrawItemUPP
GetDrawItemUPP()
{
	static SysDataBrowser::DrawItemUPP	sDrawItemUPP(&DrawItemCallback);
	
	return sDrawItemUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	~CustomContent

CustomContent::~CustomContent()
{
	RemoveCustomCallbacks();
}


// ---------------------------------------------------------------------------
//	Install

void
CustomContent::Install(
	DataBrowserView*	inBrowserView)
{
	Content::Install(inBrowserView);		// Install standard callbacks

	DataBrowserCustomCallbacks	theCallbacks;
	theCallbacks.version = kDataBrowserLatestCustomCallbacks;

	::InitDataBrowserCustomCallbacks(&theCallbacks);
	
	theCallbacks.u.v1.drawItemCallback = GetDrawItemUPP();
	
	LoadCustomCallbacks(theCallbacks);
	
	::SetDataBrowserCustomCallbacks(GetSysDataBrowser(), &theCallbacks);
}


// ---------------------------------------------------------------------------
//	Remove

void
CustomContent::Remove()
{
	RemoveCustomCallbacks();
	Content::Remove();
}


// ---------------------------------------------------------------------------
//	Invoke

void
CustomContent::Invoke(
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	DataBrowserItemState	inItemState,
	const Rect&				inItemBounds,
	SInt16					inBitDepth,
	bool					inIsColorDevice)
{
	DrawItem(inItemID, inPropertyID, inItemState, inItemBounds,
				inBitDepth, inIsColorDevice);
}


// ---------------------------------------------------------------------------
//	LoadCustomCallbacks												 [private]
/**
	Loads UPPs to data browser custom callback functions

	@param	outCallbacks	Struct of UPPs to data browser custom callbacks
	
	Subclasses must override to load the custom callbacks which they
	implement																*/

void
CustomContent::LoadCustomCallbacks(
	DataBrowserCustomCallbacks&	/* outCallbacks */)
{
}


// ---------------------------------------------------------------------------
//	RemoveCustomCallbacks											 [private]

void
CustomContent::RemoveCustomCallbacks()
{
	if (GetSysDataBrowser() != nil) {	// Reset custom callbacks to defaults
		DataBrowserCustomCallbacks	theCallbacks;
		theCallbacks.version = kDataBrowserLatestCustomCallbacks;

		::InitDataBrowserCustomCallbacks(&theCallbacks);
		::SetDataBrowserCustomCallbacks(GetSysDataBrowser(), &theCallbacks);
	}
}


#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser edit-item callback function

pascal Boolean
EditItemCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	CFStringRef				inStringToEdit,
	Rect*					outMaxTextRect,
	Boolean*				outShrinkToFit)
{
	bool	startEditing = false;
	bool	shrinkToFit  = *outShrinkToFit;

	try {
		InvokeCallback<DataBrowser::EditItem>(
			inBrowserRef, inItemID, inPropertyID, inStringToEdit,
			outMaxTextRect, &shrinkToFit, &startEditing);
	}
	
	catch (...) { }
	
	*outShrinkToFit = shrinkToFit;
	
	return startEditing;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser edit-item callback function

DataBrowserEditItemUPP
GetEditItemUPP()
{
	static SysDataBrowser::EditItemUPP	sEditItemUPP(&EditItemCallback);
	
	return sEditItemUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke

void
EditItem::Invoke(
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	CFStringRef				inStringToEdit,
	Rect*					outMaxTextRect,
	bool*					outShrinkToFit,
	bool*					outStartEditing)
{
	*outStartEditing = StartEditingItem(inItemID, inPropertyID,
							inStringToEdit, *outMaxTextRect, *outShrinkToFit);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for edit-item callback function

	@param	outCallbacks	Struct of UPPs to data browser custom callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::CustomContent::LoadCustomCallbacks()					*/
	

void
EditItem::LoadCallback(
	DataBrowserCustomCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.editTextCallback = GetEditItemUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser hit-test callback function

pascal Boolean
HitTestCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	const Rect*				inItemBounds,
	const Rect*				inMouseRect)
{
	bool	itemIsHit = false;

	try {
		InvokeCallback<DataBrowser::HitTest>(
			inBrowserRef, inItemID, inPropertyID, *inItemBounds,
			*inMouseRect, &itemIsHit);
	}
	
	catch (...) { }
	
	return itemIsHit;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser hit-test callback function

DataBrowserHitTestUPP
GetHitTestUPP()
{
	static SysDataBrowser::HitTestUPP	sHitTestUPP(&HitTestCallback);
	
	return sHitTestUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke

void
HitTest::Invoke(
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	const Rect&				inItemBounds,
	const Rect&				inMouseRect,
	bool*					outItemIsHit)
{
	*outItemIsHit = ItemIsHit(inItemID, inPropertyID, inItemBounds, inMouseRect);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for hit-test callback function

	@param	outCallbacks	Struct of UPPs to data browser custom callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::CustomContent::LoadCustomCallbacks()					*/
	

void
HitTest::LoadCallback(
	DataBrowserCustomCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.hitTestCallback = GetHitTestUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser tracking callback function

pascal DataBrowserTrackingResult
TrackingCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	const Rect*				inItemBounds,
	Point					inStartPt,
	EventModifiers			inModifierKeys)
{
	DataBrowserTrackingResult	trackingResult = kDataBrowserNothingHit;

	try {
		InvokeCallback<DataBrowser::Tracking>(
			inBrowserRef, inItemID, inPropertyID, *inItemBounds,
			inStartPt, inModifierKeys, &trackingResult);
	}
	
	catch (...) { }
	
	return trackingResult;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser tracking callback function

DataBrowserTrackingUPP
GetTrackingUPP()
{
	static SysDataBrowser::TrackingUPP	sTackingUPP(&TrackingCallback);
	
	return sTackingUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke

void
Tracking::Invoke(
	DataBrowserItemID			inItemID,
	DataBrowserPropertyID		inPropertyID,
	const Rect&					inItemBounds,
	Point						inStartPt,
	EventModifiers				inModifierKeys,
	DataBrowserTrackingResult*	outTrackingResult)
{
	*outTrackingResult = TrackItem(inItemID, inPropertyID,
								inItemBounds, inStartPt, inModifierKeys);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for tracking callback function

	@param	outCallbacks	Struct of UPPs to data browser custom callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::CustomContent::LoadCustomCallbacks()					*/
	

void
Tracking::LoadCallback(
	DataBrowserCustomCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.trackingCallback = GetTrackingUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser item-drag-rgn callback function

pascal void
ItemDragRgnCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	const Rect*				inItemBounds,
	RgnHandle				outDragRgn)
{
	try {
		InvokeCallback<DataBrowser::ItemDragRgn>(
			inBrowserRef, inItemID, inPropertyID, *inItemBounds, outDragRgn);
	}
	
	catch (...) { }
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser item-drag-rgn callback function

DataBrowserItemDragRgnUPP
GetItemDragRgnUPP()
{
	static SysDataBrowser::ItemDragRgnUPP	sItemDragRgnUPP(&ItemDragRgnCallback);
	
	return sItemDragRgnUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke

void
ItemDragRgn::Invoke(
	DataBrowserItemID			inItemID,
	DataBrowserPropertyID		inPropertyID,
	const Rect&					inItemBounds,
	RgnHandle					outDragRgn)
{
	MakeItemDragRgn(inItemID, inPropertyID, inItemBounds, outDragRgn);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for item-drag-rgn callback function

	@param	outCallbacks	Struct of UPPs to data browser custom callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::CustomContent::LoadCustomCallbacks()					*/
	

void
ItemDragRgn::LoadCallback(
	DataBrowserCustomCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.dragRegionCallback = GetItemDragRgnUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser item-accept-drag callback function

pascal DataBrowserDragFlags
ItemAcceptDragCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	const Rect*				inItemBounds,
	DragRef					inDragRef)
{
	DataBrowserDragFlags	dragFlags = kDataBrowserItemNoState;

	try {
		InvokeCallback<DataBrowser::ItemAcceptDrag>(
			inBrowserRef, inItemID, inPropertyID, *inItemBounds,
			inDragRef, &dragFlags);
	}
	
	catch (...) { }
	
	return dragFlags;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser tracking callback function

DataBrowserItemAcceptDragUPP
GetItemAcceptDragUPP()
{
	static SysDataBrowser::ItemAcceptDragUPP	sItemAcceptDragUPP(&ItemAcceptDragCallback);
	
	return sItemAcceptDragUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke

void
ItemAcceptDrag::Invoke(
	DataBrowserItemID			inItemID,
	DataBrowserPropertyID		inPropertyID,
	const Rect&					inItemBounds,
	DragRef						inDragRef,
	DataBrowserDragFlags*		outDragFlags)
{
	*outDragFlags = DragIsAcceptable(inItemID, inPropertyID, inItemBounds, inDragRef);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for item-accept-drag callback function

	@param	outCallbacks	Struct of UPPs to data browser custom callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::CustomContent::LoadCustomCallbacks()					*/
	

void
ItemAcceptDrag::LoadCallback(
	DataBrowserCustomCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.acceptDragCallback = GetItemAcceptDragUPP();
}

#pragma mark -

namespace { // unnamed

// ---------------------------------------------------------------------------
//	Data Browser item-receive-drag callback function

pascal Boolean
ItemReceiveDragCallback(
	ControlRef				inBrowserRef,
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	DataBrowserDragFlags	inDragFlags,
	DragRef					inDragRef)
{
	bool	dragSuccess = false;

	try {
		InvokeCallback<DataBrowser::ItemReceiveDrag>(
			inBrowserRef, inItemID, inPropertyID, inDragFlags,
			inDragRef, &dragSuccess);
	}
	
	catch (...) { }
	
	return dragSuccess;
}


// ---------------------------------------------------------------------------
//	Returns UPP to Data Browser item-receive-drag callback function

DataBrowserItemReceiveDragUPP
GetItemReceiveDragUPP()
{
	static SysDataBrowser::ItemReceiveDragUPP	sItemReceiveDragUPP(&ItemReceiveDragCallback);
	
	return sItemReceiveDragUPP.Get();
}

} // unnamed namespace


// ---------------------------------------------------------------------------
//	Invoke

void
ItemReceiveDrag::Invoke(
	DataBrowserItemID		inItemID,
	DataBrowserPropertyID	inPropertyID,
	DataBrowserDragFlags	inDragFlags,
	DragRef					inDragRef,
	bool*					outDragSuccess)
{
	*outDragSuccess = ReceiveDrag(inItemID, inPropertyID, inDragFlags, inDragRef);
}


// ---------------------------------------------------------------------------
//	LoadCallback												   [protected]
/**
	Loads UPP for hit-test callback function

	@param	outCallbacks	Struct of UPPs to data browser custom callbacks
	
	Subclasses must call this function from their override of
	PPx::DataBrowser::CustomContent::LoadCustomCallbacks()					*/
	

void
ItemReceiveDrag::LoadCallback(
	DataBrowserCustomCallbacks&	outCallbacks)
{
	outCallbacks.u.v1.receiveDragCallback = GetItemReceiveDragUPP();
}


} // namespace DataBrowser

} // namespace PPx
