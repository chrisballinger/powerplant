// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMDropArea.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/07/96
//	   $Date: 2006/01/18 01:32:40 $
//	$History: DMDropArea.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/14/96   Time: 16:51
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed a bug in SpendTime. It wasn't selecting the dropped items.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 18:54
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Added UnhiliteDropArea method. Improved commenting
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
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
//		* DMDropArea
// ===========================================================================
//
//	DMDropArea serves as a common base class for views which can
//	accept data model drags. It provides common behavior for converting
//	drag items to data model elements, auto-scrolling the target view,
//	and hilighting the target.
//
//	Since the Drag Manager won't allow dialogs to be shown from within
//	the drag-receive callback, DMDropArea automatically caches the
//	results of a drag until the next event loop. This allows warning
//	dialogs to be presented without any restrictions.
//
// ===========================================================================

class DMDropArea :	public LDragAndDrop,
					protected LPeriodical {

public:
							DMDropArea(LView* inThisAsView);
	virtual					~DMDropArea();

	// accessors
	
	inline DMSelection*		GetSelection() const
									{ return mSelection; }
	inline void				SetSelection(DMSelection* inSelection)
									{ mSelection = inSelection; }

	inline ResIDT			GetEditMenuStrings() const
									{ return mEditMenuStrings; }
	inline void				SetEditMenuStrings(ResIDT inEditMenuStrings)
									{ mEditMenuStrings = inEditMenuStrings; }

	// drag hit-testing behavior

protected:
	virtual Boolean			PointInDropArea(
									Point				inGlobalPt);

	virtual Boolean			DragIsAcceptable(
									DragReference		inDragRef);
	virtual Boolean			ItemIsAcceptable(
									DragReference		inDragRef,
									ItemReference		inItemRef);
	virtual Boolean			ChoosePlacementForElement(
									DragReference		inDragRef,
									ItemReference		inItemRef,
									const DMElement*	inElement,
									DM_DragPlacement&	ioCurrentPlacement) = 0;
	virtual Boolean			ElementIsAcceptable(
									const DMElement*	inElement);

	// drag target drawing

	virtual void			EnterDropArea(
									DragReference		inDragRef,
									Boolean				inDragHasLeftSender);
	virtual void			InsideDropArea(
									DragReference		inDragRef);
	virtual void			LeaveDropArea(
									DragReference		inDragRef);

	virtual void			HiliteDropArea(
									DragReference		inDragRef);
	virtual void			UnhiliteDropArea(
									DragReference		inDragRef);

	virtual void			SetDropCursor(
									DragReference		inDragRef);
	virtual void			AutoScrollDropArea(
									DragReference		inDragRef);

	// drag receive behaviors

	virtual void			DoDragReceive(
									DragReference		inDragRef);
	virtual void			ReceiveDragItem(
									DragReference		inDragRef,
									DragAttributes		inDragAttrs,
									ItemReference		inItemRef,
									Rect&				inItemBounds);
	virtual DMElement*		HostDragElement(
									DMElement*			inElement,
									Rect&				inItemLocalBounds,
									DM_DragPlacement&	ioCurrentPlacement);

	// delayed processing of drag receive

	virtual void			SpendTime(
									const EventRecord&	inMacEvent);

	
	// data members

protected:
	LView&					mThisAsView;				// the view we're mixed in with
	DragReference			mDragRef;					// current drag reference (if any)

	DM_DragPlacement		mCurrentPlacement;			// current placement for dragged items
	DM_DragPlacement		mPreviousPlacement;			// previous placement for dragged items
	DMSelectionPtr			mSelection;					// list of selected items
	ResIDT					mEditMenuStrings;			// resource containing strings for Edit menu actions

	LArray					mDragItemBounds;			// (of Rect) drop locations of dragged items
	LSharableArray			mDragItemElements;			// (of DMElement*) dragged items (cached for later processing)

};
