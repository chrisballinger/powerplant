// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMLengthCheckException.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/02/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMLengthCheckException.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:30
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Improved commenting.
//
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:00
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:21
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Exceptions'
//	Comment: Fixed CR/LF problem
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

#include "DMLengthCheckException.h"


// ===========================================================================

#pragma mark *** DMLengthCheckException ***

// ---------------------------------------------------------------------------
//		* DMLengthCheckException(...)
// ---------------------------------------------------------------------------
//	Constructor

DMLengthCheckException::DMLengthCheckException(
	DMAttribute*	inAttribute,
	SInt32			inMaximumLength,
	SInt32			inAttemptedLength)
{
	ValidateObject_(inAttribute);
	mAttribute = inAttribute;
	mMaximumLength = inMaximumLength;
	mAttemptedLength = inAttemptedLength;
}


// ---------------------------------------------------------------------------
//		* ~DMLengthCheckException
// ---------------------------------------------------------------------------
//	Destructor

DMLengthCheckException::~DMLengthCheckException()
{
}


// ---------------------------------------------------------------------------
//		* GetErrorString
// ---------------------------------------------------------------------------
//	Make an error string describing the disallowed change.

void
DMLengthCheckException::GetErrorString(
	LStr255& outDescriptor) const 
{
	LStr255 attrTitle;

	if (mAttribute != nil) {
		ValidateObject_(mAttribute.GetObject());
		mAttribute->GetTitle(attrTitle);
	}

	outDescriptor =  (StringPtr) "\046The text you entered is too long for \"";			//! TEMPORARY: should be in STR#
	outDescriptor += (StringPtr) attrTitle;
	outDescriptor += (StringPtr) "\041.\" Values must be no longer than ";
	outDescriptor += (StringPtr) LStr255(mMaximumLength);
	outDescriptor += (StringPtr) "\014 characters.";
}
