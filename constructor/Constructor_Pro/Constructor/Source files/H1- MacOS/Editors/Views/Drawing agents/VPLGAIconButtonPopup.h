// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAIconButtonPopup.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:59 $
//  $History: VPLGAIconButtonPopup.h $
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
//	User: scouten      QDate: 11/17/96   Time: 13:55
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLGAIconButton.h"

	// PowerPlant : GA : Grayscale controls
#include <LGAIconButtonPopup.h>


// ===========================================================================
//		* VPLGAIconButtonPopup
// ===========================================================================
//	Drawing agent for the PowerPlant class LGAIconButtonPopup.

class VPLGAIconButtonPopup : public VPLGAIconButton {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGAIconButtonPopup; }

							VPLGAIconButtonPopup() {}
	virtual					~VPLGAIconButtonPopup() {}

	virtual LPane*			CreateFromStream(LStream* inStream); 

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
	
};


// ===========================================================================
//		* VPFGAIconButtonPopup
// ===========================================================================
//	Helper class for LGAIconButtonPopup. Prevents popup from loading menus
//	that it shouldn't (i.e. Constructor's menus).

class VPFGAIconButtonPopup : public LGAIconButtonPopup {

public:
							VPFGAIconButtonPopup(LStream* inStream);
	virtual					~VPFGAIconButtonPopup();

	virtual MenuHandle		LoadPopupMenuH() const;
	virtual	void			LoadIconSuiteHandle();

	friend class VPLGAIconButtonPopup;

};
