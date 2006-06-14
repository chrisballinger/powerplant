// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMIterator.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/04/96
//	   $Date: 2006/01/18 01:32:40 $
//	$History: DMIterator.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:00
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:23
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMIterator.h"

	// Core : Data model : Core objects
#include "DMElement.h"

	// PowerPlant : CA : Array classes
#include <LSharableArray.h>


// ===========================================================================

#pragma mark *** DMIterator ***

// ---------------------------------------------------------------------------
//		* DMIterator()
// ---------------------------------------------------------------------------
//	Default constructor

DMIterator::DMIterator(
	LSharableArray&		inArray,
	ArrayIndexT			inPosition)

: LArrayIterator(inArray, inPosition)

{
}


// ---------------------------------------------------------------------------
//		* ~DMIterator
// ---------------------------------------------------------------------------
//	Destructor

DMIterator::~DMIterator()
{
}


// ---------------------------------------------------------------------------
//		* NextElement
// ---------------------------------------------------------------------------
//	Advance the iterator by one element. Return true if not at end of list.

Boolean
DMIterator::NextElement()
{
	void* obj;
	return Next(&obj);
}


// ---------------------------------------------------------------------------
//		* PreviousElement
// ---------------------------------------------------------------------------
//	Reverse the iterator by one element. Return true if not at start of list.

Boolean
DMIterator::PreviousElement()
{
	void* obj;
	return Previous(&obj);
}


// ---------------------------------------------------------------------------
//		* CurrentElement
// ---------------------------------------------------------------------------
//	Return the current element.

DMElement*
DMIterator::CurrentElement()
{
	LSharable* item;
	if (Current(&item)) {
		ValidateObject_(item);
		return dynamic_cast<DMElement*>(item);
	}
	else
		return nil;
}


// ===========================================================================

#pragma mark *** DMFastIterator ***

// ---------------------------------------------------------------------------
//		* DMFastIterator()
// ---------------------------------------------------------------------------
//	Default constructor

DMFastIterator::DMFastIterator(
	const LSharableArray&	inArray,
	ArrayIndexT				inPosition)

: LFastArrayIterator(inArray, inPosition)

{
}


// ---------------------------------------------------------------------------
//		* ~DMFastIterator
// ---------------------------------------------------------------------------
//	Destructor

DMFastIterator::~DMFastIterator()
{
}


// ---------------------------------------------------------------------------
//		* NextElement
// ---------------------------------------------------------------------------
//	Advance the iterator by one element. Return true if not at end of list.

Boolean
DMFastIterator::NextElement()
{
	void* obj;
	return Next(&obj);
}


// ---------------------------------------------------------------------------
//		* PreviousElement
// ---------------------------------------------------------------------------
//	Reverse the iterator by one element. Return true if not at start of list.

Boolean
DMFastIterator::PreviousElement()
{
	void* obj;
	return Previous(&obj);
}


// ---------------------------------------------------------------------------
//		* CurrentElement
// ---------------------------------------------------------------------------
//	Return the current element.

DMElement*
DMFastIterator::CurrentElement()
{
	LSharable* item;
	if (Current(&item)) {
		ValidateObject_(item);
		return dynamic_cast<DMElement*>(item);
	}
	else
		return nil;
}
