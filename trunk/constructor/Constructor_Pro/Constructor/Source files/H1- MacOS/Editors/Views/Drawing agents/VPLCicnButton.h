// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLCicnButton.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:58 $
//  $History: VPLCicnButton.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:25
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLControl.h"


// ===========================================================================
//		* VPLCicnButton
// ===========================================================================
//	Drawing agent for the PowerPlant class LCicnButton.

class VPLCicnButton : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLCicnButton; }

							VPLCicnButton() {}
	virtual					~VPLCicnButton() {}

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
//		* VPFCicnButton
// ===========================================================================
//	Helper class for LButton. Restricts search for 'cicn' resources
//	to the top resource file.

class VPFCicnButton : public LCicnButton {

public:
							VPFCicnButton(LStream* inStream);
	virtual					~VPFCicnButton();

protected:
	virtual void			DrawSelf();
	virtual void			LoadGraphic();


	// data members

protected:
	CIconHandle				mIcon;

	friend class VPLCicnButton;

};
