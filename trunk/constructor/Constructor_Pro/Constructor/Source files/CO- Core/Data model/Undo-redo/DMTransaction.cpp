// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMTransaction.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: DMTransaction.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/05/96   Time: 22:31
//	Updated in $/Constructor/Source files/Data model/Undo-redo
//	Added GetDescription overload to avoid "hides inherited virtual
//	function" warning.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:15
//	Updated in $/Constructor/Source files/Data model/Undo-redo
//	Updated for new undo/redo menu string architecture.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:33
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMTransaction.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Data model : Undo-redo
#include "DMSubAction.h"

	// Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** DMTransaction ***

// ---------------------------------------------------------------------------
//		* DMTransaction(ConstStringPtr, DMSelection*)			[protected]
// ---------------------------------------------------------------------------
//	Constructor, specifying the undo/redo string and selection
//	Called only by StDMTransactionBuilder.

DMTransaction::DMTransaction(
	ConstStringPtr	inDescriptionString,
	DMSelection*	inSelection)

: mSubActions(sizeof (DMSubAction*)),
  mDescription(inDescriptionString)

{

	// Validate selection pointer.

	if (inSelection != nil)
		ValidateObject_(inSelection);

	// Reset variables.

	mIsDone = true;
	mSelection = inSelection;

	// Trim ellipsis from description.
	
	StApplicationContext appContext;
	LStr255 trailingEllipsis(STR_UndoRedoStrings, str_TrailingEllipsis);
	
	SInt16 ellipsisIndex = mDescription.ReverseFind(trailingEllipsis);
	if (ellipsisIndex && (ellipsisIndex == (mDescription.Length() - trailingEllipsis.Length() + 1)))
		mDescription[(UInt8)0] = ellipsisIndex - 1;

}


// ---------------------------------------------------------------------------
//		* ~DMTransaction										[protected]
// ---------------------------------------------------------------------------
//	Destructor

DMTransaction::~DMTransaction()
{

	ValidateThis_();
	
	// Delete all of the subactions.

	LFastArrayIterator iter(mSubActions);
	DMSubAction* theSubAction;
	while (iter.Next(&theSubAction)) {
		ValidateObject_(theSubAction);
		delete theSubAction;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** undo/redo

// ---------------------------------------------------------------------------
//		* RedoSelf												[protected]
// ---------------------------------------------------------------------------
//	Redo all the subactions in sequence from first to last.

void
DMTransaction::RedoSelf()
{

	ValidateThis_();

	// Redo all the subactions.

	LFastArrayIterator iter(mSubActions);
	DMSubAction* theSubAction;
	while (iter.Next(&theSubAction)) {
		ValidateObject_(theSubAction);
		theSubAction->Redo();
	}
	
	// Select all the affected objects.
	
	SelectAffectedObjects();
	
}


// ---------------------------------------------------------------------------
//		* UndoSelf												[protected]
// ---------------------------------------------------------------------------
//	Undo all the subactions in sequence from last to first.

void
DMTransaction::UndoSelf()
{

	ValidateThis_();

	// This is where the action *really* gets undone.

	LFastArrayIterator iter(mSubActions, LArrayIterator::from_End);
	DMSubAction* theSubAction;
	while (iter.Previous(&theSubAction)) {
		ValidateObject_(theSubAction);
		theSubAction->Undo();
	}

	// Select all the affected objects.
	
	SelectAffectedObjects();
	
}


// ---------------------------------------------------------------------------
//		* Finalize												[protected]
// ---------------------------------------------------------------------------
//	Finalize all the subactions.

void
DMTransaction::Finalize()
{
	ValidateThis_();
	
	// Finalize all the subactions.

	LFastArrayIterator iter(mSubActions);
	DMSubAction* theSubAction;
	while (iter.Next(&theSubAction)) {
		ValidateObject_(theSubAction);
		theSubAction->Finalize();
	}
	
	// Finalize this action.
	
	LAction::Finalize();

}


// ---------------------------------------------------------------------------
//		* SelectAffectedObjects									[protected]
// ---------------------------------------------------------------------------
//	Called by RedoSelf and UndoSelf to select all objects that were
//	affected by this transaction.

void
DMTransaction::SelectAffectedObjects()
{

	ValidateThis_();

	// Sanity check: Make sure there's a selection
	
	if (mSelection == nil)
		return;
	ValidateObject_(mSelection.GetObject());
	
	// Start from empty selection.

	mSelection->SelectNone();
	
	// Select all affected objects.

	LFastArrayIterator iter(mSubActions);
	DMSubAction* theSubAction;
	while (iter.Next(&theSubAction)) {
		DMObject* affectedObject = theSubAction->GetAffectedObject();
		if (affectedObject != nil) {
			ValidateObject_(affectedObject);
			mSelection->SelectAdd(affectedObject);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu descriptions

// ---------------------------------------------------------------------------
//		* GetDescription(Str255, Str255)						[protected]
// ---------------------------------------------------------------------------
//	Returns the string that should be displayed for the Undo or Redo command.

void
DMTransaction::GetDescription(
	Str255	outRedoString,
	Str255	outUndoString) const
{

	ValidateThis_();
	
	// The undo/redo strings are made from a combination
	// of a predefined string (i.e. "Undo " or "Redo ") and
	// a text description which was provided by the transaction
	// builder.
	
	LStr255 redo(STR_UndoRedoStrings, str_RedoSpace);
	LStr255 undo(STR_UndoRedoStrings, str_UndoSpace);
	redo += mDescription;
	undo += mDescription;
		
	LString::CopyPStr(redo, outRedoString);
	LString::CopyPStr(undo, outUndoString);

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* AcceptSubActions										[protected]
// ---------------------------------------------------------------------------
//	Called by StDMTransactionBuilder::Commit to hand off the list
//	of subactions that comprise this action.

void
DMTransaction::AcceptSubActions(
	LArray& inSubActions)
{
	
	ValidateThis_();
	
	// Copy the subaction pointers one by one.

	LArrayIterator iter(inSubActions);
	DMSubAction* theSubAction;
	while (iter.Next(&theSubAction)) {
		ValidateObject_(theSubAction);
		mSubActions.InsertItemsAt(1, LArray::index_Last, &theSubAction);
	}
	
}
