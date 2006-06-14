// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGALittleArrows.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/17/96
//     $Date: 2006/01/18 01:34:00 $
//  $History: VPLGALittleArrows.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:57
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:47
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/17/96   Time: 13:40
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLControl.h"

	// PowerPlant : GA : Grayscale controls
#include <LGALittleArrows.h>


// ===========================================================================
//		* VPLGALittleArrows
// ===========================================================================
//	Drawing agent for the PowerPlant class LGALittleArrows.

class VPLGALittleArrows : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGALittleArrows; }

							VPLGALittleArrows() {}
	virtual					~VPLGALittleArrows() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LGALittleArrows(inStream); }

};
