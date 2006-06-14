// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMSequenceComparator.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/22/96
//	   $Date: 2006/01/18 01:32:41 $
//	$History: DMSequenceComparator.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:23
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/23/96   Time: 19:03
//	Created
//	Comment: Added new class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMSequenceComparator.h"

	// Core : Data model : Core objects
#include "DMElement.h"
#include "DMContainerMixin.h"


// ===========================================================================

#pragma mark *** DMSequenceComparator ***

// ---------------------------------------------------------------------------
//		* CompareElements
// ---------------------------------------------------------------------------
//	Compare the two elements based on their position in the data model
//	hierarchy. The rules for comparison are described in the header file.

SInt32
DMSequenceComparator::CompareElements(
	const DMElement*	inElement1,
	const DMElement*	inElement2) const 
{

	// Validate input parameters.
	
	ValidateThis_();
	ValidateObject_(inElement1);
	ValidateObject_(inElement2);
	
	// If one object is the superelement of the other, it's first.
	
	if (inElement1->IsSubElementOf(inElement2))
		return -1;
	if (inElement2->IsSubElementOf(inElement1))
		return 1;
	
	// Search up the inheritance tree until we find parents of these
	// elements which share the same superelement.

	SInt32 depth1 = GetNestingDepth(inElement1);
	SInt32 depth2 = GetNestingDepth(inElement2);

	SInt32 commonDepth = depth1;
	if (depth2 < commonDepth)
		commonDepth = depth2;
	
	const DMElement* ancestor1 = GetNthAncestor(inElement1, depth1 - commonDepth);
	const DMElement* ancestor2 = GetNthAncestor(inElement2, depth2 - commonDepth);

	ValidateObject_(ancestor1);
	ValidateObject_(ancestor2);

	// We now have ancestors of the two elements that are at the same
	// depth in the data model tree. Now continue searching until
	// we find the ancestors that share a common super element.
	
	while (commonDepth-- > 0) {
		
		const DMElement* super1 = ancestor1->GetSuperElement();
		const DMElement* super2 = ancestor2->GetSuperElement();
		ValidateObject_(super1);
		ValidateObject_(super2);

		if (super1 == super2) {
		
			// Okay, they have the same superelement. Compare indexes of the two elements.
			
			const DMContainerMixin* container = dynamic_const_cast(super1, DMElement*, DMContainerMixin*);
			ValidateObject_(container);
			
			ArrayIndexT index1 = container->GetSubElements().FetchIndexOf(&ancestor1);
			ArrayIndexT index2 = container->GetSubElements().FetchIndexOf(&ancestor2);
			
			return index1 - index2;
		
		}
		
		// Not same superelement. Proceed up one level in the inheritance tree and try again.
		
		ancestor1 = super1;
		ancestor2 = super2;
	
	}

	// No common superelement. There's no way to compare these two.

	SignalCStr_("Can't compare elements with no common root.");
	return 0;

}

// ---------------------------------------------------------------------------
//		* GetNestingDepth										[protected]
// ---------------------------------------------------------------------------
//	Find the number of links from the named element to the top of the
//	data model hierarchy.

SInt32
DMSequenceComparator::GetNestingDepth(
	const DMElement*	inElement) const
{
	
	ValidateObject_(inElement);
	
	SInt32 depth = 0;
	const DMElement* super = inElement;
	
	while (super != nil) {
		depth++;
		ValidateObject_(super);
		super = super->GetSuperElement();
	}

	return depth;
}


// ---------------------------------------------------------------------------
//		* GetNthAncestor										[protected]
// ---------------------------------------------------------------------------
//	Find the nth ancestor of the named element (i.e. call GetSuperElement
//	n times).

const DMElement*
DMSequenceComparator::GetNthAncestor(
	const DMElement*	inElement,
	SInt32				inAncestorNumber) const
{
	
	ValidateObject_(inElement);

	SInt32 n = inAncestorNumber;
	const DMElement* super = inElement;
	
	while (n--) {
		super = super->GetSuperElement();
		ValidateObject_(super);
	}

	return super;
}
