// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDResource.cpp				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:04 $
//  $History: PDResource.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:40
//	Updated in $/Constructor/Source files/CO- Core/Documents/Project document
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/05/97    Time: 2:39p
//	Updated in $/ConstructorWin/Sources
//	Initial edit for building under MSVC (prefix file, /p, and dynamic
//	casts)
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

#include "PDResource.h"

	// Core : Documents : Project document
#include "PDContainer.h"
#include "PDType.h"

	// Core : Resource manager
#include "RMResource.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** PDResource ***

// ---------------------------------------------------------------------------
//		* PDResource(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional data is read

PDResource::PDResource(
	LStream*	inStream)

: DMObject(inStream)

{
	InitPDResource();
}


// ---------------------------------------------------------------------------
//		* PDResource(PDResource&)
// ---------------------------------------------------------------------------
//	Copy constructor

PDResource::PDResource(
	const PDResource&	inOriginal)

: DMObject(inOriginal)

{
	InitPDResource();
	mDeletedResData = inOriginal.PackResData();
	mResIDsSignificant = inOriginal.mResIDsSignificant;
}


// ---------------------------------------------------------------------------
//		* ~PDResource
// ---------------------------------------------------------------------------
//	Destructor

PDResource::~PDResource()
{

	// Release cached resource data (if any).

	if (mDeletedResData != nil) {
		ValidateHandle_(mDeletedResData);
		::DisposeHandle(mDeletedResData);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource map accessors

// ---------------------------------------------------------------------------
//		* GetPDContainer
// ---------------------------------------------------------------------------
//	Scan up through the data model looking for the resource container object.

PDContainer*
PDResource::GetPDContainer() const
{

	// Validate pointers.
	
	ValidateThis_();

	// Look for resource container by walking up the data model. This is ugly. *sigh*
	
	PDContainer* container = nil;
	DMElement* super = GetSuperElement();

	while (super != nil) {
		ValidateObject_(super);
		container = (dynamic_cast<PDContainer*>(super));
		if (container != nil)
			return container;
		super = super->GetSuperElement();
	}

	// Couldn't find container.
	
	return nil;

}


// ---------------------------------------------------------------------------
//		* GetPDType
// ---------------------------------------------------------------------------
//	Scan up through the data model looking for the resource type object.

PDType*
PDResource::GetPDType() const
{

	// Validate pointers.
	
	ValidateThis_();

	// Look for resource type by walking up the data model. This is ugly. *sigh*
	
	PDType* type = nil;
	DMElement* super = GetSuperElement();

	while (super != nil) {
		ValidateObject_(super);
		type = (dynamic_cast<PDType*>(super));
		if (type != nil)
			return type;
		super = super->GetSuperElement();
	}

	// Couldn't find type.
	
	return nil;

}


// ---------------------------------------------------------------------------
//		* GetRMMap
// ---------------------------------------------------------------------------
//	Return a pointer to the low-level resource map for this
//	resource (if any).

RMMap*
PDResource::GetRMMap() const
{

	// Validate pointers.

	ValidateThis_();
	
	// If we have a low-level resource, we can find the resource map.

	if (mRMResource != nil) {
		ValidateObject_(mRMResource);
		return &(mRMResource->GetRMMap());
	}
	else
		return nil;

}


// ---------------------------------------------------------------------------
//		* GetRMType
// ---------------------------------------------------------------------------
//	Return a pointer to the low-level resource type entry for
//	this resource (if any).

RMType*
PDResource::GetRMType() const
{

	// Validate pointers.

	ValidateThis_();
	
	// If we have a low-level resource, we can find the resource type.

	if (mRMResource != nil) {
		ValidateObject_(mRMResource);
		return &(mRMResource->GetRMType());
	}
	else
		return nil;

}


// ---------------------------------------------------------------------------
//		* SetRMResource
// ---------------------------------------------------------------------------
//	Attach this data model object to a low-level resource. Should be
//	called only once as the data model is being built. Syncs the data
//	model attributes with the low-level resource map.

void
PDResource::SetRMResource(
	RMResource*		inRMResource)
{

	// Sanity check: Must be called only once and with a valid pointer.

	ValidateThis_();
	ValidateObject_(inRMResource);
	SignalIf_(mRMResource != nil);

	// Remember that pointer.

	mRMResource = inRMResource;
	
	// Subclasses will want to sync up their data model with the
	// characteristics of this resource.
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource deletion

// ---------------------------------------------------------------------------
//		* SetDeleted
// ---------------------------------------------------------------------------
//	Notification that this element has been added to or removed from the
//	data model. Make sure the low-level resource map is correct.

void
PDResource::SetDeleted(
	Boolean		inDeleted)
{

	// Validate pointers.

	ValidateThis_();

	if (inDeleted) {
		
		// We're being deleted.
		// Sanity check: Make sure we have a resource pointer.
		
		if (mRMResource == nil)
			return;
		
		// Cache the resource data in case we get undeleted.
		
		mDeletedResData = PackResData();
		ValidateHandle_(mDeletedResData);
		
		// Get the related resources list.
		
		LArray related(sizeof (RMResource*));
		GetRelatedResources(related);

		// Delete this resource from the map.
		
		mRMResource->DeleteResource();
		
		// Delete related resources from map.
		
		LFastArrayIterator iter(related);
		RMResource* rsrc;
		while (iter.Next(&rsrc)) {
			ValidateObject_(rsrc);
			rsrc->DeleteResource();
		}
		
		// Cut link to map's resource.
		
		mRMResource = nil;

	}
	else {
	
		// We're being restored. Unpack resource data
		// (unless this is the first time we've been created).
		
		if ((mDeletedResData != nil) && (mRMResource == nil))
			UnpackResData();
	
	}
}


// ---------------------------------------------------------------------------
//		* PackResData
// ---------------------------------------------------------------------------
//	Copy the resource data, name, attributes, etc., into a single handle
//	that can be stored while the resource is not part of the data model.
//	Used for undo/redo of resource deletion, and for copying resources.
//	The format of this handle is private to the PDResource subclass and
//	subject to change.

Handle
PDResource::PackResData() const
{

	// Validate pointers.

	ValidateThis_();

	// If we've already packed data, just copy it and return.
	
	if (mDeletedResData != nil) {
		ValidateHandle_(mDeletedResData);
		StHandleLocker lockOrig(mDeletedResData);
		Handle dupHandle = ::NewHandle(0);
		::HandAndHand(mDeletedResData, dupHandle);
		return dupHandle;
	}

	// Sanity check: Make sure we have a resource.
	
	if (mRMResource == nil)
		return nil;
	ValidateObject_(mRMResource);
	
	// Get related resources list.

	LArray relatedRsrcs(sizeof (RMResource*));
	GetRelatedResources(relatedRsrcs);

	// Pack data for primary resource.

	LHandleStream packData;
	PackOneResData(packData, mRMResource);

	// Pack data for related resources.
	
	LFastArrayIterator iter(relatedRsrcs);
	RMResource* rsrc;
	while (iter.Next(&rsrc)) {
		ValidateObject_(rsrc);
		PackOneResData(packData, rsrc);
	}

	// Return this data.
	
	return packData.DetachDataHandle();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** common initialization

// ---------------------------------------------------------------------------
//		* InitPDResource										[private]
// ---------------------------------------------------------------------------
//	Common initialization for PDResource constructors.

void
PDResource::InitPDResource()
{
	mRMResource = nil;
	mDeletedResData = nil;
	mResIDsSignificant = true;
}
