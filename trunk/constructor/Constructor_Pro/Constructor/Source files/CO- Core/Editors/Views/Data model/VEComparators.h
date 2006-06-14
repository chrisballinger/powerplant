// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEComparators.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/10/96
//     $Date: 2006/01/18 01:33:11 $
//  $History: VEComparators.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:10
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Changed both classes so they derive from DMComparator instead of
//	LComparator.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:53
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added file. Moved classes from VELayoutView.cpp and .h.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMComparator.h"


// ===========================================================================
//		* VEHorizComparator
// ===========================================================================
//	VEHorizComparator compares two objects by their horizontal origin.

class VEHorizComparator : public DMComparator {

public:
							VEHorizComparator() {}
	virtual					~VEHorizComparator() {}

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const;

};


// ===========================================================================
//		* VEVertComparator
// ===========================================================================
//	VEVertComparator compares two objects by their vertical origin.

class VEVertComparator : public DMComparator {

public:
							VEVertComparator() {}
	virtual					~VEVertComparator() {}

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const;

};
