// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RFType.cpp					   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/23/97
//     $Date: 2006/01/18 01:34:07 $
//  $History: RFType.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:08
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:51
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:20
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/27/97   Time: 14:00
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Removed the multi-file capability. It was never used anyway.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:03
//	Created in $/Constructor/Source files/H1- MacOS/Resource manager
//	Refactored the RF* classes.
//	
// ===========================================================================

	// The following headers are needed to compile this code on Windows
#ifdef WINVER
	#include "asiport.h"			// Altura
	#include "asiextrn.h"			// Altura
	#include "ctor.h"				// Constructor Windows include
#endif

// ===========================================================================

#include "RFType.h"

	// Core : Resource manager
#include "RMIdentifier.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** RFType ***

// ---------------------------------------------------------------------------
//		* RFType(RFMap&, ResType)								[protected]
// ---------------------------------------------------------------------------
//	Constructor, specifying the RFMap that the type belongs to and the
//	four-character resource type. Called only by RFMap::FindResType.

RFType::RFType(
	RFMap&		inMap,
	ResType		inResType)

: RMType(inMap),
  mResMap(inMap)

{
	mResType = inResType;
}


// ---------------------------------------------------------------------------
//		* ~RFType												[protected]
// ---------------------------------------------------------------------------
//	Destructor

RFType::~RFType()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource accessors

// ---------------------------------------------------------------------------
//		* FindResource(RMIdentifier&, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the RFResource object for the specified resource ID.
//	(The type has already determined by the RFType instance.)
//	If the resource doesn't already exist, the flag inCreateIfNeeded is
//	examined: if true, the resource object is created; otherwise, nil
//	is returned.

RMResource*
RFType::FindResource(
	const RMIdentifier&	inResID,
	Boolean				inCreateIfNeeded)
{

	// If ID isn't numeric, can't create the resource no matter what.

	if (!inResID.IsNumericID())
		return nil;
	
	// ID is kosher, now look for the resource.
	
	return FindResource(inResID.GetNumericID(), inCreateIfNeeded);

}


// ---------------------------------------------------------------------------
//		* FindResource(Int16, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the RFResource object for the specified resource ID.
//	(The type has already determined by the RFType instance.)
//	If the resource doesn't already exist, the flag inCreateIfNeeded is
//	examined: if true, the resource object is created; otherwise, nil
//	is returned.

RFResource*
RFType::FindResource(
	SInt16		inResID,
	Boolean		inCreateIfNeeded)
{

	// Validate pointers.

	ValidateThis_();
	
	// See if this resource already exists.
	
	{
		LFastArrayIterator iter(mResources);
		RFResource* theResource;

		while (iter.Next(&theResource)) {
			ValidateObject_(theResource);
			if ((theResource->GetResID() == inResID) && (!theResource->mResDeleted))
				return theResource;
		}
	}
	
	// It doesn't. Create it if so requested.
	
	if (inCreateIfNeeded) {

		// Create the resource object.

		RFResource* newRsrc = new RFResource(*this, inResID);
		ValidateObject_(newRsrc);
		
		// Add it to our resource list.
		
		mResources.InsertItemsAt(1, LArray::index_Last, &newRsrc);
		
		// Broadcast a change message.
		
		RMMap_ChangeMessage msg = { &(GetRMMap()), this, newRsrc };
		GetRMMap().BroadcastMessage(Resource_Added, &msg);
		
		// Return the resource object.
		
		return newRsrc;
		
	}
	else {
		
		// No such resource.
		
		return nil;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource ID arbitration

// ---------------------------------------------------------------------------
//		* UniqueID
// ---------------------------------------------------------------------------
//	Finds a resource ID that is unique within this resource type.
//	First counts upward from inStartingFrom. In the unlikely event 
//	all resource IDs from inStartingFrom to 32767 are already used,
//	starts counting downward from inStartingFrom. Since there can't be
//	32K resources in a single resource map, it assumes that an ID will
//	be found.

SInt16
RFType::UniqueID(
	SInt16	inStartingFrom)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Set up iterator. It may be used several times.

	LFastArrayIterator iter(mResources);
	SInt16 startPoint = inStartingFrom;
	
	// Repeat until a valid ID is found.
	
	while (1) {
	
		// Reset the iterator.
	
		iter.ResetTo(LArrayIterator::from_Start);
		
		// Walk the resource list, checking for conflicts.
		
		RFResource* theResource;
		Boolean collision = false;
		
		while (iter.Next(&theResource)) {
		
			// See if this resource ID conflicts with the ID we're testing.
		
			if ((theResource->GetResID() == startPoint) && !(theResource->IsDeleted())) {
				if (startPoint == 32767)
					startPoint = inStartingFrom - 1;
				else if (startPoint >= inStartingFrom)
					startPoint++;
				else
					startPoint--;
				collision = true;
			}
		}
		if (!collision)
			return startPoint;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** implementation details

// ---------------------------------------------------------------------------
//		* ScanRF												[protected]
// ---------------------------------------------------------------------------
//	Called by RMMap::ScanRF to scan the resource file for all resources of
//	this object's type.

void
RFType::ScanRF()
{

	// Validate pointers.
	
	ValidateThis_();

	// Enumerate the resources of this type that are in the file.
	// (Assume that we're already in the right resource context.)
	
	SInt16 numRsrcs = ::Count1Resources(GetResType());
	for (SInt16 i = 1; i <= numRsrcs; i++) {

		// Get a resource. Note that we cannot use the
		// ValidateHandle_ macro here because the resource
		// is *not* loaded.

		Handle theResourceH = ::Get1IndResource(GetResType(), i);
		ThrowIfNil_(theResourceH);

		// Create a resource object for this resource.

		RFResource* theResourceObj = new RFResource(*this, theResourceH);
		ValidateObject_(theResourceObj);

		// Add this resource to our list. First ensure that the
		// resource doesn't already exist in our map (which would be
		// an odd condition.)

		if (FindResource(theResourceObj->GetResID(), false)) {
			SignalCStr_("Duplicate resource found.");
			delete theResourceObj;
		}
		else {
		
			// Add it to the list.
		
			mResources.AddItem(&theResourceObj);
			
			// Broadcast a change message.
			
			RMMap_ChangeMessage msg = { &(GetRMMap()), this, theResourceObj };
			GetRMMap().BroadcastMessage(Resource_Added, &msg);

		}
	}
}