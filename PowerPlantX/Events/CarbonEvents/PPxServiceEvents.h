// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxServiceEvents.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxServiceEvents.h
	@brief		Event handlers for service Carbon Events
*/

#ifndef H_PPxServiceEvents
#define H_PPxServiceEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles the service for copying data from current focus */

class	ServiceCopyDoer : public SpecificEventDoer<
									kEventClassService,
									kEventServiceCopy> {
protected:
	virtual OSStatus	DoServiceCopy(
								SysCarbonEvent& ioEvent,
								ScrapRef		inScrap) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the service for pasting data into the current focus */

class	ServicePasteDoer : public SpecificEventDoer<
									kEventClassService,
									kEventServicePaste> {
protected:
	virtual OSStatus	DoServicePaste(
								SysCarbonEvent& ioEvent,
								ScrapRef		inScrap) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the service getting the types of data which can be copied
	and pasted																*/

class	ServiceGetTypesDoer : public SpecificEventDoer<
										kEventClassService,
										kEventServiceGetTypes> {
protected:
	virtual OSStatus	DoServiceGetTypes(
								SysCarbonEvent& 	ioEvent,
								CFMutableArrayRef	inCopyTypes,
								CFMutableArrayRef	inPasteTypes) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles performing a service */

class	ServicePerformDoer : public SpecificEventDoer<
										kEventClassService,
										kEventServicePerform> {
protected:
	virtual OSStatus	DoServicePerform(
								SysCarbonEvent& ioEvent,
								ScrapRef 		inScrap,
								CFStringRef		inMessageName,
								CFStringRef		inUserData) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxServiceEvents
