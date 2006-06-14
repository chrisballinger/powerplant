// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMouseEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxMouseEvents.h
	@brief		Event handlers for mouse Carbon Events
*/

#ifndef H_PPxMouseEvents
#define H_PPxMouseEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles the mouse button being pressed */

class	MouseDownDoer : public SpecificEventDoer<
									kEventClassMouse,
									kEventMouseDown> {
protected:	
	virtual OSStatus	DoMouseDown(
								SysCarbonEvent& ioEvent,
								const HIPoint&	inMouseLoc) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the mouse button being released */

class	MouseUpDoer : public SpecificEventDoer<
								kEventClassMouse,
								kEventMouseUp> {
protected:	
	virtual OSStatus	DoMouseUp(
								SysCarbonEvent& ioEvent,
								const HIPoint&	inMouseLoc) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the mouse button being moved */

class	MouseMovedDoer : public SpecificEventDoer<
									kEventClassMouse,
									kEventMouseMoved> {
protected:	
	virtual OSStatus	DoMouseMoved(
								SysCarbonEvent& ioEvent,
								const HIPoint&	inMouseLoc) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the mouse button being moved while the button is down */

class	MouseDraggedDoer : public SpecificEventDoer<
									kEventClassMouse,
									kEventMouseDragged> {
protected:	
	virtual OSStatus	DoMouseDragged(
								SysCarbonEvent& ioEvent,
								const HIPoint&	inMouseLoc) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the mouse entering a tracking area */

class	MouseEnteredDoer : public SpecificEventDoer<
									kEventClassMouse,
									kEventMouseEntered> {
protected:	
	virtual OSStatus	DoMouseEntered(
								SysCarbonEvent& 	ioEvent,
								MouseTrackingRef	inTrackingRef,
								WindowRef			inWindowRef,
								const HIPoint&		inMouseLoc,
								const HIPoint&		inWindowMouseLoc,
								UInt32				inKeyModifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the mouse leaving a tracking area */

class	MouseExitedDoer : public SpecificEventDoer<
									kEventClassMouse,
									kEventMouseExited> {
protected:	
	virtual OSStatus	DoMouseExited(
								SysCarbonEvent& 	ioEvent,
								MouseTrackingRef	inTrackingRef,
								WindowRef			inWindowRef,
								const HIPoint&		inMouseLoc,
								const HIPoint&		inWindowMouseLoc,
								UInt32				inKeyModifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the mouse wheel being moved */

class	MouseWheelMovedDoer : public SpecificEventDoer<
										kEventClassMouse,
										kEventMouseWheelMoved> {
protected:	
	virtual OSStatus	DoMouseWheelMoved(
								SysCarbonEvent& 	ioEvent,
								WindowRef			inWindowRef,
								const HIPoint&		inMouseLoc,
								const HIPoint&		inWindowMouseLoc,
								UInt32				inKeyModifiers,
								EventMouseWheelAxis	inWheelAxis,
								SInt32				inWheelDelta) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxMouseEvents
