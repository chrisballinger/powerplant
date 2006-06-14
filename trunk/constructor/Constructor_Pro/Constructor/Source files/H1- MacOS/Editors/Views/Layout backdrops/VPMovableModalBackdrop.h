// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPMovableModalBackdrop.h	� 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/18/96
//     $Date: 2006/01/18 01:34:04 $
//  $History: VPMovableModalBackdrop.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:36
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added CalcLocalBackdropFrame method.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/18/96   Time: 20:43
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Layout backdrops
#include "VPNormalDialogBackdrop.h"


// ===========================================================================
//		* VPMovableModalBackdrop
// ===========================================================================
//
//	VPMovableModalBackdrop is a layout backdrop that simulates the appearance
//	of a movable modal dialog window.
//
// ===========================================================================

class VPMovableModalBackdrop : public VPNormalDialogBackdrop {

public:
							VPMovableModalBackdrop();
	virtual					~VPMovableModalBackdrop();

	// backdrop drawing

	virtual void			DrawSelf();
	
	// backdrop location
	
	virtual Boolean			CalcLocalBackdropFrame(
									Rect&				outFrameRect);
	virtual void			GetBackdropOverscan(
									SDimension32&		outTopLeft,
									SDimension32&		outBotRight);

	// window title area

protected:	
	virtual Boolean			CalcLocalTitleRect(
									Rect&				outFrameRect);

};
