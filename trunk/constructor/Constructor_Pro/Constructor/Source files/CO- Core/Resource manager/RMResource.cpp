// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMResource.cpp				© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/09/95
//     $Date: 2006/01/18 01:33:25 $
//	$History: RMResource.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Improved commenting.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:46
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Finished adaptation to refactored RM/RF classes.
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

#include "RMResource.h"

	// Core : Resource manager
#include "RMMap.h"
#include "RMType.h"
//#include "StResourceContext.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>


// ===========================================================================

#pragma mark *** RMResource ***

// ---------------------------------------------------------------------------
//		* RMResource(RMType&, LFile*)							[protected]
// ---------------------------------------------------------------------------
//	Constructor, specifying the resource type object and the resource
//	handle. Called from RMType::ScanRF when an existing resource is
//	found. Note that the handle is empty (size = 0) since we set
//	ResLoad to false.

RMResource::RMResource(
	RMType&		inResType)

: mResType(inResType)

{
	mResDeleted = false;
}


// ---------------------------------------------------------------------------
//		* ~RMResource											[protected]
// ---------------------------------------------------------------------------
//	Destructor

RMResource::~RMResource()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource attribute accessors

// ---------------------------------------------------------------------------
//		* IsModified
// ---------------------------------------------------------------------------
//	Return true if the resource has been modified.

Boolean
RMResource::IsModified() const
{
	return mResDeleted;
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource map accessors

// ---------------------------------------------------------------------------
//		* GetRMMap
// ---------------------------------------------------------------------------
//	Return the resource map that this resource belongs to.

RMMap&
RMResource::GetRMMap() const
{
	return mResType.GetRMMap();
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource info changers

// ---------------------------------------------------------------------------
//		* DeleteResource
// ---------------------------------------------------------------------------
//	Flag this resource to be deleted at the next UpdateRF cycle. If
//	any changes have been made to the resource data (via SetResData)
//	they are discarded.

void
RMResource::DeleteResource()
{

	// Validate pointers.
	
	ValidateThis_();

	// Remember that we've been deleted.

	mResDeleted = true;
	
	// Broadcast a message saying the resource has been deleted.
	
	SendRsrcMessage(Resource_Removed);

}


// ===========================================================================

#pragma mark -
#pragma mark ** internal implementation details

// ---------------------------------------------------------------------------
//		* SendRsrcMessage										[protected]
// ---------------------------------------------------------------------------
//	Send a message about this resource to the RMMap's listeners.

void
RMResource::SendRsrcMessage(
	MessageT	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Create a resource-changed message and send it around.

	RMMap_ChangeMessage msg = { &(GetRMMap()), &(GetRMType()), this };
	GetRMMap().BroadcastMessage(inMessage, &msg);

}


// ---------------------------------------------------------------------------
//		* UpdateRF												[protected]
// ---------------------------------------------------------------------------
//	Called by RMType::UpdateRF to write any changes to this resource
//	to the file. Since the Resource Manager is generally slow, we make as
//	few calls as possible.

void
RMResource::UpdateRF()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* UpdateComplete										[protected]
// ---------------------------------------------------------------------------
//	Called by RMType::UpdateComplete to clear modified flags for this resource.

void
RMResource::UpdateComplete()
{

	// Validate pointers.

	ValidateThis_();
	
	// If the resource was deleted, inform the res type.

	if (mResDeleted) {
		mResType.ResourceDeleted(this);
		return;
	}
}
