// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLProgressBar.cp				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLProgressBar.h"
#include "VPDataModel.h"


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLProgressBar::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{
	switch (inAttributeKey) {
	
		case pPBarIndeterminate:
			RebuildMe();
			break;
	
		default:
			VPLControlPane::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}
