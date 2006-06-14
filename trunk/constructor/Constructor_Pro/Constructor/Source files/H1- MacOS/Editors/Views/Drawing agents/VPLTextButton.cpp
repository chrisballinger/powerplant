// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLTextButton.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:02 $
//  $History: VPLTextButton.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:16
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLTextButton.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLTextButton ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLTextButton::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Control selected style, text traits: Rebuild control.

		case pPaneButtonSelectedBold:
		case pPaneButtonSelectedUnderline:
		case pPaneButtonSelectedItalic:
		case pPaneButtonSelectedOutline:
		case pPaneButtonSelectedShadow:
		case pPaneButtonSelectedCondensed:
		case pPaneButtonSelectedExtended:
		case pPaneTextTraits:
			RebuildMe();
			break;

		// Otherwise, see if LControl wants it.

		default:
			VPLControl::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}
