// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysHIObject.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:36 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysHIObject.h>
#include <SysEventHandler.h>

namespace PPx {

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Toolbox callback function for HIObject Carbon Events
	//
	//	The Toolbox requires that HIObjects handle "construct" and "destruct"
	//	events. This simple callback does nothing, which is appropriate if
	//	using a C++ wrapper class that handles construction and destruction.

	pascal OSStatus
	EventHandlerTBCallback(
		EventHandlerCallRef		/* inCallRef */,
		EventRef				/* inEventRef */,
		void*					/* inUserData */)
	{
		return noErr;
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to Carbon Event callback function
	
	EventHandlerUPP
	GetSysEventHandlerUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysEventHandlerUPP	sConstructEventUPP(EventHandlerTBCallback);
		
		return sConstructEventUPP.Get();
	}
	
} // unnamed namespace

#pragma mark -

// ---------------------------------------------------------------------------
//	SysHIObject														  [public]
/**
	Default constructor */

SysHIObject::SysHIObject()
{
	mHIObjectRef = nil;
}


// ---------------------------------------------------------------------------
//	SysHIObject														  [public]
/**
	Constructs from an existing HIObjectRef
	
	@param	inObjectRef	HIObjectRef for this object to use					*/

SysHIObject::SysHIObject(
	HIObjectRef	inObjectRef)
{
	mHIObjectRef = inObjectRef;
	
	if (inObjectRef != nil) {
		::CFRetain(inObjectRef);
	}
}


// ---------------------------------------------------------------------------
//	SysHIObject														  [public]
/**
	Constructs from HIObject creation parameters
	
	@param	inClassID			Class ID of HIObject
	@param	inConstructData		Event to send during construction			*/

SysHIObject::SysHIObject(
	CFStringRef		inClassID,
	EventRef		inConstructData)
{
	mHIObjectRef = nil;
	
	CreateSysObject(inClassID, inConstructData);
}


// ---------------------------------------------------------------------------
//	~SysHIObject													  [public]
/**
	Destructor */

SysHIObject::~SysHIObject()
{
	if (mHIObjectRef != nil) {
		::CFRelease(mHIObjectRef);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Adopt															  [public]
/**
	Adopts an existing HIObjectRef
	
	@param	inObjectRef	HIObjectRef to adopt
	
	Releases current HIObjectRef and takes ownership of the input HIObjectRef */

void
SysHIObject::Adopt(
	HIObjectRef	inObjectRef)
{
	if (mHIObjectRef != nil) {
		::CFRelease(mHIObjectRef);
	}
	
	mHIObjectRef = inObjectRef;
	
	if (inObjectRef != nil) {
		::CFRetain(inObjectRef);
	}
}


// ---------------------------------------------------------------------------
//	CreateSysObject													  [public]
/**
	Create system HIObject from creation parameters
	
	@param	inClassID			Class ID of HIObject
	@param	inConstructData		Event to send during construction			*/

void
SysHIObject::CreateSysObject(
	CFStringRef		inClassID,
	EventRef		inConstructData)
{
	if (mHIObjectRef != nil) {
		PPx_Throw_(LogicError, err_Logic, "HIObject already exists");
	}
	
	OSStatus	status = ::HIObjectCreate(inClassID, inConstructData,
											&mHIObjectRef);
	
	PPx_ThrowIfOSError_(status, "HIObjectCreate failed");
}


// ---------------------------------------------------------------------------
//	GetSysEventTarget												  [public]
/**
	Returns the event target for the HIObject
	
	@return Event target for the HIObject									*/

EventTargetRef
SysHIObject::GetSysEventTarget() const
{
	return ::HIObjectGetEventTarget(mHIObjectRef);
}


// ---------------------------------------------------------------------------
//	GetSysObject													  [public]
/**
	Returns the HIObjectRef for the object
	
	@return HIObjectRef for the object										*/

HIObjectRef
SysHIObject::GetSysObject() const
{
	return mHIObjectRef;
}


// ---------------------------------------------------------------------------
//	RegisterSysClass										 [static] [public]
/**
	Register with the system a class for creating HIObjects
	
	@param	inClassID			Class ID of HIObject
	@param	inBaseClassID		Class ID of base class of HIObject
	@param	inOptions			Special optons (always 0 for now)
	@param	inContructProc		Event handler for constructing HIObject
	@param	inNumEvents			Number of events to handle
	@param	inEventList			List of events to handle
	@param	inConstructData		User-defined intialization data
	@param	outClassRef			Reference registered HIObject class			*/

void
SysHIObject::RegisterSysClass(
	CFStringRef				inClassID,
	CFStringRef				inBaseClassID,
	OptionBits				inOptions,
	EventHandlerUPP			inContructProc,
	UInt32					inNumEvents,
	const EventTypeSpec*	inEventList,
	void*					inConstructData,
	HIObjectClassRef*		outClassRef)
{
	OSStatus	status = ::HIObjectRegisterSubclass(
								inClassID, inBaseClassID, inOptions,
								inContructProc, inNumEvents, inEventList,
								inConstructData, outClassRef);
	
	PPx_ThrowIfOSError_(status, "HIObjectRegisterSubclass failed");
}


// ---------------------------------------------------------------------------
//	RegisterSysClass										 [static] [public]
/**
	Simple registration for an HIObject subclass that handles its own
	construction and destruction process
	
	@param	inClassID			Class ID of HIObject
	@param	inBaseClassID		Class ID of base class of HIObject			*/

void
SysHIObject::RegisterSysClass(
	CFStringRef				inClassID,
	CFStringRef				inBaseClassID)
{
		// Handle just the required "construct" and "destruct" events
		
	EventTypeSpec	eventTypes[2] = {
						{kEventClassHIObject, kEventHIObjectConstruct},
						{kEventClassHIObject, kEventHIObjectDestruct} };

	OSStatus	status = ::HIObjectRegisterSubclass(
								inClassID, inBaseClassID, 0,
								GetSysEventHandlerUPP(), 2, eventTypes,
								nil, nil);
	
	PPx_ThrowIfOSError_(status, "HIObjectRegisterSubclass failed");
}


} // end namespace PPx
