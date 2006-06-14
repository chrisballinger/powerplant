// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPopupGroupBox.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"
#include "LPopupGroupBox.h"

class VPLPopupGroupBox : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPopupGroupBox; }

							VPLPopupGroupBox() { }
	virtual					~VPLPopupGroupBox() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream);
};
