// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMMap.cpp					© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/09/95
//     $Date: 2006/04/12 08:48:13 $
//	$History: RMMap.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:24
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/27/97   Time: 14:01
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Removed the multi-file capability. It was never used anyway.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:08
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Refactored the RF* classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/23/97   Time: 15:02
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Fixed a missing #include.
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

#include "RMMap.h"

	// Core : Resource manager
#include "RMType.h"
//#include "StResourceContext.h"

	// PowerPlant : Utilities
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** RMMap ***

// ---------------------------------------------------------------------------
//		* RMMap()
// ---------------------------------------------------------------------------
//	Constructor

RMMap::RMMap()

: mResTypes(sizeof (RMType*))

{
	mMainFile = nil;
}


// ---------------------------------------------------------------------------
//		* ~RMMap
// ---------------------------------------------------------------------------
//	Destructor

RMMap::~RMMap()
{

	// Tell everyone we're going away.

	RMMap_ChangeMessage msg = { this, nil, nil };
	BroadcastMessage(ResourceMap_GoingAway, &msg);

	// Delete all the resource types, which will in turn delete all the resources.

	LFastArrayIterator iter(mResTypes);
	RMType* theType;

	while (iter.Next(&theType)) {
		ValidateObject_(theType);
		delete theType;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource file updating

// ---------------------------------------------------------------------------
//		* UpdateRF
// ---------------------------------------------------------------------------
//	Write all the changes in the resource map to the file.

void
RMMap::UpdateRF()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Walk each resource type, telling it to update the file.

	LFastArrayIterator iter(mResTypes);
	RMType* theType;
	
	while (iter.Next(&theType)) {
		ValidateObject_(theType);
		theType->UpdateRF();
	}
}


// ---------------------------------------------------------------------------
//		* UpdateComplete
// ---------------------------------------------------------------------------
//	Called when a save operation was completed successfully. Clears all
//	modified flags, deletes the RMResource objects for removed resources.

void
RMMap::UpdateComplete()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Tell each resource type that the update completed.

	LFastArrayIterator iter(mResTypes);
	RMType* theType;
	while (iter.Next(&theType)) {
		ValidateObject_(theType);
		theType->UpdateComplete();
	}
}


// ---------------------------------------------------------------------------
//		* IsModified
// ---------------------------------------------------------------------------
//	Returns true if any resource has been added, modified, or deleted.

Boolean
RMMap::IsModified()
{

	// Validate pointers.
	
	ValidateThis_();

	// Ask each res type if they've been modified.
	// Shortcut out if anyone has been modified.

	LFastArrayIterator iter(mResTypes);
	RMType* theType;
	
	while (iter.Next(&theType)) {
		ValidateObject_(theType);
		if (theType->IsModified())
			return true;
	}
	
	// None were modified.
	
	return false;

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource file access

// ---------------------------------------------------------------------------
//		* ScanNewMainFile
// ---------------------------------------------------------------------------
//	Attach a file object to this resource map. Scans the resource map and
//	creates RMType objects for all resource types, and RMResource objects
//	for all resources in the file. Used when opening a new file.

void
RMMap::ScanNewMainFile(
	LFile*	inFile)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inFile);
	
	// Save pointer to this new file.

	mMainFile = inFile;
	
	// Scan for resources.
	
	ScanRF();

}


// ---------------------------------------------------------------------------
//		* SetMainFile
// ---------------------------------------------------------------------------
//	Change the main resource file associated with this map without causing
//	a rescan. Should be used when a "Save As..." is done.

void
RMMap::SetMainFile(
	LFile*	inFile)
{

	// Validate pointers. Can't validate "inFile" since it might be
	// a stack-based object.
	
	ValidateThis_();
	
	// Save pointer to new file.
	
	mMainFile = inFile;

}
