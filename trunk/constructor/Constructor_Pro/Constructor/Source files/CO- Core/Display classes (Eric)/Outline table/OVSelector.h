// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVSelector.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/21/96
//     $Date: 2006/01/18 01:32:44 $
//  $History: OVSelector.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:24
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 19:14
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

	// Core : Data model : Utilities
#include "DMDragSelector.h"
#include "DMListener.h"

	// PowerPlant : Table classes
#include <UTableHelpers.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class OVTable;


// ===========================================================================
//		* OVSelector
// ===========================================================================
//
//	OVSelector serves as a bridge between the LTableView selection model
//	(LTableSelector) and the data model selection model (DMSelection).
//	It also provides the marquee selection behavior.
//
// ===========================================================================

class OVSelector :	public LTableSelector,
					public DMDragSelector,
					public DMListener {

public:
							OVSelector(
									OVTable*				inOutlineTable,
									DMSelection*			inSelection);
	virtual					~OVSelector();

	// cell selection overrides

	virtual Boolean			CellIsSelected(
									const STableCell&		inCell) const;

	virtual STableCell		GetFirstSelectedCell() const;
	virtual TableIndexT		GetFirstSelectedRow() const;

	virtual void			SelectAllCells();
	virtual void			UnselectAllCells();

	virtual void			SelectCell(
									const STableCell&		inCell);
	virtual void			UnselectCell(
									const STableCell&		inCell);

	virtual void			ClickSelect(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown);

	// drag selection

	virtual Boolean			DragSelect(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown);
	virtual void			CheckElements(
									const Rect&				inOldMarqueeRect,
									const Rect&				inNewMarqueeRect);

	// update notifications

protected:
	virtual void			SelectionChanged(
									DM_SelectionChanged*	inMessage);

	// implementation helper

	DMElement*				FindElementForCell(
									const STableCell&		inCell) const;


	// data members

protected:
	OVTable*				mOutlineTable;				// table we're selecting for

};
