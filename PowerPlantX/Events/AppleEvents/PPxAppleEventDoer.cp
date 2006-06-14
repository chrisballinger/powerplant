// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAppleEventDoer.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxAppleEventDoer.h>
#include <SysAppleEvent.h>

namespace PPx {

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Toolbox callback function for Apple Events
	//
	//	We use the same callback function for all Apple Events. The refcon
	//	is a pointer to an AppleEventDoer object.
	
	pascal OSErr
	AEHandlerTBCallback(
		const AppleEvent*	inAppleEvent,
		AppleEvent*			outAEReply,
		long				inRefCon)
	{
		AppleEventDoer*		theDoer =
							reinterpret_cast<AppleEventDoer*>(inRefCon);
							
		OSStatus	status = noErr;
		
		try {
			AutoAEDesc	theEvent(*inAppleEvent);
			AutoAEDesc	theReply(*outAEReply);
			
			status = theDoer->Invoke(theEvent, theReply);
		}
		
		catch (const OSError& inOSError) {
			status = inOSError.GetOSErrorCode();
		}
		
		catch (...) {
			status = errAEEventNotHandled;
		}
		
		return static_cast<OSErr>(status);
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to Apple Event callback function

	AEEventHandlerUPP
	GetSysAEHandlerUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysAEHandlerUPP		sAEHandlerUPP(AEHandlerTBCallback);
		
		return sAEHandlerUPP.Get();
	}
	
} // unnamed namespace


#pragma mark -

// ---------------------------------------------------------------------------
//	AppleEventDoer													  [public]
/**
	Default constructor */

AppleEventDoer::AppleEventDoer()
{
}


// ---------------------------------------------------------------------------
//	AppleEventDoer													  [public]
/**
	Constructs from an AppleEvent class and ID and installs a handler
	
	@param	inEventClass		AppleEvent class
	@param	inEventID			AppleEvent ID
	@param	inIsSystemHandler	Whether handler is system-wide (vs. local)	*/

AppleEventDoer::AppleEventDoer(
	AEEventClass	inEventClass,
	AEEventID		inEventID,
	bool			inIsSystemHandler)
{
	Install(inEventClass, inEventID, inIsSystemHandler);
}


// ---------------------------------------------------------------------------
//	~AppleEventDoer													  [public]
/**
	Destructor */

AppleEventDoer::~AppleEventDoer()
{
	Remove();
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Installs handler for an Apple Event
	
	@param	inEventClass		AppleEvent class
	@param	inEventID			AppleEvent ID
	@param	inIsSystemHandler	Whether the handler is system-wide or local	*/

void
AppleEventDoer::Install(
	AEEventClass	inEventClass,
	AEEventID		inEventID,
	bool			inIsSystemHandler)
{
	mAEHandler.Install(inEventClass, inEventID, GetSysAEHandlerUPP(),
						this, inIsSystemHandler);
}


// ---------------------------------------------------------------------------
//	Remove															  [public]
/**
	Removes handler for the Apple Event */

void
AppleEventDoer::Remove()
{
	mAEHandler.Remove();
}


// ---------------------------------------------------------------------------
//	Invoke															  [public]
/**
	Calls function to handle an AppleEvent
	
	@param	inAppleEvent	AppleEvent to handle
	@param	outAEReply		Reply AppleEvent
	
	@return OS error code													*/

OSStatus
AppleEventDoer::Invoke(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAppleEvent(inAppleEvent, outAEReply);
}


} // namespace PPx