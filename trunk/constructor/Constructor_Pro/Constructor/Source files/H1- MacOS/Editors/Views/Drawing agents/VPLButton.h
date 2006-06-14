// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLButton.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:58 $
//  $History: VPLButton.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 10:26
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Improved handling of graphics resources.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:23
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLControl.h"


// ===========================================================================
//		* VPLButton
// ===========================================================================
//	Drawing agent for the PowerPlant class LButton.

class VPLButton : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLButton; }

							VPLButton() {}
	virtual					~VPLButton() {}

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
//		* VPFButton
// ===========================================================================
//	Helper class for LButton. Restricts search for graphics resources
//	to the top resource file.

class VPFButton : public LButton {

public:
							VPFButton(LStream* inStream);
	virtual					~VPFButton();

protected:
	virtual void			LoadGraphic();
	virtual void			DrawGraphic(ResIDT inGraphicID);


	// data members

protected:
	Handle					mIcon;
	Handle					mIconSuite;
	PicHandle				mPicture;
	
	friend class VPLButton;

};
