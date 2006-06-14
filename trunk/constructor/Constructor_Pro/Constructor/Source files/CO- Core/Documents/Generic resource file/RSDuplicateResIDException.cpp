// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSDuplicateResIDException.cpp
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:32:59 $
//  $History: RSDuplicateResIDException.cpp $
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
//	User: scouten      QDate: 11/05/96   Time: 14:53
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSDuplicateResIDException.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_DuplicateResID		= 15450;
const SInt16	str_ErrorString			= 1;


// ===========================================================================

#pragma mark *** RSDuplicateResIDException ***

// ---------------------------------------------------------------------------
//		* GetErrorString
// ---------------------------------------------------------------------------

void
RSDuplicateResIDException::GetErrorString(
	LStr255&	outDescriptor) const 
{
	outDescriptor = LStr255(STR_DuplicateResID, str_ErrorString);
}
