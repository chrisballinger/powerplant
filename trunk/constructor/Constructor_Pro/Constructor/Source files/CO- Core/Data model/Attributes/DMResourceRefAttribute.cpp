// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMResourceRefAttribute.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 01/30/97
//     $Date: 2006/01/18 01:32:32 $
//  $History: DMResourceRefAttribute.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: Andrew       Date: 02/06/97   Time: 13:47
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial changes for compiling with MSVC: prefix file, ???? constant
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 01/31/97   Time: 11:55
//	Created
//	Comment: Added class.
//
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMResourceRefAttribute.h"


// ===========================================================================

#pragma mark *** DMResourceRefAttribute ***

// ---------------------------------------------------------------------------
//		* DMResourceRefAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMResourceRefAttribute::DMResourceRefAttribute()
{
	mResourceType = '\?\?\?\?';
}


// ---------------------------------------------------------------------------
//		* DMResourceRefAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[DMIntegerAttribute data]
//		UInt32:		resource type

DMResourceRefAttribute::DMResourceRefAttribute(
	LStream*	inStream)

: DMIntegerAttribute(inStream)

{
	(*inStream)	>> mResourceType;
}


// ---------------------------------------------------------------------------
//		* DMResourceRefAttribute(DMResourceRefAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMResourceRefAttribute::DMResourceRefAttribute(
	const DMResourceRefAttribute&	inOriginal)

: DMIntegerAttribute(inOriginal)

{

	// Validate pointers.

	ValidateObject_(&inOriginal);
	
	// Copy the originals resource type
	
	mResourceType = inOriginal.mResourceType;

}
