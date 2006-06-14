// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RETypeEntry.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/96
//	   $Date: 2006/01/18 01:33:06 $
//	$History: RETypeEntry.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/16/96   Time: 14:49
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Fixed a minor bug in MatchResType. (Bug fix #1113.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:19
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

#include "RETypeEntry.h"

	// Core : Editors : Generic resource editor
#include "RETypeList.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// Core : Utilities
#include "UIconUtilities.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================

#pragma mark *** RETypeEntry ***

// ---------------------------------------------------------------------------
//		* RETypeEntry()
// ---------------------------------------------------------------------------
//	Constructor

RETypeEntry::RETypeEntry()

: mResTypes(sizeof (ResType)),
  mDefaultResIDs(sizeof (ResIDT))

{
	mFirstPrimaryType = 0;
	mPrimaryResTypeCount = 0;
	mResTypeName[0] = 0;
	mResFolderName[0] = 0;
	mResTypeIcon = nil;
	mResFolderIcon = nil;
	mSortSequence = 0;
	mCreateEmptyFolder = false;
	mUpdateResDataOnIDChange = false;
	mDefaultNewResID = 128;
}


// ---------------------------------------------------------------------------
//		* ~RETypeEntry
// ---------------------------------------------------------------------------
//	Destructor

RETypeEntry::~RETypeEntry()
{

	// Validate pointers.

	ValidateThis_();

	// Release resource type/folder icons.

	if (mResTypeIcon != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(mResTypeIcon);
#else
		ValidateHandle_(mResTypeIcon);
#endif
		::DisposeIconSuite(mResTypeIcon, true);
	}
	
	if (mResFolderIcon != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(mResFolderIcon);
#else
		ValidateHandle_(mResFolderIcon);
#endif
		::DisposeIconSuite(mResFolderIcon, true);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource type locator
	
// ---------------------------------------------------------------------------
//		* FindResTypeEntry										[static]
// ---------------------------------------------------------------------------
//	Find the resource type entry for a given ResType.

RETypeEntry*
RETypeEntry::FindResTypeEntry(
	ResType		inResType)
{

	// Find resource type list.
	
	RETypeList* typeList = RETypeList::GetTypeList();
	ValidateObject_(typeList);
	
	// Ask it for the type entry.
	
	return typeList->FindResTypeEntry(inResType);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource type accessors
	
// ---------------------------------------------------------------------------
//		* MatchResType
// ---------------------------------------------------------------------------
//	Returns true if this type entry describes the resource type.

Boolean
RETypeEntry::MatchResType(
	ResType		inResType) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Optimization: See if it matches the "first primary type."
	
	if (inResType == mFirstPrimaryType)
		return true;
	
	// If there are more primary types, check the list.
	
	if (mPrimaryResTypeCount > 1) {
		LFastArrayIterator iter(mResTypes, 1);
		ResType primaryType;
		
		while (iter.Next(&primaryType) && (iter.GetCurrentIndex() <= mPrimaryResTypeCount)) {
			if (inResType == primaryType)
				return true;
		}
	}
	
	// No match.
	
	return false;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** default resource accessors
	
// ---------------------------------------------------------------------------
//		* CountDefaultResources
// ---------------------------------------------------------------------------
//	Return the number of default resources for this resource type.

SInt32
RETypeEntry::CountDefaultResources() const
{

	// Validate pointers.

	ValidateThis_();

	// Return number of resources in default resources ID array.
	
	return mDefaultResIDs.GetCount();

}


// ---------------------------------------------------------------------------
//		* GetIndexedDefaultResName
// ---------------------------------------------------------------------------
//	Get the name of the nth default resource for this type.

void
RETypeEntry::GetIndexedDefaultResName(
	SInt32		inResIndex,
	LStr255&	outResName) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Get the resource's ID.
	
	ResIDT resID;
	if (mDefaultResIDs.FetchItemAt(inResIndex, &resID)) {
	
		// Okay, we have a valid ID. Get the resource's name.
		// Disable resource loading. We don't need the res data here.

		StApplicationContext appContext;
		StResLoad noLoad(false);
		
		// Get the resource handle. Verify that a resource was loaded.
		
		StResource defaultResource(mFirstPrimaryType, resID, false, true);
		ThrowIfResFail_((Handle) defaultResource);
		
		// Grab the resource name from it.
		
		ResType defResType;
		ResIDT defResID;
		::GetResInfo(defaultResource, &defResID, &defResType, outResName);
	
	}
	else {
		
		// The resource index is invalid.
		
		SignalCStr_("Invalid resource index");
		outResName[(UInt8)0] = 0;
		
	}
	
}


// ---------------------------------------------------------------------------
//		* GetIndexedDefaultResData
// ---------------------------------------------------------------------------
//	Get the data from the nth default resource for this type.
//	A detached resource handle is returned. The caller is responsible
//	for disposing the handle when done.

Handle
RETypeEntry::GetIndexedDefaultResData(
	SInt32		inResIndex,
	ResType		inResType) const
{

	// Validate pointers.

	ValidateThis_();
	
	Handle	returnResource = nil;
	
	// Get the resource's ID.
	
	ResIDT resID;
	if (mDefaultResIDs.FetchItemAt(inResIndex, &resID)) {
	
		// Okay, we have a valid ID. Get the resource's name.
		// Be sure that the resource is loaded.

		StApplicationContext appContext;
		StResLoad loadEmUp(true);
		
		// Get the resource handle. Verify that a resource was loaded.
	
		returnResource = ::Get1Resource(inResType, resID);
		
		if (returnResource != nil) {
			
			// Okay, a resource was loaded. Detach it from our resource map.
			
			ValidateHandle_(returnResource);
			::HNoPurge(returnResource);
			::DetachResource(returnResource);
		}
	
	} else {
		
		// The resource index is invalid.
		
		SignalCStr_("Invalid resource index");
		
	}
		
	return returnResource;
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* InitializeFromRSCP									[protected]
// ---------------------------------------------------------------------------
//	Fill in the data members of this resource type entry from a stream.
//	The stream must be in the format of an RSCP resource.
//	Returns false if initialization failed.

Boolean
RETypeEntry::InitializeFromRSCP(
	LStream&	inStream)
{

	// Validate pointers.

	ValidateThis_();

	// Check resource version numbers.
	
	SInt16 currentVersion;
	SInt16 minVersion;
	
	inStream	>> currentVersion
				>> minVersion;

	if (currentVersion < 1) {
		SignalCStr_("Invalid RSCP format -- discarding resource");
		return false;
	}
	if (minVersion > 1) {
		SignalCStr_("RSCP format is too new -- discarding resource");
		return false;
	}
	
	// Read primary resource types.
	
	SInt16 primaryCount;
	inStream >> primaryCount;
	mPrimaryResTypeCount = primaryCount;
	
	ResType resType;
	while (primaryCount-- > 0) {
	
		// Insert each primary type into our array. Also keep the first
		// primary type as an optimization.
		
		inStream >> resType;
		if (mResTypes.InsertItemsAt(1, LArray::index_Last, &resType) == 1)
			mFirstPrimaryType = resType;
	
	}
	
	// Read secondary resource types.
	
	SInt16 secondaryCount;
	inStream >> secondaryCount;
	
	while (secondaryCount-- > 0) {
	
		// Insert each secondary type into our array.
		
		inStream >> resType;
		mResTypes.InsertItemsAt(1, LArray::index_Last, &resType);
	
	}
	
	// Read resource type and folder names.
	
	inStream	>> mResTypeName
				>> mResFolderName;

	// Read (and ignore) resource editor class name.
	// This will be used later when we have plug-ins.
	
	Str255 resEditorClassName;
	inStream	>> resEditorClassName;
	
	// Read resource type and folder icons.
	
	ResIDT resTypeIconID;
	ResIDT resFolderIconID;
	
	inStream	>> resTypeIconID
				>> resFolderIconID;

	if (resTypeIconID != 0) {
		mResTypeIcon = UIconUtilities::Get1DetachedIconSuite(resTypeIconID, svAllAvailableData);
#if PP_Target_Carbon
		ThrowIfNil_(mResTypeIcon);
#else
		ValidateHandle_(mResTypeIcon);
#endif
	}
	
	if (resFolderIconID != 0) {
		mResFolderIcon = UIconUtilities::Get1DetachedIconSuite(resFolderIconID, svAllAvailableData);
#if PP_Target_Carbon
		ThrowIfNil_(mResFolderIcon);
#else
		ValidateHandle_(mResFolderIcon);
#endif
	}

	// Read default resource IDs.
	
	ResIDT firstDefaultID;	
	ResIDT lastDefaultID;
	
	inStream	>> firstDefaultID
				>> lastDefaultID;

	ResIDT masterID = firstDefaultID - 1;
	while ((++masterID) <= lastDefaultID) {

		// See if a default resource of this ID exists.

		StResource master(mFirstPrimaryType, masterID, false, true);
		if ((Handle) master == nil)
			continue;
		
		// It does, read its ID and store it for later reference.

		ValidateHandle_(master);
		Str255 resName;
		ResType resType;
		ResIDT resID;
		::GetResInfo(master, &resID, &resType, resName);
		mDefaultResIDs.InsertItemsAt(1, LArray::index_Last, &resID);

	}
	
	// Read flags at end of resource.
	
	inStream	>> mSortSequence
				>> mCreateEmptyFolder
				>> mUpdateResDataOnIDChange
				>> mDefaultNewResID;
	
	if (mSortSequence == 0)
		mSortSequence = mFirstPrimaryType;

	// Signal successful build.
	
	return true;

}
