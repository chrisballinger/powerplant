// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxApplicationEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxApplicationEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	AppActivatedDoer

OSStatus
AppActivatedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	WindowRef	clickedWindow = nil;
	
	SysEventParam::GetOptional(ioEvent, kEventParamWindowRef, clickedWindow);
										
	return DoAppActivated(ioEvent, clickedWindow);
}


// ===========================================================================
//	AppDeactivatedDoer

OSStatus
AppDeactivatedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoAppDeactivated(ioEvent);
}


// ===========================================================================
//	AppQuitDoer

OSStatus
AppQuitDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoAppQuit(ioEvent);
}


// ===========================================================================
//	AppLaunchNotificationDoer

OSStatus
AppLaunchNotificationDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ProcessSerialNumber		thePSN;
	UInt32					launchRefCon;
	OSStatusType			launchErr;

	SysEventParam::Get(ioEvent, kEventParamProcessID, thePSN);
	SysEventParam::Get(ioEvent, kEventParamLaunchRefCon, launchRefCon);
	SysEventParam::Get(ioEvent, kEventParamLaunchErr, launchErr);

	return DoAppLaunchNotification(ioEvent, thePSN, launchRefCon, launchErr);
}


// ===========================================================================
//	AppLaunchedDoer

OSStatus
AppLaunchedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ProcessSerialNumber		thePSN;

	SysEventParam::Get(ioEvent, kEventParamProcessID, thePSN);

	return DoAppLaunched(ioEvent, thePSN);
}


// ===========================================================================
//	AppTerminatedDoer

OSStatus
AppTerminatedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ProcessSerialNumber		thePSN;

	SysEventParam::Get(ioEvent, kEventParamProcessID, thePSN);

	return DoAppTerminated(ioEvent, thePSN);
}


// ===========================================================================
//	AppFrontSwitchedDoer

OSStatus
AppFrontSwitchedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ProcessSerialNumber		thePSN;

	SysEventParam::Get(ioEvent, kEventParamProcessID, thePSN);

	return DoAppFrontSwitched(ioEvent, thePSN);
}


// ===========================================================================
//	AppFocusMenuBarDoer

OSStatus
AppFocusMenuBarDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UInt32		keyModifiers;

	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);

	return DoAppFocusMenuBar(ioEvent, keyModifiers);
}


// ===========================================================================
//	AppFocusNextDocumentWindowDoer

OSStatus
AppFocusNextDocumentWindowDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UInt32		keyModifiers;

	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);

	return DoAppFocusNextDocumentWindow(ioEvent, keyModifiers);
}


// ===========================================================================
//	AppFocusNextFloatingWindowDoer

OSStatus
AppFocusNextFloatingWindowDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UInt32		keyModifiers;

	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);

	return DoAppFocusNextFloatingWindow(ioEvent, keyModifiers);
}


// ===========================================================================
//	AppFocusToolbarDoer

OSStatus
AppFocusToolbarDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UInt32		keyModifiers;

	SysEventParam::Get(ioEvent, kEventParamKeyModifiers, keyModifiers);

	return DoAppFocusToolbar(ioEvent, keyModifiers);
}


// ===========================================================================
//	AppGetDockTileMenuDoer

OSStatus
AppGetDockTileMenuDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef		menu;

	OSStatus	status = DoAppGetDockTileMenu(ioEvent, menu);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamMenuRef, menu);
	}
	
	return status;
}


// ===========================================================================
//	AppHiddenDoer

OSStatus
AppHiddenDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoAppHidden(ioEvent);
}


// ===========================================================================
//	AppShownDoer

OSStatus
AppShownDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoAppShown(ioEvent);
}


// ===========================================================================
//	AppSystemUIModeChangedDoer

OSStatus
AppSystemUIModeChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UInt32		mode;
	
	SysEventParam::Get(ioEvent, kEventParamSystemUIMode, mode);

	return DoAppSystemUIModeChanged(ioEvent, mode);
}


} // namespace PPx