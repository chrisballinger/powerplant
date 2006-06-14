// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMRangeCheckException.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/29/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMRangeCheckException.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:31
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/05/96   Time: 12:39
//	Updated in $/Constructor/Source files/Data model/Exceptions
//	Moved error strings to STR# resource.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMRangeCheckException.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_RangeCheckException		= 22900;
const SInt16	str_ValueYouEntered			= 1;			// "The value you entered, "
const SInt16	str_NotAcceptable			= 2;			// " is not an acceptable value for Ò"
const SInt16	str_ValuesMustBe			= 3;			// .Ó Values must be in the range "
const SInt16	str_To						= 4;			// " to "
const SInt16	str_Period					= 5;			// "."


// ===========================================================================

#pragma mark *** DMRangeCheckException ***

// ---------------------------------------------------------------------------
//		* DMRangeCheckException(...)
// ---------------------------------------------------------------------------
//	Constructor

DMRangeCheckException::DMRangeCheckException(
	DMAttribute*	inAttribute,
	SInt32			inMinimumValue,
	SInt32			inMaximumValue,
	SInt32			inAttemptedValue)
{
	
	// Validate pointers.

	ValidateObject_(inAttribute);
	
	// Record exception description.
	
	mAttribute = inAttribute;
	mMinimumValue = inMinimumValue;
	mMaximumValue = inMaximumValue;
	mAttemptedValue = inAttemptedValue;

}


// ---------------------------------------------------------------------------
//		* ~DMRangeCheckException
// ---------------------------------------------------------------------------
//	Destructor

DMRangeCheckException::~DMRangeCheckException()
{
}


// ---------------------------------------------------------------------------
//		* GetErrorString
// ---------------------------------------------------------------------------
//	Make an error string describing the disallowed change.

void
DMRangeCheckException::GetErrorString(
	LStr255&	outDescriptor) const 
{

	// Get attribute's title.

	LStr255 attrTitle;
	if (mAttribute != nil) {
		ValidateObject_(mAttribute.GetObject());
		mAttribute->GetTitle(attrTitle);
	}

	// Build description string.

	outDescriptor =
			LStr255(STR_RangeCheckException, str_ValueYouEntered) +	// "The value you entered, "
			LStr255(mAttemptedValue) +
			LStr255(STR_RangeCheckException, str_NotAcceptable) +	// " is not an acceptable value for Ò"
			attrTitle +
			LStr255(STR_RangeCheckException, str_ValuesMustBe) +	// .Ó Values must be in the range "
			LStr255(mMinimumValue) +
			LStr255(STR_RangeCheckException, str_To) +				// " to "
			LStr255(mMaximumValue) +
			LStr255(STR_RangeCheckException, str_Period);			// "."
}
