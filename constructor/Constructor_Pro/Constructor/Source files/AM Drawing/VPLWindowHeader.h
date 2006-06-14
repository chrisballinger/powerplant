// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLWindowHeader.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"
#include "LWindowHeader.h"

class VPLWindowHeader : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLWindowHeader; }

							VPLWindowHeader() { }
	virtual					~VPLWindowHeader() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LWindowHeader(inStream);
								}
};
