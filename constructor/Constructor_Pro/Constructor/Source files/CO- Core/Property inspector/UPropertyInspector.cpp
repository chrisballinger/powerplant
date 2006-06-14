// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UPropertyInspector.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 05/14/96
//     $Date: 2006/01/18 01:33:24 $
//	$History: UPropertyInspector.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/28/97   Time: 13:03
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for property inspectors in the Window menu.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/18/97   Time: 18:10
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added a missing #include.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/18/97   Time: 16:03
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for multiple property inspector windows.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:07p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 18:04
//	Updated in $/Constructor/Source files/Property inspector
//	Changed resource ID allocation to avoid conflict with Display classes
//	(Eric) folder. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:44
//	Updated in $/Constructor/Source files/Property inspector
//	Improved pointer validation. Registered PIFontPopup class.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "UPropertyInspector.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Property inspector
#include "PIFontPopup.h"
#include "PIInspectorTable.h"
#include "PILabelPopup.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

LWindow* UPropertyInspector::sInspectorWindow = nil;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_PropertyInspector	= 21000;
const ResIDT	PPob_PropertyInspectorInWindow	= 21001;
const PaneIDT	Pane_PIInspectorTable	= 'INSP';


// ===========================================================================

#pragma mark *** UPropertyInspector ***

// ---------------------------------------------------------------------------
//		* InspectSelection										[static]
// ---------------------------------------------------------------------------
//	Inspect the current selection. Route undo/redo actions to the commander
//	object that is provided.

void
UPropertyInspector::InspectSelection(
	DMSelection*	inSelection,
	LCommander*		inUndoHost,
	Boolean 		inUseHostAsCommander,
	Boolean			inPlaceInWindowMenu)
{

	// Validate pointers.
	
	if (inSelection != nil)
		ValidateObject_(inSelection);
	if (inUndoHost != nil)
		ValidateObject_(inUndoHost);
	
	// See if there's a free-floating inspector window for this object.
	
	if (inSelection != nil) {
		if (inSelection->GetSelectedElements().GetCount() >= 1) {

			DMElement* element = nil;
			inSelection->GetSelectedElements().FetchItemAt(1, &element);
			ValidateObject_(element);

			if (SelectInspectorWindow(element))
				return;
		
		}
	}

	// Create an inspector window if none is already open.
	
	Boolean createdWindow = false;
	if (sInspectorWindow == nil) {
		sInspectorWindow = CreateInspectorWindow(
			(inUseHostAsCommander) ? inUndoHost : nil,inPlaceInWindowMenu);
		createdWindow = true;
	}

	// Sanity check: Make sure we now have an inspector window.
	
	ValidateObject_(sInspectorWindow);
	LWindow* newWindow = sInspectorWindow;

	// Configure the inspector to use the named selection.
	
	ConfigureInspectorWindow(inSelection, inUndoHost);

	// Position & show the window.

	if (createdWindow) {
		newWindow->DoSetZoom(true);
		newWindow->ProcessCommand(cmd_PositionWindow);
		newWindow->Show();
	}
	else
		newWindow->Select();
	
}

// ---------------------------------------------------------------------------
//		* SetTopSelection										[static]
// ---------------------------------------------------------------------------
//	The topmost window has changed. Switch selections and inspected objects
//	to match it.

void
UPropertyInspector::SetTopSelection(
	DMSelection*	inSelection,
	LCommander*		inUndoHost)
{

	// Validate pointers.
	
	if (inSelection != nil)
		ValidateObject_(inSelection);
	if (inUndoHost != nil)
		ValidateObject_(inUndoHost);

	// Set up the inspector window accordingly.

	ConfigureInspectorWindow(inSelection, inUndoHost);

}


// ---------------------------------------------------------------------------
//		* RegisterPIClasses										[static]
// ---------------------------------------------------------------------------
//	Register the PowerPlant display classes that are used by the inspector.

void
UPropertyInspector::RegisterPIClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register PI display classes.
	
	URegistrar::RegisterClass(PIFontPopup::class_ID, (ClassCreatorFunc) PIFontPopup::CreateFromStream);
	URegistrar::RegisterClass(PIInspectorTable::class_ID, (ClassCreatorFunc) PIInspectorTable::CreateFromStream);
	URegistrar::RegisterClass(PILabelPopup::class_ID, (ClassCreatorFunc) PILabelPopup::CreateFromStream);

}


// ---------------------------------------------------------------------------
//		* CreateInspectorWindow							[protected, static]
// ---------------------------------------------------------------------------
//	Build a window for the property inspector.

LWindow*
UPropertyInspector::CreateInspectorWindow(
	LCommander *inCommander,
	Boolean 	inPlaceInWindowMenu)
{

	if (inCommander == nil)
		inCommander = LCommander::GetTopCommander();
	// Ensure that classes are registered.
	
	RegisterPIClasses();

	// Create the window.

	StApplicationContext appContext;
	LWindow* newWindow = LWindow::CreateWindow(
		(inPlaceInWindowMenu) ? PPob_PropertyInspectorInWindow : PPob_PropertyInspector,inCommander);
	ValidateObject_(newWindow);
	
	// Return it to client.
	
	return newWindow;
	
}


// ---------------------------------------------------------------------------
//		* ConfigureInspectorWindow						[protected, static]
// ---------------------------------------------------------------------------
//	Make sure the inspector window is updated to match the selection
//	and commander that are named.

void
UPropertyInspector::ConfigureInspectorWindow(
	DMSelection*	inSelection,
	LCommander*		inUndoHost)
{

	// Validate pointers.
	
	if (inSelection != nil)
		ValidateObject_(inSelection);
	if (inUndoHost != nil)
		ValidateObject_(inUndoHost);

	// Sanity check: Make sure there's an inspector window.
	
	if (sInspectorWindow == nil)
		return;
	ValidateObject_(sInspectorWindow);

	// Configure the inspector to use the named selection.
	
	PIInspectorTable* inspectorRoot = (PIInspectorTable*) sInspectorWindow->FindPaneByID(Pane_PIInspectorTable);
	ValidateObject_(inspectorRoot);

	inspectorRoot->SetFollowSelection(inSelection, inUndoHost);

}


// ---------------------------------------------------------------------------
//		* ClosingInspectorWindow						[protected, static]
// ---------------------------------------------------------------------------
//	Called by the inspector table when it is going away. Informs the
//	UPropertyInspector unit that there is no longer an inspector window.

void
UPropertyInspector::ClosingInspectorWindow(
	LWindow*	inWindow)
{

	// Validate pointers.

	ValidateObject_(inWindow);
	
	// If this was the global inspector window, forget our pointer to it.

	if (inWindow == sInspectorWindow)
		sInspectorWindow = nil;

}


// ---------------------------------------------------------------------------
//		* SelectInspectorWindow							[protected, static]
// ---------------------------------------------------------------------------
//	If there is an inspector window for the specified data model element,
//	bring this window to the front and return true. Otherwise, return false.

Boolean
UPropertyInspector::SelectInspectorWindow(
	DMElement*	inElement)
{
	
	// Validate pointers.
	
	ValidateObject_(inElement);
	
	// See if there's an inspector for this element.
	
	PIInspectorTable* table = (PIInspectorTable*)
								inElement->FindUserInterfaceObject(PIInspectorTable::object_ID);
	if (table == nil)
		return false;
	
	// There's a table. Select its window.
	
	LWindow* window = LWindow::FetchWindowObject(GetWindowFromPort(table->GetMacPort()));
	ValidateObject_(window);
	
	window->Select();
	return true;
	
}
