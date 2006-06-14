// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMListSubAction.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/29/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMListSubAction.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:49
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
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

#pragma once


	// Core : Data model : Core objects
#include "DMObject.h"
#include "DMPointers.h"

	// Core : Data model : Undo-redo
#include "DMSubAction.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMContainerMixin;


// ===========================================================================
//		* DMListSubAction
// ===========================================================================
//
//	A DMListSubAction is a subaction to add or remove a DMElement
//	from a container element (using DMContainerMixin). It is
//	automatically created by DMContainerMixin::InsertElementAt or
//	DMContainerMixin::RemoveElement if a transaction builder is active.
//
// ===========================================================================

class DMListSubAction : public DMSubAction {

public:
							DMListSubAction(
									DMContainerMixin*	inContainer,
									DMElement*			inContainerElement,
									DMElement*			inMovedElement,
									Boolean				inChangeSuperElement,
									UInt32				inAfterIndex);
	virtual					~DMListSubAction();

	virtual void			RedoSelf();
	virtual void			UndoSelf();
	virtual DMObject*		GetAffectedObject() const;

protected:
	void					MoveFromTo(
									UInt32				inFromIndex,
									UInt32				inToIndex);

	
	// data members

protected:
	DMContainerMixin*		mContainer;
	DMElementPtr			mContainerElement;
	DMElementPtr			mMovedElement;
	DMObjectPtr				mAffectedObject;
	Boolean					mChangeSuperElement;
	
	UInt32					mBeforeIndex;
	UInt32					mAfterIndex;

};
