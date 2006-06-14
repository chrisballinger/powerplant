// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RFMap.cpp				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/23/97
//     $Date: 2006/01/18 01:34:06 $
//  $History: RFMap.cpp $
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

#include "RFMap.h"

	// Core : Resource manager
#include "RMIdentifier.h"
//#include "StResourceContext.h"

	// MacOS : Resource manager
#include "RFResource.h"
#include "RFType.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** RFMap ***

// ---------------------------------------------------------------------------
//		* RFMap()
// ---------------------------------------------------------------------------
//	Default constructor

RFMap::RFMap()
{
}


// ---------------------------------------------------------------------------
//		* ~RFMap
// ---------------------------------------------------------------------------
//	Destructor

RFMap::~RFMap()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource accessors

// ---------------------------------------------------------------------------
//		* FindResource(RMIdentifier&, RMIdentifier&, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the RFResource object for the specified type
//	and ID. If the resource doesn't already exist, the flag
//	inCreateIfNeeded is examined: if true, the resource object is created;
//	otherwise, nil is returned.

RMResource*
RFMap::FindResource(
	const RMIdentifier&	inResType,
	const RMIdentifier&	inResID,
	Boolean				inCreateIfNeeded)
{

	// If type or ID aren't numeric, we won't find it and can't create it.

	if (!inResType.IsNumericID() || !inResID.IsNumericID())
		return nil;
	
	// Both are kosher, now try to find the resource.
	
	return FindResource(inResType.GetNumericID(), inResID.GetNumericID(), inCreateIfNeeded);

}


// ---------------------------------------------------------------------------
//		* FindResource(ResType, Int16, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the RFResource object for the specified type
//	and ID. If the resource doesn't already exist, the flag
//	inCreateIfNeeded is examined: if true, the resource object is created;
//	otherwise, nil is returned.

RFResource*
RFMap::FindResource(
	ResType			inResType,
	SInt16			inResID,
	Boolean			inCreateIfNeeded)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find the resource type entry for this ResType.

	RFType* theType = FindResType(inResType, inCreateIfNeeded);
	if (theType != nil) {
	
		// Found it: Ask it to find the individual resource.
	
		ValidateObject_(theType);
		return theType->FindResource(inResID, inCreateIfNeeded);

	}
	else {
	
		// No resource type entry exist for this ResType, so
		// obviously the resource itself doesn't exist.
	
		return nil;
	}
}


// ---------------------------------------------------------------------------
//		* FindResType(RMIdentifier&, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the RFType object corresponding to a given
//	resource type. If the type doesn't exist and inCreateIfNeeded is
//	true, it will be created.

RMType*
RFMap::FindResType(
	const RMIdentifier&	inResType,
	Boolean				inCreateIfNeeded)
{

	// If type isn't numeric, we won't find it and can't create it.

	if (!inResType.IsNumericID())
		return nil;
	
	// Both are kosher, now try to find the type.
	
	return FindResType(inResType.GetNumericID(), inCreateIfNeeded);

}


// ---------------------------------------------------------------------------
//		* FindResType(ResType, Boolean)
// ---------------------------------------------------------------------------
//	Returns a pointer to the RFType object corresponding to a given
//	resource type. If the type doesn't exist and inCreateIfNeeded is
//	true, it will be created.

RFType*
RFMap::FindResType(
	ResType		inResType,
	Boolean		inCreateIfNeeded)
{

	// Validate pointers.
	
	ValidateThis_();

	// See if this resource type already exists.

	{
		LFastArrayIterator iter(mResTypes);
		RFType* theType;
		while (iter.Next(&theType)) {
			ValidateObject_(theType);
			if (theType->GetResType() == inResType)
				return theType;
		}
	}
	
	// It doesn't. Create it if so requested.
	
	if (inCreateIfNeeded) {
		
		// Create the res type.
		
		RFType* newType = new RFType(*this, inResType);
		ValidateObject_(newType);

		// Add it to the type list.
		
		mResTypes.AddItem(&newType);

		// Broadcast a change message.

		RMMap_ChangeMessage msg = { this, newType, nil };
		BroadcastMessage(ResourceType_Added, &msg);
		
		// Return the newly-created type.
		
		return newType;

	}
	else {
	
		// No such res type find.
	
		return nil;
		
	}
}


// ---------------------------------------------------------------------------
//		* GetResourceContext
// ---------------------------------------------------------------------------
//	Return the resource fork refnum for the primary resource file.

SInt16
RFMap::GetResourceContext() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Default to no resource file.

	SInt16 theResourceContext = ResourceContext_NoFile;

	// If we have a file, ask it for the resource fork's ref num.
	// Can't validate mMainFile since it might be a stack-based
	// object.

	if (mMainFile != nil)
		theResourceContext = mMainFile->GetResourceForkRefNum();

	// Return that ref num.

	return theResourceContext;

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource file updating

// ---------------------------------------------------------------------------
//		* UpdateRF
// ---------------------------------------------------------------------------
//	Write all the changes in the resource map to the file.

void
RFMap::UpdateRF()
{

	// Tell individual resources to write to file.
	
	RMMap::UpdateRF();
	
	// Tell the Mac Resource Manager to flush the file.
	
	::UpdateResFile(GetResourceContext());

}


// ===========================================================================

#pragma mark -
#pragma mark ** implementation details

// ---------------------------------------------------------------------------
//		* ScanRF												[private]
// ---------------------------------------------------------------------------
//	Scan the resource map and create RMType and RMResource objects to 
//	correspond to the actual types and resources in the file.

void
RFMap::ScanRF()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See if we have a file to scan.
	
	LFile* file = GetMainFile();
	ValidateObject_(file);
	
	// Switch to the file's resource fork.
	
	StResourceContext resFile(file->GetResourceForkRefNum());
	if (!resFile.IsValid())
		return;
	
	// Prevent resource loading.
	
	StResLoad noLoad(false);
	
	// Walk the file's res type list.
	
	SInt16 numTypes = ::Count1Types();
	for (SInt16 i = 1; i <= numTypes; i++) {
	
		// Get the next resource type.

		ResType theType = 0;
		::Get1IndType(&theType, i);
		ThrowIfNil_(theType);
		
		// Create a resource type object for this type.

		RFType* theRFType = FindResType(theType, true);
		ValidateObject_(theRFType);
		
		// Ask the resource type to scan for resources of that type.

		theRFType->ScanRF();

	}
}
