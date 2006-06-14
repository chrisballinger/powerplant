// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventUtils.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxEventUtils.h>
#include <SysCarbonEvent.h>
#include <SysEventParam.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SetMenuCommandStatus
/**
	Enables or disables the menu item with a certain command ID
	
	@param	inCommand		Command ID number of a menu item
	@param	inStatus		true = enable, false = disable					*/

void
EventUtils::SetMenuCommandStatus(
	MenuCommand		inCommand,
	bool			inStatus)
{
	if (inStatus) {
		::EnableMenuCommand(nil, inCommand);
	} else {
		::DisableMenuCommand(nil, inCommand);
	}
}


// ---------------------------------------------------------------------------
//	ProcessCommandID
/**
	Sends a CarbonEvent to process a command where the kind of event is a
	comamand ID
	
	@param	inCommand		HICommand struct for the command to process
	@param	inKeyModifiers	Keyboard modifier keys
	@param	inMenuContext	Menu context of the command

	The CarbonEvent parameters are the same as for the event
	(kEventClassCommand, kCommandProcess), but the class is
	eventClass_DoCommand and the kind is the commandID of the HICommand.
	The event gets sent to the user focus, so the event can propagate
	through the command chain.

	This gives each command a unique event signature, and clients can
	process the command by installing a custom handler for that event.
	
	@note This function should normally be called from the DoCommandProcess
	function of a CommandProcessDoer subclass to relay a generic
	command CarbonEvent as a CarbonEvent for a specific command ID.			*/

OSStatus
EventUtils::ProcessCommandID(
	HICommand	inCommand,
	UInt32		inKeyModifiers,
	UInt32		inMenuContext)
{
	SysCarbonEvent	commandEvent( eventClass_ProcessCommand,
								  inCommand.commandID );
	
	SysEventParam::Set(commandEvent, kEventParamDirectObject, inCommand);
	
	if (inKeyModifiers != attributes_None) {
		SysEventParam::Set(commandEvent, kEventParamKeyModifiers, inKeyModifiers);
	}
	
	if (inMenuContext != attributes_None) {
		SysEventParam::Set(commandEvent, kEventParamMenuContext, inMenuContext);
	}
	
	return commandEvent.SendTo(::GetUserFocusEventTarget());
}


// ---------------------------------------------------------------------------
//	UpdateCommandID
/**
	Sends a CarbonEvent to update the status of a command where the kind of
	event is command ID
	
	@param	inCommand		HICommand struct for the command to process
	@param	inMenuContext	Menu context of the command
	
	The CarbonEvent parameters are the same as for the event
	(kEventClassCommand, kEventCommandUpdateStatus), but the class is
	eventClass_UpdateCmdStatus and the kind is the commandID of the HICommand.
	The event gets sent to the user focus, so the event can propagate
	through the command chain.

	This gives each command a unique event signature, and clients can
	update the command status by installing a custom handler for that event.
	
	@note This function should normally be called from th
	DoCommandUpdateStatus function of a CommandUpdateStatusDoer subclass to
	relay a generic update command CarbonEvent as a CarbonEvent to update a
	specific command ID.													*/
	
OSStatus
EventUtils::UpdateCommandID(
	HICommand	inCommand,
	UInt32		inMenuContext)
{
	SysCarbonEvent	updateEvent( eventClass_UpdateCmdStatus,
								 inCommand.commandID );
	
	SysEventParam::Set(updateEvent, kEventParamDirectObject, inCommand);
	
	if (inMenuContext != attributes_None) {
		SysEventParam::Set(updateEvent, kEventParamMenuContext, inMenuContext);
	}
	
	return updateEvent.SendTo(::GetUserFocusEventTarget());
}


// ---------------------------------------------------------------------------
//	PostCommandID
/**
	Posts a Carbon Event for a specified command ID
	
	@param	inCommandID		Command ID number
	@param	inTarget		Target for receiving the event, may be nil
	@param	inQueueRef		Event queue in which to post the event, may be nil
	@param	inPriority		Event priority level
	
	Posting an event is asynchronous. The event is queued and execution
	returns immediately to the caller.
*/

void
EventUtils::PostCommandID(
	CommandIDT			inCommandID,
	EventTargetRef		inTarget,
	EventQueueRef		inQueueRef,
	EventPriority		inPriority)
{
	SysCarbonEvent	theEvent(eventClass_ProcessCommand, inCommandID);
	
	HICommand	hiCmd;
	hiCmd.attributes = 0;
	hiCmd.commandID  = inCommandID;
	
	SysEventParam::Set(theEvent, kEventParamDirectObject, hiCmd);
	
	theEvent.PostTo(inTarget, inQueueRef, inPriority);
}


// ---------------------------------------------------------------------------
//	SendCommandID
/**
	Sends a Carbon Event for a specified command ID
	
	@param	inCommandID		Command ID number
	@param	inTarget		Target for receiving the event
	@param	inOptions		Options for sending the event
	
	@return	Status of handling the command event
	
	Sending an event is synchronous. The target receives the event
	immediately and execution does not return to the caller until the event
	has been handled.
*/

OSStatus
EventUtils::SendCommandID(
	CommandIDT			inCommandID,
	EventTargetRef		inTarget,
	OptionBits			inOptions)
{
	SysCarbonEvent	theEvent(eventClass_ProcessCommand, inCommandID);
	
	HICommand	hiCmd;
	hiCmd.attributes = 0;
	hiCmd.commandID  = inCommandID;
	
	SysEventParam::Set(theEvent, kEventParamDirectObject, hiCmd);
	
	return theEvent.SendTo(inTarget, inOptions);
}


// ---------------------------------------------------------------------------
//	SendTargetDeletedEvent
/**
	Sends a "target deleted" event to the specified target
	
	@param	inTarget	Target being deleted
	
	This is a notification event that should be sent just before deleting
	a system event target. Objects that wish to know when a target is
	deleted can install an event handler for this event.					*/

void
EventUtils::SendTargetDeletedEvent(
	EventTargetRef	inTarget)
{
	SysCarbonEvent	deletedEvent(eventClass_PPx, eventKind_TargetDeleted);
	
	SysEventParam::Set(deletedEvent, kEventParamDirectObject, inTarget);
	
	deletedEvent.SendTo(inTarget, kEventTargetSendToAllHandlers);
}


} // namespace PPx