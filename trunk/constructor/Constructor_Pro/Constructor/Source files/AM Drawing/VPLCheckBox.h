// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLCheckBox.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
 
#pragma once

#include "VPLControlPane.h"
#include "LCheckBox.h"

class VPLCheckBox : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLCheckBox; }

							VPLCheckBox() { }
	virtual					~VPLCheckBox() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LCheckBox(inStream);
								}
};
