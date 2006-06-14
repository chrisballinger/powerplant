// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMMinMaxIntegerAttribute.cpp
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/12/96
//     $Date: 2006/01/18 01:32:32 $
//  $History: DMMinMaxIntegerAttribute.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/07/97   Time: 14:10
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Rolled in Clint's fixes 02/07/97.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/13/96   Time: 13:48
//	Created in $/Constructor/Source files/Data model/Attributes
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMMinMaxIntegerAttribute.h"


// ===========================================================================

#pragma mark *** DMMinMaxIntegerAttribute ***

// ---------------------------------------------------------------------------
//		* DMMinMaxIntegerAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMMinMaxIntegerAttribute::DMMinMaxIntegerAttribute()
{
	mMinimumValue = DMIntegerAttribute::GetMinValue();
	mMaximumValue = DMIntegerAttribute::GetMaxValue();
}


// ---------------------------------------------------------------------------
//		* DMMinMaxIntegerAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[DMIntegerAttribute data]
//		Int32:			minimum value
//		Int32:			maximum value

DMMinMaxIntegerAttribute::DMMinMaxIntegerAttribute(
	LStream*	inStream)

: DMIntegerAttribute(inStream)

{

	// Read stream data.

	(*inStream)	>> mMinimumValue >> mMaximumValue;
	
	// Do sanity checks.
	
	SignalIf_(mMinimumValue < DMIntegerAttribute::GetMinValue());
	SignalIf_(mMaximumValue > DMIntegerAttribute::GetMaxValue());

}


// ---------------------------------------------------------------------------
//		* DMMinMaxIntegerAttribute(DMMinMaxIntegerAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMMinMaxIntegerAttribute::DMMinMaxIntegerAttribute(
	const DMMinMaxIntegerAttribute&		inOriginal)

: DMIntegerAttribute(inOriginal)

{
	mMaximumValue = inOriginal.mMaximumValue;
	mMinimumValue = inOriginal.mMinimumValue;
}


// ---------------------------------------------------------------------------
//		* ~DMMinMaxIntegerAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMMinMaxIntegerAttribute::~DMMinMaxIntegerAttribute()
{
}


// ---------------------------------------------------------------------------
//		* GetMaxValue
// ---------------------------------------------------------------------------
//	Return the maximum value for this attribute, as specified in the
//	data model specification.

SInt32
DMMinMaxIntegerAttribute::GetMaxValue() const
{
	return mMaximumValue;
}


// ---------------------------------------------------------------------------
//		* GetMinValue
// ---------------------------------------------------------------------------
//	Return the minimum value for this attribute, as specified in the
//	data model specification.

SInt32
DMMinMaxIntegerAttribute::GetMinValue() const
{
	return mMinimumValue;
}
