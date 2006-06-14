// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSComparators.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:32:58 $
//  $History: RSComparators.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSComparators.h"

	// Core : Documents : Generic resource file
#include "RSResource.h"
#include "RSType.h"

	// Core : Editors : Generic resource editor
#include "RETypeEntry.h"


// ===========================================================================

#pragma mark *** RSResTypeComparator ***

// ---------------------------------------------------------------------------
//		* CompareElements
// ---------------------------------------------------------------------------
//	Make sure that both elements are RSType. If so, compare them by the
//	sort sequence of the resource type.

SInt32
RSResTypeComparator::CompareElements(
	const DMElement*	inElement1,
	const DMElement*	inElement2) const 
{
	
	const RSType* res1 = dynamic_const_cast( inElement1, DMElement*, RSType* );
	const RSType* res2 = dynamic_const_cast( inElement2, DMElement*, RSType* );
	
	if ((res1 == nil) || (res2 == nil))
		return 0;
	else
		return GetSortSequence(res1) - GetSortSequence(res2);
}


// ---------------------------------------------------------------------------
//		* GetSortSequence
// ---------------------------------------------------------------------------
//	Called only by CompareElements. Get the sort sequence for the
//	named resource type entry.

SInt32
RSResTypeComparator::GetSortSequence(
	const RSType*	inRSType) const
{

	// Validate input parameters.

	ValidateObject_(inRSType);
	
	// Find the resource type entry for this type.
	
	RETypeEntry* typeEntry = inRSType->GetTypeEntry();
	ValidateObject_(typeEntry);
	return typeEntry->GetSortSequence();

}

// ===========================================================================

#pragma mark -
#pragma mark *** RSResIDComparator ***

// ---------------------------------------------------------------------------
//		* CompareElements
// ---------------------------------------------------------------------------
//	Make sure that both elements are RSResource. If so, compare them by the
//	resource ID.

SInt32
RSResIDComparator::CompareElements(
	const DMElement*	inElement1,
	const DMElement*	inElement2) const 
{
	const RSResource* res1 = dynamic_const_cast(inElement1, DMElement*, RSResource*);
	const RSResource* res2 = dynamic_const_cast(inElement2, DMElement*, RSResource*);
	
	if ((res1 == nil) || (res2 == nil))
		return 0;
	else
		return res1->GetResID() - res2->GetResID();
}


// ===========================================================================

#pragma mark -
#pragma mark *** RSResNameComparator ***

// ---------------------------------------------------------------------------
//		* CompareElements
// ---------------------------------------------------------------------------
//	Make sure that both elements are RSResource. If so, compare them by the
//	sort sequence of the resource name.

SInt32
RSResNameComparator::CompareElements(
	const DMElement*	inElement1,
	const DMElement*	inElement2) const 
{
	const RSResource* res1 = dynamic_const_cast(inElement1, DMElement*, RSResource*);
	const RSResource* res2 = dynamic_const_cast(inElement2, DMElement*, RSResource*);
	
	if ((res1 == nil) || (res2 == nil))
		return 0;
	else {
		LStr255 name1;
		LStr255 name2;
		name1.SetCompareFunc(LString::CompareIgnoringCase);
		
		res1->GetResName(name1);
		res2->GetResName(name2);

		return name1.CompareTo(name2);
	}
}
