// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAGrowZone.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/29/96
//     $Date: 2006/01/18 01:33:26 $
//  $History: CAGrowZone.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 03/17/97   Time: 11:49
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Optimized MemoryIsLow.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:35
//	Created in $/Constructor/Source files/Utility classes
//	Added class.
//	
// ===========================================================================

#include "CAGrowZone.h"

	// Core : Utilities
#include "UMemoryUtils.h"


// ===========================================================================

#pragma mark *** CAGrowZone ***

// ---------------------------------------------------------------------------
//		* CAGrowZone()
// ---------------------------------------------------------------------------
//	Constructor

CAGrowZone::CAGrowZone(
	Size	inReserveSize)

: LGrowZone(inReserveSize)

{
	mMemoryWasLow = false;
}


// ---------------------------------------------------------------------------
//		* ~CAGrowZone
// ---------------------------------------------------------------------------
//	Destructor

CAGrowZone::~CAGrowZone()
{
}


// ---------------------------------------------------------------------------
//		* MemoryIsLow
// ---------------------------------------------------------------------------
//	We want to disable commands even if the grow zone reserve hasn't yet
//	been depleted.

Boolean
CAGrowZone::MemoryIsLow()
{

	// On every 10th call, we check MaxBlock to ensure that
	// a reasonably large allocation remains available.

	static int countdown = 0;
	
	if (--countdown < 0) {
		countdown = 10;

		UInt32 maxBlock = ::MaxBlock();
		if (maxBlock < 32768)
			return true;
	}

	// Otherwise, see if grow zone reserve is still available.
	
	return LGrowZone::MemoryIsLow();

}


// ---------------------------------------------------------------------------
//		* SpendTime												[protected]
// ---------------------------------------------------------------------------
//	Replaces LGrowZone::SpendTime. I wanted a different alert to appear
//	when memory is low.

void
CAGrowZone::SpendTime(
	const EventRecord&	/* inMacEvent */)
{

	// Try to reestablish the reserve.

	if ((mLocalReserve == nil) || (*mLocalReserve == nil)) {
		if (::MaxBlock() > mReserveSize + 2048) {
			if (mLocalReserve == nil)
				mLocalReserve = ::NewHandle(mReserveSize);
			else
				::ReallocateHandle(mLocalReserve, mReserveSize);
		}
	}

	// See if we need to give a warning or not.
	
	if (MemoryIsLow()) {
		if (!mMemoryWasLow) {
			StopRepeating();
			try {
				UMemoryUtils::ShowLowMemoryAlert();
			}
			catch(...) {
			}
			StartRepeating();
		}
		mMemoryWasLow = true;
	}
	else
		mMemoryWasLow = false;

}
