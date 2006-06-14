// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMTransactionBuilder.h	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: StDMTransactionBuilder.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Fixed CR/LF problem
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

#pragma once

	// Core : Data model : Core classes
#include "DMPointers.h"
#include "DMSelection.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMSubAction;


// ===========================================================================
//		* StDMTransactionBuilder
// ===========================================================================
//
//	StDMTransactionBuilder is a helper class which collects
//	subactions for later packaging into a DMTransaction. Several
//	StDMTransactionBuilder objects may be on the stack at one time;
//	only the first one (topmost on the stack) is active.
//
//	See the "theory of operation" in the undo/redo README document
//	in this folder.
//
// ===========================================================================

class StDMTransactionBuilder {

public:
							StDMTransactionBuilder(
									LCommander*		inUndoerHost = nil,
									DMSelection*	inSelection = nil,
									CommandT		inMenuCommand = cmd_Nothing);
							~StDMTransactionBuilder();

	// command building

	void					SetDescription(ConstStringPtr inDescription);
	void					PostSubAction(DMSubAction* inSubAction);

	// completion of command

	void					Commit();
	void					Abort();

	// singleton accessor

	static StDMTransactionBuilder* GetSubActionHost();


	// data members

protected:
	LArray					mSubActions;				// (DMSubAction*) actions recorded during this session
	Boolean					mCommitted;					// true if transaction committed successfully
	Boolean					mAborted;					// true if transaction was aborted
	LCommander*				mUndoerHost;				// commander to receive transaction to when committed
	DMSelectionPtr			mSelection;					// selection object to update
	LStr255					mDescription;				// description string

	static StDMTransactionBuilder* sCurrentBuilder;		// currently active transaction builder

	friend class StDMTransactionHider;

};
