// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SLStringListTable.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/02/96
//     $Date: 2006/01/18 01:33:52 $
//	$History: SLStringListTable.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/28/97   Time: 20:03
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Added support for multi-line strings. (Bug fix #1230.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/17/96   Time: 16:40
//	Updated in $/Constructor/Source files/Editors/String list
//	Added code to make string cells more sensitive for in-place editing.
//	(Bug fix #1133.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/04/96   Time: 16:45
//	Updated in $/Constructor/Source files/Editors/String list
//	Fixed a commenting error.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/04/96   Time: 16:44
//	Created in $/Constructor/Source files/Editors/String list
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"
#include "OVItem.h"
#include "OVItemFactory.h"


// ===========================================================================
//		* Forward class definitions
// ===========================================================================

class DMStringAttribute;


// ===========================================================================
//		* SLStringListTable
// ===========================================================================
//
//	SLStringListTable presents the list of display classes known
//	to this view editor. It relies on information provided by a
//	VETypeList.
//
// ===========================================================================

class SLStringListTable : public OVTable {

public:
	enum { class_ID = 'SLST' };
	
	static LPane*			CreateFromStream(LStream* inStream)
									{ return new SLStringListTable(inStream); }

							SLStringListTable(LStream* inStream);
	virtual					~SLStringListTable();

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

	virtual void			NewStringItem();

	virtual void			DuplicateStrings();

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
//		* SLItemFactory
// ===========================================================================
//	SLItemFactory creates CLTypeEntryItems for each display class.

class SLItemFactory : public OVItemFactory {

public:
							SLItemFactory() {}
	virtual					~SLItemFactory() {}

	virtual OVItem*			CreateItemForElement(DMElement* inElement);

};


// ===========================================================================
//		* SLStringItem
// ===========================================================================
//	SLStringItem is the outline item for a single display class.

class SLStringItem : public OVItem {

public:
							SLStringItem();
	virtual					~SLStringItem();

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
	DMStringAttribute*		mStringAttr;

};
