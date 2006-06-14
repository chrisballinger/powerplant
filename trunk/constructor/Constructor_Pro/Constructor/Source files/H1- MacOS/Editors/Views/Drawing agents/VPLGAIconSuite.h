// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAIconSuite.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:00 $
//  $History: VPLGAIconSuite.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:01
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:56
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLPane.h"

	// PowerPlant : GA : Grayscale controls
#include <LGAIconSuite.h>


// ===========================================================================
//		* VPLGAIconSuite
// ===========================================================================
//	Drawing agent for the PowerPlant class LGAIconSuite.

class VPLGAIconSuite : public VPLPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGAIconSuite; }

							VPLGAIconSuite() {}
	virtual					~VPLGAIconSuite() {}

	virtual LPane*			CreateFromStream(LStream* inStream);

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

	virtual Boolean			ListenToMap();
	virtual void			ResourceChanged(
									RMResource*			inResource);
};


// ===========================================================================
//		* VPFGAIconSuite
// ===========================================================================
//	Helper class for VPLGAIconSuite. Restricts search for graphics resources
//	to the top resource file.

class VPFGAIconSuite : public LGAIconSuite {

public:
							VPFGAIconSuite(LStream* inStream);
	virtual					~VPFGAIconSuite() { }

	virtual	void			LoadIconSuiteHandle();
};
