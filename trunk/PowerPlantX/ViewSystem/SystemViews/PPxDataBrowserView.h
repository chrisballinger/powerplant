// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataBrowserView.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDataBrowserView.h
	@brief		A system data browser view
*/

#ifndef H_PPxDataBrowserView
#define H_PPxDataBrowserView
#pragma once

#include <PPxView.h>

namespace PPx {

namespace DataBrowser {
	class Content;
}


// ===========================================================================
//	DataBrowser
/**
	A system data browser view */

class	DataBrowserView : public View {
public:
						DataBrowserView();
						
	virtual				~DataBrowserView();
	
	void				Initialize(
								View*					inSuperView,
								const HIRect&			inFrame,
								bool					inVisible,
								bool					inEnabled,
								DataBrowserViewStyle	inStyle,
								DataBrowser::Content*	inContent);
								
	DataBrowser::Content*
						ChangeContent( DataBrowser::Content* inNewContent );
								
	void				SetCallbacks( const DataBrowserCallbacks& inCallbacks );
	
	void				GetCallbacks( DataBrowserCallbacks& outCallbacks );
	
							// Data Browser Attributes
								
	DataBrowserViewStyle
						GetViewStyle() const;
	
	void				SetSortProperty( DataBrowserPropertyID inSortProperty );
	
	void				SetSortOrder( DataBrowserSortOrder inSortOrder );
	
							// Row and Column Manipulation
						
	void				SetDefaultRowHeight( UInt16 inHeight );
	
	void				SetDefaultColumnWidth( UInt16 inWidth );
	
	void				AddColumn(
								const DataBrowserListViewColumnDesc&	inColDesc,
								DataBrowserTableViewColumnIndex			inPosition);
								
	void				SetDisclosureColumn(
								DataBrowserTableViewColumnID	inColumnID,
								bool							inExpandableRows = false);
								
	void				AddItems(
								UInt32						inItemCount,
								const DataBrowserItemID*	inItemIDs,
								DataBrowserItemID			inContainer = kDataBrowserNoItem,
								DataBrowserPropertyID		inPreSortProperty = kDataBrowserItemNoProperty);
								
	void				AddOneItem(
								DataBrowserItemID		inItemID,
								DataBrowserItemID		inContainer = kDataBrowserNoItem);
								
	void				RemoveItems(
								UInt32						inItemCount,
								const DataBrowserItemID*	inItemIDs,
								DataBrowserItemID			inContainer = kDataBrowserNoItem,
								DataBrowserPropertyID		inPreSortProperty = kDataBrowserItemNoProperty);
								
	void				RemoveOneItem(
								DataBrowserItemID		inItemID,
								DataBrowserItemID		inContainer = kDataBrowserNoItem);
	
	void				RemoveAllItems();
	
	void				UpdateItems(
								UInt32						inItemCount,
								const DataBrowserItemID*	inItemIDs,
								DataBrowserPropertyID		inPropertyID,
								DataBrowserItemID			inContainer = kDataBrowserNoItem,
								DataBrowserPropertyID		inPreSortProperty = kDataBrowserItemNoProperty);
	
	void				UpdateOneItem(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inProperty = kDataBrowserNoItem,
								DataBrowserItemID		inContainer = kDataBrowserNoItem);
								
							// Setting item data

	void				SetItemDataText(
								DataBrowserItemDataRef	inItemDataRef,
								CFStringRef				inText);
								
	void				SetItemDataIcon(
								DataBrowserItemDataRef	inItemDataRef,
								IconRef					inIcon);
								
	void				SetItemDataValue(
								DataBrowserItemDataRef	inItemDataRef,
								SInt32					inValue);
								
	void				SetItemDataMinimum(
								DataBrowserItemDataRef	inItemDataRef,
								SInt32					inMinimum);
								
	void				SetItemDataMaximum(
								DataBrowserItemDataRef	inItemDataRef,
								SInt32					inMaximum);
								
	void				SetItemDataBoolean(
								DataBrowserItemDataRef	inItemDataRef,
								bool					inBoolean);
								
							// Getting item data
							
	CFString			GetItemDataText(
								DataBrowserItemDataRef	inItemDataRef) const;
								
	SInt32				GetItemDataValue( DataBrowserItemDataRef inItemDataRef ) const;
								
	bool				GetItemDataBoolean( DataBrowserItemDataRef inItemDataRef ) const;
								
							// Selection	
								
	void				SetSelectionFlags( DataBrowserSelectionFlags inFlags );
	
	void				SetSelectedItems(
								UInt32						inItemCount,
								const DataBrowserItemID*	inItemIDs,
								DataBrowserSetOption		inOption);
								
	void				SetHiliteStyle( DataBrowserTableViewHiliteStyle inStyle );
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	std::auto_ptr<DataBrowser::Content>	mContent;
};


} // namespace PPx

#endif	// H_PPxDataBrowserView
