// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLSeparatorLine.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LSeparatorLine.h"

class VPLSeparatorLine : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLSeparatorLine; }

							VPLSeparatorLine() { }
	virtual					~VPLSeparatorLine() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LSeparatorLine(inStream);
								}
};
