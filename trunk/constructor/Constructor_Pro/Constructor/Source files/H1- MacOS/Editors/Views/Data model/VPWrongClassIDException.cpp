// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPWrongClassIDException.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/06/96
//     $Date: 2006/01/18 01:33:57 $
//  $History: VPWrongClassIDException.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:40
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:45
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/06/96   Time: 15:10
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added  class.
//	
// ===========================================================================

#include "VPWrongClassIDException.h"

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"
#include "VETypeList.h"

	// Core : Utilities
#include "UTextID.h"


// ===========================================================================
//		* Resource IDS
// ===========================================================================

const ResIDT	STR_WrongClassID		= 3102;
const SInt16	str_PaneIDEntered		= 1;
const SInt16	str_CannotBeUsed		= 2;
const SInt16	str_MustPointTo			= 3;
const SInt16	str_Period				= 4;


// ===========================================================================

#pragma mark *** VPWrongClassIDException ***

// ---------------------------------------------------------------------------
//		* VPWrongClassIDException(...)
// ---------------------------------------------------------------------------
//	Constructor, specifying the parameters of the exception

VPWrongClassIDException::VPWrongClassIDException(
	DMAttribute*	inAttribute,
	PaneIDT			inAttemptedPaneID,
	ClassIDT		inClassID)
{

	// Validate pointers.
	
	ValidateObject_(inAttribute);
	
	// Save exception parameters.
	
	mAttribute = inAttribute;
	mAttemptedPaneID = inAttemptedPaneID;
	mClassID = inClassID;
	
}


// ---------------------------------------------------------------------------
//		* ~VPWrongClassIDException
// ---------------------------------------------------------------------------
//	Destructor

VPWrongClassIDException::~VPWrongClassIDException()
{
}


// ---------------------------------------------------------------------------
//		* GetErrorString
// ---------------------------------------------------------------------------
//	Return a string that describes the error that has occurred.

void
VPWrongClassIDException::GetErrorString(
	LStr255&	outDescriptor) const 
{

	// Get attribute's title.

	LStr255 attrTitle;
	if (mAttribute != nil) {
		ValidateObject_(mAttribute.GetObject());
		mAttribute->GetTitle(attrTitle);
	}

	// Convert attempted pane ID to a printable string.
	
	Str255 attemptedIDStr;
	UTextID::MakePrintableID(mAttemptedPaneID, attemptedIDStr, true, 32);

	// Convert required class ID to printable name.
	
	LStr255 className;

	VETypeList* globalTypeList = VETypeList::GetTypeList();
	ValidateObject_(globalTypeList);
	
	VETypeEntry* type = globalTypeList->FindTypeByID(mClassID);
	if (type != nil) {
		ValidateObject_(type);
		type->GetClassName(className);
	}
	
	// Build description string.

	outDescriptor =
			LStr255(STR_WrongClassID, str_PaneIDEntered) +	// "The pane ID you entered, "
			LStr255(attemptedIDStr) +
			LStr255(STR_WrongClassID, str_CannotBeUsed) +	// ", cannot be used. The attribute Ò"
			LStr255(attrTitle) +
			LStr255(STR_WrongClassID, str_MustPointTo) +	// "Ó must point to an object of type "
			LStr255(className) +
			LStr255(STR_WrongClassID, str_Period);			// "."

}
