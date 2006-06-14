// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMContainerMixin.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/26/96
//	   $Date: 2006/01/18 01:32:34 $
//	$History: DMContainerMixin.cpp $
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 14  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 01/29/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed a dangling pointer reference in RemoveElement. (Bug fix #1236.)
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 01/07/97   Time: 17:48
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Rolled in an bug fix from Clint 01/05/97.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 12/08/96   Time: 17:37
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Changed behavior of FinishCreate so that subelements are initialized
//	before the parent.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 12/08/96   Time: 11:33
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Fixed a bug in CopySubElementsFrom. It no longer ignores subelements if
//	they are shared (i.e. superelement != this).
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:21
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/05/96   Time: 18:50
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Changed RemoveElement to send the list-changed message (and thus the
//	going-away message) before actually removing the element from the
//	display.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/01/96   Time: 01:51
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added CopySubElementsFrom method. Changed copy constructor to use it.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:08
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added low-memory handling to copy constructor.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/24/96   Time: 18:52
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Fixed a bug in SendListChangeMessage. It was sending the going-away
//	message for deleted elements that weren't really subelements.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/14/96   Time: 16:09
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added accessors for mIsSubObjectList. Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:46
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Replaced ItemControl and CanMakeChanges with Enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMContainerMixin.h"

	// Core : Data model : Attribute classes
#include "DMAttribute.h"

	// Core : Data model : Core objects
#include "DMElement.h"

	// Core : Data model : Undo/redo
#include "DMListSubAction.h"
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Utilities
#include "StKeepSharableAlive.h"


// ===========================================================================

#pragma mark *** DMContainerMixin ***

// ---------------------------------------------------------------------------
//		* DMContainerMixin(DMElement&)							[protected]
// ---------------------------------------------------------------------------
//	Constructor. Specify the DMElement which that this object
//	is mixed in with.

DMContainerMixin::DMContainerMixin(
	DMElement&	inThisAsElement)

: mThisAsElement(inThisAsElement)

{

	// Validate pointers.

	ValidateObject_(&mThisAsElement);

	// Set default value for sub-object list flag.

	mIsSubObjectList = false;

}


// ---------------------------------------------------------------------------
//		* DMContainerMixin(DMElement&, DMContainerMixin&)		[protected]
// ---------------------------------------------------------------------------
//	Copy constructor. Clones all of the items in the
//	original's subobject list (if so requested).

DMContainerMixin::DMContainerMixin(
	DMElement&				inThisAsElement,
	const DMContainerMixin&	inOriginal)

: mThisAsElement(inThisAsElement)

{

	// Validate pointers.

	ValidateObject_(&inOriginal);
	ValidateObject_(&mThisAsElement);

	// Copy subobject list flag.

	mIsSubObjectList = inOriginal.mIsSubObjectList;
	
	// Clone all objects in subobject list, but not until
	// we've been fully constructed.
	
	mOriginal = (dynamic_cast<DMElement*>((const_cast<DMContainerMixin*>(&inOriginal))));
	ValidateObject_(mOriginal.GetObject());

}


// ---------------------------------------------------------------------------
//		* ~DMContainerMixin										[protected]
// ---------------------------------------------------------------------------
//	Destructor

DMContainerMixin::~DMContainerMixin()
{

	// Iterate through subelements, informing them that
	// this object is being deleted. They should remove
	// the superelement reference.

	DMIterator iter(mSubElements);
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		objDM->SuperDeleted(&mThisAsElement);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** items list

// ---------------------------------------------------------------------------
//		* InsertElementAt
// ---------------------------------------------------------------------------
//	Public method to insert an element at a specified location in the
//	subelement list. Typically called by DMReanimator or DMListSubAction.

void
DMContainerMixin::InsertElementAt(
	ArrayIndexT		inAtIndex,
	DMElement*		inElement,
	Boolean			inBeSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// If there is a transaction active, make sure it knows about the list change.

	StDMTransactionBuilder* builder = StDMTransactionBuilder::GetSubActionHost();
	if (builder != nil) {
		builder->PostSubAction(new DMListSubAction(
					this, &mThisAsElement, inElement, inBeSuperElement, inAtIndex));

		mThisAsElement.MakeModified();
		if (inBeSuperElement)
			inElement->MakeModified();
	}

	// Make the change.

	mSubElements.InsertItemAt(inAtIndex, inElement);
	if (inBeSuperElement) {
		inElement->SetSuperElement(&mThisAsElement);
		inElement->SetDeleted(false);
	}
	
	// Tell the world about it.
	
	SendListChangeMessage(inElement, true);

}


// ---------------------------------------------------------------------------
//		* RemoveElement
// ---------------------------------------------------------------------------
//	Public method to remove an element from the subelements list.
//	Typically called by DMReanimator or DMListSubAction.

void
DMContainerMixin::RemoveElement(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);
	ValidateObject_(inElement);

	// Keep a reference to this element so we don't delete it out from
	// under ourselves. That could be fatal.
	
	DMElementPtr deletedElement;
	deletedElement = inElement;

	// If there is a transaction active, make sure it knows about the list change.

	StDMTransactionBuilder* builder = StDMTransactionBuilder::GetSubActionHost();
	if (builder != nil) {
		builder->PostSubAction(new DMListSubAction(this, &mThisAsElement, deletedElement,
							deletedElement->GetSuperElement() == &mThisAsElement, 0));
		mThisAsElement.MakeModified();
	}

	// Tell the world about it.
	
	SendListChangeMessage(deletedElement, false);

	// Make the change.

	mSubElements.RemoveItem(deletedElement);
	if (deletedElement->GetSuperElement() == &mThisAsElement) {
		if (builder != nil)
			deletedElement->SetDeleted(true);
		deletedElement->SetSuperElement(nil);
	}
}


// ---------------------------------------------------------------------------
//		* CopySubElementsFrom
// ---------------------------------------------------------------------------
//	Copy subelement list from another container.

void
DMContainerMixin::CopySubElementsFrom(
	const DMElement*	inOriginal,
	Boolean				inCloneSubObjects)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);
	ValidateObject_(inOriginal);

	// Convert original element to container.
	
	const DMContainerMixin* originalContainer = dynamic_const_cast(inOriginal, DMElement*, DMContainerMixin*);
	ValidateObject_(originalContainer);
	
	// Twiddle the use count field so neither this object, nor
	// the original gets zapped by InsertItemAt.

	StKeepSharableAlive keepThis(&mThisAsElement);
	StKeepSharableAlive keepOrig(const_cast<DMElement*>(inOriginal));

	// Clone all objects in subobject list.

	DMFastIterator iter(originalContainer->mSubElements);
	while (iter.NextElement()) {
	
		// Find the next element in the original's list.

		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		
		// Clone it (if so required).
		
		DMElementPtr objClone;
		if (inCloneSubObjects) {
			objClone = objDM->Clone();
			objClone->SetSuperElement(&mThisAsElement);
		}
		else
			objClone = objDM;

		ValidateObject_(objClone.GetObject());

		// Add it to our subelement list.

		mSubElements.InsertItemAt(LArray::index_Last, objClone);
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamData										[protected]
// ---------------------------------------------------------------------------
//	Ask all the attributes in this container to read their values from
//	the specified stream.

void
DMContainerMixin::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{

	// Validate pointers.

	ValidateThis_();
	
	// Iterate through subelements. Allow each to read data from this stream.

	DMFastIterator iter(mSubElements);
	while (iter.NextElement()) {

		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);

		DMAttribute* attr = (dynamic_cast<DMAttribute*>(objDM));
		if (attr != nil) {
			ValidateObject_(attr);
			attr->ReadStreamData(inStream, inStreamSelector);
		}
	}
}


// ---------------------------------------------------------------------------
//		* WriteStreamData										[protected]
// ---------------------------------------------------------------------------
//	Ask all the attributes in this container to write their values to
//	the specified stream.

void
DMContainerMixin::WriteStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);
	
	// Iterate through subelements, allowing them to write stream data.
	
	DMFastIterator iter(mSubElements);
	while (iter.NextElement()) {

		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		
		DMAttribute* attr = (dynamic_cast<DMAttribute*>(objDM));
		if (attr != nil) {
			ValidateObject_(attr);
			attr->WriteStreamData(inStream, inStreamSelector);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications/requests

// ---------------------------------------------------------------------------
//		* FinishCreate											[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::FinishCreate.

void
DMContainerMixin::FinishCreate()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Prevent object from being disposed during the method.

	StKeepSharableAlive lockObj(&mThisAsElement);
	
	// Clone objects from original list.
	
	if (mOriginal != nil) {
		ValidateObject_(mOriginal.GetObject());
		
		DMElementPtr original = mOriginal;
		mOriginal = nil;

		CopySubElementsFrom(original, true);
	}
	else {
	
		// Only do this next block in an "else," because Clone
		// calls FinishCreate for subelements in the above block
		// [CCP: 1/5/97].

		// We were created from stream, so 
		// now iterate through subelements and let them finish
		// their initialization.

		DMIterator iter(mSubElements);
		while (iter.NextElement()) {
			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			objDM->FinishCreate();
		}
	}
	
	// Allow this element to do post-initialization.

	mThisAsElement.DMElement::FinishCreate();

}


// ---------------------------------------------------------------------------
//		* GoingAway												[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::GoingAway.

void
DMContainerMixin::GoingAway()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Prevent object from being disposed during the method.

	StSharer lockObj(&mThisAsElement);
	
	// Tell this element's listeners that we're going away.

	mThisAsElement.DMElement::GoingAway();

	// Tell subelements that we're going away.

	DMIterator iter(mSubElements);
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		objDM->SuperGoingAway(&mThisAsElement);
	}
}


// ---------------------------------------------------------------------------
//		* CollectChanges										[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::CollectChanges.

void
DMContainerMixin::CollectChanges()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Prevent object from being disposed during the method.

	StSharer lockObj(&mThisAsElement);		// so we don't go away during method

	// Let this element collect changes.

	mThisAsElement.DMElement::CollectChanges();

	// Now ask all subelements to collect changes.

	DMFastIterator iter(mSubElements);
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		objDM->SuperCollectChanges(&mThisAsElement);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* FindAttributeByKey									[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::FindAttributeByKey.

DMAttribute*
DMContainerMixin::FindAttributeByKey(
	FourCharCode	inKey) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Prevent object from being disposed during the method.

	StSharer lockObj(&mThisAsElement);
	
	// Iterate through subelements. If they are attributes,
	// ask them if they match this key.
	
	if (!mIsSubObjectList) {
		DMFastIterator iter(mSubElements);
		while (iter.NextElement()) {

			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);

			DMAttribute* attr = objDM->FindAttributeByKey(inKey);
			if (attr != nil) {
				ValidateObject_(attr);
				return attr;
			}
		}
	}

	// Didn't find it.

	return nil;

}


// ---------------------------------------------------------------------------
//		* FindAttributeByTitle									[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::FindAttributeByTitle.

DMAttribute*
DMContainerMixin::FindAttributeByTitle(
	ConstStringPtr	inTitle) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Prevent object from being disposed during the method.

	StSharer lockObj(&mThisAsElement);
	
	// Iterate through subelements. If they are attributes,
	// ask them if they match this title.
	
	if (!mIsSubObjectList) {
		DMFastIterator iter(mSubElements);
		while (iter.NextElement()) {
		
			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			
			DMAttribute* attr = objDM->FindAttributeByTitle(inTitle);
			if (attr != nil) {
				ValidateObject_(attr);
				return attr;
			}
		}
	}

	// Nope. Didn't find it.

	return nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** modified flag accessors

// ---------------------------------------------------------------------------
//		* IsSubModified											[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::IsSubModified.

Boolean
DMContainerMixin::IsSubModified(
	Boolean		inTentative) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Prevent object from being disposed during the method.

	StSharer lockObj(&mThisAsElement);
	
	// If this element already has it's submodified flag set,
	// return it.

	if (mThisAsElement.DMElement::IsSubModified(inTentative))
		return true;
	else if (inTentative) {
	
		// We want to collect tentative changes (partially
		// completed in-place editing, etc.). This is more
		// time-consuming because it means that each subelement
		// has to broadcast a message to its user-interface
		// objects. We short-circuit this evaluation: as soon
		// as any object says that it's being modified, we return true.

		DMFastIterator iter(mSubElements);
		while (iter.NextElement()) {

			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);

			if (objDM->IsSubModified(inTentative))
				return true;
		}
	}
	
	// Nope, nothing's modified here.
	
	return false;

}


// ---------------------------------------------------------------------------
//		* ResetModified											[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::ResetModified.

void
DMContainerMixin::ResetModified()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Prevent object from being disposed during the method.

	StSharer lockObj(&mThisAsElement);
	
	// Clear this element's modified flag.

	mThisAsElement.DMElement::ResetModified();

	// Clear all subelements' modified flags.

	DMFastIterator iter(mSubElements);
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		objDM->SuperResetModified(&mThisAsElement);
	}
}


// ---------------------------------------------------------------------------
//		* DisableSubs											[protected]
// ---------------------------------------------------------------------------
//	Call to tell all subelements that they must become disabled.

void
DMContainerMixin::DisableSubs()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Iterate through subelements and disable them.

	DMFastIterator iter(mSubElements);
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		objDM->SuperDisable(&mThisAsElement);
	}
}


// ---------------------------------------------------------------------------
//		* EnableSubs											[protected]
// ---------------------------------------------------------------------------
//	Call to tell all subelements that they may become enabled.

void
DMContainerMixin::EnableSubs()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);

	// Iterate through subelements and enable them.

	DMFastIterator iter(mSubElements);
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		objDM->SuperEnable(&mThisAsElement);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** data model changes

// ---------------------------------------------------------------------------
//		* CanHostElement										[protected]
// ---------------------------------------------------------------------------
//	Return true if this element or any subelement can host the new element.

Boolean
DMContainerMixin::CanHostElement(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);
	ValidateObject_(inNewElement);

	// Prevent object from being disposed during the method.

	StSharer lockObj(&mThisAsElement);
	
	// If this is not the subobject list, we allow the subelements
	// to take a crack at hosting this element.

	if (!mIsSubObjectList) {

		DMFastIterator iter(mSubElements);
		while (iter.NextElement()) {

			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);

			if (objDM->SuperCanHostElement(inNewElement, ioPlacement))
				return true;
		}
	}
	
	// Subelements didn't want it, see if this element wants it.

	return mThisAsElement.DMElement::CanHostElement(inNewElement, ioPlacement);

}


// ---------------------------------------------------------------------------
//		* HostElementSelf										[protected]
// ---------------------------------------------------------------------------
//	An optional helper method to insert elements which have already been 
//	approved (via CanHostElementSelf) into the subelement list.

void
DMContainerMixin::HostElementSelf(
	DMElement*					inNewElement,
	const DM_DragPlacement*		inPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);
	
	// Add this element to our subelement list.
	
	InsertElementAt(inPlacement->mInsertionIndex, inNewElement, mIsSubObjectList);

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendListChangeMessage									[protected]
// ---------------------------------------------------------------------------
//	Common code used to send list change messages whenever an element
//	is added or removed from th elist.

void
DMContainerMixin::SendListChangeMessage(
	DMElement*	inElement,
	Boolean		inElementAdded)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mThisAsElement);
	ValidateObject_(inElement);
	
	// Create a list change message describing this
	// container and the {inserted/removed} element.
	
	DM_ListChange msg = { &mThisAsElement, inElement, inElementAdded };
	mThisAsElement.SendUpdateMessage(Container_ListChange, &msg, &mThisAsElement);

	// If the element is being removed, tell the world that it's going away.
	
	if ((!inElementAdded) && (inElement->GetSuperElement() == &mThisAsElement))
		inElement->GoingAway();

}


// ===========================================================================

#pragma mark -
#pragma mark ** shallow cloning helper

// ---------------------------------------------------------------------------
//		* SetShallowClone										[protected]
// ---------------------------------------------------------------------------
//	Recommended override for DMElement::SetShallowClone.

void
DMContainerMixin::SetShallowClone()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Stomp out "original" object pointer so we don't grab its elements.
	// This call must be placed before FinishCreate() in order to be
	// effective.

	mOriginal = nil;

}
