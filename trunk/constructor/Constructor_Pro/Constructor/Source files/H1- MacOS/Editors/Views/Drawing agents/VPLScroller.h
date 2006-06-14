// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLScroller.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:34:02 $
//  $History: VPLScroller.h $
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
//	User: scouten      QDate: 11/17/96   Time: 14:12
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLView.h"


// ===========================================================================
//		* VPLScroller
// ===========================================================================
//	Drawing agent for the PowerPlant class LScroller.

class VPLScroller : public VPLView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLScroller; }

							VPLScroller() {}
	virtual					~VPLScroller() {}

	virtual LPane*			CreateFromStream(LStream* inStream);
	virtual void			DrawDisplayObject();


protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
	virtual void			SubPaneDeletedSelf(
									LPane*				inSubPane);

};


// ===========================================================================
//		* VPFScroller
// ===========================================================================
//	Helper class for LGAFocusBorder drawing agent. Includes safety checks
//	for scrolling view ID.

class VPFScroller : public LScroller {

public:
							VPFScroller(LStream* inStream);
	virtual					~VPFScroller();

protected:
	virtual void			FinishCreateSelf();


	friend class VPLScroller;

};
