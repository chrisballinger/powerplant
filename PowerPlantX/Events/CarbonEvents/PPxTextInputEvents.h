// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTextInputEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxTextInputEvents.h
	@brief		Event handlers for text input Carbon Events
*/
	
#ifndef H_PPxTextInputEvents
#define H_PPxTextInputEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Updates contents of a text input area */

class	TextInputUpdateActiveInputAreaDoer : public SpecificEventDoer<
									kEventClassTextInput,
									kEventTextInputUpdateActiveInputArea> {
protected:
	virtual OSStatus	DoTextInputUpdateActiveInputArea( SysCarbonEvent& ioEvent ) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles unicode text input from the keyboard */

class	TextInputUnicodeForKeyEventDoer : public SpecificEventDoer<
									kEventClassTextInput,
									kEventTextInputUnicodeForKeyEvent> {
protected:
	virtual OSStatus	DoTextInputUnicodeForKeyEvent(
								SysCarbonEvent&		ioEvent,
								UInt32				inUniCharCount,
								const UniChar*		inUniChars) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Converts from inline session text offset to global QD point */

class	TextInputOffsetToPosDoer : public SpecificEventDoer<
									kEventClassTextInput,
									kEventTextInputOffsetToPos> {
protected:
	virtual OSStatus	DoTextInputOffsetToPos(
								SysCarbonEvent&		ioEvent,
								SInt32				inTextOffset,
								Point&				outGlobalPoint) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Converts from global QD point to inline session text offset */

class	TextInputPosToOffsetDoer : public SpecificEventDoer<
									kEventClassTextInput,
									kEventTextInputPosToOffset> {
protected:
	virtual OSStatus	DoTextInputPosToOffset(
								SysCarbonEvent&		ioEvent,
								const Point&		inGlobalPoint,
								SInt32&				outRegionClass,
								SInt32&				outTextOffset) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Shows or hides the bottom line input window */

class	TextInputShowHideBottomWindowDoer : public SpecificEventDoer<
									kEventClassTextInput,
									kEventTextInputShowHideBottomWindow> {
protected:
	virtual OSStatus	DoTextInputShowHideBottomWindow(
								SysCarbonEvent&		ioEvent)  = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the selected text */

class	TextInputGetSelectedTextDoer : public SpecificEventDoer<
									kEventClassTextInput,
									kEventTextInputGetSelectedText> {
protected:
	virtual OSStatus	DoTextInputGetSelectedText(
								SysCarbonEvent&		ioEvent) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Inputs unicode text  */

class	TextInputUnicodeTextDoer : public SpecificEventDoer<
									kEventClassTextInput,
									kEventTextInputUnicodeText> {
protected:
	virtual OSStatus	DoTextInputUnicodeText(
								SysCarbonEvent&		ioEvent,
								UniChar				inUniChar) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxTextInputEvents
