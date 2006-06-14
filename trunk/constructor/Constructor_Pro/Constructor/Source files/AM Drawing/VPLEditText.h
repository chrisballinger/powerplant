// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLEditText.h					©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LEditText.h"

class VPLEditText : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLEditText; }

							VPLEditText() { }
	virtual					~VPLEditText() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LEditText(inStream);
								}
};
