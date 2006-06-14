// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMTransaction.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: DMTransaction.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/05/96   Time: 22:32
//	Updated in $/Constructor/Source files/Data model/Undo-redo
//	Added GetDescription overload to avoid "hides inherited virtual
//	function" warning.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:14
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
#include "DMSelection.h"
#include "DMPointers.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_UndoRedoStrings		= 22800;
const SInt16	str_UndoSpace			= 1;
const SInt16	str_RedoSpace			= 2;
const SInt16	str_SetSpace			= 3;
const SInt16	str_TrailingEllipsis	= 4;


// ===========================================================================
//		* DMTransaction
// ===========================================================================
//
//	A DMTransaction is an LAction (PowerPlant) that contains a list
//	of sub-actions to be redone or undone as a group. This architecture
//	lets us undo/redo an arbitrarily complex user action with relatively
//	little effort. 
//
//	A DMTransaction should never be created directly. Always use an
//	StDMTransactionBuilder to create transaction.
//
// ===========================================================================

class DMTransaction : public LAction {

protected:
							
							DMTransaction(
									ConstStringPtr	inDescriptionString,
									DMSelection*	inSelection);
	virtual					~DMTransaction();

	// undo/redo

	virtual void			RedoSelf();
	virtual void			UndoSelf();
	virtual void			Finalize();
	virtual void			SelectAffectedObjects();

	// menu descriptions

	virtual void			GetDescription(
									Str255			outRedoString,
									Str255			outUndoString) const;

	// configuration

	void					AcceptSubActions(
									LArray&			inSubActions);


	// data members

protected:
	LArray					mSubActions;			// list of subactions for this action
	DMSelectionPtr			mSelection;				// selection to be updated
	LStr255					mDescription;			// undo/redo string

	friend class StDMTransactionBuilder;

};
