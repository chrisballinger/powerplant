// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDContainer.cpp				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:03 $
//  $History: PDContainer.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:40
//	Updated in $/Constructor/Source files/CO- Core/Documents/Project document
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/05/97    Time: 2:34p
//	Updated in $/ConstructorWin/Sources
//	Added include of prefix file for MSVC build
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

#include "PDContainer.h"

	// Core : Data model : Core classes
#include "DMReanimator.h"

	// Core : Documents : Project document
#include "PDResource.h"
#include "PDSourceControl.h"
#include "PDType.h"

	// Core : Editors : Generic
#include "REEditor.h"
#include "RESession.h"
#include "RETypeEntry.h"

	// Core : Resource manager
#include "RMIdentifier.h"
#include "RMMap.h"
#include "RMResource.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** PDContainer ***

// ---------------------------------------------------------------------------
//		* PDContainer(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

PDContainer::PDContainer(
	LStream*	inStream)

: DMObject(inStream)

{

	// Do common initialization.

	InitPDContainer();

	// Read initialization data.
	
	ResIDT typeDSPC;
	ResIDT resourceDSPC;
	
	(*inStream)	>> typeDSPC
				>> resourceDSPC
				>> mResIDsSignificant;

	// Cache prototype resource & type entry.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);
	reanimator->ElementsFromResource('DSPC', typeDSPC, mPrototypeType);
	reanimator->ElementsFromResource('DSPC', resourceDSPC, mPrototypeResource);
	
	PDType* type = (dynamic_cast<PDType*>(mPrototypeType.GetObject()));
	ValidateObject_(type);
	
	PDResource* res = (dynamic_cast<PDResource*>(mPrototypeResource.GetObject()));
	ValidateObject_(res);
	
	type->SetResIDsSignificant(mResIDsSignificant);
	res->SetResIDsSignificant(mResIDsSignificant);

}


// ---------------------------------------------------------------------------
//		* PDContainer(PDContainer&)
// ---------------------------------------------------------------------------
//	Copy constructor

PDContainer::PDContainer(
	const PDContainer&	inOriginal)

: DMObject(inOriginal)

{

	// Validate pointers.

	ValidateObject_(&inOriginal);

	// Do standard initialization.

	InitPDContainer();
	
	// Copy values from original container.

	mPrototypeType = inOriginal.mPrototypeType;
	mPrototypeResource = inOriginal.mPrototypeResource;
	mResIDsSignificant = inOriginal.mResIDsSignificant;
	mSession = inOriginal.mSession;

}


// ---------------------------------------------------------------------------
//		* ~PDContainer
// ---------------------------------------------------------------------------
//	Destructor

PDContainer::~PDContainer()
{
	
	// Validate pointers.

	ValidateThis_();
	
	// Delete the source control object, if there is one.
	
	if (mSourceControl != nil) {
		ValidateObject_(mSourceControl);
		delete mSourceControl;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** access control

// ---------------------------------------------------------------------------
//		* CanDeleteThis
// ---------------------------------------------------------------------------
//	As top-level data model object, the container can never be deleted.

Boolean
PDContainer::CanDeleteThis() const
{
	return false;
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource locators

// ---------------------------------------------------------------------------
//		* FindResource
// ---------------------------------------------------------------------------
//	Returns a pointer to the PDResource object for the specified type
//	and ID. If the resource doesn't already exist, the flag
//	inCreateIfNeeded is examined: if true, the resource object is created;
//	otherwise, nil is returned.

PDResource*
PDContainer::FindResource(
	const RMIdentifier&	inResType,
	const RMIdentifier&	inResID,
	Boolean				inCreateIfNeeded)
{

	// Validate pointers.

	ValidateThis_();
	
	// Find the resource type object. If none exists and we are creating
	// the resource, create the restype object as well.
	
	PDType* theType = FindResType(inResType, inCreateIfNeeded);
	if (theType != nil) {
	
		// Okay, we found the resource type object. Find or create the resource.
	
		ValidateObject_(theType);
		return theType->FindResource(inResType, inResID, inCreateIfNeeded);

	}
	else {
	
		// Couldn't find a resource type object. Obviously, that resource
		// doesn't exist either.
	
		return nil;

	}
}


// ---------------------------------------------------------------------------
//		* IsEditableResourceType
// ---------------------------------------------------------------------------
//	Returns true if the specified resource type is known to this document.
//	Should be overriden. Default method always returns false.

Boolean
PDContainer::IsEditableResourceType(
	const RMIdentifier&	inResType) const
{

	// See if we found an RSCP resource for this type.
	
	RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(inResType.GetNumericID());
	return typeEntry != nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** new resource creation

// ---------------------------------------------------------------------------
//		* CopyMasterResourceSet
// ---------------------------------------------------------------------------
//	Using a master resource set that exists in the application file, copy
//	a primary resource and any secondary resources associated with it
//	to the user's file.

void
PDContainer::CopyMasterResourceSet(
	RETypeEntry*		inResType,
	SInt32				inResIndex,
	const RMIdentifier&	inFileResID,
	ConstStr255Param	inFileResName)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inResType);
	ValidateObject_(mSession);
	ThrowIfNil_(inFileResName);

	// Copy resources.
	
	ResType theType;
	LFastArrayIterator iter(inResType->GetResTypes());
	while (iter.Next(&theType)) {
		CopyMasterResourceToFile(inResType, inResIndex, RMIdentifier(theType), inFileResID, inFileResName);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource change messages

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for changes from the resource map.

void
PDContainer::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();

	// Listen for resource map change messsages.

	switch (inMessage) {
		case ResourceType_Added:
			HandleResourceTypeAdded((RMMap_ChangeMessage*) ioParam);
			break;
		case Resource_Added:
			HandleResourceAdded((RMMap_ChangeMessage*) ioParam);
			break;
		case Resource_Removed:
			HandleResourceRemoved((RMMap_ChangeMessage*) ioParam);
			break;
		case Resource_DataChanged:
			HandleResourceDataChanged((RMMap_ChangeMessage*) ioParam);
			break;
		case Resource_AttrChanged:
			HandleResourceAttrsChanged((RMMap_ChangeMessage*) ioParam);
			break;
			
		// Source control update: Enable or disable all subelements.

		case msg_SCC_Changed: {
			ValidateObject_(mSourceControl);
			if (mSourceControl->CanMakeChanges(true))
				Enable();
			else
				Disable();

			LCommander::SetUpdateCommandStatus(true);
			break;
		}
	}
}


// ---------------------------------------------------------------------------
//		* HandleResourceTypeAdded								[protected]
// ---------------------------------------------------------------------------
//	Override hook. A resource type has been added to the low-level
//	resource map.

void
PDContainer::HandleResourceTypeAdded(
	const RMMap_ChangeMessage* /* inMessage */)
{
	// default does nothing
}


// ---------------------------------------------------------------------------
//		* HandleResourceAdded									[protected]
// ---------------------------------------------------------------------------
//	Override hook. A resource has been added to the low-level
//	resource map.

void
PDContainer::HandleResourceAdded(
	const RMMap_ChangeMessage* /* inMessage */)
{
	// default does nothing
}


// ---------------------------------------------------------------------------
//		* HandleResourceRemoved									[protected]
// ---------------------------------------------------------------------------
//	Override hook. A resource has been removed from the low-level
//	resource map.

void
PDContainer::HandleResourceRemoved(
	const RMMap_ChangeMessage* /* inMessage */)
{
	// default does nothing
}


// ---------------------------------------------------------------------------
//		* HandleResourceDataChanged								[protected]
// ---------------------------------------------------------------------------
//	Override hook. A resource's data has been changed.

void
PDContainer::HandleResourceDataChanged(
	const RMMap_ChangeMessage* /* inMessage */)
{
	// default does nothing
}


// ---------------------------------------------------------------------------
//		* HandleResourceAttrsChanged							[protected]
// ---------------------------------------------------------------------------
//	Override hook. A resource's attributes or name has been changed.

void
PDContainer::HandleResourceAttrsChanged(
	const RMMap_ChangeMessage* /* inMessage */)
{
	// default does nothing
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource creation

// ---------------------------------------------------------------------------
//		* CreateResourceType									[protected]
// ---------------------------------------------------------------------------
//	Create the appropriate subclass of PDType for the specified
//	resource type. Clones the default resource type object that we
//	read in from a DSPC resource earlier.

PDType*
PDContainer::CreateResourceType(
	ResType		inResType)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRMMap);
	ValidateObject_(mPrototypeType.GetObject());

	// Find resource type entry for this type.

	RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(inResType);
	ValidateObject_(typeEntry);

	// Clone resource type object from prototype that we read from DSPC resource.

	DMElementPtr tempContainer;
	tempContainer = mPrototypeType->Clone();
	
	PDType* theType = (dynamic_cast<PDType*>(tempContainer.GetObject()));
	ValidateObject_(theType);
	
	theType->CollapseResourceList();
	theType->SetRMMap(mRMMap);
	theType->SetTypeEntry(typeEntry);
	theType->SetPrototypeResource(mPrototypeResource);
	theType->SetResIDsSignificant(mResIDsSignificant);
	
	// Add it to the object list.
	
	DMContainerAttribute* attr = GetResourceTypeList();
	ValidateObject_(attr);
	attr->InsertElementAt(LArray::index_Last, theType, true);	
	
	// Return it to whoever called.
	
	return theType;

}


// ---------------------------------------------------------------------------
//		* CopyMasterResourceToFile								[protected]
// ---------------------------------------------------------------------------
//	Copy a single master resource from the application's resource file
//	to the user's project file.

void
PDContainer::CopyMasterResourceToFile(
	RETypeEntry*			inResType,
	SInt32					inResIndex,
	const RMIdentifier&		inCopyResType,
	const RMIdentifier&		inFileResID,
	ConstStr255Param		inFileResName)
{

	// Do pointer sanity checks.
	
	ValidateThis_();
	ValidateObject_(inResType);
	ValidateObject_(mRMMap);
	ThrowIfNil_(inFileResName);
	
	// Get original resource data from res type entry.
	
	Handle resData = inResType->GetIndexedDefaultResData(inResIndex, inCopyResType.GetNumericID());
	if (resData == nil)
		return;
	
	ValidateHandle_(resData);

	// Put it in user's file.

	try {
		RMResource* theRsrc = mRMMap->FindResource(inCopyResType, inFileResID, true);
		ValidateObject_(theRsrc);
		
		theRsrc->SetResData(resData);
		theRsrc->SetResName(inFileResName);
	}
	catch(...) {
		::DisposeHandle(resData);
		#if SKIPOMPARSE
			throw;
		#endif
	}
	::DisposeHandle(resData);

}


// ===========================================================================

#pragma mark -
#pragma mark ** data model changes

// ---------------------------------------------------------------------------
//		* CanHostElementSelf									[protected]
// ---------------------------------------------------------------------------
//	Accept the element only if it's a resource or resource type that's
//	from *another* document.

Boolean
PDContainer::CanHostElementSelf(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate input parameters.

	ValidateThis_();
	ValidateObject_(inNewElement);

	// See if element is already part of this file. If so, reject it.
	
	if (inNewElement->IsSubElementOf(this))
		return false;

	// Not already in this file. See what kind of object this is.

	const PDResource* newRes = dynamic_const_cast(inNewElement,DMElement*,PDResource*);
	const PDType* newType = dynamic_const_cast(inNewElement,DMElement*,PDType*);

	// If it's either a resource or a resource type, we can host it.
	
	if ((newRes != nil) || (newType != nil)) {
		ioPlacement->mHostElement = this;
		ioPlacement->mInsertionIndex = 0;
		ioPlacement->ioInsertAsDuplicate = true;
		return true;
	}
	
	// Nope. We don't know what this is, so we can't host it.

	return false;

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetRMMap												[protected]
// ---------------------------------------------------------------------------
//	Should be called only once as the document is being built. Attach the
//	data model to the low-level resource map.

void
PDContainer::SetRMMap(
	RMMap*	inRMMap)
{

	// Do pointer sanity checks.

	ValidateThis_();
	ValidateObject_(inRMMap);			// don't validate: map is a contained object

	// Make sure that this method hasn't been called before.
	
	ThrowIf_(mRMMap != nil);

	// Link container to low-level resource map.

	mRMMap = inRMMap;
	mRMMap->AddListener(this);

	// Now that we have the map, build source control.
	
	BuildSCCObject();
	
}


// ---------------------------------------------------------------------------
//		* BuildSCCObject										[protected]
// ---------------------------------------------------------------------------
//	Create a source-control object for this resource system. Default
//	implementation only recognizes file locks.

void
PDContainer::BuildSCCObject()
{
	
	// Do pointer validation.
	
	ValidateThis_();

	// Create basic source control object.
	
	mSourceControl = new PDSourceControl;
	ValidateObject_(mSourceControl);

}


// ===========================================================================

#pragma mark -
#pragma mark ** common initialization

// ---------------------------------------------------------------------------
//		* InitPDContainer										[private]
// ---------------------------------------------------------------------------
//	Common initialization for PDContainer constructors.

void
PDContainer::InitPDContainer()
{
	mRMMap = nil;
	mSourceControl = nil;
	mSession = nil;
	mResIDsSignificant = true;
}
