// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventAttachments.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxEventAttachments.h
	@brief		Attachment classes for handling Carbon Events
*/

#ifndef H_PPxEventAttachments
#define H_PPxEventAttachments
#pragma once

#include <PPxAttachment.h>
#include <PPxEventDoer.h>
#include <PPxEventTarget.h>
#include <PPxHIObjectEvents.h>

namespace PPx {

// ===========================================================================
//	TargetAttachment
/**
	Abstract attachment that has an associated event target */

class	TargetAttachment : public Attachment {
public:
						TargetAttachment();
						
	virtual				~TargetAttachment();
	
	void				SetEventTarget( EventTarget* inTarget );
	
	EventTarget*		GetEventTarget() const;

protected:
	virtual void		InitState( const DataReader& inReader );

	virtual void		WriteState( DataWriter& ioWriter ) const;

private:
	EventTarget*		mTarget;
};


// ===========================================================================
//	EventDoerAttachment
/**
	Abstract attachment that has an associated event target and specific
	event type																*/

class	EventDoerAttachment : public TargetAttachment,
							  public EventDoer {
public:
						EventDoerAttachment();
						
	virtual				~EventDoerAttachment();
	
	void				Initialize(
								EventTarget*	inTarget,
								EventClassT		inEventClass,
								EventKindT		inEventKind);

	void				InstallEventHandler();
	
	void				RemoveEventHandler();

protected:
	virtual void		InitState( const DataReader& inReader );

	virtual void		WriteState( DataWriter& ioWriter ) const;

private:
	SysEventSpec		mTargetEventSpec;
	SysEventHandler		mTargetEventHandler;
};


// ===========================================================================
//	ResponseAttachment
/**
	Attachment which responds to an event by sending another event. The
	target of the response event is either the same target, the current
	user focus, or the application.											*/

class	ResponseAttachment : public EventDoerAttachment {
public:
						ResponseAttachment();
						
	virtual				~ResponseAttachment();
	
	void				Initialize(
								EventTarget*			inTarget,
								EventClassT				inEventClass,
								EventKindT				inEventKind,
								EMetaTarget				inResponseTarget,
								const SysCarbonEvent&	inResponseEvent);
	
protected:
	virtual void		InitState( const DataReader& inReader );

	virtual void		WriteState( DataWriter& ioWriter ) const;
	
	EventTargetRef		GetResponseSysEventTarget() const;

private:	
	virtual CFStringRef	ClassName() const;

	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );

private:
	SysCarbonEvent		mResponseEvent;
	EMetaTarget			mResponseTarget;
};


// ===========================================================================
//	MessageAttachment
/**
	Attachment which responds to an event by sending a message event to
	another target. The message recipient may be any event target.			*/

class	MessageAttachment : public EventDoerAttachment,
							public HIObjectDestructDoer {
public:
						MessageAttachment();
						
	virtual				~MessageAttachment();
	
	void				Initialize(
								EventTarget*			inTarget,
								EventClassT				inEventClass,
								EventKindT				inEventKind,
								EventTarget*			inMessageTarget,
								const SysCarbonEvent&	inMessageEvent);
								
	void				SetMessageTarget( EventTarget* inMessageTarget );
	
protected:
	virtual void		InitState( const DataReader& inReader );

	virtual void		WriteState( DataWriter& ioWriter ) const;

	virtual OSStatus	DoHIObjectDestruct( SysCarbonEvent& ioEvent );

private:	
	virtual CFStringRef	ClassName() const;

	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
	
private:
	SysCarbonEvent		mMessageEvent;
	EventTarget*		mMessageTarget;
	SysEventHandler		mDestructHandler;
};


} // namespace PPx

#endif	// H_PPxEventAttachments
