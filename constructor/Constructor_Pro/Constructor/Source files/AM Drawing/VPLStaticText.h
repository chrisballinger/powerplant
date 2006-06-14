// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLStaticText.h					©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LStaticText.h"

class VPLStaticText : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLStaticText; }

							VPLStaticText() { }
	virtual					~VPLStaticText() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LStaticText(inStream);
								}
};
