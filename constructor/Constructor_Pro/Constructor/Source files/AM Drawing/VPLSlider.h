// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLSlider.h						©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LSlider.h"

class VPLSlider : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLSlider; }

							VPLSlider() { }
	virtual					~VPLSlider() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LSlider(inStream);
								}

protected:
	virtual void			ValueChangedSelf(
									FourCharCode	inAttributeKey,
									DMAttribute*	inAttribute);
};
