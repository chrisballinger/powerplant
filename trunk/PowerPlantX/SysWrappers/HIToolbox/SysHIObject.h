// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysHIObject.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:36 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysHIObject.h
	@brief		Wrapper class for a Mac Toolbox HIObject
*/

#ifndef H_SysHIObject
#define H_SysHIObject
#pragma once

#include <PPxPrefix.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysHIObject
/**
	Wrapper class for a Mac HIObject */

class	SysHIObject {
public:
					SysHIObject();
					
					SysHIObject( HIObjectRef inObjectRef );
					
					SysHIObject(
							CFStringRef		inClassID,
							EventRef		inConstructData = nil);
					
					~SysHIObject();
					
	void			Adopt( HIObjectRef inObjectRef );

	void			CreateSysObject(
							CFStringRef		inClassID,
							EventRef		inConstructData = nil);

	EventTargetRef	GetSysEventTarget() const;

	HIObjectRef		GetSysObject() const;
	
	static void		RegisterSysClass(
							CFStringRef				inClassID,
							CFStringRef				inBaseClassID,
							OptionBits				inOptions,
							EventHandlerUPP			inContructProc,
							UInt32					inNumEvents,
							const EventTypeSpec*	inEventList,
							void*					inConstructData,
							HIObjectClassRef*		outClassRef);
							
	static void		RegisterSysClass(
							CFStringRef				inClassID,
							CFStringRef				inBaseClassID = nil);
							
private:								// Forbid copy and assignment
					SysHIObject( const SysHIObject& );
	SysHIObject&	operator = ( const SysHIObject& );
							
private:
	HIObjectRef		mHIObjectRef;		/**< System HIObject reference */
};


} // namespace PPx

#endif	// H_SysHIObject
