// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SLEditor.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/02/96
//     $Date: 2006/01/18 01:33:51 $
//  $History: SLEditor.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 03/18/97   Time: 15:27
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Removed RFResource-specific code.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:09
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/28/97   Time: 20:03
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/04/96   Time: 16:37
//	Created in $/Constructor/Source files/Editors/String list
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "SLEditor.h"

	// Core : Data model : Attributes
#include "DMListAttribute.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"

	// Core : Display classes (Eric) : Window positioner
#include "LWindowPositioner.h"

	// Core : Editors : Generic editor
#include "RESession.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Editors : String list
#include "SLStringListTable.h"

	// MacOS : Resource manager
#include "RFResource.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	DSPC_StrData		= 5200;

const ResIDT	PPob_StrEditor		= 5200;
const PaneIDT	Pane_HierarchyView	= 'SLST';


// ===========================================================================

#pragma mark *** SLEditor ***

// ---------------------------------------------------------------------------
//		* SLEditor()
// ---------------------------------------------------------------------------
//	Constructor

SLEditor::SLEditor(
	LCommander*		inSuper,
	RESession&		inSession)

: REEditor(inSuper, inSession)

{
	RegisterSLClasses();
}


// ---------------------------------------------------------------------------
//		* ~SLEditor
// ---------------------------------------------------------------------------
//	Destructor

SLEditor::~SLEditor()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** editor override hooks

// ---------------------------------------------------------------------------
//		* ReadResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read the STR# resource.

void
SLEditor::ReadResourceDataSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(mPrimaryResource);
//	SignalIfNot_(mPrimaryResource->GetResType() == 'STR#');

	// Build input stream for the resource.
	
	Handle strData = mPrimaryResource->GetResData();
	ValidateHandle_(strData);
	
	LHandleStream strStream(strData);

	// Build a dynamic list object.
	
	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->ElementsFromResource('DSPC', DSPC_StrData, mDataModelRoot);
	ValidateObject_(mDataModelRoot.GetObject());
	
	// Use it to read data from resource.
	
	DMListAttribute* rootDML = (dynamic_cast<DMListAttribute*>(mDataModelRoot.GetObject()));
	ValidateObject_(rootDML);
	
	rootDML->ReadStreamData(&strStream);

	// Get a selection for this menu editor.
	
	mSession.GetSelectionForEditor(this, mSelection);
	ValidateObject_(mSelection.GetObject());

	if (mSelection->GetRootElement() == nil)
		mSelection->SetRootElement(mDataModelRoot);

}


// ---------------------------------------------------------------------------
//		* WriteResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Write the 'STR#' resource format.

void
SLEditor::WriteResourceDataSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());
	ValidateSimpleObject_(mPrimaryResource);

	// Build output stream for the resource.
	
	LHandleStream strStream;

	// Ask the string list attribute to spill its data.
	
	DMListAttribute* rootDML = (dynamic_cast<DMListAttribute*>(mDataModelRoot.GetObject()));
	ValidateObject_(rootDML);
	
	rootDML->WriteStreamData(&strStream);
	
	// Write the 'STR#' resource data.
	
	mPrimaryResource->SetResData(strStream.GetDataHandle());

}


// ---------------------------------------------------------------------------
//		* OpenEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Open the string list editor window.

LWindow*
SLEditor::OpenEditorWindowSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());

	// Create the window.

	LWindow* window = nil;
	{
		StApplicationContext appContext;
		window = LWindow::CreateWindow(PPob_StrEditor, this);
		ValidateObject_(window);
	}

	// Set the window title to the resource name.
	
	LStr255 title;
	mSession.GetWindowTitleForEditor(this, title);
	window->SetDescriptor(title);

	// Attach hierarchy view.

	SLStringListTable* outlineTable = (dynamic_cast<SLStringListTable*>(window->FindPaneByID(Pane_HierarchyView)));
	ValidateObject_(outlineTable);
	outlineTable->SetSelection(mSelection);
	outlineTable->SetElement(mDataModelRoot);
	
	// Position & show the window.

	window->DoSetZoom(true);
	window->ProcessCommand(cmd_PositionWindow);
	window->Show();

	return window;

}


// ===========================================================================

#pragma mark -
#pragma mark ** editor override hooks

// ---------------------------------------------------------------------------
//		* RegisterSLClasses								[static, protected]
// ---------------------------------------------------------------------------
//	Register the display classes used by the string list editor.

void
SLEditor::RegisterSLClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register string list view display classes.
	
	URegistrar::RegisterClass(SLStringListTable::class_ID, (ClassCreatorFunc) SLStringListTable::CreateFromStream);

}
