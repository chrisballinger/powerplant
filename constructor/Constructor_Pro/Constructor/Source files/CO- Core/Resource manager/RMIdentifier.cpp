// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMIdentifier.cpp			   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/02/97
//     $Date: 2006/01/18 01:33:24 $
//  $History: RMIdentifier.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/06/97    Time: 4:03p
//	Updated in $/ConstructorWin/Sources
//	Initial merge for MSVC build
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/03/97   Time: 10:05
//	Created in $/Constructor/Source files/CO- Core/Resource manager
//	Added class.
//	
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RMIdentifier.h"


// ===========================================================================

#pragma mark *** RMIdentifier ***

// ---------------------------------------------------------------------------
//		* RMIdentifier(Int32)
// ---------------------------------------------------------------------------
//	Constructor, specifying ordinal value

RMIdentifier::RMIdentifier(
	SInt32	inOrdinal)
{
	mOrdinal = inOrdinal;
}


// ---------------------------------------------------------------------------
//		* RMIdentifier(RMIdentifier&)
// ---------------------------------------------------------------------------
//	Copy constructor

RMIdentifier::RMIdentifier(
	const RMIdentifier&	 inOriginal)
{
	mOrdinal = inOriginal.mOrdinal;
}


// ---------------------------------------------------------------------------
//		* ~RMIdentifier
// ---------------------------------------------------------------------------
//	Destructor

RMIdentifier::~RMIdentifier()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetStringID
// ---------------------------------------------------------------------------
//	Return the string ID or empty string if it uses a numeric ID.
//	Note that it returns the ANSI version of the string ID (it is
//	stored in UNICODE).

void
RMIdentifier::GetStringID(
	LStr255&	outStringID) const
{
	outStringID[(UInt8)0] = 0;
}



// ---------------------------------------------------------------------------
//		* IsNumericID
// ---------------------------------------------------------------------------
//	Return true if the ID is expressed as a number. Default returns true
//	since there is no support in the base class for storing strings.

Boolean
RMIdentifier::IsNumericID() const
{
	return true;
}


// ===========================================================================

#pragma mark -
#pragma mark ** comparisons

// ---------------------------------------------------------------------------
//		* IsMatchFor(Int32)
// ---------------------------------------------------------------------------
//	Return true if this ID matches the ordinal value passed in.

Boolean
RMIdentifier::IsMatchFor(
	SInt32	inNumericID) const
{
	return inNumericID == mOrdinal;
}


// ---------------------------------------------------------------------------
//		* IsMatchFor(LStr255&)
// ---------------------------------------------------------------------------
//	Return true if the ID matches the ANSI string. The comparison will
//	be case insensitive.

Boolean
RMIdentifier::IsMatchFor(
	const LStr255&	/* inStringID */) const
{
	return false;
}


// ---------------------------------------------------------------------------
//		* IsMatchFor(RMIdentifier&)
// ---------------------------------------------------------------------------
//	Return true if this ID matches the other ID object.

Boolean
RMIdentifier::IsMatchFor(
	const RMIdentifier&	inIdentifier) const
{
	return IsMatchFor(inIdentifier.mOrdinal);
}
