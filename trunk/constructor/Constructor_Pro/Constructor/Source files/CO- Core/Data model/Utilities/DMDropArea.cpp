// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMDropArea.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/07/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMDropArea.cpp $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 10  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Fixed CR/LF problem
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/18/96   Time: 12:08
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed a bug in SpendTime. It can now handle a nil return value from
//	HostDragElement.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/06/96   Time: 15:57
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed a bug in SpendTime which was allowing it to read edit menu
//	resources from user files.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:00
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Changed ItemIsAcceptable to clone objects that don't share the same
//	root object.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/14/96   Time: 16:50
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed a bug in SpendTime. It wasn't selecting the dropped items.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/03/96   Time: 22:02
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed a bug in UnhiliteDropArea. It wasn't focusing the drop area.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:25
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Updated for new undo/redo menu string architecture.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 18:55
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Added UnhiliteDropArea method. Simplified AutoScrollDropArea.
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

#include "DMDropArea.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Core objects
#include "DMContainerMixin.h"
#include "DMSelection.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMDragTask.h"
#include "DMIterator.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	CURS_DuplicateCursor	= 22700;


// ===========================================================================

#pragma mark *** DMDropArea ***

// ---------------------------------------------------------------------------
//		* DMDropArea(LView*)
// ---------------------------------------------------------------------------
//	Constructor, specifying the view that this object is mixed in with

DMDropArea::DMDropArea(
	LView*	inThisAsView)

: LDragAndDrop(GetWindowFromPort((inThisAsView->GetMacPort())), inThisAsView),
  mThisAsView(*inThisAsView),
  mDragItemBounds(sizeof (Rect))

{

	// Validate pointers.

	ValidateObject_(inThisAsView);
	
	// Reset variables.
	
	mEditMenuStrings = 0;
	mDragRef = 0;

}


// ---------------------------------------------------------------------------
//		* ~DMDropArea
// ---------------------------------------------------------------------------
//	Destructor

DMDropArea::~DMDropArea()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag hit-testing behavior

// ---------------------------------------------------------------------------
//		* PointInDropArea										[protected]
// ---------------------------------------------------------------------------
//	Returns true if the point is within the drop area. Overriden to create
//	a 16-pixel slop area that can be used to trigger auto-scrolling.

Boolean
DMDropArea::PointInDropArea(
	Point	inGlobalPt)
{

	// Validate pointers.

	ValidateThis_();

	// See if the mouse is in or near this view.
	// We accept a slop of 16 pixels. This allows auto-scrolling
	// to be triggered by dragging the mouse outside the view.

	Point portPoint = inGlobalPt;
	mThisAsView.GlobalToPortPoint(portPoint);
	
	Rect portFrame;
	mThisAsView.CalcPortFrameRect(portFrame);
	
	::InsetRect(&portFrame, -16, -16);
	return ::PtInRect(portPoint, &portFrame);

}


// ---------------------------------------------------------------------------
//		* DragIsAcceptable										[protected]
// ---------------------------------------------------------------------------
//	Overriden to reset the placement indicator. For a drag to be acceptable,
//	all items must be acceptable at the SAME insertion location.

Boolean
DMDropArea::DragIsAcceptable(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();
	
	// Reset placement indicator.
	
	mPreviousPlacement = mCurrentPlacement;
	mCurrentPlacement.mHostElement = nil;
	mCurrentPlacement.mInsertionIndex = 0;
	mCurrentPlacement.ioInsertAsDuplicate = false;

	// If option key is down, always duplicate this item.

	SInt16 modifiers;	
	ThrowIfOSErr_(::GetDragModifiers(inDragRef, &modifiers, nil, nil));
	
	if (modifiers & optionKey)
		mCurrentPlacement.ioInsertAsDuplicate = true;
	
	// Now do normal drag testing.
	
	return LDragAndDrop::DragIsAcceptable(inDragRef);

}


// ---------------------------------------------------------------------------
//		* ItemIsAcceptable										[protected]
// ---------------------------------------------------------------------------
//	Attempt to convert the drag item to a data model element. If this works,
//	ask the data model if the new element is acceptable.

Boolean
DMDropArea::ItemIsAcceptable(
	DragReference	inDragRef,
	ItemReference	inItemRef)
{

	// Validate pointers.

	ValidateThis_();

	// Make sure this is a local drag.

	DragAttributes dragAttrs;
	if (::GetDragAttributes(inDragRef, &dragAttrs) != noErr)
		return false;

	if ((dragAttrs & kDragInsideSenderApplication) == 0)
		return false;

	// Okay, it's a local drag. Look for the data model flavor.
	
	FlavorFlags flavorFlags;
	if (::GetFlavorFlags(inDragRef, inItemRef, DM_DragFlavor, &flavorFlags) != noErr)
		return false;

	// Got data model flavor. We can cast the item reference to a data model element.
	
	DMElement* element = (DMElement*) inItemRef;
	ValidateObject_(element);
	
	// If we have already decided on a placement, succeeding items must be acceptable
	// at the same location. Otherwise we can try any location that makes sense.
	
	if (mCurrentPlacement.mHostElement == nil) {
	
		// First decide if this item should be duplicated.
		
		if (!mCurrentPlacement.ioInsertAsDuplicate) {
		
			if (mSelection != nil) {
				ValidateObject_(mSelection.GetObject());
				
				DMElement* selectionRoot = mSelection->GetRootElement();
				if (selectionRoot != nil) {
					if (!(element->IsSubElementOf(selectionRoot)))
						mCurrentPlacement.ioInsertAsDuplicate = true;
				}
			}
		}
		
		// See initial placement.
	
		return ChoosePlacementForElement(inDragRef, inItemRef, element, mCurrentPlacement);

	}
	else {
	
		// See if this item can be placed at the same location.
	
		DM_DragPlacement tempPlace = mCurrentPlacement;
		ValidateObject_(mCurrentPlacement.mHostElement);
		return mCurrentPlacement.mHostElement->CanHostElement(element, &tempPlace);
	}
}


// ---------------------------------------------------------------------------
//		* ElementIsAcceptable									[protected]
// ---------------------------------------------------------------------------
//	Return true if the named element can be hosted at the currently planned
//	drag target.

Boolean
DMDropArea::ElementIsAcceptable(
	const DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Find the existing placement. Make a copy
	// so the tentative host doesn't trample the placement object.
	
	DM_DragPlacement tempCopy = mCurrentPlacement;
	DMElement* host = tempCopy.mHostElement;
	ValidateObject_(host);
	
	// See if the host can host this element at the same location.
	
	return host->CanHostElement(inElement, &tempCopy);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag target drawing

// ---------------------------------------------------------------------------
//		* EnterDropArea											[protected]
// ---------------------------------------------------------------------------
//	Overriden to reset the initial drop placement.

void
DMDropArea::EnterDropArea(
	DragReference	inDragRef,
	Boolean			inDragHasLeftSender)
{

	// Validate pointers.

	ValidateThis_();
	
	// Save a copy of the drag reference for drawing.
	
	mDragRef = inDragRef;

	// Clear placement.

	mPreviousPlacement.mHostElement = nil;
	mPreviousPlacement.mInsertionIndex = 0;
	mPreviousPlacement.ioInsertAsDuplicate = false;

	// Draw drag hiliting if appopriate.

	LDropArea::EnterDropArea(inDragRef, inDragHasLeftSender);

}


// ---------------------------------------------------------------------------
//		* InsideDropArea										[protected]
// ---------------------------------------------------------------------------
//	Called while tracking an acceptable drag. Change the cursor to the copy
//	cursor if appropriate, and auto-scroll the image if appropriate.

void
DMDropArea::InsideDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();

	// Update the mouse cursor.

	SetDropCursor(inDragRef);

	// Auto-scroll the view.

	AutoScrollDropArea(inDragRef);

}

// ---------------------------------------------------------------------------
//		* LeaveDropArea											[protected]
// ---------------------------------------------------------------------------
//	Overriden to restore the cursor to the normal arrow cursor.

void
DMDropArea::LeaveDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();
	
	// Forget drag reference.

	mDragRef = 0;

	// Restore normal mouse cursor

	UCursor::SetArrow();

	// Remove drag hiliting.

	LDropArea::LeaveDropArea(inDragRef);

}


// ---------------------------------------------------------------------------
//		* HiliteDropArea										[protected]
// ---------------------------------------------------------------------------
//	Overriden to remove the one-pixel inset in LDropArea::HiliteDropArea.

void
DMDropArea::HiliteDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();

	// Focus drawing area.

	mThisAsView.FocusDraw();

	// Calculate the drawing region. Unlike LDropArea,
	// we do *not* want to inset the region by one pixel.

	Rect dropRect;
	mThisAsView.CalcLocalFrameRect(dropRect);

	RgnHandle dropRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(dropRgn);
#else
	ValidateHandle_((Handle) dropRgn);
#endif
	::RectRgn(dropRgn, &dropRect);

	// Draw the drag hiliting. Make sure the drawing state is
	// normalized before drawing.

	StColorPenState::Normalize();
	::ShowDragHilite(inDragRef, dropRgn, true);
	
	// Release the drawing region.
	
	::DisposeRgn(dropRgn);

}


// ---------------------------------------------------------------------------
//		* UnhiliteDropArea										[protected]
// ---------------------------------------------------------------------------
//	Overriden to ensure that pen is normalized before removing drag hiliting.

void
DMDropArea::UnhiliteDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();

	// Focus the drawing area.

	mThisAsView.FocusDraw();
	
	// Be sure drawing state is normalized before
	// talking to the Drag Manager.

	StColorPenState::Normalize();
	::HideDragHilite(inDragRef);

}


// ---------------------------------------------------------------------------
//		* SetDropCursor											[protected]
// ---------------------------------------------------------------------------
//	If the element will be placed as a duplicate, show a copy cursor instead of
//	the normal arrow cursor.

void
DMDropArea::SetDropCursor(
	DragReference	/* inDragRef */)
{

	// Validate pointers.
	
	ValidateThis_();

	// If we're copying the object, use our spiffy new copy cursor.
	
	if (mCurrentPlacement.ioInsertAsDuplicate) {
		CursHandle theCursH = ::GetCursor(CURS_DuplicateCursor);
		if (theCursH != nil) {
			ValidateHandle_((Handle) theCursH);
			::SetCursor(*theCursH);
		}
	}
	else {
 		UCursor::SetArrow();
	}
	
}


// ---------------------------------------------------------------------------
//		* AutoScrollDropArea									[protected]
// ---------------------------------------------------------------------------
//	If the mouse is just outside the frame of this view, scroll the image.

void
DMDropArea::AutoScrollDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();

	// Auto-scroll image if needed.
	// Assumes that LView::ScrollImageBy will be overriden
	// to remove and restore the drag hiliting. (See OVTable::ScrollImageBy
	// for an example.)

	Point foo, mouse;
	ThrowIfOSErr_(::GetDragMouse(inDragRef, &foo, &mouse));
	mThisAsView.GlobalToPortPoint(mouse);
	mThisAsView.PortToLocalPoint(mouse);

	mThisAsView.AutoScrollImage(mouse);

}


// ===========================================================================

#pragma mark -
#pragma mark ** drag receive behaviors

// ---------------------------------------------------------------------------
//		* DoDragReceive											[protected]
// ---------------------------------------------------------------------------
//	Overriden to set up the delayed drag processing. The actual drag receive
//	will take place when SpendTime is called.

void
DMDropArea::DoDragReceive(
	DragReference	inDragRef)
{

	// Validate pointers.

	ValidateThis_();

	// Clear drag arrays.
	
	mDragItemBounds.RemoveItemsAt(mDragItemBounds.GetCount(), 1);
	mDragItemElements.RemoveItemsAt(mDragItemElements.GetCount(), 1);

	// Do standard drag receive.

	LDropArea::DoDragReceive(inDragRef);
	
	// If anything was recorded, make sure Constructor comes to front and
	// handles the drag on the next event loop.
	
	if (mDragItemElements.GetCount() > 0) {
		
		// Make sure we get called on the next event loop.
	
		StartRepeating();
	
		// Pull Constructor to front.
		
		ProcessSerialNumber currentPSN;
		if (::GetCurrentProcess(&currentPSN) == noErr)
			::SetFrontProcess(&currentPSN);
		
		// Kill any pending user events.
		
		::FlushEvents(mDownMask | mUpMask | keyDownMask | keyUpMask | autoKeyMask, 0);
		
	}
}


// ---------------------------------------------------------------------------
//		* ReceiveDragItem										[protected]
// ---------------------------------------------------------------------------
//	Convert the drag item to a data model element and cache its location.

void
DMDropArea::ReceiveDragItem(
	DragReference		inDragRef,
	DragAttributes		inDragAttrs,
	ItemReference		inItemRef,
	Rect&				inItemBounds)
{

	// Validate pointers.

	ValidateThis_();

	// Make sure this is a local drag.

	if ((inDragAttrs & kDragInsideSenderApplication) == 0)
		return;

	// Okay, it's a local drag. Look for the data model flavor.
	
	FlavorFlags flavorFlags;
	if (::GetFlavorFlags(inDragRef, inItemRef, DM_DragFlavor, &flavorFlags) != noErr)
		return;

	// Got data model flavor. We can cast the item reference to a data model element.
	
	DMElement* element = (DMElement*) inItemRef;
	ValidateObject_(element);
	
	// Record reference to drag and come back for it later.
	
	mDragItemBounds.InsertItemsAt(1, LArray::index_Last, &inItemBounds);
	mDragItemElements.InsertItemAt(LArray::index_Last, element);
	
}


// ---------------------------------------------------------------------------
//		* HostDragElement										[protected]
// ---------------------------------------------------------------------------
//	Called by SpendTime. Place a single element into the data model at the
//	specified location.

DMElement*
DMDropArea::HostDragElement(
	DMElement*			inElement,
	Rect&				/* inItemLocalBounds */,
	DM_DragPlacement&	ioCurrentPlacement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	ValidateObject_(ioCurrentPlacement.mHostElement);

	// Decide what object to insert.
	
	Boolean clone = ioCurrentPlacement.ioInsertAsDuplicate;
	DMElementPtr itemToInsert = clone ? inElement->Clone() : inElement;
	ValidateObject_(itemToInsert.GetObject());

	// If not cloning, remove the element from its old location.
	
	if (!clone) {

		// We're not cloning, see where the object is currently contained.

		DMElement* super = inElement->GetSuperElement();
		if (super != nil) {
		
			// Element has a superelement. We need to remove it from
			// its existing host.

			ValidateObject_(super);
			DMContainerMixin* superCM = (dynamic_cast<DMContainerMixin*>(super));

			if (superCM != nil) {
			
				ValidateObject_(superCM);
			
				// If moving within the same container, we may have to adjust
				// the insertion index because the remove happens before the
				// insertion.
			
				if (super == ioCurrentPlacement.mHostElement) {
	
					// Find the element's current index.
	
					SInt32 index = superCM->GetSubElements().FetchIndexOf(&inElement);
					
					// If inserting at same location, shortcut out of here.
					
					if ((index == static_cast<SInt32>(ioCurrentPlacement.mInsertionIndex)) ||
						(index == static_cast<SInt32>(ioCurrentPlacement.mInsertionIndex - 1))) {
						
						ioCurrentPlacement.mInsertionIndex++;
						return inElement;
					}
	
					// If before insertion index, adjust insertion index.
					
					if (index < static_cast<SInt32>(ioCurrentPlacement.mInsertionIndex))
						ioCurrentPlacement.mInsertionIndex--;
					
				}
				
				// Now remove the element.
				
				superCM->RemoveElement(inElement);
	
			}
		}
	}

	// Now we can insert the element in its new location.
	
	ioCurrentPlacement.mHostElement->HostElement(itemToInsert, &ioCurrentPlacement);

	// Adjust placement to compensate for newly placed item.
	
	if (ioCurrentPlacement.mInsertionIndex < LArray::index_Last)
		ioCurrentPlacement.mInsertionIndex++;
	
	// Return a pointer to what was just inserted.
	
	if (itemToInsert->GetUseCount() < 2)			// ugh!
		return nil;
	else
		return itemToInsert;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag receive behaviors

// ---------------------------------------------------------------------------
//		* SpendTime												[protected]
// ---------------------------------------------------------------------------
//	Now that it's safe to show dialogs, etc., we can actually process the drag.

void
DMDropArea::SpendTime(
	const EventRecord&	/* inMacEvent */) 
{

	// Validate pointers.

	ValidateThis_();

	// Only need to process a drag once.
	
	StopRepeating();
	
	// Now that we're outside the drag receive callback, we can move items
	// more easily. Moving items may cause dialogs to be shown, which is
	// a Bad Thing while the Drag Manager is active. Now it's OK.

	try {

		// Select the window we're in.

		LView* view = &mThisAsView;
		while (view->GetSuperView() != nil) {
			ValidateObject_(view);
			view = view->GetSuperView();
		}
		
		LWindow* window = (dynamic_cast<LWindow*>(view));
		if (window == nil)
			return;
		ValidateObject_(window);
		
		window->Select();
		
		// Clear selection.
		
		if (mSelection != nil) {
			ValidateObject_(mSelection.GetObject());
			mSelection->SelectNone();
		}
		
		// Set up a transaction to undo/redo drag receive.
		
		StDMTransactionBuilder trans(window, mSelection);
		
		Str255 description;
		{
			StApplicationContext appContext;
			::GetIndString(description, mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings, str_Drag);
		}
		trans.SetDescription(description);

		// Iterate over recorded drag items. Ask the data model to host the elements
		// in their new locations.

		LFastArrayIterator boundsIter(mDragItemBounds);
		DMFastIterator elementIter(mDragItemElements);
		
		Rect bounds;
		while (elementIter.NextElement()) {
		
			// Find the element to be moved.
		
			DMElement* element = elementIter.CurrentElement();
			ValidateObject_(element);

			// Ask subclass to place this element at the location
			// that the user requested (i.e. the final drop location).
			
			boundsIter.Next(&bounds);
			DMElement* newElement = HostDragElement(element, bounds, mCurrentPlacement);
			
			// Select the new element.
			
			if ((mSelection != nil) && (newElement != nil)) {
				ValidateObject_(mSelection.GetObject());
				mSelection->SelectAdd(newElement);
			}
		}
		
		// Commit the transaction.
		
		trans.Commit();
		
	}
	catch (...) {
		// ignore errors
	}

	// Clear drag arrays.
	
	mDragItemBounds.RemoveItemsAt(mDragItemBounds.GetCount(), 1);
	mDragItemElements.RemoveItemsAt(mDragItemElements.GetCount(), 1);

}
