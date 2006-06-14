// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCommandEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxCommandEvents.h
	@brief		Event handlers for command Carbon Events
*/

#ifndef H_PPxCommandEvents
#define H_PPxCommandEvents
#pragma once

#include <PPxEventDoer.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	CommandProcessDoer
/**
	Handles HICommands */

class	CommandProcessDoer : public SpecificEventDoer<
										kEventClassCommand,
										kEventCommandProcess> {
protected:
	virtual OSStatus	DoCommandProcess(
								SysCarbonEvent&	ioEvent,
								HICommand		inCommand,
								UInt32			inKeyModifiers,
								UInt32			inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
//	CommandUpdateStatusDoer
/**
	Handles updating the status of items that invoke commands */

class	CommandUpdateStatusDoer : public SpecificEventDoer<
											kEventClassCommand,
											kEventCommandUpdateStatus> {
protected:
	virtual OSStatus	DoCommandUpdateStatus(
								SysCarbonEvent&	ioEvent,
								HICommand		inCommand,
								UInt32			inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
//	CommandConverter
/**
	Handles processing and updating command events by converting them into
	events for specific commands											*/

class	CommandConverter : public CommandProcessDoer,
						   public CommandUpdateStatusDoer {
public:
	void				Install( EventTargetRef inTarget );

protected:
	virtual OSStatus	DoCommandProcess(
								SysCarbonEvent&	ioEvent,
								HICommand		inCommand,
								UInt32			inKeyModifiers,
								UInt32			inMenuContext);

	virtual OSStatus	DoCommandUpdateStatus(
								SysCarbonEvent&	ioEvent,
								HICommand		inCommand,
								UInt32			inMenuContext);
};


// ===========================================================================
/**	Template which creates a unique type for a literal command ID value */

template <UInt32 TCommandID>
struct CommandIDType {
	enum { value = TCommandID };
};


// ===========================================================================
//	SpecificCommandDoer
/**
	Handles processing a specific command */

template <UInt32 TCommandID>
class	SpecificCommandDoer : public SpecificEventDoer<
										eventClass_ProcessCommand,
										TCommandID> {
protected:	
	virtual OSStatus	DoSpecificCommand(
								CommandIDType<TCommandID>,
								SysCarbonEvent&		ioEvent) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ---------------------------------------------------------------------------

template <UInt32 TCommandID>
OSStatus
SpecificCommandDoer<TCommandID>::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoSpecificCommand(CommandIDType<TCommandID>(), ioEvent);
}


// ===========================================================================
//	SpecificCommandStatusDoer
/**
	Handles updating the status of a specific command */

template <UInt32 TCommandID>
class	SpecificCommandStatusDoer : public SpecificEventDoer<
												eventClass_UpdateCmdStatus,
												TCommandID> {
protected:	
	virtual OSStatus	DoSpecificCommandStatus(
								CommandIDType<TCommandID>,
								SysCarbonEvent&		ioEvent) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ---------------------------------------------------------------------------

template <UInt32 TCommandID>
OSStatus
SpecificCommandStatusDoer<TCommandID>::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoSpecificCommandStatus(CommandIDType<TCommandID>(), ioEvent);
}


// ===========================================================================
//	CommandHandler
/**
	Handles processing and updating the status of a specific command */

template <UInt32 TCommandID>
class	CommandHandler : public SpecificCommandDoer<TCommandID>,
						 public SpecificCommandStatusDoer<TCommandID> {
public:
	void				Install( EventTargetRef inTarget )
						{
							SpecificCommandDoer<TCommandID>::Install(inTarget);
							SpecificCommandStatusDoer<TCommandID>::Install(inTarget);
						}
};


// ===========================================================================
//	SpecificMenuCommandEnableDoer
/**
	Always enables a specific menu command */

template <UInt32 TCommandID>
class	SpecificMenuCommandEnableDoer : public SpecificEventDoer<
												eventClass_UpdateCmdStatus,
												TCommandID> {
private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ---------------------------------------------------------------------------

template <UInt32 TCommandID>
OSStatus
SpecificMenuCommandEnableDoer<TCommandID>::DoEvent(
	SysCarbonEvent&	/* ioEvent */)
{
	::EnableMenuCommand(nil, TCommandID);
	return noErr;
}


// ===========================================================================
//	SpecificMenuCommandDoer
/**
	Handles processing a specific menu command that is always enabled
	when the object is in the current focus chain							*/

template <UInt32 TCommandID>
class	SpecificMenuCommandDoer : public SpecificCommandDoer<TCommandID>,
								  public SpecificMenuCommandEnableDoer<TCommandID> {
public:
	void				Install( EventTargetRef inTarget )
						{
							SpecificCommandDoer<TCommandID>::Install(inTarget);
							SpecificMenuCommandEnableDoer<TCommandID>::Install(inTarget);
						}
};

} // namespace PPx

#endif	// H_PPxCommandEvents
