// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGASecondaryBox.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLGASecondaryBox.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/21/97   Time: 17:11
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:02
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLGAPrimaryBox.h"

	// PowerPlant : GA : Grayscale controls
#include <LGASecondaryBox.h>


// ===========================================================================
//		* VPLGASecondaryBox
// ===========================================================================
//	Drawing agent for the PowerPlant class LGASecondaryBox.

class VPLGASecondaryBox : public VPLGAPrimaryBox {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGASecondaryBox; }

							VPLGASecondaryBox() {}
	virtual					~VPLGASecondaryBox() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LGASecondaryBox(inStream); }

};
