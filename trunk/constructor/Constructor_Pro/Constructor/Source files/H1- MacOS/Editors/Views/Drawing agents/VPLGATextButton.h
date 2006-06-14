// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGATextButton.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLGATextButton.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:47
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:03
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLControl.h"

	// PowerPlant : GA : Grayscale controls
#include <LGATextButton.h>


// ===========================================================================
//		* VPLGATextButton
// ===========================================================================
//	Drawing agent for the PowerPlant class LGATextButton.

class VPLGATextButton : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGATextButton; }

							VPLGATextButton() {}
	virtual					~VPLGATextButton() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LGATextButton(inStream); }

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

};
