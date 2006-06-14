// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLLittleArrows.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LLittleArrows.h"

class VPLLittleArrows : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLLittleArrows; }

							VPLLittleArrows() { }
	virtual					~VPLLittleArrows() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LLittleArrows(inStream);
								}
};
