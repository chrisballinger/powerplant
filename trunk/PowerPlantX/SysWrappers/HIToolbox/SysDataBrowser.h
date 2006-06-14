// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysDataBrowser.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:36 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysDataBrowser.h
	@brief		Classes for using a system Data Browser view
*/

#ifndef H_SysDataBrowser
#define H_SysDataBrowser
#pragma once

#include <Carbon/Carbon.h>

namespace PPx {

namespace SysDataBrowser {

// ===========================================================================
//	ItemDataUPP
/**
	Wrapper class for a Data Browser item-data callback function UPP */

class	ItemDataUPP {
public:
	explicit			ItemDataUPP( DataBrowserItemDataProcPtr inCallbackFunc );
	
						~ItemDataUPP();
					
	DataBrowserItemDataUPP
						Get() const;
						
private:								// Forbid copy and assignment
						ItemDataUPP( const ItemDataUPP& );
	ItemDataUPP&		operator = ( const ItemDataUPP& );
	
private:
	DataBrowserItemDataUPP	mItemDataUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserItemDataUPP
ItemDataUPP::Get() const
{
	return mItemDataUPP;
}

#pragma mark -

// ===========================================================================
//	ItemNotificationUPP
/**
	Wrapper class for a Data Browser item-notification callback function UPP */

class	ItemNotificationUPP {
public:
	explicit			ItemNotificationUPP( DataBrowserItemNotificationWithItemProcPtr inCallbackFunc );
	
						~ItemNotificationUPP();
						
	DataBrowserItemNotificationWithItemUPP
						Get() const;
						
private:								// Forbid copy and assignment
							ItemNotificationUPP( const ItemNotificationUPP& );
	ItemNotificationUPP&	operator = ( const ItemNotificationUPP& );
	
private:
	DataBrowserItemNotificationWithItemUPP	mItemNotificationUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserItemNotificationWithItemUPP
ItemNotificationUPP::Get() const
{
	return mItemNotificationUPP;
}

#pragma mark -

// ===========================================================================
//	ItemCompareUPP
/**
	Wrapper class for a Data Browser item-compare callback function UPP */

class	ItemCompareUPP {
public:
	explicit			ItemCompareUPP( DataBrowserItemCompareProcPtr inCallbackFunc );
	
						~ItemCompareUPP();
						
	DataBrowserItemCompareUPP
						Get() const;
						
private:								// Forbid copy and assignment
						ItemCompareUPP( const ItemCompareUPP& );
	ItemCompareUPP&		operator = ( const ItemCompareUPP& );

private:
	DataBrowserItemCompareUPP	mItemCompareUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserItemCompareUPP
ItemCompareUPP::Get() const
{
	return mItemCompareUPP;
}

#pragma mark -

// ===========================================================================
//	AddDragItemUPP
/**
	Wrapper class for a Data Browser add-drag-item callback function UPP */

class	AddDragItemUPP {
public:
	explicit			AddDragItemUPP( DataBrowserAddDragItemProcPtr inCallbackFunc );
	
						~AddDragItemUPP();
						
	DataBrowserAddDragItemUPP
						Get() const;
						
private:								// Forbid copy and assignment
						AddDragItemUPP( const AddDragItemUPP& );
	AddDragItemUPP&		operator = ( const AddDragItemUPP& );

private:
	DataBrowserAddDragItemUPP	mAddDragItemUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserAddDragItemUPP
AddDragItemUPP::Get() const
{
	return mAddDragItemUPP;
}

#pragma mark -

// ===========================================================================
//	AcceptDragUPP
/**
	Wrapper class for a Data Browser accept-drag callback function UPP */

class	AcceptDragUPP {
public:
	explicit			AcceptDragUPP( DataBrowserAcceptDragProcPtr inCallbackFunc );
	
						~AcceptDragUPP();
						
	DataBrowserAcceptDragUPP
						Get() const;
						
private:								// Forbid copy and assignment
						AcceptDragUPP( const AcceptDragUPP& );
	AcceptDragUPP&		operator = ( const AcceptDragUPP& );

private:
	DataBrowserAcceptDragUPP	mAcceptDragUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserAcceptDragUPP
AcceptDragUPP::Get() const
{
	return mAcceptDragUPP;
}

#pragma mark -

// ===========================================================================
//	ReceiveDragUPP
/**
	Wrapper class for a Data Browser receive-drag callback function UPP */

class	ReceiveDragUPP {
public:
	explicit			ReceiveDragUPP( DataBrowserAcceptDragProcPtr inCallbackFunc );
	
						~ReceiveDragUPP();
						
	DataBrowserReceiveDragUPP
						Get() const;
						
private:								// Forbid copy and assignment
						ReceiveDragUPP( const ReceiveDragUPP& );
	ReceiveDragUPP&		operator = ( const ReceiveDragUPP& );

private:
	DataBrowserReceiveDragUPP	mReceiveDragUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserReceiveDragUPP
ReceiveDragUPP::Get() const
{
	return mReceiveDragUPP;
}

#pragma mark -

// ===========================================================================
//	PostProcessDragUPP
/**
	Wrapper class for a Data Browser post-process-drag callback
	function UPP */

class	PostProcessDragUPP {
public:
	explicit			PostProcessDragUPP( DataBrowserPostProcessDragProcPtr inCallbackFunc );
	
						~PostProcessDragUPP();
						
	DataBrowserPostProcessDragUPP
						Get() const;
						
private:								// Forbid copy and assignment
						PostProcessDragUPP( const PostProcessDragUPP& );
	PostProcessDragUPP&		operator = ( const PostProcessDragUPP& );

private:
	DataBrowserPostProcessDragUPP	mPostProcessDragUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserPostProcessDragUPP
PostProcessDragUPP::Get() const
{
	return mPostProcessDragUPP;
}

#pragma mark -

// ===========================================================================
//	GetContextualMenuUPP
/**
	Wrapper class for a Data Browser get-contextual-menu callback function UPP */

class	GetContextualMenuUPP {
public:
	explicit			GetContextualMenuUPP( DataBrowserGetContextualMenuProcPtr inCallbackFunc );
	
						~GetContextualMenuUPP();
						
	DataBrowserGetContextualMenuUPP
						Get() const;
						
private:								// Forbid copy and assignment
						GetContextualMenuUPP( const GetContextualMenuUPP& );
	GetContextualMenuUPP&		operator = ( const GetContextualMenuUPP& );

private:
	DataBrowserGetContextualMenuUPP	mGetContextualMenuUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserGetContextualMenuUPP
GetContextualMenuUPP::Get() const
{
	return mGetContextualMenuUPP;
}

#pragma mark -

// ===========================================================================
//	SelectContextualMenuUPP
/**
	Wrapper class for a Data Browser select-contextual-menu callback
	function UPP */

class	SelectContextualMenuUPP {
public:
	explicit			SelectContextualMenuUPP( DataBrowserSelectContextualMenuProcPtr inCallbackFunc );
	
						~SelectContextualMenuUPP();
						
	DataBrowserSelectContextualMenuUPP
						Get() const;
						
private:								// Forbid copy and assignment
						SelectContextualMenuUPP( const SelectContextualMenuUPP& );
	SelectContextualMenuUPP&		operator = ( const SelectContextualMenuUPP& );

private:
	DataBrowserSelectContextualMenuUPP	mSelectContextualMenuUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserSelectContextualMenuUPP
SelectContextualMenuUPP::Get() const
{
	return mSelectContextualMenuUPP;
}

#pragma mark -

// ===========================================================================
//	ItemHelpContentUPP
/**
	Wrapper class for a Data Browser item-help-content callback
	function UPP */

class	ItemHelpContentUPP {
public:
	explicit			ItemHelpContentUPP( DataBrowserItemHelpContentProcPtr inCallbackFunc );
	
						~ItemHelpContentUPP();
						
	DataBrowserItemHelpContentUPP
						Get() const;
						
private:								// Forbid copy and assignment
						ItemHelpContentUPP( const ItemHelpContentUPP& );
	ItemHelpContentUPP&	operator = ( const ItemHelpContentUPP& );

private:
	DataBrowserItemHelpContentUPP	mItemHelpContentUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserItemHelpContentUPP
ItemHelpContentUPP::Get() const
{
	return mItemHelpContentUPP;
}

#pragma mark -

// ===========================================================================
//	DrawItemUPP
/**
	Wrapper class for a Data Browser draw-item callback function UPP */

class	DrawItemUPP {
public:
	explicit			DrawItemUPP( DataBrowserDrawItemProcPtr inCallbackFunc );
	
						~DrawItemUPP();
						
	DataBrowserDrawItemUPP
						Get() const;
						
private:								// Forbid copy and assignment
						DrawItemUPP( const DrawItemUPP& );
	DrawItemUPP&		operator = ( const DrawItemUPP& );

private:
	DataBrowserDrawItemUPP	mDrawItemUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserDrawItemUPP
DrawItemUPP::Get() const
{
	return mDrawItemUPP;
}

#pragma mark -

// ===========================================================================
//	EditItemUPP
/**
	Wrapper class for a Data Browser edit-item callback function UPP */

class	EditItemUPP {
public:
	explicit			EditItemUPP( DataBrowserEditItemProcPtr inCallbackFunc );
	
						~EditItemUPP();
						
	DataBrowserEditItemUPP
						Get() const;
						
private:								// Forbid copy and assignment
						EditItemUPP( const EditItemUPP& );
	EditItemUPP&		operator = ( const EditItemUPP& );

private:
	DataBrowserEditItemUPP	mEditItemUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserEditItemUPP
EditItemUPP::Get() const
{
	return mEditItemUPP;
}

#pragma mark -

// ===========================================================================
//	HitTestUPP
/**
	Wrapper class for a Data Browser hit-test callback function UPP */

class	HitTestUPP {
public:
	explicit			HitTestUPP( DataBrowserHitTestProcPtr inCallbackFunc );
	
						~HitTestUPP();
						
	DataBrowserHitTestUPP
						Get() const;
						
private:								// Forbid copy and assignment
						HitTestUPP( const HitTestUPP& );
	HitTestUPP&			operator = ( const HitTestUPP& );

private:
	DataBrowserHitTestUPP	mHitTestUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserHitTestUPP
HitTestUPP::Get() const
{
	return mHitTestUPP;
}

#pragma mark -

// ===========================================================================
//	TrackingUPP
/**
	Wrapper class for a Data Browser tracking callback function UPP */

class	TrackingUPP {
public:
	explicit			TrackingUPP( DataBrowserTrackingProcPtr inCallbackFunc );
	
						~TrackingUPP();
						
	DataBrowserTrackingUPP
						Get() const;
						
private:								// Forbid copy and assignment
						TrackingUPP( const TrackingUPP& );
	TrackingUPP&		operator = ( const TrackingUPP& );

private:
	DataBrowserTrackingUPP	mTrackingUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserTrackingUPP
TrackingUPP::Get() const
{
	return mTrackingUPP;
}

#pragma mark -

// ===========================================================================
//	ItemDragRgnUPP
/**
	Wrapper class for a Data Browser item-drag-rgn callback function UPP */

class	ItemDragRgnUPP {
public:
	explicit			ItemDragRgnUPP( DataBrowserItemDragRgnProcPtr inCallbackFunc );
	
						~ItemDragRgnUPP();
						
	DataBrowserItemDragRgnUPP
						Get() const;
						
private:								// Forbid copy and assignment
						ItemDragRgnUPP( const ItemDragRgnUPP& );
	ItemDragRgnUPP&		operator = ( const ItemDragRgnUPP& );

private:
	DataBrowserItemDragRgnUPP	mItemDragRgnUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserItemDragRgnUPP
ItemDragRgnUPP::Get() const
{
	return mItemDragRgnUPP;
}

#pragma mark -

// ===========================================================================
//	ItemAcceptDragUPP
/**
	Wrapper class for a Data Browser item-accept-drag callback function UPP */

class	ItemAcceptDragUPP {
public:
	explicit			ItemAcceptDragUPP( DataBrowserItemAcceptDragProcPtr inCallbackFunc );
	
						~ItemAcceptDragUPP();
						
	DataBrowserItemAcceptDragUPP
						Get() const;
						
private:								// Forbid copy and assignment
						ItemAcceptDragUPP( const ItemAcceptDragUPP& );
	ItemAcceptDragUPP&	operator = ( const ItemAcceptDragUPP& );

private:
	DataBrowserItemAcceptDragUPP	mItemAcceptDragUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserItemAcceptDragUPP
ItemAcceptDragUPP::Get() const
{
	return mItemAcceptDragUPP;
}

#pragma mark -

// ===========================================================================
//	ItemReceiveDragUPP
/**
	Wrapper class for a Data Browser item-receive-drag callback function UPP */

class	ItemReceiveDragUPP {
public:
	explicit			ItemReceiveDragUPP( DataBrowserItemReceiveDragProcPtr inCallbackFunc );
	
						~ItemReceiveDragUPP();
						
	DataBrowserItemReceiveDragUPP
						Get() const;
						
private:								// Forbid copy and assignment
						ItemReceiveDragUPP( const ItemReceiveDragUPP& );
	ItemReceiveDragUPP&	operator = ( const ItemReceiveDragUPP& );

private:
	DataBrowserItemReceiveDragUPP	mItemReceiveDragUPP;
};


// ---------------------------------------------------------------------------

inline
DataBrowserItemReceiveDragUPP
ItemReceiveDragUPP::Get() const
{
	return mItemReceiveDragUPP;
}


} // namespace SysDataBrowser

} // namespace PPx

#endif	// H_SysDataBrowser
