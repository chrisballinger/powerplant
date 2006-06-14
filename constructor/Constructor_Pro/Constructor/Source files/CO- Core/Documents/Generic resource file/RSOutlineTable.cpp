// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSOutlineTable.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:33:00 $
//  $History: RSOutlineTable.cpp $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:29
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Added code to conform to Pilot's numbering scheme. (Each root form is
//	separated by 100 in ResIDT.)
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/05/97    Time: 4:13p
//	Updated in $/ConstructorWin/Sources
//	Minor fixes for MSVC build
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:44
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:27
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/21/97   Time: 15:01
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Rolled in Clint's fixes for MSL.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/22/96   Time: 15:23
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Improved error handling in case REEditor::OpenEditorWindow fails.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/21/96   Time: 15:00
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Fixed a bug in ChooseNewID that caused it to create an empty type
//	category. (Bug fix #1060.)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:54
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSOutlineTable.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Undo/redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMEditAttachment.h"
#include "DMIterator.h"

	// Core : Display classes (Eric) : Dialog handlers
#include "StKeyDialogHandler.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Documents : Generic resource file
#include "RSComparators.h"
#include "RSContainer.h"
#include "RSResource.h"
#include "RSResourceItem.h"
#include "RSType.h"
#include "RSTypeItem.h"

	// Core : Editors : Generic editor
#include "REEditor.h"
#include "RESession.h"
#include "RETypeEntry.h"
#include "RETypeList.h"
	
	// Core : Resource manager
#include "RMIdentifier.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Core : Utility classes
#include "UMemoryUtils.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFType.h"

	// PowerPlant : Table classes
#include <UTableHelpers.h>

#include <LPopupButton.h>
#include <LEditText.h>
#include <LStaticText.h>

	// MSL Headers
#include <new>

using namespace std;

// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_NewResourceDialog	= 15300;
const PaneIDT	Pane_ResourceType		= 'TYPE';
const PaneIDT	Pane_ResourceMaster		= 'MSTR';
const PaneIDT	Pane_ResID				= 'ID  ';
const PaneIDT	Pane_ResIDCaption		= 'IDCA';
const PaneIDT	Pane_ResName			= 'NAME';
const PaneIDT	Pane_Cancel				= 'CNCL';
const PaneIDT	Pane_Create				= 'OK  ';

const ResIDT	STR_RSEditMenuStrings	= 15100;

const ResIDT	STR_NewMenuStrings		= 15110;
const SInt16	str_NewGenericRsrc		= 1;
const SInt16	str_NewRsrcPrefix		= 2;
const SInt16	str_NewRsrcSuffix		= 3;
const SInt16	str_NewRsrcDialogSuffix	= 4;

const ResIDT	STR_ResourceCopyNames	= 15400;
const SInt16	str_UntitledResource	= 1;
const SInt16	str_CopyOfResource		= 2;
const SInt16	str_2ndCopyOfResource	= 3;


// ===========================================================================

#pragma mark *** RSOutlineTable ***

// ---------------------------------------------------------------------------
//		* RSOutlineTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

RSOutlineTable::RSOutlineTable(
	LStream*	inStream)

: OVTable(inStream, 18, false)

{
	SetItemFactory(new RSItemFactory);
	SetSubItemComparator(new RSResTypeComparator);
	mContainer = nil;
	mSession = nil;
	DMDropArea::SetEditMenuStrings(STR_RSEditMenuStrings);
}


// ---------------------------------------------------------------------------
//		* ~RSOutlineTable
// ---------------------------------------------------------------------------
//	Destructor

RSOutlineTable::~RSOutlineTable()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu command behaviors

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Overriden to enable the "Edit Resource" command.

void
RSOutlineTable::FindCommandStatus(
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

		// New item: Always enabled, unless file is locked.
		//	Menu item text changes based on current selection.
	
		case cmd_NewItem:
		case cmd_NewItemDialog:
		
			outEnabled = mContainer->IsEnabled();
			ResType selectedType;
			selectedType = GetSelectedResType();
			if (selectedType == 0) {
				
				// No type currently selected, do dialog.
				
				::GetIndString(outName, STR_NewMenuStrings, str_NewGenericRsrc);
			
			}
			else {
			
				// Only one resource type selected, make menu title reflect that type.
				
				RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(selectedType);
				ValidateObject_(typeEntry);
				
				LStr255 newRsrcString(STR_NewMenuStrings, str_NewRsrcPrefix);
				LStr255 resTypeName;
				typeEntry->GetResTypeName(resTypeName);
				
				newRsrcString += resTypeName;

				if ((typeEntry->CountDefaultResources() == 1) && (inCommand == cmd_NewItem))
					newRsrcString += LStr255(STR_NewMenuStrings, str_NewRsrcSuffix);
				else
					newRsrcString += LStr255(STR_NewMenuStrings, str_NewRsrcDialogSuffix);
					
				LString::CopyPStr(newRsrcString, outName);
				
			}
			break;
		
		// Duplicate: Enabled if resources are selected and file isn't locked.
		
		case cmd_Duplicate:
			if (!mContainer->IsEnabled())
				break;
			// otherwise, fall through
		
		// Edit item (and Duplicate): Enabled if resources are selected.
			
		case cmd_EditItem:
			outEnabled = false;
			if (mSelection != nil) {
				ValidateObject_(mSelection.GetObject());
				DMFastIterator iter(mSelection->GetSelectedElements());
				while (iter.NextElement()) {
					RSResource* res = (dynamic_cast<RSResource*>(iter.CurrentElement()));
					if (res != nil) {
						outEnabled = true;
						break;
					}
				}
			}
			break;
		
		default:
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
	
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Overriden to respond to the "Edit Resource" command.

Boolean
RSOutlineTable::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();

	// Dispatch command.

	switch (inCommand) {
	
		// Edit item leads to resource editors.
	
		case cmd_EditItem:
			EditSelection();
			return true;
		
		// New item: Create new resource (possibly showing dialog).
		
		case cmd_NewItem:
		case cmd_NewItemDialog:
			DoNewResource(inCommand == cmd_NewItemDialog);
			return true;
		
		default:
			return OVTable::ObeyCommand(inCommand, ioParam);
	
	}
}


// ---------------------------------------------------------------------------
//		* HandleKeyPress
// ---------------------------------------------------------------------------
// 	Overriden to map the return key to Edit Resource.

Boolean
RSOutlineTable::HandleKeyPress(
	const EventRecord&	inKeyEvent)
{

	// Validate pointers.

	ValidateThis_();

	// Test for return/enter key. Edit selected resources if so.

	UInt16 theKey = inKeyEvent.message;
	switch (theKey & charCodeMask) {

		case char_Enter:
		case char_Return:
			EditSelection();
			return true;

	}		

	// Wasn't return/enter, let superclass or supercommander handle it.

	return OVTable::HandleKeyPress(inKeyEvent);

}


// ---------------------------------------------------------------------------
//		* EditSelection
// ---------------------------------------------------------------------------
//	Open resource editors for all selected resources.

void
RSOutlineTable::EditSelection()
{

	// Validate pointers.

	ValidateThis_();

	// Sanity check: Make sure we have a session & selection.

	if ((mSession == nil) || (mSelection == nil))
		return;
	
	ValidateObject_(mSession);
	ValidateObject_(mSelection.GetObject());
	
	// Make a copy of selection, so it doesn't get bumped as windows get opened.
	
	DMSelectionPtr tempSelection;
	tempSelection = (DMSelection*) mSelection->Clone();
	ValidateObject_(tempSelection.GetObject());

	// Iterate through selected objects.
	
	DMFastIterator iter(tempSelection->GetSelectedElements());
	while (iter.NextElement()) {

		// See if this is in fact a resource. If not, ignore it.

		RSResource* resource = (dynamic_cast<RSResource*>(iter.CurrentElement()));
		if (resource != nil) {
			
			// Okay, it's a resource. Try to make an editor for it.

			ValidateObject_(resource);
			REEditor* editor = mSession->GetEditorForResource(resource->GetResType(), resource->GetResID(), true);

			if (editor != nil) {

				ValidateObject_(editor);

				// Got an editor, ask editor to make a window.
					
				try {
				
					LWindow* window = editor->OpenEditorWindow();
					if (window != nil) {
					
						// Got a window, bring window to front.
					
						ValidateObject_(window);
						window->Select();
	
					}
				}
				catch (const LException& err) {
					mSession->TearDownEditor(editor);
					if (err.GetErrorCode() == memFullErr)
						UMemoryUtils::ShowOutOfMemoryAlert();
					break;
				}
				catch (bad_alloc) {
					mSession->TearDownEditor(editor);
					UMemoryUtils::ShowOutOfMemoryAlert();
					break;
				}
				catch (...) {
					mSession->TearDownEditor(editor);
					break;
				}
			}
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection accessors

// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Call once before displaying this table to set the selection object
//	for this table.

void
RSOutlineTable::SetSelection(
	DMSelection*	inSelection)
{

	ValidateThis_();
	ValidateObject_(inSelection);

	OVTable::SetSelection(inSelection);

	AddAttachment(new DMEditAttachment(
					this,						// inActionHost
					mSelection,					// inSelection
					STR_RSEditMenuStrings));	// inEditMenuStrings
}


// ===========================================================================

#pragma mark -
#pragma mark ** custom resizing behaviors

// ---------------------------------------------------------------------------
//		* AdjustImageSize
// ---------------------------------------------------------------------------
//	Leave one extra pixel at the bottom of the image so the shaded bars look
//	natural.

void
RSOutlineTable::AdjustImageSize(
	Boolean		inRefresh)
{

	// Validate pointers.

	ValidateThis_();
	
	// See if it's okay to adjust image size. (It might not be OK if
	// we're adding lots of items at once.)
	
	if (mOkayToAdjustImageSize) {

		// Add one pixel to the default table size so that the
		// shaded bars look natural (i.e. they don't touch the
		// scroll bar when window is zoomed to standard size).

		UInt32 width, height;
		ValidateObject_(mTableGeometry);
		mTableGeometry->GetTableDimensions(width, height);
		ResizeImageTo(width, height + 1, inRefresh);

	}
}


// ---------------------------------------------------------------------------
//		* ResizeFrameBy
// ---------------------------------------------------------------------------
//	Resize column 1 (the resource name column) so that the image always
//	fills the frame.

void
RSOutlineTable::ResizeFrameBy(
	SInt16		inWidthDelta,
	SInt16		inHeightDelta,
	Boolean		inRefresh)
{

	// Validate pointers.

	ValidateThis_();

	// Resize the view.

	LView::ResizeFrameBy(inWidthDelta, inHeightDelta, inRefresh);
	
	// Stretch the name/ID column to fill space.
	
	if (inWidthDelta != 0)
		SetColWidth(GetColWidth(1) + inWidthDelta, 1, 1);

}


// ---------------------------------------------------------------------------
//		* CalcStandardSize
// ---------------------------------------------------------------------------
//	Always set the window size to 300 pixels wide. It just looks nice that way.

void
RSOutlineTable::CalcStandardSize(
	GReferencedWindow*	inWindow)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inWindow);

	// Find our image size.

	SDimension32 hierImageSize;
	GetImageSize(hierImageSize);
	hierImageSize.width = 300;

	// Calculate overscan (i.e. how far are we inside the window).
	
	SDimension16 overscanSize;
	SDimension16 windowFrameSize;
	inWindow->GetFrameSize(windowFrameSize);
	overscanSize.width = windowFrameSize.width - mFrameSize.width;
	overscanSize.height = windowFrameSize.height - mFrameSize.height;

	// Set window's size.

	SDimension16 windStandardSize;
	windStandardSize.width = ((hierImageSize.width) > 16000
								? 16000
								: hierImageSize.width) + overscanSize.width;
	windStandardSize.height = ((hierImageSize.height) > 16000
								? 16000
								: hierImageSize.height) + overscanSize.height;

	inWindow->SetStandardSize(windStandardSize);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard behaviors

// ---------------------------------------------------------------------------
//		* DoPaste
// ---------------------------------------------------------------------------
//	Overriden to ensure that resource ID conflict dialog is shown (if needed)
//	when pasting resources.

void
RSOutlineTable::DoPaste()
{

	// Validate pointers.

	ValidateThis_();
	
	// Select behavior for handling resource ID conflicts.
	
	RSType::SetResIDConflictResult(GetResIDsSignificant() ? icr_NotAsked : icr_Renumber);

	// Paste in the resources.

	OVTable::DoPaste();
	
}


// ---------------------------------------------------------------------------
//		* DoDuplicate
// ---------------------------------------------------------------------------
//	Duplicate all of the currently selected resources.

void
RSOutlineTable::DoDuplicate()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// Resolve resource ID conflicts (which *WILL* happen)
	// by renumbering resources.	
	
	RSType::SetResIDConflictResult(icr_Renumber);

	// Build a transaction for undo/redo.

	StDMTransactionBuilder trans(this, mSelection, cmd_Duplicate);

	// Keep a copy of selection so we know what to duplicate.
	// Then clear the selection so we can add the newly cloned items.

	DMSelectionPtr oldSelection = ((DMSelection*) mSelection->Clone());
	ValidateObject_(oldSelection.GetObject());
	mSelection->SelectNone();
	
	// Remember which is the first resource we're cloning.
	
	Boolean firstItem = true;
	
	// Iterate through each item. Clone those that we can clone.

	DMFastIterator iter(oldSelection->GetSelectedElements());
	while (iter.NextElement()) {
	
		// Make sure this is a resource item. Ignore it if not.
	
		RSResource* resource = (dynamic_cast<RSResource*>(iter.CurrentElement()));
		if (resource == nil)
			continue;
		ValidateObject_(resource);
		
		// Make a copy of the resource.
		
		DMElementPtr resCloneDM = resource->Clone();
		RSResource* resClone = (dynamic_cast<RSResource*>((DMElement*) resCloneDM));
		ValidateObject_(resClone);

		// Choose a reasonable name. We'll install the name later,
		// after the resource has been installed in the data model.

		LStr255 resName;
		resource->GetResName(resName);
		
		// Get the "copy" strings.

		StApplicationContext appContext;
		LStr255 copyOf(STR_ResourceCopyNames, str_CopyOfResource);			// " copy"
		LStr255 copyOf2(STR_ResourceCopyNames, str_2ndCopyOfResource);		// " copy "

		// Try to find those strings.

		SInt16 copyOfIndex = resName.ReverseFind(copyOf);
		SInt16 copyOf2Index = resName.ReverseFind(copyOf2);
		SInt16 truncIndex = resName.Length();
		SInt16 copyNum = 0;
		
		// If the strings exist, try to figure out *which* copy it is.
		
										//* 2.3d9: BUG FIX #807: added "copyOfIndex != 0"
										//		condition to prevent garbage chars from being added
		if ((copyOfIndex != 0) && (copyOfIndex == (resName.Length() - copyOf.Length() + 1))) {
			copyNum = 1;
			truncIndex = copyOfIndex - 1;
		}
		else if (copyOf2Index != 0) {
			SInt16 endOfCopy2Index = copyOf2Index + copyOf2.Length();
			LStr255 afterCopy2;
			afterCopy2.Assign(resName, endOfCopy2Index, resName.Length() - endOfCopy2Index + 1);
			copyNum = (SInt32) afterCopy2;
			if (copyNum)
				truncIndex = copyOf2Index - 1;
		}
		
		// Remove the old copy string and tack on the new one.
		
		resName[(UInt8)0] = truncIndex;
		copyNum++;
		
		if (copyNum == 1) {
			resName += copyOf;
		}
		else {
			resName += copyOf2;
			resName += LStr255((SInt32) copyNum);
		}
		
		// Install the new one in the data model. Scan up until we find
		// the res type entry.
		
		DMElement* super = resource->GetSuperElement();
		while (super != nil) {
			if ((dynamic_cast<RSType*>(super)) != nil)
				break;
			super = super->GetSuperElement();
		}
		if (super == nil)
			continue;
		
		// We've found the res type entry in the data model.
		// Install the copied resource there.
		
		DM_DragPlacement place = { super, 0, false };
		super->HostElement(resCloneDM, &place);

		// Now that resource is installed, we can change its name.
		
		resClone->SetResourceName(resName);
		
		// Select the new item. If it's the first item, scroll it into selection
		
		mSelection->SelectAdd(resCloneDM);
		if (firstItem) {
			ScrollElementIntoView(resCloneDM, false);
			firstItem = false;
		}
	}

	// Nothing has gone wrong. Finalize the transaction.

	trans.Commit();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag-and-drop behaviors

// ---------------------------------------------------------------------------
//		* DoDragReceive											[protected]
// ---------------------------------------------------------------------------
//	Overriden to ensure that resource ID conflict dialog appears (unless
//	resource IDs aren't significant for the target platform).

void
RSOutlineTable::DoDragReceive(
	DragReference	inDragRef)
{
	
	// Validate pointers.

	ValidateThis_();
	
	// If resource IDs are significant, we have to complain about
	// duplicate resource IDs.
	
	RSType::SetResIDConflictResult(GetResIDsSignificant() ? icr_NotAsked : icr_Renumber);

	// Do normal drag receive.

	OVTable::DoDragReceive(inDragRef);

}


// ===========================================================================

#pragma mark -
#pragma mark ** new resource dialog

// ---------------------------------------------------------------------------
//		* DoNewResource											[protected]
// ---------------------------------------------------------------------------
//	Create a new resource. Present the new resource dialog if the
//	selection is ambiguous (i.e. no resources are selected or multiple
//	resource types are selected), or the option key was held down.

void
RSOutlineTable::DoNewResource(
	Boolean		inMustShowDialog)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mContainer);
	
	// Make sure changes are allowed.
	
	if (!mContainer->IsEnabled())
		return;
	
	// Kill any pending transactions.
	
	LCommander::PostAnAction(nil);
	
	// Set up default new resource configuration.
	
	ResType		selectedType = GetSelectedResType();
	SInt16		selectedMasterID = 1;
	ResIDT		newResID = 128;
	LStr255		newResName(STR_ResourceCopyNames, str_UntitledResource);

	// See if we need to run new resource dialog.
	
	Boolean needsDialog = inMustShowDialog;
	
	if (selectedType == 0) {
	
		// No resources selected or multiple ResTypes selected.
		// Must show dialog.
	
		needsDialog = true;

	}
	else {
		
		// Only one resource type is selected, but if it has
		// multiple master resources, we run the dialog anyway.

		RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(selectedType);
		ValidateObject_(typeEntry);
		
		if (typeEntry->CountDefaultResources() != 1)
			needsDialog = true;
		
		// Choose a suitable default resource ID.
		
		newResID = ChooseNewResID(typeEntry);
	
	}

	// Show the dialog if needed.
	
	if (needsDialog) {
		if (!ShowNewResourceDialog(selectedType, selectedMasterID, newResID, newResName))
			return;
	}

	// Clear any existing transactions.
	
	LCommander::PostAnAction(nil);
	
	// Adjust the resource ID so it doesn't conflict with anything.

	RSType* type = mContainer->FindResType(selectedType, true);
	ValidateObject_(type);
	newResID = type->UniqueID(newResID);
	
	// Make sure the display for this type is expanded.
	
	OVItem* typeItem = FindItemForSubElement(type, true);
	ValidateObject_(typeItem);
	typeItem->Expand();

	// Create a transaction to undo new resource.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_NewItem);

	// Get information about this resource type.
	
	RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(selectedType);
	ValidateObject_(typeEntry);
	
	// Copy master resource suite.
	
	mContainer->CopyMasterResourceSet(typeEntry, selectedMasterID, RMIdentifier(newResID), newResName);
	
	// Select the newly created resource.
	
	RSResource* resource = mContainer->FindResource(typeEntry->GetPrimaryResType(), newResID, false);
	if (resource != nil) {
		ValidateObject_(resource);
		ScrollElementIntoView(resource, true);
	}
	
	// Commit the transaction.
	
	trans.Commit();

}


// ---------------------------------------------------------------------------
//		* ShowNewResourceDialog									[protected]
// ---------------------------------------------------------------------------
//	Run the dialog that offers to create a new resource.

Boolean
RSOutlineTable::ShowNewResourceDialog(
	ResType&	ioSelectedType,
	SInt16&		outSelectedMasterID,
	ResIDT&		ioNewResID,
	LStr255&	ioNewResName)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mContainer);

	// Get resource type list.
	
	RETypeList* typeList = RETypeList::GetTypeList();
	ValidateObject_(typeList);

	// Create the dialog object.
	
	StApplicationContext app;
	StKeyDialogHandler dialog(PPob_NewResourceDialog, this);

	// Find fields.

	LPopupButton* resTypePopup =
					(dynamic_cast<LPopupButton*>(dialog.GetDialog()->FindPaneByID(Pane_ResourceType)));
	ValidateObject_(resTypePopup);
	
	LPopupButton* resMasterPopup =
					(dynamic_cast<LPopupButton*>(dialog.GetDialog()->FindPaneByID(Pane_ResourceMaster)));
	ValidateObject_(resMasterPopup);
	
	LEditText* resNameField =
					(dynamic_cast<LEditText*>(dialog.GetDialog()->FindPaneByID(Pane_ResName)));
	ValidateObject_(resNameField);
	
	LEditText* resIDField =
					(dynamic_cast<LEditText*>(dialog.GetDialog()->FindPaneByID(Pane_ResID)));
	ValidateObject_(resIDField);

	LStaticText* resIDCaption =
					(dynamic_cast<LStaticText*>(dialog.GetDialog()->FindPaneByID(Pane_ResIDCaption)));
	ValidateObject_(resIDCaption);

	// If resource IDs aren't useful, hide the ID field.
	
	if (!mContainer->GetResIDsSignificant()) {
		resIDField->Hide();
		resIDCaption->Hide();
//		dialog.GetDialog()->ResizeFrameBy(-20, 0);
	}

	// Create resource type popup.
	
	LArray popupTypes(sizeof (ResType));
	SetupResTypePopup(resTypePopup, typeList, popupTypes, ioSelectedType);
	
	// Create resource master popup.
	
	SetupResMasterPopup(resMasterPopup, ioSelectedType);
	
	// Setup the resource name and ID.
	
	resIDField->SetValue(ChooseNewResID(typeList->FindResTypeEntry(ioSelectedType)));
	resNameField->SetDescriptor(ioNewResName);
	
	// Run the dialog.
	dialog.GetDialog()->ProcessCommand(cmd_PositionWindow);
	dialog.GetDialog()->Show();
	MessageT message = msg_Nothing;

	while (1) {

		message = dialog.DoDialog();
		
		if (message == msg_Cancel || message == msg_OK)
			break;
		
		if (message == Pane_ResourceType) {
			popupTypes.FetchItemAt(resTypePopup->GetValue(), &ioSelectedType);
			SetupResMasterPopup(resMasterPopup, ioSelectedType);
			resIDField->SetValue(ChooseNewResID(typeList->FindResTypeEntry(ioSelectedType)));
		}
	}

	dialog.GetDialog()->ProcessCommand(cmd_RecordPosition);
	if (message == msg_Cancel)
		return false;
	
	// Pull the information out of the dialog.

	outSelectedMasterID = resMasterPopup->GetValue();
	ioNewResID = resIDField->GetValue();
	resNameField->GetDescriptor(ioNewResName);
	
	return true;

}


// ---------------------------------------------------------------------------
//		* SetupResTypePopup										[protected]
// ---------------------------------------------------------------------------
//	Configure the resource type popup. Create one entry for each
//	type we know how to edit. Insert divider lines between groups of
//	resources (as identified by breaks of 100 or more in the sort
//	sequence).

void
RSOutlineTable::SetupResTypePopup(
	LPopupButton*		inPopup,
	RETypeList*		inTypeList,
	LArray&			ioPopupTypes,
	ResType&		ioSelectedType)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inPopup);
	ValidateObject_(inTypeList);
	
	// Get the popup's menu handle.
	
	MenuHandle theMenu = inPopup->GetMacMenuH();
#if PP_Target_Carbon
	ThrowIf_(theMenu == nil);
#else
	ValidateHandle_((Handle) theMenu);
#endif
	
	// Erase any menu items that may be there now.
	
	SInt16 menuItemCount = ::CountMenuItems(theMenu);
	while (menuItemCount--) {
		::DeleteMenuItem(theMenu, 1);
	}
	
	// Erase any entries in the menu type array.
	
	ioPopupTypes.RemoveItemsAt(ioPopupTypes.GetCount(), 1);

	// Now add menu items for each resource type.
	
	LFastArrayIterator typeIter(inTypeList->GetResTypeEntries());
	RETypeEntry* typeEntry;
	UInt32 previousSortSequence = 0;
	
	while (typeIter.Next(&typeEntry)) {
	
		// Validate pointer.
		
		ValidateObject_(typeEntry);
	
		// See if we need to insert a divider line. This is signalled by
		// a sort sequence in a different 100 range.
		
		UInt32 thisSortSequence = typeEntry->GetSortSequence();
		if ((previousSortSequence <= 0x20202020) && (previousSortSequence != 0)) {
		
			UInt32 prevSortGroup = previousSortSequence / 100;
			UInt32 thisSortGroup = thisSortSequence / 100;
			
			if (prevSortGroup != thisSortGroup) {
			
				// They're in different sort bins. Create a divider line.
				
				::AppendMenu(theMenu, (StringPtr)"\001-");

				ResType nullType = 0;
				ioPopupTypes.InsertItemsAt(1, LArray::index_Last, &nullType);
			
			}
		}

		// Get the name of this resource type.
		
		LStr255 resTypeName;
		typeEntry->GetResTypeName(resTypeName);
		
		// Create a menu item for it.
		
		::AppendMenu(theMenu, (StringPtr)"\001x");
		::SetMenuItemText(theMenu, ::CountMenuItems(theMenu), resTypeName);
		
		ResType theType = typeEntry->GetPrimaryResType();
		ioPopupTypes.AddItem(&theType);
		
		// If no type is currently selected, choose this one.
		
		if (ioSelectedType == 0)
			ioSelectedType = theType;
		
		// Record the sort sequence of this resource type.
		
		previousSortSequence = thisSortSequence;
	
	}
	
	// Reset the menu configuration.
	
	inPopup->SetMinValue(1);
	inPopup->SetMaxValue(::CountMenuItems(theMenu));
	inPopup->SetValue(ioPopupTypes.FetchIndexOf(&ioSelectedType));
	inPopup->Refresh();
	
}


// ---------------------------------------------------------------------------
//		* SetupResMasterPopup									[protected]
// ---------------------------------------------------------------------------
//	Set up the popup which lists the possible master resources for
//	a given resource type. Disables the popup if there is only one
//	choice.

void
RSOutlineTable::SetupResMasterPopup(
	LPopupButton*		inPopup,
	ResType			inSelectedType)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inPopup);
	
	// Find the resource type entry for this type.
	
	RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(inSelectedType);
	ValidateObject_(typeEntry);
	
	// Get the popup's menu handle.
	
	MenuHandle theMenu = inPopup->GetMacMenuH();
#if PP_Target_Carbon
	ThrowIf_(theMenu == nil);
#else
	ValidateHandle_((Handle) theMenu);
#endif
	
	// Erase any menu items that may be there now.
	
	SInt16 menuItemCount = ::CountMenuItems(theMenu);
	while (menuItemCount--) {
		::DeleteMenuItem(theMenu, 1);
	}

	// Now add menu items for each master resource.
	
	SInt16 masterResourceCount = typeEntry->CountDefaultResources();
	SInt16 masterResourceIndex = 0;
	
	while (++masterResourceIndex <= masterResourceCount) {
	
		// Get the name of this master resource.
		
		LStr255 resMasterName;
		typeEntry->GetIndexedDefaultResName(masterResourceIndex, resMasterName);
		
		// Create a menu item for it.
		
		::AppendMenu(theMenu, (StringPtr)"\001x");
		::SetMenuItemText(theMenu, ::CountMenuItems(theMenu), resMasterName);
	
	}
	
	// Reset the menu configuration.
	
	if (masterResourceCount < 2)
		inPopup->Disable();
	else
		inPopup->Enable();

	inPopup->SetMinValue(1);
	inPopup->SetMaxValue(masterResourceCount);
	inPopup->SetValue(1);
	inPopup->Refresh();

}

// ---------------------------------------------------------------------------
//		* ChooseNewResID										[protected]
// ---------------------------------------------------------------------------
//	Choose a default resource ID for a given resource type. The ID is based
//	on the default ID specified by the RSCP resource. It is then adapted
//	so that it doesn't conflict with any existing resources of that type and
//	also so that the ID is near any selected resource of the type.

ResIDT
RSOutlineTable::ChooseNewResID(
	RETypeEntry*	inTypeEntry)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inTypeEntry);
	ValidateObject_(mContainer);
	ValidateObject_(mSelection.GetObject());
	
	// Starting point for resource ID is specified by resource type entry.
	
	ResIDT defaultID = inTypeEntry->GetDefaultNewResID();
	
	// Walk selection list and see if there are any resources of this type selected.
	
	DMFastIterator iter(mSelection->GetSelectedElements());
	while (iter.NextElement()) {

		// Is this item a resource?
		
		RSResource* resource = (dynamic_cast<RSResource*>(iter.CurrentElement()));
		if (resource != nil) {
			
			// It is, see if it's the same type.

			ValidateObject_(resource);
			if (resource->GetResTypeEntry() != inTypeEntry)
				continue;
						
			// It is, set starting resource ID to one above this ID.

			ResIDT thisResID = resource->GetResID();
			if ((thisResID >= defaultID) && (thisResID != 32767))
#if Constructor_ForPilot
				if ((resource->GetResType() == 'tFRM')||(resource->GetResType() == 'PLob'))
					while (defaultID <= thisResID)
						defaultID += 100;  //pick next 100-even number above this one, to match the pilot Numbering convention
				else
					defaultID = thisResID + 1;
#else
				defaultID = thisResID + 1;
#endif
		}
	}

	// Now make sure we have a unique ID within this type.
	
	RSType* type = mContainer->FindResType(inTypeEntry->GetPrimaryResType(), false);
	if (type != nil) {
		ValidateObject_(type);
		defaultID = type->UniqueID(defaultID);
	}

	// Okay, we've got a unique ID that's based on the selection. Return it.
	
	return defaultID;
	
}


// ---------------------------------------------------------------------------
//		* GetSelectedResType									[protected]
// ---------------------------------------------------------------------------
//	See if there is a single resource type that is selected. This is used
//	as a starting point for the new resource dialog or menu command.

ResType
RSOutlineTable::GetSelectedResType() const
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Iterate over selected resource types.
	
	ResType selectedType = 0;
	DMFastIterator iter(mSelection->GetSelectedElements());

	while (iter.NextElement()) {

		// Is this item a resource?
		
		ResType thisResType = 0;
		
		RSResource* resource = (dynamic_cast<RSResource*>(iter.CurrentElement()));
		if (resource != nil) {
			ValidateObject_(resource);
			thisResType = resource->GetResType();
		}
		else {

			// ... or maybe it's a resource type?

			RSType* type = (dynamic_cast<RSType*>(iter.CurrentElement()));
			if (type != nil) {
				ValidateObject_(type);
				thisResType = type->GetPrimaryResType();
			}
		}
		
		// See if this type is the same as the other selected resources.

		if (selectedType == 0)
			selectedType = thisResType;
		else if (selectedType != thisResType)
			return 0;				// multiple types selected, no point in continuing
		
	}

	// Either nothing is selected or they're all the same type. Return it.
	
	return selectedType;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure the root element is an RSContainer.

void
RSOutlineTable::SetElementSelf(
	DMElement*	inElement)
{

	ValidateThis_();

	// Sanity check: Make sure we have a resource container.

	mContainer = (dynamic_cast<RSContainer*>(inElement));
	ValidateObject_(mContainer);
	
	// Okay, it's a container. Create type views for all of its resource types.

	SetContainer(mContainer->GetResourceTypeList());

}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Set up default columns.

void
RSOutlineTable::FinishCreateSelf()
{

	// Validate pointers.

	ValidateThis_();
	
	// Do inherited initialization.
	
	OVTable::FinishCreateSelf();
	
	// Set up default column widths.
	
	InsertCols(2, 0, nil, 0, false);
	SetColWidth(210, 1, 1);
	SetColWidth(70, 2, 2);

}


// ===========================================================================

#pragma mark -
#pragma mark *** RSItemFactory ***

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------

OVItem*
RSItemFactory::CreateItemForElement(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Create elements for RSResource or RSType.

	if ((dynamic_cast<RSResource*>(inElement)) != nil)
		return new RSResourceItem;
	if ((dynamic_cast<RSType*>(inElement)) != nil)
		return new RSTypeItem;

	return nil;
}
