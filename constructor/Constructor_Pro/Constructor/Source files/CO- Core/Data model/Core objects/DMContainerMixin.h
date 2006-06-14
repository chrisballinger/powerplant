// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMContainerMixin.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/26/96
//	   $Date: 2006/01/18 01:32:34 $
//	$History: DMContainerMixin.h $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/29/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed a dangling pointer reference in RemoveElement. (Bug fix #1236.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:21
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/01/96   Time: 01:51
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added CopySubElementsFrom method. Changed copy constructor to use it.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:06
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/14/96   Time: 16:08
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added accessors for mIsSubObjectList.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:46
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Replaced ItemControl and CanMakeChanges with Enabled flag.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMPointers.h"

	// PowerPlant : CA : Array classes
#include <LSharableArray.h>


// ===========================================================================
//		* Forward class definitions
// ===========================================================================

class DMAttribute;
class DMElement;
struct DM_DragPlacement;


// ===========================================================================
//		* DMContainerMixin									[mix-in class]
// ===========================================================================
//
//	DMContainerMixin is a mix-in class for DMElement objects which
//	can act as host for a list of other elements. It is not intended for
//	use outside of the data modelling framework, thus most methods are
//	declared protected.
//
//	NOTE: Several update messages are defined at the end of this header file.
//
// ===========================================================================

class DMContainerMixin {

protected:
							DMContainerMixin(
									DMElement&				inThisAsElement);
							DMContainerMixin(
									DMElement&				inThisAsElement,
									const DMContainerMixin&	inOriginal);
	virtual					~DMContainerMixin();

	// items list

public:
	virtual void			InsertElementAt(
									ArrayIndexT				inAtIndex,
									DMElement*				inElement,
									Boolean					inBeSuperElement);
	virtual void			RemoveElement(
									DMElement*				inElement);

	virtual void			CopySubElementsFrom(
									const DMElement*		inOriginal,
									Boolean					inCloneSubObjects);

	const LSharableArray&	GetSubElements() const
									{ return mSubElements; }

	// data streaming
	
	virtual void			ReadStreamData(
									LStream*				inStream,
									FourCharCode			inStreamSelector);
	virtual void			WriteStreamData(
									LStream*				inStream,
									FourCharCode			inStreamSelector) const;

	// update notifications/requests

	void					FinishCreate();

	void					GoingAway();
	void					CollectChanges();
	

	// attribute accessors
	
	DMAttribute*			FindAttributeByKey(
									FourCharCode			inKey) const;
	DMAttribute*			FindAttributeByTitle(
									ConstStringPtr			inTitle) const;

	// modified flag accessors

	Boolean					IsSubModified(
									Boolean					inTentative = false) const;
	void					ResetModified();


	// sub-object list flag accessors

	inline Boolean			IsSubObjectList() const
									{ return mIsSubObjectList; }
	inline void				SetSubObjectList(Boolean inSubObjectList)
									{ mIsSubObjectList = inSubObjectList; }

	// enabled flag implementation
	
	void					EnableSubs();
	void					DisableSubs();
	
	// data model changes

	Boolean					CanHostElement(
									const DMElement*		inNewElement,
									DM_DragPlacement*		ioPlacement);
	void					HostElementSelf(
									DMElement*				inNewElement,
									const DM_DragPlacement*	inPlacement);

	// update notifications
	
	void					SendListChangeMessage(
									DMElement*				inElement,
									Boolean					inElementAdded);

	// shallow cloning helper

	virtual void			SetShallowClone();


	// data members

protected:
	DMElement&				mThisAsElement;					// element we're mixed in with
	DMElementPtr			mOriginal;						// original container to copy
	
	LSharableArray			mSubElements;					// subelement list
	Boolean					mIsSubObjectList;				// true if subelements are "owned"
															//	by this container

	friend class DMListSubAction;

};


// ===========================================================================
//		* Update messages
// ===========================================================================
//	A container (DMContainer) is always mixed in with DMElement, which is an
//	LBroadcaster. User interface objects which act on this object should make
//	themselves listeners to the element so they can be made aware of changes
//	to the object's item list.

// ---------------------------------------------------------------------------
//	* Subitem list change message:
//		This message is sent in response to a FindCommandStatus request
//		for the Save command. All user interface objects should return
//		true in the mIsChanged member of this struct if there are changes
//		pending (such as a partially completed editfield entry).
//		These changes should NOT be recorded.

const MessageT Container_ListChange = 'lst∆';
	// ioParam = DM_ListChange*

struct DM_ListChange {
	DMElement*		mContainer;
	DMElement*		mMovedElement;
	Boolean			mElementAdded;			// false if removed
};
