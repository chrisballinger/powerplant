// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLRadioButton.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LRadioButton.h"

class VPLRadioButton : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLRadioButton; }

							VPLRadioButton() { }
	virtual					~VPLRadioButton() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LRadioButton(inStream);
								}
};
