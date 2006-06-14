// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMSelection.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/01/18 01:32:36 $
//	$History: DMSelection.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:22
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 18:51
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Added SortSelection method.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:52
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Update to use new Enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMSelection.h"

	// Core : Data model : Utility classes
#include "DMIterator.h"
#include "DMSequenceComparator.h"


// ===========================================================================

#pragma mark *** DMSelection ***

// ---------------------------------------------------------------------------
//		* DMSelection()
// ---------------------------------------------------------------------------
//	Default constructor

DMSelection::DMSelection()
{
}


// ---------------------------------------------------------------------------
//		* DMSelection(DMSelection&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMSelection::DMSelection(
	const DMSelection&	inOriginal)

: DMElement(inOriginal)

{

	// Validate pointers.
	
	ValidateObject_(&inOriginal);
	
	// Copy other selection's root element.
	
	mRootElement = inOriginal.mRootElement;
	
	// Copy other selection's selected items list.
	
	CopySelectionList(&inOriginal);

}


// ---------------------------------------------------------------------------
//		* ~DMSelection
// ---------------------------------------------------------------------------
//	Destructor

DMSelection::~DMSelection()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection changers

// ---------------------------------------------------------------------------
//		* SelectNone
// ---------------------------------------------------------------------------
//	Make the selection empty (i.e. remove all elements from the selection).

void
DMSelection::SelectNone()
{

	// Validate pointers.

	ValidateThis_();
	
	// Empty the selection list.
	
	PrimListClear();

}


// ---------------------------------------------------------------------------
//		* SelectSimple
// ---------------------------------------------------------------------------
//	Select a single element. Empties the selection, then adds the specified
//	element.

void
DMSelection::SelectSimple(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// Remove all elements from list, then add this one.
	
	PrimListClear();
	PrimListAdd(inElement);

}


// ---------------------------------------------------------------------------
//		* SelectToggle
// ---------------------------------------------------------------------------
//	If the specified element is selected, deselect it. If not selected,
//	select it now. Does not affect the selection status of any other
//	element. Used primarily for shift-clicking on an item.

void
DMSelection::SelectToggle(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// If this element is selected, remove it.
	// Otherwise add it.

	if (IsSelected(inElement))
		PrimListRemove(inElement);
	else
		PrimListAdd(inElement);

}


// ---------------------------------------------------------------------------
//		* SelectAdd
// ---------------------------------------------------------------------------
//	Add the specified element to the selection if it isn't already selected.

void
DMSelection::SelectAdd(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Add this element to selection.

	PrimListAdd(inElement);

}


// ---------------------------------------------------------------------------
//		* SelectRemove
// ---------------------------------------------------------------------------
//	Remove the specified element from the selection.

void
DMSelection::SelectRemove(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// Remove this element.

	PrimListRemove(inElement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** selection list manipulations

// ---------------------------------------------------------------------------
//		* CopySelectionList
// ---------------------------------------------------------------------------
//	Make this selection list an exact duplicate of another selection
//	object's selection list.

void
DMSelection::CopySelectionList(
	const DMSelection*	inSelection)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSelection);

	// Clear our selection list.

	SelectNone();

	// Copy the other selection's list one by one.

	DMFastIterator iter(inSelection->mSelectedElements);
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		SelectAdd(objDM);
	}
}


// ---------------------------------------------------------------------------
//		* AncestorIsSelected
// ---------------------------------------------------------------------------
//	Return true if any ancestor of the named element is selected.

Boolean
DMSelection::AncestorIsSelected(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// Walk up the data model hierarchy until we find either
	// the top of the chain or an ancestor that is selected.
	
	DMElement* ancestor = inElement->GetSuperElement();
	while (ancestor != nil) {
		ValidateObject_(ancestor);
		if (IsSelected(ancestor))
			return true;
		ancestor = ancestor->GetSuperElement();
	}

	// Got to top of chain, no ancestor was selected.

	return false;

}


// ---------------------------------------------------------------------------
//		* EliminateRecursion
// ---------------------------------------------------------------------------
//	Create a selection list which doesn't contain any recursion
//	(i.e., none of the selected elements' ancestors are selected).

void
DMSelection::EliminateRecursion()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Remove every element in this selection whose ancestor
	// is also selected.
	
	DMIterator iter(mSelectedElements);
	while (iter.NextElement()) {
		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		if (AncestorIsSelected(element))
			SelectRemove(element);
	}
}


// ---------------------------------------------------------------------------
//		* SortSelection
// ---------------------------------------------------------------------------
//	Sort the selection so that elements appear in the same order as they
// 	do in the actual data model.

void
DMSelection::SortSelection()
{

	// Validate pointers.
	
	ValidateThis_();

	// Sort the selected elements by their position in the
	// data model hierarchy.

	mSelectedElements.SetComparator(new DMSequenceComparator);
	mSelectedElements.Sort();

}


// ===========================================================================

#pragma mark -
#pragma mark ** command status

// ---------------------------------------------------------------------------
//		* SelectionIsNotEmpty
// ---------------------------------------------------------------------------
//	Return true if there is at least one selected element.

Boolean
DMSelection::SelectionIsNotEmpty()
{

	// Validate pointers.

	ValidateThis_();
	
	// Selection is non-empty if the number of items is non-zero.
	
	return mSelectedElements.GetCount() > 0;

}


// ---------------------------------------------------------------------------
//		* SelectionIsDeletable
// ---------------------------------------------------------------------------
//	Return true if there is at least one selected element that can be deleted.

Boolean
DMSelection::SelectionIsDeletable()
{

	// Validate pointers.

	ValidateThis_();
	
	// Iterate over selected elements. If at least one element
	// is deletable, we say that the selection is deletable.
	
	DMFastIterator iter(mSelectedElements);
	while (iter.NextElement()) {
		DMElement* item = iter.CurrentElement();
		ValidateObject_(item);
		if (item->CanDeleteThis())
			return true;
	}
	
	// No elements were deletable.
	
	return false;

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	DMSelection listens for "going away" messages from all the selected
//	elements. If it receives one, the element is immediately removed from
//	the selection.

void
DMSelection::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();
	
	// Catch going away message.
	
	switch (inMessage) {
		case Element_GoingAway: {
			DMElement* deadElement = (DMElement*) ioParam;
			ValidateObject_(deadElement);
			SelectRemove(deadElement);
			break;
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection primitives

// ---------------------------------------------------------------------------
//		* PrimListAdd											[protected]
// ---------------------------------------------------------------------------
//	Primitive for adding an element to selection. Set up listener relationship
//	with the element. Broadcast selection change message.

void
DMSelection::PrimListAdd(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// If this element is already selected, nothing more to do.

	if (IsSelected(inElement))
		return;

	// Listen for going away message from this element.

	inElement->AddListener(this);

	// Add it to our selection list.

	mSelectedElements.InsertItemAt(LArray::index_Last, inElement);

	// Send a selection changed message.

	DM_SelectionChanged msg = { this, inElement, true };
	BroadcastMessage(Selection_Changed, &msg);
	
	// Menus may need to be enabled/disabled.
	
	LCommander::SetUpdateCommandStatus(true);

}


// ---------------------------------------------------------------------------
//		* PrimListRemove										[protected]
// ---------------------------------------------------------------------------
//	Primitive for removing an element from selection. Close listener
//	relationship with the element. Broadcast selection change message.

void
DMSelection::PrimListRemove(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// If this element is already deselected, nothing more to do.

	if (!IsSelected(inElement))
		return;

	// Stop listening for messages from this element.

	inElement->RemoveListener(this);

	// Remove it from our selection list.

	mSelectedElements.RemoveItem(inElement);

	// Send a selection changed message.

	DM_SelectionChanged msg = { this, inElement, false };
	BroadcastMessage(Selection_Changed, &msg);

	// Menus may need to be enabled/disabled.
	
	LCommander::SetUpdateCommandStatus(true);

}


// ---------------------------------------------------------------------------
//		* PrimListClear											[protected]
// ---------------------------------------------------------------------------
//	Primitive for emptying the entire selection. Calls PrimListRemove
//	for all selected elements.

void
DMSelection::PrimListClear()
{

	// Validate pointers.

	ValidateThis_();

	// Use the primitive list remover. Remove everything
	// in the list.

	DMIterator iter(mSelectedElements);
	while (iter.NextElement()) {
		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		PrimListRemove(element);
	}
}
