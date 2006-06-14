// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLControlPane.cp				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLControlPane.h"
#include "VPDataModel.h"


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLControlPane::ValueChangedSelf(
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
	
		default:
			VPLControl::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}