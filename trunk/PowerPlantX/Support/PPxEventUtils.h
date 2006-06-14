// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventUtils.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxEventUtils.h
	@brief		Utility functions for working with Carbon Events
*/

#ifndef H_PPxEventUtils
#define H_PPxEventUtils
#pragma once

#include <PPxPrefix.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
/**	Utility functions for working with Carbon Events */

namespace EventUtils {

	void		SetMenuCommandStatus(
						MenuCommand		inCommand,
						bool			inStatus);
					
	OSStatus	ProcessCommandID(
						HICommand	inCommand,
						UInt32		inKeyModifiers = attributes_None,
						UInt32		inMenuContext = attributes_None);
						
	OSStatus	UpdateCommandID(
						HICommand	inCommand,
						UInt32		inMenuContext = attributes_None);

	void		PostCommandID(
						CommandIDT		inCommandID,
						EventTargetRef	inTarget = nil,
						EventQueueRef	inQueueRef = nil,
						EventPriority	inPriority = kEventPriorityStandard);

	OSStatus	SendCommandID(
						CommandIDT		inCommandID,
						EventTargetRef	inTarget,
						OptionBits		inOptions = options_None);
						
	void		SendTargetDeletedEvent( EventTargetRef inTarget );
}


} // namespace PPx

#endif	// H_PPxEventUtils
