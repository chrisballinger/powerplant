// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPlacard.h					©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"
#include "LPlacard.h"

class VPLPlacard : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPlacard; }

							VPLPlacard() { }
	virtual					~VPLPlacard() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LPlacard(inStream);
								}
};
