// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	REEditor.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:33:05 $
//	$History: REEditor.cpp $
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:34
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Added GetPDContainer method.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Improved commenting.
//	
//	*****************  Version 10  *****************
//	User: Andrew       Date: 2/05/97    Time: 5:32p
//	Updated in $/ConstructorWin/Sources
//	Adjusted for error-free compilation via a Microsoft product
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/06/96   Time: 14:21
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added undoer to constructor.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/30/96   Time: 20:15
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added SetModified, EnableSelf, and DisableSelf methods.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:37
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Fixed a bug that could crop up when window creation proceeds part-way,
//	then fails.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/25/96   Time: 21:01
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added mSuppressErrors flag.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/11/96   Time: 10:40
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added FinishCreateSelf method to REEditor.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:15
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

#include "REEditor.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionHider.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Documents : Project document
#include "PDResource.h"

	// Core : Editors : Generic editor
#include "RESession.h"

	// Core : Resource manager
#include "RMResource.h"

	// Core : Generic resource file
#include "RSResource.h"


// ===========================================================================

#pragma mark *** REEditor ***

// ---------------------------------------------------------------------------
//		* REEditor(LCommander*, RESession&)
// ---------------------------------------------------------------------------
//	Constructor, specifying supercommander and editor session document

REEditor::REEditor(
	LCommander*		inSuper,
	RESession&		inSession)

: LCommander(inSuper),
  mSession(inSession)

{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSuper);
	ValidateObject_(&inSession);

	// Clear variables.

	mEditorWindow = nil;
	mPrimaryResource = nil;
	mResourceDataRead = false;
	mSuppressErrors = false;

	// Create an undoer attachment. Editors have independent undo/redo stacks.
	
	AddAttachment(new LUndoer);

}


// ---------------------------------------------------------------------------
//		* ~REEditor
// ---------------------------------------------------------------------------
//	Destructor

REEditor::~REEditor()
{

	// Validate pointers.

	ValidateThis_();
	
	// Make sure editor window is deleted.

	if (mEditorWindow != nil) {
		ValidateObject_(mEditorWindow);
		delete mEditorWindow;
		mEditorWindow = nil;
	}
	
	// Recheck that the editor is still around.

	ValidateThis_();
	
	// Remove this editor from the list of editors to be deleted.
	
	mSession.mDeletedEditors.Remove(this);		// to avoid double deletion

}


// ===========================================================================

#pragma mark -
#pragma mark ** session management functions

// ---------------------------------------------------------------------------
//		* SetPrimaryResource
// ---------------------------------------------------------------------------
//	Tell the editor which resource it will be editing.

void
REEditor::SetPrimaryResource(
	RMResource*		inResource)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inResource);

	// Save pointer to resource.

	mPrimaryResource = inResource;
	StDMTransactionHider hide;
	SetPrimaryResourceSelf(inResource);
	
	// Set up selection.
	
	mSession.GetSelectionForEditor(this, mSelection);

}


// ---------------------------------------------------------------------------
//		* ReadResourceData
// ---------------------------------------------------------------------------
//	Ask the editor to read the resource data and set up its internal
//	data model.

void
REEditor::ReadResourceData()
{

	// Validate pointers.

	ValidateThis_();
	
	// If resource data hasn't been read, read it now.

	if (!mResourceDataRead) {
		StDMTransactionHider hide;
		ReadResourceDataSelf();
		mResourceDataRead = true;
		mSession.ConfigureDataModel(this);
	}
}


// ---------------------------------------------------------------------------
//		* WriteResourceData
// ---------------------------------------------------------------------------
//	Ask the resource editor to convert its internal data model to resource
//	data in the resource map.

void
REEditor::WriteResourceData()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Call override hook.
	
	WriteResourceDataSelf();

}


// ---------------------------------------------------------------------------
//		* ResourceDeleted
// ---------------------------------------------------------------------------
//	A resource that this editor has acquired has been deleted from the
//	project window. Editor should adapt its data model accordingly.
//	Note that this method will NOT be called when the primary resource is
//	deleted. (The editor will be closed instead.)

void
REEditor::ResourceDeleted(
	RMResource*		inResource)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Call override hook.
	
	ResourceDeletedSelf(inResource);
	
}


// ---------------------------------------------------------------------------
//		* OpenEditorWindow
// ---------------------------------------------------------------------------
//	Ask the editor to open its default editor window. This method will
//	ensure that the data model has been configured already.

LWindow*
REEditor::OpenEditorWindow()
{

	// Validate pointers.

	ValidateThis_();

	// If no editor window, create one.

	if (mEditorWindow == nil) {
	
		// Read resource data.
	
		ReadResourceData();
		
		// Now create the window.
		
		LWindow* window = OpenEditorWindowSelf();
		if (window != nil) {
			ValidateObject_(window);
			window->SetSuperCommander(this);
			AddUser(window);
			mEditorWindow = window;
		}

	}
	
	// Return the window pointer.
	
	return mEditorWindow;

}


// ---------------------------------------------------------------------------
//		* CloseEditorWindow
// ---------------------------------------------------------------------------
//	Ask the editor to close its editor window.

void
REEditor::CloseEditorWindow()
{

	// Validate pointers.

	ValidateThis_();
	
	// See if we have an editor window open.
	
	if (mEditorWindow != nil) {
	
		// Give subclass a chance to close window.
	
		ValidateObject_(mEditorWindow);
		CloseEditorWindowSelf();
		
		// If subclass didn't close window, we'll do it now.
		
		if (mEditorWindow != nil) {
			ValidateObject_(mEditorWindow);		
			mEditorWindow->DoClose();		// GHD Do this instead of deleting mEditorWindow
			mEditorWindow = nil;
		}
	}
}


// ---------------------------------------------------------------------------
//		* UpdateEditorWindowTitle
// ---------------------------------------------------------------------------
//	The user has changed our primary resource's name or ID. The editor
//	window title should be adjusted accordingly.

void
REEditor::UpdateEditorWindowTitle()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&mSession);
	
	// If we have a primary editor window, we'll take care of it automatically.
	
	if (mEditorWindow != nil) {
	
		ValidateObject_(mEditorWindow);
		
		// Ask session for window title.
		
		LStr255 windowTitle;
		mSession.GetWindowTitleForEditor(this, windowTitle);

		// Assign it to window.

		mEditorWindow->SetDescriptor(windowTitle);
	
	}
	
	// Allow editor to change title for any other windows it may have.
	
	UpdateEditorWindowTitleSelf();
	
}


// ---------------------------------------------------------------------------
//		* IsModified
// ---------------------------------------------------------------------------
//	Return true if there are unsaved changes to the resource data.

Boolean
REEditor::IsModified()
{
	
	// Validate pointers.

	ValidateThis_();
	
	// Return true if override hook says we're modified.
	
	if (IsModifiedSelf())
		return true;
	
	// Check data model for modified flag.
	
	if (mDataModelRoot != nil) {
		ValidateObject_(mDataModelRoot.GetObject());
		if (mDataModelRoot->IsModified() || mDataModelRoot->IsSubModified())
			return true;
	}
	
	// Nope. We're not modified.
	
	return false;
	
}


// ---------------------------------------------------------------------------
//		* SetModified
// ---------------------------------------------------------------------------
//	Called by editor subclass when the user has changed the resource data.
//	Note: If you're using the data model in your editor subclass, calling
//	this method is unnecessary.

void
REEditor::SetModified()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Iterate over data model resources, making them modified.
	
	DMFastIterator iter(mResources);
	while (iter.NextElement()) {
		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		element->MakeModified();
	}
}


// ---------------------------------------------------------------------------
//		* ResetModified
// ---------------------------------------------------------------------------
//	The file has been saved. Reset modified flag.

void
REEditor::ResetModified()
{

	// Validate pointers.

	ValidateThis_();
	
	// Call override hook.
	
	ResetModifiedSelf();
	
	// If there is a data model, reset its modified flag.
	
	if (mDataModelRoot != nil) {
		ValidateObject_(mDataModelRoot.GetObject());
		mDataModelRoot->ResetModified();
	}
}


// ---------------------------------------------------------------------------
//		* UpdateResID
// ---------------------------------------------------------------------------
//	The primary resource's ID has been changed. Ask the editor to change
//	any data which may reflect the ID (such as menu IDs).

void
REEditor::UpdateResID(
	RMResource*		inResource,
	ResIDT			inOldResID,
	ResIDT			inNewResID)
{

	// Validate input parameters.

	ValidateThis_();
	ValidateObject_(inResource);

	// Hide side-effects of resource ID changes.

	StDMTransactionHider hide;
	
	// Make sure resource data is available.
	
	ReadResourceData();
	
	// Let editor subclass make data changes.
	
	UpdateResIDSelf(inResource, inOldResID, inNewResID);

}


// ---------------------------------------------------------------------------
//		* GetPDContainer										[protected]
// ---------------------------------------------------------------------------
//	Accessor to grab the PDContainer

PDContainer*	
REEditor::GetPDContainer() const
{
	return mSession.mContainer;
}



// ===========================================================================

#pragma mark -
#pragma mark ** editor override hooks

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook. The object has been fully constructed, but nothing
//	else has been done yet. Perform any initialization that can't be done
//	inside the constructor here.

void
REEditor::FinishCreateSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* SetPrimaryResourceSelf								[protected]
// ---------------------------------------------------------------------------
//	Override hook. The primary resource has been specified.

void
REEditor::SetPrimaryResourceSelf(
	RMResource*		/* inResource */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* ReadResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Override hook. Read resource data and create data model.
//	Should always be overriden.

void
REEditor::ReadResourceDataSelf()
{
	SignalCStr_("ReadResourceDataSelf not overriden.");
	// override hook
}


// ---------------------------------------------------------------------------
//		* WriteResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Override hook. The editor should convert its internal data model
//	to resource data. Should always be overriden.

void
REEditor::WriteResourceDataSelf()
{
	SignalCStr_("WriteResourceDataSelf not overriden.");
	// override hook
}


// ---------------------------------------------------------------------------
//		* ResourceDeletedSelf									[protected]
// ---------------------------------------------------------------------------
//	Override hook. A secondary resource that we're editing has been deleted.

void
REEditor::ResourceDeletedSelf(
	RMResource*		/* inResource */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* OpenEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Should be overriden to create the editor window for this resource type.

LWindow*
REEditor::OpenEditorWindowSelf()
{
	SignalCStr_("OpenEditorWindowSelf not overriden.");
	return nil;				// override hook
}


// ---------------------------------------------------------------------------
//		* CloseEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Close any extra windows the editor may have opened.

void
REEditor::CloseEditorWindowSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* UpdateEditorWindowTitleSelf							[protected]
// ---------------------------------------------------------------------------
//	The user has changed our primary resource's name or ID. The editor
//	window title should be adjusted accordingly. This routine should be
//	overriden only if the editor opens extra windows (like the view editor's
//	hierarchy window). The primary window for the editor will be retitled
//	automatically.

void
REEditor::UpdateEditorWindowTitleSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* IsModifiedSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook. Return true if there are unsaved changes to the resource.

Boolean
REEditor::IsModifiedSelf()
{
	return false;			// override hook
}


// ---------------------------------------------------------------------------
//		* ResetModifiedSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook. The file has been saved. Clear modified flag.

void
REEditor::ResetModifiedSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Notification to editor that changes to the resource data are no longer
//	permitted.

void
REEditor::DisableSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* EnableSelf											[protected]
// ---------------------------------------------------------------------------
//	Notification to editor that changes to the resource data are now
//	permitted.

void
REEditor::EnableSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* UpdateResIDSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook. The resource ID has changed. Update any resource data
//	which may refer to this ID. This method will only be called if the
//	"Update resource data on ID change" flag in the RSCP resource was set.

void
REEditor::UpdateResIDSelf(
	RMResource*		/* inResource */,
	ResIDT			/* inOldResID */,
	ResIDT			/* inNewResID */)
{
	// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** editor management

// ---------------------------------------------------------------------------
//		* NoMoreUsers											[protected]
// ---------------------------------------------------------------------------
//	Deleting the editor may cause problems (references to the editor still
//	on the stack). Use the session to tear down the editor later when its
//	safer.

void
REEditor::NoMoreUsers()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(&mSession);
	
	// Ask editor session to delete this editor.
	
	mSession.TearDownEditor(this);
	
}


void
REEditor::RemoveSubCommander(
	LCommander	*inSub)
{

	// Validate pointers.

	ValidateThis_();

	// If the subcommander is the editor window, forget our pointer.

	if (inSub == mEditorWindow) {
	
		// Release imaginary "reference" from editor window.
		
		RemoveUser(mEditorWindow);
		mEditorWindow = nil;

	}

	// Let LCommander forget reference.

	LCommander::RemoveSubCommander(inSub);

}


// ---------------------------------------------------------------------------
//		* AllowSubRemoval										[protected]
// ---------------------------------------------------------------------------
//	Watch for the editor window going away. If so, we manually remove
//	the reference from it.
//
//	GHD: New override. Moved code to write changes from RemoveSubCommander.

Boolean
REEditor::AllowSubRemoval(
	LCommander	*inSub)
{

	// Validate pointers.

	ValidateThis_();

	// If the subcommander is the editor window, forget our pointer.

	if (inSub == mEditorWindow) {
	
		// Write resource data if changes were made.
	
		if (IsModified()) {
			WriteResourceDataSelf();
			ResetModified();
		}

	}

	return true;
}

// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* FindUIObject											[protected]
// ---------------------------------------------------------------------------
//	Responds to find UI messages from any of the data model resources
//	that we know about.

void
REEditor::FindUIObject(
	DM_FindUIObject*	inFindUIMsg)
{

	// Validate pointers.

	ValidateThis_();
	
	// Return pointer to self if this is a request for editors.

	if ((inFindUIMsg->mObjectIdentifier == objID_Editor)
	  && (mResources.FetchIndexOf(&(inFindUIMsg->mElement)) != 0)) {
		inFindUIMsg->mObjectPointer = this;
	}
}


// ---------------------------------------------------------------------------
//		* GoingAway												[protected]
// ---------------------------------------------------------------------------
//	If the primary resource is going away, we need to close our editor window.

void
REEditor::GoingAway(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// See if this is a resource. If not, ignore the message.
	
	RSResource* theRSRRsrc = (dynamic_cast<RSResource*>(inElement));
	if (theRSRRsrc == nil)
		return;
	
	ValidateObject_(theRSRRsrc);
	
	// Find the low-level resource that matches this resource.
	// This is a bit trickier than usual because the PDResource
	// has already severed its link with the resource map. Argh.
	

	RMResource* theRMRsrc = mSession.GetResource(
								theRSRRsrc->GetResType(),		// inResType
								theRSRRsrc->GetResID(),			// inResID
								false);							// inCreateIfNeeded

	ValidateObject_(theRMRsrc);
	
	// If it is our primary resource, kill the editor.
	
	if (theRMRsrc == mPrimaryResource) {
		CloseEditorWindow();
		if (mUseCount < 1)
			NoMoreUsers();
	}
	else {

		// If it's any other resource, post a notification to editor subclass.
		
		if (mResources.FetchIndexOf(&theRSRRsrc) != 0)
			ResourceDeletedSelf(theRMRsrc);
	}
}


// ---------------------------------------------------------------------------
//		* CollectChanges										[protected]
// ---------------------------------------------------------------------------
//	Write resource data and reset our internal modified flags.

void
REEditor::CollectChanges(
	DMElement*	/* inElement */)
{
	
	// Validate pointers.

	ValidateThis_();
	
	// If resource data is modified, spill data to resource map.
	
	if (IsModified()) {
		WriteResourceDataSelf();
		ResetModified();
	}
}


// ---------------------------------------------------------------------------
//		* CollectTentativeChanges								[protected]
// ---------------------------------------------------------------------------
//	Collect any changes that may have been made (like in-place editing
//	that's in progress).

void
REEditor::CollectTentativeChanges(
	DM_CollectTentativeChanges*	inMessage)
{

	// Validate pointers.

	ValidateThis_();
	
	// Record if therewas a change.
	
	if (mDataModelRoot != nil)
		inMessage->mIsChanged |= IsModified();

}



// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Watch for signs that the primary resource's ID or name has changed.
//	If so, trigger a window retitling.

void
REEditor::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMessage->mAttribute);
	ValidateObject_(inMessage->mObject);

	// See if this is a resource. If not, ignore the message.
	
	PDResource* thePDRsrc = (dynamic_cast<PDResource*>(inMessage->mObject));
	if (thePDRsrc == nil)
		return;
	
	ValidateObject_(thePDRsrc);
	
	// Find the low-level resource that matches this resource.
	
	RMResource* theRMRsrc = thePDRsrc->GetRMResource();
	
	// If it's our resource, see if the ID or name was changed.
	// If so, we retitle the window.
	
	if (theRMRsrc == mPrimaryResource) {
	
		ValidateObject_(theRMRsrc);
		switch (inMessage->mAttribute->GetKey()) {
		
			case RSResource::attrKey_ResourceID:
			case RSResource::attrKey_ResourceName:
				UpdateEditorWindowTitle();
				break;

		}
	}
}


// ---------------------------------------------------------------------------
//		* EnabledChanged										[protected]
// ---------------------------------------------------------------------------
//	Catch enabled-changed messages from the data model resource. Pass them
//	along to the editor subclass.

void
REEditor::EnabledChanged(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// If the element is our primary resource, send message to editor.

	if (mResources.FetchIndexOf(&inElement) == 1) {
		if (inElement->IsEnabled())
			EnableSelf();
		else
			DisableSelf();
	}
}
