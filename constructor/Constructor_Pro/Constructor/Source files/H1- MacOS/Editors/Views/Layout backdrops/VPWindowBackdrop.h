// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPWindowBackdrop.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:04 $
//  $History: VPWindowBackdrop.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:01
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Layout backdrops
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 21:37
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added DrawContentArea and window title area helpers.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:26
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Layout backdrops
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Editors : Views : User interface
#include "VEBackdrop.h"


// ===========================================================================
//		* VPWindowBackdrop
// ===========================================================================
//
//	VPWindowBackdrop is a common base class for layout backdrops that
//	mimic the appearance of the standard MacOS windows.
//
// ===========================================================================

class VPWindowBackdrop : public VEBackdrop {

public:
							VPWindowBackdrop();
	virtual					~VPWindowBackdrop();

	// backdrop drawing

	virtual void			DrawSelf();
	virtual void			DrawContentArea();

	// window content area

protected:
	virtual Boolean			CalcLocalContentRect(
									Rect&			outFrameRect);

	virtual void			GetWindowContentColor(
									RGBColor&		outContentColor);
	virtual void			RootAttributeChanged(
									DMAttribute*	inAttribute);

	// window title area
	
	virtual Boolean			CalcLocalTitleRect(
									Rect&			outFrameRect);
#if !PP_Target_Carbon
	virtual void			DrawWindowTitle(
									ResIDT			inTxtrID,
									Int16			inHorizInset);
#endif
};

#if PP_Target_Carbon
pascal void DrawWindowTitle(const Rect *bounds, SInt16 depth, Boolean colorDevice, UInt32 userData);
#endif