// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysDataBrowser.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:35 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysDataBrowser.h>

namespace PPx {

namespace SysDataBrowser {

// ---------------------------------------------------------------------------
//	ItemDataUPP

ItemDataUPP::ItemDataUPP(
	DataBrowserItemDataProcPtr	inCallbackFunc)
{
	mItemDataUPP = ::NewDataBrowserItemDataUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ItemDataUPP

ItemDataUPP::~ItemDataUPP()
{
	::DisposeDataBrowserItemDataUPP(mItemDataUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ItemNotificationUPP

ItemNotificationUPP::ItemNotificationUPP(
	DataBrowserItemNotificationWithItemProcPtr	inCallbackFunc)
{
	mItemNotificationUPP = ::NewDataBrowserItemNotificationWithItemUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ItemNotificationUPP

ItemNotificationUPP::~ItemNotificationUPP()
{
	::DisposeDataBrowserItemNotificationWithItemUPP(mItemNotificationUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ItemCompareUPP

ItemCompareUPP::ItemCompareUPP(
	DataBrowserItemCompareProcPtr	inCallbackFunc)
{
	mItemCompareUPP = ::NewDataBrowserItemCompareUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ItemCompareUPP

ItemCompareUPP::~ItemCompareUPP()
{
	::DisposeDataBrowserItemCompareUPP(mItemCompareUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AddDragItemUPP

AddDragItemUPP::AddDragItemUPP(
	DataBrowserAddDragItemProcPtr	inCallbackFunc)
{
	mAddDragItemUPP = ::NewDataBrowserAddDragItemUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~AddDragItemUPP

AddDragItemUPP::~AddDragItemUPP()
{
	::DisposeDataBrowserAddDragItemUPP(mAddDragItemUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AcceptDragUPP

AcceptDragUPP::AcceptDragUPP(
	DataBrowserAcceptDragProcPtr	inCallbackFunc)
{
	mAcceptDragUPP = ::NewDataBrowserAcceptDragUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~AcceptDragUPP

AcceptDragUPP::~AcceptDragUPP()
{
	::DisposeDataBrowserAcceptDragUPP(mAcceptDragUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ReceiveDragUPP

ReceiveDragUPP::ReceiveDragUPP(
	DataBrowserReceiveDragProcPtr	inCallbackFunc)
{
	mReceiveDragUPP = ::NewDataBrowserReceiveDragUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ReceiveDragUPP

ReceiveDragUPP::~ReceiveDragUPP()
{
	::DisposeDataBrowserReceiveDragUPP(mReceiveDragUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	PostProcessDragUPP

PostProcessDragUPP::PostProcessDragUPP(
	DataBrowserPostProcessDragProcPtr	inCallbackFunc)
{
	mPostProcessDragUPP = ::NewDataBrowserPostProcessDragUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~PostProcessDragUPP

PostProcessDragUPP::~PostProcessDragUPP()
{
	::DisposeDataBrowserPostProcessDragUPP(mPostProcessDragUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetContextualMenuUPP

GetContextualMenuUPP::GetContextualMenuUPP(
	DataBrowserGetContextualMenuProcPtr	inCallbackFunc)
{
	mGetContextualMenuUPP = ::NewDataBrowserGetContextualMenuUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~GetContextualMenuUPP

GetContextualMenuUPP::~GetContextualMenuUPP()
{
	::DisposeDataBrowserGetContextualMenuUPP(mGetContextualMenuUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SelectContextualMenuUPP

SelectContextualMenuUPP::SelectContextualMenuUPP(
	DataBrowserSelectContextualMenuProcPtr	inCallbackFunc)
{
	mSelectContextualMenuUPP = ::NewDataBrowserSelectContextualMenuUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SelectContextualMenuUPP

SelectContextualMenuUPP::~SelectContextualMenuUPP()
{
	::DisposeDataBrowserSelectContextualMenuUPP(mSelectContextualMenuUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ItemHelpContentUPP

ItemHelpContentUPP::ItemHelpContentUPP(
	DataBrowserItemHelpContentProcPtr	inCallbackFunc)
{
	mItemHelpContentUPP = ::NewDataBrowserItemHelpContentUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ItemHelpContentUPP

ItemHelpContentUPP::~ItemHelpContentUPP()
{
	::DisposeDataBrowserItemHelpContentUPP(mItemHelpContentUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	DrawItemUPP

DrawItemUPP::DrawItemUPP(
	DataBrowserDrawItemProcPtr	inCallbackFunc)
{
	mDrawItemUPP = ::NewDataBrowserDrawItemUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~DrawItemUPP

DrawItemUPP::~DrawItemUPP()
{
	::DisposeDataBrowserDrawItemUPP(mDrawItemUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	EditItemUPP

EditItemUPP::EditItemUPP(
	DataBrowserEditItemProcPtr	inCallbackFunc)
{
	mEditItemUPP = ::NewDataBrowserEditItemUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~EditItemUPP

EditItemUPP::~EditItemUPP()
{
	::DisposeDataBrowserEditItemUPP(mEditItemUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	HitTestUPP

HitTestUPP::HitTestUPP(
	DataBrowserHitTestProcPtr	inCallbackFunc)
{
	mHitTestUPP = ::NewDataBrowserHitTestUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~HitTestUPP

HitTestUPP::~HitTestUPP()
{
	::DisposeDataBrowserHitTestUPP(mHitTestUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	TrackingUPP

TrackingUPP::TrackingUPP(
	DataBrowserTrackingProcPtr	inCallbackFunc)
{
	mTrackingUPP = ::NewDataBrowserTrackingUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~TrackingUPP

TrackingUPP::~TrackingUPP()
{
	::DisposeDataBrowserTrackingUPP(mTrackingUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ItemDragRgnUPP

ItemDragRgnUPP::ItemDragRgnUPP(
	DataBrowserItemDragRgnProcPtr	inCallbackFunc)
{
	mItemDragRgnUPP = ::NewDataBrowserItemDragRgnUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ItemDragRgnUPP

ItemDragRgnUPP::~ItemDragRgnUPP()
{
	::DisposeDataBrowserItemDragRgnUPP(mItemDragRgnUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ItemAcceptDragUPP

ItemAcceptDragUPP::ItemAcceptDragUPP(
	DataBrowserItemAcceptDragProcPtr	inCallbackFunc)
{
	mItemAcceptDragUPP = ::NewDataBrowserItemAcceptDragUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ItemAcceptDragUPP

ItemAcceptDragUPP::~ItemAcceptDragUPP()
{
	::DisposeDataBrowserItemAcceptDragUPP(mItemAcceptDragUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ItemReceiveDragUPP

ItemReceiveDragUPP::ItemReceiveDragUPP(
	DataBrowserItemReceiveDragProcPtr	inCallbackFunc)
{
	mItemReceiveDragUPP = ::NewDataBrowserItemReceiveDragUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~ItemReceiveDragUPP

ItemReceiveDragUPP::~ItemReceiveDragUPP()
{
	::DisposeDataBrowserItemReceiveDragUPP(mItemReceiveDragUPP);
}


} // namespace SysDataBrowser

} // namespace PPx