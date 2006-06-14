// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAFocusBorder.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:59 $
//  $History: VPLGAFocusBorder.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:56
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:46
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:52
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLView.h"

	// PowerPlant : GA : Grayscale controls
#include <LGAFocusBorder.h>


// ===========================================================================
//		* VPLGAFocusBorder
// ===========================================================================
//	Drawing agent for the PowerPlant class LGAFocusBorder.

class VPLGAFocusBorder : public VPLView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGAFocusBorder; }

							VPLGAFocusBorder() {}
	virtual					~VPLGAFocusBorder() {}

	virtual LPane*			CreateFromStream(LStream* inStream);

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
	virtual void			SubPaneDeletedSelf(
									LPane*				inSubPane);

};


// ===========================================================================
//		* VPFGAFocusBorder
// ===========================================================================
//	Helper class for LGAFocusBorder drawing agent. Includes safety checks
//	for inset sub pane ID.

class VPFGAFocusBorder : public LGAFocusBorder {

public:
							VPFGAFocusBorder(LStream* inStream);
	virtual					~VPFGAFocusBorder();

protected:
	virtual void			FinishCreateSelf();


	friend class VPLGAFocusBorder;

};

