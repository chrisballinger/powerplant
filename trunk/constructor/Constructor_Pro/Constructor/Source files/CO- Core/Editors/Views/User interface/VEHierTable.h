// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEHierTable.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/05/96
//     $Date: 2006/01/18 01:33:17 $
//	$History: VEHierTable.h $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/17/96   Time: 15:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added code to disallow duplication of the root element in a view
//	hierarchy. (Bug fix #1056.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/08/96   Time: 20:52
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added support for moving title bar in menu editor window.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/21/96   Time: 12:41
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Hierarchy table now adjusts the size of the first column to fit the
//	maximum nesting depth in the table.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:28
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added support for dynamic columns in hierarchy window.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:33
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added SetSelection method.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItemFactory.h"
#include "OVTable.h"


// ===========================================================================
//		* VEHierTable
// ===========================================================================
//
//	VEHierTable is the class that displays the object hierarchy window for
//	all target frameworks. It can be customized by the code that creates
//	it, so that it displays an appropriate set of columns. Subclassing
//	VEHierTable or VEHierPaneItem should not be necessary.
//
// ===========================================================================

class VEHierTable : public OVTable {

public:
	enum { class_ID = 'VEHT' };
	
	static LPane*			CreateFromStream(LStream* inStream)
									{ return new VEHierTable(inStream); }

							VEHierTable(LStream* inStream);
	virtual					~VEHierTable();

	// menu commands
	
	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);

	virtual Boolean			HandleKeyPress(
									const EventRecord	&inKeyEvent);

	// dynamic columns
	
	virtual void			InsertColumn(
									TableIndexT			inAfterCol,
									FourCharCode		inAttributeKey,
									UInt16				inColumnWidth);
	virtual void			RemoveCols(
									UInt32				inHowMany,
									TableIndexT			inFromCol,
									Boolean				inRefresh);
	DM_AttributeKey			GetAttributeKeyForColumn(
									TableIndexT			inColumn) const;

	virtual void			SetColWidth(
									UInt16				inWidth,
									TableIndexT			inFromCol,
									TableIndexT			inToCol);

	// configuration

	virtual void			SetSelection(
									DMSelection*		inSelection);
	virtual void			DoDuplicate();



	// drag and drop/clipboard primitives

protected:
	virtual DMElement*		HostDragElement(
									DMElement*			inElement,
									Rect&				inItemLocalBounds,
									DM_DragPlacement&	ioCurrentPlacement);

	// drawing behaviors

	virtual void			MaximumIndentChanged();

	// initialization

	virtual void			SetElementSelf(
									DMElement*			inElement);
	virtual void			FinishCreateSelf();



	// data members

protected:
	LArray					mColumnKeys;				// attribute keys for each column

};


// ===========================================================================
//		* VEHierItemFactory
// ===========================================================================
//	Item factory that creates VEHierOutlineItem objects for each
//	visual object in the hierarchy.

class VEHierItemFactory : public OVItemFactory {

public:
							VEHierItemFactory() {}
	virtual					~VEHierItemFactory() {}

	virtual OVItem*			CreateItemForElement(DMElement* inElement);

};

