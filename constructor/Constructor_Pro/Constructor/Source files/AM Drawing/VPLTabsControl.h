// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLTabsControl.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"
#include "LTabsControl.h"

class VPLTabsControl : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLTabsControl; }

							VPLTabsControl() { }
	virtual					~VPLTabsControl() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LTabsControl(inStream);
								}
};
