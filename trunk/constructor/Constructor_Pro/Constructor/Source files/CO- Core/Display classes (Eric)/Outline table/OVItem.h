// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVItem.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 05/31/96
//     $Date: 2006/01/18 01:32:43 $
//	$History: OVItem.h $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/28/97   Time: 19:58
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Moved multi-line text support to LOutlineItem.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/12/96   Time: 17:35
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added CollectTentativeChanges method so Save is enabled when IPE is
//	changing. (Bug fix #1075.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:16
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added override of Expand which collapses the item if an exception is
//	thrown.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/19/96   Time: 15:38
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed a bug in StartInPlaceEdit. If the window was brought to front by
//	editing, the edit field wasn't installed in the correct location in the
//	command hierarchy.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 19:12
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added RefreshRow and RefreshCell methods. Improvements to in-place editing.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/16/96   Time: 23:35
//	Checked in '$/Constructor/Source files/Display classes (Eric)/Outline table'
//	Comment: Added double-click as a shortcut for property inspector.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:44
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVListener.h"

	// PowerPlant : CA : Outline table
#include <LEditableOutlineItem.h>


// ===========================================================================
//		* OVItem												[abstract]
// ===========================================================================
//
//	OVItem is a base class for outline table items that display data
//	model objects. It provides common functionality for ensuring that
//	the display and data model stay in sync.
//
// ===========================================================================

class OVItem :	public LEditableOutlineItem,
				public OVListener {

public:
							OVItem();
	virtual					~OVItem();

	// disclosure triangle

	virtual Boolean			CanExpand() const;
	virtual void			Expand();

	// modified flag
	
	virtual Boolean			IsModified() const;

	// subelement accessors

	virtual const LArray*	GetSubElementItems() const;

	// attribute accessors
	
	virtual TableIndexT		GetCellForAttribute(
									const DMAttribute*	inAttribute) const;
	virtual DMAttribute*	GetAttributeForCell(
									TableIndexT			inColumn) const;
	virtual DM_AttributeKey	GetAttributeKeyForCell(
									TableIndexT			inColumn) const;

	// drawing behaviors
	
	virtual void			RefreshRow();
	virtual void			RefreshCell(TableIndexT		inColumn);
	
	virtual void			MakeDragRegion(
									const STableCell&	inCell,
									RgnHandle			outGlobalDragRgn,
									Rect&				outGlobalItemBounds);

	// in-place editing

	virtual void			StartInPlaceEdit(
									const STableCell&	inCell);

	// update notifications
	
protected:
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			ValueChanged(
									DM_ValueChange*		inMessage);
	virtual void			ModifiedFlagChanged(
									DMElement*			inElement);
	virtual void			ContainerListChanged(
									DM_ListChange*		inMessage);
	virtual void			CollectTentativeChanges(
									DM_CollectTentativeChanges* inMessage);

	// subitem list

	virtual void			ExpandSelf();
	virtual void			AddSubItem(
									LOutlineItem*		inSubItem,
									LOutlineItem*		inAfterItem);
	virtual void			RemoveSubItem(
									LOutlineItem*		inSubItem);

	// property inspector

	virtual void			DoubleClick(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									const SOutlineDrawContents&	inDrawContents,
									Boolean					inHitText);

	// in-place editing

	virtual void			IPEChanged();

	virtual void			PrepareDrawContents(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);

	// drag-receive behaviors

	virtual Boolean			ChooseDragPlacement(
									DragReference		inDragRef,
									ItemReference		inItemRef,
									const DMElement*	inElement,
									DM_DragPlacement&	ioPlacement,
									const Point&		inDragMouseLocal,
									const STableCell&	inCell);

	// modified flag drawing

	virtual void			DrawRowAdornments(
									const Rect&			inLocalRowRect);

	virtual void			DrawModifiedFlag();
	virtual void			RefreshModifiedFlag();
	
	virtual Boolean			CalcModifiedFlagRect(
									Rect&				outModFlagRect);


	enum { msg_IPEChanged = 'ipe∆' };

	friend class OVTable;

};
