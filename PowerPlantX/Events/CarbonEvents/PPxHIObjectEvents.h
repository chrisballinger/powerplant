// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxHIObjectEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxHIObjectEvents.h
	@brief		Event handlers for HIObject Carbon Events
*/

#ifndef H_PPxHIObjectEvents
#define H_PPxHIObjectEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles constructing an HIObject */

class	HIObjectConstructDoer : public SpecificEventDoer<
											kEventClassHIObject,
											kEventHIObjectConstruct> {
protected:
	virtual OSStatus	DoHIObjectConstruct(
								SysCarbonEvent&	ioEvent,
								HIObjectRef		inObjectRef) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles initializing an HIObject */

class	HIObjectInitializeDoer : public SpecificEventDoer<
											kEventClassHIObject,
											kEventHIObjectInitialize> {
protected:
	virtual OSStatus	DoHIObjectInitialize( SysCarbonEvent& ioEvent ) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles destroying an HIObject */

class	HIObjectDestructDoer : public SpecificEventDoer<
										kEventClassHIObject,
										kEventHIObjectDestruct> {
protected:
	virtual OSStatus	DoHIObjectDestruct( SysCarbonEvent& ioEvent ) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Determines if an HIObject is eaual to another HIObject */

class	HIObjectIsEqualDoer : public SpecificEventDoer<
										kEventClassHIObject,
										kEventHIObjectIsEqual> {
protected:
	virtual OSStatus	DoHIObjectIsEqual(
								SysCarbonEvent&	ioEvent,
								HIObjectRef		inObjectRef) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles request to print debugging information */

class	HIObjectPrintDebugInfoDoer : public SpecificEventDoer<
												kEventClassHIObject,
												kEventHIObjectPrintDebugInfo> {
protected:
	virtual OSStatus	DoHIObjectPrintDebugInfo( SysCarbonEvent& ioEvent ) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxHIObjectEvents
