// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMType.cpp					© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/09/95
//     $Date: 2006/01/18 01:33:25 $
//	$History: RMType.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:46
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:24
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:08
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Refactored the RF* classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/23/97   Time: 14:06
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Cleaned up source in preparation for refactoring.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// The following headers are needed to compile this code on Windows
#ifdef WINVER
	#include "asiport.h"			// Altura
	#include "asiextrn.h"			// Altura
	#include "ctor.h"				// Constructor Windows include
#endif

// ===========================================================================

#include "RMType.h"

	// Core : Resource manager
#include "RMMap.h"
#include "RMResource.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** RMType ***

// ---------------------------------------------------------------------------
//		* RMType(RMMap&)										[protected]
// ---------------------------------------------------------------------------
//	Constructor, specifying the RMMap that the type belongs to.

RMType::RMType(
	RMMap&		inResMap)

: mResMap(inResMap),
  mResources(sizeof (RMResource*))

{
}


// ---------------------------------------------------------------------------
//		* ~RMType												[protected]
// ---------------------------------------------------------------------------
//	Destructor. RMTypes should only be deleted by the RMMap that owns them.

RMType::~RMType()
{

	// Delete each resource.

	LFastArrayIterator iter(mResources);
	RMResource* theResource;
	
	while (iter.Next(&theResource)) {
		ValidateObject_(theResource);
		delete theResource;
	}
}


// ===========================================================================

#pragma mark ** resource accessors

// ---------------------------------------------------------------------------
//		* IsModified
// ---------------------------------------------------------------------------
//	Returns true if any resource has been added, modified, or deleted.

Boolean
RMType::IsModified()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Ask each resource if it has been modified.
	// Short-circuit evaluation: If any resource has been modified,
	// stop immediately.

	LFastArrayIterator iter(mResources);
	RMResource* theResource;
	while (iter.Next(&theResource)) {
		ValidateObject_(theResource);
		if (theResource->IsModified())
			return true;
	}
	
	// No modified resources.
	
	return false;

}


// ===========================================================================

#pragma mark -
#pragma mark ** implementation details

// ---------------------------------------------------------------------------
//		* UpdateRF												[protected]
// ---------------------------------------------------------------------------
//	Called by RMMap::UpdateRF to write all the changes for resources
//	of this type to the file.

void
RMType::UpdateRF()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Tell each resource to update the res file.

	LFastArrayIterator iter(mResources);
	RMResource* theResource;
	
	while (iter.Next(&theResource)) {
		ValidateObject_(theResource);
		theResource->UpdateRF();
	}
}


// ---------------------------------------------------------------------------
//		* UpdateComplete										[protected]
// ---------------------------------------------------------------------------
//	Called when a save operation was completed successfully. Clears all
//	modified flags, deletes the RMResource objects for removed resources.

void
RMType::UpdateComplete()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Tell each resource that the update is complete.

	LArrayIterator iter(mResources);
	RMResource* theResource;

	while (iter.Next(&theResource)) {
		ValidateObject_(theResource);
		theResource->UpdateComplete();
	}
}


// ---------------------------------------------------------------------------
//		* ResourceDeleted										[protected]
// ---------------------------------------------------------------------------
//	Called by RMResource when it wants to be deleted. (Usually from a chain
//	of UpdateComplete events.)

void
RMType::ResourceDeleted(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inResource);
	
	// Remove the resource from our list.

	mResources.Remove(&inResource);
	
	// Delete the resource object.
	
	delete inResource;

}
