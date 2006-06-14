// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEEditor.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/12/96
//     $Date: 2006/04/12 08:48:13 $
//	$History: VEEditor.cpp $
//	
//	*****************  Version 20  *****************
//	User: scouten      QDate: 03/19/97   Time: 12:08
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Disabled Arrange menu items when document is disabled. (Bug fix #1311.)
//	
//	*****************  Version 19  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 18  *****************
//	User: scouten      QDate: 02/07/97   Time: 13:59
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Rolled in Clint's fixes 02/06/97.
//	
//	*****************  Version 17  *****************
//	User: Andrew       Date: 2/06/97    Time: 11:51a
//	Updated in $/ConstructorWin/Sources
//	Added RM* includes
//	
//	*****************  Version 16  *****************
//	User: Andrew       Date: 2/04/97    Time: 8:54p
//	Updated in $/ConstructorWin/Sources
//	Added prefix file for MSVC; split initializations within case statement
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:47
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 12/06/96   Time: 14:19
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Removed undoer from constructor.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 12/05/96   Time: 19:22
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Removed Display Classes window.3
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:02
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added registration for VECatalogPage. Fixed a bug in
//	OpenEditorWindowSelf. (It no longer stores the partially-built window
//	pointer in mEditorWindow.)
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 11/24/96   Time: 23:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug in AlignPanes that caused a signal to occur when
//	user-defined spread was selected.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/24/96   Time: 23:25
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added CreateAlignmentPalette static function.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/24/96   Time: 22:10
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug that allowed multiple hierarchy windows to be opened.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:53
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added UpdateEditorWindowTitleSelf method.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:22
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added support for dynamic columns in hierarchy window. Added
//	AddDefaultHierColumns method.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/19/96   Time: 14:12
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Improved commenting in ShowArrangeDialog.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:28
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 11/02/96   Time: 18:10
//	Checked in '$/Constructor/Source files/Editors/Views/+ Generic view editor'
//	Comment: Updated for new undo/redo menu string architecture.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEEditor.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Display classes (Eric) : Window psoitioning
#include "LWindowPositioner.h"

	// Core : Editors : Generic editor
#include "RESession.h"

	// Core : Editors : Views : Data model
#include "VECatalogPage.h"
#include "VEComparators.h"
#include "VEModelObject.h"

	// Core : Editors : Views : User interface
#include "VEArrangeDialog.h"
#include "VEDrawingAgent.h"
#include "VEHierTable.h"
#include "VELayoutView.h"
#include "VEMenuResources.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>
#include "RMResource.h"
#include "RMMap.h"

	// Core : Utility classes
#include "UMemoryUtils.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"

	// PowerPlant : Utility classes
#include <UModalDialogs.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

LMenu*	VEEditor::sArrangeMenu			= nil;			// Arrange menu
LMenu*	VEEditor::sLayoutMenu			= nil;			// Layout menu


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_LayoutWindow		= 2000;
const PaneIDT	Pane_LayoutView			= 'VELV';

const ResIDT	PPob_HierarchyWindow	= 2020;
const PaneIDT	Pane_HierarchyView		= 'VEHT';

const ResIDT	PPob_ArrangeDialog		= 2300;
const ResIDT	PPob_AlignPalette		= 2350;

const ResIDT	STR_VEMiscStrings		= 2007;
const SInt16	str_HierarchyFor		= 1;


// ===========================================================================

#pragma mark *** VEEditor ***

// ---------------------------------------------------------------------------
//		* VEEditor(LCommander*, RESession&)
// ---------------------------------------------------------------------------
//	Constructor, specifying supercommander and editor session document

VEEditor::VEEditor(
	LCommander*		inSuper,
	RESession&		inSession)

: REEditor(inSuper, inSession)

{
	mHierWindow = nil;
	RegisterVEClasses();
}


// ---------------------------------------------------------------------------
//		* ~VEEditor
// ---------------------------------------------------------------------------
//	Destructor

VEEditor::~VEEditor()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu commands

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Enable the show hierarchy command.

void
VEEditor::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{

	// Validate pointers.

	ValidateThis_();
	
	// Dispatch commmands.

	switch (inCommand) {
		
		case cmd_ShowHierarchy:
			outEnabled = true;
			break;

		case cmd_Duplicate: {
			SInt32 selectCount = 0;
			if (mSelection != nil) {
				ValidateObject_(mSelection.GetObject());
				selectCount = mSelection->GetSelectedElements().GetCount();
				if (mSelection->IsSelected(mDataModelRoot))
					selectCount--;
			}
			outEnabled = selectCount >= 1;
			break;
		}
		
		case cmd_AlignLeft:
		case cmd_AlignHorizCenter:
		case cmd_AlignRight:
		case cmd_AlignTop:
		case cmd_AlignVertCenter:
		case cmd_AlignBottom:
		case cmd_SpreadHoriz:
		case cmd_SpreadHorizContainer:
		case cmd_SpreadHorizQuery:
		case cmd_SpreadVert:
		case cmd_SpreadVertContainer:
		case cmd_SpreadVertQuery:
		case cmd_AlignDialog: {
			SInt32 selectCount = 0;

			if ((mSelection != nil)
			  && mSelection->GetRootElement()->IsEnabled()
			  && !UMemoryUtils::MemoryIsLow()) {

				ValidateObject_(mSelection.GetObject());
				selectCount = mSelection->GetSelectedElements().GetCount();
				if (mSelection->IsSelected(mDataModelRoot))
					selectCount--;
			}
			outEnabled = selectCount >= 2;
			break;
		}
	
		default:
			REEditor::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
	
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Respond to the show hierarchy command.

Boolean
VEEditor::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Dispatch commands.

	switch (inCommand) {
	
		case cmd_ShowHierarchy:
			OpenHierWindowSelf();
			return true;
		
		case cmd_Duplicate:
			DuplicateSelection();
			return true;
		
		case cmd_AlignLeft:
		case cmd_AlignHorizCenter:
		case cmd_AlignRight:
		case cmd_AlignTop:
		case cmd_AlignVertCenter:
		case cmd_AlignBottom:

		case cmd_SpreadHoriz:
		case cmd_SpreadHorizContainer:
		case cmd_SpreadHorizUserAmt:
		case cmd_SpreadHorizQuery:
		case cmd_SpreadHorizContQuery:

		case cmd_SpreadVert:
		case cmd_SpreadVertContainer:
		case cmd_SpreadVertUserAmt:
		case cmd_SpreadVertQuery:
		case cmd_SpreadVertContQuery:
			AlignPanes(inCommand, ioParam);
			return true;
			
		case cmd_AlignDialog:
			ShowArrangeDialog();
			return true;
			
		default:
			return REEditor::ObeyCommand(inCommand, ioParam);
	}
}

Boolean VEEditor::HandleKeyPress(const EventRecord &inKeyEvent)
{
	if ((inKeyEvent.modifiers & cmdKey) && IsCmdChar(&inKeyEvent, 'J'))
	{
		OpenHierWindowSelf();
		return true;
	}
	else
		return false;
}


// ---------------------------------------------------------------------------
//		* DuplicateSelection
// ---------------------------------------------------------------------------
//	Duplicate the selected objects and place them in the layout offset
//	down and to the right by five pixels.

void
VEEditor::DuplicateSelection()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// Build a transaction for undo/redo.

	StDMTransactionBuilder trans(this, mSelection, cmd_Duplicate);

	// Keep a copy of selection so we know what to duplicate.
	// Disallow duplicating the root object.
	// Make sure there is no recursion in the selection.

	DMSelectionPtr oldSelection = ((DMSelection*) mSelection->Clone());
	ValidateObject_(oldSelection.GetObject());
	oldSelection->SelectRemove(mDataModelRoot);
	oldSelection->EliminateRecursion();

	// Empty the editor's selection so we can select the newly cloned items.

	mSelection->SelectNone();
	
	// Iterate through each item. Clone those that we can clone.

	DMIterator iter(oldSelection->GetSelectedElements());
	while (iter.NextElement()) {
	
		// Make sure this is a visual object item. Ignore it if not.
	
		VEModelObject* object = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		if (object == nil)
			continue;
		ValidateObject_(object);
		
		// Make a copy of the object.
		
		DMElementPtr objCloneDM = object->Clone();
		VEModelObject* objClone = (dynamic_cast<VEModelObject*>(objCloneDM.GetObject()));
		ValidateObject_(objClone);

		// Install the new one in the data model.
		
		DMElement* super = object->GetSuperElement();
		ValidateObject_(super);

		DM_DragPlacement place = { super, LArray::index_Last, false };
		super->HostElement(objCloneDM, &place);
		
		// Offset the duplicate by five pixels.
		
		objClone->MoveBy(5, 5);

		// Select the new item.
		
		mSelection->SelectAdd(objCloneDM);

	}

	// Nothing has gone wrong. Finalize the transaction.

	trans.Commit();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** alignment palette

// ---------------------------------------------------------------------------
//		* CreateAlignmentPalette								[static]
// ---------------------------------------------------------------------------
//	Create the global alignment palette. Called by
//	CAGenericTarget::ObeyCommand.

LWindow*
VEEditor::CreateAlignmentPalette()
{
	
	// Create the alignment palette window.

	StApplicationContext appContext;
	LWindow* palette = LWindow::CreateWindow(PPob_AlignPalette, LCommander::GetTopCommander());
	ValidateObject_(palette);
	
	// Position it and show it.
	
	palette->ProcessCommand(cmd_PositionWindow);
	palette->Show();

	// Return the window pointer.
	
	return palette;

}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* RegisterVEClasses										[static]
// ---------------------------------------------------------------------------
//	Register the classes that are used by the generic view editor.

void
VEEditor::RegisterVEClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register generic view editor view classes.
	
	URegistrar::RegisterClass(VEArrangeDialog::class_ID, (ClassCreatorFunc) VEArrangeDialog::CreateFromStream);
	URegistrar::RegisterClass(VEHierTable::class_ID, (ClassCreatorFunc) VEHierTable::CreateFromStream);
	URegistrar::RegisterClass(VELayoutView::class_ID, (ClassCreatorFunc) VELayoutView::CreateFromStream);

	// Register generic view editor data model classes.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->RegisterClass(VECatalogPage::class_ID, VECatalogPage::CreateFromStream);

}


// ===========================================================================

#pragma mark -
#pragma mark ** command hooks

// ---------------------------------------------------------------------------
//		* OpenEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Open the layout window for this editor. (Note that all view editors
//	share the same code for layout window, so this method is not normally
//	overriden.)

LWindow*
VEEditor::OpenEditorWindowSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mDataModelRoot.GetObject());
	ValidateSimpleObject_(mPrimaryResource);

	// Create a selection for window.
	
	mSession.GetSelectionForEditor(this, mSelection);

	// Create the window.

	LWindow* window = nil;
	{
		StApplicationContext appContext;
		window = LWindow::CreateWindow(GetLayoutViewPPob(), this);
		ValidateObject_(window);
	}

	// Set the window title to the resource name.
	
	LStr255 title;
	mSession.GetWindowTitleForEditor(this, title);
	window->SetDescriptor(title);
	
	// Attach layout view.

	VELayoutView* layoutView = (VELayoutView*) window->FindPaneByID(Pane_LayoutView);
	ValidateObject_(layoutView);
	layoutView->SetSelection(mSelection);
//	layoutView->SetRFMap(&(mPrimaryResource->GetRFMap()));

		//! TEMPORARY workaround follows
		
		RFMap* mapRF = (dynamic_cast<RFMap*>(&(mPrimaryResource->GetRMMap())));
		ValidateObject_(mapRF);
		layoutView->SetRFMap(mapRF);

	layoutView->SetRootObject(dynamic_cast<VEModelObject*>(mDataModelRoot.GetObject()));

	// Position & show the window.

	window->DoSetZoom(true);
	window->ProcessCommand(cmd_PositionWindow);
	window->Show();

	// Return the window.

	return window;

}

// ---------------------------------------------------------------------------
//		* GetLayoutViewPPob									[protected]
// ---------------------------------------------------------------------------
//	Override hook for subclasses that want a different Layout View

ResIDT
VEEditor::GetLayoutViewPPob()
{
	return PPob_LayoutWindow;
}

// ---------------------------------------------------------------------------
//		* OpenHierWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Open the hierarchy window for this editor. (Note that all view editors
//	share the same code for hierarchy window, so this method is not normally
//	overriden.)

LWindow*
VEEditor::OpenHierWindowSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mDataModelRoot.GetObject());

	// Make sure a hierarchy window doesn't already exist.
	// If it does, just bring it to front.
	
	if (mHierWindow != nil) {
		mHierWindow->Select();
		return mHierWindow;
	}

	// Create the window.

	{
		StApplicationContext appContext;
		mHierWindow = LWindow::CreateWindow(PPob_HierarchyWindow, this);
		ValidateObject_(mHierWindow);
	}

	// Set the window title to the resource name.
	
	UpdateEditorWindowTitleSelf();
	
	// Attach hierarchy view.

	VEHierTable* outlineTable = (dynamic_cast<VEHierTable*>(mHierWindow->FindPaneByID(Pane_HierarchyView)));
	ValidateObject_(outlineTable);
	outlineTable->SetSelection(mSelection);
	outlineTable->SetElement(mDataModelRoot);
	
	// Add default columns to hierarchy view.
	//	(In a later version, this will be user configurable.)
	
	AddDefaultHierColumns(outlineTable);

	// Position & show the window.

	mHierWindow->DoSetZoom(true);
	mHierWindow->ProcessCommand(cmd_PositionWindow);
	mHierWindow->Show();

	// Return the window.

	return mHierWindow;

}


// ---------------------------------------------------------------------------
//		* UpdateEditorWindowTitleSelf							[protected]
// ---------------------------------------------------------------------------
//	The resource's ID or title has changed. Update the window title to
//	match this change.

void
VEEditor::UpdateEditorWindowTitleSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&mSession);
	
	// If we have a hierarchy editor window, retitle it now.
	
	if (mHierWindow != nil) {
	
		ValidateObject_(mHierWindow);
		
		// Ask session for window title.
		
		LStr255 windowTitle;
		mSession.GetWindowTitleForEditor(this, windowTitle);
		
		LStr255 hierFor(STR_VEMiscStrings, str_HierarchyFor);
		hierFor += windowTitle;

		// Assign it to window.

		mHierWindow->SetDescriptor(hierFor);
	
	}
}


// ---------------------------------------------------------------------------
//		* AddDefaultHierColumns									[protected]
// ---------------------------------------------------------------------------
//	Override hook. Subclasses should add the default attribute columns
//	to the hierarchy window that's being created.

void
VEEditor::AddDefaultHierColumns(
	VEHierTable*	/* inHierTable */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* RemoveSubCommander									[protected]
// ---------------------------------------------------------------------------
//	If the removed commander happens to be the hierarchy window, forget
//	our pointer to the window.

void
VEEditor::RemoveSubCommander(
	LCommander*		inSub)
{

	// Validate pointers.

	ValidateThis_();

	// If the subcommander is the hierarchy window, forget our pointer.

	if (inSub == mHierWindow)
		mHierWindow = nil;

	// Let REEditor forget reference.

	REEditor::RemoveSubCommander(inSub);

}


// ===========================================================================

#pragma mark -
#pragma mark ** alignment implementation

// ---------------------------------------------------------------------------
//		* AlignPanes											[protected]
// ---------------------------------------------------------------------------
//	Given an alignment command, align a set of pane objects so that they
//	match the requested alignment.

void
VEEditor::AlignPanes(
	CommandT	inWhichAlign,
	void*		ioParam)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Scan the selected panes. Find bounding box of the group of panes.
	
	Rect bbox = { 0, 0, 0, 0 };
	DMIterator iter(mSelection->GetSelectedElements());
	
	LArray alignObjects(sizeof (VEModelObject*));
	SDimension32 sizeSum = { 0, 0 };
	
	while (iter.NextElement()) {
		VEModelObject* objVE = (dynamic_cast<VEModelObject*>(iter.CurrentElement()));
		ValidateObject_(objVE);
		
		if ((DMElement*) objVE == mDataModelRoot)		// can't align root object
			continue;

		VEDrawingAgent* agent = objVE->FindDrawingAgent();
		if (agent == nil)
			continue;
		ValidateObject_(agent);
		
		Rect frame;
		if (!agent->CalcPortFrameRect(frame))
			continue;

		alignObjects.InsertItemsAt(1, LArray::index_Last, &objVE);
		
		sizeSum.height += frame.bottom - frame.top;
		sizeSum.width += frame.right - frame.left;

		switch (inWhichAlign) {
			case cmd_SpreadHorizContainer:
			case cmd_SpreadVertContainer:
			case cmd_SpreadHorizContQuery:				//* 2.2d9: BUG FIX #591: add support for container queries
			case cmd_SpreadVertContQuery:
				agent->CalcPortFrameRectForSuperview(frame);
				break;
		}
		if (::EmptyRect(&bbox))
			bbox = frame;
		else
			::UnionRect(&bbox, &frame, &bbox);
	}
	
	// If no panes found, bail out now.

	if (alignObjects.GetCount() < 2)
		return;

	// Sort objects (if needed).
	
	float spreadAmt = 0;
	float spreadLoc = 0;

	switch (inWhichAlign) {
		case cmd_SpreadHoriz:
		case cmd_SpreadHorizContainer:
		case cmd_SpreadHorizQuery:
		case cmd_SpreadHorizContQuery:				//* 2.2d9: BUG FIX #591: add support for container queries
			spreadAmt = (bbox.right - bbox.left) - sizeSum.width;
			spreadAmt /= alignObjects.GetCount() - 1;
		case cmd_SpreadHorizUserAmt:
			alignObjects.SetComparator(new VEHorizComparator);
			alignObjects.Sort();
			spreadLoc = bbox.left + .49;			// to adjust for rounding
			break;

		case cmd_SpreadVert:
		case cmd_SpreadVertContainer:
		case cmd_SpreadVertQuery:
		case cmd_SpreadVertContQuery:				//* 2.2d9: BUG FIX #591: add support for container queries
			spreadAmt = (bbox.bottom - bbox.top) - sizeSum.height;
			spreadAmt /= alignObjects.GetCount() - 1;
		case cmd_SpreadVertUserAmt:
			alignObjects.SetComparator(new VEVertComparator);
			alignObjects.Sort();
			spreadLoc = bbox.top + .49;				// to adjust for rounding
			break;
	}
	
	// If it's a query, answer it now.
	
	SInt16* queryResponse = (SInt16*) ioParam;
	switch (inWhichAlign) {
		case cmd_SpreadHorizQuery:
		case cmd_SpreadVertQuery:
		case cmd_SpreadHorizContQuery:				//* 2.2d9: BUG FIX #591: add support for container queries
		case cmd_SpreadVertContQuery:
			*queryResponse = static_cast<SInt16>(spreadAmt + .49);
			return;
		
		case cmd_SpreadHorizUserAmt:
		case cmd_SpreadVertUserAmt:
			spreadLoc -= .49;
			spreadAmt = static_cast<SInt16>(reinterpret_cast<SInt32>(ioParam));			//* 2.2d9: BUG FIX #545: accept user amt
	}
	
	// Set up the transaction.
	
	CommandT alignCommand = inWhichAlign;
	
	switch (inWhichAlign) {
		case cmd_SpreadHorizUserAmt:
		case cmd_SpreadVertUserAmt:
			alignCommand = cmd_Nothing;
			break;
	}

	StDMTransactionBuilder trans(this, mSelection, alignCommand);
	
	// Okay, move the panes.
	
	LArrayIterator alignIter(alignObjects);
	VEModelObject* alignObj;

	while (alignIter.Next(&alignObj)) {

		ValidateObject_(alignObj);

		VEDrawingAgent* agent = alignObj->FindDrawingAgent();
		ValidateObject_(agent);
		
		Rect frame;
		if (!agent->CalcPortFrameRect(frame))
			continue;

		switch (inWhichAlign) {
			
			case cmd_AlignLeft:
				alignObj->MoveBy(bbox.left - frame.left, 0);
				break;
				
			case cmd_AlignHorizCenter:
				SInt16 bboxHCtr;
				bboxHCtr = (bbox.right + bbox.left) >> 1;
				SInt16 frameHCtr;
				frameHCtr = (frame.right + frame.left) >> 1;
				alignObj->MoveBy(bboxHCtr - frameHCtr, 0);
				break;
				
			case cmd_AlignRight:
				alignObj->MoveBy(bbox.right - frame.right, 0);
				break;

			case cmd_SpreadHoriz:
			case cmd_SpreadHorizContainer:
			case cmd_SpreadHorizUserAmt:
				alignObj->MoveBy(static_cast<SInt32>(spreadLoc - frame.left), 0);
				spreadLoc += frame.right - frame.left;
				spreadLoc += spreadAmt;
				break;

			case cmd_AlignTop:
				alignObj->MoveBy(0, bbox.top - frame.top);
				break;
			
			case cmd_AlignVertCenter:
				SInt16 bboxVCtr;
				bboxVCtr = (bbox.bottom + bbox.top) >> 1;
				SInt16 frameVCtr;
				frameVCtr = (frame.bottom + frame.top) >> 1;
				alignObj->MoveBy(0, bboxVCtr - frameVCtr);
				break;
				
			case cmd_AlignBottom:
				alignObj->MoveBy(0, bbox.bottom - frame.bottom);
				break;

			case cmd_SpreadVert:
			case cmd_SpreadVertContainer:
			case cmd_SpreadVertUserAmt:
				alignObj->MoveBy(0, static_cast<SInt32>(spreadLoc - frame.top));
				spreadLoc += frame.bottom - frame.top;
				spreadLoc += spreadAmt;
				break;
		}
	}
	
	trans.Commit();

}


// ---------------------------------------------------------------------------
//		* ShowArrangeDialog										[protected]
// ---------------------------------------------------------------------------
//	Run the dialog that offers all of the alignment choices.

void
VEEditor::ShowArrangeDialog()
{

	// Validate pointers.

	ValidateThis_();

	// Create alignment data structure. This is filled
	// in once the dialog is completed.

	SAlignmentData arrangeData;

	// Run the dialog.

	{
	
		// Create the dialog handler.

		StApplicationContext appContext;
		StDialogHandler theArrangeDialog(PPob_ArrangeDialog, this);
		
		// Get a reference to the dialog box object.
		
		VEArrangeDialog* theDialog = (dynamic_cast<VEArrangeDialog*>(theArrangeDialog.GetDialog()));
		ValidateObject_(theDialog);
				
		// Restore window position. It was saved last time we ran this dialog.
		// Then show the dialog.
		
		theDialog->ProcessCommand(cmd_PositionWindow);
		theDialog->Show();
		
		// Loop around until we get a message from the Apply or Cancel buttons.

		while (1) {

			// Process an event.
			
			MessageT theMessage = theArrangeDialog.DoDialog();
			
			// If Cancel or Apply buttons were hit, close the dialog.
			
			if ((theMessage == msg_Cancel) || (theMessage == msg_OK)) {
			
				// Record window position.
				
				theDialog->ProcessCommand(cmd_RecordPosition);

				// Save current alignment settings so that next time the dialog
				// is opened, these settings can be restored.

				theDialog->SaveArrangementData ();

				// If Cancel was hit, bail out now.

				if (theMessage == msg_Cancel)
					return;
				
				// Get the alignment settings from the dialog.
				
				theDialog->GetArrangementData(arrangeData);
				
				// Exit the dialog loop.
				
				break;
				
			}
		}
	}

	// Make the calls to get the changes in the arrangement data
	// processed by the associated commands.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_AlignDialog);

	if (arrangeData.alignHorizontal != alignHoriz_None)
		ProcessCommand(arrangeData.alignHorizontal, (void*) arrangeData.horizSpread);
	if (arrangeData.alignVertical != alignVert_None)
		ProcessCommand(arrangeData.alignVertical, (void*) arrangeData.vertSpread);

	trans.Commit();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** dynamic menus

// ---------------------------------------------------------------------------
//		* PutOnDuty												[proteted]
// ---------------------------------------------------------------------------
//	Add the Arrange and Layout menus to the menu bar.

void
VEEditor::PutOnDuty( LCommander* /* inNewTarget */)
{

	// Validate pointers.
	
	ValidateThis_();

	// Load the menu resources if this hasn't been done already.

	{
		StApplicationContext appContext;

		if (sArrangeMenu == nil)
			sArrangeMenu = new LMenu(menu_Arrange);
		
		if (sLayoutMenu == nil)
			sLayoutMenu = new LMenu(menu_Layout);
	}
	
	// Install these menus in the menu bar.
	
	LMenuBar* theBar = LMenuBar::GetCurrentMenuBar();
	ValidateObject_(theBar);
	ValidateSimpleObject_(sArrangeMenu);
	ValidateSimpleObject_(sLayoutMenu);

	theBar->InstallMenu(sArrangeMenu, menu_Windows);
	theBar->InstallMenu(sLayoutMenu, menu_Windows);
	
}


// ---------------------------------------------------------------------------
//		* TakeOffDuty											[protected]
// ---------------------------------------------------------------------------
//	Remove the Arrange and Layout menus from the menu bar.

void
VEEditor::TakeOffDuty()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(sArrangeMenu);
	ValidateSimpleObject_(sLayoutMenu);

	// Remove menus from the menu bar.

	LMenuBar* theBar = LMenuBar::GetCurrentMenuBar();
	ValidateObject_(theBar);
	
	theBar->RemoveMenu(sArrangeMenu);
	theBar->RemoveMenu(sLayoutMenu);

}
