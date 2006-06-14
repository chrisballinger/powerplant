// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxKeyboardEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxKeyboardEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	RawKeyDownDoer

OSStatus
RawKeyDownDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	char		theChar;
	UInt32		keyCode;
	UInt32		keyModifiers;
	UInt32		keyboardType;
	
	SysEventParam::Get(ioEvent, kEventParamKeyMacCharCodes, theChar);
	SysEventParam::Get(ioEvent, kEventParamKeyCode, keyCode);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::Get(ioEvent, kEventParamKeyboardType, keyboardType);
										
	return DoRawKeyDown(ioEvent, theChar, keyCode, keyModifiers, keyboardType);
}


// ===========================================================================
//	RawKeyRepeatDoer

OSStatus
RawKeyRepeatDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	char		theChar;
	UInt32		keyCode;
	UInt32		keyModifiers;
	UInt32		keyboardType;
	
	SysEventParam::Get(ioEvent, kEventParamKeyMacCharCodes, theChar);
	SysEventParam::Get(ioEvent, kEventParamKeyCode, keyCode);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::Get(ioEvent, kEventParamKeyboardType, keyboardType);
										
	return DoRawKeyRepeat(ioEvent, theChar, keyCode, keyModifiers, keyboardType);
}


// ===========================================================================
//	RawKeyUpDoer

OSStatus
RawKeyUpDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	char		theChar;
	UInt32		keyCode;
	UInt32		keyModifiers;
	UInt32		keyboardType;
	
	SysEventParam::Get(ioEvent, kEventParamKeyMacCharCodes, theChar);
	SysEventParam::Get(ioEvent, kEventParamKeyCode, keyCode);
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::Get(ioEvent, kEventParamKeyboardType, keyboardType);
										
	return DoRawKeyUp(ioEvent, theChar, keyCode, keyModifiers, keyboardType);
}


// ===========================================================================
//	RawKeyModifiersChangedDoer

OSStatus
RawKeyModifiersChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UInt32		keyModifiers;
	
	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);
										
	return DoRawKeyModifiersChanged(ioEvent, keyModifiers);
}


// ===========================================================================
//	HotKeyPressedDoer

OSStatus
HotKeyPressedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	EventHotKeyID		hotKeyID;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, hotKeyID);
										
	return DoHotKeyPressed(ioEvent, hotKeyID);
}


// ===========================================================================
//	HotKeyReleasedDoer

OSStatus
HotKeyReleasedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	EventHotKeyID		hotKeyID;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, hotKeyID);
										
	return DoHotKeyReleased(ioEvent, hotKeyID);
}


} // namespace PPx
