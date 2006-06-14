// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVTable.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/31/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: OVTable.h $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Rolled in an update from Clint toi make table activate/deactivate
//	correctly in multi-view windows.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/07/97   Time: 13:39
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Rolled in Clint's fixes 02/06/97.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/17/96   Time: 00:08
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added an override to RestorePlace to fix a bug in LPane::RestorePlace.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/04/96   Time: 13:38
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added ScrollElementIntoView method.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 19:20
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added duplicate command. Improved scrolling behavior (esp.
//	WRT dragging and selections).
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMDropArea.h"

	// Core : Display classes (Eric) : Outline table
#include "OVListener.h"

	// Core : Display classes (Eric) : Window classes
#include "GReferencedWindow.h"

	// PowerPlant : CA : Outline table
#include <LOutlineTable.h>


// ===========================================================================
//		* OVTable												[abstract]
// ===========================================================================
//
//	OVTable is the common base class for outline tables that display
//	data model hierarchies. It requires that all outline items displayed
//	within it be derived from OVItem.
//
// ===========================================================================

class OVTable :		public LOutlineTable,
					public LCommander,
					public OVListener,
					public DMDropArea,
					public GZoomer {

protected:
							OVTable(
									LStream*			inStream,
									SInt16				inRowHeight,
									Boolean				inAcceptReturnKey);
	virtual					~OVTable();

	// selection accessors
	
public:
	inline DMSelection*		GetSelection() const
									{ return mSelection; }
	virtual void			SetSelection(
									DMSelection*		inSelection);

	// subitem accessors

	virtual const LArray*	GetSubElementItems() const;

	virtual void			InsertItem(
									LOutlineItem*		inOutlineItem,
									LOutlineItem*		inSuperItem,
									LOutlineItem*		inAfterItem,
									Boolean				inRefresh = true,
									Boolean				inAdjustImageSize = true);
	virtual void			RemoveItem(
									LOutlineItem*		inOutlineItem,
									Boolean				inRefresh = true,
									Boolean				inAdjustImageSize = true);

	// mouse-down

	virtual void			Click(
									SMouseDownEvent&	inMouseDown);
	
	// target/activation

	virtual void			PutOnDuty( LCommander* /* inNewTarget */ )
									{ Activate(); }
	virtual void			TakeOffDuty()
									{ Deactivate();}

	// menu and key command behaviors
	
	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);
	virtual Boolean			ObeyCommand(
									CommandT			inCommand,
									void*				ioParam = nil);

	// clipboard behaviors
	
	virtual Boolean			CanPaste() const;
	virtual void			DoPaste();
	virtual void			DoDuplicate();
	
	// auto-scrolling
	
	virtual void			ScrollElementIntoView(
									const DMElement*	inElement,
									Boolean				inSelectCell = true);
	
	// temporary helper for pane-hosting geometry
	
	virtual void			SetCellPane(
									const STableCell&	inCell,
									LPane*				inPane);

	// drawing behaviors

	virtual void			ScrollImageBy(
									SInt32				inLeftDelta,
									SInt32				inTopDelta,
									Boolean				inRefresh);
	virtual void			RestorePlace(
									LStream*			inPlace);

	// window sizing

protected:
	virtual void			CalcStandardSize(
									GReferencedWindow*	inWindow);

	virtual void			FinishCreateSelf();

	// drag origination

	virtual void			TrackDrag(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown);

	virtual Boolean			MakeDragRegion(
									const DMElement*		inElement,
									RgnHandle				outGlobalDragRgn,
									Rect&					outGlobalItemBounds);
	virtual void			MakeSubItemDragRegion(
									OVItem*					inItem,
									RgnHandle				outGlobalDragRgn);
	

	virtual void			MakeDragImage(
									DragReference			inDragRef,
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									RgnHandle&				ioImageRgn,
									GWorldPtr&				ioImageGWorld);

	// drag receive

	virtual Boolean			ChoosePlacementForElement(
									DragReference			inDragRef,
									ItemReference			inItemRef,
									const DMElement*		inElement,
									DM_DragPlacement&		ioPlacement);

	virtual void			EnterDropArea(
									DragReference			inDragRef,
									Boolean					inDragHasLeftSender);
	virtual void			LeaveDropArea(
									DragReference			inDragRef);

	virtual void			InsideDropArea(
									DragReference			inDragRef);

	virtual void			DrawSelf();

	virtual void			DrawPlacementIndicator(
									const DM_DragPlacement&	inPlacement);
	virtual void			RefreshPlacementIndicator(
									const DM_DragPlacement&	inPlacement,
									Boolean					inRefreshTriangle,
									Boolean					inRefreshInsertionPoint);

	virtual Boolean			CalcPlacementDisplayLoc(
									const DM_DragPlacement&	inPlacement,
									Point&					outDisplayLoc);
	virtual RgnHandle		MakePlacementTriangleRgn(
									Point					inIndicatorPoint);

	// clipboard/drag-and-drop primitives

	virtual Boolean			CanHostElement(
									DMElement*				inElement,
									DM_DragPlacement*		ioPlacement) const;
	virtual void			HostElement(
									DMElement*				inElement,
									DM_DragPlacement*		ioPlacement);


	// data members

protected:
	DMSelectionPtr			mSelection;						// selection for the table (may be nil)
	DMSelectionPtr			mDragSelection;					// selection target while receiving drags
	DragReference			mCurrentDragRef;				// current drag-receive

	Boolean					mInsertionPointVisible;			// true if drag-receive insertion point is visible
	UInt32					mInsertionPointNextTick;		// time when insertion point will be toggled
	
};
