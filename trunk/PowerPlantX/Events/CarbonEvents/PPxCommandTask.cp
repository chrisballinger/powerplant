// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCommandTask.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxCommandTask.h>
#include <PPxSerializer.h>
#include <SysEventParam.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Key names for persistent data

namespace {
	const CFStringRef	key_CommandID	= CFSTR("command id");
	const CFStringRef	key_EventTarget	= CFSTR("event target");
}


// ---------------------------------------------------------------------------
//	CommandTask													 	 [public]
/**
	Default constructor */

CommandTask::CommandTask()
{
	mTarget		= nil;
	mCommandID	= commandID_None;
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Specifies target and command ID
	
	@param	inTarget		Target which receives command events
	@param	inCommandID		Command to handle								*/

void
CommandTask::Initialize(
	EventTarget*	inTarget,
	CommandIDT		inCommandID)
{
	mTarget		= inTarget;
	mCommandID	= inCommandID;
	
	if ( (inTarget != nil)  &&  (inCommandID != commandID_None) ) {
		
		mCommandProcessor.Install( this,
								   &CommandTask::DoCommandProcessEvent,
								   inTarget->GetSysEventTarget(),
								   eventClass_ProcessCommand,
								   inCommandID );
	}
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
CommandTask::InitState(
	const DataReader&	inReader)
{
	Attachment::InitState(inReader);

	UInt32	commandID = 0;
	inReader.ReadOptional(key_CommandID, commandID);
	
	EventTarget*	target;
	inReader.ReadObjectValue(key_EventTarget, target);
	
	Initialize(target, commandID);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
CommandTask::WriteState(
	DataWriter&	ioWriter) const
{
	Attachment::WriteState(ioWriter);

	ioWriter.WriteValue(key_CommandID, mCommandID);
	ioWriter.WriteObjectValue(key_EventTarget, mTarget);
}


// ---------------------------------------------------------------------------
//	DoCommandProcessEvent											  [public]

OSStatus
CommandTask::DoCommandProcessEvent(
	SysCarbonEvent&	ioEvent)
{
	HICommand		command;
	UInt32			keyModifiers = attributes_None;
	UInt32			menuContext  = attributes_None;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, command);
										
	SysEventParam::GetOptional(ioEvent, kEventParamKeyModifiers, keyModifiers);
	SysEventParam::GetOptional(ioEvent, kEventParamMenuContext, menuContext);
										
	return DoCommandProcess(command, keyModifiers, menuContext);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	StatusCommandTask												  [public]

StatusCommandTask::StatusCommandTask()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]

void
StatusCommandTask::Initialize(
	EventTarget*	inTarget,
	CommandIDT		inCommandID)
{
	CommandTask::Initialize(inTarget, inCommandID);

	if ( (inTarget != nil)  &&  (inCommandID != commandID_None) ) {
		
		mCommandUpdater.Install( this,
								 &StatusCommandTask::DoCommandUpdateStatusEvent,
								 inTarget->GetSysEventTarget(),
								 eventClass_UpdateCmdStatus,
								 inCommandID );
	}
}


// ---------------------------------------------------------------------------
//	DoCommandUpdateStatusEvent									  [public]

OSStatus
StatusCommandTask::DoCommandUpdateStatusEvent(
	SysCarbonEvent&	ioEvent)
{
	HICommand		command;
	UInt32			menuContext = attributes_None;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, command);
										
	SysEventParam::GetOptional(ioEvent, kEventParamMenuContext, menuContext);
										
	return DoCommandUpdateStatus(command, menuContext);
}


} // namespace PPx
