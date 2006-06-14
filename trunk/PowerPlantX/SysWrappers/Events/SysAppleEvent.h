// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysAppleEvent.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysAppleEvent.h
	@brief		Wrapper class for an Apple Event
*/

#ifndef H_SysAppleEvent
#define H_SysAppleEvent
#pragma once

#include <PPxPrefix.h>
#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	SysAppleEvent
/**
	Wrapper class for an Apple Event */

class	SysAppleEvent {
public:
						SysAppleEvent();

						SysAppleEvent( const AppleEvent& inAppleEvent );
						
						SysAppleEvent(
								AEEventClass			inEventClass,
								AEEventID				inEventID,
								const AEAddressDesc&	inAETarget,
								AEReturnID				inReturnID = kAutoGenerateReturnID,
								AETransactionID			inTransID = kAnyTransactionID);
						
						SysAppleEvent( const SysAppleEvent& inOriginal );
						
	SysAppleEvent&		operator = ( const SysAppleEvent& inOriginal );
	
						~SysAppleEvent();
						
	const AppleEvent&	GetAppleEvent() const;
	
	AppleEvent&			GetAppleEvent();
	
	AEEventClass		GetEventClass() const;
	
	AEEventID			GetEventKind() const;			

	OSStatus			GetParameter(
								AEKeyword		inName,
								DescType		inDesiredType,
								DescType*		outActualType,
								Size			inBufferSize,
								Size*			outActualSize,
								void*			outData) const;

	void				SetParameter(
								AEKeyword		inName,
								DescType		inType,
								Size			inSize,
								const void*		inDataPtr);
								
	OSStatus			GetParamDesc(
								AEKeyword		inName,
								DescType		inType,
								AEDesc&			outDesc) const;
	
	void				SetParamDesc(
								AEKeyword		inName,
								const AEDesc&	inDesc);

	void				Send(	AppleEvent*		inReply = nil,
								AESendMode		inSendMode = kAENoReply,
								AESendPriority	inPriority = kAENormalPriority,
								SInt32			inTimeOut = kAEDefaultTimeout,
								AEIdleUPP		inIdleProc = nil,
								AEFilterUPP		inFilterProc = nil);

private:	
	void				MakeAppleEvent(
								AEEventClass			inEventClass,
								AEEventID				inEventID,
								const AEAddressDesc&	inAETarget,
								AEReturnID				inReturnID = kAutoGenerateReturnID,
								AETransactionID			inTransID = kAnyTransactionID);
								
	void				Dispose();

private:
	AppleEvent			mAppleEvent;			/**< System AppleEvent data structure */
};


} // namespace PPx

#endif	// H_PAppleEventCallback
