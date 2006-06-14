// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLCheckBoxGroupBox.h			©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"
#include "LCheckBoxGroupBox.h"

class VPLCheckBoxGroupBox : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLCheckBoxGroupBox; }

							VPLCheckBoxGroupBox() { }
	virtual					~VPLCheckBoxGroupBox() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LCheckBoxGroupBox(inStream);
								}
};
