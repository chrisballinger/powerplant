// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLControlView.cp				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLControlView.h"
#include "VPDataModel.h"
#include "DMIntegerAttribute.h"


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLControlView::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{
	switch (inAttributeKey) {
	
		case pRebuildMe:
		case pPaneControlKind:
		case pPaneTextTraits:
		case pPaneDescriptor:	// JCD 980701 - added to fix MW05674
			RebuildMe();
			break;
			
		case pPaneValue: {
			LControlView* view = dynamic_cast<LControlView*>(GetDisplayPane());
			if (view != nil) {
				DMIntegerAttribute* attr = (dynamic_cast<DMIntegerAttribute*>(inAttribute));
				if (attr != nil) {
					view->SetValue(attr->GetIntValue());
					view->Refresh();
				}
			}
			break;
		}
	
		default:
			VPLView::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}

// ---------------------------------------------------------------------------
//		* DrawDisplayObject
// ---------------------------------------------------------------------------
//	Draw the visual representation of this object. Do NOT draw any of
//	its subpanes.

void
VPLControlView::DrawDisplayObject()
{

	// Validate pointers.
	
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for drawing.
	
	LControlView* view = dynamic_cast<LControlView*>(GetDisplayPane());
	
	if (view != nil) {
		ValidateObject_(view);
		view->DrawWholeView();
	}
}
