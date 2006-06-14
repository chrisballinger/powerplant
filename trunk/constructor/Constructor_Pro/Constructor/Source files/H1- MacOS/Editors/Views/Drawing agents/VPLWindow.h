// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLWindow.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:34:03 $
//  $History: VPLWindow.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/18/96   Time: 20:34
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added support for layout backdrops.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLView.h"


// ===========================================================================
//		* VPLWindow
// ===========================================================================
//	Drawing agent for the PowerPlant class LWindow.

class VPLWindow : public VPLView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLWindow; }

							VPLWindow() {}
	virtual					~VPLWindow() {}

	virtual LPane*			CreateFromStream(LStream* inStream);

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

	virtual void			UpdateLayoutKind();

};


// ===========================================================================
//		* VPFWindow
// ===========================================================================
//	Helper class that implements LWindow's ExpandSubPane behavior,
//	but isn't an LWindow itself.

class VPFWindow : public LView {

public:
							VPFWindow(
									const SPaneInfo&	inPaneInfo,
									const SViewInfo&	inViewInfo);
	virtual					~VPFWindow();

	virtual void			ExpandSubPane(
									LPane*				inSub,
									Boolean				inExpandHoriz,
									Boolean				inExpandVert);

};
