// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEResizeReceiver.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/10/96
//     $Date: 2006/01/18 01:33:19 $
//	$History: VEResizeReceiver.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:42
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a broken include.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Editors : Views : User interface
#include "VEPrefsResources.h"

	// PowerPlant : CA : Features
#include <LMarqueeTask.h>


class VELayoutView;


// ===========================================================================
//		* VEResizeReceiver
// ===========================================================================
//	VEResizeReceiver is a helper class used by VELayoutView during the
//	dragging of a selection (resize) handle.

class VEResizeReceiver : public LMarqueeReceiver {

protected:
							VEResizeReceiver(
									VELayoutView&		inLayoutView,
									Point				inMouseDownLocal,
									Point				inAnchorPoint,
									const SDimension16&	inMouseDelta,
									Boolean				inConstrainHoriz,
									Boolean				inConstrainVert);
	virtual					~VEResizeReceiver();

	virtual void			ConstrainMouse(
									const LMarqueeTask&	inMarqueeTask,
									Point&				ioMouseLocal,
									UInt16				inModifiers);
									
	virtual LView*			GetTargetView() const;

	// data members

protected:
	VELayoutView&			mLayoutView;
	SGridInfo				mGridInfo;
	Point					mMouseDownLocal;
	Point					mAnchorPoint;
	SDimension16			mMouseDelta;
	SDimension32			mTopLeftSlop;
	Boolean					mConstrainHoriz;
	Boolean					mConstrainVert;


	friend class VELayoutView;

};
