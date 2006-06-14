// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMResListAttribute.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/12/96
//     $Date: 2006/01/18 01:32:32 $
//  $History: DMResListAttribute.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/13/96   Time: 13:51
//	Created in $/Constructor/Source files/Data model/Attributes
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMResListAttribute.h"


// ===========================================================================

#pragma mark *** DMResListAttribute ***

// ---------------------------------------------------------------------------
//		* DMResListAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMResListAttribute::DMResListAttribute()
{
	mResType = 0;
}


// ---------------------------------------------------------------------------
//		* DMResListAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[DMLabelledIntegerAttribute data]
//		ResType:		Resource type for popup menu

DMResListAttribute::DMResListAttribute(
	LStream*	inStream)

: DMLabelledIntegerAttribute(inStream)

{

	// Read stream data.

	(*inStream) >> mResType;

	// Do sanity check. Resource ID attributes should
	// be 16 bit signed integers.

	SignalIf_(mValueSize != 16);
	SignalIf_(!mSigned);
	
}


// ---------------------------------------------------------------------------
//		* DMResListAttribute(DMResListAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMResListAttribute::DMResListAttribute(
	const DMResListAttribute&	inOriginal)

: DMLabelledIntegerAttribute(inOriginal)

{
	mResType = inOriginal.mResType;
}


// ---------------------------------------------------------------------------
//		* ~DMResListAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMResListAttribute::~DMResListAttribute()
{
}
