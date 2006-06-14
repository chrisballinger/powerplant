// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEComparators.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/10/96
//     $Date: 2006/01/18 01:33:11 $
//  $History: VEComparators.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:07p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEComparators.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"

	// Core : Editors : Views : User interface
#include "VEDrawingAgent.h"


// ===========================================================================

#pragma mark *** VEHorizComparator ***

// ---------------------------------------------------------------------------
//		* CompareElements
// ---------------------------------------------------------------------------
//	Compare horizontal origins of the two objects.

SInt32
VEHorizComparator::CompareElements(
	const DMElement*	inElement1,
	const DMElement*	inElement2) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement1);
	ValidateObject_(inElement2);
	
	// Cast to view editor objects.
	
	const VEModelObject* obj1 = dynamic_const_cast(inElement1, DMElement*, VEModelObject*);
	const VEModelObject* obj2 = dynamic_const_cast(inElement2, DMElement*, VEModelObject*);
	
	ValidateObject_(obj1);
	ValidateObject_(obj2);
	
	// Find drawing agents.

	VEDrawingAgent* agent1 = obj1->FindDrawingAgent();
	VEDrawingAgent* agent2 = obj2->FindDrawingAgent();
	
	if ((agent1 == nil) || (agent2 == nil))
		return 0;
	
	ValidateObject_(agent1);
	ValidateObject_(agent2);

	// Get frame locations of two objects.

	Rect frame1;
	Rect frame2;
	
	if (!agent1->CalcPortFrameRect(frame1))
		return 0;
	if (!agent2->CalcPortFrameRect(frame2))
		return 0;

	// Compare left origins.
	
	return frame1.left - frame2.left;

}


// ===========================================================================

#pragma mark -
#pragma mark *** VEVertComparator ***

// ---------------------------------------------------------------------------
//		* CompareElements
// ---------------------------------------------------------------------------
//	Compare horizontal origins of the two objects.

SInt32
VEVertComparator::CompareElements(
	const DMElement*	inElement1,
	const DMElement*	inElement2) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement1);
	ValidateObject_(inElement2);
	
	// Cast to view editor objects.
	
	const VEModelObject* obj1 = dynamic_const_cast(inElement1, DMElement*, VEModelObject*);
	const VEModelObject* obj2 = dynamic_const_cast(inElement2, DMElement*, VEModelObject*);
	
	ValidateObject_(obj1);
	ValidateObject_(obj2);
	
	// Find drawing agents.

	VEDrawingAgent* agent1 = obj1->FindDrawingAgent();
	VEDrawingAgent* agent2 = obj2->FindDrawingAgent();
	
	if ((agent1 == nil) || (agent2 == nil))
		return 0;
	
	ValidateObject_(agent1);
	ValidateObject_(agent2);

	// Get frame locations of two objects.

	Rect frame1;
	Rect frame2;
	
	if (!agent1->CalcPortFrameRect(frame1))
		return 0;
	if (!agent2->CalcPortFrameRect(frame2))
		return 0;

	// Compare top origins.
	
	return frame1.top - frame2.top;

}
