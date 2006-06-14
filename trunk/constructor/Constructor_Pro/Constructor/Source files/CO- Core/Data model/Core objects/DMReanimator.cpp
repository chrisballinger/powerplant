// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMReanimator.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMReanimator.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/03/96   Time: 11:38
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Changed internal CTYP resources to DSPC.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMReanimator.h"

	// Core : Data model : Core objects
#include "DMElement.h"
#include "DMContainerMixin.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

DMReanimator* DMReanimator::sReanimator = nil;


// ===========================================================================
//		* DSPC resource format
// ===========================================================================
//	Looks a lot like the PPob resource format...

typedef SInt32 TagID;

enum {
	tag_ObjectData		= 'objd',
	tag_BeginSubs		= 'begs',
	tag_EndSubs			= 'ends',
	tag_ClassAlias		= 'dopl',
	tag_End				= 'end.',
	
	object_Null			= 'null'
};


// ===========================================================================

#pragma mark *** DMReanimator ***

// ---------------------------------------------------------------------------
//		* DMReanimator()
// ---------------------------------------------------------------------------
//	Constructor

DMReanimator::DMReanimator()
{
	if (sReanimator == nil)
		sReanimator = this;

	mTableH = nil;
	mClassCount = 0;
}


// ---------------------------------------------------------------------------
//		* ~DMReanimator
// ---------------------------------------------------------------------------
//	Destructor

DMReanimator::~DMReanimator()
{
	ValidateThis_();

	if (sReanimator == this)
		sReanimator = nil;

	if (mTableH != nil) {
		ValidateHandle_((Handle) mTableH);
		::DisposeHandle((Handle) mTableH);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** prototype builder

// ---------------------------------------------------------------------------
//		* ElementsFromResource
// ---------------------------------------------------------------------------
//	This method can be called to reanimate elements from a resource
//	in the 'DSPC' format. Specify the resource type and ID, and provide
//	a DMElementPtr to hold the newly built elements.

void
DMReanimator::ElementsFromResource(
	ResType			inResType,
	ResIDT			inResID,
	DMElementPtr&	inElementPtr)
{
	ValidateThis_();

	StApplicationContext appContext;
	StResLoad loadEmUp(true);
	StResource objectRes(inResType, inResID);

	ValidateHandle_(objectRes.mResourceH);
	::HLockHi(objectRes.mResourceH);
	
	LDataStream objectStream(*objectRes.mResourceH,
							 ::GetHandleSize(objectRes.mResourceH));

	ElementsFromStream(&objectStream, inElementPtr);
	
	ValidateObject_(inElementPtr.GetObject());

}


// ---------------------------------------------------------------------------
//		* ElementsFromHandle
// ---------------------------------------------------------------------------
//	This method can be called to reanimate elements from a handle
//	in the 'DSPC' format. Specify the handle and provide a DMElementPtr
//	to hold the newly built elements.

void
DMReanimator::ElementsFromHandle(
	Handle			inHandle,
	DMElementPtr&	inElementPtr)
{
	ValidateThis_();
	ValidateHandle_(inHandle);
	
	::HLockHi(inHandle);

	LDataStream objectStream(*inHandle, ::GetHandleSize(inHandle));
	ElementsFromStream(&objectStream, inElementPtr);
	
	ValidateObject_(inElementPtr.GetObject());
}


// ---------------------------------------------------------------------------
//		* ElementsFromStream
// ---------------------------------------------------------------------------
//	Read the header of the 'DSPC' format, then call ReadElementsFromTags
//	to read the description of the elements.

void
DMReanimator::ElementsFromStream(
	LStream*		inStream,
	DMElementPtr&	inElementPtr)
{
	ValidateThis_();

	SInt16 ctypVersion = 0;
	*inStream >> ctypVersion;
	SignalIf_(ctypVersion != 1);
	ReadElementsFromTags(inStream, nil, inElementPtr);
	
	ValidateObject_(inElementPtr.GetObject());
	
	if (inElementPtr != nil) {
		inElementPtr->ResetModified();
		inElementPtr->FinishCreate();
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** element class registration

// ---------------------------------------------------------------------------
//		* RegisterClass
// ---------------------------------------------------------------------------
//	Like URegistrar::RegisterClass, register a data model class and the
//	static helper function which can create an object of that class from stream.

void
DMReanimator::RegisterClass(
	ClassIDT				inClassID,
	DM_ElementCreatorFunc	inCreatorFunc)
{

	ValidateThis_();

	// One-based index into class table

	SInt16 classIndex = 1;
	if (mClassCount == 0) {
	
		// Create new class table.
		
		mTableH = (DM_ClassTableH) ::NewHandle(sizeof (DM_ClassTable));
		ThrowIfMemFail_(mTableH);
		mClassCount = 1;
	}
	else {

		// See if class is already in table.
		
		classIndex = FetchClassIndex(inClassID);
		if (classIndex == 0) {
		
			// Not in table, so grow table by 1.
			
			ValidateHandle_((Handle) mTableH);
			::SetHandleSize((Handle) mTableH, (mClassCount+1) * sizeof (DM_ClassTable));
			ThrowIfMemError_();

			mClassCount++;
			classIndex = mClassCount;
		}
	}

	// Store ID and CreatorFunc.

	ValidateHandle_((Handle) mTableH);	
	(*mTableH)[classIndex-1].classID = inClassID;
	(*mTableH)[classIndex-1].creatorFunc = inCreatorFunc;

}


// ---------------------------------------------------------------------------
//		* CreateObject
// ---------------------------------------------------------------------------
//	Create a data model element class from a class ID and data stream.

void
DMReanimator::CreateObject(
	ClassIDT		inClassID,
	LStream*		inStream,
	DMElementPtr&	inElementPtr)
{
	ValidateThis_();

	SInt16 index = FetchClassIndex(inClassID);
	if (index != 0) {
		ValidateHandle_((Handle) mTableH);
		inElementPtr = (*(*mTableH)[index - 1].creatorFunc)(inStream);
		ValidateObject_(inElementPtr.GetObject());
	}
	else
		inElementPtr = nil;
}


// ===========================================================================

#pragma mark -
#pragma mark ** implementation details

// ---------------------------------------------------------------------------
//		* ReadElementsFromTags									[protected]
// ---------------------------------------------------------------------------
//	Called recursively to unpack each level of the data model element
//	hierarchy from the data stream.

void
DMReanimator::ReadElementsFromTags(
	LStream*			inStream,
	DMContainerMixin*	inHostElement,
	DMElementPtr&		inElementPtr)
{

	ValidateThis_();

	DMContainerMixin* newHost = nil;
	ClassIDT aliasClassID = 'null';
	Boolean readingTags = true;

	do {
		TagID theTag = tag_End;
		*inStream >> theTag;
		
		switch (theTag) {
		
			case tag_ObjectData: {
				
				// Object data consists of a byte length, class ID,
				// and then the data for the object. We use the
				// byte length to manually set the stream marker
				// after creating the object just in case the
				// object constructor doesn't read all the data.
					
				SInt32 dataLength;
				*inStream >> dataLength;
				
				SInt32 dataStart = inStream->GetMarker();
				ClassIDT classID;
				*inStream >> classID;
				
				// See if previous tag specified an alias.
				
				if (aliasClassID != 'null')
					classID = aliasClassID;

				// Create the object.
								
				DMElementPtr currentObject;
				CreateObject(classID, inStream, currentObject);
				ValidateObject_(currentObject.GetObject());
				
				inStream->SetMarker(dataStart + dataLength, streamFrom_Start);
				
				// Cancel the alias.
				
				aliasClassID = 'null';
				
				// If we created an object, make sure it takes its correct place in hierarchy.
				
				if (currentObject != nil) {

					if (inElementPtr == nil)
						inElementPtr = currentObject;
					if (inHostElement != nil) {
						// Can't validate inHostElement: it's a mixin pointer.
						inHostElement->InsertElementAt(LArray::index_Last, currentObject, true);
					}

					newHost = (dynamic_cast<DMContainerMixin*>(currentObject.GetObject()));
					// Can't validate newHost since it's a mixin class.

				}
				else if (classID != 'null')
					SignalCStr_("nil element created from tag");

				break;
			}
				
			case tag_BeginSubs:
				if (newHost != nil) {
					// Can't validate newHost since it's a mixin class.
					DMElementPtr x;
					ReadElementsFromTags(inStream, newHost, x);
				}
				else 
					SignalCStr_("BeginSubs on non-container");
				break;
				
			case tag_EndSubs:
			case tag_End:
				readingTags = false;
				break;
				
			case tag_ClassAlias:
			
				// The ClassAlias tag defines the ClassID the for
				// the next object in the Stream. This allows you
				// to define an object which belongs to a subclass
				// of another class, but has the same data as that
				// other class.
					
				*inStream >> aliasClassID;
				break;
				
			default:
				SignalCStr_("Unrecognized tag");
				readingTags = false;
				break;
		}
	} while (readingTags);

}


// ---------------------------------------------------------------------------
//		* FetchClassIndex										[protected]
// ---------------------------------------------------------------------------
//	Search the class table for the entry which matches the specified class ID.

SInt16
DMReanimator::FetchClassIndex(
	ClassIDT inClassID)
{

	ValidateThis_();
	ValidateHandle_((Handle) mTableH);

	// Search class table. Stop when reaching the end or finding
	// a matching ClassID.

	SInt16 classIndex = 0;
	DM_ClassTable* tableP = *mTableH;
	for (SInt16 i = 0; i < mClassCount; i++) {
		if (tableP[i].classID == inClassID) {
			classIndex = i + 1;
			break;
		}
	}
	return classIndex;
}
