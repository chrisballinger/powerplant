// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCorrespondent.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxCorrespondent.h>
#include <PPxEventUtils.h>
#include <PPxSerializer.h>
#include <SysEventHandler.h>

namespace PPx {

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Toolbox callback function for HIObject Carbon Events
	//
	//	The Toolbox requires that HIObjects handle "contruct" and "destruct"
	//	events. Our C++ wrapper class deals with construction and destruction,
	//	so the Toolbox callback does nothing

	pascal OSStatus
	EventHandlerTBCallback(
		EventHandlerCallRef		/* inCallRef */,
		EventRef				/* inEventRef */,
		void*					/* inUserData */)
	{
		return noErr;
	}
}


// ---------------------------------------------------------------------------
//	Correspondent							Default Constructor		  [public]

Correspondent::Correspondent()
{
		// Local static ensures that we register our EventTarget class with
		// the Toolbox the first time we are called
		
	static	bool	isRegistered = false;
	
	if (not isRegistered) {
	
			// Register a custom HIObject subclass with a callback that
			// handles the required construct and destruct events
			
		static SysEventHandlerUPP
						sConstructEventUPP(&EventHandlerTBCallback);
	
		EventTypeSpec	eventTypes[2] = {
							{kEventClassHIObject, kEventHIObjectConstruct},
							{kEventClassHIObject, kEventHIObjectDestruct} };
	
		SysHIObject::RegisterSysClass( GetClassName(), nil,
									   0,
									   sConstructEventUPP.Get(),
									   2, eventTypes,
									   nil, nil);
						
		isRegistered = true;
	}
	
	mSysHIObject.CreateSysObject(GetClassName());	// Create Toolbox HIObject
}


// ---------------------------------------------------------------------------
//	~Correspondent							Destructor				  [public]

Correspondent::~Correspondent()
{
								// Send notification of our impending demise
	EventUtils::SendTargetDeletedEvent(GetSysEventTarget());
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
Correspondent::InitState(
	const DataReader&	inReader)
{
	ReadAttachments(inReader);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
Correspondent::WriteState(
	DataWriter&	ioWriter) const
{
	WriteAttachments(ioWriter);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
Correspondent::ClassName() const
{
	return CFSTR("PPx::Correspondent");
}


// ---------------------------------------------------------------------------
//	SysEventTarget													 [private]

EventTargetRef
Correspondent::SysEventTarget() const
{
	return mSysHIObject.GetSysEventTarget();
}



} // namespace PPx