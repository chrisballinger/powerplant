// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysAppleEvent.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysAppleEvent.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysAppleEvent													  [public]
/**
	Constructor */

SysAppleEvent::SysAppleEvent()
{
	::AEInitializeDesc(&mAppleEvent);
}


// ---------------------------------------------------------------------------
//	SysAppleEvent													  [public]
/**
	Constructs from an existing AppleEvent record
	
	@param	inAppleEvent	AppleEvent record
	
	Creates a copy of the input AppleEvent									*/

SysAppleEvent::SysAppleEvent(
	const AppleEvent&	inAppleEvent)
{
	OSStatus	err = ::AEDuplicateDesc(&inAppleEvent, &mAppleEvent);
	PPx_ThrowIfOSError_(err, "AEDuplicateDesc failed");
}


// ---------------------------------------------------------------------------
//	SysAppleEvent													  [public]
/**
	Constructs from AppleEvent creation parameters
	
	@param	inEventClass	Apple Event class
	@param	inEventID		Apple Event ID
	@param	inAETarget		Target program for sending the event
	@param	inReturnID		Return ID
	@param	inTransID		Transaction ID									*/

SysAppleEvent::SysAppleEvent(
	AEEventClass			inEventClass,
	AEEventID				inEventID,
	const AEAddressDesc&	inAETarget,
	AEReturnID				inReturnID,
	AETransactionID			inTransID)
{
	::AEInitializeDesc(&mAppleEvent);
	MakeAppleEvent(inEventClass, inEventID, inAETarget, inReturnID, inTransID);
}


// ---------------------------------------------------------------------------
//	SysAppleEvent													  [public]
/**
	Copy constructor */

SysAppleEvent::SysAppleEvent(
	const SysAppleEvent&	inOriginal)
{
	OSStatus	err = ::AEDuplicateDesc(&inOriginal.mAppleEvent, &mAppleEvent);
	PPx_ThrowIfOSError_(err, "AEDuplicateDesc failed");
}



// ---------------------------------------------------------------------------
//	SysAppleEvent													  [public]
/**
	Assignment operator */

SysAppleEvent&
SysAppleEvent::operator = (
	const SysAppleEvent&	inOriginal)
{
	AppleEvent	theCopy;
	OSStatus	err = ::AEDuplicateDesc(&inOriginal.mAppleEvent, &theCopy);
	PPx_ThrowIfOSError_(err, "AEDuplicateDesc failed");
	
	::AEDisposeDesc(&mAppleEvent);
	
	mAppleEvent = theCopy;
	
	return *this;
}


// ---------------------------------------------------------------------------
//	~SysAppleEvent													  [public]
/**
	Destructor */

SysAppleEvent::~SysAppleEvent()
{
	::AEDisposeDesc(&mAppleEvent);
}


// ---------------------------------------------------------------------------
//	MakeAppleEvent													  [public]
/**
	Makes an Apple Event record from input creation parameters
	
	@param	inEventClass	Apple Event class
	@param	inEventID		Apple Event ID
	@param	inAETarget		Target program for sending the event
	@param	inReturnID		Return ID
	@param	inTransID		Transaction ID									*/

void
SysAppleEvent::MakeAppleEvent(
	AEEventClass			inEventClass,
	AEEventID				inEventID,
	const AEAddressDesc&	inAETarget,
	AEReturnID				inReturnID,
	AETransactionID			inTransID)
{	
	OSStatus	status = ::AECreateAppleEvent(
								inEventClass, inEventID, &inAETarget,
								inReturnID, inTransID, &mAppleEvent);
	PPx_ThrowIfOSError_(status, "AECreateAppleEvent failed");
}


// ---------------------------------------------------------------------------
//	GetAppleEvent													  [public]
/**
	Returns const reference to AppleEvent record
	
	@return Const reference to AppleEvent record							*/

const AppleEvent&
SysAppleEvent::GetAppleEvent() const
{
	return mAppleEvent;
}


// ---------------------------------------------------------------------------
//	GetAppleEvent													  [public]
/**
	Returns non-const reference to AppleEvent record
	
	@return Non-const reference to AppleEvent record						*/

AppleEvent&
SysAppleEvent::GetAppleEvent()
{
	return mAppleEvent;
}


// ---------------------------------------------------------------------------
//	GetEventClass													  [public]
/**
	Returns the class of the AppleEvent
	
	@return Class of the AppleEvent											*/

AEEventClass
SysAppleEvent::GetEventClass() const
{
	AEEventClass	eventClass;
	DescType		actualType;
	Size			actualSize;
	
	OSStatus status = ::AEGetAttributePtr(
								&mAppleEvent, keyEventClassAttr, typeType,
								&actualType, &eventClass,
								sizeof(UInt32), &actualSize);
								
	PPx_ThrowIfOSError_(status, "AEGetAttributePtr failed to get event class");

	return eventClass;
}


// ---------------------------------------------------------------------------
//	GetEventKind													  [public]
/**
	Returns the kind of the AppleEvent
	
	@return	Kind of the AppleEvent											*/

AEEventID
SysAppleEvent::GetEventKind() const
{
	AEEventID		eventKind;
	DescType		actualType;
	Size			actualSize;
	
	OSStatus status = ::AEGetAttributePtr(
								&mAppleEvent, keyEventIDAttr, typeType,
								&actualType, &eventKind,
								sizeof(UInt32), &actualSize);

	PPx_ThrowIfOSError_(status, "AEGetAttributePtr failed to get event kind");

	return eventKind;
}


// ---------------------------------------------------------------------------
//	GetParameter													  [public]
/**
	Gets a named parameter from the AppleEvent
	
	@param	inName			Parameter name
	@param	inDesiredType	Desired type for parameter value
	@param	outActualType	Actual type of value retrieved
	@param	inBufferSize	Size of buffer for storing parameter
	@param	outActualSize	Actual number of bytes retrieved
	@param	outData			Pointer to buffer for storing parameter
	
	@return OS error code													*/

OSStatus
SysAppleEvent::GetParameter(
	AEKeyword		inName,
	DescType		inDesiredType,
	DescType*		outActualType,
	Size			inBufferSize,
	Size*			outActualSize,
	void*			outData) const
{
	OSStatus	status = ::AEGetParamPtr(
								&mAppleEvent, inName, inDesiredType,
								outActualType, outData,
								inBufferSize, outActualSize);
								
	return status;
}


// ---------------------------------------------------------------------------
//	SetParameter													  [public]
/**
	Sets a named parameter in a AppleEvent
	
	@param	inName		Parameter name
	@param	inType		Parameter type
	@param	inSize		Size of parameter value
	@param	inDataPtr	Pointer to parameter data							*/

void
SysAppleEvent::SetParameter(
	AEKeyword		inName,
	DescType		inType,
	Size			inSize,
	const void*		inDataPtr)
{
	OSStatus	status = ::AEPutParamPtr( &mAppleEvent, inName, inType,
										  inDataPtr, inSize );
	
	PPx_ThrowIfOSError_(status, "AEPutParamPtr failed");
}


// ---------------------------------------------------------------------------
//	GetParamDesc													  [public]
/**
	Gets a keyword descriptor from an AppleEvent
	
	@param	inName		Keyword name
	@param	inType		Descriptor type
	@param	outDesc		Descriptor for the keyword
	
	@return OS error code													*/

OSStatus
SysAppleEvent::GetParamDesc(
	AEKeyword		inName,
	DescType		inType,
	AEDesc&			outDesc) const
{
	OSStatus	status = ::AEGetParamDesc(&mAppleEvent, inName, inType, &outDesc);
								
	return status;
}


// ---------------------------------------------------------------------------
//	SetParamDesc													  [public]
/**
	Sets a keyword descriptor in an AppleEvent
	
	@param	inName		Keyword name
	@param	inDesc		Descriptor											*/

void
SysAppleEvent::SetParamDesc(
	AEKeyword		inName,
	const AEDesc&	inDesc)
{
	OSStatus	status = ::AEPutParamDesc( &mAppleEvent, inName, &inDesc);
	
	PPx_ThrowIfOSError_(status, "AEPutParamDesc failed");
}


// ---------------------------------------------------------------------------
//	Send															  [public]
/**
	Sends an AppleEvent
	
	@param	inReply			Reply AppleEvent
	@param	inSendMode		Mode for sending the event
	@param	inPriority		Event priority
	@param	inTimeOut		Time out interval
	@param	inIdleProc		Idle proc
	@param	inFilterProc	Filter proc										*/

void
SysAppleEvent::Send(
	AppleEvent*		inReply,
	AESendMode		inSendMode,
	AESendPriority	inPriority,
	SInt32			inTimeOut,
	AEIdleUPP		inIdleProc,
	AEFilterUPP		inFilterProc)
{
	OSStatus	status = ::AESend( &mAppleEvent, inReply, inSendMode,
								   inPriority, inTimeOut, inIdleProc,
								   inFilterProc );
	
	PPx_ThrowIfOSError_(status, "AESend failed");
}

						
} // namespace PPx
