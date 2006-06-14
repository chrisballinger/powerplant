// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCommandTask.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxCommandTask.h
	@brief		Attachment classes for handling commands
*/

#ifndef H_PPxCommandTask
#define H_PPxCommandTask
#pragma once

#include <PPxAttachment.h>
#include <PPxEventTarget.h>
#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
//	CommandTask
/**
	Abstract class for an Attachment which handles a command event */

class	CommandTask : public Attachment {
public:
						CommandTask();

	void				Initialize(
								EventTarget*	inTarget,
								CommandIDT		inCommandID);
	
	OSStatus			DoCommandProcessEvent( SysCarbonEvent& ioEvent );

protected:	
	virtual void		InitState( const DataReader& inReader );

	virtual void		WriteState( DataWriter& ioWriter ) const;
	
	virtual OSStatus	DoCommandProcess(
								HICommand	inCommand,
								UInt32		inKeyModifiers,
								UInt32		inMenuContext) = 0;
	
private:
	EventTarget*					mTarget;
	CommandIDT						mCommandID;
	EventDoerCallback<CommandTask>	mCommandProcessor;
};


// ===========================================================================
//	StatusCommandTask
/**
	Abstract class for a Command event handler Attachment that also handles
	updating the status of items that invoke the command.
	
	Typically, updating the status means enabling or disabling a menu item.	*/

class	StatusCommandTask : public CommandTask {
public:
						StatusCommandTask();
						
	void				Initialize(
								EventTarget*	inTarget,
								CommandIDT		inCommandID);
	
	OSStatus			DoCommandUpdateStatusEvent( SysCarbonEvent& ioEvent );

protected:								
	virtual OSStatus	DoCommandUpdateStatus(
								HICommand	inCommand,
								UInt32		inMenuContext ) = 0;

private:
	EventDoerCallback<StatusCommandTask>	mCommandUpdater;
};


} // namespace PPx

#endif	// H_PPxCommandTask
