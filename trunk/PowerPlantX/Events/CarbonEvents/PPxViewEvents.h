// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxViewEvents.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:16 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxViewEvents.h
	@brief		Event handlers for view Carbon Events (kEventClassControl)
*/

#ifndef H_PPxViewEvents
#define H_PPxViewEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles a control being disposed */

class	ControlDisposeDoer : public SpecificEventDoer<
										kEventClassControl,
										kEventControlDispose> {
protected:
	virtual OSStatus	DoControlDispose(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a click in a control. A click occurs when a mouse down and
	subsequent mouse up are within the same part of a control.				*/

class	ControlHitDoer : public SpecificEventDoer<
									kEventClassControl,
									kEventControlHit> {
protected:
	virtual OSStatus	DoControlHit(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								ControlPartCode		inPartCode,
								UInt32				inKeyModifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a simulating a click in a control */

class	ControlSimulateHitDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlSimulateHit> {
protected:
	virtual OSStatus	DoControlSimulateHit(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								UInt32				inKeyModifiers,
								ControlPartCode&	ioPartCode) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles testing whether a point is within a control */

class	ControlHitTestDoer : public SpecificEventDoer<
										kEventClassControl,
										kEventControlHitTest> {
protected:
	virtual OSStatus	DoControlHitTest(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								const HIPoint&		inHitPoint,
								ControlPartCode&	outPartCode) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles drawing a control */

class	ControlDrawDoer : public SpecificEventDoer<
									kEventClassControl,
									kEventControlDraw> {
protected:
	virtual OSStatus	DoControlDraw(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								ControlPartCode		inPartCode,
								RgnHandle			inClipRgn,
								CGContextRef		inContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles applying a control's background to a port */

class	ControlApplyBackgroundDoer : public SpecificEventDoer<
												kEventClassControl,
												kEventControlApplyBackground> {
protected:
	virtual OSStatus	DoControlApplyBackground(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								ControlRef			inSubControl,
								SInt16				inDrawDepth,
								bool				inDrawInColor) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles applying a control's text color to a port/context */

class	ControlApplyTextColorDoer : public SpecificEventDoer<
												kEventClassControl,
												kEventControlApplyTextColor> {
protected:
	virtual OSStatus	DoControlApplyTextColor(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								ControlRef			inSubControl,
								SInt16				inDrawDepth,
								bool				inDrawInColor,
								CGContextRef		inContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles setting the focus to a part of a control */

class	ControlSetFocusPartDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlSetFocusPart> {
protected:
	virtual OSStatus	DoControlSetFocusPart(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								bool				inFocusEverything,
								ControlPartCode&	ioFocusPart) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the currently focused part of a control */

class	ControlGetFocusPartDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlGetFocusPart> {
protected:
	virtual OSStatus	DoControlGetFocusPart(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								ControlPartCode&	outFocusPart) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a control becoming active */

class	ControlActivateDoer : public SpecificEventDoer<
										kEventClassControl,
										kEventControlActivate> {
protected:
	virtual OSStatus	DoControlActivate(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a control becoming inactive */

class	ControlDeactivateDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlDeactivate> {
protected:
	virtual OSStatus	DoControlDeactivate(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles setting the cursor when the mouse is inside a control */

class	ControlSetCursorDoer : public SpecificEventDoer<
										kEventClassControl,
										kEventControlSetCursor> {
protected:
	virtual OSStatus	DoControlSetCursor(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								const HIPoint&	inMouseLocation,
								UInt32			inKeyModifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a mouse down event inside a control */

class	ControlClickDoer : public SpecificEventDoer<
									kEventClassControl,
									kEventControlClick> {
protected:
	virtual OSStatus	DoControlClick(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								const HIPoint&	inMouseLocation) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles mouse down tracking inside a control */

class	ControlTrackDoer : public SpecificEventDoer<
									kEventClassControl,
									kEventControlTrack> {
protected:
	virtual OSStatus	DoControlTrack(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								const HIPoint&		inMouseLocation,
								UInt32				inKeyModifiers,
								ControlActionUPP	inActionUPP,
								ControlPartCode&	outPartCode) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drag entering a control */

class	ControlDragEnterDoer : public SpecificEventDoer<
										kEventClassControl,
										kEventControlDragEnter> {
protected:
	virtual OSStatus	DoControlDragEnter(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								DragRef			inDragRef) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drag remaining inside a control */

class	ControlDragWithinDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlDragWithin> {
protected:
	virtual OSStatus	DoControlDragWithin(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								DragRef			inDragRef) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drag leaving a control */

class	ControlDragLeaveDoer : public SpecificEventDoer<
										kEventClassControl,
										kEventControlDragLeave> {
protected:
	virtual OSStatus	DoControlDragLeave(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								DragRef			inDragRef) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drag being dropped in a control */

class	ControlDragReceiveDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlDragReceive> {
protected:
	virtual OSStatus	DoControlDragReceive(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								DragRef			inDragRef) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the bounding region of a control part */

class	ControlGetPartRegionDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlGetPartRegion> {
protected:
	virtual OSStatus	DoControlGetPartRegion(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								ControlPartCode	inPartCode,
								RgnHandle		inRegion) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the bounding rectangle of a control part */

class	ControlGetPartBoundsDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlGetPartBounds> {
protected:
	virtual OSStatus	DoControlGetPartBounds(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								ControlPartCode	inPartCode,
								HIRect&			outBounds) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the optimal bounds for a control */

class	ControlGetOptimalBoundsDoer : public SpecificEventDoer<
												kEventClassControl,
												kEventControlGetOptimalBounds> {
protected:
	virtual OSStatus	DoControlGetOptimalBounds(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								HIRect&			outBounds,
								SInt16&			outBaseline) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the minimum and maximum sizes for a control */

class	ControlGetSizeConstraintsDoer : public SpecificEventDoer<
													kEventClassControl,
													kEventControlGetSizeConstraints> {
protected:
	virtual OSStatus	DoControlGetSizeConstraints(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								HISize&			outMinSize,
								HISize&			outMaxSize) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification when the value, minimum value, maximum value,
	or view size of a control changes										*/

class	ControlValueFieldChangedDoer : public SpecificEventDoer<
												kEventClassControl,
												kEventControlValueFieldChanged> {
protected:
	virtual OSStatus	DoControlValueFieldChanged( 
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification when the hilite state of a control changes */

class	ControlHiliteChangedDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlHiliteChanged> {
protected:
	virtual OSStatus	DoControlHiliteChanged(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles adapting to a change in the bounds of a control */

class	ControlBoundsChangedDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlBoundsChanged> {
protected:
	virtual OSStatus	DoControlBoundsChanged(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								UInt32			inChangeAttributes,
								const HIRect&	inOriginalBounds,
								const HIRect&	inCurrentBounds) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification when the title of a control changes */

class	ControlTitleChangedDoer : public SpecificEventDoer<
											kEventClassControl,
											kEventControlTitleChanged> {
protected:
	virtual OSStatus	DoControlTitleChanged(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification when a control is enabled or disabled */

class	ControlEnabledStateChangedDoer : public SpecificEventDoer<
													kEventClassControl,
													kEventControlEnabledStateChanged> {
protected:
	virtual OSStatus	DoControlEnabledStateChanged(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification when a control moves into a different window */

class	ControlOwningWindowChangedDoer : public SpecificEventDoer<
													kEventClassControl,
													kEventControlOwningWindowChanged> {
protected:
	virtual OSStatus	DoControlOwningWindowChanged(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								UInt32			inAttributes,
								WindowRef		inFormerWindow,
								WindowRef		inCurrentWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification when a subcontrol is added */

class	ControlAddedSubControlDoer : public SpecificEventDoer<
												kEventClassControl,
												kEventControlAddedSubControl> {
protected:
	virtual OSStatus	DoControlAddedSubControl(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								ControlRef		inSubControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification when a subcontrol is being removed */

class	ControlRemovingSubControlDoer : public SpecificEventDoer<
													kEventClassControl,
													kEventControlRemovingSubControl> {
protected:
	virtual OSStatus	DoControlRemovingSubControl(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								ControlRef		inSubControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles old-style CDEF messages */

class	ControlArbitraryMessageDoer : public SpecificEventDoer<
												kEventClassControl,
												kEventControlArbitraryMessage> {
protected:
	virtual OSStatus	DoControlArbitraryMessage(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								SInt16			inMessage,
								SInt32			inParameter,
								SInt32&			outResult) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxViewEvents
