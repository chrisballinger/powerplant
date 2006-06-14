// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVListener.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 05/10/96
//     $Date: 2006/01/18 01:32:43 $
//	$History: OVListener.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/28/97   Time: 19:59
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Moved multi-line text support to LOutlineItem.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/19/96   Time: 18:42
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added a fix that prevented disclosure triangle icons from being loaded
//	from user documents. Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 19:13
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added pointer validation in GoingAway.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:44
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "OVListener.h"

	// Core : Data model : Utility classes
#include "DMIterator.h"

	// Core : Display classes (Eric) : Outline table
#include "OVItemFactory.h"
#include "OVItem.h"
#include "OVTable.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>

	// PowerPlant : CA : Outline table
#include <LOutlineItem.h>
#include <LOutlineTable.h>


// ===========================================================================

#pragma mark *** OVListener ***

// ---------------------------------------------------------------------------
//		* OVListener(OVItem*)
// ---------------------------------------------------------------------------
//	Constructor, specifying the outline item (if any) that we're mixed in with

OVListener::OVListener(
	OVItem*		inThisAsItem)
{

	// Validate pointers.

	if (inThisAsItem != nil)
		ValidateObject_(inThisAsItem);

	// Reset pointers to outline table and item.
	
	mOVItem = inThisAsItem;
	mOVTable = nil;
	mSubElements = nil;

}


// ---------------------------------------------------------------------------
//		* ~OVListener
// ---------------------------------------------------------------------------
//	Destructor

OVListener::~OVListener()
{

	// Delete the subelements array.

	if (mSubElements != nil) {
		ValidateObject_(mSubElements);
		delete mSubElements;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** display accessors

// ---------------------------------------------------------------------------
//		* SetOVTable
// ---------------------------------------------------------------------------

void
OVListener::SetOVTable(
	OVTable*	inTable)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inTable);
	
	// Record pointer to table.

	mOVTable = inTable;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** element accessors

// ---------------------------------------------------------------------------
//		* SetElement
// ---------------------------------------------------------------------------
//	Set the element that this view will be displaying. Should be called only
//	once when the view is being initialized.

void
OVListener::SetElement(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// Sanity checks: Make sure we have a valid element.
	// Also disallow changing the element once it has been set.

	if (mElement != nil) {
		SignalCStr_("OVListener::SetElement - mElement != nil");
		return;
	}
	
	// Okay, it's legal. Change the element.

	mElement = inElement;
	mElement->AddListener(this);

	// Call override hook for new element.
	
	SetElementSelf(inElement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** container accessors

// ---------------------------------------------------------------------------
//		* SetContainer
// ---------------------------------------------------------------------------
//	Set container for this item. Typically this is the container attribute
//	that holds the subelements we want to display.

void
OVListener::SetContainer(
	DMElement*	inContainer)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inContainer);
	
	// Save a reference to this container.

	mContainer = inContainer;
	mContainer->AddListener(this);
	
	// Create display items for subelements unless this item is collapsed.
	
	Boolean createSubs = true;
	
	if (mOVItem != nil) {
		ValidateObject_(mOVItem);
		if (!mOVItem->IsExpanded())
			createSubs = false;
	}
	
	if (createSubs)
		CreateItemsForSubElements();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** subelement accessors

// ---------------------------------------------------------------------------
//		* FindItemForSubElement
// ---------------------------------------------------------------------------
//	Find the display item (if there is one) for a subelement of this element.
//	This search can be recursive (slow, but complete) or non-recursive (fast).

OVItem*
OVListener::FindItemForSubElement(
	const DMElement*	inElement,
	Boolean				inRecursive) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Shortcut out if no subitems.

	const LArray* subItems = GetSubElementItems();
	if (subItems == nil)
		return nil;
	if (subItems->GetCount() == 0)
		return nil;	

	// Do a breadth-first search.
	// This is an optimization since most requests will be for immediate submembers.
	
	LFastArrayIterator iter(*subItems);
	LOutlineItem* item;
	
	while (iter.Next(&item)) {
	
		ValidateObject_(item);
	
		OVItem* itemOV = (dynamic_cast<OVItem*>(item));
		ValidateObject_(itemOV);
	
		if (itemOV->GetElement() == inElement)
			return itemOV;

	}
	
	// Nothing at immediate sublevel, search deeper.

	if (inRecursive) {
		iter.ResetTo(LArrayIterator::from_Start);

		while (iter.Next(&item)) {
		
			ValidateObject_(item);

			OVItem* itemOV = (dynamic_cast<OVItem*>(item));
			ValidateObject_(itemOV);

			OVItem* subItem = itemOV->FindItemForSubElement(inElement, true);
			if (subItem != nil) {
				ValidateObject_(subItem);
				return subItem;
			}

		}
	}
	
	// No display for this element at any level.
	
	return nil;

}


// ---------------------------------------------------------------------------
//		* SetSubItemComparator
// ---------------------------------------------------------------------------
//	Set the comparator object that will be used to sort the subelements of
//	this element. If there are existing subelement displays, they are torn
//	down and rebuilt according to the new sequence.

void
OVListener::SetSubItemComparator(
	LComparator*	inComparator)
{

	// Validate pointers.

	ValidateThis_();

	// If we have been sorting, tear down the sort array.
	
	if (mSubElements != nil) {
		ValidateObject_(mSubElements);
		delete mSubElements;
	}
	mSubElements = nil;
	
	// If we are now sorting, create a new sort array.
	// A nil comparator means use the data model sequence.
	
	if (inComparator != nil) {
		ValidateObject_(inComparator);
		mSubElements = new LSharableArray(inComparator, true);
		ValidateObject_(mSubElements);
	}

	// See if the subitems array exists. If so, tear down and rebuild display items.
	// Shortcut out of this if no items exist.

	const LArray* subItems = GetSubElementItems();
	if (subItems != nil) {
	
		if (subItems->GetCount() > 0) {

			ValidateObject_(mOVTable);

			LFastArrayIterator iter(*subItems);
			LOutlineItem* item;

			while (iter.Next(&item)) {
				ValidateObject_(item);
				mOVTable->RemoveItem(item, false);
			}

			CreateItemsForSubElements();
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag receive behaviors

// ---------------------------------------------------------------------------
//		* ChooseDragPlacementAsSub
// ---------------------------------------------------------------------------
//	Find an appropriate home for this drag item, given the current mouse location.

Boolean
OVListener::ChooseDragPlacementAsSub(
	DragReference			inDragRef,
	ItemReference			inItemRef,
	const DMElement*		inElement,
	DM_DragPlacement&		ioPlacement,
	const Point&			inDragMouseLocal)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(inElement);

	// Don't accept placement as subitem if mouse is to the left
	// of our display. (This test may not apply if we're the left
	// most item in the display.)
	
	Boolean okayAsSub = true;
	if (mOVItem != nil) {
	
		ValidateObject_(mOVItem);
		if (inDragMouseLocal.h < mOVItem->GetLeftEdge())
			okayAsSub = false;
		if (mOVItem->GetSuperItem() == nil)
			okayAsSub = true;

	}

	// If allowed, test for placement as subitem.
	
	if (okayAsSub) {

		SInt32 index = 1;
	
		// Scan subitems list if there is one.
	
		const LArray* subItems = GetSubElementItems();
		if (subItems != nil) {
			
			// Find the insertion index that most closely approximates
			// the current mouse location.
			
			LFastArrayIterator iter(*subItems);
			LOutlineItem* item;
	
			while (iter.Next(&item)) {
			
				ValidateObject_(item);
	
				Rect cellFrame;
				STableCell cell(item->FindRowForItem(), 1);
				if (!mOVTable->GetLocalCellRect(cell, cellFrame)) {
					index++;
					continue;
				}
				
				SInt16 midVert = (cellFrame.top + cellFrame.bottom) >> 1;
				if (inDragMouseLocal.v < midVert)
					break;
				
				index++;
			}
		}
			
		// Okay, we've got an index. Now ask the element if this location is OK.
		
		DM_DragPlacement tempPlace;
		tempPlace.mHostElement = mContainer;
		tempPlace.mInsertionIndex = index;
		tempPlace.ioInsertAsDuplicate = ioPlacement.ioInsertAsDuplicate;
		
		if (mContainer != nil) {
			ValidateObject_(mContainer.GetObject());
			if (mContainer->CanHostElement(inElement, &tempPlace)) {
				ioPlacement = tempPlace;
				return true;
			}
		}
	
		if (mElement != nil) {
			ValidateObject_(mElement.GetObject());
			tempPlace.mHostElement = mElement;
			if (mElement->CanHostElement(inElement, &tempPlace)) {
				ioPlacement = tempPlace;
				return true;
			}
		}
	}
	
	// Didn't find an acceptable placement here, try super item.
	
	OVItem* superItem = nil;
	if (mOVItem != nil)
		superItem = (dynamic_cast<OVItem*>(mOVItem->GetSuperItem()));

	if (superItem != nil) {
		ValidateObject_(superItem);
		return superItem->ChooseDragPlacementAsSub(inDragRef, inItemRef,
										inElement, ioPlacement, inDragMouseLocal);
	}
	if (mOVTable != this) {
		return mOVTable->ChooseDragPlacementAsSub(inDragRef, inItemRef,
										inElement, ioPlacement, inDragMouseLocal);
	}
	return false;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	The display element has been changed. Override hook.

void
OVListener::SetElementSelf(
	DMElement*	/* inElement */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* GoingAway												[protected]
// ---------------------------------------------------------------------------
//	Notification that a data model element is going away. If it's ours,
//	ask the table to remove this item.

void
OVListener::GoingAway(
	DMElement*	inElement)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	ValidateObject_(mElement.GetObject());
	ValidateObject_(mOVTable);

	// First check to see if the element is the one we're displaying.
	// If not, ignore the message.

	if (inElement != mElement)
		return;
	
	// This element is going away, must remove this view.
	
	if (mOVItem != nil) {
		ValidateObject_(mOVItem);
		mOVTable->RemoveItem(mOVItem, true);
	}
	else {

		// This is the root of the outline. If the root element is going away,
		// we remove the entire window.
		
		LView* superView = mOVTable;
		while (1) {
			ValidateObject_(superView);
			if (superView->GetSuperView())
				superView = superView->GetSuperView();
			else
				break;
		}
		delete superView;
	}

}


// ---------------------------------------------------------------------------
//		* ContainerListChanged									[protected]
// ---------------------------------------------------------------------------
//	Notification that a data model object was added or removed from a
//	container list. If the list is ours, update the display accordingly.

void
OVListener::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();

	// See if this container is the one that's adding the node.
	// If not, ignore the message.

	if ((mContainer == nil) || (inMessage->mContainer != mContainer))
		return;

	// Make sure this node is expanded. If not, we can ignore the message.

	if (mOVItem != nil) {
		ValidateObject_(mOVItem);
		if (!(mOVItem->IsExpanded()))
			return;
	}

	ValidateObject_(inMessage->mMovedElement);

	// Search for an existing display for this element.

	OVItem* itemDisplay = FindItemForSubElement(inMessage->mMovedElement, true);

	// Are we adding or removing this element?

	if (inMessage->mElementAdded) {
	
		// Adding it. Create a display if one doesn't already exist.
	
		if (itemDisplay == nil)
			CreateItemForElement(inMessage->mMovedElement);
		
	}
	else {
	
		// Removing it. Remove the display if it exists.

		if ((mOVTable != nil) && (itemDisplay != nil)) {
			ValidateObject_(mOVTable);
			mOVTable->RemoveItem(itemDisplay, true);
		}
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** subitem creation

// ---------------------------------------------------------------------------
//		* CreateItemsForSubElements								[protected]
// ---------------------------------------------------------------------------
//	Create a display item for each subelement (if possible).

void
OVListener::CreateItemsForSubElements()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// If no container attribute, there are no subelements.
	
	if (mContainer == nil)
		return;
	
	// Look for subelement array in container.

	DMContainerMixin* container = (dynamic_cast<DMContainerMixin*>(mContainer.GetObject()));
	ValidateObject_(container);
	
	// Iterate over subelements. Crate an outline item for each.

	DMFastIterator iter(container->GetSubElements());
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		CreateItemForElement(objDM, false, false);
	}
	
	// Redraw the table accordingly.

	mOVTable->AdjustImageSize(false);
	mOVTable->Refresh();

}


// ---------------------------------------------------------------------------
//		* CreateItemForElement									[protected]
// ---------------------------------------------------------------------------
//	Create and place a display item for a single new element. Creates the object,
//	configures it, and installs it in the appropriate place in the table.

void
OVListener::CreateItemForElement(
	DMElement*	inElement,
	Boolean		inRefresh,
	Boolean		inAdjustImageSize)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	ValidateObject_(mOVTable);

	// Sanity check #2: Make sure the item display doesn't already exist.
	
	if (FindItemForSubElement(inElement, false) != nil)
		return;

	// Create the specific subclass of OVItem for this element.

	OVItem* newItem = CreateItemForElementSelf(inElement);
	if (newItem == nil)
		return;
	
	ValidateObject_(newItem);
	
	// Configure this view for the element it will display.
	
	newItem->SetOVTable(mOVTable);
	newItem->SetElement(inElement);
	newItem->SetItemFactory(mItemFactory);

	// Place it in the outline at an appropriate location.

	LOutlineItem* afterItem = nil;
	LOutlineItem* superItem = nil;
	ChoosePlacementForNewItem(inElement, superItem, afterItem);
	mOVTable->InsertItem(newItem, superItem, afterItem, inRefresh, inAdjustImageSize);
	
	// If it wants to be expanded, do so now.
	
	if (newItem->IsExpanded()) {
		newItem->Collapse();
		newItem->Expand();
	}
}


// ---------------------------------------------------------------------------
//		* CreateItemForElementSelf								[protected]
// ---------------------------------------------------------------------------
//	Create the actualy display element for a new data model element.
//	No configuration is involved. By default, defers to the item factory.

OVItem*
OVListener::CreateItemForElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Use item factory to create outline item.

	if (mItemFactory != nil) {
	
		// Make sure application's resource fork is on top.
		
		StApplicationContext appContext;
		
		// Make sure we have a valid item factory before calling it.
	
		ValidateObject_(mItemFactory.GetObject());
		return mItemFactory->CreateItemForElement(inElement);

	}
	else
		return nil;

}


// ---------------------------------------------------------------------------
//		* ChoosePlacementForNewItem								[protected]
// ---------------------------------------------------------------------------
//	A new outline item has been created. Choose the appropriate location in
//	the table for this item.

void
OVListener::ChoosePlacementForNewItem(
	DMElement*			inNewElement,
	LOutlineItem*&		outSuperItem,
	LOutlineItem*&		outAfterItem)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);

	// By default, just place it as the first child of this item.

	outSuperItem = mOVItem;
	outAfterItem = nil;
	
	// Shortcut if no existing subitems.
	
	const LArray* subItems = GetSubElementItems();
	if (subItems == nil)
		return;
	if (subItems->GetCount() == 0)
		return;
	
	// If items are being sorted, use the sort comparator to determine placement.
	
	if (mSubElements != nil) {
	
		ValidateObject_(mSubElements);

		// See where comparator wants to put this item.
		// If it will be the first item, we can take a shortcut.

		ArrayIndexT index = mSubElements->FetchInsertIndexOf(&inNewElement);
		if (index < 2)
			return;				// shortcut: insert as first item

		// Not first item, find the first item before this one that has a display.
		
		DMFastIterator iter(*mSubElements, index);
		while (iter.PreviousElement()) {

			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			
			LOutlineItem* item = FindItemForSubElement(objDM, false);
			if (item != nil) {
				outAfterItem = item;
				return;
			}
		}

	}
	else {
	
		// No special sort is being applied, just put this item in the same
		// position as it appears in the container's list.
	
		// If our element is a container (which it should be if this method is called),
		// scan the container's subelement list and try to arrive at a better placement.
	
		ValidateObject_(mContainer.GetObject());
		
		DMContainerMixin* container = (dynamic_cast<DMContainerMixin*>(mContainer.GetObject()));
		ValidateObject_(container);
	
		// LOutlineTable wants to know which existing item the new item
		// should go AFTER, so we start from just the existing item and work
		// back to the top of the list. As soon as we find an element that has an
		// existing item display, we stop.

		DMFastIterator iter(container->GetSubElements(),
							container->GetSubElements().FetchIndexOf(&inNewElement));

		while (iter.PreviousElement()) {

			// If there's a view for an container that's before the new
			// container in the hierarchy, be sure new view goes after it.
			
			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			
			OVItem* item = FindItemForSubElement(objDM, false);
			if (item != nil) {
				outAfterItem = item;
				return;
			}
		}
	}
}
