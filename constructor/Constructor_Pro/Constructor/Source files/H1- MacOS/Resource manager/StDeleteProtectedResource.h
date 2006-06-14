// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDeleteProtectedResource.h	   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/28/97
//     $Date: 2006/01/18 01:34:07 $
//  $History: StDeleteProtectedResource.h $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 01/28/97   Time: 15:28
//	Created in $/Constructor/Source files/H1- MacOS/Resource manager
//	Added class. (Bug fix #1214.)
//	
// ===========================================================================

#pragma once

	// PowerPlant : Utility classes
#include <UMemoryMgr.h>


// ===========================================================================
//		* StDeleteProtectedResource
// ===========================================================================
//
//	StDeleteProtectedResource is a replacement for the StDeleteResource
//	class that correctly handles deleting protected resources. It can be
//	used with non-protected resources as well.
//
// ===========================================================================

class StDeleteProtectedResource : public StResource {

public:
							StDeleteProtectedResource(
									ResType		inResType,
									ResIDT		inResID,
									Boolean		inThrowFail,
									Boolean		inGet1Resource);
	virtual					~StDeleteProtectedResource();

};
