// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLToggleButton.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:03 $
//  $History: VPLToggleButton.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:18
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLControl.h"

	// PowerPlant : GA : Grayscale controls
#include <LToggleButton.h>


// ===========================================================================
//		* VPLToggleButton
// ===========================================================================
//	Drawing agent for the PowerPlant class LToggleButton.

class VPLToggleButton : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLToggleButton; }

							VPLToggleButton() {}
	virtual					~VPLToggleButton() {}

	virtual LPane*			CreateFromStream(LStream* inStream);

	virtual Boolean			ListenToMap();
	virtual void			ResourceChanged(
									RMResource*			inResource);

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
	
};


// ===========================================================================
//		* VPFToggleButton
// ===========================================================================
//	Helper class for LToggleButton. Restricts search for graphics resources
//	to the top resource file.

class VPFToggleButton : public LToggleButton {

public:
							VPFToggleButton(LStream* inStream);
	virtual					~VPFToggleButton();

protected:
	virtual void			LoadGraphic();
	virtual void			DrawGraphic(
									ResIDT			inGraphicID);


	// data members

protected:
	Handle					mIcon;
	Handle					mIconSuite;
	PicHandle				mPicture;

	friend class VPLToggleButton;

};
