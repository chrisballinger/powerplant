// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAGrowZone.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/29/96
//     $Date: 2006/01/18 01:33:26 $
//  $History: CAGrowZone.h $
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
//	User: scouten      QDate: 11/30/96   Time: 17:34
//	Created in $/Constructor/Source files/Utility classes
//	Added class.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Support classes
#include <LGrowZone.h>


// ===========================================================================
//		* CAGrowZone
// ===========================================================================
//
//	CAGrowZone is a grow-zone object that improves on the low-memory test
//	and shows a grayscale alert when memory is low.
//
// ===========================================================================

class CAGrowZone : public LGrowZone {

public:
							CAGrowZone(
									Size				inReserveSize);
	virtual					~CAGrowZone();

	virtual Boolean			MemoryIsLow();

protected:
	virtual	void			SpendTime(
									const EventRecord&	inMacEvent);
	

	// data members

protected:
	Boolean					mMemoryWasLow;

};
