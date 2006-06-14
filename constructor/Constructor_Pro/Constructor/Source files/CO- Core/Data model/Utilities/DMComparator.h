// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMComparator.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/01/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMComparator.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:22
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:31
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Arrays
#include <LComparator.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMElement;


// ===========================================================================
//		* DMComparator											[abstract]
// ===========================================================================
//
//	DMComparator is a common base class for comparators (see LArray)
//	that compare data model elements. It provides the common code for
//	casting from void* to DMElement*.
//
// ===========================================================================

class DMComparator : public LComparator {

public:
							DMComparator();
	virtual					~DMComparator();

	virtual SInt32			Compare(
									const void*			inItemOne,
									const void* 		inItemTwo,
									UInt32				inSizeOne,
									UInt32				inSizeTwo) const;
	virtual Boolean			IsEqualTo(
									const void*			inItemOne,
									const void* 		inItemTwo,
									UInt32				inSizeOne,
									UInt32				inSizeTwo) const;

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const = 0;

};
