// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxApplicationEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxApplicationEvents.h
	@brief		Event handlers for application Carbon Events
*/

#ifndef H_PPxApplicationEvents
#define H_PPxApplicationEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles notification that an application has resumed */

class	AppActivatedDoer : public SpecificEventDoer<
									kEventClassApplication,
									kEventAppActivated> {
protected:
	virtual OSStatus	DoAppActivated(
								SysCarbonEvent&	ioEvent,
								WindowRef		inClickedWindow ) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that an application has suspended */

class	AppDeactivatedDoer : public SpecificEventDoer<
										kEventClassApplication,
										kEventAppDeactivated> {
protected:
	virtual OSStatus	DoAppDeactivated( SysCarbonEvent& ioEvent ) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to quit an application */

class	AppQuitDoer : public SpecificEventDoer<
								kEventClassApplication,
								kEventAppQuit> {
protected:
	virtual OSStatus	DoAppQuit( SysCarbonEvent& ioEvent ) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that an application we launched asynchronously
	has actually launched													*/

class	AppLaunchNotificationDoer : public SpecificEventDoer<
										kEventClassApplication,
										kEventAppLaunchNotification> {
protected:
	virtual OSStatus	DoAppLaunchNotification(
								SysCarbonEvent&				ioEvent,
								const ProcessSerialNumber&	inPSN,
								UInt32						inLaunchRefCon,
								OSStatus					inLaunchError) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that another application has launched */

class	AppLaunchedDoer : public SpecificEventDoer<
									kEventClassApplication,
									kEventAppLaunched> {
protected:
	virtual OSStatus	DoAppLaunched(
								SysCarbonEvent&				ioEvent,
								const ProcessSerialNumber&	inPSN) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that another application has terminated */

class	AppTerminatedDoer : public SpecificEventDoer<
										kEventClassApplication,
										kEventAppTerminated> {
protected:
	virtual OSStatus	DoAppTerminated(
								SysCarbonEvent&				ioEvent,
								const ProcessSerialNumber&	inPSN) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that the active application has changed */

class	AppFrontSwitchedDoer : public SpecificEventDoer<
										kEventClassApplication,
										kEventAppFrontSwitched> {
protected:
	virtual OSStatus	DoAppFrontSwitched(
								SysCarbonEvent&				ioEvent,
								const ProcessSerialNumber&	inPSN) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles request to set the keyboard focus to the menu bar */

class	AppFocusMenuBarDoer : public SpecificEventDoer<
										kEventClassApplication,
										kEventAppFocusMenuBar> {
protected:
	virtual OSStatus	DoAppFocusMenuBar(
								SysCarbonEvent&		ioEvent,
								UInt32				inKeyModifiers) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles request to set the keyboard focus to the next document window
*/

class	AppFocusNextDocumentWindowDoer : public SpecificEventDoer<
													kEventClassApplication,
								 					kEventAppFocusNextDocumentWindow> {
protected:
	virtual OSStatus	DoAppFocusNextDocumentWindow(
								SysCarbonEvent&		ioEvent,
								UInt32				inKeyModifiers) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles request to set the keyboard focus to the next floating window */

class	AppFocusNextFloatingWindowDoer : public SpecificEventDoer<
													kEventClassApplication,
								 					kEventAppFocusNextFloatingWindow> {
protected:
	virtual OSStatus	DoAppFocusNextFloatingWindow(
								SysCarbonEvent&		ioEvent,
								UInt32				inKeyModifiers) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles request to set the keyboard focus to the toolbar in the
	currently focused window												*/

class	AppFocusToolbarDoer : public SpecificEventDoer<
										kEventClassApplication,
										kEventAppFocusToolbar> {
protected:
	virtual OSStatus	DoAppFocusToolbar(
								SysCarbonEvent&		ioEvent,
								UInt32				inKeyModifiers) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the menu to display from an application's dock tile */

class	AppGetDockTileMenuDoer : public SpecificEventDoer<
											kEventClassApplication,
											kEventAppGetDockTileMenu> {
protected:
	virtual OSStatus	DoAppGetDockTileMenu(
								SysCarbonEvent&		ioEvent,
								MenuRef&			outMenu) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that an application has been hidden */

class	AppHiddenDoer : public SpecificEventDoer<
									kEventClassApplication,
									kEventAppHidden> {
protected:
	virtual OSStatus	DoAppHidden( SysCarbonEvent& ioEvent ) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that an application has been shown */

class	AppShownDoer : public SpecificEventDoer<
								kEventClassApplication,
								kEventAppShown> {
protected:
	virtual OSStatus	DoAppShown( SysCarbonEvent& ioEvent ) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that the system UI mode of the front application
	has changed																*/

class	AppSystemUIModeChangedDoer : public SpecificEventDoer<
												kEventClassApplication,
								 				kEventAppSystemUIModeChanged> {
protected:
	virtual OSStatus	DoAppSystemUIModeChanged(
								SysCarbonEvent&		ioEvent,
								UInt32				inUIMode) = 0;
								
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxApplicationEvents
