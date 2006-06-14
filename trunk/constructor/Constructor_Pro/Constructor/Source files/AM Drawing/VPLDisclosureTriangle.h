// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLDisclosureTriangle.h			©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LDisclosureTriangle.h"

class VPLDisclosureTriangle : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLDisclosureTriangle; }

							VPLDisclosureTriangle() { }
	virtual					~VPLDisclosureTriangle() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LDisclosureTriangle(inStream);
								}
};
