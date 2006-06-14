// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLControlView.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLView.h"
#include "LControlView.h"

class VPLControlView : public VPLView {

public:
							VPLControlView() { }
	virtual					~VPLControlView() { }
	
	virtual void			DrawDisplayObject();

protected:
	virtual void			ValueChangedSelf(
									FourCharCode	inAttributeKey,
									DMAttribute*	inAttribute);
};
