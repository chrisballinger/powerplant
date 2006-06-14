// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMDragSelector.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 02/12/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMDragSelector.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/06/96   Time: 13:47
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed pointer validation in constructor (now accepts nil selection).
//	Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMDragSelector.h"

	// Core : Data model : Core objects
#include "DMElement.h"
#include "DMSelection.h"

	// Core : Data model : Utilities
#include "DMIterator.h"


// ===========================================================================

#pragma mark *** DMDragSelector ***

// ---------------------------------------------------------------------------
//		* DMDragSelector(LView&, DMSelection*)
// ---------------------------------------------------------------------------
//	Constructor, specifiying the view to draw onto and the selection to
//	be modified

DMDragSelector::DMDragSelector(
	LView&			inView,
	DMSelection*	inSelection)

: mView(inView)

{

	// Validate pointers.

	ValidateObject_(&mView);
	if (inSelection != nil)
		ValidateObject_(inSelection);

	// Save selection pointer.

	mSelection = inSelection;
	
}


// ---------------------------------------------------------------------------
//		* ~DMDragSelector
// ---------------------------------------------------------------------------
//	Destructor

DMDragSelector::~DMDragSelector()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** overrides for selection behavior

// ---------------------------------------------------------------------------
//		* CheckSelectionChanged
// ---------------------------------------------------------------------------
//	Returns true if the list of selected objects is going to change.
//	LMarqueeTask uses this to determine if it needs to remove the
//	marquee rectangle drawing.

Boolean
DMDragSelector::CheckSelectionChanged(
	const LMarqueeTask&	inMarqueeTask,
	const Rect&			inOldMarqueeRect,
	const Rect&			inNewMarqueeRect)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mView);

	// Convert local mouse location to port coordinates.
	
	Point localPoint;
	inMarqueeTask.GetCurrentPoint(localPoint);
	mView.LocalToPortPoint(localPoint);

	// See if auto-scrolling is called for.

	Boolean wantsToAutoScroll = !mView.PointIsInFrame(localPoint.h, localPoint.v);

	// See if selection is changing.

	CheckElements(inOldMarqueeRect, inNewMarqueeRect);
	
	// Return true in either case.
	
	return (mAddToSelection.GetCount() > 0)
		|| (mRemoveFromSelection.GetCount() > 0)
		|| wantsToAutoScroll;

}


// ---------------------------------------------------------------------------
//		* UpdateSelection
// ---------------------------------------------------------------------------
//	CheckSelectionChanged returned true. We can now redraw the
//	screen. This includes autoscrolling and changing the selection.

void
DMDragSelector::UpdateSelection(
	const LMarqueeTask&	inMarqueeTask,
	const Rect&			/* inOldMarqueeRect */,
	const Rect&			/* inNewMarqueeRect */)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mView);
	ValidateObject_(mSelection.GetObject());

	// Auto-scroll the display.
	
	Point localPoint;
	inMarqueeTask.GetCurrentPoint(localPoint);
	mView.AutoScrollImage(localPoint);

	// Some objects moved in or out of the selection, adjust selection accordingly.
	// Bracket because we want the iterators to go away before we change the lists.
	
	DMElement* obj;
	{
		DMIterator addIter(mAddToSelection);
		while (addIter.NextElement()) {
			obj = addIter.CurrentElement();
			ValidateObject_(obj);
			mSelection->SelectAdd(obj);
		}
		DMIterator removeIter(mRemoveFromSelection);
		while (removeIter.NextElement()) {
			obj = removeIter.CurrentElement();
			ValidateObject_(obj);
			mSelection->SelectRemove(obj);
		}
	}

	// Empty the selection change lists.
	
	mAddToSelection.RemoveItemsAt(mAddToSelection.GetCount(), 1);
	mRemoveFromSelection.RemoveItemsAt(mRemoveFromSelection.GetCount(), 1);
	
	// Make sure the display is updated.
	
	mView.UpdatePort();

}


// ---------------------------------------------------------------------------
//		* FocusMarqueeArea
// ---------------------------------------------------------------------------
//	Configure the drawing environment for drawing in the view which
//	is hosting the marquee.

void
DMDragSelector::FocusMarqueeArea()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mView);
	
	// Ask the view to focus itself.

	mView.FocusDraw();
	
}


LView*
DMDragSelector::GetTargetView() const
{
	return &mView;
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection testing

// ---------------------------------------------------------------------------
//		* FoundElement
// ---------------------------------------------------------------------------
//	Called by subclass's CheckElements when the physical display for
//	an element has been located. If the element is hit by the new
//	marquee rectangle, inIsInSelectionRect should be set to true.

void
DMDragSelector::FoundElement(
	DMElement*	inElement,
	Boolean		inIsInSelectionRect)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&mView);
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(inElement);

	// If this object was marked as originally in the selection,
	// treat it backwards from the normal (i.e. deselect if inside
	// the selection rect).

	if (mInvertSelection.FetchIndexOf(&inElement))
		inIsInSelectionRect = !inIsInSelectionRect;
	
	// Make sure object is (not) selected.

	if (mSelection->IsSelected(inElement) != inIsInSelectionRect) {
	
		// Something changed, flag it for later { insertion, removal }.

		if (inIsInSelectionRect)
			mAddToSelection.InsertItemAt(LArray::index_Last, inElement);
		else
			mRemoveFromSelection.InsertItemAt(LArray::index_Last, inElement);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection testing

// ---------------------------------------------------------------------------
//		* RecordInitialSelection
// ---------------------------------------------------------------------------
//	Call just before doing the marquee selection. Set inInvertCurrentSelection
//	to true if the shift key is down.

void
DMDragSelector::RecordInitialSelection(
	Boolean		inInvertCurrentSelection)
{

	// Validate this.

	ValidateThis_();
	ValidateObject_(&mView);
	ValidateObject_(mSelection.GetObject());
	
	// Clear any previous invert selection list we may have had.

	mInvertSelection.RemoveItemsAt(mInvertSelection.GetCount(), 1);

	// Record selection status. If shift key is down, we record the existing
	// selection. If not, we deselect everything.

	if (inInvertCurrentSelection) {
		DMFastIterator iter(mSelection->GetSelectedElements());
		while (iter.NextElement()) {
			DMElement* obj = iter.CurrentElement();
			ValidateObject_(obj);
			mInvertSelection.InsertItemAt(LArray::index_Last, obj);
		}
	}
	else
		mSelection->SelectNone();

	// Make sure display is accurate before we start dragging.

	mView.UpdatePort();

}
