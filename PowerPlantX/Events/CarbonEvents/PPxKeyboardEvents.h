// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxKeyboardEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxKeyboardEvents.h
	@brief		Event handlers for keyboard Carbon Events
*/

#ifndef H_PPxKeyboardEvents
#define H_PPxKeyboardEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles a key being pressed */

class	RawKeyDownDoer : public SpecificEventDoer<
									kEventClassKeyboard,
									kEventRawKeyDown> {
protected:
	virtual OSStatus	DoRawKeyDown(
								SysCarbonEvent&	ioEvent,
								char			inCharacter,
								UInt32			inKeyCode,
								UInt32			inKeyModifiers,
								UInt32			inKeyboardType) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a key being held down */

class	RawKeyRepeatDoer : public SpecificEventDoer<
									kEventClassKeyboard,
									kEventRawKeyRepeat> {
protected:
	virtual OSStatus	DoRawKeyRepeat(
								SysCarbonEvent&	ioEvent,
								char			inCharacter,
								UInt32			inKeyCode,
								UInt32			inKeyModifiers,
								UInt32			inKeyboardType) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a key being released */

class	RawKeyUpDoer : public SpecificEventDoer<
									kEventClassKeyboard,
									kEventRawKeyUp> {
protected:
	virtual OSStatus	DoRawKeyUp(
								SysCarbonEvent&	ioEvent,
								char			inCharacter,
								UInt32			inKeyCode,
								UInt32			inKeyModifiers,
								UInt32			inKeyboardType) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles change in what modifier keys are pressed */

class	RawKeyModifiersChangedDoer : public SpecificEventDoer<
												kEventClassKeyboard,
												kEventRawKeyModifiersChanged> {
protected:
	virtual OSStatus	DoRawKeyModifiersChanged(
								SysCarbonEvent&	ioEvent,
								UInt32			inKeyModifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a hot key being pressed */

class	HotKeyPressedDoer : public SpecificEventDoer<
										kEventClassKeyboard,
										kEventHotKeyPressed> {
protected:
	virtual OSStatus	DoHotKeyPressed(
								SysCarbonEvent&			ioEvent,
								const EventHotKeyID&	inHotKey) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a hot key being released */

class	HotKeyReleasedDoer : public SpecificEventDoer<
										kEventClassKeyboard,
										kEventHotKeyReleased> {
protected:
	virtual OSStatus	DoHotKeyReleased(
								SysCarbonEvent&			ioEvent,
								const EventHotKeyID&	inHotKey) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxKeyboardEvents
