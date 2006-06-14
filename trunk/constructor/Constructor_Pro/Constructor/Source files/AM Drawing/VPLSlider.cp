// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLSlider.cp					©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLSlider.h"
#include "VPDataModel.h"


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLSlider::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{
	switch (inAttributeKey) {
	
		case pPaneWidth:			// May need to change slider orientation
		case pPaneHeight:
			RebuildMe();
			break;
	
		default:
			VPLControlPane::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}
