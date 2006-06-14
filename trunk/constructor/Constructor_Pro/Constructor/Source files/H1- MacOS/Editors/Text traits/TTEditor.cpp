// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTEditor.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTEditor.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 03/18/97   Time: 15:27
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Removed RFResource-specific code.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:18
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:44
//	Updated in $/Constructor/Source files/Editors/Text traits
//	Fixed a bug in OpenEditorWindowSelf.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:23
//	Updated in $/Constructor/Source files/Editors/Text traits
//	Resource editor shell overhaul.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/05/96   Time: 09:37
//	Updated in $/Constructor/Source files/Editors/Text traits
//	Fixed a commenting error.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/03/96   Time: 11:35
//	Updated in $/Constructor/Source files/Editors/Text traits
//	Changed internal CTYP resources to DSPC.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/30/96   Time: 21:49
//	Checked in '$/Constructor/Source files/Editors/Text traits'
//	Comment: 
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/30/96   Time: 21:07
//	Checked in '$/Constructor/Source files/Editors/Text traits'
//	Comment: 
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/30/96   Time: 18:28
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#include "TTEditor.h"

	// Core : Data model : Core objects
#include "DMObject.h"
#include "DMReanimator.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Editors : Generic editor
#include "RESession.h"

	// Core : Property inspector
#include "UPropertyInspector.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Editors : Text traits
#include "TTInspectorTable.h"
#include "TTSampleCaption.h"

	// MacOS : Resource manager
#include "RFResource.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_TxtrEditor			= 5000;
const PaneIDT	Pane_TxtrInspector		= 'TXIT';
const PaneIDT	Pane_TxtrSample			= 'TTSC';

const ResIDT	DSPC_TxtrData			= 5000;


// ===========================================================================

#pragma mark *** TTEditor ***

// ---------------------------------------------------------------------------
//		* TTEditor(LCommander*, RESession&)
// ---------------------------------------------------------------------------
//	Constructor

TTEditor::TTEditor(
	LCommander*		inSuper,
	RESession&		inSession)

: REEditor(inSuper, inSession)

{

	// Validate pointers.

	ValidateObject_(inSuper);
	ValidateObject_(&inSession);

	// Register Txtr editor classes.
	
	RegisterTTClasses();

}


// ---------------------------------------------------------------------------
//		* ~TTEditor
// ---------------------------------------------------------------------------
//	Destructor

TTEditor::~TTEditor()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** session management implementation overrides

// ---------------------------------------------------------------------------
//		* ReadResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read the 'Txtr' resource data format.

void
TTEditor::ReadResourceDataSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(mPrimaryResource);

	// Build input stream for the resource.
	
	Handle txtrData = mPrimaryResource->GetResData();
	ValidateHandle_(txtrData);
	
	LHandleStream txtrStream(txtrData);

	// Build a text traits data object.
	
	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->ElementsFromResource('DSPC', DSPC_TxtrData, mDataModelRoot);
	ValidateObject_(mDataModelRoot.GetObject());
	
	// Use it to read data from resource.
	
	DMObject* rootDMO = (dynamic_cast<DMObject*>(mDataModelRoot.GetObject()));
	ValidateObject_(rootDMO);
	
	rootDMO->ReadStreamData(&txtrStream);
	
}


// ---------------------------------------------------------------------------
//		* WriteResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Write the 'Txtr' resource data format.

void
TTEditor::WriteResourceDataSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());
	ValidateSimpleObject_(mPrimaryResource);

	// Build output stream for the resource.
	
	LHandleStream txtrStream;

	// Ask the txtr data object to spill its data.
	
	DMObject* rootDMO = (dynamic_cast<DMObject*>(mDataModelRoot.GetObject()));
	ValidateObject_(rootDMO);
	
	rootDMO->WriteStreamData(&txtrStream);
	
	// Write the 'Txtr' resource data.
	
	mPrimaryResource->SetResData(txtrStream.GetDataHandle());

}


// ---------------------------------------------------------------------------
//		* OpenEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Create the property window for a text traits resource.

LWindow*
TTEditor::OpenEditorWindowSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());

	// Create the window.

	LWindow* window = nil;
	{
		StApplicationContext appContext;
		window = LWindow::CreateWindow(PPob_TxtrEditor, this);
		ValidateObject_(window);
	}

	// Set the window title to the resource name.
	
	LStr255 title;
	mSession.GetWindowTitleForEditor(this, title);
	window->SetDescriptor(title);

	// Attach text traits property view.

	TTInspectorTable* txtrView = (dynamic_cast<TTInspectorTable*>
				(window->FindPaneByID(Pane_TxtrInspector)));

	ValidateObject_(txtrView);
	txtrView->SetElement(mDataModelRoot);

	// Find text traits sample caption.
	
	TTSampleCaption* txtrSample = (dynamic_cast<TTSampleCaption*>
				(window->FindPaneByID(Pane_TxtrSample)));
	
	ValidateObject_(txtrSample);
	txtrSample->SetObject(dynamic_cast<DMObject*>(mDataModelRoot.GetObject()));

	// Position & show the window.

	window->ProcessCommand(cmd_PositionWindow);
	window->Show();

	return window;

}


// ===========================================================================

#pragma mark -
#pragma mark ** session management implementation overrides

// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Overriden to cause the resource to be updated every time a property
//	of the Txtr changes. This causes the PPob editor to redraw every pane
//	that uses this Txtr, so the effects of the change are seen immediately
//	throughout Constructor.

void
TTEditor::ValueChanged(
	DM_ValueChange*		inMessage)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMessage->mObject);
	
	// If it's our object, write the resource data.
	
	if (inMessage->mObject == mDataModelRoot.GetObject())
		WriteResourceData();
	
	// Otherwise let basic editor listen for messages.
	
	REEditor::ValueChanged(inMessage);

}


// ===========================================================================

#pragma mark -
#pragma mark ** class registration

// ---------------------------------------------------------------------------
//		* RegisterTTClasses								[static, protected]
// ---------------------------------------------------------------------------
//	Register the display and data model classes that are used to
//	implement the Txtr editor.

void
TTEditor::RegisterTTClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register property inspector classes.
	
	UPropertyInspector::RegisterPIClasses();

	// Register Txtr editor view classes.

	URegistrar::RegisterClass(TTInspectorTable::class_ID, (ClassCreatorFunc) TTInspectorTable::CreateFromStream);
	URegistrar::RegisterClass(TTSampleCaption::class_ID, (ClassCreatorFunc) TTSampleCaption::CreateFromStream);

}
