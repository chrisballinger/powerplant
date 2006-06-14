// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLTextGroupBox.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"
#include "LTextGroupBox.h"

class VPLTextGroupBox : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLTextGroupBox; }

							VPLTextGroupBox() { }
	virtual					~VPLTextGroupBox() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LTextGroupBox(inStream);
								}
};
