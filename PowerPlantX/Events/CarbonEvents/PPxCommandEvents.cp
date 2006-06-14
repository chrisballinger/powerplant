// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCommandEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxCommandEvents.h>
#include <PPxEventUtils.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	CommandProcessDoer

OSStatus
CommandProcessDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HICommand		command;
	UInt32			keyModifiers = attributes_None;
	UInt32			menuContext  = attributes_None;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, command);
										
	SysEventParam::GetOptional(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::GetOptional(ioEvent, kEventParamMenuContext, menuContext);
										
	return DoCommandProcess(ioEvent, command, keyModifiers, menuContext);
}


// ===========================================================================
//	CommandUpdateStatusDoer

OSStatus
CommandUpdateStatusDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HICommand		command;
	UInt32			menuContext = attributes_None;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, command);
										
	SysEventParam::GetOptional(ioEvent, kEventParamMenuContext, menuContext);
										
	return DoCommandUpdateStatus(ioEvent, command, menuContext);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Install
/**
	Installs handlers for command and update command status events
	
	@param	inTarget	Event target for which to install handlers			*/

void
CommandConverter::Install(
	EventTargetRef	inTarget)
{
	CommandProcessDoer::Install(inTarget);
	CommandUpdateStatusDoer::Install(inTarget);
}


// ---------------------------------------------------------------------------
//	DoCommandProcess
/**
	Handles a command process CarbonEvent by relaying it as an event
	for a specific command													*/

OSStatus
CommandConverter::DoCommandProcess(
	SysCarbonEvent&		/* ioEvent */,
	HICommand			inCommand,
	UInt32				inKeyModifiers,
	UInt32				inMenuContext)
{
		// ProcessCommandID sends a CarbonEvent that has a special
		// event class (eventClass_ProcessCommand) and an event kind
		// that is the command ID

	return EventUtils::ProcessCommandID(inCommand, inKeyModifiers,
											inMenuContext);
}


// ---------------------------------------------------------------------------
//	DoCommandUpdateStatus
/**
	Handles a command update status CarbonEvent by relaying it as an
	event for a specific command											*/

OSStatus
CommandConverter::DoCommandUpdateStatus(
	SysCarbonEvent&		/* ioEvent */,
	HICommand			inCommand,
	UInt32				inMenuContext)
{
		// UpdateCommandID sends a CarbonEvent that has a special
		// event class (eventClass_UpdateCmdStatus) and an event kind
		// that is the command ID

	if (EventUtils::UpdateCommandID(inCommand, inMenuContext) != noErr) {
	
			// No one handled the event. If it's a menu command,
			// disable it.
	
		if (inCommand.attributes & kHICommandFromMenu) {
			::DisableMenuCommand(nil, inCommand.commandID);
		}
	}
	
	return noErr;
}


} // namespace PPx