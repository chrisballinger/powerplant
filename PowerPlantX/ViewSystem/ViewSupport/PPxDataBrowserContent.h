// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataBrowserContent.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:47 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDataBrowserContent.h
	@brief		Classes for supplying content for a Data Browswer view
*/

#ifndef H_PPxDataBrowserContent
#define H_PPxDataBrowserContent
#pragma once

#include <PPxDataBrowserView.h>
#include <PPxPersistent.h>
#include <PPxSignature.h>

#include <Carbon/Carbon.h>

namespace PPx {

class	DataBrowserView;

namespace DataBrowser {

// ===========================================================================
//	Content

class	Content : public Persistent {
public:
						Content();
						
	virtual				~Content();

	virtual void		Install( DataBrowserView*  inBrowserView );
	
	virtual void		Remove();
	
	OSStatus			Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								DataBrowserItemDataRef	inItemDataRef,
								bool					inSetValue);

protected:
	DataBrowserView*	GetBrowserView()	{ return mBrowserView; }
	
	HIViewRef			GetSysDataBrowser()	{ return mBrowserView ? 
												mBrowserView->GetSysView() : nil; }
								
private:
	virtual	void		LoadItemCallbacks( DataBrowserCallbacks& outCallbacks );
	
	virtual OSStatus	SupplyItemData(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								DataBrowserItemDataRef	inItemDataRef) = 0;
								
	virtual OSStatus	ChangeItemData(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								DataBrowserItemDataRef	inItemDataRef);
								
private:
	DataBrowserView*	mBrowserView;
};


// ===========================================================================
//	ItemNotification

class	ItemNotification {
public:
	void				Invoke(	DataBrowserItemID			inItemID,
								DataBrowserItemNotification	inMessage,
								DataBrowserItemDataRef		inItemDataRef);

protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:

	virtual void		NotifyAboutItem(
								DataBrowserItemID			inItemID,
								DataBrowserItemNotification	inMessage,
								DataBrowserItemDataRef		inItemDataRef) = 0;
};


// ===========================================================================
//	ItemCompare

class	ItemCompare {
public:
	void				Invoke(	DataBrowserItemID		inItemIDOne,
								DataBrowserItemID		inItemIDTwo,
								DataBrowserPropertyID	inSortPropertyID,
								bool*					outResult);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:
	virtual bool		CompareItems(
								DataBrowserItemID		inItemIDOne,
								DataBrowserItemID		inItemIDTwo,
								DataBrowserPropertyID	inSortPropertyID) = 0;
};


// ===========================================================================
//	AddDragItem

class	AddDragItem {
public:
	void				Invoke(	DragRef				inDragRef,
								DataBrowserItemID	inItemID,
								ItemReference*		outItemRef,
								bool*				outItemIsInDrag);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );
	
private:
	virtual bool		AddItemToDrag(
								DragRef				inDragRef,
								DataBrowserItemID	inItemID,
								ItemReference&		outItemRef) = 0;
};


// ===========================================================================
//	AcceptDrag

class	AcceptDrag {
public:
	void				Invoke(	DragRef				inDragRef,
								DataBrowserItemID	inItemID,
								bool*				outDragIsAcceptable);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:
	virtual bool		DragIsAcceptable(
								DragRef				inDragRef,
								DataBrowserItemID	inItemID) = 0;
};


// ===========================================================================
//	ReceiveDrag

class	ReceiveDrag {
public:
	void				Invoke(	DragRef				inDragRef,
								DataBrowserItemID	inItemID,
								bool*				outDragSucceeded);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:
	virtual bool		ReceiveDraggedItems(
								DragRef				inDragRef,
								DataBrowserItemID	inItemID) = 0;
};


// ===========================================================================
//	PostProcessDrag

class	PostProcessDrag {
public:
	void				Invoke(	DragRef				inDragRef,
								OSStatus			inTrackDragResult);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:
	virtual void		PostProcessDragFromBrowser(
								DragRef				inDragRef,
								OSStatus			inTrackDragResult) = 0;
};


// ===========================================================================
//	ItemHelpContent

class	ItemHelpContent {
public:
	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								HMContentRequest		inRequest,
								HMContentProvidedType*	outProvidedType,
								HMHelpContentPtr		ioHelpContent);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:
	virtual void		SupplyHelpContent(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								HMContentProvidedType&	outProvidedType,
								HMHelpContentPtr		ioHelpContent) = 0;
								
	virtual void		DisposeHelpContent(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								HMContentProvidedType&	outProvidedType,
								HMHelpContentPtr		ioHelpContent);
};


// ===========================================================================
//	GetContextualMenu

class	GetContextualMenu {
public:
	void				Invoke(	MenuRef*			outMenu,
								UInt32*				outHelpType,
								CFStringRef*		outHelpItemString,
								AEDesc*				outSelection);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:
	virtual void		SupplyContextualMenu(
								MenuRef&			outMenu,
								UInt32&				outHelpType,
								CFStringRef&		outHelpItemString,
								AEDesc&				outSelection) = 0;
};


// ===========================================================================
//	SelectContextualMenu

class	SelectContextualMenu {
public:
	void				Invoke(	MenuRef				inMenu,
								UInt32				inSelectionType,
								SInt16				inMenuID,
								MenuItemIndex		inMenuItem);
								
protected:
	void				LoadCallback( DataBrowserCallbacks& outCallbacks );

private:
	virtual void		ProcessContextualMenuSelection(
								MenuRef				inMenu,
								UInt32				inSelectionType,
								SInt16				inMenuID,
								MenuItemIndex		inMenuItem) = 0;
};


// ===========================================================================
//	CustomContent

class	CustomContent : public Content {
public:
	virtual				~CustomContent();

	virtual void		Install( DataBrowserView* inBrowserView );
	
	virtual void		Remove();

	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								DataBrowserItemState	inItemState,
								const Rect&				inItemBounds,
								SInt16					inBitDepth,
								bool					inIsColorDevice);

private:
	virtual	void		LoadCustomCallbacks( DataBrowserCustomCallbacks& outCallbacks );
	
	void				RemoveCustomCallbacks();

	virtual void		DrawItem(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								DataBrowserItemState	inItemState,
								const Rect&				inItemBounds,
								SInt16					inBitDepth,
								bool					inIsColorDevice) = 0;
};


// ===========================================================================
//	EditItem

class	EditItem {
public:
	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								CFStringRef				inStringToEdit,
								Rect*					outMaxTextRect,
								bool*					outShrinkToFit,
								bool*					outStartEditing);
								
protected:
	void				LoadCallback( DataBrowserCustomCallbacks& outCallbacks );

private:
	virtual bool		StartEditingItem(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								CFStringRef				inStringToEdit,
								Rect&					outMaxTextRect,
								bool&					outShrinkToFit) = 0;
};


// ===========================================================================
//	HitTest

class	HitTest {
public:
	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								const Rect&				inMouseRect,
								bool*					outItemIsHit);
								
protected:
	void				LoadCallback( DataBrowserCustomCallbacks& outCallbacks );

private:
	virtual bool		ItemIsHit(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								const Rect&				inMouseRect) = 0;
};


// ===========================================================================
//	Tracking

class	Tracking {
public:
	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								Point					inStartPt,
								EventModifiers			inModifierKeys,
								DataBrowserTrackingResult*	outTrackingResult);
								
protected:
	void				LoadCallback( DataBrowserCustomCallbacks& outCallbacks );

private:
	virtual DataBrowserTrackingResult
						TrackItem(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								Point					inStartPt,
								EventModifiers			inModifierKeys) = 0;
};


// ===========================================================================
//	ItemDragRgn

class	ItemDragRgn {
public:
	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								RgnHandle				outDragRgn);
								
protected:
	void				LoadCallback( DataBrowserCustomCallbacks& outCallbacks );

private:
	virtual void		MakeItemDragRgn(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								RgnHandle				outDragRgn) = 0;
};


// ===========================================================================
//	ItemAcceptDrag

class	ItemAcceptDrag {
public:
	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								DragRef					inDragRef,
								DataBrowserDragFlags*	outDragFlags);
								
protected:
	void				LoadCallback( DataBrowserCustomCallbacks& outCallbacks );

private:
	virtual DataBrowserDragFlags
						DragIsAcceptable(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								const Rect&				inItemBounds,
								DragRef					inDragRef) = 0;
};


// ===========================================================================
//	ItemReceiveDrag

class	ItemReceiveDrag {
public:
	void				Invoke(	DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								DataBrowserDragFlags	inDragFlags,
								DragRef					inDragRef,
								bool*					outDragSuccess);
								
protected:
	void				LoadCallback( DataBrowserCustomCallbacks& outCallbacks );

private:
	virtual bool		ReceiveDrag(
								DataBrowserItemID		inItemID,
								DataBrowserPropertyID	inPropertyID,
								DataBrowserDragFlags	inDragFlags,
								DragRef					inDragRef) = 0;
};
	
	
} // namespace DataBrowser

} // namespace PPx

#endif	// H_PPxDataBrowserContent
