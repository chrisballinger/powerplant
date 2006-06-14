// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTOutlineTable.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/10/96
//     $Date: 2006/01/18 01:33:51 $
//  $History: CTOutlineTable.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CTOutlineTable.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMEditAttachment.h"

	// MacOS : Editors : Custom types
#include "CTModelObject.h"
#include "CTOutlineItem.h"
#include "CTTypeList.h"

#include "UPropertyInspector.h"


// ===========================================================================

#pragma mark *** CTOutlineTable ***

// ---------------------------------------------------------------------------
//		* CTOutlineTable()
// ---------------------------------------------------------------------------
//	Constructor

CTOutlineTable::CTOutlineTable(
	LStream*	inStream)

: OVTable(inStream, 15, false)

{

	// Change default indent size.

	mFirstLevelIndent = 24;

	// Establish item factory for table.

	SetItemFactory(new CTItemFactory);
	
	// Set edit strings.
	
	DMDropArea::SetEditMenuStrings(6000);

}


// ---------------------------------------------------------------------------
//		* ~CTOutlineTable
// ---------------------------------------------------------------------------
//	Destructor

CTOutlineTable::~CTOutlineTable()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu commands

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Enable the New ___ Item commands.

void
CTOutlineTable::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{

	switch (inCommand) {
	
		case 'intv':
		case 'pstr':
		case 'rgbc':
		case 'resl':
			ValidateObject_(mElement.GetObject());
			outEnabled = mElement->IsEnabled();
			break;

		default:
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
	
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Respond to the New ___ Item commands.

Boolean
CTOutlineTable::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	switch (inCommand) {
	
		case 'intv':
		case 'pstr':
		case 'rgbc':
		case 'resl':
			CreateNewItem(inCommand);
			break;
			
		default:
			return OVTable::ObeyCommand(inCommand, ioParam);
	
	}

	return true;

}


// ---------------------------------------------------------------------------
//		* CreateNewItem
// ---------------------------------------------------------------------------
//	Create a new attribute. The command number is the data model type of
//	of the attribute to create.

void
CTOutlineTable::CreateNewItem(
	CommandT	inCommand)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Clone the existing attribute item prototype.
	
	CTTypeList* typeList = CTTypeList::GetTypeList();
	ValidateObject_(typeList);

	CTModelObject* prototype = typeList->FindPrototype(inCommand);
	ValidateObject_(prototype);
	
	CTModelObject* itemCT = (CTModelObject*) prototype->Clone();
	ValidateObject_(itemCT);
	
	// Now add it to the data model. Find preferred location.

	DM_DragPlacement placement = { nil, 0, true };
	if (!CanHostElement(itemCT, &placement)) {
		SignalCStr_("Can't place new item.");
		return;
	}
	
	// Create transaction to record the change.
	
	StDMTransactionBuilder trans(this, mSelection, inCommand);
	
	// Empty selection.
	
	mSelection->SelectNone();

	// It is acceptable. Add it to the data model.

	HostElement(itemCT, &placement);
	mSelection->SelectAdd(itemCT);

	// Make sure it's visible.
	// JWW - ...but only if it's not in a collapsed pane (comes back nil)

	OVItem* item = FindItemForSubElement(itemCT, true);
	
	if (item != nil)
	{
		ValidateObject_(item);
		
		STableCell cell;
		cell.row = FindRowForItem(item);
		cell.col = 1;
		ScrollCellIntoFrame(cell);
	}

	// Commit the transaction.
	
	trans.Commit();

}

// ---------------------------------------------------------------------------
//		* HandleKeyPress
// ---------------------------------------------------------------------------
//	Overridden to allow pressing of the return/enter key in the hierarchy
//	window table to open up the current selection (as it does everywhere
//	else in the application). - JCD 980529

Boolean
CTOutlineTable::HandleKeyPress(
	const EventRecord &inKeyEvent )
{
	Boolean	keyHandled = false;


	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Dispatch key event.

	switch (inKeyEvent.message & charCodeMask) {
	
		case char_Enter:
		case char_Return:
		{
			UPropertyInspector::InspectSelection(mSelection, this);
			keyHandled = true;
		}
		break;
		
		default:
		{
			keyHandled = LCommander::HandleKeyPress(inKeyEvent);
		}
		break;
	}
	
	return keyHandled;
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Set the selection object for this table.

void
CTOutlineTable::SetSelection(
	DMSelection*	inSelection)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSelection);

	// Let outline table record selection.

	OVTable::SetSelection(inSelection);
	
	// Create edit-menu handler attachment.

	AddAttachment(new DMEditAttachment(
					this,						// inActionHost
					mSelection,					// inSelection
					6000));						// inEditMenuStrings

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Post-constructor initialization. Create default columns.

void
CTOutlineTable::FinishCreateSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Let OVTable do its initialization.

	OVTable::FinishCreateSelf();
	
	// Set up default columns.
	
	InsertCols(4, 0, nil, 0, false);
	SetColWidth(120, 1, 1);
	SetColWidth( 48, 2, 2);
	SetColWidth(144, 3, 3);
	SetColWidth(144, 4, 4);

}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Create outline items for the CTYP resource contents.

void
CTOutlineTable::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Create outline items.

	CreateItemForElement(inElement, false, true);

}


// ===========================================================================

#pragma mark -
#pragma mark *** CTItemFactory ***

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------
//	Create an outline item for each record in the CTYP resource.

OVItem*
CTItemFactory::CreateItemForElement(
	DMElement*	inElement) 
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// This object should be a CTModelObject.
	
	if ((dynamic_cast<CTModelObject*>(inElement)) != nil)
		return new CTOutlineItem;
	else
		return nil;

}
