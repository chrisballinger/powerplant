// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLControl.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:58 $
//  $History: VPLControl.cpp $
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
//	User: scouten      QDate: 11/17/96   Time: 11:26
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLControl.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLControl ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLControl::ValueChangedSelf(
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

	// Cast display pane to LControl.
	
	LControl* control = (dynamic_cast<LControl*>(mDisplayPane));
	ValidateObject_(control);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Control value: Redraw control.

		case pPaneValue:
			ValidateObject_(intAttr);
			control->SetValue(intAttr->GetIntValue());
			control->Refresh();
			break;
		
		// Control minimum value: Redraw control.

		case pPaneMinimumValue:
			ValidateObject_(intAttr);
			control->SetMinValue(intAttr->GetIntValue());
			control->Refresh();
			break;

		// Control maximum value: Redraw control.

		case pPaneMaximumValue:
			ValidateObject_(intAttr);
			control->SetMaxValue(intAttr->GetIntValue());
			control->Refresh();
			break;

		// Otherwise, see if LPane wants it.

		default:
			VPLPane::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}
