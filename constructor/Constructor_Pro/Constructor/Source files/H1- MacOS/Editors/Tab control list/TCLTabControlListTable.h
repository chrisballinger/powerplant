// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLTabControlListTable.h	© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================


#pragma once

#include "OVTable.h"
#include "OVItem.h"
#include "OVItemFactory.h"
#include "TCLModelTabInfo.h"


// ===========================================================================
//		* Forward class definitions
// ===========================================================================

class DMStringAttribute;
class DMIntegerAttribute;


// ===========================================================================
//		* TCLTabControlListTable
// ===========================================================================
//
//	TCLTabControlListTable presents the list of display classes known
//	to this view editor. It relies on information provided by a
//	VETypeList.
//
// ===========================================================================

class TCLTabControlListTable : public OVTable {

public:
	enum { class_ID = 'TCLT' };
	
	static LPane*			CreateFromStream(LStream* inStream)
									{ return new TCLTabControlListTable(inStream); }

							TCLTabControlListTable(LStream* inStream);
	virtual					~TCLTabControlListTable();

	// menu command status

	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);
	virtual Boolean			ObeyCommand(
									CommandT			inCommand,
									void*				ioParam = nil);

	virtual void			NewTabInfoItem();

	// configuration
	
	virtual void			SetSelection(
									DMSelection*		inSelection);

	// internal configuration

protected:
	virtual void			FinishCreateSelf();

	virtual void			SetElementSelf(
									DMElement*			inElement);
	
	
	// custom resizing behaviors

	virtual void			ResizeFrameBy(
									SInt16				inWidthDelta,
									SInt16				inHeightDelta,
									Boolean				inRefresh);

};


// ===========================================================================
//		* TCLItemFactory
// ===========================================================================
//	TCLItemFactory creates CLTypeEntryItems for each display class.

class TCLItemFactory : public OVItemFactory {

public:
							TCLItemFactory() {}
	virtual					~TCLItemFactory() {}

	virtual OVItem*			CreateItemForElement(DMElement* inElement);

};


// ===========================================================================
//		* TCLTabInfoItem
// ===========================================================================
//	TCLTabInfoItem is the outline item for a single display class.

class TCLTabInfoItem : public OVItem {

public:
							TCLTabInfoItem();
	virtual					~TCLTabInfoItem();

	// attribute accessors

	virtual DM_AttributeKey	GetAttributeKeyForCell(
									TableIndexT					inColumn) const;

	// configuration

protected:
	virtual void			SetElementSelf(
									DMElement*					inElement);
	// drawing
	
	virtual void			GetDrawContentsSelf(
									const STableCell&			inCell,
									SOutlineDrawContents&		ioDrawContents);

	// mouse-down behaviors

	virtual void			SingleClick(
									const STableCell&			inCell,
									const SMouseDownEvent&		inMouseDown,
									const SOutlineDrawContents&	inDrawContents,
									Boolean						inHitText);

	virtual void			TrackEmptyClick(
									const STableCell&			inCell,
									const SMouseDownEvent&		inMouseDown,
									const SOutlineDrawContents&	inDrawContents);
	virtual void			UpdateRowSize();

	virtual void			ValueChanged(
									DM_ValueChange*		inMessage);


	// data members

protected:
	TCLModelTabInfo*		mTabInfo;
};
