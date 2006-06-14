// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxScrollableEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxScrollableEvents.h
	@brief		Carbon event handlers for scrollable events
*/

#ifndef H_PPxScrollableEvents
#define H_PPxScrollableEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Returns information about a scrollable view */

class	ScrollableGetInfoDoer : public SpecificEventDoer<
											kEventClassScrollable,
											kEventScrollableGetInfo> {
protected:
	virtual OSStatus	DoScrollableGetInfo(
								SysCarbonEvent&	ioEvent,
								HISize&			outImageSize,
								HISize&			outViewSize,
								HISize&			outLineSize,
								HIPoint&		outOrigin) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that a scrollable view has changed */

class	ScrollableInfoChangedDoer : public SpecificEventDoer<
												kEventClassScrollable,
												kEventScrollableInfoChanged> {
protected:
	virtual OSStatus	DoScrollableInfoChanged(
								SysCarbonEvent&	ioEvent) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles scrolling a view to a specific location */

class	ScrollableScrollToDoer : public SpecificEventDoer<
											kEventClassScrollable,
											kEventScrollableScrollTo> {
protected:
	virtual OSStatus	DoScrollableScrollTo(
								SysCarbonEvent&	ioEvent,
								const HIPoint&	inLocation) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxScrollableEvents
