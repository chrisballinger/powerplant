// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEHierTable.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/05/96
//     $Date: 2006/04/12 08:48:13 $
//	$History: VEHierTable.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/17/96   Time: 15:35
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEHierTable.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Utilities
#include "DMEditAttachment.h"
#include "DMIterator.h"

	// Core : Editors : Views : User interface
#include "VEDrawingAgent.h"
#include "VEEditor.h"
#include "VEHierOutlineItem.h"
#include "VEMenuResources.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"

#include "UPropertyInspector.h"


// ===========================================================================

#pragma mark *** VEHierTable ***

// ---------------------------------------------------------------------------
//		* VEHierTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VEHierTable::VEHierTable(
	LStream*	inStream)

: OVTable(inStream, 18, false),
  mColumnKeys(sizeof (FourCharCode))

{

	// Establish item factory for table.

	SetItemFactory(new VEHierItemFactory);
	
	// Set edit strings.
	
	DMDropArea::SetEditMenuStrings(STR_VEEditStrings);

}


// ---------------------------------------------------------------------------
//		* ~VEHierTable
// ---------------------------------------------------------------------------
//	Destructor

VEHierTable::~VEHierTable()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu commands

// ---------------------------------------------------------------------------
//		* FindCommandStatus										[protected]
// ---------------------------------------------------------------------------
//	Overriden to disable the Show Hierarchy command.
//	This keeps Stefan happy. He thinks that since the window
//	is already open, you shouldn't offer to open it again.
//	I can't argue with that. :-)

void
VEHierTable::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{

	// Validate pointers.
	
	ValidateThis_();

	// Dispatch command.

	switch (inCommand) {
	
		// Disable the Show Hierarchy command, since this window is already displayed.
		//	This keeps Stefan happy. :-)
		
		case cmd_ShowHierarchy:
			outEnabled = false;
			break;
		
		// Disable Duplicate command if only the root object is selected.
		
		case cmd_Duplicate:
			ValidateObject_(mSelection.GetObject());

			SInt32 numObjects;
			numObjects = mSelection->GetSelectedElements().GetCount();
			if (mSelection->IsSelected(GetElement()))
				numObjects--;

			if (numObjects == 0) {
				outEnabled = false;
				break;
			}
			// break; 			// fall through
		
		// Otherwise see if OVTable wants it.
		
		default:
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
	
	}
}


// ---------------------------------------------------------------------------
//		* HandleKeyPress
// ---------------------------------------------------------------------------
//	Overridden to allow pressing of the return/enter key in the hierarchy
//	window table to open up the current selection (as it does everywhere
//	else in the application). - JCD 980529

Boolean
VEHierTable::HandleKeyPress(
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
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* InsertColumn
// ---------------------------------------------------------------------------
//	Add a column for an attribute.

void
VEHierTable::InsertColumn(
	TableIndexT		inAfterCol,
	FourCharCode	inAttributeKey,
	UInt16			inColumnWidth)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Add attribute key to local array.
	
	mColumnKeys.InsertItemsAt(1, inAfterCol + 1, &inAttributeKey);
	
	// Insert row.
	
	OVTable::InsertCols(1, inAfterCol, nil, 0, true);
	
	// Set column width.
	
	SetColWidth(inColumnWidth, inAfterCol + 1, inAfterCol + 1);
	
}


// ---------------------------------------------------------------------------
//		* RemoveCols
// ---------------------------------------------------------------------------
//	Overriden to remove the attribute key from the column list.

void
VEHierTable::RemoveCols(
	UInt32			inHowMany,
	TableIndexT		inFromCol,
	Boolean			inRefresh)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Remove attribute keys from array.
	
	mColumnKeys.RemoveItemsAt(inHowMany, inFromCol);
	
	// Remove items from the table.

	LOutlineTable::RemoveCols(inHowMany, inFromCol, inRefresh);

}


// ---------------------------------------------------------------------------
//		* GetAttributeKeyForColumn
// ---------------------------------------------------------------------------
//	Identify the attribute key that's drawn in a given column.

DM_AttributeKey
VEHierTable::GetAttributeKeyForColumn(
	TableIndexT		inColumn) const
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Get attribute key.
	
	DM_AttributeKey key = 0;
	mColumnKeys.FetchItemAt(inColumn, &key);
	
	// Return the key.
	
	return key;
	
}


// ---------------------------------------------------------------------------
//		* SetColWidth
// ---------------------------------------------------------------------------
//	Overridden to slide the title bar elements as menu frame moves.

void
VEHierTable::SetColWidth(
	UInt16			inWidth,
	TableIndexT		inFromCol,
	TableIndexT		inToCol)
{

	// Validate pointers. 

	ValidateThis_();
	ValidateObject_(mSuperView);

	// Resize the column.

	LTableView::SetColWidth(inWidth, inFromCol, inToCol);

	// Slide the title bar elements over.
	
	if (inFromCol == 1) {
		LPane* titleBar = mSuperView->GetSuperView()->FindPaneByID('TBAR');		//! TEMPORARY: magic number
		ValidateObject_(titleBar);
		titleBar->PlaceInSuperFrameAt(inWidth, 0, true);
	}
}

// ---------------------------------------------------------------------------
//		* 
// ---------------------------------------------------------------------------

void
VEHierTable::DoDuplicate()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mElement.GetObject());

	// Remove the root object from selection.
	
	mSelection->SelectRemove(mElement);
	
	// Now do the duplicate.

	OVTable::DoDuplicate();

}



// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Called by VEEditor::OpenHierWindowSelf to set up the selection for the
//	table.

void
VEHierTable::SetSelection(
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
					STR_VEEditStrings));		// inEditMenuStrings

}


// ===========================================================================

#pragma mark -
#pragma mark ** drag and drop/clipboard primitives

// ---------------------------------------------------------------------------
//		* HostDragElement										[protected]
// ---------------------------------------------------------------------------
//	If the object is moving within the same layout resource, we want
//	to make sure that it keeps its visual placement on the screen as
//	it moves up or down in the object hierarchy.

DMElement*
VEHierTable::HostDragElement(
	DMElement*			inElement,
	Rect&				inItemLocalBounds,
	DM_DragPlacement&	ioCurrentPlacement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);

	// See if we're placing a visual object.
	// If so, see where it exists.
	
	VEModelObject* originalVE = (dynamic_cast<VEModelObject*>(inElement));
	Rect originalBounds;
	Boolean originalBoundsValid = false;
	
	if (originalVE != nil) {
		ValidateObject_(originalVE);
		VEDrawingAgent* originalAgent = originalVE->FindDrawingAgent();
		if (originalAgent != nil) {
			ValidateObject_(originalAgent);
			originalBoundsValid = originalAgent->CalcPortFrameRect(originalBounds);
		}
	}
	
	// This code is to fix a bug where dragging an item downwards in the hierarchy
	// layout causes the item to be placed one item below where it should be.  It
	// works fine when moving an item upwards.
	ArrayIndexT origIndex = LArray::index_Bad;
	DMContainerMixin* mixin = dynamic_cast<DMContainerMixin*>(inElement->GetSuperElement());
	if (mixin != NULL)
	{
		DMFastIterator iter(mixin->GetSubElements());
		while (iter.NextElement())
		{
			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			if (objDM == inElement)
			{
				origIndex = iter.GetCurrentIndex();
				break;
			}
		}
	}
	if ((origIndex != LArray::index_Bad) and
		(origIndex < static_cast<SInt32>(ioCurrentPlacement.mInsertionIndex)))
	{
		// Check to see if we're within the same container or not.
		// Hmmm, I wonder if there's a better way to do this check...
		DMElement* super1 = NULL;
		DMElement* super2 = NULL;
		DMElement* super3 = NULL;
		super1 = inElement->GetSuperElement();
		if (super1 != NULL)
			super2 = super1->GetSuperElement();
		if (super2 != NULL)
			super3 = super2->GetSuperElement();
		if (super3 != NULL)
		{
			Assert_(ioCurrentPlacement.mHostElement->GetSuperElement());
			if (super3 == ioCurrentPlacement.mHostElement->GetSuperElement())
			{
				ioCurrentPlacement.mInsertionIndex--;
			}
		}
	}
	
	// Move or copy the element.

	DMElement* placedElement =
			DMDropArea::HostDragElement(inElement, inItemLocalBounds, ioCurrentPlacement);
	ValidateObject_(placedElement);

	// Adjust the screen position of the placed element
	// so it matches that of the original.

	VEModelObject* placedVE = (dynamic_cast<VEModelObject*>(placedElement));
	if (originalBoundsValid && (placedVE != nil)) {
	
		ValidateObject_(placedVE);
	
		VEDrawingAgent* placedAgent = originalVE->FindDrawingAgent();
		Rect placedBounds;

		if (placedAgent != nil) {
			ValidateObject_(placedAgent);
			if (placedAgent->CalcPortFrameRect(placedBounds)) {
				placedVE->MoveBy(originalBounds.left - placedBounds.left,
								 originalBounds.top - placedBounds.top);
			}
		}
	}
	
	// Return the placed element.
	
	return placedElement;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* MaximumIndentChanged									[protected]
// ---------------------------------------------------------------------------
//	Overridden to adjust the size of the first column for the depth
//	of the indentation. Behaves like the Finder's outline view.

void
VEHierTable::MaximumIndentChanged()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Adjust the size of the first column to fit this new indentation depth.
	
	SetColWidth(188 + (22 * mMaximumIndent), 1, 1);
	Refresh();

}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Call with the root element in the object hierarchy.
//	Causes the display objects to be built.

void
VEHierTable::SetElementSelf(
	DMElement*		inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Create outline items.

	CreateItemForElement(inElement, false, true);

}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Set up default columns.

void
VEHierTable::FinishCreateSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Let OVTable do its initialization.

	OVTable::FinishCreateSelf();
	
	// Set up default column 1.
	
	InsertCols(1, 0, nil, 0, false);
	SetColWidth(210, 1, 1);

	// Fill in the empty attribute key for column 1.

	FourCharCode code = 0;
	mColumnKeys.InsertItemsAt(1, 1, &code);
	
}


// ===========================================================================

#pragma mark -
#pragma mark *** VEHierItemFactory ***

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------
//	Create an outline item for visual objects.

OVItem*
VEHierItemFactory::CreateItemForElement(
	DMElement*	inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// This object should be a VEModelObject.
	
	if ((dynamic_cast<VEModelObject*>(inElement)) != nil)
		return new VEHierOutlineItem;
	else
		return nil;

}
