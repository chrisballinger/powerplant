// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIInspectorTable.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/18/96
//     $Date: 2006/01/18 01:33:22 $
//	$History: PIInspectorTable.h $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:49
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Made SetFollowSelection public.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for multiple property inspectors. (Suggestion #1048.)
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/30/97   Time: 18:38
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed FindCommandStatus and ObeyCommand so they pass along the printing
//	commands. (Bug fix #1259.)
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:48
//	Updated in $/Constructor/Source files/Property inspector
//	Added ListenToMessage method. Now closes PI window if memory is low.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/26/96   Time: 09:42
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed a commenting error.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/25/96   Time: 19:07
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for inspected object label. Changed behavior when the
//	selection is empty: it now leaves the previously-inspected object in
//	the table.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 22:38
//	Updated in $/Constructor/Source files/Property inspector
//	Added SelectionChanged overload to avoid "hides inherited virtual
//	function" warning.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:35
//	Updated in $/Constructor/Source files/Property inspector
//	Updated for new undo/redo menu string architecture.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:36
//	Updated in $/Constructor/Source files/Property inspector
//	Bug fixes and standard sizing.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:08
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"
#include "OVItemFactory.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMListAttribute;
class LStaticText;
class LBevelButton;

// ===========================================================================
//		* PIInspectorTable
// ===========================================================================
//
//	PIInspectorTable is the outline table subclass that implements the
//	Constructor property editor.
//
// ===========================================================================

class PIInspectorTable : public OVTable {

public:
	enum { class_ID = 'INSP' };
	enum { object_ID = 'INSP' };

	static LView*			CreateFromStream(LStream* inStream)
									{ return new PIInspectorTable(inStream); }

							PIInspectorTable(LStream* inStream);
	virtual					~PIInspectorTable();

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
	virtual Boolean			HandleKeyPress(
									const EventRecord&	inKeyEvent);

	// undo/redo helpers

	virtual	void			PostAction(
									LAction*			inAction = nil);

	// custom resizing behaviors

	virtual void			CalcStandardSize(
									GReferencedWindow*	inWindow);
	virtual void			ResizeFrameBy(
									SInt16				inWidthDelta,
									SInt16				inHeightDelta,
									Boolean				inRefresh);
	
	// configuration

	virtual void			SetFollowSelection(
									DMSelection*		inFollowSelection,
									LCommander*			inFollowUndoHost);

	// configuration

protected:
	virtual void			SetElement(
									DMElement*			inElement);
	virtual void			SetElementSelf(
									DMElement*			inElement);

	// drawing behaviors

	virtual void			DrawSelf();
	virtual void			FinishCreateSelf();

	// dynamic items
	
	virtual Boolean			GetSelectedListItem(
									DMListAttribute*&	outListAttr,
									UInt32&				outInsertionIndex);

	virtual void			NewDynamicItem();
	virtual void			ClearDynamicItem();
	virtual void			DuplicateDynamicItem();
	
	// selection-following behaviors

	virtual void			SelectionChanged(
									DM_SelectionChanged* inMessage);
	virtual void			GoingAway(
									DMElement*			inElement);

	virtual void			SpendTime(
									const EventRecord&	inMacEvent);
	virtual void			SetElementFromSelection();
	
	virtual void			DetachFromSelection();
	
	// "hides inherited virtual function" warning...
	
	virtual void			SelectionChanged()
									{ LTableView::SelectionChanged(); }

	// drag-receive behaviors

	virtual Boolean			CanHostElement(
									DMElement*			inElement,
									DM_DragPlacement*	ioPlacement) const;
	virtual Boolean			DragIsAcceptable(
									DragReference		inDragRef);

	// low-memory handling

	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			FindUIObject(
									DM_FindUIObject*	inFindUIMsg);


	// data members

protected:
	DMSelectionPtr			mFollowSelection;
	LCommander*				mFollowUndoHost;
	LStaticText*					mInspectedObjectCaption;
	LBevelButton*				mDetachPIButton;
	Boolean					mSelectionChanged;
	Boolean					mIsDetached;

	friend class UPropertyInspector;

};


// ===========================================================================
//		* PIItemFactory
// ===========================================================================

class PIItemFactory : public OVItemFactory {

public:
							PIItemFactory() {}
	virtual					~PIItemFactory() {}

	virtual OVItem*			CreateItemForElement(
									DMElement*		inElement);

};