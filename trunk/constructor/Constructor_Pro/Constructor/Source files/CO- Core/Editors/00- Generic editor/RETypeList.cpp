// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RETypeList.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/96
//	   $Date: 2006/04/12 08:47:45 $
//	$History: RETypeList.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:20
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

#include "RETypeList.h"

	// Core : Editors : Generic resource editor
#include "RETypeEntry.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Core : Utilities
#include "UApplicationFile.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

RETypeList*	RETypeList::sTypeList = nil;				// pointer to singleton class list


// ===========================================================================

#pragma mark *** RETypeList ***

// ---------------------------------------------------------------------------
//		* RETypeList()											[protected]
// ---------------------------------------------------------------------------
//	Constructor. Created only by GetTypeList.

RETypeList::RETypeList()

: mResTypeEntries(sizeof (RETypeEntry*))

{
	mResTypeEntries.SetComparator(new RETypeSortComparator);
	mResTypeEntries.SetKeepSorted(true);
}


// ---------------------------------------------------------------------------
//		* ~RETypeList											[protected]
// ---------------------------------------------------------------------------
//	Destructor. Deleted only by ReleaseTypeList.

RETypeList::~RETypeList()
{

	// Validate pointers.

	ValidateThis_();
	
	// Delete all of the resource type entries.
	
	LArrayIterator iter(mResTypeEntries);
	RETypeEntry* typeEntry;
	while (iter.Next(&typeEntry)) {
		ValidateObject_(typeEntry);
		delete typeEntry;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** singleton accessor
	
// ---------------------------------------------------------------------------
//		* GetTypeList											[static]
// ---------------------------------------------------------------------------
//	Return the pointer to the singleton resource type list.
//	If none exists, create it.

RETypeList*
RETypeList::GetTypeList()
{

	// Create singleton object if it doesn't already exist.

	if (sTypeList == nil) {
		
		sTypeList = new RETypeList;
		ValidateObject_(sTypeList);
		
		sTypeList->ScanAtLaunch();
	
	}
	
	// Return the existing singleton.
	
	ValidateObject_(sTypeList);
	return sTypeList;
	
}


// ---------------------------------------------------------------------------
//		* ReleaseTypeList										[static]
// ---------------------------------------------------------------------------
//	Should be called only when the application is closing down.
//	Deletes the type list object and all type entries.

void
RETypeList::ReleaseTypeList()
{

	// Delete singleton if it exists.

	if (sTypeList != nil) {
		ValidateObject_(sTypeList);
		delete sTypeList;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark **	type entry lookup
	
// ---------------------------------------------------------------------------
//		* FindResTypeEntry
// ---------------------------------------------------------------------------
//	Find the type entry that matches the named resource type.

RETypeEntry*
RETypeList::FindResTypeEntry(
	ResType		inResType) const
{

	// Validate pointers.
	
	ValidateThis_();

	// Scan the resource type list and ask each one if it matches.

	LFastArrayIterator iter(mResTypeEntries);
	RETypeEntry* typeEntry;
	while (iter.Next(&typeEntry)) {
		ValidateObject_(typeEntry);
		if (typeEntry->MatchResType(inResType))
			return typeEntry;
	}
	
	// No match.
	
	return nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** type specification builders

// ---------------------------------------------------------------------------
//		* ScanAtLaunch											[protected]
// ---------------------------------------------------------------------------
//	Build the list of resource types that we know how to edit.

void
RETypeList::ScanAtLaunch()
{

	// Validate pointers.
	
	ValidateThis_();

	// Find application file.
	
	FSSpec appFile;
	UApplicationFile::GetApplicationFile(appFile);

	// Read type descriptions from application file.

	{
		StApplicationContext context;
		ScanCurrentResourceFile(appFile);
	}
	
	// Once plug-in interface is defined, we'll add code here to scan
	// the plug-in folder/files.

}


// ---------------------------------------------------------------------------
//		* ScanCurrentResourceFile								[protected]
// ---------------------------------------------------------------------------
//	Scan the current resource file for resource type descriptions.

void
RETypeList::ScanCurrentResourceFile(
	const FSSpec&	inSourceFile)
{

	// Validate pointers.

	ValidateThis_();

	// Look at each RSCP resource.
	
	SInt32 numResources = ::Count1Resources('RSCP');
	SInt32 resIndex = 0;
	
	while ((++resIndex) <= numResources) {

		// Okay, we have a resource. Grab it.
		
		Handle rscpResource = ::Get1IndResource('RSCP', resIndex);
		ThrowIfResFail_(rscpResource);

		ValidateHandle_(rscpResource);
		::HNoPurge(rscpResource);
		::DetachResource(rscpResource);
		ValidateHandle_(rscpResource);
		
		// Try to make a resource type spec from it.
		
		LHandleStream rscpData(rscpResource);
		MakeSpecFromRSCP(rscpData, inSourceFile);
		
	}
}


// ---------------------------------------------------------------------------
//		* MakeSpecFromRSCP										[protected]
// ---------------------------------------------------------------------------

void
RETypeList::MakeSpecFromRSCP(
	LStream&		inStream,
	const FSSpec&	/* inSourceFile */)
{

	// Validate pointers.
	
	ValidateThis_();

	// Attempt to create a type entry object.
	
	RETypeEntry* newEntry = new RETypeEntry;
	ValidateObject_(newEntry);
	
	// Fill in type entry's data from RSCP resource.
	
	newEntry->InitializeFromRSCP(inStream);
	
	// Add the entry to our list of types.
	
	mResTypeEntries.InsertItemsAt(1, LArray::index_Last, &newEntry);
	
}


// ===========================================================================

#pragma mark -
#pragma mark *** RETypeSortComparator ***

// ---------------------------------------------------------------------------
//		* Compare
// ---------------------------------------------------------------------------
//	Compare two RETypeEntry objects by their sort sequence.

SInt32
RETypeSortComparator::Compare(
	const void*		inItemOne,
	const void* 	inItemTwo,
	UInt32			/* inSizeOne */,
	UInt32			/* inSizeTwo */) const
{
	RETypeEntry* item1RE = *((RETypeEntry**) inItemOne);
	RETypeEntry* item2RE = *((RETypeEntry**) inItemTwo);

	ValidateObject_(item1RE);
	ValidateObject_(item2RE);

	return item1RE->GetSortSequence() - item2RE->GetSortSequence();
}


// ---------------------------------------------------------------------------
//		* IsEqualTo
// ---------------------------------------------------------------------------
//	Overriden to do pointer comparison on the two objects.

Boolean
RETypeSortComparator::IsEqualTo(
	const void*		inItemOne,
	const void* 	inItemTwo,
	UInt32			/* inSizeOne */,
	UInt32			/* inSizeTwo */) const
{
	RETypeEntry* item1RE = *((RETypeEntry**) inItemOne);
	RETypeEntry* item2RE = *((RETypeEntry**) inItemTwo);

	ValidateObject_(item1RE);
	ValidateObject_(item2RE);

	return item1RE == item2RE;
}
