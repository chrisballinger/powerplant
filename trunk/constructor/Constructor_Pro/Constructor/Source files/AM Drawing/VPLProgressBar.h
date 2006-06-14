// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLProgressBar.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include "LProgressBar.h"

class VPLProgressBar : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLProgressBar; }

							VPLProgressBar() { }
	virtual					~VPLProgressBar() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									LProgressBar	*bar = new LProgressBar(inStream);
									bar->StopIdling();
									return bar;
								}

protected:
	virtual void			ValueChangedSelf(
									FourCharCode	inAttributeKey,
									DMAttribute*	inAttribute);
};
