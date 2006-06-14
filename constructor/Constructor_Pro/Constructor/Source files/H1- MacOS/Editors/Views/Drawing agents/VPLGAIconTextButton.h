// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAIconTextButton.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:00 $
//  $History: VPLGAIconTextButton.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:56
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:46
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:58
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLGAIconButton.h"

	// PowerPlant : GA : Grayscale controls
#include <LGAIconTextButton.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class VPFGAIconTextButton;


// ===========================================================================
//		* VPLGAIconTextButton
// ===========================================================================
//	Drawing agent for the PowerPlant class LGAIconTextButton.

class VPLGAIconTextButton : public VPLGAIconButton {

public:
	static VEDrawingAgent*	CreateAgent()
								    { return new VPLGAIconTextButton; }

							VPLGAIconTextButton() {}
	virtual					~VPLGAIconTextButton() {}

	virtual LPane*			CreateFromStream(LStream* inStream);

};


// ===========================================================================
//		* VPFGAIconTextButton
// ===========================================================================
//	Helper class for LGAIconTextButton. Restricts search for graphics resources
//	to the top resource file.

class VPFGAIconTextButton : public LGAIconTextButton {

public:
							VPFGAIconTextButton(LStream* inStream);
	virtual					~VPFGAIconTextButton() { }

	virtual	void			LoadIconSuiteHandle();
};
