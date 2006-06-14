// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VELayoutView.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/17/96
//     $Date: 2006/04/12 08:48:13 $
//	$History: VELayoutView.cpp $
//	
//	*****************  Version 30  *****************
//	User: scouten      Date: 3/05/97    Time: 10:50a
//	Updated in $/ConstructorWin/Core/Source
//	Shielded access to RF* classes behind Constructor_UseRF switch.
//	
//	*****************  Version 29  *****************
//	User: scouten      QDate: 02/28/97   Time: 10:23
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Added SuperActivate and SuperDeactivate overrides to ensure that
//	classes that redraw themselves on (de)activate events draw properly.
//	
//	*****************  Version 28  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 27  *****************
//	User: scouten      QDate: 02/18/97   Time: 16:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed DrawSelection so that it normalizes the pen state before
//	redrawing.
//	
//	*****************  Version 26  *****************
//	User: scouten      QDate: 02/07/97   Time: 14:08
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Rolled in Clint's fixes 02/07/97.
//	
//	*****************  Version 25  *****************
//	User: scouten      QDate: 02/07/97   Time: 13:59
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Rolled in Clint's fixes 02/06/97.
//	
//	*****************  Version 24  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 23  *****************
//	User: scouten      QDate: 01/31/97   Time: 10:11
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Enabled printing of views. (Bug fix #1204.)
//	
//	*****************  Version 22  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 21  *****************
//	User: scouten      QDate: 01/21/97   Time: 15:02
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Added some code to HostDragElement to reduce flicker. (Stil needs
//	work.)
//	
//	*****************  Version 20  *****************
//	User: scouten      QDate: 12/22/96   Time: 11:58
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug in BuildObjectDisplay that prevent icons from displaying in
//	GA classes. (Bug fix #1127.)
//	
//	*****************  Version 19  *****************
//	User: scouten      QDate: 12/21/96   Time: 17:41
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed problems with shift-click and command-click. (Bug fix #1140 and
//	#1145.)
//	
//	*****************  Version 18  *****************
//	User: scouten      QDate: 12/17/96   Time: 14:25
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added code to BuildObjectDisplay to keep document's resource fork on
//	top during construction and FinishCreate.
//	
//	*****************  Version 17  *****************
//	User: scouten      QDate: 12/16/96   Time: 14:16
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Changed color of grid lines to be more visible on AGA gray background.
//	(Bug fix #1030.)
//	
//	*****************  Version 16  *****************
//	User: scouten      QDate: 12/16/96   Time: 13:16
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Bracketed iterator in DragSelectedObjects to prevent problems when
//	dragging objects to trash. (Bug fix #1118.)
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 12/07/96   Time: 11:23
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Updated toRobin's drop 12/06/96.
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 12/06/96   Time: 17:33
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed MoveSelectionBy and ResizeSelectionBy to eliminate recursion
//	before performing their operations.
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 12/06/96   Time: 17:23
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed SelectAll so it does not select the root object.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 12/06/96   Time: 16:06
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Updated to use undo/redo strings when moving or resizing objects.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 12/04/96   Time: 20:43
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added code to ClickSelf to give precedence to currently-selected
//	objects only if the option key is held down on the mouse-down.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 11/24/96   Time: 22:27
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug in ClickSelf. Command-click now properly selects the object
//	behind the current selection.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:29
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Modified HitTestObject so that it tests the window frame and selects
//	the root object when hit.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/19/96   Time: 14:34
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug in BuildObjectDisplay.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/17/96   Time: 21:45
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Changed references to VEBasicBackdrop to VEBackdrop. (The classes were
//	merged.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:37
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Tweaked refactoring of layout view.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 11/02/96   Time: 18:11
//	Checked in '$/Constructor/Source files/Editors/Views/+ Generic view editor'
//	Comment: Updated for new undo/redo menu string architecture.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 23:28
//	Checked in '$/Constructor/Source files/Editors/Views/+ Generic view editor'
//	Comment: Added support for drag-to-trash.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 17:48
//	Checked in '$/Constructor/Source files/Editors/Views/+ Generic view editor'
//	Comment: Replaced DMElement::CanMakeChanges with IsEnabled.
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

#include "VELayoutView.h"

	// Core : Application
#include "CAPreferencesFile.h"

	// Core : Data model : Core classes
#include "DMSelection.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Data model : Undo/redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMClipboard.h"
#include "DMDragTask.h"
#include "DMEditAttachment.h"
#include "DMIterator.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"

	// Core : Editors : Views : User interface
#include "VEBackdrop.h"
#include "VEDrawingAgent.h"
#include "VEEditor.h"
#include "VEMenuResources.h"
#include "VEResizeReceiver.h"

	// Core : Property inspector
#include "UPropertyInspector.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// MacOS : Resource manager
#if Constructor_UseRF
	#include "RFMap.h"
#endif

	// PowerPlant : Utilities
#include <UModalDialogs.h>
#include <UResourceMgr.h>

	// PowerPlant : CA : Utilities
#include <UPrintingUtils.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>
#include <LEditText.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

SShowHideInfo	VELayoutView::sShowHideInfo	=
					{ false,						// mShowGrid
					  true,							// mShowPaneAdornments
					  true,							// mShowPaneEdges
					  true,							// mShowPaneIDs
					  true,							//  (unused)
					  false,						//  (unused)
					  false,						//  (unused)
					  false };						// mShowActualWindow

SGridInfo		VELayoutView::sGridInfo		= { false, {8, 8} };
Boolean			VELayoutView::sPrefsLoaded	= false;
Boolean			VELayoutView::sPrefsChanged	= false;
RFMap*			VELayoutView::sCurrentMap	= nil;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_GridDialog				= 2170;
const PaneIDT	Pane_HorizontalGrid			= 'hGRD';
const PaneIDT	Pane_VerticalGrid			= 'vGRD';

const ResIDT	STR_ResizeStrings			= 2009;
const SInt16	str_MoveObjects				= 1;
const SInt16	str_ResizeObjects			= 2;

const ResIDT	STR_DisplayMenuHideStrings	= 2020;
const ResIDT	STR_DisplayMenuShowStrings	= 2021;
const SInt16	sSnapToGrid					= 1;
const SInt16	sShowPaneEdges				= 2;
const SInt16	sShowPaneIDs				= 3;
const SInt16	sShowInvisiblePanes			= 4;
const SInt16	sShowGrid					= 5;
const SInt16	sShowActualWindow			= 6;
const SInt16	sShowToolPalette			= 7;


// ===========================================================================

#pragma mark *** VELayoutView ***

// ---------------------------------------------------------------------------
//		* VELayoutView(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VELayoutView::VELayoutView(
	LStream*	inStream)

: LView(inStream),
  DMDropArea(this),
  DMDragSelector(*this, nil)

{

	// Clear pointers.
	
	mLayoutBackdrop = nil;
	mRFMap = nil;
	mRootObjectMoves = false;  //CCP 2/7/97 
	
	SetRefreshAllWhenResized(false);

	// Make sure we get control of menu/keyboard.

	if (mSuperCommander != nil) {
		ValidateObject_(mSuperCommander);
		mSuperCommander->SetLatentSub(this);
	}

	// Load global prefs if not already done.
	
	if (!sPrefsLoaded) {
		StPreferencesContext prefsContext;
		if (prefsContext.IsValid()) {
			StResource showHidePrefsR(pref_ResourceType, pref_ShowHide, false);
			Handle showHidePrefsH = (Handle) showHidePrefsR;
			
			if (showHidePrefsH != nil) {
				ValidateHandle_(showHidePrefsH);
				sShowHideInfo = **((SShowHideInfo**) showHidePrefsH);
			}

			StResource gridPrefsR(pref_ResourceType, pref_Grid, false);
			Handle gridPrefsH = (Handle) gridPrefsR;
			
			if (gridPrefsH != nil) {
				ValidateHandle_(gridPrefsH);
				sGridInfo = **((SGridInfo**) gridPrefsH);
			}
		}
		sPrefsLoaded = true;
	}
	
	// Copy global prefs to local.
	
	mShowHideInfo = sShowHideInfo;
	mGridInfo = sGridInfo;

	// Set edit menu strings.
	
	DMDropArea::SetEditMenuStrings(STR_VEEditStrings);

}


// ---------------------------------------------------------------------------
//		* ~VELayoutView
// ---------------------------------------------------------------------------
//	Destructor

VELayoutView::~VELayoutView()
{

	// Release on-screen pane representation.

	if (mRootObject != nil) {
		ValidateObject_(mRootObject.GetObject());
		DeleteObjectDisplay(mRootObject, false);
	}

	// Save preferences.
	
	if (sPrefsChanged) {
		StPreferencesContext prefsContext;
		if (prefsContext.IsValid()) {
			StNewResource showHideInfoR(pref_ResourceType, pref_ShowHide, sizeof (SShowHideInfo));
			Handle showHideInfoH = (Handle) showHideInfoR;
			ValidateHandle_(showHideInfoH);
			**((SShowHideInfo**) showHideInfoH) = sShowHideInfo;

			StNewResource gridInfoR(pref_ResourceType, pref_Grid, sizeof (SGridInfo));
			Handle gridInfoH = (Handle) gridInfoR;
			ValidateHandle_(gridInfoH);
			**((SGridInfo**) gridInfoH) = sGridInfo;
		}
		sPrefsChanged = false;
	}
	
	// Get rid of the backdrop object.

	if (mLayoutBackdrop != nil) {
		ValidateObject_(mLayoutBackdrop);
		delete mLayoutBackdrop;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu and key commands

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Enable several view-related commands.

void
VELayoutView::FindCommandStatus(
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
	
		case cmd_Paste:
			outEnabled = CanPaste();
			break;
	
		case cmd_SelectAll:
			outEnabled = true;
			break;
	
		case cmd_SnapToGrid: 
			{
				StApplicationContext appContext;
				::GetIndString(outName,
						mGridInfo.mSnapToGrid ? STR_DisplayMenuShowStrings : STR_DisplayMenuHideStrings,
						sSnapToGrid);
				outEnabled = true;
				outUsesMark = false;
			}
			break;
		
		case cmd_EditGrid:
			outEnabled = true;
			break;

		case cmd_ShowPaneEdges:
			{
				StApplicationContext appContext;
				::GetIndString(outName,
						mShowHideInfo.mShowPaneEdges ? STR_DisplayMenuShowStrings : STR_DisplayMenuHideStrings,
						sShowPaneEdges);
				outEnabled = true;
				outUsesMark = false;
			}
			break;
			
		case cmd_ShowPaneIDs:
			{
				StApplicationContext appContext;
				::GetIndString(outName,
						mShowHideInfo.mShowPaneAdornments ? STR_DisplayMenuShowStrings : STR_DisplayMenuHideStrings,
						sShowPaneIDs);
				outEnabled = true;
				outUsesMark = false;
			}
			break;
			
		case cmd_ShowInvisiblePanes:
			{
				StApplicationContext appContext;
				::GetIndString(outName,
						mShowHideInfo.mShowInvisiblePanes ? STR_DisplayMenuShowStrings : STR_DisplayMenuHideStrings,
						sShowInvisiblePanes);
				outEnabled = true;
				outUsesMark = false;
			}
			break;
			
		case cmd_ShowGrid:
			{
				StApplicationContext appContext;
				::GetIndString(outName,
						mShowHideInfo.mShowGrid ? STR_DisplayMenuShowStrings : STR_DisplayMenuHideStrings,
						sShowGrid);
				outEnabled = true;
				outUsesMark = false;
			}
			break;
			
		default:
			LCommander::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
			
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------

Boolean
VELayoutView::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Dispatch command.

	switch (inCommand) {
		
		case cmd_Paste:
			DoPaste();
			return true;

		case cmd_SelectAll:
			mSelection->SelectNone();
			SelectAll(mRootObject);
			break;

		case cmd_EditGrid:
			EditGridDialog();
			break;

		case cmd_SnapToGrid:
			mGridInfo.mSnapToGrid = sGridInfo.mSnapToGrid = !mGridInfo.mSnapToGrid;
			sPrefsChanged = true;
			break;

		case cmd_ShowPaneEdges:
			mShowHideInfo.mShowPaneEdges = sShowHideInfo.mShowPaneEdges = !mShowHideInfo.mShowPaneEdges;
			sPrefsChanged = true;
			RefreshRootObject();
			break;

 		case cmd_ShowPaneIDs:
			mShowHideInfo.mShowPaneAdornments = sShowHideInfo.mShowPaneAdornments = !mShowHideInfo.mShowPaneAdornments;
			sPrefsChanged = true;
			RefreshRootObject();
			break;

 		case cmd_ShowInvisiblePanes:
			mShowHideInfo.mShowInvisiblePanes = sShowHideInfo.mShowInvisiblePanes = !mShowHideInfo.mShowInvisiblePanes;
			sPrefsChanged = true;
			RefreshRootObject();
			break;

 		case cmd_ShowGrid:
			mShowHideInfo.mShowGrid = sShowHideInfo.mShowGrid = !mShowHideInfo.mShowGrid;
			sPrefsChanged = true;
			RefreshRootObject();
			break;

		default:
			return LCommander::ObeyCommand(inCommand, ioParam);
	}
	
	return true;
}


// ---------------------------------------------------------------------------
//		* HandleKeyPress
// ---------------------------------------------------------------------------
//	Enable arrow keys for moving or resizing, and enter/return for
//	opening property inspector.

Boolean
VELayoutView::HandleKeyPress(
	const EventRecord&	inKeyEvent)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Dispatch key event.

	switch (inKeyEvent.message & charCodeMask) {
	
		case char_Enter:
		case char_Return:
			UPropertyInspector::InspectSelection(mSelection, this);
			return true;

		case char_LeftArrow:
			if (inKeyEvent.modifiers & shiftKey)
				ResizeSelectionBy(-1, 0);
			else
				MoveSelectionBy(-1, 0);
			UpdatePort();						//* 2.2d9: BUG FIX #544: force update
			return true;
			
		case char_RightArrow:
			if (inKeyEvent.modifiers & shiftKey)
				ResizeSelectionBy(1, 0);
			else
				MoveSelectionBy(1, 0);
			UpdatePort();						//* 2.2d9: BUG FIX #544: force update
			return true;
			
		case char_UpArrow:
			if (inKeyEvent.modifiers & shiftKey)
				ResizeSelectionBy(0, -1);
			else
				MoveSelectionBy(0, -1);
			UpdatePort();						//* 2.2d9: BUG FIX #544: force update
			return true;
			
		case char_DownArrow:
			if (inKeyEvent.modifiers & shiftKey)
				ResizeSelectionBy(0, 1);
			else
				MoveSelectionBy(0, 1);
			UpdatePort();						//* 2.2d9: BUG FIX #544: force update
			return true;

		default:
			return LCommander::HandleKeyPress(inKeyEvent);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard beheaviors

// ---------------------------------------------------------------------------
//		* CanPaste
// ---------------------------------------------------------------------------
//	Return true if any of the items in the clipboard can
//	be pasted into this layout.

Boolean
VELayoutView::CanPaste() const
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());
	
	// Sanity check: Make sure there's a data model clipboard.
	
	DMClipboard* clip = DMClipboard::GetDMClipboard();
	if (clip == nil)
		return false;
	ValidateObject_(clip);

	// Get the list of clipboard items.
	
	DMFastIterator iter(clip->GetDMScrap());
	DM_DragPlacement placement = { nil, 0, true };

	while (iter.NextElement()) {
		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		if (mRootObject->CanHostElement(element, &placement))
			return true;
	}

	return false;
}


// ---------------------------------------------------------------------------
//		* DoPaste
// ---------------------------------------------------------------------------
//	Called when the user selects the Paste command. Pastes the
//	items in the clipboard into the layout at an arbitrary location
//	near the currently selected items (if possible).

void
VELayoutView::DoPaste()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mRootObject.GetObject());

	// Sanity check: Make sure there's a data model clipboard (and a selection).
	
	DMClipboard* clip = DMClipboard::GetDMClipboard();
	if (clip == nil)
		return;
	ValidateObject_(clip);

	// Scan through existing selection looking for upper left corner.
	
	DMIterator iter(mSelection->GetSelectedElements());
	DMElement* element;
	Boolean hasNoLocation = true;
	Point selectionUpperLeft = { 0, 0 };
	
	while (iter.NextElement()) {
		VEModelObject* objVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		if (objVE == nil)
			continue;
		ValidateObject_(objVE);
		
		VEDrawingAgent* agent = objVE->FindDrawingAgent();
		if (agent == nil)
			continue;
		ValidateObject_(agent);
		
		Rect itemFrame;
		if (agent->CalcPortFrameRect(itemFrame)) {
			if (hasNoLocation) {
				selectionUpperLeft = topLeft(itemFrame);
				hasNoLocation = false;
			}
			else {
				if (itemFrame.top < selectionUpperLeft.v)
					selectionUpperLeft.v = itemFrame.top;
				if (itemFrame.left < selectionUpperLeft.h)
					selectionUpperLeft.h = itemFrame.left;
			}
		}
	}

	// Start a transaction.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_Paste);

	// Get the list of clipboard items.
	
	mSelection->SelectNone();

	DMFastIterator scrapIter(clip->GetDMScrap());
	while (scrapIter.NextElement()) {
		element = scrapIter.CurrentElement();
		ValidateObject_(element);
		DM_DragPlacement placement = { nil, 0, true };
		
		if (mRootObject->CanHostElement(element, &placement)) {
			DMElementPtr clone = element->Clone();
			ValidateObject_(clone.GetObject());
			placement.mHostElement->HostElement(clone, &placement);
			mSelection->SelectAdd(clone);
		}
	}

	// Find upper left corner of new selection.
	
	iter.ResetTo(LArrayIterator::from_Start);
	Boolean pasteHasNoLocation = true;
	Point pasteSelectionUpperLeft = { 0, 0 };
	
	while (iter.NextElement()) {
		VEModelObject* objVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		if (objVE == nil)
			continue;
		ValidateObject_(objVE);
		
		VEDrawingAgent* agent = objVE->FindDrawingAgent();
		if (agent == nil)
			continue;
		ValidateObject_(agent);
		
		Rect itemFrame;
		if (agent->CalcPortFrameRect(itemFrame)) {
			if (pasteHasNoLocation) {
				pasteSelectionUpperLeft = topLeft(itemFrame);
				pasteHasNoLocation = false;
			}
			else {
				if (itemFrame.top < pasteSelectionUpperLeft.v)
					pasteSelectionUpperLeft.v = itemFrame.top;
				if (itemFrame.left < pasteSelectionUpperLeft.h)
					pasteSelectionUpperLeft.h = itemFrame.left;
			}
		}
	}

	// Move selected panes so that they're not directly on current selection
	// which is often the original that's been copied.

	if (!(hasNoLocation || pasteHasNoLocation)) {
		Point offset = { selectionUpperLeft.v - pasteSelectionUpperLeft.v + 5,
						 selectionUpperLeft.h - pasteSelectionUpperLeft.h + 5 };
	
		iter.ResetTo(LArrayIterator::from_Start);
		while (iter.NextElement()) {
			VEModelObject* objVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
			ValidateObject_(objVE);
			objVE->MoveBy(offset.h, offset.v);
		}
	}

	// Commit transaction.

	trans.Commit();

}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* SetRootObject
// ---------------------------------------------------------------------------
//	Configure the layout view to display this object and all of its
//	subobjects.

void
VELayoutView::SetRootObject(
	VEModelObject*	inRootObject)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inRootObject);
	
	// Remember new root object.

	mRootObject = inRootObject;

	// Set up initial display.

	if (mRootObject != nil) {
		mRootObject->AddListener(this);
		SetLayoutBackdrop(new VEBackdrop);
		BuildObjectDisplay(mRootObject, true);
	}
}


// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Set the selection object for this layout.

void
VELayoutView::SetSelection(
	DMSelection*	inSelection)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSelection);

	// Remember selection pointer.

	mSelection = inSelection;
	DMDragSelector::mSelection = inSelection;
	DMDropArea::SetSelection(inSelection);

	// Create edit menu attachment.

	AddAttachment(new DMEditAttachment(
					this,						// inActionHost
					mSelection,					// inSelection
					STR_VEEditStrings));		// inEditMenuStrings

}


// ---------------------------------------------------------------------------
//		* SetLayoutBackdrop
// ---------------------------------------------------------------------------
//	Change the layout backdrop helper.

void
VELayoutView::SetLayoutBackdrop(
	VEBackdrop*		inBackdrop)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inBackdrop);

	// Switch backdrops.

	if (mLayoutBackdrop != nil) {
		ValidateObject_(mLayoutBackdrop);
		delete mLayoutBackdrop;
	}
	mLayoutBackdrop = inBackdrop;

	// Adjust location of backdrop.
	
	mLayoutBackdrop->PutInside(this, true);
	mLayoutBackdrop->FinishCreate();
	ExpandSubPane(mLayoutBackdrop, true, true);
	
	SBooleanRect bindings = { true, true, true, true };
	mLayoutBackdrop->SetFrameBinding(bindings);
	if (mRootObject != nil)
		mLayoutBackdrop->SetRootObject(mRootObject);

	// Make sure the base pane is in the correct location.
	
	CheckRootObjectPosition();
	
	// Redraw the entire layout view.
	
	Refresh();

}


// ===========================================================================

#pragma mark -
#pragma mark ** window sizing

// ---------------------------------------------------------------------------
//		* AdjustImageSize
// ---------------------------------------------------------------------------
//	Recalculate the image size for the layout view.

void
VELayoutView::AdjustImageSize()
{

	// Validate pointers.
	
	ValidateThis_();

	// First make sure we have a display pane for the root object.
	// If not, just use an arbitrary default size.

	LPane* basePaneDisplay = GetBasePaneDisplay();
	if (basePaneDisplay == nil) {
		ResizeImageTo(300, 200, false);		// arbitrary default
		return;
	}
	ValidateObject_(basePaneDisplay);

	// Okay, we've got a root display pane. Ask it for its size.

	SDimension16 frameSize;
	basePaneDisplay->GetFrameSize(frameSize);
	
	// Find out what the overscan is for our layout.
	
	SDimension32 tlSlop;
	SDimension32 brSlop;
	GetDesktopOverscan(tlSlop, brSlop);
	
	// Add pane's size to desktop size and that's our image size.
	
	SDimension32 image;
	image.width = frameSize.width + tlSlop.width + brSlop.width;
	image.height = frameSize.height + tlSlop.height + brSlop.height;

	ResizeImageTo(image.width, image.height, false);
	
}


// ---------------------------------------------------------------------------
//		* CalcStandardSize
// ---------------------------------------------------------------------------
//	Make sure the layout window zooms properly.

void
VELayoutView::CalcStandardSize(
	GReferencedWindow*	inWindow) 
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inWindow);

	// Recalculate image size.
	
	AdjustImageSize();
	
	// Adjust window's preferred size to match image size.

	SDimension16 windStandardSize;
	SDimension32 layoutImageSize;
	GetImageSize(layoutImageSize);
	
	windStandardSize.width = (layoutImageSize.width > 16000 ? 16000 : layoutImageSize.width)
						+ (mShowHideInfo.mShowActualWindow ? 0 : 15);
	windStandardSize.height = (layoutImageSize.height > 16000 ? 16000 : layoutImageSize.height)
						+ (mShowHideInfo.mShowActualWindow ? 0 : 15);

	inWindow->SetStandardSize(windStandardSize);

}


// ===========================================================================

#pragma mark -
#pragma mark ** public configuration

// ---------------------------------------------------------------------------
//		* EditGridDialog
// ---------------------------------------------------------------------------
//	Run the dialog that allows users to configure the grid size.

void
VELayoutView::EditGridDialog()
{

	// Validate pointers.
	
	ValidateThis_();

	// Create dialog.

	StApplicationContext appContext;
	StDialogHandler dialog(PPob_GridDialog, this);

	LWindow* dialogWindow = dialog.GetDialog();
	ValidateObject_(dialogWindow);
	
	// Fill in fields in dialog.
	
	LEditText* horizGrid = (dynamic_cast<LEditText*>(dialogWindow->FindPaneByID(Pane_HorizontalGrid)));
	LEditText* vertGrid = (dynamic_cast<LEditText*>(dialogWindow->FindPaneByID(Pane_VerticalGrid)));
	ValidateObject_(horizGrid);
	ValidateObject_(vertGrid);
	
	horizGrid->SetValue(mGridInfo.mGridSize.width);
	vertGrid->SetValue(mGridInfo.mGridSize.height);
	
	// Run the dialog.
	
	dialogWindow->ProcessCommand(cmd_PositionWindow);
	dialogWindow->Show();
	MessageT theMessage = msg_Nothing;
	do {
		theMessage = dialog.DoDialog();
	} while (theMessage == msg_Nothing);
	dialogWindow->ProcessCommand(cmd_RecordPosition);

	// If OK was clicked, pull values out of dialog.

	if (theMessage == msg_OK) {
		if ((horizGrid->GetValue() < 2) || (vertGrid->GetValue() < 2)) {
			::SysBeep(1);
			return;
		}
		Refresh();
		sGridInfo.mGridSize.width = mGridInfo.mGridSize.width = horizGrid->GetValue();
		sGridInfo.mGridSize.height = mGridInfo.mGridSize.height = vertGrid->GetValue();
		sPrefsChanged = true;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** activeate/deactivate events

// ---------------------------------------------------------------------------
//		* SuperActivate
// ---------------------------------------------------------------------------
//	Overridden to ensure that text traits, etc., are properly configured
//	while processing the activate event.

void
VELayoutView::SuperActivate()
{
	
	// Make sure drawing agents can get at our resource map.
	// Some objects like to redraw themselves on the activate
	// event rather than waiting for refresh.
	
	sCurrentMap = mRFMap;

	// Activate the display.

	LView::SuperActivate();

	// Restore normal map.
	
	sCurrentMap = nil;

}


// ---------------------------------------------------------------------------
//		* SuperDeactivate
// ---------------------------------------------------------------------------
//	Overridden to ensure that text traits, etc., are properly configured
//	while processing the deactivate event.

void
VELayoutView::SuperDeactivate()
{
	
	// Make sure drawing agents can get at our resource map.
	// Some objects like to redraw themselves on the deactivate
	// event rather than waiting for refresh.
	
	sCurrentMap = mRFMap;

	// Activate the display.

	LView::SuperDeactivate();

	// Restore normal map.
	
	sCurrentMap = nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* Draw													[protected]
// ---------------------------------------------------------------------------
//	Draw the layout window. Don't draw any subpanes; this will be handled
//	by DrawSelf, which has knowledge about the proper z-ordering.
//
//	Adapted from LView::Draw. Removes behavior of drawing subpanes.

void
VELayoutView::Draw(
	RgnHandle	inSuperDrawRgnH)
{
	
	// Validate pointers.

	ValidateThis_();

	// Don't draw if invisible or unable to put in focus.
	
	if (IsVisible() && FocusDraw()) {

		// Area of this View to draw is the intersection
		// of inSuperDrawRgnH with the Revealed area of this view.

		::RectRgn(mUpdateRgn, &mRevealedRect);
		if (inSuperDrawRgnH != nil)
			::SectRgn(inSuperDrawRgnH, mUpdateRgn, mUpdateRgn);

		if (!::EmptyRgn(mUpdateRgn)) {

			// Some portion needs to be drawn.

			Rect frame;
			CalcLocalFrameRect(frame);
			if (ExecuteAttachments(msg_DrawOrPrint, &frame))
				DrawSelf();

			// Skip the drawing of subpanes.
			
		}

		// Emptying update region frees up memory
		// if this region wasn't rectangular.
		
		::SetEmptyRgn(mUpdateRgn);

	}
}


// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Draw the backdrop, display objects, and selection indication.

void
VELayoutView::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();

	// Find frame of base pane.
	
	Rect baseFrame = { 0, 0, 0, 0 };
	LPane* basePane = GetBasePaneDisplay();
	if (basePane != nil) {
		ValidateObject_(basePane);
		basePane->CalcPortFrameRect(baseFrame);
		PortToLocalPoint(topLeft(baseFrame));
		PortToLocalPoint(botRight(baseFrame));
	}

	// Draw simulated frame of window (if appropriate).
	
	if (mLayoutBackdrop != nil) {
		ValidateObject_(mLayoutBackdrop);

		mLayoutBackdrop->FocusDraw();
		mLayoutBackdrop->DrawSelf();
		
		FocusDraw();
	}

	// Draw subpanes and selection indication.

	if (mRootObject != nil) {
		ValidateObject_(mRootObject.GetObject());
		DrawModelObject(mRootObject, false);
	}
	DrawSelection();

}


// ---------------------------------------------------------------------------
//		* DrawModelObject										[protected]
// ---------------------------------------------------------------------------
//	Draw the named object and all of its subobjects.

void
VELayoutView::DrawModelObject(
	VEModelObject*	inObject,
	Boolean			inPrinting)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);
	
	// Make sure drawing agents can get at our resource map.
	
	sCurrentMap = mRFMap;

	// Make sure this object is in fact visible.
	
	if (!ObjectIsVisible(inObject))
		return;
	
	// Okay, it's visible. Draw this object.

	VEDrawingAgent* agent = FindDrawingAgentForObject(inObject);
	if (agent != nil) {
	
		// Draw the object.

		ValidateObject_(agent);	
		agent->DrawDisplayObject();
		
		// Draw pane adornments.
		
		LPane* displayPane = agent->GetDisplayPane();
		if (displayPane != nil) {

			// Validate pane pointer.

			ValidateObject_(displayPane);
			
			// Draw the pane itself.
		
			Rect localFrame;
			displayPane->CalcLocalFrameRect(localFrame);
			displayPane->FocusDraw();
			
			// Draw grid (if this is the root object).
			
			if ((inObject == mRootObject) && mShowHideInfo.mShowGrid)		//* J-1.0a2: BUG FIX #909
				DrawGrid(localFrame);											//		(moved from DrawSelf)
			
			// Draw pane edges and IDs.

			DrawPaneAdornments(inObject, localFrame, inPrinting);

		}
	}
	
	// Now see if there are any subs. Draw them if so.
	
	DMContainerAttribute* subs = inObject->GetSubPaneList();
	if (subs != nil) {

		ValidateObject_(subs);

		DMFastIterator iter(subs->GetSubElements());
		while (iter.NextElement()) {

			VEModelObject* sub = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
			if (sub != nil) {
				ValidateObject_(sub);
				DrawModelObject(sub, inPrinting);
			}
		}
	
	}

	// Restore normal map.
	
	sCurrentMap = nil;

}


// ---------------------------------------------------------------------------
//		* DrawPaneAdornments									[protected]
// ---------------------------------------------------------------------------
//	Draw any pane adornments for the named pane. Currently these include
//	pane ID and page outline.

void
VELayoutView::DrawPaneAdornments(
	VEModelObject*		inObject,
	const Rect&			inLocalFrame,
	Boolean				inPrinting)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// Draw pane outline if so requested.
	
	if ((inObject != mRootObject) && (mShowHideInfo.mShowPaneEdges)) {

		// For printing, we use a solid hairline.
		// On screen, we use a dotted gray pattern.
		
		::PenNormal();
	
		if (inPrinting) {
			StFractionalPenWidth hairline(1, 4);
			::FrameRect(&inLocalFrame);
		}
		else {
			RGBColor theColor = UGAColorRamp::GetColor(6);
			::RGBForeColor(&theColor);
			Pattern thePattern;
			UQDGlobals::GetGrayPat(&thePattern);
			::PenPat(&thePattern);
			::PenMode(patOr);
			::FrameRect(&inLocalFrame);
		}
		::PenNormal();
		
	}

	// Draw pane ID if possible.
	
	if (mShowHideInfo.mShowPaneAdornments) {
		
		// Get ID string.
		
		LStr255 idString;
		inObject->GetPrintableID(idString);
		
		if (idString.Length() > 0) {
		
			// Prepare to draw the string.
			
			StTextState text;
			StColorPenState pen;
			text.Normalize();
			pen.Normalize();
			::TextFont(applFont);
			::TextSize(9);
			::TextFace(0);
			
			Rect tagBBox;
			tagBBox.left = inLocalFrame.right - ::StringWidth(idString) - 3;
			tagBBox.right = inLocalFrame.right;
			tagBBox.top = inLocalFrame.top;
			tagBBox.bottom = inLocalFrame.top + 11;		//! TEMPORARY: should read font ascent
			
			// Make sure it will fit.
			
			if ((tagBBox.left < inLocalFrame.left) || (tagBBox.bottom > inLocalFrame.bottom))
				return;
			
			// Now draw bounding box.
		
			if (inPrinting) {
				StFractionalPenWidth hairline(1, 4);
				::EraseRect(&tagBBox);
				::FrameRect(&tagBBox);
			}
			else {
				RGBColor backColor = { 0xFFFF, 0xCCCC, 0x9999 };		// a light peach color
				::RGBBackColor(&backColor);
				::EraseRect(&tagBBox);
			}
			
			// Now draw the label.
			
			::TextMode(srcOr);
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor(&theColor);
			::MoveTo(tagBBox.left + 2, tagBBox.bottom - 2);
			::DrawString(idString);
		
		}
	}

}


// ---------------------------------------------------------------------------
//		* DrawSelection											[protected]
// ---------------------------------------------------------------------------
//	Draw selection handles on all selected objects.

void
VELayoutView::DrawSelection()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Make sure the pen mode is re-normalized.
	
	StColorPenState::Normalize();
	
	// Walk the selection list and redraw selection handles
	// on selected items. Actually what happens is we call
	// the drawing agent. The drawing agent knows how to draw
	// the correct selection handle.
	
	DMFastIterator iter(mSelection->GetSelectedElements());
	while (iter.NextElement()) {

		VEModelObject* obj = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		if ((obj != nil) && ObjectIsVisible(obj)) {
			ValidateObject_(obj);
			VEDrawingAgent* agent = FindDrawingAgentForObject(obj);
			if (agent != nil) {
				ValidateObject_(agent);
				agent->DrawSelection();
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* DrawGrid												[protected]
// ---------------------------------------------------------------------------
//	Draw the grid lines which may be used for alignment.

void
VELayoutView::DrawGrid(
	const Rect&  inBaseFrame)
{

	// Validate pointers.
	
	ValidateThis_();

	// Set up the pen.
	
	StColorPenState pen;
	pen.Normalize();
	RGBColor gridColor;
	GetGridColor(gridColor);	//CCP 2/7/97 For Pilot, which has a pretty dark screen
	::RGBForeColor(&gridColor);
	Pattern thePattern;
	UQDGlobals::GetGrayPat(&thePattern);
	::PenPat(&thePattern);
	::PenMode(patOr);			//* J-1.0a3: BUG FIX #909: changed penMode
								//*		so that grid looks OK on non-white backgrounds
	
	// Draw horizontal gridlines.
	
	SInt16 horizStep = mGridInfo.mGridSize.width;
	for (SInt16 x = inBaseFrame.left; x < inBaseFrame.right; x += horizStep) {
		::MoveTo(x, inBaseFrame.top);
		::LineTo(x, inBaseFrame.bottom - 1);
	}

	// Draw vertical gridlines.

	SInt16 vertStep = mGridInfo.mGridSize.height;
	for (SInt16 y = inBaseFrame.top; y < inBaseFrame.bottom; y += vertStep) {
		::MoveTo(inBaseFrame.left, y);
		::LineTo(inBaseFrame.right - 1, y);
	}
}


// ---------------------------------------------------------------------------
//		* GetGridColor											[protected]
// ---------------------------------------------------------------------------
//	Override Hook

void
VELayoutView::GetGridColor(
	RGBColor& outColor)
{
	outColor = UGAColorRamp::GetColor(4);
}


// ---------------------------------------------------------------------------
//		* ActivateSelf											[protected]
// ---------------------------------------------------------------------------
//	Redraw selection handles as solid squares.

void
VELayoutView::ActivateSelf()
{
	RefreshSelection();
}


// ---------------------------------------------------------------------------
//		* DeactivateSelf										[protected]
// ---------------------------------------------------------------------------
//	Redraw selection handles as hollow squares.

void
VELayoutView::DeactivateSelf()
{
	RefreshSelection();
}


// ---------------------------------------------------------------------------
//		* RefreshSelection										[protected]
// ---------------------------------------------------------------------------
//	Called by ActivateSelf and DeactivateSelf. Force a refresh of all
//	selection handles.

void
VELayoutView::RefreshSelection()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Walk the selection list and redraw selection handles
	// on selected items.
	
	DMFastIterator iter(mSelection->GetSelectedElements());
	while (iter.NextElement()) {

		VEModelObject* obj = dynamic_cast<VEModelObject*>(iter.CurrentElement());
		if (obj != nil) {
			ValidateObject_(obj);
			VEDrawingAgent* agent = FindDrawingAgentForObject(obj);
			if (agent != nil) {
				ValidateObject_(agent);
				agent->RefreshSelection();
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* RefreshRootObject										[protected]
// ---------------------------------------------------------------------------
//	Cause the root display object to redraw.

void
VELayoutView::RefreshRootObject()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());
	
	// Ask the root object's drawing agent to refresh itself.
	
	VEDrawingAgent* agent = FindDrawingAgentForObject(mRootObject);
	if (agent != nil) {
		ValidateObject_(agent);
		agent->RefreshDisplayObject();
	}
}


// ---------------------------------------------------------------------------
//		* ObjectIsVisible										[protected]
// ---------------------------------------------------------------------------
//	Returns true if the named object should be visible in the layout view.
//	This may or may not have anything to do with the object's visible
//	attribute.

Boolean
VELayoutView::ObjectIsVisible(
	VEModelObject*	inObject) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// Root object is always visible.

	if (inObject == mRootObject)
		return true;
	
	// If object says it's visible, it's visible.

	if (inObject->IsVisible())
		return true;

	// If object says it's invisible, we check the "show invisible objects"
	// preference.
	
	return mShowHideInfo.mShowInvisiblePanes;

}


// ===========================================================================

#pragma mark -
#pragma mark ** printing behaviors

// ---------------------------------------------------------------------------
//		* SuperPrintPanel										[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that subpanes are printed in the correct sequence.

void
VELayoutView::SuperPrintPanel(
	const PanelSpec&	inSuperPanel,
	RgnHandle			inSuperPrintRgnH)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Don't print if invisible.

	if (IsVisible()) {

		// Area of this view to print is the
		// intersection of inSuperPrintRgnH
		// with the revealed area of this view.
		
		::RectRgn(mUpdateRgn, &mRevealedRect);
		if (inSuperPrintRgnH != nil)
			::SectRgn(inSuperPrintRgnH, mUpdateRgn, mUpdateRgn);

		if (!::EmptyRgn(mUpdateRgn) && FocusDraw()) {

			// Some portion needs to be printed.
			// Print panel of this view.
			
			Rect frame;
			CalcLocalFrameRect(frame);
			if (ExecuteAttachments(msg_DrawOrPrint, &frame))
				PrintPanelSelf(inSuperPanel);

			// Do NOT print subpanes.

		}

		// Emptying update region frees up memory
		// if this region wasn't rectangular.

		::SetEmptyRgn(mUpdateRgn);
		
	}
}


// ---------------------------------------------------------------------------
//		* PrintPanel											[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that subpanes are printed in the correct sequence.

void
VELayoutView::PrintPanel(
	const PanelSpec&	inPanel,
	RgnHandle			inSuperPrintRgnH)
{

	// Validate pointers.
	
	ValidateThis_();

	// Don't print if invisible.
	
	if (IsVisible()) {

		// Area of this view to print is the
		// intersection of inSuperPrintRgnH
		// with the revealed area of this view.

		::RectRgn(mUpdateRgn, &mRevealedRect);
		if (inSuperPrintRgnH != nil)
			::SectRgn(inSuperPrintRgnH, mUpdateRgn, mUpdateRgn);
		
		if (!::EmptyRgn(mUpdateRgn) && ScrollToPanel(inPanel) && FocusDraw()) {

			Rect frame;
			CalcLocalFrameRect(frame);

			if (ExecuteAttachments(msg_DrawOrPrint, &frame))
				PrintPanelSelf(inPanel);

			// Do NOT print subpanes.

		}

		// Emptying update region frees up memory
		// if this region wasn't rectangular.

		::SetEmptyRgn(mUpdateRgn);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** mouse-down behaviors

// ---------------------------------------------------------------------------
//		* Click													[protected]
// ---------------------------------------------------------------------------
//	Override LView::Click to disallow subpanes from handling clicks.

void
VELayoutView::Click(
	SMouseDownEvent&	inMouseDown)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Code adapted from LPane::Click. We don't want subpanes to handle clicks.

	PortToLocalPoint(inMouseDown.whereLocal);
	UpdateClickCount(inMouseDown);
	if (ExecuteAttachments(msg_Click, &inMouseDown))
		ClickSelf(inMouseDown);

}


// ---------------------------------------------------------------------------
//		* ClickSelf												[protected]
// ---------------------------------------------------------------------------
//	The main mouse-event dispatcher. Determines what object was hit,
//	(de)selects it if necessary, then determines whether the mouse-down
//	should be considered a click or drag. Dispatches to several routines
//	once this interpretation is made.

void
VELayoutView::ClickSelf(
	const SMouseDownEvent& inMouseDown)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mRootObject.GetObject());

	// Setup for hit testing.

	VEModelObject* hitObject = nil;
	VEModelObject* behindObject = nil;

	Boolean toggleSelection = (inMouseDown.macEvent.modifiers & shiftKey) != 0;
	Boolean hitHandle = false;
	Boolean hitBottomHandle = false;
	Boolean hitRightHandle = false;
	
	SInt32 horizPort = inMouseDown.wherePort.h;
	SInt32 vertPort = inMouseDown.wherePort.v;

	// Do hit-testing on the display objects. If the
	// control key is down, ignore it. We'll pretend that
	// nothing got hit and offer marquee selection instead.

	if (!(inMouseDown.macEvent.modifiers & controlKey)) {
	
		// See if any of the selection handles got hit.
		// Ignore this test if the command key is down, since
		// we'll want to select behind.
		
		if (!(inMouseDown.macEvent.modifiers & cmdKey)) {
			hitObject = HitTestSelectionHandles(horizPort, vertPort,
												hitBottomHandle, hitRightHandle);
			if (hitObject != nil)
				hitHandle = true;
		}

		// Didn't hit any of the selection handles.
		// See if anything that was already selected got hit.

		if (hitObject == nil) {

			if (inMouseDown.macEvent.modifiers & (optionKey | cmdKey))
				hitObject = HitTestSelectedObjects(horizPort, vertPort);

			if ((inMouseDown.macEvent.modifiers & cmdKey) && (hitObject == mRootObject))
				hitObject = nil;

			if (hitObject != nil) {
				
				ValidateObject_(hitObject);
			
				if (inMouseDown.macEvent.modifiers & cmdKey) {
				
					// Command key is down, try to select behind the object
					// that was just hit.
									
					behindObject = hitObject;
					toggleSelection = false;

					hitObject = HitTestObject(mRootObject, behindObject, horizPort, vertPort);
					
					// Ignore shift key. Just select this object by itself.
					
					ValidateObject_(hitObject);
					mSelection->SelectSimple(hitObject);
					
					// Reset click count.
					
					sClickCount = 0;
		
				}
			}
			
			if (hitObject == nil) {
			
				// We didn't hit an already-selected item, now hit test everything.
			
				hitObject = HitTestObject(mRootObject, behindObject, horizPort, vertPort);
				
				// Make sure this object is selected. If shift key is down, we
				// add it to the selection; otherwise, we replace the existing
				// selection.
				
				if ((hitObject != nil) && (hitObject != mRootObject)) {

					ValidateObject_(hitObject);
					
					// Only change selection if this object isn't already selected.
					
					if (!mSelection->IsSelected(hitObject)) {

						if (inMouseDown.macEvent.modifiers & shiftKey)
							mSelection->SelectAdd(hitObject);
						else
							mSelection->SelectSimple(hitObject);

						toggleSelection = false;
					}
				}
			}
		}
	}

	// Okay, hit-testing is done. Make sure display is current
	// (i.e. recent selection changes are reflected), then see if
	// mouse begins to move (i.e. drags).
	
	UpdatePort();
	FocusDraw();

	// Don't drag until mouse has moved at least one pixel.
	// This is our own version of WaitMouseMoved, which is somewhat
	// more sensitive than their three-pixel delay.

	Boolean isDragging = false;
	while (1) {
	
		// If mouse is released, it's not a drag.
	
		if (!::StillDown())
			break;

		// Check mouse position. If it's moved by at least one pixel,
		// it's a drag.

		Point mouseLocal;
		::GetMouse(&mouseLocal);

		if ((mouseLocal.h != inMouseDown.whereLocal.h) || (mouseLocal.v != inMouseDown.whereLocal.v)) {
			isDragging = true;
			break;
		}
	}

	// Okay, now we know what (if anything) was clicked, and whether
	// the user wants to drag or click. Dispatch to the appropriate
	// routine.

	if (isDragging) {
	
		// User is dragging. See if a selection handle was hit.
		
		if (hitHandle) {
			DragSelectionHandle(hitObject, hitBottomHandle, hitRightHandle, inMouseDown);
			return;
		}
		
		// Not dragging a selection handle, are they are dragging a selected object?
		
		if ((hitObject != nil) && (hitObject != mRootObject)) {
		
			// Yes, a selected object was hit. Drag it.
		
			DragSelectedObjects(inMouseDown);

		}
		else {
		
			// No selected object was hit. Offer marquee selection.
			// If shift key is down, toggle selection; otherwise,
			// start from scratch.
		
			if (!(inMouseDown.macEvent.modifiers & shiftKey))
				mSelection->SelectNone();
			DoMarqueeSelection(inMouseDown);

		}
	}
	else {
	
		// This is not a drag, it's a click. See what was hit.
		
		if (hitObject == mRootObject) {

			ValidateObject_(hitObject);
			
			// Root object was hit. We didn't do selection on it before
			// since we didn't want to interfere with drag-selection.
			// Now we know it's not a drag. Select or deselect it properly.

			if (!mSelection->IsSelected(hitObject)) {

				if (inMouseDown.macEvent.modifiers & shiftKey)
					mSelection->SelectAdd(hitObject);
				else
					mSelection->SelectSimple(hitObject);

				toggleSelection = false;
			}
		}
		
		// If we didn't touch anything, deselect everything.
		// If we did hit an object, call ClickOnDisplayObject
		// which is an override hook for simple clicks.
		
		if (hitObject == nil)
			mSelection->SelectNone();
		else {
			ValidateObject_(hitObject);
			ClickOnDisplayObject(hitObject, inMouseDown, toggleSelection);
		}
	}
}


// ---------------------------------------------------------------------------
//		* HitTestObject											[protected]
// ---------------------------------------------------------------------------
//	See if the named pane (or one of its subpanes) is touched by the named
//	mouse location.

VEModelObject*
VELayoutView::HitTestObject(
	VEModelObject*		inParentObject,
	VEModelObject*&		inBehindObject,
	SInt32				inHorizPort,
	SInt32				inVertPort)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inParentObject);

	// Make sure this object is in fact visible.
	
	if (!ObjectIsVisible(inParentObject))
		return nil;
	
	// Optimization: Skip this pane and subs if click is outside
	// this object's frame rect.
	
	VEModelObject* hitObject = nil;
	VEDrawingAgent* agent = FindDrawingAgentForObject(inParentObject);
	Boolean hitFrame = true;
	Rect thisFrame;

	if (agent != nil) {
		ValidateObject_(agent);
		if (agent->CalcPortFrameRect(thisFrame)) {
			Point hitPt = { inVertPort, inHorizPort };
			if (!::PtInRect(hitPt, &thisFrame))
				hitFrame = false;
		}
		else {
			if (agent->GetDisplayPane() != nil)
				hitFrame = false;
		}
	}

	// Hit-test the subobjects first.
	
	if (hitFrame) {

		DMContainerAttribute* subs = inParentObject->GetSubPaneList();
		if (subs != nil) {
		
			ValidateObject_(subs);
		
			DMFastIterator iter(subs->GetSubElements(), LArrayIterator::from_End);
			while (iter.PreviousElement()) {
				
				VEModelObject* subVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
				ValidateObject_(subVE);
			
				hitObject = HitTestObject(subVE, inBehindObject, inHorizPort, inVertPort);
				if (hitObject != nil)
					return hitObject;

			}
		}
	}

	// None of the subs were hit, see if the parent was hit.

	if (hitFrame) {
		if (agent != nil) {
			ValidateObject_(agent);
			if ((agent->HitTestDisplayObject(inHorizPort, inVertPort)) && (inBehindObject == nil))
				hitObject = inParentObject;
		}
	}

	// If we're doing hit-behind testing, clear the hit-behind flag if we've
	// now checked the "behind" object.
	
	if (inBehindObject == inParentObject)
		inBehindObject = nil;

	// If this is the root object, see if the window simulation was hit.
	
	if (inParentObject == mRootObject) {

		// Convert mouse-down location to port coordinates.

		Point localPt;
		localPt.h = inHorizPort;
		localPt.v = inVertPort;
		PortToLocalPoint(localPt);
		
		// Get frame of window simulation.
		
		Rect backdropFrame;
		ValidateObject_(mLayoutBackdrop);
		if (mLayoutBackdrop->CalcLocalBackdropFrame(backdropFrame)) {
		
			if (::PtInRect(localPt, &backdropFrame))
				hitObject = inParentObject;
		
		}
	}
	
	// Return what we found.
	
	return hitObject;
		
}


// ---------------------------------------------------------------------------
//		* HitTestSelectedObjects								[protected]
// ---------------------------------------------------------------------------
//	Walk the selection list to see if any selected item was hit by the
//	specified mouse-down location.

VEModelObject*
VELayoutView::HitTestSelectedObjects(
	SInt32	inHorizPort,
	SInt32	inVertPort)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Hit-test each item in the selection list.
	
	DMFastIterator iter(mSelection->GetSelectedElements());
	while (iter.NextElement()) {

		VEModelObject* subVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		ValidateObject_(subVE);

		if (ObjectIsVisible(subVE)) {
			VEDrawingAgent* agent = FindDrawingAgentForObject(subVE);
			if (agent != nil) {
				ValidateObject_(agent);
				if (agent->HitTestDisplayObject(inHorizPort, inVertPort))
				 	return subVE;
			}
		}
	}

	return nil;

}

// ---------------------------------------------------------------------------
//		* HitTestSelectionHandles								[protected]
// ---------------------------------------------------------------------------
//	Walk the selection list to see if any selected item's selection handle
//	was hit by the specified mouse-down location.

VEModelObject*
VELayoutView::HitTestSelectionHandles(
	SInt32		inHorizPort,
	SInt32		inVertPort,
	Boolean&	outBottomCorner,
	Boolean&	outRightCorner)
{

	// Validate poitners.
	
	ValidateThis_();
	
	// Hit-test each item in the selection list.
	
	DMFastIterator iter(mSelection->GetSelectedElements());
	while (iter.NextElement()) {
		VEModelObject* subVE = dynamic_cast<VEModelObject*>(iter.CurrentElement());
		ValidateObject_(subVE);
		if (ObjectIsVisible(subVE)) {
			VEDrawingAgent* agent = FindDrawingAgentForObject(subVE);
			if (agent != nil) {
				ValidateObject_(agent);
				if (agent->HitTestSelection(inHorizPort, inVertPort, outBottomCorner, outRightCorner))
				 	return subVE;
			}
		}
	}

	return nil;

}


// ---------------------------------------------------------------------------
//		* NearestSnapPoint										[protected]
// ---------------------------------------------------------------------------
//	Find the nearest grid point to the specified mouse location.
//	May be updated in a later version to snap to other items, such as the
//	edges of nearby panes.

void
VELayoutView::NearestSnapPoint(
	Point&	ioClickPoint)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Calculate nearest grid lines.

	SInt16 horizGrid = mGridInfo.mGridSize.width;
	SInt16 vertGrid = mGridInfo.mGridSize.height;
	
	ioClickPoint.h = (ioClickPoint.h + (horizGrid >> 1)) / horizGrid;
	ioClickPoint.h *= horizGrid;
	
	ioClickPoint.v = (ioClickPoint.v + (vertGrid >> 1)) / vertGrid;
	ioClickPoint.v *= vertGrid;
}


// ---------------------------------------------------------------------------
//		* DragSelectionHandle									[protected]
// ---------------------------------------------------------------------------
//	The user is dragging a selection handle. Track the drag as a
//	Finder-marquee like drag, and then move and/or resize all selected
//	panes by the indicated amount.

void
VELayoutView::DragSelectionHandle(
	VEModelObject*			inHitObject,
	Boolean					inHitBottomCorner,
	Boolean					inHitRightCorner,
	const SMouseDownEvent&	inMouseDown)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inHitObject);
	ValidateObject_(mRootObject.GetObject());

	// Make sure changes are allowed.

	if (!mRootObject->IsEnabled())
		return;

	// Get the location of this object we're dragging.
	
	VEDrawingAgent* agent = FindDrawingAgentForObject(inHitObject);
	if (agent == nil)
		return;
	ValidateObject_(agent);
	
	Rect originalFrameRect;
	agent->CalcPortFrameRect(originalFrameRect);

	// Calculate the offset from the edge of the selection handle
	// to where the mouse touched it.
	
	SDimension16 mouseDelta;
	Point anchorPoint;

	if (inHitRightCorner) {
		mouseDelta.width = inMouseDown.wherePort.h - originalFrameRect.right + 1;
		anchorPoint.h = originalFrameRect.left;			// anchor is opposite corner
	}
	else {
		mouseDelta.width = inMouseDown.wherePort.h - originalFrameRect.left;
		anchorPoint.h = originalFrameRect.right - 1;
	}

	if (inHitBottomCorner) {
		mouseDelta.height = inMouseDown.wherePort.v - originalFrameRect.bottom + 1;
		anchorPoint.v = originalFrameRect.top;
	}
	else {
		mouseDelta.height = inMouseDown.wherePort.v - originalFrameRect.top;
		anchorPoint.v = originalFrameRect.bottom - 1;
	}

	// Hit a selection handle, do a resize task.

	FocusDraw();
	PortToLocalPoint(topLeft(originalFrameRect));
	PortToLocalPoint(botRight(originalFrameRect));
	PortToLocalPoint(anchorPoint);

	VEResizeReceiver receiver(
						*this,												// inLayoutView
						inMouseDown.whereLocal,								// inMouseDownLocal
						anchorPoint,										// inAnchorPoint
						mouseDelta,											// inMouseDelta
						(!mRootObjectMoves && (inHitObject == mRootObject)) && !inHitRightCorner,	// inConstrainLeft //CCP 2/7/97 Changed to respect mRootObjectMoves flag
						(!mRootObjectMoves && (inHitObject == mRootObject)) && !inHitBottomCorner); // inConstrainTop  //CCP 2/7/97 Changed to respect mRootObjectMoves flag

	LMarqueeTask marquis(receiver, anchorPoint);
	marquis.DoMarquee();
	
	Rect finalMarquee;
	Point moveBy;
	Point resizeBy;
	marquis.GetMarqueeRect(finalMarquee);
	
	moveBy.h = finalMarquee.left - originalFrameRect.left;
	moveBy.v = finalMarquee.top - originalFrameRect.top;
	resizeBy.h = finalMarquee.right - originalFrameRect.right - moveBy.h;
	resizeBy.v = finalMarquee.bottom - originalFrameRect.bottom - moveBy.v;

	if ((moveBy.h | moveBy.v | resizeBy.h | resizeBy.v) == 0)
		return;					// nothing to do

	// Set up transaction recording.

	Boolean resizing = (resizeBy.h || resizeBy.v);

	StDMTransactionBuilder trans(this, mSelection);
	LStr255 transName(STR_ResizeStrings, resizing ? str_ResizeObjects : str_MoveObjects);
	trans.SetDescription(transName);
	
	// Move/resize the panes.
	
	MoveSelectionBy(moveBy.h, moveBy.v);
	ResizeSelectionBy(resizeBy.h, resizeBy.v);
	
	// Commit the transaction.
	
	trans.Commit();

}


// ---------------------------------------------------------------------------
//		* DragSelectedObjects									[protected]
// ---------------------------------------------------------------------------
//	The user is dragging a selected object (but not its handle). Start
//	a Drag Manager task which will be used to either relocate the pane
//	within this layout or make a copy of it in another layout.

void
VELayoutView::DragSelectedObjects(
	const SMouseDownEvent&	inMouseDown)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// Request a non-recursive selection list.

	DMSelectionPtr copyList;
	copyList = (DMSelection*) mSelection->Clone();
	ValidateObject_(copyList.GetObject());
	copyList->EliminateRecursion();

	if (copyList->GetSelectedElements().GetCount() < 1)
		return;
	
	// Start a drag.
	
	DMDragTask drag(inMouseDown.macEvent);

	// Add all selected items to the drag.
	
	RgnHandle tempRgn = ::NewRgn();
	Rect itemBounds;

	{
		DMFastIterator iter(copyList->GetSelectedElements());
		while (iter.NextElement()) {
			::SetEmptyRgn(tempRgn);
			VEModelObject* objVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
			ValidateObject_(objVE);
			if (MakeDragRegion(objVE, tempRgn, itemBounds))
				drag.AddDragElement(objVE, tempRgn, &itemBounds);
		}
	}
	::DisposeRgn(tempRgn);

	// Make the translucent drag image for the dragged item.

	RgnHandle imageRgn = nil;
	GWorldPtr imageGWorld = nil;
//	MakeDragImage(drag.GetDragReference(), inObject, imageRgn, imageGWorld);
	
	// Track the drag.
	
	drag.DoDrag();

	// If dragged to trash, delete items.
	
	if (drag.WasDraggedToTrash())
		ProcessCommand(cmd_Clear);

	// Dispose image.
	
	if (imageRgn != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(imageRgn);
#else
		ValidateHandle_((Handle) imageRgn);
#endif
		::DisposeRgn(imageRgn);
	}
	if (imageGWorld != nil)
		::DisposeGWorld(imageGWorld);

}


// ---------------------------------------------------------------------------
//		* DoMarqueeSelection									[protected]
// ---------------------------------------------------------------------------
//	The user is dragging in the root object's display, or in the backdrop
//	area. Select all objects that are touched by the marquee.

void
VELayoutView::DoMarqueeSelection(
	const SMouseDownEvent&	inMouseDown)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Do marquee selection.

	RecordInitialSelection((inMouseDown.macEvent.modifiers & shiftKey) != 0);
	LMarqueeTask marquis(*this, inMouseDown);
	marquis.DoMarquee();

}


// ---------------------------------------------------------------------------
//		* ClickOnDisplayObject									[protected]
// ---------------------------------------------------------------------------
//	The user has clicked on a display object in this layout. The item is
//	currently selected and the mouse has already gone up. If it's a double-
//	click, we open a property inspector. If it's a shift click, we may need
//	to deselect the item.

void
VELayoutView::ClickOnDisplayObject(
	VEModelObject*			inObject,
	const SMouseDownEvent&	/* inMouseDown */,
	Boolean					inDeselectObject)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);
	ValidateObject_(mSelection.GetObject());

	// If double-click, start property inspector.
	
	if (LPane::GetClickCount() > 1) {
		UPropertyInspector::InspectSelection(mSelection, this);
		return;
	}
	
	// If shift-click, deselect item.
	
	if (inDeselectObject) {
		mSelection->SelectRemove(inObject);
		return;
	}

	// Ignore single clicks.

}


// ===========================================================================

#pragma mark -
#pragma mark ** cursor adjusting

#if __PowerPlant__ < 0x02114006

// ---------------------------------------------------------------------------
//		* AdjustCursor											[protected]
// ---------------------------------------------------------------------------
//	Overriden to prevent subpanes (i.e. display objects) from twiddling
//	the cursor.

void
VELayoutView::AdjustCursor(
	Point				inPortPt,
	const EventRecord&	inMacEvent)
{
	LPane::AdjustCursor(inPortPt, inMacEvent);
		// skip LView::AdjustCursor which allows sub panes to fiddle with cursor
}

#else

void
VELayoutView::AdjustMouse(
	Point				inPortPt,
	const EventRecord&	inMacEvent,
	RgnHandle			outMouseRgn)
{
	LPane::AdjustMouse(inPortPt, inMacEvent, outMouseRgn);
		// skip LView::AdjustMouse which allows sub panes to fiddle with cursor
}

#endif // __PowerPlant__ < 0x02114006

// ===========================================================================

#pragma mark -
#pragma mark ** drawing agent interactions

// ---------------------------------------------------------------------------
//		* BuildObjectDisplay									[protected]
// ---------------------------------------------------------------------------
//	Create a pane which will represent the given data model object in
//	the layout window. Causes a drawing agent to be created (if needed).
//	Calls itself recursively to create display objects for all subelements
//	in the display hierarchy.

void
VELayoutView::BuildObjectDisplay(
	VEModelObject*	inObject,
	Boolean			inCallFinishCreate)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);
	ValidateObject_(mSelection.GetObject());

	// Make sure drawing agents can get at our resource map.
	
	RFMap* oldMap = sCurrentMap;
	sCurrentMap = mRFMap;

	// If there's already a drawing agent, remove it.
	
	VEDrawingAgent* oldAgent = FindDrawingAgentForObject(inObject);
	if (oldAgent != nil) {
		ValidateObject_(oldAgent);
		DeleteObjectDisplay(inObject, true);
	}

	// Make sure we're aware of changes to this pane.
	
	inObject->AddListener(this);
	
	// Select superview for this pane. This code is probably
	// over-defensive, but...
	
	DMElement* ancestor = inObject;
	LView* installView = this;

	while (1) {
	
		// If this element is a view object, check to see if it has a
		// drawing agent. If so, see if it has a display pane that's actually
		// a view.
	
		VEModelObject* ancestorVE = (dynamic_cast<VEModelObject*>(ancestor));
		if (ancestorVE != nil) {

			ValidateObject_(ancestorVE);
			VEDrawingAgent* agent = FindDrawingAgentForObject(ancestorVE);

			if (agent != nil) {

				ValidateObject_(agent);
				LView* tempView = (dynamic_cast<LView*>(agent->GetDisplayPane()));

				if (tempView != nil) {
					ValidateObject_(tempView);
					installView = tempView;
					break;
				}
			}
		}
		
		// Stop if we've hit the top of the view hiearchy.
		
		if (ancestorVE == mRootObject)
			break;

		// Go up one level in data hierarchy.

		ancestor = ancestor->GetSuperElement();
		if (ancestor == nil)
			break;
		ValidateObject_(ancestor);

	}
	
	// Now that we know where view is going, let's create the drawing agent.

	VEDrawingAgent* newAgent = inObject->GetTypeEntry()->CreateDrawingAgent(inObject);
	LPane* newPane = nil;

	// If we have a drawing agent, use it to create the visual representation.
	
	if (newAgent != nil) {

		// Validate new drawing agent.
		
		ValidateObject_(newAgent);

		// Configure the agent.
		
		newAgent->SetSelection(mSelection);
		newAgent->SetModelObject(inObject);
		newAgent->SetLayoutView(this);

		// Let the drawing agent create the pane. ReanimateDisplayObject
		// is a pure virtual method of VEDrawingAgent since each framework
		// has its own unique mechanism for conveying initialization data.

		{
#if Constructor_UseRF
			StResourceContext docContext(mRFMap->GetResourceContext());
#endif
			newAgent->CreateDisplayObject(installView);
		}

		// If a pane was created, do a bit of post-processing on it.

		newPane = newAgent->GetDisplayPane();
		if (newPane != nil) {
		
			// Validate new pane pointer.
			
			ValidateObject_(newPane);
		
			// If the pane was installed in the base view, we override
			// the location so that it always appears in a reasonable location.

			if (inObject == mRootObject) {
				CheckRootObjectPosition();
				if (!mRootObjectMoves) 	//CCP 2/7/97
					newAgent->SetCantMove(true);
			}
			
			// Override frame bindings and visible flags.
			// We take care of this ourselves.
		
			SBooleanRect binding = { false, false, false, false };
			newPane->SetFrameBinding(binding);
			newPane->Show();

		}
	}

	// Do subpanes also.
	
	DMContainerAttribute* subPanes = inObject->GetSubPaneList();
	if (subPanes != nil) {
		ValidateObject_(subPanes);
		DMFastIterator iter(subPanes->GetSubElements());
		while (iter.NextElement()) {
			VEModelObject* subObject = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
			if (subObject != nil) {
				ValidateObject_(subObject);
				BuildObjectDisplay(subObject, (newPane == nil) && inCallFinishCreate);
			}
		}
	}
	
	// Do post initialization and force a redraw.
	
	if ((newPane != nil) && inCallFinishCreate) {

#if Constructor_UseRF
		StResourceContext docContext(mRFMap->GetResourceContext());
#endif

		newPane->FinishCreate();
		if (newPane->IsVisible())			//* BUG FIX #1170: avoid flicker
			newPane->Refresh();
	}

	// Restore normal map.
	
	sCurrentMap = oldMap;

}


// ---------------------------------------------------------------------------
//		* DeleteObjectDisplay									[protected]
// ---------------------------------------------------------------------------
//	Remove the pane which represents the named model object in the
//	layout window. Delete its drawing agent also.

void
VELayoutView::DeleteObjectDisplay(
	VEModelObject*	inObject,
	Boolean			inRefresh)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// Delete subpanes first.
	
	DMContainerAttribute* subPanes = inObject->GetSubPaneList();
	if (subPanes != nil) {
		ValidateObject_(subPanes);
		DMFastIterator iter(subPanes->GetSubElements());
		while (iter.NextElement()) {
			VEModelObject* subObject = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
			if (subObject != nil) {
				ValidateObject_(subObject);
				DeleteObjectDisplay(subObject, inRefresh);
			}
		}
	}
	
	// See if there's a display object for this pane.
	
	VEDrawingAgent* oldAgent = FindDrawingAgentForObject(inObject);
	if (oldAgent == nil)
		return;
	ValidateObject_(oldAgent);
	
	// There is, get rid of it. First refresh the area where the pane is displayed.
	
	if (inRefresh) {
		oldAgent->RefreshDisplayObject();
		oldAgent->RefreshSelection();
	}

	// Now ask the drawing agent to delete the display pane.

	oldAgent->RemoveDisplayObject();
	
	// Now delete the drawing agent itself.
	
	delete oldAgent;

}


// ---------------------------------------------------------------------------
//		* FindDrawingAgentForObject								[protected]
// ---------------------------------------------------------------------------
//	Given a model object, find its drawing agent.

VEDrawingAgent*
VELayoutView::FindDrawingAgentForObject(
	VEModelObject*	inObject)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);
	
	// Use the "find UI object" message to find the drawing agent.

	VEDrawingAgent* agent = (VEDrawingAgent*)
							inObject->FindUserInterfaceObject(VEDrawingAgent::object_ID);
	return agent;

}


// ---------------------------------------------------------------------------
//		* CheckRootObjectPosition								[protected]
// ---------------------------------------------------------------------------
//	Make sure the display pane for the root object in the view hierarchy
//	is located in the proper location relative to the backdrop.

void
VELayoutView::CheckRootObjectPosition()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for base pane display.
	
	LPane* basePaneDisplay = GetBasePaneDisplay();
	if (basePaneDisplay != nil) {

		// Found it, ask it for location.
		
		ValidateObject_(basePaneDisplay);
	
		SDimension32 tlSlop;
		SDimension32 brSlop;
		GetDesktopOverscan(tlSlop, brSlop);
		basePaneDisplay->PlaceInSuperImageAt(tlSlop.width, tlSlop.height, false);
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag-tracking helpers

// ---------------------------------------------------------------------------
//		* MakeDragRegion										[protected]
// ---------------------------------------------------------------------------
//	Form the drag region for the named item. Creates a rectangle the size
//	of the display pane.

Boolean
VELayoutView::MakeDragRegion(
	VEModelObject*		inObject,
	RgnHandle			outGlobalDragRgn,
	Rect&				outGlobalItemBounds)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// Make sure there's a drawing agent for this object.

	VEDrawingAgent* agent = FindDrawingAgentForObject(inObject);
	if (agent != nil) {

		// Validate pointers.
		
		ValidateObject_(agent);

		// There is, ask it for its bounds rect.
		
		Rect frame;
		if (agent->CalcPortFrameRect(frame)) {
			
			// It has a frame, use it to build display region.
			
			PortToGlobalPoint(topLeft(frame));
			PortToGlobalPoint(botRight(frame));
			
			// Get a temporary working region.
		
			RgnHandle tempRgn = ::NewRgn();
#if PP_Target_Carbon
			ThrowIfNil_(tempRgn);
#else
			ValidateHandle_((Handle) tempRgn);
#endif
			
			// Add the frame to the drag region.
		
			::RectRgn(tempRgn, &frame);
			::UnionRgn(outGlobalDragRgn, tempRgn, outGlobalDragRgn);
			
			// Return item bounds.
			
			outGlobalItemBounds = frame;
			
			// Release temporary region.
			
			::DisposeRgn(tempRgn);
			
			// Successfully found item bounds.
			
			return true;

		}
	}

	// No display item, no item bounds.

	return false;

}


// ---------------------------------------------------------------------------
//		* MakeDragImage											[protected]
// ---------------------------------------------------------------------------
//	Make a translucent drag image for the named pane.

void
VELayoutView::MakeDragImage(
	DragReference		inDragRef,
	VEModelObject*		inObject,
	RgnHandle&			ioImageRgn,
	GWorldPtr&			ioImageGWorld)
{
	#pragma unused (inDragRef)
	#pragma unused (inObject)
	#pragma unused (ioImageRgn)
	#pragma unused (ioImageGWorld)
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag-selection helpers

// ---------------------------------------------------------------------------
//		* CheckElements											[protected]
// ---------------------------------------------------------------------------
//	Called during marquee selection to see which items fall within the
//	selection rectangle.

void
VELayoutView::CheckElements(
	const Rect&		inOldMarqueeRect,
	const Rect&		inNewMarqueeRect)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());

	// Convert rectangles to port coordinates.
	
	Rect oldPort = inOldMarqueeRect;
	Rect newPort = inNewMarqueeRect;

	LocalToPortPoint(topLeft(oldPort));
	LocalToPortPoint(botRight(oldPort));
	LocalToPortPoint(topLeft(newPort));
	LocalToPortPoint(botRight(newPort));

	// Find out what area we have to test.

	Rect testRect;
	::UnionRect(&oldPort, &newPort, &testRect);
	
	// Retest all panes that lie within this area.
	
	CheckMarqueeSelection(mRootObject, newPort, testRect);

}


// ---------------------------------------------------------------------------
//		* CheckMarqueeSelection									[protected]
// ---------------------------------------------------------------------------
//	Called during marquee selection to see if the named object (or any of
//	its subobjects) are in the selection rectangle. Calls itself recursively
//	to test subobjects.

void
VELayoutView::CheckMarqueeSelection(
	VEModelObject*		inParentObject,
	const Rect&			inNewMarqueeRect,
	const Rect&			inTestAreaRect)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inParentObject);

	// Make sure this object is in fact visible.
	
	if (!ObjectIsVisible(inParentObject))
		return;
	
	// Optimization: Skip this pane and subs if test area is outside
	// this object's frame rect. Don't do this for root object.
	
	VEDrawingAgent* agent = FindDrawingAgentForObject(inParentObject);
	Rect thisFrame, ignore;

	Boolean hasFrame = false;
	if (agent != nil) {
		ValidateObject_(agent);
		hasFrame = agent->CalcPortFrameRect(thisFrame);
		if (hasFrame) {
			if (!::SectRect(&inTestAreaRect, &thisFrame, &ignore))
				return;
		}
		else if (agent->GetDisplayPane() != nil)
			return;
	}

	// See if this object was hit by the marquee.
	// The root object is never hit by the marquee.

	Rect subMarqueeRect;
	Boolean marqueeHitFrame = ::SectRect(&inNewMarqueeRect, &thisFrame, &subMarqueeRect);
	FoundElement(inParentObject, (inParentObject != mRootObject) ? marqueeHitFrame : false);
	
	// Now hit-test the subobjects (if any).
	
	DMContainerAttribute* subs = inParentObject->GetSubPaneList();
	if (subs != nil) {
		ValidateObject_(subs);
		DMFastIterator iter(subs->GetSubElements());
		while (iter.NextElement()) {
			VEModelObject* sub = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
			if (sub != nil) {
				ValidateObject_(sub);
				CheckMarqueeSelection(sub, subMarqueeRect, inTestAreaRect);
			}
		}
	}
}

// ===========================================================================

#pragma mark -
#pragma mark ** drag-receive helpers

// ---------------------------------------------------------------------------
//		* ChoosePlacementForElement								[protected]
// ---------------------------------------------------------------------------
//	Decide where to place a dragged object in the view object hierarchy.
//	The placement decides on which superview is touched by the current
//	drag-mouse location.

Boolean
VELayoutView::ChoosePlacementForElement(
	DragReference		inDragRef,
	ItemReference		/* inItemRef */,
	const DMElement*	inElement,
	DM_DragPlacement&	ioCurrentPlacement) 
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Disallow dragging items that must be root.
	
	const VEModelObject* elementVE = dynamic_const_cast( inElement, DMElement*, VEModelObject* );
	if (elementVE != nil) {
		ValidateObject_(elementVE);
		if (elementVE->GetTypeEntry()->MustBeRoot())
			return false;
	}

	// Convert mouse location to image coordinates.
	
	Point dragMouse;
	ThrowIfOSErr_(::GetDragMouse(inDragRef, &dragMouse, nil));
	GlobalToPortPoint(dragMouse);
	
	// Find the object that's hit by this mouse point.
	
	VEModelObject* behindObject = nil;
	VEModelObject* dragTarget = HitTestObject(
									mRootObject,			// inParentObject
									behindObject,			// inBehindObject
									dragMouse.h,			// inHorizPort
									dragMouse.v);			// inVertPort

	if (dragTarget == nil)
		return false;
	ValidateObject_(dragTarget);
	
	// Scan up through the object hierarchy until we find an object that
	// can host the dragged element.
	
	while (1) {

		if (!dragTarget->IsSubElementOf(inElement)
		  && dragTarget->CanHostElement(inElement, &ioCurrentPlacement))
			return true;
		
		DMElement* ancestor = dragTarget;
		dragTarget = nil;

		while (1) {
			ancestor = ancestor->GetSuperElement();
			if (ancestor == nil)
				break;
			ValidateObject_(ancestor);
			
			dragTarget = (dynamic_cast<VEModelObject*>(ancestor));
			if (dragTarget != nil)
				break;
		}
		
		if (dragTarget == nil)
			break;
		ValidateObject_(dragTarget);
	
	}

	// Didn't hit an object or object didn't want it, can't accept element.
	
	return false;

}


// ---------------------------------------------------------------------------
//		* InsideDropArea										[protected]
// ---------------------------------------------------------------------------
//	Overriden to add constraints and redraw highlighting if drag placement
//	(host superview) has changed.

void
VELayoutView::InsideDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.
	
	ValidateThis_();

	// Set cursor and auto-scroll if appropriate.

	DMDropArea::InsideDropArea(inDragRef);
	
	// See if drag placement has changed.
	
	if (mCurrentPlacement.mHostElement != mPreviousPlacement.mHostElement) {
		FocusDropArea();
		UnhiliteDropArea(inDragRef);
		mIsHilited = true;
		HiliteDropArea(inDragRef);
	}

	// Do gridding and axis constraints.
	
	DoDragConstraints(inDragRef);
	
}

// ---------------------------------------------------------------------------
//		* HiliteDropArea										[protected]
// ---------------------------------------------------------------------------
//	Overriden to show which object will serve as the container for the
//	newly-placed items.

void
VELayoutView::HiliteDropArea(
	DragReference	inDragRef)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// If no host, no hiliting.

	if (mCurrentPlacement.mHostElement == nil)
		return;
	
	// Find the object that's receiving the drag.
	
	DMElement* object = mCurrentPlacement.mHostElement->GetAffectedObject();
	if (object == nil)
		return;
	ValidateObject_(object);
	
	// Cast it to VEModelObject.

	VEModelObject* host = (dynamic_cast<VEModelObject*>(object));
	if (host == nil)
		return;
	ValidateObject_(host);
	
	// Find its drawing agent.
	
	VEDrawingAgent* agent = FindDrawingAgentForObject(host);
	if (agent == nil)
		return;
	ValidateObject_(agent);

	// Find the display pane.

	LPane* hostPane = agent->GetDisplayPane();
	if (hostPane == nil)
		return;
	ValidateObject_(hostPane);

	// Ask the display pane for its frame rect.
	// Hilite this area.

	Rect dropRect;
	if (hostPane->CalcPortFrameRect(dropRect)) {
		FocusDraw();
		PortToLocalPoint(topLeft(dropRect));
		PortToLocalPoint(botRight(dropRect));
		RgnHandle dropRgn = ::NewRgn();
		::RectRgn(dropRgn, &dropRect);
		::ShowDragHilite(inDragRef, dropRgn, true);
		::DisposeRgn(dropRgn);
	}
}


// ---------------------------------------------------------------------------
//		* DoDragConstraints										[protected]
// ---------------------------------------------------------------------------
//	Called by VELayoutView::InsideDropArea. Adjust the drag mouse position
//	if appropriate for gridding and axis constraints.

void
VELayoutView::DoDragConstraints(
	DragReference	inDragRef)
{

	// Validate pointers.
	
	ValidateThis_();

	// Get mouse location and keyboard modifiers.
	
	Point dragMouse;
	::GetDragMouse(inDragRef, &dragMouse, nil);
	
	SInt16 dragModifiers;
	::GetDragModifiers(inDragRef, &dragModifiers, nil, nil);

	// Scan the dragged items looking for item bounds.

	UInt16 numItems;
	::CountDragItems(inDragRef, &numItems);
	
	Rect dragItemBounds = { 0, 0, 0, 0 };
	for (SInt16 index = 1; index <= numItems; index++) {
		ItemReference theItemRef;
		Rect thisItemBounds;
		::GetDragItemReferenceNumber(inDragRef, index, &theItemRef);
		::GetDragItemBounds(inDragRef, theItemRef, &thisItemBounds);
		if (::EmptyRect(&dragItemBounds))
			dragItemBounds = thisItemBounds;
		else
			::UnionRect(&dragItemBounds, &thisItemBounds, &dragItemBounds);
	}
	
	// Calculate drag origin.
	
	Point dragOrigin;
	::GetDragOrigin(inDragRef, &dragOrigin);

	// Decide which grid constraints we can do.
	
	Boolean shiftConstrain = (dragModifiers & shiftKey) && !sDragHasLeftSender;
	Boolean gridConstrain = ((dragModifiers & cmdKey) == cmdKey) ^ (mGridInfo.mSnapToGrid);

	if (shiftConstrain || gridConstrain) {
	
		// Convert mouse location to local coordinates of the base view.
		
		SDimension32 tlSlop, brSlop;
		GetDesktopOverscan(tlSlop, brSlop);
	
		Point dragUpperLeft = { dragItemBounds.top, dragItemBounds.left };
		SInt16 hMouseOffset = dragMouse.h - dragUpperLeft.h;
		SInt16 vMouseOffset = dragMouse.v - dragUpperLeft.v;
	
		dragOrigin.h -= hMouseOffset;
		dragOrigin.v -= vMouseOffset;
	
		GlobalToPortPoint(dragUpperLeft);
		GlobalToPortPoint(dragMouse);
		GlobalToPortPoint(dragOrigin);
		PortToLocalPoint(dragUpperLeft);
		PortToLocalPoint(dragMouse);
		PortToLocalPoint(dragOrigin);
	
		dragUpperLeft.h -= tlSlop.width;
		dragUpperLeft.v -= tlSlop.height;
		dragMouse.h -= tlSlop.width;
		dragMouse.v -= tlSlop.height;
		dragOrigin.h -= tlSlop.width;
		dragOrigin.v -= tlSlop.height;
		
		// Execute constraints.
	
		if (gridConstrain)
			NearestSnapPoint(dragUpperLeft);
	
		Boolean lockVertical = false;
		Boolean lockHorizontal = false;
	
		if (shiftConstrain) {
			
			// Shift key is down, decide constraint based on mouse motion.
			
			SInt16 dv = dragUpperLeft.v - dragOrigin.v;
			SInt16 dh = dragUpperLeft.h - dragOrigin.h;
			if (dv < 0)
				dv = -dv;
			if (dh < 0)
				dh = -dh;
	
			if (dh >= dv)
				lockVertical = true;
			else
				lockHorizontal = true;
		}
	
		// Do we have some sort of constraint?
		
		if (lockHorizontal)
			dragUpperLeft.h = dragOrigin.h;
		if (lockVertical)
			dragUpperLeft.v = dragOrigin.v;
	
		// Convert everything back to global screen coordinates.
	
		dragMouse.h = dragUpperLeft.h + hMouseOffset + tlSlop.width;
		dragMouse.v = dragUpperLeft.v + vMouseOffset + tlSlop.height;
		LocalToPortPoint(dragMouse);
		PortToGlobalPoint(dragMouse);
		
		// Now we can stomp out original mouse location & substitute our own.
		
		::SetDragMouse(inDragRef, dragMouse);

	}
}


// ---------------------------------------------------------------------------
//		* HostDragElement										[protected]
// ---------------------------------------------------------------------------
//	An acceptable object has been dropped into the layout view. Move or
//	copy the item as appropriate.

DMElement*
VELayoutView::HostDragElement(
	DMElement*			inElement,
	Rect&				inItemLocalBounds,
	DM_DragPlacement&	ioCurrentPlacement)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Decide what item we're inserting.
	// We may be just moving an object within this layout.

	DMElementPtr itemToInsert = inElement;

	// Are we dragging an object within its superview?
	
	bool isDragWithinSuperview = false;
	DMContainerMixin* mixin = dynamic_cast<DMContainerMixin*>(ioCurrentPlacement.mHostElement);
	if (mixin != NULL)
	{
		DMFastIterator iter(mixin->GetSubElements());
		while (iter.NextElement())
		{
			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			if (objDM == dynamic_cast<DMElement*>(inElement))
			{
				isDragWithinSuperview = true;
				break;
			}
		}
	}

	if (ioCurrentPlacement.ioInsertAsDuplicate || !isDragWithinSuperview)
	{
		// We're either cloning this object or we're moving it
		// from one superview to another, we have to use the standard
		// code for moving and/or cloning.
		
		// If it's moving within the same superview, we bypass this code
		// so that we can preserve z-ordering.

		itemToInsert = DMDropArea::HostDragElement(inElement, inItemLocalBounds, ioCurrentPlacement);

	}

	// Adjust the screen position of the just-moved object
	// so that it matches the drag item bounds.

	VEModelObject* objVE = (dynamic_cast<VEModelObject*>((DMElement*) itemToInsert));
	if (objVE != nil) {
	
		// Prevent flicker caused by placing catalog elements at (0,0).
		
		ValidateObject_(objVE);
		
		VEDrawingAgent* agent = FindDrawingAgentForObject(objVE);		
		if (agent != nil) {
			ValidateObject_(agent);
			LPane* pane = agent->GetDisplayPane();
			if (pane != nil) {
				ValidateObject_(pane);
				pane->DontRefresh();
				pane->Hide();
			}
		}
		
		// Now adjust screen position.
		
		AdjustScreenPosition(objVE, inItemLocalBounds);
	
		// Restore pane's visibility. We can't cache the pane or agent pointer
		// since the move might have caused a rebuild.
		
		agent = FindDrawingAgentForObject(objVE);		
		if (agent != nil) {
			ValidateObject_(agent);
			LPane* pane = agent->GetDisplayPane();
			if (pane != nil) {
				ValidateObject_(pane);
				pane->Show();
				pane->Refresh();
			}
		}
	}

	// Return a pointer to what was just inserted.
	
	return itemToInsert;

}


// ---------------------------------------------------------------------------
//		* AdjustScreenPosition									[protected]
// ---------------------------------------------------------------------------
//	Make sure the view object named is properly located. Adjust the data
//	model's coordinates to match the specified screen location.

void
VELayoutView::AdjustScreenPosition(
	VEModelObject*	inObject,
	Rect&			inItemLocalBounds)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// Find the drawing agent for this object.
	
	VEDrawingAgent* agent = FindDrawingAgentForObject(inObject);
	if (agent == nil)
		return;
	ValidateObject_(agent);

	// Find the current display location.
	
	Rect displayLoc;
	if (!(agent->CalcPortFrameRect(displayLoc)))
		return;

	// Move the pane as necessary to make it fit.
	
	PortToLocalPoint(topLeft(displayLoc));
	inObject->MoveBy(inItemLocalBounds.left - displayLoc.left, inItemLocalBounds.top - displayLoc.top);

}


// ===========================================================================

#pragma mark -
#pragma mark ** selection behaviors

// ---------------------------------------------------------------------------
//		* SelectAll												[protected]
// ---------------------------------------------------------------------------
//	Select all objects that are submembers of this object.

void
VELayoutView::SelectAll(
	VEModelObject*	inParentObject)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inParentObject);
	ValidateObject_(mSelection.GetObject());
	
	// Make sure this object is in fact visible.
	// We don't select objects that aren't visible.
	
	if (!ObjectIsVisible(inParentObject))
		return;
	
	// Okay, it's visible. Select this object.

	if (inParentObject != mRootObject)
		mSelection->SelectAdd(inParentObject);
	
	// Now see if there are any subs. Select them if so.
	
	DMContainerAttribute* subs = inParentObject->GetSubPaneList();
	if (subs != nil) {
		ValidateObject_(subs);
		DMFastIterator iter(subs->GetSubElements());
		while (iter.NextElement()) {
			VEModelObject* sub = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
			if (sub != nil) {
				ValidateObject_(sub);
				SelectAll(sub);
			}
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ContainerListChanged									[protected]
// ---------------------------------------------------------------------------
//	Listen for object added/removed notifications. Update the display
//	as appropriate to reflect these changes.

void
VELayoutView::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ThrowIfNil_(inMessage);
	ValidateObject_(inMessage->mMovedElement);

	// See if the object that's moving is a visual object.

	VEModelObject* objVE = (dynamic_cast<VEModelObject*>(inMessage->mMovedElement));
	if (objVE != nil) {

		// Validate pointers.
		
		ValidateObject_(objVE);
	
		// Yes, create or tear down the drawing agent as appropriate.
		
		if (inMessage->mElementAdded) {

			// Creating a pane. First, make sure it's part of our visual hierarchy.

			if ((inMessage->mMovedElement->IsSubElementOf(mRootObject))
			  && (inMessage->mMovedElement->GetSuperElement() == inMessage->mContainer)) {
	
				BuildObjectDisplay(objVE, true);

			}
		}
		else
			DeleteObjectDisplay(objVE, true);
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** pane resizing behaviors

// ---------------------------------------------------------------------------
//		* MoveSelectionBy										[protected]
// ---------------------------------------------------------------------------
//	Move all selected objects by the specified number of pixels.

void
VELayoutView::MoveSelectionBy(
	SInt32	inHorizDelta,
	SInt32	inVertDelta)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// Request a non-recursive selection list.
	// Be sure the root object isn't selected if it can't move.

	DMSelectionPtr copyList;
	copyList = (DMSelection*) mSelection->Clone();
	ValidateObject_(copyList.GetObject());
	
	if (!mRootObjectMoves) //CCP 2/7/97
		copyList->SelectRemove(mRootObject);
	copyList->EliminateRecursion();
	
	// Set up a transaction.
	
	StDMTransactionBuilder trans(this, mSelection);
	LStr255 transName(STR_ResizeStrings, str_MoveObjects);
	trans.SetDescription(transName);

	// Move the objects.
	
	DMFastIterator iter(copyList->GetSelectedElements());
	while (iter.NextElement()) {
		VEModelObject* objVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		if ((objVE != nil) && ((objVE != mRootObject)||(mRootObjectMoves))) { //CCP 2/7/97 Added mRootObjectMoves check
			ValidateObject_(objVE);
			objVE->MoveBy(inHorizDelta, inVertDelta);
		}
	}

	// Commit the transaction.
	
	trans.Commit();
	
}


// ---------------------------------------------------------------------------
//		* ResizeSelectionBy										[protected]
// ---------------------------------------------------------------------------
//	Resize all selected objects by the specified number of pixels.

void
VELayoutView::ResizeSelectionBy(
	SInt32	inHorizDelta,
	SInt32	inVertDelta)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// Request a non-recursive selection list.

	DMSelectionPtr copyList;
	copyList = (DMSelection*) mSelection->Clone();
	ValidateObject_(copyList.GetObject());
	
	copyList->EliminateRecursion();
	
	// Set up a transaction.
	
	StDMTransactionBuilder trans(this, mSelection);
	LStr255 transName(STR_ResizeStrings, str_ResizeObjects);
	trans.SetDescription(transName);

	// Resize the objects.
	
	DMFastIterator iter(copyList->GetSelectedElements());
	while (iter.NextElement()) {
		VEModelObject* objVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		if (objVE != nil) {
			ValidateObject_(objVE);
			objVE->ResizeBy(inHorizDelta, inVertDelta);
		}
	}

	// Commit the transaction.
	
	trans.Commit();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** internal layout configuration

// ---------------------------------------------------------------------------
//		* GetBasePaneDisplay									[protected]
// ---------------------------------------------------------------------------
//	Return a pointer to the PowerPlant pane object that represents the
//	named display object.

LPane*
VELayoutView::GetBasePaneDisplay()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See if we have a root object yet.
	
	if (mRootObject == nil)
		return nil;
	ValidateObject_(mRootObject.GetObject());
	
	// Look for root object's drawing agent.

	VEDrawingAgent* agent = FindDrawingAgentForObject(mRootObject);
	if (agent == nil)
		return nil;
	ValidateObject_(agent);
	
	// Ask agent for pane. Return this value.
	
	return agent->GetDisplayPane();

}


// ---------------------------------------------------------------------------
//		* GetDesktopOverscan									[protected]
// ---------------------------------------------------------------------------
//	Return the preferred size of the backdrop.

void
VELayoutView::GetDesktopOverscan(
	SDimension32&	outTopLeft,
	SDimension32&	outBotRight)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// If there's a backdrop, ask it for overscan. Otherwise
	// use an arbitrary default.

	if (mLayoutBackdrop != nil) {
		ValidateObject_(mLayoutBackdrop);
		mLayoutBackdrop->GetBackdropOverscan(outTopLeft, outBotRight);
	}
	else {
		outTopLeft.width = 16;
		outTopLeft.height = 16;
		outBotRight.width = 16;
		outBotRight.height = 16;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------

void
VELayoutView::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();

	// Do inherited initialization.

	LView::FinishCreateSelf();
	
	// Make sure we get to control window size.

	LView* topView = this;
	while (topView->GetSuperView()) {
		topView = topView->GetSuperView();
		ValidateObject_(topView);
	}
	
	GReferencedWindow* theWindow = (dynamic_cast<GReferencedWindow*>(topView));
	if (theWindow != nil) {
		ValidateObject_(theWindow);
		theWindow->SetZoomer(this);
	}
}