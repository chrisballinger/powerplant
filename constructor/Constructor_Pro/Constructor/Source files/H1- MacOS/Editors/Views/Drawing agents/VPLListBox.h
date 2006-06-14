// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLListBox.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/08/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLListBox.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/08/96   Time: 17:16
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLPane.h"


// ===========================================================================
//		* VPLListBox
// ===========================================================================
//	Drawing agent for the PowerPlant class LListBox.

class VPLListBox : public VPLPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLListBox; }

							VPLListBox() {}
	virtual					~VPLListBox() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LListBox(inStream); }

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

	virtual void			ContainerListChanged(
									DM_ListChange*		inMessage);

};
