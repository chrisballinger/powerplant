// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPopupGroupBox.cp				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLPopupGroupBox.h"


LPane*
VPLPopupGroupBox::CreateFromStream(
	LStream		*inStream)
{
	return new LPopupGroupBox(inStream);
}
