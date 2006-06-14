// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxToolbarEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxToolbarEvents.h
	@brief		Event handlers for toolbar and toolbar item Carbon Events
*/
	// ### Not Finished
	
#ifndef H_PPxToolbarEvents
#define H_PPxToolbarEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Returns list of default item identifiers for a toolbar */

class	ToolbarGetDefaultIdentifiersDoer : public SpecificEventDoer<
										kEventClassToolbar,
										kEventToolbarGetDefaultIdentifiers> {
protected:
	virtual OSStatus	DoToolbarGetDefaultIdentifiers(
								SysCarbonEvent&		ioEvent,
								HIToolbarRef		inToolbarRef,
								CFMutableArrayRef	ioIdentifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns list of default item identifiers for a toolbar */

class	ToolbarGetAllowedIdentifiersDoer : public SpecificEventDoer<
										kEventClassToolbar,
										kEventToolbarGetAllowedIdentifiers> {
protected:
	virtual OSStatus	DoToolbarGetAllowedIdentifiers(
								SysCarbonEvent&		ioEvent,
								HIToolbarRef		inToolbarRef,
								CFMutableArrayRef	ioIdentifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Creates a new toolbar item with a specified identifier */

class	ToolbarCreateItemWithIdentifierDoer : public SpecificEventDoer<
										kEventClassToolbar,
										kEventToolbarCreateItemWithIdentifier> {
protected:
	virtual OSStatus	DoToolbarCreateItemWithIdentifier(
								SysCarbonEvent&		ioEvent,
								HIToolbarRef		inToolbarRef,
								CFStringRef			inItemIdentifier,
								CFTypeRef			inItemConfigData,
								HIToolbarItemRef&	outToolbarItem) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Creates a new toolbar item from a drag and drop operation */

class	ToolbarCreateItemFromDragDoer : public SpecificEventDoer<
										kEventClassToolbar,
										kEventToolbarCreateItemFromDrag> {
protected:
	virtual OSStatus	DoToolbarCreateItemFromDrag(
								SysCarbonEvent&		ioEvent,
								HIToolbarRef		inToolbarRef,
								DragRef				inDragRef,
								HIToolbarItemRef&	outToolbarItem) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxToolbarEvents
