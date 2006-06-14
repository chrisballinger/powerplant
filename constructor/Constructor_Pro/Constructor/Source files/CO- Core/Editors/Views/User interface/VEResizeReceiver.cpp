// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEResizeReceiver.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/10/96
//     $Date: 2006/01/18 01:33:19 $
//	$History: VEResizeReceiver.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEResizeReceiver.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"


// ===========================================================================

#pragma mark *** VEResizeReceiver ***

// ---------------------------------------------------------------------------
//		* VEResizeReceiver(...)									[protected]
// ---------------------------------------------------------------------------
//	Constructor
//
//	Should only be created and destroyed by a VELayoutView. 

VEResizeReceiver::VEResizeReceiver(
	VELayoutView&		inLayoutView,
	Point				inMouseDownLocal,
	Point				inAnchorPoint,
	const SDimension16&	inMouseDelta,
	Boolean				inConstrainHoriz,
	Boolean				inConstrainVert)

: mLayoutView(inLayoutView),
  mGridInfo(inLayoutView.mGridInfo),
  mMouseDownLocal(inMouseDownLocal),
  mAnchorPoint(inAnchorPoint),
  mMouseDelta(inMouseDelta)

{
	SDimension32 ignore;
	mLayoutView.GetDesktopOverscan(mTopLeftSlop, ignore);
	
	mMouseDownLocal.h -= mTopLeftSlop.width;
	mMouseDownLocal.v -= mTopLeftSlop.height;

	mAnchorPoint.h -= mTopLeftSlop.width;
	mAnchorPoint.v -= mTopLeftSlop.height;

	mConstrainHoriz = inConstrainHoriz;
	mConstrainVert = inConstrainVert;
}



// ---------------------------------------------------------------------------
//		* ~VEResizeReceiver										[protected]
// ---------------------------------------------------------------------------
//	Destructor

VEResizeReceiver::~VEResizeReceiver()
{
}


// ---------------------------------------------------------------------------
//		* ConstrainMouse										[protected]
// ---------------------------------------------------------------------------
//	Implement gridding and axis constraints.

void
VEResizeReceiver::ConstrainMouse(
	const LMarqueeTask&	/* inMarqueeTask */,
	Point&				ioMouseLocal,
	UInt16				inModifiers)
{

	// Shift mouse location by offset in handle.
	
	ioMouseLocal.h -= mMouseDelta.width;					//* 2.1.3b1: BUG FIX #456: -=, not +=
	ioMouseLocal.v -= mMouseDelta.height;					//* 2.1.3b1: BUG FIX #456: -=, not +=

	// Decide which grid constraints we can do.
	
	Boolean shiftConstrain = (inModifiers & shiftKey) == shiftKey;
	Boolean gridConstrain = ((inModifiers & cmdKey) == cmdKey) ^ (mGridInfo.mSnapToGrid);
	
	if (mConstrainHoriz || mConstrainVert)
		shiftConstrain = false;
	
	if (!shiftConstrain && !gridConstrain && !mConstrainHoriz && !mConstrainVert)
		return;						// if no constraints, bail out now

	// Convert mouse location to local coordinates of the base view.
	
	Point dragMouse = ioMouseLocal;
	dragMouse.h -= mTopLeftSlop.width;
	dragMouse.v -= mTopLeftSlop.height;
	
	// Execute constraints.

	if (gridConstrain) {
		mLayoutView.NearestSnapPoint(dragMouse);
		if (dragMouse.h > mAnchorPoint.h)
			dragMouse.h--;
		if (dragMouse.v > mAnchorPoint.v)
			dragMouse.v--;
	}

	Boolean lockVertical = mConstrainVert;
	Boolean lockHorizontal = mConstrainHoriz;

	if (shiftConstrain) {
		
		// Shift key is down, decide constraint based on mouse motion.
		
		SInt16 dv = dragMouse.v - mMouseDownLocal.v;
		SInt16 dh = dragMouse.h - mMouseDownLocal.h;
		if (dv < 0)
			dv = -dv;
		if (dh < 0)
			dh = -dh;

		if (dh >= dv)
			lockVertical = true;
		else
			lockHorizontal = true;
	}
	
	// Do we have some sort of constraint?
	
	if (lockHorizontal)
		dragMouse.h = mMouseDownLocal.h - mMouseDelta.width;
	if (lockVertical)
		dragMouse.v = mMouseDownLocal.v - mMouseDelta.height;

	// Convert everything back to local coordinates of the layout view.

	dragMouse.h += mTopLeftSlop.width;
	dragMouse.v += mTopLeftSlop.height;
	ioMouseLocal = dragMouse;

}


LView*
VEResizeReceiver::GetTargetView() const
{
	return &mLayoutView;
}