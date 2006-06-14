// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAPrimaryBox.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:00 $
//  $History: VPLGAPrimaryBox.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:52
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:47
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:00
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLView.h"

	// PowerPlant : GA : Grayscale panes
#include <LGAPrimaryBox.h>


// ===========================================================================
//		* VPLGAPrimaryBox
// ===========================================================================
//	Drawing agent for the PowerPlant class LGAPrimaryBox.

class VPLGAPrimaryBox : public VPLView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGAPrimaryBox; }

							VPLGAPrimaryBox() {}
	virtual					~VPLGAPrimaryBox() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LGAPrimaryBox(inStream); }

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

};
