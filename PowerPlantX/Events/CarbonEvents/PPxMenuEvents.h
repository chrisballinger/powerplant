// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMenuEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxMenuEvents.h
	@brief		Event handlers for menu Carbon Events
*/

#ifndef H_PPxMenuEvents
#define H_PPxMenuEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles the start of tracking the menubar or a pop-up menu */

class	MenuBeginTrackingDoer : public SpecificEventDoer<
											kEventClassMenu,
											kEventMenuBeginTracking> {
protected:	
	virtual OSStatus	DoMenuBeginTracking(
								SysCarbonEvent&		ioEvent,
								MenuRef				inMenuRef,
								MenuTrackingMode	inTrackingMode,
								UInt32				inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the end of tracking the menubar or a pop-up menu */

class	MenuEndTrackingDoer : public SpecificEventDoer<
										kEventClassMenu,
										kEventMenuEndTracking> {
protected:	
	virtual OSStatus	DoMenuEndTracking(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								UInt32			inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles changing between mouse and keyboard menu tracking modes */

class	MenuChangeTrackingModeDoer : public SpecificEventDoer<
												kEventClassMenu,
												kEventMenuChangeTrackingMode> {
protected:	
	virtual OSStatus	DoMenuChangeTrackingMode(
								SysCarbonEvent&		ioEvent,
								MenuTrackingMode	inCurrentMode,
								MenuTrackingMode	inNewMode,
								UInt32				inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a menu being opened (about to be displayed) */

class	MenuOpeningDoer : public SpecificEventDoer<
									kEventClassMenu,
									kEventMenuOpening> {
protected:	
	virtual OSStatus	DoMenuOpening(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								bool			inFirstOpen,
								UInt32			inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a menu being closed */

class	MenuClosedDoer : public SpecificEventDoer<
									kEventClassMenu,
									kEventMenuClosed> {
protected:	
	virtual OSStatus	DoMenuClosed(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								UInt32			inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles the mouse moving over a menu item */

class	MenuTargetItemDoer : public SpecificEventDoer<
										kEventClassMenu,
										kEventMenuTargetItem> {
protected:	
	virtual OSStatus	DoMenuTargetItem(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								MenuItemIndex	inItem,
								MenuCommand		inCommand,
								UInt32			inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns menu item matching a command key equivalent */

class	MenuMatchKeyDoer : public SpecificEventDoer<
									kEventClassMenu,
									kEventMenuMatchKey> {
protected:	
	virtual OSStatus	DoMenuMatchKey(
								SysCarbonEvent&		ioEvent,
								MenuRef				inMenuRef,
								EventRef			inMenuEventRef,
								MenuEventOptions	inOptions,
								UInt32				inMenuContext,
								MenuItemIndex&		outItem) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles enabling or disabling items in a menu */

class	MenuEnableItemsDoer : public SpecificEventDoer<
										kEventClassMenu,
										kEventMenuEnableItems> {
protected:	
	virtual OSStatus	DoMenuEnableItems(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								bool			inIsKeyEvent,
								UInt32			inMenuContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles populating a menu with items prior to use */

class	MenuPopulateDoer : public SpecificEventDoer<
									kEventClassMenu,
									kEventMenuPopulate> {
protected:	
	virtual OSStatus	DoMenuPopulate(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								UInt32			inMenuContext,
								MenuCommand		inCommand) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the width, in pixels, of a menu item */

class	MenuMeasureItemWidthDoer : public SpecificEventDoer<
											kEventClassMenu,
											kEventMenuMeasureItemWidth> {
protected:	
	virtual OSStatus	DoMenuMeasureItemWidth(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								MenuItemIndex	inItem,
								SInt16&			outItemWidth) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the height, in pixels, of a menu item */

class	MenuMeasureItemHeightDoer : public SpecificEventDoer<
												kEventClassMenu,
												kEventMenuMeasureItemHeight> {
protected:	
	virtual OSStatus	DoMenuMeasureItemHeight(
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef,
								MenuItemIndex	inItem,
								SInt16&			outItemHeight) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles drawing a menu item */

class	MenuDrawItemDoer : public SpecificEventDoer<
									kEventClassMenu,
									kEventMenuDrawItem> {
protected:	
	virtual OSStatus	DoMenuDrawItem(
								SysCarbonEvent&		ioEvent,
								MenuRef				inMenuRef,
								const Rect&			inCurrentBounds,
								MenuItemIndex		inItem,
								const Rect&			inItemBounds,
								SInt32				inVirtualTop,
								SInt32				inVirtualBottom,
								ThemeMenuState		inDrawState,
								ThemeMenuItemType	inItemType,
								CGContextRef		inCGContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles drawing the content of a menu item */

class	MenuDrawItemContentDoer : public SpecificEventDoer<
											kEventClassMenu,
											kEventMenuDrawItemContent> {
protected:	
	virtual OSStatus	DoMenuDrawItemContent(
								SysCarbonEvent&		ioEvent,
								MenuRef				inMenuRef,
								MenuItemIndex		inItem,
								const Rect&			inItemBounds,
								SInt16				inDeviceDepth,
								bool				inDeviceHasColor,
								CGContextRef		inCGContext) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a menu being disposed */

class	MenuDisposeDoer : public SpecificEventDoer<
									kEventClassMenu,
									kEventMenuDispose> {
protected:	
	virtual OSStatus	DoMenuDispose( 
								SysCarbonEvent&	ioEvent,
								MenuRef			inMenuRef) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxMenuEvents
