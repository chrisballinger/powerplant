// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSContainer.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/28/96
//	   $Date: 2006/01/18 01:32:58 $
//	$History: RSContainer.cpp $
//	
//	*****************  Version 16  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 02/07/97   Time: 15:05
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Restored the #include "VPModelObject.h".
//	
//	*****************  Version 14  *****************
//	User: Andrew       Date: 2/06/97    Time: 11:43a
//	Updated in $/ConstructorWin/Sources
//	Added prefix file include for MSVC; removed dependency on VPModelObject
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:44
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 02/01/97   Time: 14:16
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Silenced compiler warnings in HostNewPPob when compiling non-MacOS
//	targets.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 01/30/97   Time: 15:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed HostNewPPob to properly initialize the WIND resource ID field.
//	(Bug fix #1210.)
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 01/28/97   Time: 15:35
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed a silly syntax error.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/28/97   Time: 15:30
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed a problem with the "resource IDs significant" flag. (Bug fix
//	#1137.)
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:04
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/18/96   Time: 12:10
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Added some rather skanky code to allow dragging from the catalog to
//	project window.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/05/96   Time: 18:08
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Added Clint's changes to HandleResourceDataChanged.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:13
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Made CanDeleteThis const to avoid hiding DMElement's method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:49
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 18:46
//	Checked in '$/Constructor/Source files/Documents/+ Generic resource file'
//	Comment: Deleted CanMakeChanges, updated to use new DMClassID_ macro.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:55
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSContainer.h"

	// Core : Documents : Generic resource file
#include "RSResource.h"
#include "RSSourceControl.h"
#include "RSType.h"

	// Core : Editors : Generic
#include "RETypeEntry.h"

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"

	// Core : Resource manager
#include "RMIdentifier.h"

	// MacOS : Resource manager
#include "RFMap.h"

	// MacOS : Editors : Views : Data model
#if Constructor_ForMacOS
	#include "VPModelObject.h"
#endif

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** RSContainer ***

// ---------------------------------------------------------------------------
//		* RSContainer(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

RSContainer::RSContainer(
	LStream*	inStream)

: PDContainer(inStream)

{
	InitRSContainer();
}


// ---------------------------------------------------------------------------
//		* RSContainer(RSContainer&)
// ---------------------------------------------------------------------------
//	Copy constructor

RSContainer::RSContainer(
	const RSContainer& inOriginal)

: PDContainer(inOriginal)

{
	InitRSContainer();
}


// ---------------------------------------------------------------------------
//		* ~RSContainer
// ---------------------------------------------------------------------------
//	Destructor

RSContainer::~RSContainer()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** access control

// ---------------------------------------------------------------------------
//		* CanDeleteThis
// ---------------------------------------------------------------------------
//	As top-level data model object, the container can never be deleted.

Boolean
RSContainer::CanDeleteThis() const
{
	return false;
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource locators

// ---------------------------------------------------------------------------
//		* FindResType(RMIdentifier&, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the PDType object corresponding to a given
//	resource type. If the type doesn't exist and inCreateIfNeeded is
//	true, it will be created.

PDType*
RSContainer::FindResType(
	const RMIdentifier&	inResType,
	Boolean				inCreateIfNeeded)
{

	// Can't find non-numeric resource types.

	if (!inResType.IsNumericID())
		return nil;

	// Okay, the type is kosher. 

	return FindResType(inResType.GetNumericID(), inCreateIfNeeded);
		
}


// ---------------------------------------------------------------------------
//		* FindResType(ResType, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the RSType object corresponding to a given
//	resource type. If the type doesn't exist and inCreateIfNeeded is
//	true, it will be created.

RSType*
RSContainer::FindResType(
	ResType		inResType,
	Boolean		inCreateIfNeeded)
{

	// Validate pointers.

	ValidateThis_();

	// Find resource type list attribute.

	DMContainerAttribute* attr = GetResourceTypeList();
	if (attr == nil)
		return nil;

	ValidateObject_(attr);
	
	// Search for the named type.
	
	{
		LFastArrayIterator iter(attr->GetSubElements());
		RSType* theType;
		while (iter.Next(&theType)) {
			ValidateObject_(theType);
			if (theType->MatchResType(inResType))
				return theType;
		}
	}
	
	// Didn't find it. Create it if so requested. Disallow
	// creation of resource types we can't edit.
	
	if (inCreateIfNeeded && IsEditableResourceType(inResType))
		return (dynamic_cast<RSType*>(CreateResourceType(inResType)));
	else
		return nil;
	
}


// ---------------------------------------------------------------------------
//		* FindResource
// ---------------------------------------------------------------------------
//	Returns a pointer to the RSResource object for the specified type
//	and ID. If the resource doesn't already exist, the flag
//	inCreateIfNeeded is examined: if true, the resource object is created;
//	otherwise, nil is returned.

RSResource*
RSContainer::FindResource(
	ResType		inResType,
	ResIDT		inResID,
	Boolean		inCreateIfNeeded)
{

	// Validate pointers.

	ValidateThis_();
	
	// Find the resource type object. If none exists and we are creating
	// the resource, create the restype object as well.
	
	RSType* theType = FindResType(inResType, inCreateIfNeeded);
	if (theType != nil) {
	
		// Okay, we found the resource type object. Find or create the resource.
	
		ValidateObject_(theType);
		return (dynamic_cast<RSResource*>(theType->FindResource(inResType, inResID, inCreateIfNeeded)));

	}
	else {
	
		// Couldn't find a resource type object. Obviously, that resource
		// doesn't exist either.
	
		return nil;

	}
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
RSContainer::CopyMasterResourceSet(
	RETypeEntry*		inResTypeEntry,
	SInt32				inResIndex,
	const RMIdentifier&	inFileResID,
	ConstStr255Param	inFileResName)
{

	// Validate pointers and copy resources.

	PDContainer::CopyMasterResourceSet(inResTypeEntry, inResIndex, inFileResID, inFileResName);

	// Update resource data if resource IDs are contained internally.
	
	if (inResTypeEntry->UpdateResDataOnIDChange()) {
		
		// Find the data model resource.
		
		RSResource* newRsrc = FindResource(
									inResTypeEntry->GetPrimaryResType(),
									inFileResID.GetNumericID(),
									false);
		ValidateObject_(newRsrc);
		
		// Ask the resource to update its data.
		
		newRsrc->UpdateResIDData(inFileResID.GetNumericID(), inFileResID.GetNumericID());

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource change messages

// ---------------------------------------------------------------------------
//		* HandleResourceTypeAdded								[protected]
// ---------------------------------------------------------------------------
//	A resource type has been added to the low-level resource map.
//	Copy it into the data model.

void
RSContainer::HandleResourceTypeAdded(
	const RMMap_ChangeMessage*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ThrowIfNil_(inMessage);
	ValidateObject_(inMessage->mRsrcType);
	
	// Create a data model resource type for the RFType.
	
	RFType* theType = (dynamic_cast<RFType*>(inMessage->mRsrcType));
	ValidateObject_(theType);

	FindResType(theType->GetResType(), true);

}


// ---------------------------------------------------------------------------
//		* HandleResourceDataChanged								[protected]
// ---------------------------------------------------------------------------
//	A resource's data has been changed. Keeps the RSResource's name in sync
//	with its  RFResource.

void
RSContainer::HandleResourceDataChanged(
	const RMMap_ChangeMessage* inMessage)
{

	// Find the resource.

	RFResource* changedRF = (dynamic_cast<RFResource*>(inMessage->mResource));
	ValidateObject_(changedRF);

	RSResource* changedRS = FindResource(changedRF->GetResType(), changedRF->GetResID());
	if (changedRS == nil)
		return;
	
	ValidateObject_(changedRS);

	// Get the name of the new resource.

	LStr255 resName;
	changedRF->GetResName(resName);

	StValueChanger<Boolean> dontAsk(mIsListening, false); //so we don't loop

	// We have to keep the RSResource from trying to tell everyone,
	// because this can happen at a bad time for REEditor w.r.t. its
	// editing window.

	changedRS->StopBroadcasting();
	changedRS->SetResourceName(resName);
	changedRS->StartBroadcasting();

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
RSContainer::CanHostElementSelf(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate input parameters.

	ValidateThis_();
	ValidateObject_(inNewElement);

	// See if this element is an acceptable resource or type object.
	
	if (PDContainer::CanHostElementSelf(inNewElement, ioPlacement))
		return true;
	
	// Skanky hack #1: See if this is a view editor object.
	
	#if Constructor_ForMacOS
	
		const VPModelObject* newVPMO = (dynamic_cast<const VPModelObject*>(inNewElement));
		if (newVPMO != nil) {
			
			// Make sure that it has no subobjects.
			
			DMContainerAttribute* subs = newVPMO->GetSubObjectList();
			if (subs != nil) {
				ValidateObject_(subs);
				if (subs->GetSubElements().GetCount() > 0)
					return false;
			}
		
			// It's okay. Accept the drag.
					
			ioPlacement->mHostElement = this;
			ioPlacement->mInsertionIndex = 0;
			ioPlacement->ioInsertAsDuplicate = true;
			return true;

		}
	
	#endif

	// Nope. We don't know what this is, so we can't host it.

	return false;

}


// ---------------------------------------------------------------------------
//		* HostElementSelf										[protected]
// ---------------------------------------------------------------------------
//	If this is a resource object or resource type object, add it to the
//	data model for this file.

void
RSContainer::HostElementSelf(
	DMElement*				inNewElement,
	const DM_DragPlacement*	/* ioPlacement */)
{

	// Validate pointers.

	ValidateThis_();

	// See what kind of object this is.

	RSResource* newRes = (dynamic_cast<RSResource*>(inNewElement));
	RSType* newType = (dynamic_cast<RSType*>(inNewElement));

#if Constructor_ForMacOS
	VPModelObject* newVPMO = (dynamic_cast<VPModelObject*>(inNewElement));
#endif

	// Get the resource type from the object.
	
	ResType resType = 0;
	if (newRes != nil) {
		ValidateObject_(newRes);
		resType = newRes->GetResType();
	}
	else if (newType != nil) {
		ValidateObject_(newType);
		resType = newType->GetPrimaryResType();
	}
#if Constructor_ForMacOS
	else if (newVPMO != nil) {
		ValidateObject_(newVPMO);
		resType = 'PPob';
	}
#endif
	else
		return;
	
	// Make sure there's a resource type entry for this resource.
	
	RSType* ourType = FindResType(resType, true);
	if (ourType == nil) {
		SignalCStr_("Couldn't create RSType object");
		return;
	}
	ValidateObject_(ourType);
	
	// Ask it to host the resource(s).
	
	if (newRes != nil)
		ourType->HostResource(newRes);
	else if (newType != nil)
		ourType->HostResType(newType);
#if Constructor_ForMacOS
	else if (newVPMO != nil)
		HostNewPPob(ourType, newVPMO);
#endif

}


// ---------------------------------------------------------------------------
//		* HostNewPPob											[protected]
// ---------------------------------------------------------------------------
//	Skanky hack to allow dragging from the catalog to project window.

void
RSContainer::HostNewPPob(
	RSType*			inPPobType,
	VPModelObject*	inNewPPob)
{
	
#if Constructor_ForMacOS

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inPPobType);
	ValidateObject_(inNewPPob);
	ValidateObject_(mRFMap);
	
	// Find a resource ID for the new resource.
	
	ResIDT newResID = inPPobType->UniqueID(128);
	
	// Create the PPob stream.
	
	LHandleStream ppobStream;
	
	SInt16 ppobVersion = 2;
	ppobStream << ppobVersion;
	
	SInt32 theTag = 'objd';
	ppobStream << theTag;
	
	SInt32 length = 0;
	ppobStream << length;
	
	SInt32 dataStart = ppobStream.GetMarker();
	
	ppobStream << inNewPPob->GetTypeEntry()->GetClassID();
	
	inNewPPob->WriteStreamData(&ppobStream, 'PPob');
	
	length = ppobStream.GetMarker() - dataStart;
	
	theTag = 'end.';
	ppobStream << theTag;
	
	ppobStream.SetMarker(dataStart - 4, streamFrom_Start);
	ppobStream << length;
	
	// Create the WIND stream.
	
	LHandleStream windStream;
	inNewPPob->WriteStreamData(&windStream, 'WIND');
	
	// Spill these to resources.
	
	RFResource* ppobRsrc = mRFMap->FindResource('PPob', newResID, true);
	ValidateObject_(ppobRsrc);
	ppobRsrc->SetResData(ppobStream.GetDataHandle());

	if (windStream.GetLength() > 0) {
		RFResource* windRsrc = mRFMap->FindResource('WIND', newResID, true);
		ValidateObject_(windRsrc);
		windRsrc->SetResData(windStream.GetDataHandle());
	}	
	
	// Make sure WIND resource ID is filled in correctly.
	//* ES 97-01-30: BUG FIX #1210: added section
	
	RSResource* theRS = FindResource('PPob', newResID, false);
	ValidateObject_(theRS);
	
	theRS->UpdateResIDData(0, newResID);

#else
	#pragma unused (inPPobType)
	#pragma unused (inNewPPob)
#endif	

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
RSContainer::SetRMMap(
	RMMap*		inRMMap)
{
	
	// Validate pointers & record map pointer.
	
	PDContainer::SetRMMap(inRMMap);
	
	// Cast this to am map object.
	
	mRFMap = (dynamic_cast<RFMap*>(inRMMap));
	ValidateObject_(mRFMap);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** source control system

// ---------------------------------------------------------------------------
//		* BuildSCCObject										[protected]
// ---------------------------------------------------------------------------

void
RSContainer::BuildSCCObject()
{

	// Validate pointers.
	
	ValidateThis_();

	// Make sure our resource map is an RFMap.
	
	RFMap* mapRF = (dynamic_cast<RFMap*>(mRMMap));
	ValidateObject_(mapRF);

	// Build source control object.
	
	mSourceControl = new RSSourceControl(*mapRF);
	ValidateObject_(mSourceControl);

	mSourceControl->AddListener(this);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** common initialization

// ---------------------------------------------------------------------------
//		* InitRSContainer										[private]
// ---------------------------------------------------------------------------
//	Common initialization for RSContainer constructors.

void
RSContainer::InitRSContainer()
{
	mRFMap = nil;
}
