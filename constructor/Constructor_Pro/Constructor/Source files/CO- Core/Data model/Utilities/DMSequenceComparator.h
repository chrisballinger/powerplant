// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMSequenceComparator.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/22/96
//	   $Date: 2006/01/18 01:32:41 $
//	$History: DMSequenceComparator.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 01/24/97   Time: 17:23
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 19:03
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Fixed header comment.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 19:01
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Inlined (empty) constructor and destructor.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/23/96   Time: 19:00
//	Created
//	Comment: Added new class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMComparator.h"


// ===========================================================================
//		* DMSequenceComparator
// ===========================================================================
//
//	DMSequenceComparator compares two data elements based on their position
//	in the data model hierarchy. The following rules are used to establish
//	the relative position:
//
//		1.	If the two elements share the same superelement, they are
//			compared by insertion position in the container's list.
//
//		2.	If one element is an ancestor of the other, the ancestor
//			is considered to be "first."
//
//		3.	If the two elements are cousins, the algorithm searches up
//			until two ancestors of the elements are found which share
//			the same superelement. Then rule (1) is applied.
//
// ===========================================================================

class DMSequenceComparator : public DMComparator {

public:
							DMSequenceComparator() {}
	virtual					~DMSequenceComparator() {}

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const;

protected:
	virtual SInt32			GetNestingDepth(
									const DMElement*	inElement) const;
	virtual const DMElement* GetNthAncestor(
									const DMElement*	inElement,
									SInt32				inAncestorNumber) const;

};
