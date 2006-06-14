// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLControlPane.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControl.h"
#include "LControlPane.h"

class VPLControlPane : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLControlPane; }

							VPLControlPane() { }
	virtual					~VPLControlPane() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LControlPane(inStream);
								}

protected:
	virtual void			ValueChangedSelf(
									FourCharCode	inAttributeKey,
									DMAttribute*	inAttribute);

};
