// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEOutlineTable.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/11/96
//	   $Date: 2006/01/18 01:33:11 $
//	$History: MEOutlineTable.h $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:23
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed a bug in HostDragElement that caused duplicate menus to be
//	inserted when dragging from the project windows. (Bug fix #1271.)
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/22/97   Time: 20:10
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed Constructor_ForJava #ifdefs.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/07/97   Time: 18:12
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Rolled in an update from Clint 01/06/97.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/08/96   Time: 20:27
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Added support for moving title bar in menu editor window.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/06/96   Time: 23:55
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:07
//	Updated in $/Constructor/Source files/Editors/Menus
//	Updated for new undo/redo menu string architecture.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/24/96   Time: 01:18
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Finished menu editor.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 20:56
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Baseline working version of menu editor.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:59
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItemFactory.h"
#include "OVTable.h"

	// Core : Data model : Utilities
#include "DMIterator.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class MEEditor;
class MEModelMenuBar;
class MEModelMenu;


// ===========================================================================
//		* Columns in menu outline table
// ===========================================================================

enum {
	MenuColumn_LeftGap = 1,				// gap to left of menu and menu frame
	MenuColumn_MarkChar,				// mark character
	MenuColumn_ItemText,				// menu item text
	MenuColumn_CloverChar,				// command key (cloverleaf character)
	MenuColumn_CmdKey,					// command key (key equivalent)
 	MenuColumn_RightGap,				// space to right of menu and menu frame
	MenuColumn_CmdID					// command ID
};


// ===========================================================================
//		* MEOutlineTable
// ===========================================================================
//
//	MEOutlineTable is the class that displays the simulated pull-down menu
//	for the menu editor.
//
// ===========================================================================

class MEOutlineTable : public OVTable {

public:
	enum { class_ID = 'MEOT' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new MEOutlineTable(inStream); }

							MEOutlineTable(LStream* inStream);
	virtual					~MEOutlineTable();

	// configuration
	
	inline void				SetMEEditor(MEEditor* inMEEditor)
									{ mMEEditor = inMEEditor; }

	// selection accessor

	virtual void			SetSelection(
									DMSelection*			inSelection);

	// menu commands

	virtual void			FindCommandStatus(
									CommandT				inCommand,
									Boolean&				outEnabled,
									Boolean&				outUsesMark,
									UInt16&					outMark,
									Str255					outName);
	virtual Boolean			ObeyCommand(
									CommandT				inCommand,
									void*					ioParam = nil);

	virtual void			CreateNewItem(CommandT			inCommand);
	virtual void			CreateNewMenu(CommandT			inCommand);

	// key-event processing

	virtual Boolean			ProcessKeyPress(
									const EventRecord&		inKeyEvent);

	virtual void			SelectLeftMenu();
	virtual void			SelectRightMenu();
	
	// drawing behaviors
	
	virtual void			AdjustImageSize(
									Boolean					inRefresh);
	virtual void			ScrollImageBy(
									SInt32					inLeftDelta,
									SInt32					inTopDelta,
									Boolean					inRefresh);
	virtual void			SetColWidth(
									UInt16					inWidth,
									TableIndexT				inFromCol,
									TableIndexT				inToCol);

protected:
	virtual void			DrawSelf();
	virtual void			DrawMenuAdornments();

	virtual void			ShowMenu(
									MEModelMenu*			inMenu);
	virtual void			UpdateLeftGap();

	// mouse-down behaviors
	
	virtual void			TrackEmptyClick(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									const SOutlineDrawContents&	inDrawContents);

	// configuration

	virtual void			FinishCreateSelf();
	virtual void			SetElementSelf(
									DMElement*				inElement);

	// drag receive

	virtual Boolean			ItemIsAcceptable(
									DragReference		inDragRef,
									ItemReference		inItemRef);

	virtual Boolean			ChoosePlacementForElement(
									DragReference			inDragRef,
									ItemReference			inItemRef,
									const DMElement*		inElement,
									DM_DragPlacement&		ioPlacement);
	virtual Boolean			ChoosePlacementInMenuBar(
									DragReference			inDragRef,
									ItemReference			inItemRef,
									const DMElement*		inElement,
									DM_DragPlacement&		ioPlacement,
									const SPoint32&			inImagePt);

	virtual DMElement*		HostDragElement(
									DMElement*				inElement,
									Rect&					inItemLocalBounds,
									DM_DragPlacement&		ioCurrentPlacement);

	virtual void			HiliteDropArea(
									DragReference			inDragRef);
	virtual void			UnhiliteDropArea(
									DragReference			inDragRef);
	virtual void			RefreshDropBorder();

	virtual void			DrawPlacementIndicator(
									const DM_DragPlacement&	inPlacement);
	virtual void			RefreshPlacementIndicator(
									const DM_DragPlacement&	inPlacement,
									Boolean					inRefreshTriangle,
									Boolean					inRefreshInsertionPoint);
	virtual Boolean			CalcPlacementDisplayLoc(
									const DM_DragPlacement&	inPlacement,
									Point&					outDisplayLoc);

	// menu adjusting

	virtual void			PutOnDuty( LCommander *inNewTarget );
	virtual void			TakeOffDuty();

	// data model updates

	virtual void			GoingAway(
									DMElement*				inElement);
	virtual void			ValueChanged(
									DM_ValueChange*			inMessage);
	virtual void			ContainerListChanged(
									DM_ListChange*			inMessage);
	virtual Boolean			CanHostElement(
									DMElement*				inElement,
									DM_DragPlacement*		ioPlacement) const;


	// data members

protected:
	MEModelMenuBar*			mMenuBar;					// menu bar data model object
	MEModelMenu*			mMenuHeader;				// menu header data model object
	MEEditor*				mMEEditor;					// menu editor object

};


// ===========================================================================
//		* MEItemFactory
// ===========================================================================

class MEItemFactory : public OVItemFactory {

public:
							MEItemFactory() {}
	virtual					~MEItemFactory() {}

	virtual OVItem*			CreateItemForElement(DMElement* inElement);

};


// ===========================================================================
//		* MEMenuBarIterator
// ===========================================================================
//	Helper class used by display objects to iterate through menu titles.

class MEMenuBarIterator : protected DMFastIterator {

public:
							MEMenuBarIterator(MEModelMenuBar* inMenuBar);
	virtual					~MEMenuBarIterator();

	// menu accessors
	
	Boolean					NextMenu();
	MEModelMenu*			CurrentMenu();
	
	// drawing accessors
	
	void					GetText(LStr255& outText);
	void					GetTextOrigin(Point& outTextOrigin);
	void					GetTextRect(Rect& outTextRect);
	
 
	// data members

protected:
	SInt16					mPosition;

};
