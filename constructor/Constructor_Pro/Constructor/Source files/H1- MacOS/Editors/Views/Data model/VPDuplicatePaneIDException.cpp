// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPDuplicatePaneIDException.cpp
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/06/96
//     $Date: 2006/01/18 01:33:54 $
//  $History: VPDuplicatePaneIDException.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/06/96   Time: 15:08
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added  class.
//	
// ===========================================================================

#include "VPDuplicatePaneIDException.h"

	// Core : Utility classes
#include "UTextID.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_DuplicateIDException	= 3100;
const SInt16	str_PaneIDEntered			= 1;		// "The pane ID you entered, "
const SInt16	str_CannotBeUsed			= 2;		// ", cannot be used because ..."


// ===========================================================================

#pragma mark *** VPDuplicatePaneIDException ***

// ---------------------------------------------------------------------------
//		* VPDuplicatePaneIDException(PaneIDT)
// ---------------------------------------------------------------------------
//	Constructor, specifying the disallowed pane ID

VPDuplicatePaneIDException::VPDuplicatePaneIDException(
	PaneIDT		inAttemptedPaneID)

{
	mAttemptedPaneID = inAttemptedPaneID;
}


// ---------------------------------------------------------------------------
//		* ~VPDuplicatePaneIDException
// ---------------------------------------------------------------------------
//	Destructor

VPDuplicatePaneIDException::~VPDuplicatePaneIDException()
{
}


// ---------------------------------------------------------------------------
//		* GetErrorString
// ---------------------------------------------------------------------------
//	Return a string that describes the error that has occurred.

void
VPDuplicatePaneIDException::GetErrorString(
	LStr255&	outDescriptor) const 
{

	// Convert attempted pane ID to a printable string.
	
	Str255 attemptedIDStr;
	UTextID::MakePrintableID(mAttemptedPaneID, attemptedIDStr, true, 32);

	// Build description string.

	outDescriptor =
			LStr255(STR_DuplicateIDException, str_PaneIDEntered) + 	// "The pane ID you entered, "
			LStr255(attemptedIDStr) +
			LStr255(STR_DuplicateIDException, str_CannotBeUsed);	// ", cannot be used because ..."

}
