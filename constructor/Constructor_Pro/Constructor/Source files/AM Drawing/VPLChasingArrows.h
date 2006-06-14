// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLChasingArrows.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LChasingArrows.h"

class VPLChasingArrows : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLChasingArrows; }

							VPLChasingArrows() { }
	virtual					~VPLChasingArrows() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									LChasingArrows	*arrows = new LChasingArrows(inStream);
									arrows->StopIdling();
									return arrows;
								}
};
