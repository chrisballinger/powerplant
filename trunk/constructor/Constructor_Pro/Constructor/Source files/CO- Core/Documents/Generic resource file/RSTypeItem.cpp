// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSTypeItem.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:33:03 $
//  $History: RSTypeItem.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/21/96   Time: 15:19
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Added an override to AddSubItem which fixes the "changing a resource ID
//	doesn't cause it to be resorted" problem. (Bug fix #1078.)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:03
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSTypeItem.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Documents : Generic resource file
#include "RSComparators.h"
#include "RSOutlineTable.h"
#include "RSResource.h"
#include "RSResourceItem.h"

	// Core : Editors : Generic resource editor
#include "RETypeEntry.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_RSTypeDisplayStrings	= 15105;
const SInt16	str_SpaceItem				= 1;
const SInt16	str_SpaceItems				= 2;


// ===========================================================================

#pragma mark *** RSTypeItem ***

// ---------------------------------------------------------------------------
//		* RSTypeItem()
// ---------------------------------------------------------------------------
//	Constructor

RSTypeItem::RSTypeItem()
{
	mRSType = nil;
	mTypeEntry = nil;
	mExpanded = false;
}


// ---------------------------------------------------------------------------
//		* ~RSTypeItem
// ---------------------------------------------------------------------------
//	Destructor

RSTypeItem::~RSTypeItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** disclosure triangle

// ---------------------------------------------------------------------------
//		* CanExpand
// ---------------------------------------------------------------------------
//	Overriden to expand even if the data model is collapsed.

Boolean
RSTypeItem::CanExpand() const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_((const_cast<RSTypeItem*>(this))->mRSType);

	// Return true if there are resources in this type item.

	return mRSType->CountResources() > 0;

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing setup

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Draw resource type string in column 1, number of resources in col 2.

void
RSTypeItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();

	switch (inCell.col) {
	
		// Column 1: Resource type folder icon and type name.
	
		case 1: {
			ValidateObject_(mTypeEntry);
			ioDrawContents.outShowSelection = true;
			ioDrawContents.outHasIcon = true;
			ioDrawContents.outIconSuite = mTypeEntry->GetResFolderIconSuite();
			ioDrawContents.outTextTraits.style |= bold;

			LStr255 folderTitle;
			mTypeEntry->GetResFolderName(folderTitle);
			LString::CopyPStr(folderTitle, ioDrawContents.outTextString);
			break;
		}
		
		// Column 2: Number of items in folder.
		
		case 2: {
			ioDrawContents.outTextTraits.style |= underline;
			ioDrawContents.ioCellBounds.right -= 3;
			ioDrawContents.outTextTraits.justification = teJustRight;

			SInt32 numItems = mRSType->CountResources();
			LStr255 numItemsStr(numItems);
			
			numItemsStr += LStr255(STR_RSTypeDisplayStrings,
								numItems == 1 ? str_SpaceItem : str_SpaceItems);

			LString::CopyPStr(numItemsStr, ioDrawContents.outTextString);
			break;
		}
	}
}


// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	Give this row a background shading.

void
RSTypeItem::DrawRowAdornments(
	const Rect&		inLocalRowRect)
{

	// Validate pointers.

	ValidateThis_();
	
	// Shade row in a light gray color.
	
	ShadeRow(UGAColorRamp::GetColor(2), inLocalRowRect);

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	If a resource ID changes, we need to relocate it in the list.

void
RSTypeItem::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(mRSType);
	ValidateObject_(inMessage->mAttribute);
	ValidateObject_(inMessage->mObject);

	// Let inherited handle it first.
	
	OVItem::ValueChanged(inMessage);
	
	// See if it's the attribute we're sorting by.
	
	if (mRSType->GetResIDsSignificant()) {
		if (inMessage->mAttribute->GetKey() != RSResource::attrKey_ResourceID)
			return;
	}
	else {
		if (inMessage->mAttribute->GetKey() != RSResource::attrKey_ResourceName)
			return;
	}
	
	// It is. Find the element, tear it down, and rebuild it.

	OVItem* item = FindItemForSubElement(inMessage->mObject, false);
	if (item != nil) {
		ValidateObject_(item);
		mOVTable->RemoveItem(item);

		if (mSubElements != nil) {
			mSubElements->InvalidateSort();
			mSubElements->Sort();
		}

		CreateItemForElement(inMessage->mObject, true, true);
	}
}


// ---------------------------------------------------------------------------
//		* ContainerListChanged									[protected]
// ---------------------------------------------------------------------------
//	If a resource is added or removed, we need to make sure the resource
//	count remains accurate.

void
RSTypeItem::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(inMessage->mContainer);

	// Make sure items get added or removed as needed.

	OVListener::ContainerListChanged(inMessage);
	
	// If this is the resource list, we need to update the resources count display.

	if (inMessage->mContainer == mRSType->GetResourceList()) {
		STableCell cell(FindRowForItem(), 1);
		mOVTable->RefreshCell(cell);
		cell.col++;
		mOVTable->RefreshCell(cell);
	}
}


// ---------------------------------------------------------------------------
//		* AddSubItem											[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that we get change messages from the resources.

void
RSTypeItem::AddSubItem(
	LOutlineItem*	inSubItem,
	LOutlineItem*	inAfterItem)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSubItem);

	// Pass along to inherited.

	OVItem::AddSubItem(inSubItem, inAfterItem);

	// Make sure we listen to changes from this item.
	
	RSResourceItem* item = (dynamic_cast<RSResourceItem*>(inSubItem));
	ValidateObject_(item);
	
	DMElement* element = item->GetElement();
	ValidateObject_(element);
	
	element->AddListener(this);

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource list

// ---------------------------------------------------------------------------
//		* ExpandSelf											[protected]
// ---------------------------------------------------------------------------
//	Make sure the resource type's data model is expanded.

void
RSTypeItem::ExpandSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRSType);

	// Expand the resource data model first.

	{
		StValueChanger<Boolean> noUpdate(mExpanded, false);
		mRSType->ExpandResourceList();
	}
	
	// Then create items for individual resources.
	
	OVItem::ExpandSelf();

}


// ---------------------------------------------------------------------------
//		* CollapseSelf											[protected]
// ---------------------------------------------------------------------------
//	Release the resource type's data model.

void
RSTypeItem::CollapseSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRSType);
	
	// Tell the data model to collapse its resource list.
	// This will help memory use.
	
	mRSType->CollapseResourceList();

}


// ---------------------------------------------------------------------------
//		* DoubleClick											[protected]
// ---------------------------------------------------------------------------
//	Overriden to expand or collapse the type's disclosure triangle.

void	
RSTypeItem::DoubleClick(
	const STableCell&			/* inCell */,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{

	// Validate pointers.

	ValidateThis_();
	
	// Expand or collapse the disclosure triangle.
	
	if (IsExpanded())
		Collapse();
	else
		Expand();

	// Reset click count so a triple-click doesn't immediately close.
	
	RSOutlineTable::sClickCount = 0;

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure we got a PRType.

void
RSTypeItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a RSType.
	
	mRSType = (dynamic_cast<RSType*>(inElement));
	ValidateObject_(mRSType);

	// Okay, it's cool. Find the resource type entry for this type.
	
	mTypeEntry = mRSType->GetTypeEntry();
	ValidateObject_(mTypeEntry);

	// Set the sort comparator for submodels.
	// This depends on the "resource IDs" significant flag...

	if (mRSType->GetResIDsSignificant())
		SetSubItemComparator(new RSResIDComparator);
	else
		SetSubItemComparator(new RSResNameComparator);

	// Specify container for submodels.

	SetContainer(mRSType->GetResourceList());

	// If type is already expanded, we want to be expanded.
	
	if (mRSType->IsExpanded())
		Expand();

}
