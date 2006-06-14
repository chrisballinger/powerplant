// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMComparator.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/01/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMComparator.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:00
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMComparator.h"

	// Core : Data model : Core classes
#include "DMElement.h"


// ===========================================================================

#pragma mark *** DMComparator ***

// ---------------------------------------------------------------------------
//		* DMComparator()
// ---------------------------------------------------------------------------
//	Constructor

DMComparator::DMComparator()
{
}


// ---------------------------------------------------------------------------
//		* ~DMComparator
// ---------------------------------------------------------------------------
//	Destructor

DMComparator::~DMComparator()
{
}


// ---------------------------------------------------------------------------
//		* Compare
// ---------------------------------------------------------------------------
//	Overriden to deference the two pointers as LSharable*, convert them
//	to DMElement* and call the pure virtual function CompareElements.

SInt32
DMComparator::Compare(
	const void*		inItemOne,
	const void* 	inItemTwo,
	UInt32			/* inSizeOne */,
	UInt32			/* inSizeTwo */) const
{
	LSharable* item1Sh = *((LSharable**) inItemOne);
	LSharable* item2Sh = *((LSharable**) inItemTwo);

	ValidateObject_(item1Sh);
	ValidateObject_(item2Sh);

	DMElement* item1DM = (dynamic_cast<DMElement*>(item1Sh));
	DMElement* item2DM = (dynamic_cast<DMElement*>(item2Sh));

	ValidateObject_(item1DM);
	ValidateObject_(item2DM);

	return CompareElements(item1DM, item2DM);
}


// ---------------------------------------------------------------------------
//		* IsEqualTo
// ---------------------------------------------------------------------------
//	Overriden to do a simple pointer conversion.

Boolean
DMComparator::IsEqualTo(
	const void*		inItemOne,
	const void* 	inItemTwo,
	UInt32			/* inSizeOne */,
	UInt32			/* inSizeTwo */) const
{
	LSharable* item1Sh = *((LSharable**) inItemOne);
	LSharable* item2Sh = *((LSharable**) inItemTwo);

	ValidateObject_(item1Sh);
	ValidateObject_(item2Sh);

	return item1Sh == item2Sh;
}
