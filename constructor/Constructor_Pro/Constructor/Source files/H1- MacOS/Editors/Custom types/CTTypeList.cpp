// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTTypeList.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/09/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: CTTypeList.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/17/96   Time: 11:14
//	Updated in $/Constructor/Source files/Editors/Custom types
//	Added support for resource ID attributes. (Bug fix #1141.)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:23
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CTTypeList.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Core : Utility classes
#include "StKeepSharableAlive.h"

	// MacOS : Editors : Custom types
#include "CTModelObject.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

CTTypeList* CTTypeList::sTypeList = nil;


// ===========================================================================

#pragma mark *** CTTypeList ***

// ---------------------------------------------------------------------------
//		* CTTypeList()
// ---------------------------------------------------------------------------
//	Constructor

CTTypeList::CTTypeList()
{

	// Set global singleton pointer to this type list.

	if (sTypeList == nil)
		sTypeList = this;

}


// ---------------------------------------------------------------------------
//		* ~CTTypeList
// ---------------------------------------------------------------------------
//	Destructor

CTTypeList::~CTTypeList()
{

	// Cancel global singleton pointer.

	if (sTypeList == this)
		sTypeList = nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** singleton accessor

// ---------------------------------------------------------------------------
//		* GetTypeList											[static]
// ---------------------------------------------------------------------------
//	Return an accessor to the singleton type list.

CTTypeList*
CTTypeList::GetTypeList()
{
	
	// If a type list already exists, just return it.
	
	if (sTypeList != nil)
		return sTypeList;
	
	// None currently exists, create one.
	
	CTTypeList* typeList = new CTTypeList;
	ValidateObject_(typeList);
	
	StKeepSharableAlive keepIt(typeList);

	typeList->ScanAtLaunch();
	return typeList;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** type entry lookup

// ---------------------------------------------------------------------------
//		* FindPrototype
// ---------------------------------------------------------------------------
//	Find a data model prototype for a CTYP record of a given type.

CTModelObject*
CTTypeList::FindPrototype(
	ClassIDT	inObjectType) const
{

	// Validate pointers.
	
	ValidateThis_();

	// Search for the named type.
	
	DMFastIterator iter(mPrototypes);
	while (iter.NextElement()) {
		CTModelObject* theType = (dynamic_cast<CTModelObject*>(iter.CurrentElement()));
		ValidateObject_(theType);
		if (theType->mTypeID == inObjectType)
			return theType;
	}
	
	// Couldn't find it.
	
	return nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** type specification builders

// ---------------------------------------------------------------------------
//		* ScanAtLaunch											[protected]
// ---------------------------------------------------------------------------
//	Scan the app's resource fork and build up an internal type list of
//	the attribute types that can be added to the file.

void
CTTypeList::ScanAtLaunch()
{

	// Validate pointers.
	
	ValidateThis_();

	// Get all the resources of this type.
	
	StApplicationContext appContext;

	LArray resList(sizeof (void*));
	SInt32 numResources = ::Count1Resources('DSPC');
	Handle theResource;
	
	for (SInt32 index = 1; index <= numResources; index++) {

		// Get the next indexed resource.

		theResource = ::Get1IndResource('DSPC', index);
		ValidateHandle_(theResource);

		// Check its ID against the min/max bounds.

		Str255 name;
		ResIDT theID;
		ResType theType;
		::GetResInfo(theResource, &theID, &theType, name);
		if ((theID < 6000) || (theID > 6199))						//! TEMPORARY: magic numbers
			continue;
	
		// Bounds check is OK. Remove this handle from the resource map
		// and process it later.
	
		resList.InsertItemsAt(1, 0, &theResource);
		::HNoPurge(theResource);
		::LoadResource(theResource);
		::DetachResource(theResource);

	}

	// Now that we have all the resource, create specifications for them.
	// Keep iterating over the list until we've either handled all
	// resources, or we can't make any further progress.

	Boolean progress;
	do {
		progress = false;
		
		LArrayIterator iter(resList);
		while (iter.Next(&theResource)) {
			ValidateHandle_(theResource);
			LHandleStream resDataStream(theResource);
			try {
				MakeSpecFromDSPC(resDataStream);
				resList.Remove(&theResource);
				progress = true;
				continue;			// skip the detach handle
			}
			catch(...) {
				// ignore errors
			}
			resDataStream.DetachDataHandle();
		}
	} while (progress && (resList.GetCount() > 0));

	// Release forgotten resources.
	
	LArrayIterator iter(resList);
	while (iter.Next(&theResource)) {
		::DisposeHandle(theResource);
	}
}


// ---------------------------------------------------------------------------
//		* MakeSpecFromDSPC										[protected]
// ---------------------------------------------------------------------------
//	Read a DSPC resource and convert it to a data model specification.

void
CTTypeList::MakeSpecFromDSPC(
	LStream&	inStream)
{

	// Validate pointers.
	
	ValidateThis_();

	// Use the data model reanimator to create the type spec.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	DMElementPtr tempContainer;
	reanimator->ElementsFromStream(&inStream, tempContainer);
	ValidateObject_(tempContainer.GetObject());

	CTModelObjectPtr thisEntry;
	thisEntry = (dynamic_cast<CTModelObject*>(tempContainer.GetObject()));
	ValidateObject_(thisEntry.GetObject());

	// Place this class in the type list hierarchy.
	
	CTModelObject* dataModelParent = nil;

	// Make sure the class ID is valid.

	if (thisEntry->mTypeID == 0)
		Throw_(err_CantMakePrototype);
	
	// See if this class already exists. Can't replace built-in or locked types.
	
	if (FindPrototype(thisEntry->mTypeID) != nil)
		Throw_(err_CantMakePrototype);
	
	// See if we can find parent class.

	ClassIDT dataModelParentID = thisEntry->mParentTypeID;
	if ((dataModelParentID != 0) && (dataModelParentID != 0x3f3f3f3f)) {
		dataModelParent = FindPrototype(dataModelParentID);
		if (dataModelParent == nil)
			Throw_(err_CantMakePrototype);
	}
	
	// Copy attributes from parent class.
	
	if (dataModelParent != nil) {
		ValidateObject_(dataModelParent);
		
		DMFastIterator iter(dataModelParent->GetAttributes(), LArrayIterator::from_End);
		while (iter.PreviousElement()) {
			DMElement* element = iter.CurrentElement();
			thisEntry->InsertElementAt(1, element, false);
		}
	}
	
	// Put this into the type list.

	mPrototypes.InsertItemAt(LArray::index_Last, thisEntry);

}
