// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSType.cpp					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/28/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: RSType.cpp $
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 02/28/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Rolled in Clint's fixes for RM/RF classes.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/28/97   Time: 13:23
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed up a couple of build errors related to the Pilot additions.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:29
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Changes to HostResource to allow conversion of resources.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:58
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Added code to keep links current when primary resource changes.
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/05/97    Time: 5:17p
//	Updated in $/ConstructorWin/Sources
//	added prefix file include for MSVC build
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/29/97   Time: 14:33
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed a bug in CountResources. It now counts icon suites properly. (Bug
//	fix #1087.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:04
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:02
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Changed Clone to call FinishCreate.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:02
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
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

#include "RSType.h"

// ===========================================================================

	// Core : Data model : Undo-redo
#include "StDMTransactionHider.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Documents : Generic resource file
#include "RSContainer.h"
#include "RSResource.h"

	// Core : Editors : Generic resource editor
#include "REEditor.h"
#include "RESession.h"
#include "RETypeEntry.h"

	// Core : Resource manager
#include "RMIdentifier.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"

#if Constructor_ForPilot
		// Pilot : Editors : Views : User Interface
	#include "VLEditor.h"
#endif


// ===========================================================================

#pragma mark *** RSType ***

// ---------------------------------------------------------------------------
//		* RSType(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

RSType::RSType(
	LStream*	inStream)

: PDType(inStream)

{
	mRFMap = nil;
	mTypeEntry = nil;
	mExpanded = false;
	mResIDsSignificant = true;
}


// ---------------------------------------------------------------------------
//		* RSType(RSType&)
// ---------------------------------------------------------------------------
//	Copy constructor

RSType::RSType(
	const RSType&	inOriginal)

: PDType(inOriginal)

{
	mRFMap = nil;
	mTypeEntry = inOriginal.mTypeEntry;
	mExpanded = inOriginal.mExpanded;
	mResIDsSignificant = inOriginal.mResIDsSignificant;
}


// ---------------------------------------------------------------------------
//		* ~RSType
// ---------------------------------------------------------------------------
//	Destructor

RSType::~RSType()
{
}


// ---------------------------------------------------------------------------
//		* Clone
// ---------------------------------------------------------------------------
//	Be sure that this type's data model is expaded before
//	creating the clone.

DMElement*
RSType::Clone() const
{

	// Validate pointers.

	ValidateThis_();

	// Make sure data model is complete before cloning.

	if (!mExpanded)
		(const_cast<RSType*>(this))->ExpandResourceList();

	// Return a clone.

	RSType* clone = new RSType(*this);
	ValidateObject_(clone);
	
	clone->FinishCreate();
	return clone;

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource map accessors

// ---------------------------------------------------------------------------
//		* GetRFType
// ---------------------------------------------------------------------------
//	Find the low-level resource map entry for a given ResType.

RFType*
RSType::GetRFType(
	ResType		inResType) const
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_((const_cast<RSType*>(this))->mRFMap);
	
	// Ask map for the resource type entry.
	
	return mRFMap->FindResType(inResType, false);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource list

// ---------------------------------------------------------------------------
//		* ExpandResourceList
// ---------------------------------------------------------------------------
//	Make sure a data model object exists for each resource in the map.
//	When the type is first created, this is not the case (as a memory
//	optimization).

void
RSType::ExpandResourceList()
{

	// Validate pointers.

	ValidateThis_();

	// Allow resource creation.

	mExpanded = true;
	
	// Sanity check: Make sure we're attached to a resource map.
	
	if (mRFMap == nil)
		return;
	ValidateObject_(mRFMap);
	
	if (mTypeEntry == nil)
		return;
	ValidateObject_(mTypeEntry);

	// Do not include this in any action which may be occurring.
	
	StDMTransactionHider hider;

	// "Find" each resource in the low-level resource map.
	// This will cause data model objects to be created.

	LFastArrayIterator types(mTypeEntry->GetResTypes());
	SInt32 primaryTypeCount = mTypeEntry->CountPrimaryResTypes();
	ResType theType;
	
	while (types.Next(&theType) && (types.GetCurrentIndex() <= primaryTypeCount)) {
	
		// Find the resource map's entry for this restype.

		RFType* theRFType = mRFMap->FindResType(theType, false);
		if (theRFType == nil)
			continue;
		
		ValidateObject_(theRFType);

		// Okay, we have a low-level resource type. Scan its resource list.

		LFastArrayIterator iter(theRFType->GetResources());
		RFResource* resource;
		
		while (iter.Next(&resource)) {
			ValidateObject_(resource);
			if (!resource->IsDeleted())
				FindResource(theRFType->GetResType(), resource->GetResID(), true);
		}
	}
}


// ---------------------------------------------------------------------------
//		* FindResource
// ---------------------------------------------------------------------------
//	Find the data model resource type entry for the given resource ID.
//	If one doesn't exist, it will be created if the inCreateIfNeeded flag is set.

PDResource*
RSType::FindResource(
	const RMIdentifier&	inResType,
	const RMIdentifier&	inResID,
	Boolean				inCreateIfNeeded)
{

	// If type or ID aren't numeric, we won't find it and can't create it.

	if (!inResType.IsNumericID() || !inResID.IsNumericID())
		return nil;
	
	// Both are kosher, now try to find the resource.
	
	return FindResource(inResType.GetNumericID(), inResID.GetNumericID(),
						inCreateIfNeeded);

}


// ---------------------------------------------------------------------------
//		* FindResource
// ---------------------------------------------------------------------------
//	Find the data model resource type entry for the given resource ID.
//	If one doesn't exist, it will be created if the inCreateIfNeeded flag is set.

RSResource*
RSType::FindResource(
	ResType		inResType,
	ResIDT		inResID,
	Boolean		inCreateIfNeeded)
{

	// Validate pointers.

	ValidateThis_();

	// Find resource list attribute.

	DMContainerAttribute* attr = GetResourceList();
	ValidateObject_(attr);
	
	// If resource list is collapsed, expand it now.
	
	if (!mExpanded)
		ExpandResourceList();
	
	// Search for the named resource.
	
	{
		DMFastIterator iter(attr->GetSubElements());
		while (iter.NextElement()) {
			RSResource* theRes = (dynamic_cast<RSResource*>(iter.CurrentElement()));
			ValidateObject_(theRes);
			if (theRes->GetResID() == inResID)
				return theRes;
		}
	}
	
	// Didn't find it. Create it if so requested.
	
	if (inCreateIfNeeded)
		return (dynamic_cast<RSResource*>(
					CreateResource(RMIdentifier(inResType), RMIdentifier(inResID))));
	else
		return nil;

}


// ---------------------------------------------------------------------------
//		* UniqueID
// ---------------------------------------------------------------------------
//	Find a resource ID that will not conflict with any existing resources
//	of this type. Start from the named resource ID.

ResIDT
RSType::UniqueID(
	ResIDT	inStartingResID)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRFMap);

	// Make sure we have complete data model.

	if (!mExpanded)
		ExpandResourceList();

	DMContainerAttribute* resList = GetResourceList();
	ValidateObject_(resList);
	
	// Keep iterating over resources until we find an availble ID.
	
	DMFastIterator iter(resList->GetSubElements());
	ResIDT startPoint = inStartingResID;

	while (1) {
	
		// Restart iterator.

		iter.ResetTo(LArrayIterator::from_Start);
		Boolean collision = false;
		
		// Iterate through resources.

		while (iter.NextElement()) {
		
			// See if there's a conflict.
			
			RSResource* theRes = (dynamic_cast<RSResource*>(iter.CurrentElement()));
			ValidateObject_(theRes);

			if (theRes->GetResID() == startPoint) {
			
				// There is a conflict, choose next starting point.
			
				if (startPoint == 32767)
#if Constructor_ForPilot
					startPoint = inStartingResID - 100;
				else if (startPoint >= inStartingResID)
					startPoint += 100;
				else
					startPoint -= 100;
#else
					startPoint = inStartingResID - 1;
				else if (startPoint >= inStartingResID)
					startPoint++;
				else
					startPoint--;
#endif
				collision = true;
			}
		}
		
		// No collision on this iteration, stop now.

		if (!collision)
			return startPoint;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* HandleResourceAdded									[protected]
// ---------------------------------------------------------------------------
//	Create a data model for the new resource (if appropriate).

void
RSType::HandleResourceAdded(
	const RMMap_ChangeMessage* inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mTypeEntry);
	ValidateObject_(inMessage->mRsrcType);
	ValidateObject_(inMessage->mResource);

	// See if the newly-added resource falls within our category.

	RFResource* newRF = (dynamic_cast<RFResource*>(inMessage->mResource));
	ValidateObject_(newRF);

	if (mExpanded && (mTypeEntry->MatchResType(newRF->GetResType())))
		FindResource(newRF->GetResType(), newRF->GetResID(), true);

}


// ---------------------------------------------------------------------------
//		* HandleResourceDeleted									[protected]
// ---------------------------------------------------------------------------
//	If a low-level resource has been deleted, be sure to tell the
//	data model resource about it.

void
RSType::HandleResourceDeleted(
	const RMMap_ChangeMessage* inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(mTypeEntry);
	ValidateSimpleObject_(inMessage->mRsrcType);
	ValidateSimpleObject_(inMessage->mResource);

	// See if the newly-added resource falls within our category.

	RFResource* delRF = (dynamic_cast<RFResource*>(inMessage->mResource));
	ValidateObject_(delRF);

	if (mExpanded && (mTypeEntry->MatchResType(delRF->GetResType()))) {
		RSResource* delRsrc = FindResource(delRF->GetResType(), delRF->GetResID(), false);
		if (delRsrc != nil) {
			ValidateObject_(delRsrc);
			delRsrc->ResourceDeleted(delRF);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard/drag-and-drop primitives

// ---------------------------------------------------------------------------
//		* HostResource											[protected]
// ---------------------------------------------------------------------------
//	A RSResource has been dropped onto this type or container.
//	Copy it and make it a part of our data model.

void
RSType::HostResource(
	PDResource*		inNewResource)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewResource);

	// Sanity check: Make sure we're attached to a resource map.
	
	if (mRFMap == nil)
		return;
	ValidateObject_(mRFMap);

	// Make sure we have complete data model.

	if (!mExpanded)
		ExpandResourceList();

	DMContainerAttribute* resList = GetResourceList();
	ValidateObject_(resList);
	
	Boolean converting = (!MatchResType(inNewResource->GetResType()));
	

	// Make sure the new resource is an RSResource.
	
	RSResource* newResource = (dynamic_cast<RSResource*>(inNewResource));
	ValidateObject_(newResource);

	ResType newType;
	ResIDT 	newID = newResource->GetResID();
	
	if (converting)
		newType = GetPrimaryResType();
	else
		newType = inNewResource->GetResType();
	
	try	// Try for catching the cancel button inside ShowResourceIDConflictDialog
	{
		// Preflight new resource ID.
		
		RSResource* oldResource =
				(dynamic_cast<RSResource*>(FindResource(newType, newResource->GetResID(), false)));
		if ((oldResource != nil)&&(oldResource->GetRFResource())) {

			ValidateObject_(oldResource);

			RS_ConflictRsp conflictResult = sConflictResult;
			if (sConflictResult == icr_NotAsked)
				conflictResult = ShowResourceIDConflictDialog(oldResource);
		
			switch (conflictResult) {
				case icr_Renumber:
					newID = UniqueID(newID);
					
					if (!converting)
						newResource->SetResourceID(newID);
					break;
				case icr_Replace:
					resList->RemoveElement(oldResource);
					break;
				default:
					break;
			}
		}

		if (converting)
		{
			RSContainer* container = dynamic_cast<RSContainer*>(GetSuperElement()->GetSuperElement());
			ValidateObject_(container);
				
			//get the old resource's name
			LStr255 oldName;
			inNewResource->GetResName(oldName);
			
			//create the new resource
			container->CopyMasterResourceSet(GetTypeEntry(), 1, newID, oldName);
			
			//find the new resource
			RSResource *newRes = 
				container->FindResource(newType, newID, false);
			
			
			//ask the session to create an editor for the old resource
			TSharablePtr<REEditor> editor = container->GetSession()->GetEditorForResource(
					inNewResource->GetResType(),newResource->GetResID(),true);
							
			//make sure it has its data
			editor->ReadResourceData(); 

			//Ask the editor to convert the resource
			LHandleStream newResData;

	#if Constructor_ForPilot
			//cast it to a VLEditor (for now; will move ConvertTo() up to REEditor later)
			VLEditor* vlE = dynamic_cast<VLEditor*>(editor.GetObject());
			ValidateObject_(vlE);		
			
			vlE->ConvertTo(newResData,*(newRes->GetRFMap()),newType);
			
	#endif
			//Set the new resource's data
			newRes->GetRFResource()->SetResData(newResData.GetDataHandle());

			return;
		}
		else // Okay, we've resolved the conflict. Put new resource in model.
			resList->InsertElementAt(LArray::index_Last, newResource, true);

	}
	catch (...)
	{
		// Nothing goes here -- this is just to catch the cancel event
		// from the duplicate resource ID dialog
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetRFMap
// ---------------------------------------------------------------------------
//	Set the resource map associated with this document.

void
RSType::SetRMMap(
	RMMap*	inMap)
{

	// Save pointer to map.
	
	PDType::SetRMMap(inMap);
	
	// Make sure it's an RFMap.
	
	mRFMap = (dynamic_cast<RFMap*>(inMap));
	ValidateObject_(mRFMap);

}
