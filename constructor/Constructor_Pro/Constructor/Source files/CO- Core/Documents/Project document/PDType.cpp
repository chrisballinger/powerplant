// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDType.cpp					   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:05 $
//  $History: PDType.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:31
//	Updated in $/Constructor/Source files/CO- Core/Documents/Project document
//	Added changes to CanHostElementSelf to allow conversion of resources.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:40
//	Updated in $/Constructor/Source files/CO- Core/Documents/Project document
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/05/97    Time: 2:40p
//	Updated in $/ConstructorWin/Sources
//	Initial edit for MSVC build (prefix file, /p, and dynamic casts)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PDType.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionHider.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Display classes (Eric) : Dialog handlers
#include "StKeyDialogHandler.h"

	// Core : Documents : Project document
#include "PDResource.h"

	// Core : Editors : Generic resource editor
#include "RETypeEntry.h"

	// Core : Resource manager
#include "RMIdentifier.h"
#include "RMMap.h"
#include "RMResource.h"
#include "RMType.h"
#include <LStaticText.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

PD_ConflictRsp PDType::sConflictResult = icr_NotAsked;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_DuplicateResMove	= 15080;
const PaneIDT	Pane_ResourceName		= 'RSRC';
const PaneIDT	Pane_Cancel				= 'CNCL';
const PaneIDT	Pane_UniqueID			= 'UNIQ';
const PaneIDT	Pane_Replace			= 'REPL';


// ===========================================================================

#pragma mark *** PDType ***

// ---------------------------------------------------------------------------
//		* PDType(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

PDType::PDType(
	LStream*	inStream)

: DMObject(inStream)

{
	mRMMap = nil;
	mTypeEntry = nil;
	mExpanded = false;
	mResIDsSignificant = true;
}

// ---------------------------------------------------------------------------
//		* PDType(PDType&)
// ---------------------------------------------------------------------------
//	Copy constructor

PDType::PDType(
	const PDType&	inOriginal)

: DMObject(inOriginal)

{
	mRMMap = nil;
	mTypeEntry = inOriginal.mTypeEntry;
	mExpanded = inOriginal.mExpanded;
	mResIDsSignificant = inOriginal.mResIDsSignificant;
}


// ---------------------------------------------------------------------------
//		* ~PDType
// ---------------------------------------------------------------------------
//	Destructor

PDType::~PDType()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource type accessors

// ---------------------------------------------------------------------------
//		* MatchResType
// ---------------------------------------------------------------------------
//	See if this type will host the specified resource type.

Boolean
PDType::MatchResType(
	ResType		inResType) const
{

	// Validate pointers.
	
	ValidateThis_();
	if (mTypeEntry == nil)
		return false;
	ValidateObject_((const_cast<PDType*>(this))->mTypeEntry);

	// Ask type entry if we match.
	
	return mTypeEntry->MatchResType(inResType);

}


// ---------------------------------------------------------------------------
//		* GetPrimaryResType
// ---------------------------------------------------------------------------
//	Return the resource type of our primary resource type.

ResType
PDType::GetPrimaryResType() const
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_((const_cast<PDType*>(this))->mTypeEntry);
	
	// Ask the type entry for its primary resource type.
	
	return mTypeEntry->GetPrimaryResType();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource map accessors

// ---------------------------------------------------------------------------
//		* GetRMType
// ---------------------------------------------------------------------------
//	Find the low-level resource map entry for a given ResType.

RMType*
PDType::GetRMType(
	ResType		inResType) const
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_((const_cast<PDType*>(this))->mRMMap);
	
	// Ask map for the resource type entry.
	
	return mRMMap->FindResType(RMIdentifier(inResType), false);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource list

// ---------------------------------------------------------------------------
//		* CountResources
// ---------------------------------------------------------------------------
//	Return the number of resources of this ResType that are in the file.

SInt32
PDType::CountResources() const
{

	// Validate pointers.

	ValidateThis_();
	
	// If data model is valid, use it. Else ask the low-level
	// resource map.
	
	SInt32 count = 0;

	if (mExpanded) {
		
		// Resource list is expanded. Use data model's count.
	
		DMContainerAttribute* attr = GetResourceList();
		ValidateObject_(attr);
		
		count = attr->GetSubElements().GetCount();
		
	}
	else if (mRMMap != nil) {
	
		// Loop through each of the primary resource types.
		// Report the number of resources that's in the type with the most
		// items.
	
		LFastArrayIterator typeIter(mTypeEntry->GetResTypes());
		ResType resType;
		
		while (typeIter.Next(&resType)) {
		
			// Make sure this is a primary res type.
		
			if (typeIter.GetCurrentIndex() > mTypeEntry->CountPrimaryResTypes())
				break;
			
			// It is, find the low-level resource map entry.
			
			RMType* type = GetRMType(resType);
			if (type == nil)
				continue;
			ValidateObject_(type);
			
			// Count the number of non-deleted resources.
		
			SInt32 thisCount = 0;
		
			LFastArrayIterator iter(type->GetResources());
			RMResource* rsrc;
			while (iter.Next(&rsrc)) {
				ValidateObject_(rsrc);
				if (!(rsrc->IsDeleted()))
					thisCount++;
			}
			
			// Compare this count to the maximum count.
			
			if (thisCount > count)
				count = thisCount;
			
		}
	}
	else {
		SignalCStr_("No data model and no resource map. Wierd.");
	}
	
	// Return the count.
	
	return count;

}


// ---------------------------------------------------------------------------
//		* CollapseResourceList
// ---------------------------------------------------------------------------
//	The resource list is no longer being displayed. Release memory occupied
//	by data model objects.

void
PDType::CollapseResourceList()
{

	// Validate pointers.

	ValidateThis_();
	
	// Don't create new resource entries.

	mExpanded = false;

	// Remove resource entries that are only referenced by this object.
	
		//! TEMPORARY: check use counts on resources and remove those with
		//	use count == 1
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource deletion

// ---------------------------------------------------------------------------
//		* SetDeleted
// ---------------------------------------------------------------------------
//	If the resource type entry is being deleted, all resources need
//	to be deleted as well.

void
PDType::SetDeleted(
	Boolean		inDeleted)
{

	// Validate pointers.

	ValidateThis_();

	// Find resource list attribute.

	DMContainerAttribute* attr = GetResourceList();
	ValidateObject_(attr);

	// Make sure we have a data model for all resources.
	// We use the PDResource::SetDeleted method to handle undoing the resource deletion.
	
	if (inDeleted) {
		StDMTransactionHider hideThis;
		ExpandResourceList();
	}
	
	// Issue individual deleting messages for the resources.
	
	DMIterator iter((const_cast<LSharableArray&>(attr->GetSubElements())));
	while (iter.NextElement()) {

		PDResource* theRes = (dynamic_cast<PDResource*>(iter.CurrentElement()));
		ValidateObject_(theRes);

		if (inDeleted)
			attr->RemoveElement(theRes);
		else
			theRes->UnpackResData();
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for resource map change messages.

void
PDType::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();
	
	// If a resource was added to the map, we may need to add
	// it to the data model.

	switch (inMessage) {
		case Resource_Added:
			HandleResourceAdded((RMMap_ChangeMessage*) ioParam);
			break;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource creation

// ---------------------------------------------------------------------------
//		* CreateResource										[protected]
// ---------------------------------------------------------------------------
//	Create a resource entry for the given resource ID.

PDResource*
PDType::CreateResource(
	const RMIdentifier&	inResType,
	const RMIdentifier&	inResID)
{

	// Validate pointers.

	ValidateThis_();

	// Sanity check: Make sure we're attached to a resource map.
	
	if (mRMMap == nil)
		return nil;
	ValidateObject_(mRMMap);	

	// Close resource object from prototype.

	DMElementPtr tempContainer;
	tempContainer = mPrototypeResource->Clone();
	ValidateObject_(tempContainer.GetObject());

	PDResource* theResource = (dynamic_cast<PDResource*>(tempContainer.GetObject()));
	ValidateObject_(theResource);
	
	theResource->SetRMResource(mRMMap->FindResource(inResType, inResID, true));
	theResource->SetResIDsSignificant(mResIDsSignificant);
	
	// Add it to the object list.
	
	DMContainerAttribute* attr = GetResourceList();
	ValidateObject_(attr);
	attr->InsertElementAt(LArray::index_Last, theResource, true);	
	
	// Return it to whoever called.
	
	return theResource;

}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard/drag-and-drop primitives

// ---------------------------------------------------------------------------
//		* CanHostElementSelf									[protected]
// ---------------------------------------------------------------------------
//	Accept PDType and PDResource items if their ResType is the same as ours.
//	We'll figure out where to put them later.

Boolean
PDType::CanHostElementSelf(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);

	RMMap* newResMap = nil;

	Boolean converting = false;

	// First, see if it's a known type.

	const PDResource* newRes = dynamic_const_cast(inNewElement,DMElement*,PDResource*);
	if (newRes != nil) {
		ValidateObject_(newRes);
		if (MatchResType(newRes->GetResType()))
			newResMap = newRes->GetRMMap();
#if Constructor_ForPilot
		else if (((newRes->GetResType() == 'tFRM')&&(GetPrimaryResType() == 'PLob'))||
				 ((newRes->GetResType() == 'PLob')&&(GetPrimaryResType() == 'tFRM')))
		{
			converting = true;
			newResMap = newRes->GetRMMap();
		}
#endif
		else
			return false;
	}

	const PDType* newType = dynamic_const_cast(inNewElement,DMElement*,PDType*);
	if (newType != nil) {
		ValidateObject_(newType);
		if (newType->GetTypeEntry() == GetTypeEntry())
			newResMap = newType->GetRMMap();
#if Constructor_ForPilot
		else if (((newType->GetPrimaryResType() == 'tFRM')&&(GetPrimaryResType() == 'PLob'))||
				 ((newType->GetPrimaryResType() == 'PLob')&&(GetPrimaryResType() == 'tFRM')))
		{
			converting = true;
			newResMap = newType->GetRMMap();
		}
#endif
		else
			return false;
	}
	
	// It is... accept it if it's not from this document.

	if ((newRes != nil) || (newType != nil)) {

		ValidateObject_(newResMap);
		Boolean accept = (converting || (newResMap != GetRMMap()));
		if (accept) {
			ioPlacement->mHostElement = this;
			ioPlacement->mInsertionIndex = 0;
			ioPlacement->ioInsertAsDuplicate = true;
			return true;
		}
	}

	return false;
}


// ---------------------------------------------------------------------------
//		* HostElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Accept a RSResource or RSType item.

void
PDType::HostElementSelf(
	DMElement*				inNewElement,
	const DM_DragPlacement*	/* ioPlacement */)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);

	// See what kind of object this is. If it's a data model
	// resource or type, we can host it.

	PDResource* newRes = (dynamic_cast<PDResource*>(inNewElement));
	PDType* newType = (dynamic_cast<PDType*>(inNewElement));

	if (newRes != nil) {
		ValidateObject_(newRes);
		HostResource(newRes);
	}
	else if (newType != nil) {
		ValidateObject_(newType);
		HostResType(newType);
	}
}


// ---------------------------------------------------------------------------
//		* HostResType											[protected]
// ---------------------------------------------------------------------------
//	A PDType has been dropped onto this type or container. Copy all
//	its resources and add them to our data model.

void
PDType::HostResType(
	PDType*		inNewType)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewType);

	// Get list of resources.

	DMContainerAttribute* resList = inNewType->GetResourceList();
	ValidateObject_(resList);

	// Copy resources individually.
	
	DMFastIterator iter(resList->GetSubElements());
	while (iter.NextElement()) {
		PDResource* rsrc = (dynamic_cast<PDResource*>(iter.CurrentElement()));
		ValidateObject_(rsrc);
		HostResource(rsrc);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource ID conflict resolution

// ---------------------------------------------------------------------------
//		* ShowResourceIDConflictDialog							[protected]
// ---------------------------------------------------------------------------
//	Show an alert when user attempts to drag a resource into this ResType
//	and creates a conflict.

PD_ConflictRsp
PDType::ShowResourceIDConflictDialog(
	const PDResource*	inExistingResource)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inExistingResource);
	
	// If resource IDs aren't significant, just ignore the dialog.
	
	if (!GetResIDsSignificant())
		return icr_Renumber;

	// Prepare the dialog.

	StKeyDialogHandler dialog(PPob_DuplicateResMove, LCommander::GetTarget());
	ValidateObject_(dialog.GetDialog());

	// Show the existing resource's name and ID.

	LStaticText* resourceName = dynamic_cast<LStaticText*>(dialog.GetDialog()->FindPaneByID(Pane_ResourceName));
	ValidateObject_(resourceName);

	LStr255 descriptorString;
	inExistingResource->GetDisplayableName(descriptorString);
	resourceName->SetDescriptor(descriptorString);

	// Run the dialog.

	dialog.GetDialog()->Show();	
	MessageT message = msg_Nothing;
	while (message == msg_Nothing) {
		message = dialog.DoDialog();
	}
	LCommander::SetUpdateCommandStatus(true);
	
	// Decipher the results.
	
	EventRecord event;
	::EventAvail(0, &event);
	Boolean optionDown = (event.modifiers & optionKey) != 0;
	
	switch (message) {

		case Pane_UniqueID:
			if (optionDown)
				sConflictResult = icr_Renumber;
			return icr_Renumber;
		
		case Pane_Replace:
			if (optionDown)
				sConflictResult = icr_Replace;
			return icr_Replace;

		case msg_Cancel:
			Throw_(1);
	}

	return icr_NotAsked;

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource ID conflict resolution

// ---------------------------------------------------------------------------
//		* SetRMMap												[protected]
// ---------------------------------------------------------------------------
//	Set the resource map associated with this document.

void
PDType::SetRMMap(
	RMMap*	inMap)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMap);
	
	// Save pointer to map.
	
	mRMMap = inMap;
	
	// Listen to resource map's change messages.
	
	mRMMap->AddListener(this);
	
}