// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventAttachments.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxEventAttachments.h>
#include <PPxSerializer.h>

namespace PPx {

namespace {	// unnamed namespace

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_Target				= CFSTR("target");
	const CFStringRef	key_TargetEventClass	= CFSTR("target event class");
	const CFStringRef	key_TargetEventKind		= CFSTR("target event kind");
	
	const CFStringRef	key_ResponseMetaTarget	= CFSTR("response meta target");
	const CFStringRef	key_ResponseEventClass	= CFSTR("response event class");
	const CFStringRef	key_ResponseEventKind	= CFSTR("response event kind");
	
	const CFStringRef	key_MessageTarget		= CFSTR("message target");
	const CFStringRef	key_MessageEventClass	= CFSTR("message event class");
	const CFStringRef	key_MessageEventKind	= CFSTR("message event kind");
}


// ---------------------------------------------------------------------------
//	TargetAttachment												  [public]

TargetAttachment::TargetAttachment()
{
	mTarget = nil;
}


// ---------------------------------------------------------------------------
//	~TargetAttachment												  [public]

TargetAttachment::~TargetAttachment()
{
}


// ---------------------------------------------------------------------------
//	SetEventTarget												  [public]

void
TargetAttachment::SetEventTarget(
	EventTarget*	inTarget)
{
	mTarget = inTarget;
}


// ---------------------------------------------------------------------------
//	GetEventTarget												  [public]

EventTarget*
TargetAttachment::GetEventTarget() const
{
	return mTarget;
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
TargetAttachment::InitState(
	const DataReader&	inReader)
{
	Attachment::InitState(inReader);
	
	inReader.ReadObjectValue(key_Target, mTarget);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
TargetAttachment::WriteState(
	DataWriter&	ioWriter) const
{
	Attachment::WriteState(ioWriter);
	
	ioWriter.WriteObjectValue(key_Target, mTarget);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	EventDoerAttachment											  [public]

EventDoerAttachment::EventDoerAttachment()
{
}


// ---------------------------------------------------------------------------
//	~EventDoerAttachment											  [public]

EventDoerAttachment::~EventDoerAttachment()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]

void
EventDoerAttachment::Initialize(
	EventTarget*	inTarget,
	EventClassT		inEventClass,
	EventKindT		inEventKind)
{
	SetEventTarget(inTarget);
	mTargetEventSpec.eventClass = inEventClass;
	mTargetEventSpec.eventKind  = inEventKind;
	
	InstallEventHandler();
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
EventDoerAttachment::InitState(
	const DataReader&	inReader)
{
	TargetAttachment::InitState(inReader);
	
	inReader.ReadRequired(key_TargetEventClass, mTargetEventSpec.eventClass);
	inReader.ReadRequired(key_TargetEventKind, mTargetEventSpec.eventKind);
	
	InstallEventHandler();
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
EventDoerAttachment::WriteState(
	DataWriter&	ioWriter) const
{
	TargetAttachment::WriteState(ioWriter);
	
	ioWriter.WriteValue(key_TargetEventClass, mTargetEventSpec.eventClass);
	ioWriter.WriteValue(key_TargetEventKind, mTargetEventSpec.eventKind);
}


// ---------------------------------------------------------------------------
//	InstallEventHandler

void
EventDoerAttachment::InstallEventHandler()
{
	EventTarget*	target = GetEventTarget();
		
	if (target != nil) {
		mTargetEventHandler.Adopt( Install( target->GetSysEventTarget(),
											mTargetEventSpec.eventClass,
											mTargetEventSpec.eventKind ) );
	}
}


// ---------------------------------------------------------------------------
//	RemoveEventHandler

void
EventDoerAttachment::RemoveEventHandler()
{
	mTargetEventHandler.Remove();
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ResponseAttachment											  [public]

ResponseAttachment::ResponseAttachment()
{
	mResponseTarget = metaTarget_Self;
}


// ---------------------------------------------------------------------------
//	~ResponseAttachment											  [public]

ResponseAttachment::~ResponseAttachment()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]

void
ResponseAttachment::Initialize(
	EventTarget*			inTarget,
	EventClassT				inEventClass,
	EventKindT				inEventKind,
	EMetaTarget				inResponseTarget,
	const SysCarbonEvent&	inResponseEvent)
{
	EventDoerAttachment::Initialize(inTarget, inEventClass, inEventKind);

	mResponseTarget = inResponseTarget;
	mResponseEvent  = inResponseEvent;
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ResponseAttachment::ClassName() const
{
	return CFSTR("PPx::ResponseAttachment");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ResponseAttachment::InitState(
	const DataReader&	inReader)
{
	EventDoerAttachment::InitState(inReader);
	
	UInt16	metaTarget = metaTarget_Self;
	inReader.ReadOptional(key_ResponseMetaTarget, metaTarget);
	mResponseTarget = (EMetaTarget) metaTarget;
	
	EventClassT		eventClass;
	EventKindT		eventKind;
	
	inReader.ReadRequired(key_TargetEventClass, eventClass);
	inReader.ReadRequired(key_TargetEventKind, eventKind);
	
	mResponseEvent.MakeEvent(eventClass, eventKind);
	
		// +++ Need to read and set event parameters
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ResponseAttachment::WriteState(
	DataWriter&	ioWriter) const
{
	EventDoerAttachment::WriteState(ioWriter);
	
	ioWriter.WriteValue(key_ResponseMetaTarget, (UInt16) mResponseTarget);
	ioWriter.WriteValue(key_ResponseEventClass, mResponseEvent.GetEventClass());
	ioWriter.WriteValue(key_ResponseEventKind, mResponseEvent.GetEventKind());
}


// ---------------------------------------------------------------------------
//	DoEvent
// ---------------------------------------------------------------------------

OSStatus
ResponseAttachment::DoEvent(
	SysCarbonEvent&		/* ioEvent */)
{
	return mResponseEvent.SendTo(GetResponseSysEventTarget());
}


// ---------------------------------------------------------------------------
//	GetResponseSysEventTarget
// ---------------------------------------------------------------------------

EventTargetRef
ResponseAttachment::GetResponseSysEventTarget() const
{
	EventTargetRef	targetRef = nil;

	switch (mResponseTarget) {
	
		case metaTarget_Self:
			targetRef = GetEventTarget()->GetSysEventTarget();
			break;
		
		case metaTarget_UserFocus:
			targetRef = ::GetUserFocusEventTarget();
			break;
			
		case metaTarget_Application:
			targetRef = ::GetApplicationEventTarget();
			break;
	}
	
	return targetRef;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	MessageAttachment												  [public]

MessageAttachment::MessageAttachment()
{
}


// ---------------------------------------------------------------------------
//	~MessageAttachment											  [public]

MessageAttachment::~MessageAttachment()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]

void
MessageAttachment::Initialize(
	EventTarget*			inTarget,
	EventClassT				inEventClass,
	EventKindT				inEventKind,
	EventTarget*			inMessageTarget,
	const SysCarbonEvent&	inMessageEvent)
{
	EventDoerAttachment::Initialize(inTarget, inEventClass, inEventKind);

	mMessageEvent  = inMessageEvent;
	
	SetMessageTarget(inMessageTarget);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
MessageAttachment::ClassName() const
{
	return CFSTR("PPx::MessageAttachment");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
MessageAttachment::InitState(
	const DataReader&	inReader)
{
	EventDoerAttachment::InitState(inReader);
	
	EventClassT		eventClass;
	EventKindT		eventKind;
	
	inReader.ReadRequired(key_MessageEventClass, eventClass);
	inReader.ReadRequired(key_MessageEventKind, eventKind);
	
	mMessageEvent.MakeEvent(eventClass, eventKind);
	
		// +++ Need to read and set event parameters
	
	EventTarget*	messageTarget;
	inReader.ReadObjectValue(key_MessageTarget, messageTarget);
	
	SetMessageTarget(messageTarget);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
MessageAttachment::WriteState(
	DataWriter&	ioWriter) const
{
	EventDoerAttachment::WriteState(ioWriter);
	
	ioWriter.WriteObjectValue(key_MessageTarget, mMessageTarget);
	ioWriter.WriteValue(key_MessageEventClass, mMessageEvent.GetEventClass());
	ioWriter.WriteValue(key_MessageEventKind, mMessageEvent.GetEventKind());
}


// ---------------------------------------------------------------------------
//	SetMessageTarget

void
MessageAttachment::SetMessageTarget(
	EventTarget*	inMessageTarget)
{
	mMessageTarget = inMessageTarget;
	
	if (inMessageTarget != nil) {
		mDestructHandler.Adopt( HIObjectDestructDoer::Install(
									inMessageTarget->GetSysEventTarget()) );
	}
}


// ---------------------------------------------------------------------------
//	DoEvent

OSStatus
MessageAttachment::DoEvent(
	SysCarbonEvent&		/* ioEvent */)
{
	OSStatus	status = noErr;
	
	if (mMessageTarget != nil) {
		status = mMessageEvent.SendTo(mMessageTarget->GetSysEventTarget());
	}
	
	return status;
}


// ---------------------------------------------------------------------------
//	DoHIObjectDestruct

OSStatus
MessageAttachment::DoHIObjectDestruct(
	SysCarbonEvent&		/* ioEvent */)
{
	mDestructHandler.Remove();
	mMessageTarget = nil;
	
	return noErr;
}


} // namespace PPx
