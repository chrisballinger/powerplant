// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAppleEventDoer.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxAppleEventDoer.h
	@brief		Classes for handling Apple Events
*/

#ifndef H_PPxAppleEventDoer
#define H_PPxAppleEventDoer
#pragma once

#include <SysAEHandler.h>
#include <SysAEDesc.h>

namespace PPx {

// ===========================================================================
//	AppleEventDoer
/**
	Abstract class for an Apple Event handler */

class	AppleEventDoer {
public:
						AppleEventDoer();
						
						AppleEventDoer(
								AEEventClass	inEventClass,
								AEEventID		inEventID,
								bool			inIsSystemHandler = false);
								
	virtual				~AppleEventDoer();
	
	void				Install(
								AEEventClass	inEventClass,
								AEEventID		inEventID,
								bool			inIsSystemHandler = false);
								
	void				Remove();
	
	OSStatus			Invoke(	const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
								
private:
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;

private:
	SysAEHandler		mAEHandler;		/**< System AE Handler wrapper */
};


// ===========================================================================
//	SpecficAppleEventDoer
/**
	Template class for an Apple Event handler that responds to one
	specific type of event.
	
	The template parameters specify the class and kind of the event.		*/

template <AEEventClass TAEEventClass, AEEventID TAEEventID>
class	SpecificAppleEventDoer : public AppleEventDoer {
public:
			SpecificAppleEventDoer()
				{
					AppleEventDoer::Install(TAEEventClass, TAEEventID);
				}
						
	void	Install( bool inIsSystemHandler = false );
};


} // namespace PPx

#endif	// H_PPxAppleEventDoer
