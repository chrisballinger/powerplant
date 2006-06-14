// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVListener.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 05/10/96
//     $Date: 2006/01/18 01:32:43 $
//	$History: OVListener.h $
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/03/97    Time: 5:12p
//	Updated in $/ConstructorWin/Includes
//	Added #include for proper generation of template code under MSVC++
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/28/97   Time: 19:59
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Moved multi-line text support to LOutlineItem.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/19/96   Time: 18:41
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added a fix that prevented disclosure triangle icons from being loaded
//	from user documents.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 19:12
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:44
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utility classes
#include "DMListener.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LOutlineItem;
class OVItemFactory;
class OVItem;
class OVTable;


// ===========================================================================
//		* OVListener										[abstract, mixin]
// ===========================================================================
//
//	OVListener is a mixin for the OVTable and OVItem classes.
//	It provides common code for catching item added and removed messages.
//
// ===========================================================================

class OVListener : public DMListener {

public:
							OVListener(OVItem* inThisAsItem);
	virtual					~OVListener();
	
	// display accessors
	
	inline OVItem*			GetOVItem() const
									{ return mOVItem; }
	inline OVTable*			GetOVTable() const
									{ return mOVTable; }
	void					SetOVTable(OVTable* inTable);

	// view creation factory accessors
	
	inline OVItemFactory*	GetItemFactory()
									{ return mItemFactory; }
	inline void				SetItemFactory(OVItemFactory* inItemFactory)
									{ mItemFactory = inItemFactory; }

	// element accessors
	
	inline DMElement*		GetElement() const
									{ return mElement; }
	virtual void			SetElement(DMElement* inElement);

	// container accessors
	
	inline DMElement*		GetContainer() const
									{ return mContainer; }
	virtual void			SetContainer(DMElement* inContainer);

	// subelement accessors

	virtual OVItem*			FindItemForSubElement(
									const DMElement*	inElement,
									Boolean				inRecursive) const;

	virtual const LArray*	GetSubElementItems() const = 0;
	virtual void			SetSubItemComparator(
									LComparator*		inComparator);

	// drag receive behaviors

	virtual Boolean			ChooseDragPlacementAsSub(
									DragReference		inDragRef,
									ItemReference		inItemRef,
									const DMElement*	inElement,
									DM_DragPlacement&	ioPlacement,
									const Point&		inDragMouseLocal);

	// update notifications

protected:
	virtual void			SetElementSelf(
									DMElement*			inElement);
	virtual void			GoingAway(
									DMElement*			inElement);
	virtual void			ContainerListChanged(
									DM_ListChange*		inMessage);

	// subitem creation

	virtual void			CreateItemsForSubElements();
	virtual void			CreateItemForElement(
									DMElement*			inElement,
									Boolean				inRefresh = true,
									Boolean				inAdjustImageSize = true);
	virtual OVItem*			CreateItemForElementSelf(
									DMElement*			inElement);
	virtual void			ChoosePlacementForNewItem(
									DMElement*			inNewElement,
									LOutlineItem*&		outSuperItem,
									LOutlineItem*&		outAfterItem);


	// data members

protected:
	OVTable*				mOVTable;				// table that's displaying us
	OVItem*					mOVItem;				// item that we're displaying
	TSharablePtr<OVItemFactory> mItemFactory;		// factory for creating outline items
	DMElementPtr			mElement;				// element that we're displaying
	DMElementPtr			mContainer;				// element whose subobjects we're displaying

	LSharableArray*			mSubElements;			// if sorting subelements, use this array to set placement

};

// Let's make MSVC++ happy!  MSVC++ is our friend!
#include "OVItemFactory.h"
// (MSVC++ wants to build the TSharablePtr template code in any .obj file
// that comes from a compilation unit that uses the template.  Since
// TSharablePtr<OVItemFactory> is used in this class, including OVItemFactory
// here will let MSVC++ build the template code rather than generate
// nasty compile errors.)
