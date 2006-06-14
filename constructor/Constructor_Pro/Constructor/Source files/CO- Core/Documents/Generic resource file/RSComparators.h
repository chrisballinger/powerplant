// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSComparators.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:32:58 $
//  $History: RSComparators.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:27
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:48
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMComparator.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RSType;


// ===========================================================================
//		* RSResTypeComparator
// ===========================================================================
//
//	RSResTypeComparator is used by the project window display to
//	sort resource type items by their ResType.
//
// ===========================================================================

class RSResTypeComparator : public DMComparator {

public:
							RSResTypeComparator() {}
	virtual					~RSResTypeComparator() {}

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const;
	virtual SInt32			GetSortSequence(
									const RSType*		inRSType) const;
};


// ===========================================================================
//		* RSResIDComparator
// ===========================================================================
//
//	RSResIDComparator is used by the project window display to sort
//	individual resource items by their resource ID.
//
// ===========================================================================

class RSResIDComparator : public DMComparator {

public:
							RSResIDComparator() {}
	virtual					~RSResIDComparator() {}

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const;

};


// ===========================================================================
//		* RSResNameComparator
// ===========================================================================
//
//	PJResNameComparator is used by the project window display to sort
//	individual resource items by their resource name.
//
// ===========================================================================

class RSResNameComparator : public DMComparator {

public:
							RSResNameComparator() {}
	virtual					~RSResNameComparator() {}

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const;

};
