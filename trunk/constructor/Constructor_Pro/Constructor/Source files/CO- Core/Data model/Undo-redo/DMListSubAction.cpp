// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMListSubAction.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/29/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMListSubAction.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:49
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:21
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Undo-redo'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:32
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMListSubAction.h"


// ===========================================================================

#pragma mark *** DMListSubAction ***

// ---------------------------------------------------------------------------
//		* DMListSubAction(...)
// ---------------------------------------------------------------------------
//	Constructor

DMListSubAction::DMListSubAction(
	DMContainerMixin*	inContainer,
	DMElement*			inContainerElement,
	DMElement*			inMovedElement,
	Boolean				inChangeSuperElement,
	UInt32				inAfterIndex)

{
	ValidateThis_();
	ValidateObject_(inContainer);
	ValidateObject_(inContainerElement);
	ValidateObject_(inMovedElement);

	mContainer = inContainer;
	mContainerElement = inContainerElement;
	mMovedElement = inMovedElement;
	mAffectedObject = inContainerElement->GetAffectedObject();
	mChangeSuperElement = inChangeSuperElement;
	mBeforeIndex = mContainer->GetSubElements().FetchIndexOf(&inMovedElement);
	mAfterIndex = inAfterIndex;
}


// ---------------------------------------------------------------------------
//		* ~DMListSubAction
// ---------------------------------------------------------------------------
//	Destructor

DMListSubAction::~DMListSubAction()
{
	ValidateThis_();
}


// ---------------------------------------------------------------------------
//		* RedoSelf
// ---------------------------------------------------------------------------
//	Move the element to its "after" position.

void
DMListSubAction::RedoSelf() 
{
	ValidateThis_();
	MoveFromTo(mBeforeIndex, mAfterIndex);
}


// ---------------------------------------------------------------------------
//		* UndoSelf
// ---------------------------------------------------------------------------
//	Move the element to its "before" position.

void
DMListSubAction::UndoSelf() 
{
	ValidateThis_();
	MoveFromTo(mAfterIndex, mBeforeIndex);
}


// ---------------------------------------------------------------------------
//		* GetAffectedObject
// ---------------------------------------------------------------------------
//	Return the affected element, but only if it is currently in the list..

DMObject*
DMListSubAction::GetAffectedObject() const
{
	ValidateThis_();
	ValidateObject_(mMovedElement.GetObject());

	if (CanRedo()) {
		if (mBeforeIndex != 0)
			return mMovedElement->GetAffectedObject();
	}
	else {
		if (mAfterIndex != 0)
			return mMovedElement->GetAffectedObject();
	}
	return nil;
}


// ---------------------------------------------------------------------------
//		* MoveFromTo											[protected]
// ---------------------------------------------------------------------------
//	Common code to move an element into or out of the container.

void
DMListSubAction::MoveFromTo(
	UInt32	inFromIndex,
	UInt32	inToIndex)
{

	ValidateThis_();
	ValidateObject_(mContainer);
	ValidateObject_(mMovedElement.GetObject());

	// Remove the element from its old location.

	if (inFromIndex != 0) {
		mContainer->SendListChangeMessage(mMovedElement, false);
		if (mChangeSuperElement) {
			mMovedElement->SetDeleted(true);
			mMovedElement->SetSuperElement(nil);
		}
		(const_cast<LSharableArray&>(mContainer->GetSubElements())).
					RemoveItem(mMovedElement);
	}
	
	// Place the element in its new location.

	if (inToIndex != 0) {
		(const_cast<LSharableArray&>(mContainer->GetSubElements())).
					InsertItemAt(inToIndex, mMovedElement);
		if (mChangeSuperElement) {
			mMovedElement->SetSuperElement(mContainerElement);
			mMovedElement->SetDeleted(false);
		}
		mContainer->SendListChangeMessage(mMovedElement, true);
	}
}
