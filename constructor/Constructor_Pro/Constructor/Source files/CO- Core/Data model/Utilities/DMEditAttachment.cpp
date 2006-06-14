// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMEditAttachment.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/04/96
//	   $Date: 2006/01/18 01:32:40 $
//	$History: DMEditAttachment.cpp $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 10  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:23
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Fixed CR/LF problem
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:12
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Disables commands when memory is low.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/13/96   Time: 17:52
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed a bug that allowed edit menu strings to be loaded from user
//	documents.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:30
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Updated for new undo/redo menu string architecture.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/02/96   Time: 15:08
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Added cmd_NewItemDialog menu command.
//
//	*****************  Version 4   *****************
//	User: scouten      Date: 10/23/96   Time: 18:56
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Fixed bugs in Copy and Paste.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/17/96   Time: 23:30
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 17:49
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Replaced DMElement::CanMakeChanges with IsEnabled.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "DMEditAttachment.h"

	// Headers
#include "Constructor.menu.h"

	// Data model : Core classes
#include "DMContainerMixin.h"
#include "DMSelection.h"

	// Data model : Undo/redo
#include "StDMTransactionBuilder.h"

	// Data model : Utility classes
#include "DMClipboard.h"
#include "DMIterator.h"

	// Display classes (Eric) : Property inspector
#include "UPropertyInspector.h"

	// Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Utility classes
#include "UMemoryUtils.h"


// ===========================================================================

#pragma mark *** DMEditAttachment ***

// ---------------------------------------------------------------------------
//		* DMEditAttachment(LCommander, ...)
// ---------------------------------------------------------------------------
//	Constructor

DMEditAttachment::DMEditAttachment(
	LCommander*		inActionHost,
	DMSelection*	inSelection,
	ResIDT			inEditMenuStrings)

: LAttachment(msg_AnyMessage)

{

	// Validate pointers.

	ValidateObject_(inActionHost);
	ValidateObject_(inSelection);
	
	// Store configuration information.
	
	mActionHost = inActionHost;
	mSelection = inSelection;
	mEditMenuStrings = inEditMenuStrings;

}


// ---------------------------------------------------------------------------
//		* ~DMEditAttachment
// ---------------------------------------------------------------------------
//	Destructor

DMEditAttachment::~DMEditAttachment()
{

	// Make sure property inspector doesn't keep a pointer to our selection.

	UPropertyInspector::SetTopSelection(nil, nil);

}


// ===========================================================================

#pragma mark -
#pragma mark ** event dispatching

// ---------------------------------------------------------------------------
//		* ExecuteSelf											[protected]
// ---------------------------------------------------------------------------
//	Catch find status, command, and key messages.

void
DMEditAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mActionHost);
	ValidateObject_(mSelection.GetObject());

	// Common initialization & sanity check.

	SetExecuteHost(true);
	
	// Dispatch event.
	
	switch (inMessage) {
	
		case msg_CommandStatus:
			FindCommandStatus((SCommandStatus*) ioParam);
			break;
		
		case cmd_Copy:
			CopySelection();
			SetExecuteHost(false);
			break;

		case cmd_Cut:
			CopySelection();
			DeleteSelection(cmd_Cut);
			SetExecuteHost(false);
			break;

		case cmd_Clear:
			DeleteSelection(cmd_Clear);
			SetExecuteHost(false);
			break;

		case cmd_PropertyInspector:
			UPropertyInspector::InspectSelection(mSelection, mActionHost);
			SetExecuteHost(false);
			break;

		case msg_KeyPress:
			HandleKeyEvent((EventRecord*) ioParam);
			break;
	}
}


// ---------------------------------------------------------------------------
//		* FindCommandStatus										[protected]
// ---------------------------------------------------------------------------
//	Enable, disable, and rename menu items as appopriate.

void
DMEditAttachment::FindCommandStatus(
	SCommandStatus*		inCommandStatus)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mActionHost);
	ValidateObject_(mSelection.GetObject());

	// Don't allow host commander to take command status, unless specifically allowed.

	SetExecuteHost(false);
	
	// Make sure menu title strings come from application's resource fork, not
	// any user document.
	
	StApplicationContext appContext;
	
	// Dispatch command.

	switch (inCommandStatus->command) {

		case cmd_Cut:
			UPropertyInspector::SetTopSelection(mSelection, mActionHost);
			*(inCommandStatus->enabled) =
					mSelection->SelectionIsDeletable()
					&& !UMemoryUtils::MemoryIsLow();
			::GetIndString(inCommandStatus->name,
							mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings,
							str_CutDefault);
			break;

		case cmd_Copy:
			*(inCommandStatus->enabled) =
					mSelection->SelectionIsNotEmpty()
					&& !UMemoryUtils::MemoryIsLow();
			::GetIndString(inCommandStatus->name,
							mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings,
							str_CopyDefault);
			break;
		
		case cmd_Paste:
			SetExecuteHost(!UMemoryUtils::MemoryIsLow());
			::GetIndString(inCommandStatus->name,
							mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings,
							str_PasteDefault);
			break;

		case cmd_Clear:
			*(inCommandStatus->enabled) =
					mSelection->SelectionIsDeletable()
					&& !UMemoryUtils::MemoryIsLow();
			::GetIndString(inCommandStatus->name,
							mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings,
							str_ClearDefault);
			break;
		
		case cmd_Duplicate:
			if (mSelection->SelectionIsNotEmpty()
			  && mSelection->GetRootElement()->IsEnabled()
			  && !UMemoryUtils::MemoryIsLow())
			  	SetExecuteHost(true);

			::GetIndString(inCommandStatus->name,
							mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings,
							str_Duplicate);
			break;
		
		case cmd_NewItem:
		case cmd_NewItemDialog:
			SetExecuteHost(!UMemoryUtils::MemoryIsLow());
			::GetIndString(inCommandStatus->name,
							mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings,
							str_NewItem);
			break;
			
		case cmd_EditItem:
			if (mSelection->SelectionIsNotEmpty())
				SetExecuteHost(!UMemoryUtils::MemoryIsLow());

			::GetIndString(inCommandStatus->name,
							mEditMenuStrings ? mEditMenuStrings : STR_DefaultEditStrings,
							str_EditItem);
			break;

		case cmd_PropertyInspector:
			*(inCommandStatus->enabled) = !UMemoryUtils::MemoryIsLow();
			*(inCommandStatus->usesMark) = true;
			*(inCommandStatus->mark) = 0;
			break;

		default:
			SetExecuteHost(true);
	}
}


// ---------------------------------------------------------------------------
//		* HandleKeyEvent										[protected]
// ---------------------------------------------------------------------------
//	If the backspace/delete key is pressed, delete the currently
//	selected elements.

void
DMEditAttachment::HandleKeyEvent(
	const EventRecord*	inEvent)
{

	// Validate pointers.

	ValidateThis_();

	// See if this is a backspace key. If so, delete the selected elements.

	SInt16 theKey = inEvent->message & charCodeMask;
	if (theKey == char_Backspace) {
	
		// It is backspace. Get the menu command text for backspace so we can
		// have a proper title for undo/redo.

		DeleteSelection(cmd_Clear);
		SetExecuteHost(false);
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard primitives

// ---------------------------------------------------------------------------
//		* CopySelection											[protected]
// ---------------------------------------------------------------------------
//	Copy the selected items into our private scrap.

void
DMEditAttachment::CopySelection()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mActionHost);
	ValidateObject_(mSelection.GetObject());
	
	// Sanity check: Make sure there's a data model clipboard.
	
	DMClipboard* clip = DMClipboard::GetDMClipboard();
	ValidateObject_(clip);
	
	// Clear the clipboard.
	
	clip->ClearDMScrap();
	
	// Okay, we can copy items.
	// Request a non-recursive selection list.

	DMSelectionPtr copyList;
	copyList = (DMSelection*) mSelection->Clone();
	ValidateObject_(copyList.GetObject());
	
	copyList->EliminateRecursion();
	copyList->SortSelection();
	
	// Clone each item and add it to the clipboard.
	
	DMFastIterator iter(copyList->GetSelectedElements());
	while (iter.NextElement()) {

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);

		DMElementPtr clone = element->Clone();
		ValidateObject_(clone.GetObject());

		clip->AddDMScrapItem(clone, false);

	}
}


// ---------------------------------------------------------------------------
//		* DeleteSelection										[protected]
// ---------------------------------------------------------------------------
//	Delete the seleced items. If a string is passed in, it is used as the
//	undo/redo menu item title.

void
DMEditAttachment::DeleteSelection(
	CommandT	inMenuCommand)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mActionHost);
	ValidateObject_(mSelection.GetObject());

	// Build a transaction to make this change.

	StDMTransactionBuilder trans(mActionHost, mSelection, inMenuCommand);

	// Iterate over the selected elements.

	DMIterator iter(mSelection->GetSelectedElements());
	while (iter.NextElement()) {

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		
		// Make sure the element is eligible for deletion.
		// If so, tell its container to take it out.

		if (element->CanDeleteThis()) {

			DMElement* super = element->GetSuperElement();
			ValidateObject_(super);

			DMContainerMixin* superCM = (dynamic_cast<DMContainerMixin*>(super));
			ValidateObject_(superCM);

			superCM->RemoveElement(element);

		}
	}

	// Looks like we succeeded. Commit the transaction.

	trans.Commit();

}
