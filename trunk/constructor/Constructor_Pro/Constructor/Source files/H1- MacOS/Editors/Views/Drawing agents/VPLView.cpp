// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLView.cpp					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/13/96
//     $Date: 2006/01/18 01:34:03 $
//  $History: VPLView.cpp $
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
//	User: scouten      QDate: 11/17/96   Time: 14:19
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLView.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLView ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLView::ValueChangedSelf(
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

	// Cast display pane to LView.
	
	LView* view = (dynamic_cast<LView*>(mDisplayPane));
	ValidateObject_(view);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Image width: Resize image.

		case pPaneImageWidth: {
			SDimension32 imageSize;
			view->GetImageSize(imageSize);
			ValidateObject_(intAttr);
			imageSize.width = intAttr->GetIntValue();
			view->ResizeImageTo(imageSize.width, imageSize.height, true);
			break;
		}

		// Image height: Resize image.

		case pPaneImageHeight: {
			SDimension32 imageSize;
			view->GetImageSize(imageSize);
			ValidateObject_(intAttr);
			imageSize.height = intAttr->GetIntValue();
			view->ResizeImageTo(imageSize.width, imageSize.height, true);
			break;
		}
		
		// Scroll position: Rebuild view. (Brute force approach... *sigh*)

		case pPaneScrollPosHoriz:
		case pPaneScrollPosVert:
			RebuildMe();
			break;
		
		// Reconcile overhang: Adjust setting.
		
		case pPaneReconcileOverhang:
			ValidateObject_(intAttr);
			SInt32 reconcile = intAttr->GetIntValue();
			view->SetReconcileOverhang(reconcile);
			break;
		
		// Otherwise, see if LPane wants it.

		default:
			VPLPane::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}
