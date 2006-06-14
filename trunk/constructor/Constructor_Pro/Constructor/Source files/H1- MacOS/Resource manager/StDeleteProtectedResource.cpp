// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDeleteProtectedResource.cpp  © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/28/97
//     $Date: 2006/01/18 01:34:07 $
//  $History: StDeleteProtectedResource.cpp $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 01/28/97   Time: 15:28
//	Created in $/Constructor/Source files/H1- MacOS/Resource manager
//	Added class. (Bug fix #1214.)
//	
// ===========================================================================

#include "StDeleteProtectedResource.h"


// ===========================================================================

#pragma mark *** StDeleteProtectedResource ***

// ---------------------------------------------------------------------------
//		* StDeleteProtectedResource()
// ---------------------------------------------------------------------------
//	Constructor, specifying the resource to be deleted

StDeleteProtectedResource::StDeleteProtectedResource(
	ResType		inResType,
	ResIDT		inResID,
	Boolean		inThrowFail,
	Boolean		inGet1Resource)

: StResource(inResType, inResID, inThrowFail, inGet1Resource)

{
}


// ---------------------------------------------------------------------------
//		* ~StDeleteProtectedResource
// ---------------------------------------------------------------------------
//	Destructor, actually deletes the resource

StDeleteProtectedResource::~StDeleteProtectedResource()
{
	if (mResourceH != nil) {
		if (::GetResAttrs(mResourceH) & resProtected)
			::SetResAttrs(mResourceH, 0);
		::RemoveResource(mResourceH);
		::DisposeHandle(mResourceH);
		mResourceH = nil;
	}
}
