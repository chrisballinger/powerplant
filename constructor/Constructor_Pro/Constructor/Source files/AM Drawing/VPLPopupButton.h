// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPopupButton.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include <LPopupButton.h>

class VPLPopupButton : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPopupButton; }

							VPLPopupButton() { }
	virtual					~VPLPopupButton() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream);
};