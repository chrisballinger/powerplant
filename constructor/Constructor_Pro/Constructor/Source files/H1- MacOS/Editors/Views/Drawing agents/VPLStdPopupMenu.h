// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLStdPopupMenu.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:02 $
//  $History: VPLStdPopupMenu.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/24/96   Time: 22:41
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Changed VPLStdPopupMenu so it derives from VPLControl instead of
//	VPLView.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:15
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLControl.h"


// ===========================================================================
//		* VPLStdPopupMenu
// ===========================================================================
//	Drawing agent for the PowerPlant class LStdPopupMenu.

class VPLStdPopupMenu : public VPLControl {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLStdPopupMenu; }

							VPLStdPopupMenu() {}
	virtual					~VPLStdPopupMenu() {}

	virtual LPane*			CreateFromStream(LStream* inStream);

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

};


// ===========================================================================
//		* VPFPopupMenu
// ===========================================================================
//	Helper class for LStdPopupMenu drawing agent. Draws a simulated
//	popup menu.

class VPFPopupMenu : public LPane {

public:
							VPFPopupMenu(LStream* inStream);
	virtual					~VPFPopupMenu();

protected:
	virtual void			DrawSelf();


	// data members

protected:
	Str255					mTitleString;
	SInt16					mTitleStyle;
	SInt16					mTitleAlignment;
	SInt16					mTitleWidth;

	Boolean					mFixedWidth;
	Boolean					mUseResMenu;
	ResType					mResMenuType;

	ResIDT					mTextTraitsID;

	ResIDT					mMenuID;
	SInt16					mMenuInitialItem;

	friend class VPLStdPopupMenu;

};
