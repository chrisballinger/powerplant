// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPane.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/13/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLPane.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 16:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed drawing agent so it forces a redraw-all before resizing the
//	object. (Bug fix #1279.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:04
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPDrawingAgent.h"


// ===========================================================================
//		* VPLPane
// ===========================================================================
//	Drawing agent for the PowerPlant class LPane.

class VPLPane : public VPDrawingAgent {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPane; }

							VPLPane() {}
	virtual					~VPLPane() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LPane(inStream); }

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

};
