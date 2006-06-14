// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RFResource.cpp				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/23/97
//     $Date: 2006/01/18 01:34:07 $
//  $History: RFResource.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/18/97   Time: 16:46
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Added code to support empty data handles.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:51
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:20
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/29/97   Time: 11:57
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Fixed a silly syntax error.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/29/97   Time: 11:44
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Added code to DuplicateResource to prevent duplicating to the same
//	resource.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/28/97   Time: 15:27
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Fixed some bugs in UpdateRF relating to protected resources. (Bug fix
//	#1214.)
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

#include "RFResource.h"

	// Core : Resource manager
#include "RMIdentifier.h"
#include "RMMap.h"
//#include "StResourceContext.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFType.h"
#include "StDeleteProtectedResource.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>


// ===========================================================================

#pragma mark *** RFResource ***

// ---------------------------------------------------------------------------
//		* RFResource(RFType&, ResIDT)							[private]
// ---------------------------------------------------------------------------
//	Constructor, specifying the resource type object and the new
//	resource ID. Usually used for new (empty) resource only. Called
//	only from RFType.

RFResource::RFResource(
	RFType&		inResType,
	ResIDT		inResID)

: RMResource(inResType),
  mResType(inResType)

{
	InitRFResource(inResID);
}


// ---------------------------------------------------------------------------
//		* RFResource(RFType&, Handle, LFile*)					[private]
// ---------------------------------------------------------------------------
//	Constructor, specifying the resource type object and the resource
//	handle. Called from RFType::ScanRF when an existing resource is
//	found. Note that the handle is empty (size = 0) since we set
//	ResLoad to false.

RFResource::RFResource(
	RFType&		inResType,
	Handle		inResourceH)

: RMResource(inResType),
  mResType(inResType)

{

	// Fetch information about this resource.

	Str255 name;
	ResIDT theID;
	ResType theType;
	::GetResInfo(inResourceH, &theID, &theType, name);

	// Do common initialization.

	InitRFResource(theID);
	
	// Save resource attributes.

	mResAttrs = ::GetResAttrs(inResourceH) & 0xFF;		// 68K Resource Mgr is very broken!

}


// ---------------------------------------------------------------------------
//		* ~RFResource
// ---------------------------------------------------------------------------
//	Destructor

RFResource::~RFResource()
{

	// Dispose cached resource data.

	if (mResData != nil) {
		ValidateHandle_(mResData);
		::DisposeHandle(mResData);
		mResData = nil;
	}
	
	// Dispose cached resource name.
	
	if (mResName != nil) {
		ValidateHandle_((Handle) mResName);
		::DisposeHandle((Handle) mResName);
		mResName = nil;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource attribute accessors

// ---------------------------------------------------------------------------
//		* IsMatchFor(RMIdentifier&, RMIdentifier&)
// ---------------------------------------------------------------------------
//	Return true if this resource matches the specified resource type and ID.

Boolean
RFResource::IsMatchFor(
	const RMIdentifier&	inResType,
	const RMIdentifier&	inResID) const
{

	// If type or ID aren't numeric, it's not a match.

	if (!inResType.IsNumericID() || !inResID.IsNumericID())
		return false;
	
	// Both are kosher, now check the values.
	
	return IsMatchFor(inResType.GetNumericID(), inResID.GetNumericID());

}


// ---------------------------------------------------------------------------
//		* IsMatchFor(ResType, ResIDT)
// ---------------------------------------------------------------------------
//	Return true if this resource matches the specified ResType and ID.

Boolean
RFResource::IsMatchFor(
	ResType		inResType,
	ResIDT		inResID) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See if it matches.

	return (inResType == GetResType() && inResID == GetResID());

}


// ---------------------------------------------------------------------------
//		* GetResTypeRM
// ---------------------------------------------------------------------------
//	Return an RMIdentifier object that contains the resource's type.

RMIdentifier*
RFResource::GetResTypeRM() const
{
	return new RMIdentifier(GetResType());
}


// ---------------------------------------------------------------------------
//		* GetResIDRM
// ---------------------------------------------------------------------------
//	Return an RMIdentifier object that contains the resource's ID.

RMIdentifier*
RFResource::GetResIDRM() const
{
	return new RMIdentifier(mResID);
}


// ---------------------------------------------------------------------------
//		* GetResType
// ---------------------------------------------------------------------------
//	Get the four-character resource type ID.

ResType
RFResource::GetResType() const
{
	return mResType.GetResType();
}


// ---------------------------------------------------------------------------
//		* GetResName
// ---------------------------------------------------------------------------
//	Return the resource name.

StringPtr
RFResource::GetResName(
	Str255	outResName) const
{

	// Validate pointers.
	
	ValidateThis_();

	// Cache the resource name if needed.

	if ((mResName == nil) && (!mResAttrsChanged))
		(const_cast<RFResource*>(this))->CacheResName();

	// Copy the cached name (if there is one.)

	if (mResName != nil)
		::BlockMoveData(*mResName, outResName, (*mResName)[0] + 1);
	else
		outResName[0] = 0;

	// Return this name.
	
	return outResName;

}


// ---------------------------------------------------------------------------
//		* GetResData
// ---------------------------------------------------------------------------
//	Returns a copy of the resource data. The caller owns the handle
//	and is responsible for disposing of it.

Handle
RFResource::GetResData()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// If the resource is deleted, there's no data.

	if (mResDeleted)
		return nil;
	
	// Get our internal copy of the resource.

	Boolean resFromFile = false;
	Handle srcHandle = mResData;

	if (srcHandle == nil) {
		StResourceContext resFile(GetResourceContext());
		StResLoad loadEmUp(true);
		if (resFile.IsValid()) {
			srcHandle = ::Get1Resource(GetResType(), GetResID());
			resFromFile = true;
		}
	}
	if (srcHandle == nil)
		return nil;
	
	// Make a safe copy of the resource so client can manage the data safely.
	
	Handle returnH = ::NewHandle(::GetHandleSize(srcHandle));
	ThrowIfMemFail_(returnH);
	::BlockMoveData(*srcHandle, *returnH, ::GetHandleSize(srcHandle));
	::HNoPurge(returnH);
	
	if (resFromFile)
		::ReleaseResource(srcHandle);

	// Return the safe copy of the resource data.

	return returnH;

}


// ---------------------------------------------------------------------------
//		* GetRFMap
// ---------------------------------------------------------------------------
//	Find the MacOS resource map object that contains this resource.

RFMap&
RFResource::GetRFMap() const
{
	return mResType.GetRFMap();
}


// ---------------------------------------------------------------------------
//		* IsModified
// ---------------------------------------------------------------------------
//	Return true if the resource has been changed since the last file save.

Boolean
RFResource::IsModified() const
{
	return (mResData != nil) || mResAttrsChanged || RMResource::IsModified();
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource info changers

// ---------------------------------------------------------------------------
//		* SetResData
// ---------------------------------------------------------------------------
//	Change the resource data. A copy of the handle inResData is made so the
//	caller remains responsible for the handle (which is unchanged).

void
RFResource::SetResData(
	Handle	inResData)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateHandle_(inResData);

	// If resource was deleted, undelete it now.

	if (mResDeleted) {
		SendRsrcMessage(Resource_Added);
		mResDeleted = false;
	}
	
	// If we already have resource data, verify that the new data actually
	// represents a change.
	
	if (mResData != nil) {
		
		// Validate old resource data handle.
		
		ValidateHandle_(mResData);
		
		// See if data is identical to what's already there. If so, skip out.
		
		if (::GetHandleSize(inResData) == ::GetHandleSize(mResData)) {
			StHandleLocker lockNew(inResData);
			StHandleLocker lockOld(mResData);
			if (::BlocksAreEqual(*inResData, *mResData, ::GetHandleSize(inResData)))	// in UMemoryMgr.cp
				return;
		}
		
		// Not identical, need to replace data.
	
		::SetHandleSize(mResData, ::GetHandleSize(inResData));
		ThrowIfMemError_();

	}
	else {
	
		// Didn't have resource data before, make a handle for new data and
		// cache it until the resource file is updated.
	
		mResData = ::NewHandle(::GetHandleSize(inResData));
		ThrowIfMemFail_(mResData);

	}

	// Copy the new data into our cached data handle.
	// Make sure it doesn't get purged on us.

	::BlockMoveData(*inResData, *mResData, ::GetHandleSize(inResData));
	::HNoPurge(mResData);
	
	// Send around a resource-data changed message.
	
	SendRsrcMessage(Resource_DataChanged);

}


// ---------------------------------------------------------------------------
//		* SetResAttrs
// ---------------------------------------------------------------------------
//	Change the resource attributes for this resource.

void
RFResource::SetResAttrs(
	SInt16	inResAttrs)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Grab the resource name. If the attributes change,
	// we need to keep the name.

	CacheResName();

	// If the resource was deleted, undelete it now.

	if (mResDeleted) {
		SendRsrcMessage(Resource_Added);
		mResDeleted = false;
	}
	
	// Record the new attributes.
	
	mResAttrs = inResAttrs & 0xFF;
	mResAttrsChanged = true;
	
	// Send around a change message.
	
	SendRsrcMessage(Resource_AttrChanged);

}


// ---------------------------------------------------------------------------
//		* SetResName
// ---------------------------------------------------------------------------
//	Public routine to change the resource name.

void
RFResource::SetResName(
	ConstStringPtr	inResName)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// If the resource was deleted, undelete it now.

	if (mResDeleted) {
		SendRsrcMessage(Resource_Added);
		mResDeleted = false;
	}
	
	// Cache a copy of the new name.
	
	SetResNameInternal(inResName);
	mResAttrsChanged = true;
	
	// Send around an attribute-changed message.
	
	SendRsrcMessage(Resource_AttrChanged);

}


// ---------------------------------------------------------------------------
//		* DuplicateResource
// ---------------------------------------------------------------------------
//	Create a new resource whose data and attributes are an exact copy of
//	this resource. The new resource will have the specified type and ID.
//	If a resource with this type and ID already exists, it will be replaced.

RFResource*
RFResource::DuplicateResource(
	ResType		inResType,
	ResIDT		inDuplicateResID)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Create the new resource.

	RFResource* newResource = GetRFMap().FindResource(inResType, inDuplicateResID, true);
	ValidateObject_(newResource);
	
	// If the duplicate resource is this resource,
	// ignore the request.
	
	if (newResource == this)
		return this;

	// Copy all relevant data to the new resource.

	Str255 name;
	GetResName(name);
	newResource->SetResName(name);
	
	SInt16 attrs = GetResAttrs();
	newResource->SetResAttrs(attrs);
	
	Handle data = GetResData();
	if (data != nil) {
		ValidateHandle_(data);
		newResource->SetResData(data);
		::DisposeHandle(data);
	}

	// Return the new resource.

	return newResource;

}


// ---------------------------------------------------------------------------
//		* DeleteResource
// ---------------------------------------------------------------------------
//	Flag this resource to be deleted at the next UpdateRF cycle. If
//	any changes have been made to the resource data (via SetResData)
//	they are discarded.

void
RFResource::DeleteResource()
{

	// Validate pointers.
	
	ValidateThis_();

	// Release our local copy of resource data if we have one.

	if (mResData != nil) {
		ValidateHandle_(mResData);
		::DisposeHandle(mResData);
		mResData = nil;
	}
	
	// Delete the resource.
	
	RMResource::DeleteResource();

}


// ===========================================================================

#pragma mark -
#pragma mark ** internal implementation details

// ---------------------------------------------------------------------------
//		* InitRFResource										[private]
// ---------------------------------------------------------------------------
//	Common initialization for constructors.

void
RFResource::InitRFResource(
	ResIDT	inResID)
{
	mResAttrsChanged = false;
	mResAttrs = 0;
	mResData = nil;
	mResName = nil;
	mResID = inResID;
}


// ---------------------------------------------------------------------------
//		* SetResNameInternal									[private]
// ---------------------------------------------------------------------------
//	Internal routine to change the resource name. The name is stored in
//	a handle of exactly the string's size to conserve memory.

void
RFResource::SetResNameInternal(
	ConstStringPtr	inResName)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See if we're making an empty string.

	if (inResName[0]) {
	
		// No, the new name is not empty. Create a handle to hold
		// the new name.
	
		if (mResName != nil) {
			::SetHandleSize((Handle) mResName, inResName[0]+1);
			ThrowIfMemError_();
		}
		else {
			mResName = (StringHandle) ::NewHandle(inResName[0]+1);
			ThrowIfNil_(mResName);
		}
		::BlockMoveData(inResName, *mResName, inResName[0]+1);
	}
	else {
	
		// Yes, the new name is empty. Dispose of the old name's handle.
	
		if (mResName != nil) {
			::DisposeHandle((Handle) mResName);
			mResName = nil;
		}
	}
}


// ---------------------------------------------------------------------------
//		* GetResourceContext									[private]
// ---------------------------------------------------------------------------
//	Return the ref num of the file from which this resource came (if any).

SInt16
RFResource::GetResourceContext()
{

	// Ask the map for the resource context.
	
	return GetRFMap().GetResourceContext();

}


// ---------------------------------------------------------------------------
//		* CacheResName											[private]
// ---------------------------------------------------------------------------
//	GetResName has been called for this resource. Grab the resource
//	name and cache it for later re-use.

void
RFResource::CacheResName()
{

	// Validate pointers.
	
	ValidateThis_();

	// If resource deleted or we've already got the name, no need to continue.

	if (mResDeleted || mResName != nil)
		return;

	// Find the resource and grab its name. To save memory, we don't store the
	// name unless somebody's actually interested in it.

	StResourceContext resFile(GetResourceContext());
	StResLoad dontLoad(false);

	if (resFile.IsValid()) {
		Handle srcHandle = ::Get1Resource(GetResType(), GetResID());
		if (srcHandle != nil) {
			Str255 name;
			ResIDT theID;
			ResType theType;
			::GetResInfo(srcHandle, &theID, &theType, name);
			SetResNameInternal(name);
		}
	}
}


// ---------------------------------------------------------------------------
//		* UpdateRF												[private]
// ---------------------------------------------------------------------------
//	Called by RMType::UpdateRF to write any changes to this resource
//	to the file. Since the Resource Manager is generally slow, we make as
//	few calls as possible.

void
RFResource::UpdateRF()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Make sure we're in the right resource fork.

	StResourceContext resFile(GetResourceContext());
	
	// If the resource was deleted, remove it from file now.
	
	if (mResDeleted) {
		StDeleteProtectedResource delRes(GetResType(), GetResID(), false, true);
		return;
	}
	
	// If the data changed or attributes changed, update the file.
	
	if ((mResData != nil) || mResAttrsChanged) {

		StNewResource newResource(GetResType(), GetResID(), 0, true);

		// Replace existing resource data with new data (if there
		// is any new data). Create a safe copy of the resource so
		// Resource Manager doesn't munge the original handle.

		if (mResData != nil) {
			
			ValidateHandle_(mResData);
			::SetHandleSize(newResource, ::GetHandleSize(mResData));
			ThrowIfMemError_();
			
			StHandleLocker copyLock(mResData);
			::BlockMoveData(*mResData, *((Handle) newResource), ::GetHandleSize(mResData));

		}
		
		::HNoPurge(newResource);
		LStr255 name;
		GetResName(name);
		newResource.SetResAttrs(mResAttrs);
		newResource.SetResName(name);
		newResource.Write(false);				// we don't need to update the resource fork every time thru
	}
}


// ---------------------------------------------------------------------------
//		* UpdateComplete										[private]
// ---------------------------------------------------------------------------
//	Called by RMType::UpdateComplete to clear modified flags for this resource.

void
RFResource::UpdateComplete()
{

	// Validate pointers.
	
	ValidateThis_();

	// Clear changed flags.

	mResAttrsChanged = false;
	
	// Remove cached copy of resource data.
	
	if (mResData != nil) {
		::DisposeHandle(mResData);
		mResData = nil;
	}
	
	// Do inherited update.
	
	RMResource::UpdateComplete();
	
}
