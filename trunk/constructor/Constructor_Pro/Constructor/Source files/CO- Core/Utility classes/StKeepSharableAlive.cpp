// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StKeepSharableAlive.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/01/96
//     $Date: 2006/01/18 01:33:27 $
//  $History: StKeepSharableAlive.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:11
//	Created in $/Constructor/Source files/Utility classes
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "StKeepSharableAlive.h"

	// PowerPlant : Feature classes
#include <LSharable.h>


// ===========================================================================
//		* Helper class
// ===========================================================================

class LSharableFriend : public LSharable {
	friend class StKeepSharableAlive;
};


// ===========================================================================

#pragma mark *** StKeepSharableAlive ***

// ---------------------------------------------------------------------------
//		* StKeepSharableAlive()
// ---------------------------------------------------------------------------
//	Constructor

StKeepSharableAlive::StKeepSharableAlive(
	LSharable*	inSharable)
{
	mSharable = inSharable;

	if (inSharable != nil) {
		ValidateObject_(inSharable);
		inSharable->AddUser(this);
	}
}


// ---------------------------------------------------------------------------
//		* ~StKeepSharableAlive
// ---------------------------------------------------------------------------
//	Destructor

StKeepSharableAlive::~StKeepSharableAlive()
{
	if (mSharable != nil) {
		ValidateObject_(mSharable);
		
		LSharableFriend* sf = (LSharableFriend*) mSharable;
		sf->mUseCount--;
	}
}
