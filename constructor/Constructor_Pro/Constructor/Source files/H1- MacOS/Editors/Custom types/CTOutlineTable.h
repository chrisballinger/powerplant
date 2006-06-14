// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTOutlineTable.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/10/96
//     $Date: 2006/01/18 01:33:51 $
//  $History: CTOutlineTable.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/17/96   Time: 11:14
//	Updated in $/Constructor/Source files/Editors/Custom types
//	Added support for resource ID attributes. (Bug fix #1141.)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:23
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItemFactory.h"
#include "OVTable.h"


// ===========================================================================
//		* CTOutlineTable
// ===========================================================================
//	Outline table displayed in the custom types editor.

class CTOutlineTable : public OVTable {

public:
	enum { class_ID = 'CTOV' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new CTOutlineTable(inStream); }

							CTOutlineTable(LStream* inStream);
	virtual					~CTOutlineTable();

	// menu commands

	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);
	virtual Boolean			ObeyCommand(
									CommandT			inCommand,
									void*				ioParam = nil);
	virtual void			CreateNewItem(
									CommandT			inCommand);

	virtual Boolean			HandleKeyPress(
									const EventRecord	&inKeyEvent);
	
	// initialization

	virtual void			SetSelection(
									DMSelection*		inSelection);

	// configuration

protected:
	virtual void			FinishCreateSelf();

	virtual void			SetElementSelf(
									DMElement*			inElement);

};


// ===========================================================================
//		* CTItemFactory
// ===========================================================================
//	Item factory for CTOutlineTable.

class CTItemFactory : public OVItemFactory {

public:
							CTItemFactory() {}
	virtual					~CTItemFactory() {}

	virtual OVItem*			CreateItemForElement(DMElement* inElement);

};
