// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMSelection.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMSelection.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/03/97    Time: 5:05p
//	Updated in $/ConstructorWin/Includes
//	Made LListener derivation public (not protected)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 16:52
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:22
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 18:50
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Added SortSelection method.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core classes
#include "DMElement.h"

	// PowerPlant : CA : Array classes
#include <LSharableArray.h>


// ===========================================================================
//		* DMSelection
// ===========================================================================
//
//	A DMSelection maintains a list of elements which are currently
//	selected. It is used by display objects to decide which elements
//	are drawn as selected and by the undo/redo subframework to indicate
//	which elements were affected by redoing or undoing a change to
//	the data model. It broadcasts messages (defined later in this header)
//	whenever an element is added to or removed from the selection list.
//
//	NOTE: The selection update message is defined at the end of this
//	header file.
//
// ===========================================================================

class DMSelection :	public DMElement,
					public LListener {

public:
							DMSelection();
							DMSelection(const DMSelection& inOriginal);
	virtual					~DMSelection();

	// object type information

	virtual DMElement*		Clone() const
									{ return new DMSelection(*this); }
	virtual DMElement*		ShallowClone() const
									{ return new DMSelection(*this); }
	virtual Boolean			IsDerivedFrom(FourCharCode inClassID) const
									{ return (inClassID == class_ID)
										  || (DMElement::IsDerivedFrom(inClassID)); }

	// selection changers
		
	virtual void			SelectNone();

	virtual void			SelectSimple(
									DMElement*			inElement);
	virtual void			SelectToggle(
									DMElement*			inElement);
	virtual void			SelectAdd(
									DMElement*			inElement);
	virtual void			SelectRemove(
									DMElement*			inElement);
	
	// selection accessors
	
	inline Boolean			IsSelected(DMElement* inElement) const
									{ return mSelectedElements.FetchIndexOf(&inElement) != 0; }
	inline LSharableArray&	GetSelectedElements()
									{ return mSelectedElements; }

	inline DMElement*		GetRootElement() const
									{ return mRootElement; }
	inline void				SetRootElement(DMElement* inElement)
									{ mRootElement = inElement; }

	// selection list manipulations
	
	virtual void			CopySelectionList(
									const DMSelection*	inSelection);
	virtual Boolean			AncestorIsSelected(
									DMElement*			inElement);

	virtual void			SortSelection();
	virtual void			EliminateRecursion();

	// command status
	
	virtual Boolean			SelectionIsNotEmpty();
	virtual Boolean			SelectionIsDeletable();

	// update notifications

protected:	
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);

	// selection primitives
	
	void					PrimListAdd(
									DMElement*			inElement);
	void					PrimListRemove(
									DMElement*			inElement);
	void					PrimListClear();


	// data members

protected:
	LSharableArray			mSelectedElements;			// list of selected elements
	DMElementPtr			mRootElement;				// root of this element hierarchy

};


// ===========================================================================
//		* Update messages
// ===========================================================================
//	A selection (DMSelection) is an LBroadcaster (via DMElement).
//	User interface objects which display selections should make themselves
//	listeners to the selection so they can be made aware of changes to
//	the selected elements list.

// ---------------------------------------------------------------------------
//	* Selection changed message:
//		This message is sent just after an element is added to or removed from
//		the selection.

const MessageT Selection_Changed = 'sel∆';
	// ioParam = DM_SelectionChanged*

struct DM_SelectionChanged {
	DMSelection*		mSelection;
	DMElement*			mElement;
	Boolean				mElementAdded;			// true if element was added, false if removed
};
