// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysAEHandler.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysAEHandler.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysAEHandler													  [public]
/**
	Default constructor */

SysAEHandler::SysAEHandler()
{
	mAEClassID			= 0;
	mAEEventID			= 0;
	mAECallback			= nil;
	mIsSystemHandler	= false;
	mIsInstalled		= false;
}


// ---------------------------------------------------------------------------
//	~SysAEHandler													  [public]
/**
	Destructor */

SysAEHandler::~SysAEHandler()
{
	Remove();
}


// ---------------------------------------------------------------------------
//	Install															  [public]
/**
	Installs Handler for the specified kind of AppleEvent
	
	@param	inClassID			AppleEvent class ID
	@param	inEventID			AppleEvent event ID
	@param	inCallback			Callback function to handle event
	@param	inRefCon			User-defined data stored in AE handler
	@param	inIsSystemHandler	Whether handler is system-wide or local		*/

void
SysAEHandler::Install(
	AEEventClass		inClassID,
	AEEventID			inEventID,
	AEEventHandlerUPP	inCallback,
	void*				inRefCon,
	bool				inIsSystemHandler)
{
	if (mIsInstalled) {
		Remove();
	}

	mAEClassID			= inClassID;
	mAEEventID			= inEventID;
	mAECallback			= inCallback;
	mIsSystemHandler	= inIsSystemHandler;
	mIsInstalled		= false;

	OSStatus	status = noErr;
	
	status = ::AEInstallEventHandler( inClassID, inEventID, inCallback,
									  reinterpret_cast<long>(inRefCon),
									  inIsSystemHandler );
									  
	PPx_ThrowIfOSError_(status, "AEInstallEventHandler failed");

	mIsInstalled = true;
}


// ---------------------------------------------------------------------------
//	Remove															  [public]
/**
	Removes AppleEvent handler.
	
	You can call Install() at a later time to re-install the handler		*/

void
SysAEHandler::Remove()
{
	if (mIsInstalled) {
		OSStatus	status = ::AERemoveEventHandler( mAEClassID, mAEEventID,
											mAECallback, mIsSystemHandler );
											
		PPx_ThrowIfOSError_(status, "AERemoveEventHandler failed");
		
		mIsInstalled = false;
	}
}


// ---------------------------------------------------------------------------
//	GetInfo															  [public]
/**
	Get information about the AE handler
	
	@param	outClassID			AppleEvent class ID that is handled
	@param	outEventID			AppleEvent event ID that is handled
	@param	outIsSystemHandler	Whether handler is system-wide or local		*/

void
SysAEHandler::GetInfo(
	AEEventClass&		outClassID,
	AEEventID&			outEventID,
	bool&				outIsSystemHandler) const
{
	outClassID			= mAEClassID;
	outEventID			= mAEEventID;
	outIsSystemHandler	= mIsSystemHandler;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysAEHandlerUPP						Constructor					  [public]


SysAEHandlerUPP::SysAEHandlerUPP(
	AEEventHandlerProcPtr	inCallbackFunc)
{
	mAEHandlerUPP = ::NewAEEventHandlerUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysAEHandlerUPP					Destructor					  [public]

SysAEHandlerUPP::~SysAEHandlerUPP()
{
	::DisposeAEEventHandlerUPP(mAEHandlerUPP);
}

} // namespace PPx
