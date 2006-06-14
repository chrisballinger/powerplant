// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLCaption.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:58 $
//  $History: VPLCaption.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:55
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:45
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:24
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLCaption.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLCaption ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLCaption::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast attribute to integer attribute. We'll probably need it.
	
	DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(inAttribute));

	// Cast display pane to LCaption.
	
	LCaption* caption = (dynamic_cast<LCaption*>(mDisplayPane));
	ValidateObject_(caption);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Text traits: Update caption's txtr ID and redraw.

		case pPaneTextTraits:
			ValidateObject_(intAttr);
			caption->SetTextTraitsID(intAttr->GetIntValue());
			caption->Refresh();
			break;
		
		// Otherwise, see if LPane wants it.

		default:
			VPLPane::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}
