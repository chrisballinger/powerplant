// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMIterator.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/04/96
//	   $Date: 2006/01/18 01:32:41 $
//	$History: DMIterator.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
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

#pragma once

	// PowerPlant : Array classes
#include <LArrayIterator.h>

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMElement;
class LSharableArray;


// ===========================================================================
//		* DMIterator
// ===========================================================================
//
//	DMIterator is a subclass of LArrayIterator designed for the data model
//	classes. It was introduced to avoid the typical construct of using an
//	iterator with an LSharable* pointer, and casting that pointer to
//	DMElement*.
//
// ===========================================================================

class DMIterator : public LArrayIterator {

public:
							DMIterator(
									LSharableArray&			inArray,
									ArrayIndexT				inPosition = from_Start);
	virtual					~DMIterator();

	Boolean					NextElement();
	Boolean					PreviousElement();
	DMElement*				CurrentElement();

};


// ===========================================================================
//		* DMFastIterator
// ===========================================================================
//
//	DMFastIterator is a subclass of LFastArrayIterator. Same concept
//	as DMIterator.
//
// ===========================================================================

class DMFastIterator : public LFastArrayIterator {

public:
							DMFastIterator(
									const LSharableArray&	inArray,
									ArrayIndexT				inPosition = from_Start);
	virtual					~DMFastIterator();

	Boolean					NextElement();
	Boolean					PreviousElement();
	DMElement*				CurrentElement();

};
