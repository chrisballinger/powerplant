// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLTextButton.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:02 $
//  $History: VPLTextButton.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:16
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLControl.h"

	// PowerPlant : Pane classes
#include <LTextButton.h>


// ===========================================================================
//		* VPLTextButton
// ===========================================================================
//	Drawing agent for the PowerPlant class LTextButton.

class VPLTextButton : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLTextButton; }

							VPLTextButton() {}
	virtual					~VPLTextButton() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LTextButton(inStream); }

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

};
