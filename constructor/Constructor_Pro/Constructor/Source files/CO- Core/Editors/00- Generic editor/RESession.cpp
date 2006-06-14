// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RESession.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/10/96
//	   $Date: 2006/01/18 01:33:06 $
//	$History: RESession.cpp $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Improved commenting.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:46
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Added code to TearDownEditor to ensure data model inspectors get torn
//	down at the right time.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/05/97    Time: 5:57p
//	Updated in $/ConstructorWin/Sources
//	Integrated changes for building with MSVC
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/07/97   Time: 17:52
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Rolled in an update from Clint 01/05/97.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 20:17
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added code to GetEditorForResource to enable or disable editor as
//	appropriate.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/25/96   Time: 21:33
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Improved error handling in the create editor process.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/11/96   Time: 10:40
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added FinishCreateSelf method to REEditor.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:17
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:58
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RESession.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Documents : Project document
#include "PDContainer.h"
#include "PDDocument.h"
#include "PDResource.h"

	// Core : Editors : Generic editor
#include "REEditor.h"

	// Core : Resource manager
#include "RMIdentifier.h"
#include "RMMap.h"
#include "RMResource.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : Utility classes
#include <UModalDialogs.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_CantOpenResource		= 14400;


// ===========================================================================

#pragma mark *** RESession ***

// ---------------------------------------------------------------------------
//		* RESession(...)
// ---------------------------------------------------------------------------
//	Constructor

RESession::RESession(
	PDDocument*		inDocument,
	PDContainer*	inContainer,
	RMMap*			inMap)

: mDeletedEditors(sizeof (REEditor*))

{

	// Validate pointers.

	ValidateObject_(inDocument);
	ValidateObject_(inContainer);
	ValidateObject_(inMap);

	// Save configuration.

	mDocument = inDocument;
	mContainer = inContainer;
	mMap = inMap;

}


// ---------------------------------------------------------------------------
//		* ~RESession
// ---------------------------------------------------------------------------
//	Destructor

RESession::~RESession()
{

	// Delete any lingering editors.

	EventRecord x;
	SpendTime(x);

}


// ===========================================================================

#pragma mark -
#pragma mark ** editor/resource management

// ---------------------------------------------------------------------------
//		* GetEditorForResource
// ---------------------------------------------------------------------------
//	Find or create an appropriate editor for the resource type and ID
//	specified.

REEditor*
RESession::GetEditorForResource(
	ResType		inResType,
	ResIDT		inResID,
	Boolean		inCreateIfNeeded,
	Boolean		inSuppressErrors)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mContainer);
	ValidateObject_(mMap);

	// First find the data model resource.
	
	PDResource* thePDRsrc = mContainer->FindResource(
					RMIdentifier(inResType), RMIdentifier(inResID), false);

	if (thePDRsrc == nil)
		return nil;
	ValidateObject_(thePDRsrc);
	
	// Find the low-level resource.
	
	RMResource* theRMRsrc = mMap->FindResource(
					RMIdentifier(inResType), RMIdentifier(inResID), false);
	if (theRMRsrc == nil)
		return nil;
	ValidateObject_(theRMRsrc);

	// Okay, we have both. Ask the data model if it has an editor already.
	
	REEditor* theEditor = (REEditor*) thePDRsrc->FindUserInterfaceObject('edit');
	if (theEditor != nil) {
		ValidateObject_(theEditor);
		return theEditor;
	}
	
	// No editor exists. See if user wanted us to create one.
	
	if (inCreateIfNeeded) {
	
		ExceptionCode error = 0;
	
		try {
	
			// The document object knows the ResType->editor type mapping.
			// Let it create the editor object.
	
			theEditor = mDocument->CreateEditorForResource(inResType, inResID);
	
			// If an editor was created, configure it for this resource.
	
			if (theEditor != nil) {
				ValidateObject_(theEditor);
				if (thePDRsrc->IsEnabled())
					theEditor->EnableSelf();
				else
					theEditor->DisableSelf();
				theEditor->mSuppressErrors = inSuppressErrors;
				theEditor->FinishCreateSelf();
				theEditor->SetPrimaryResource(theRMRsrc);
				theEditor->ReadResourceData();
				AcquireResourceForEditing(theEditor, inResType, inResID, false);
			}
		}
		catch (const LException& inError) {
			error = inError.GetErrorCode();
		}
		catch(...) {
			error = err_CantOpen;
		}

		if (error != 0) {

			// Show editor failed dialog.
			
			if ((!inSuppressErrors) && (error != err_CantOpenSilent))
				ShowCantOpenDialog();
			
			// Destroy the editor.
			
			if (theEditor != nil) {
				ValidateObject_(theEditor);
				TearDownEditor(theEditor);
			}
			theEditor = nil;
		}
	}
	
	// Return the editor that we created.
	
	return theEditor;
	
}


// ---------------------------------------------------------------------------
//		* TearDownEditor
// ---------------------------------------------------------------------------
//	Destroy an editor. (Typically called only by the editor itself.)

void
RESession::TearDownEditor(
	REEditor*	inEditor)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inEditor);
	
	// Add this editor to list to be deleted.
	
	if (mDeletedEditors.FetchIndexOf(&inEditor) == 0)
		mDeletedEditors.InsertItemsAt(1, LArray::index_Last, &inEditor);
	
	// Close down data model.
	
	DMElement* dmRoot = inEditor->GetRootElement();
	if (dmRoot != nil) {
		ValidateObject_(dmRoot);
		dmRoot->GoingAway();
	}
	
	// Editors are deleted by SpendTime. Make sure we get there.

	StartRepeating();

}


// ---------------------------------------------------------------------------
//		* GetResource
// ---------------------------------------------------------------------------
//	Find a resource in the low-level resource map. Should be used for
//	resources which are not changed by an editor.

RMResource*
RESession::GetResource(
	ResType		inResType,
	ResIDT		inResID,
	Boolean		inCreateIfNeeded)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mMap);
	
	// Use low-level resource map to find resource.
	
	return mMap->FindResource(inResType, inResID, inCreateIfNeeded);

}


// ---------------------------------------------------------------------------
//		* AcquireResourceForEditing
// ---------------------------------------------------------------------------
//	Find a resource in the low-level resource map. Should be used for
//	resources which may be changed by an editor. If another editor has
//	this resource open for editing, it is closed.

RMResource*
RESession::AcquireResourceForEditing(
	REEditor*	inEditor,
	ResType		inResType,
	ResIDT		inResID,
	Boolean		inCreateIfNeeded)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inEditor);

	// Find the low-level resource.
	
	RMResource* theRMRsrc = mMap->FindResource(inResType, inResID, inCreateIfNeeded);
	if (theRMRsrc == nil)
		return nil;
	ValidateObject_(theRMRsrc);

	// Find the data model resource.
	
	PDResource* thePDRsrc = mContainer->FindResource(inResType, inResID, inCreateIfNeeded);
	if (thePDRsrc != nil) {

		ValidateObject_(thePDRsrc);
	
		// Found one. If there already exists an editor for this resource, kill it.
		
		REEditor* theEditor = (REEditor*) thePDRsrc->FindUserInterfaceObject('edit');
		if (theEditor != inEditor) {
			
			// Another editor is working on this resource. Close it.

			if (theEditor != nil) {
				ValidateObject_(theEditor);
				theEditor->CloseEditorWindow();
			}
			
			// Now add this resource to editor's private list of data model resources.
		
			inEditor->mResources.InsertItemAt(LArray::index_Last, thePDRsrc);
			thePDRsrc->AddListener(inEditor);

		}
		
		// Tell the resource's listeners that an editor was added.
		
		thePDRsrc->BroadcastMessage(msg_HasEditor, thePDRsrc);
		
	}
	
	// Return a pointer only to the low-level resource.
	
	return theRMRsrc;

}


// ---------------------------------------------------------------------------
//		* ReleaseResourceFromEditing
// ---------------------------------------------------------------------------
//	Called by REEditor subclass when it no longer wants to edit a resource's
//	data. Should not be called for the editor's primary resource.

void
RESession::ReleaseResourceFromEditing(
	REEditor*		inEditor,
	RMResource*		inResource)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inEditor);
	ValidateObject_(inResource);
	
	// Find the data model resource.
	
	RMIdentifier* resType = inResource->GetResTypeRM();
	RMIdentifier* resID = inResource->GetResIDRM();
	
	ValidateObject_(resType);
	ValidateObject_(resID);
	
	PDResource* thePDRsrc = mContainer->FindResource(*resType, *resID, false);
	delete resType;
	delete resID;
	
	if (thePDRsrc != nil) {

		ValidateObject_(thePDRsrc);
		
		// Found it. Confirm that this is the editor for this resource.

		REEditor* theEditor = (REEditor*) thePDRsrc->FindUserInterfaceObject('edit');
		if (theEditor != inEditor) {
			SignalCStr_("One editor can't release another's resource.");
			return;
		}
		
		// Okay, the release request is legitimate. Release the resource.
		
		inEditor->mResources.RemoveItem(thePDRsrc);
		thePDRsrc->RemoveListener(inEditor);
		
		// Tell the resource's listeners that an editor was removed.
		
		thePDRsrc->BroadcastMessage(msg_HasEditor, thePDRsrc);

	}
}


// ---------------------------------------------------------------------------
//		* GetWindowTitleForEditor
// ---------------------------------------------------------------------------
//	Creates an appropriate window title for the editor's main window.

void
RESession::GetWindowTitleForEditor(
	REEditor*	inEditor,
	LStr255&	outTitle)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inEditor);
	
	// Initialize the title to a blank string.
	
	outTitle[(UInt8)0] = 0;

	// Find the data model resource for this editor.

	RMResource* theRMRsrc = inEditor->mPrimaryResource;
	if (theRMRsrc == nil)
		return;
	ValidateObject_(theRMRsrc);

	RMIdentifier* resType = theRMRsrc->GetResTypeRM();
	RMIdentifier* resID = theRMRsrc->GetResIDRM();
	
	ValidateObject_(resType);
	ValidateObject_(resID);
	
	PDResource* thePDRsrc = mContainer->FindResource(*resType, *resID, false);
	delete resType;
	delete resID;
	
	if (thePDRsrc == nil)
		return;
	ValidateObject_(thePDRsrc);

	// Found it, ask it for displayable name.
	
	thePDRsrc->GetDisplayableName(outTitle);

}


// ---------------------------------------------------------------------------
//		* GetSelectionForEditor
// ---------------------------------------------------------------------------
//	Find or create an appropriate data model selection object for the
//	named editor.

void
RESession::GetSelectionForEditor(
	REEditor*			inEditor,
	DMSelectionPtr&		outSelection)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inEditor);

	// If there's a data model, create a selection for it.

	if (inEditor->mDataModelRoot != nil) {
	
		ValidateObject_(inEditor->mDataModelRoot.GetObject());
	
		outSelection = new DMSelection;
		ValidateObject_(outSelection.GetObject());
		
		outSelection->SetRootElement(inEditor->mDataModelRoot);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource configuration

// ---------------------------------------------------------------------------
//		* ConfigureDataModel									[protected]
// ---------------------------------------------------------------------------
//	Called by REEditor::InflateDataModel. Sets up the editor's data
//	model so it looks like a part of the project's data model.

void
RESession::ConfigureDataModel(
	REEditor*	inEditor)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inEditor);

	// Find the editor's root data model element.
	
	DMElement* element = inEditor->mDataModelRoot;
	if (element != nil) {

		ValidateObject_(element);
		
		// Got one, tell it that its superelement is the resource.

		RMResource* theRMRsrc = inEditor->mPrimaryResource;
		if (theRMRsrc != nil) {
		
			ValidateObject_(theRMRsrc);

			RMIdentifier* resType = theRMRsrc->GetResTypeRM();
			RMIdentifier* resID = theRMRsrc->GetResIDRM();
			
			ValidateObject_(resType);
			ValidateObject_(resID);
			
			PDResource* thePDRsrc = mContainer->FindResource(*resType, *resID, false);
			delete resType;
			delete resID;
			
			if (thePDRsrc != nil) {
				ValidateObject_(thePDRsrc);
				element->SetSuperElement(thePDRsrc);
			}

		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource tear-down

// ---------------------------------------------------------------------------
//		* SpendTime												[protected]
// ---------------------------------------------------------------------------
//	Just call FlushDeletedEditors.

void
RESession::SpendTime(
	const EventRecord&	/* inMacEvent */) 
{
	FlushDeletedEditors();
}


// ---------------------------------------------------------------------------
//		* FlushDeletedEditors									[public]
// ---------------------------------------------------------------------------
//  Created: 1/5/97 CCP, for java code gen, which needs to be sure that editors
//	die at a specific time
//
//	Delete any editors which were previously flagged for removal by
//	the TearDownEditor() method. This will normally happen at SpendTime,
//	but it is public so that it can be done on demand.

void			
RESession::FlushDeletedEditors()
{
	// Validate pointers.

	ValidateThis_();
	
	// Tear down editors which were released recently.

	{
		LArrayIterator iter(mDeletedEditors);
		REEditor* editor;
		while (iter.Next(&editor)) {

			// Validate editor pointer.

			ValidateObject_(editor);
			
			// Walk the editor's private list of data model resources.
			// Tell each resource that the editor was destroyed.
			// Stop the editor from listening so it doesn't look like
			// the editor is still available.
			
			editor->StopListening();
			{
				DMFastIterator iter(editor->mResources);
				while (iter.NextElement()) {
					PDResource* resource = (dynamic_cast<PDResource*>(iter.CurrentElement()));
					ValidateObject_(resource);
					resource->BroadcastMessage(msg_HasEditor, resource);
				}
			}

			// Now delete the editor object itself.

			delete editor;

		}
	}

	// Clear list of editors to be deleted.

	mDeletedEditors.RemoveItemsAt(mDeletedEditors.GetCount(), 1);

	// Don't come back here until another editor is deleted.

}


// ===========================================================================

#pragma mark -
#pragma mark ** error dialogs

// ---------------------------------------------------------------------------
//		* ShowCantOpenDialog									[protected]
// ---------------------------------------------------------------------------
//	Show a dialog indicating that a resource can't be opened for some reason.

void
RESession::ShowCantOpenDialog()
{
	
	// Show the can't open dialog.
	
	StApplicationContext appContext;
	StDialogHandler dialog(PPob_CantOpenResource, 0);

	LWindow* window = dialog.GetDialog();
	ValidateObject_(dialog.GetDialog());
	window->Show();

	// Wait until OK is hit.

	MessageT message;
	do {
		message = dialog.DoDialog();
	} while (message == msg_Nothing);

}
