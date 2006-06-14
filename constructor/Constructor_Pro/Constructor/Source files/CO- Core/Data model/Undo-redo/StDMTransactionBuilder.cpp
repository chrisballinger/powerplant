// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMTransactionBuilder.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: StDMTransactionBuilder.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/04/96   Time: 12:14
//	Updated in $/Constructor/Source files/Data model/Undo-redo
//	Fixed a bug in Abort.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:23
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

#include "StDMTransactionBuilder.h"

	// Core : Data model : Undo-redo
#include "DMSubAction.h"
#include "DMTransaction.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

StDMTransactionBuilder* StDMTransactionBuilder::sCurrentBuilder = nil;


// ===========================================================================

#pragma mark *** StDMTransactionBuilder ***

// ---------------------------------------------------------------------------
//		* StDMTransactionBuilder(...)
// ---------------------------------------------------------------------------
//	Constructor

StDMTransactionBuilder::StDMTransactionBuilder(
	LCommander*		inUndoerHost,
	DMSelection*	inSelection,
	CommandT		inMenuCommand)

: mSubActions(sizeof (DMSubAction*))

{

	// Validate pointers.

	if (inUndoerHost != nil)
		ValidateObject_(inUndoerHost);
	if (inSelection != nil)
		ValidateObject_(inSelection);

	// If no other transaction builder is active, we become the current builder.
	
	if (sCurrentBuilder == nil)
		sCurrentBuilder = this;

	// Record setup information.
	
	mCommitted = false;
	mAborted = false;
	
	mUndoerHost = (inUndoerHost != nil) ? inUndoerHost : LCommander::GetTarget();
	mSelection = inSelection;
	
	// Grab menu item text. This forms the description for the
	// undo/redo text, unless overriden by a call to SetDescription.
	
	if (inMenuCommand != cmd_Nothing) {

		LMenuBar* menuBar = LMenuBar::GetCurrentMenuBar();
		ValidateObject_(menuBar);
		
		ResIDT		menuID;
		MenuHandle	menuHandle;
		SInt16		menuItem;
	
		menuBar->FindMenuItem(inMenuCommand, menuID, menuHandle, menuItem);
		if (menuHandle != nil) {
#if !PP_Target_Carbon
			ValidateHandle_((Handle) menuHandle);
#endif
			::GetMenuItemText(menuHandle, menuItem, mDescription);
		}
		else
			SignalCStr_("Didn't find menu command.");
	}
}


// ---------------------------------------------------------------------------
//		* ~StDMTransactionBuilder
// ---------------------------------------------------------------------------
//	Destructor

StDMTransactionBuilder::~StDMTransactionBuilder()
{

	// If we haven't been committed, an exception must
	// have been thrown. Abort the transaction and roll back
	// any changes that have been made to date.

	if (!mCommitted) {
		try {
			Abort();
		}
		catch(...) {
			// ignore errors... must delete this...
		}
	}
	
	// Make sure we're no longer the current transaction.
	
	if (sCurrentBuilder == this)
		sCurrentBuilder = nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** command building

// ---------------------------------------------------------------------------
//		* SetDescription
// ---------------------------------------------------------------------------
//	Set a description string for the transaction's undo/redo string.

void
StDMTransactionBuilder::SetDescription(
	ConstStringPtr inDescription)
{
	mDescription = inDescription;
}


// ---------------------------------------------------------------------------
//		* PostSubAction
// ---------------------------------------------------------------------------
//	Call to add a subaction to the transaction. Usually called by
//	the element's set value routine.

void
StDMTransactionBuilder::PostSubAction(
	DMSubAction* inSubAction)
{

	// Validate pointers.

	ValidateObject_(inSubAction);

	// First, a couple of sanity checks.
	
	if (mCommitted) {
		SignalCStr_("Posted subaction after commit.");
		return;
	}
	
	if (mAborted) {
		SignalCStr_("Posted subaction after abort.");
		return;
	}
	
	// Add the subaction to our list.

	mSubActions.AddItem(&inSubAction);

}


// ===========================================================================

#pragma mark -
#pragma mark ** completion of command

// ---------------------------------------------------------------------------
//		* Commit
// ---------------------------------------------------------------------------
//	Call when all the subactions for this action have been successfully
//	issued. Creates a DMTransaction object to undo and redo these
//	subactions as per user requests.

void
StDMTransactionBuilder::Commit()
{

	// Validate pointers.

	ValidateObject_(mUndoerHost);
	if (mSelection != nil)
		ValidateObject_(mSelection.GetObject());

	// If empty subaction list, there's nothing to do.

	if (mSubActions.GetCount() == 0)
		return;
	
	// Allow each of the subactions to gather any additional information they may need.
	
	LArrayIterator iter(mSubActions);
	DMSubAction* subAction;
	while (iter.Next(&subAction)) {
		ValidateObject_(subAction);
		subAction->Commit();
	}
	
	// Create a DMTransaction to undo & redo this set of subactions.
	
	DMTransaction* transaction = new DMTransaction(mDescription, mSelection);
	ValidateObject_(transaction);

	transaction->AcceptSubActions(mSubActions);
	mUndoerHost->PostAction(transaction);
	mCommitted = true;
	
	LCommander::SetUpdateCommandStatus(true);
	
}


// ---------------------------------------------------------------------------
//		* Abort
// ---------------------------------------------------------------------------
//	Call when a partially completed user action needs to be cancelled.
//	Undoes all of the currently known subactions.

void
StDMTransactionBuilder::Abort()
{

	// Flag that this transaction is aborted.

	mAborted = true;
	
	// We are no longer the host for new subactions.
	
	if (sCurrentBuilder == this)
		sCurrentBuilder = nil;

	// Roll back all of the subactions.

	LArrayIterator iter(mSubActions, LArrayIterator::from_End);
	DMSubAction* subAction;
	while (iter.Previous(&subAction)) {
		ValidateObject_(subAction);
		subAction->Undo();
		subAction->Finalize();
		mSubActions.Remove(&subAction);
		delete subAction;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** singleton accessor

// ---------------------------------------------------------------------------
//		* GetSubActionHost										[static]
// ---------------------------------------------------------------------------
//	Call to get the StDMTransactionBuilder which should receive
//	any newly created subactions.

StDMTransactionBuilder*
StDMTransactionBuilder::GetSubActionHost()
{
	if (sCurrentBuilder == nil)
		return nil;
	else
		return sCurrentBuilder;
}
