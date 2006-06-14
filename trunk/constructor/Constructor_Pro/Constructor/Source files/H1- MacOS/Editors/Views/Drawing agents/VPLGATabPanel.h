// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGATabPanel.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/17/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLGATabPanel.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:47
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/21/96   Time: 18:57
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Fixed drawing problems with LGATabPanel. (Bug fix #1152.)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/17/96   Time: 14:23
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLView.h"

	// PowerPlant : GA : Grayscale panes
#include <LGATabPanel.h>


// ===========================================================================
//		* VPLGATabPanel
// ===========================================================================
//	Drawing agent for the PowerPlant class LGASeparator.

class VPLGATabPanel : public VPLView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGATabPanel; }

							VPLGATabPanel() {}
	virtual					~VPLGATabPanel() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LGATabPanel(inStream); }
	virtual void			DrawDisplayObject();


protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
	virtual void			ContainerListChanged(
									DM_ListChange*		inMessage);

};
