// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMenuEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxMenuEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	MenuBeginTrackingDoer

OSStatus
MenuBeginTrackingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef					menu;
	MenuTrackingModeType	mode;
	UInt32					context;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamCurrentMenuTrackingMode, mode);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
										
	return DoMenuBeginTracking(ioEvent, menu, mode, context);
}


// ===========================================================================
//	MenuEndTrackingDoer

OSStatus
MenuEndTrackingDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef		menu;
	UInt32		context;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
										
	return DoMenuEndTracking(ioEvent, menu, context);
}


// ===========================================================================
//	MenuChangeTrackingModeDoer

OSStatus
MenuChangeTrackingModeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuTrackingModeType	currentMode;
	MenuTrackingModeType	newMode;
	UInt32					context;
	
	SysEventParam::Get(ioEvent, kEventParamCurrentMenuTrackingMode, currentMode);
	SysEventParam::Get(ioEvent, kEventParamNewMenuTrackingMode, newMode);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
										
	return DoMenuChangeTrackingMode(ioEvent, currentMode, newMode, context);
}


// ===========================================================================
//	MenuOpeningDoer

OSStatus
MenuOpeningDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef		menu;
	Boolean		firstOpen;
	UInt32		context;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuFirstOpen, firstOpen);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
										
	return DoMenuOpening(ioEvent, menu, firstOpen, context);
}


// ===========================================================================
//	MenuClosedDoer

OSStatus
MenuClosedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef		menu;
	UInt32		context;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
										
	return DoMenuClosed(ioEvent, menu, context);
}


// ===========================================================================
//	MenuTargetItemDoer

OSStatus
MenuTargetItemDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef				menu;
	MenuItemIndexType	item;
	MenuCommandType		command;
	UInt32				context;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuItemIndex, item);
	SysEventParam::Get(ioEvent, kEventParamMenuCommand, command);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
										
	return DoMenuTargetItem(ioEvent, menu, item, command, context);
}


// ===========================================================================
//	MenuMatchKeyDoer

OSStatus
MenuMatchKeyDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef					menu;
	EventRef				menuEvent;
	MenuEventOptionsType	options;
	UInt32					context;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamEventRef, menuEvent);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
	
	MenuItemIndexType	item(0);
	
	OSStatus	status = DoMenuMatchKey(ioEvent, menu, menuEvent, options,
										context, item);
										
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamMenuItemIndex, item);
	}
	
	return status;
}


// ===========================================================================
//	MenuEnableItemsDoer

OSStatus
MenuEnableItemsDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef		menu;
	Boolean		isKeyEvent;
	UInt32		context;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamEnableMenuForKeyEvent, isKeyEvent);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
										
	return DoMenuEnableItems(ioEvent, menu, isKeyEvent, context);
}


// ===========================================================================
//	MenuPopulateDoer

OSStatus
MenuPopulateDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef				menu;
	UInt32				context;
	MenuCommandType		command = commandID_None;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuContext, context);
	
	SysEventParam::GetOptional(ioEvent, kEventParamMenuCommand, command);
										
	return DoMenuPopulate(ioEvent, menu, context, command);
}


// ===========================================================================
//	MenuMeasureItemWidthDoer

OSStatus
MenuMeasureItemWidthDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef				menu;
	MenuItemIndexType	item;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuItemIndex, item);
	
	SInt16		width = 0;
	
	OSStatus	status = DoMenuMeasureItemWidth(ioEvent, menu, item, width);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamMenuItemWidth, width);
	}
	
	return status;
}


// ===========================================================================
//	MenuMeasureItemHeightDoer

OSStatus
MenuMeasureItemHeightDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef				menu;
	MenuItemIndexType	item;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuItemIndex, item);
	
	SInt16		height = 0;
	
	OSStatus	status = DoMenuMeasureItemHeight(ioEvent, menu, item, height);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamMenuItemHeight, height);
	}
	
	return status;
}


// ===========================================================================
//	MenuDrawItemDoer

OSStatus
MenuDrawItemDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef					menu;
	Rect					currentBounds;
	MenuItemIndexType		item;
	Rect					itemBounds;
	SInt32					virtualTop;
	SInt32					virtualBottom;
	ThemeMenuStateType		menuState;
	ThemeMenuItemTypeType	itemType;
	CGContextRef			cgContext;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamCurrentBounds, currentBounds);
	SysEventParam::Get(ioEvent, kEventParamMenuItemIndex, item);
	SysEventParam::Get(ioEvent, kEventParamMenuItemBounds, itemBounds);
	SysEventParam::Get(ioEvent, kEventParamMenuVirtualTop, virtualTop);
	SysEventParam::Get(ioEvent, kEventParamMenuVirtualBottom, virtualBottom);
	SysEventParam::Get(ioEvent, kEventParamMenuDrawState, menuState);
	SysEventParam::Get(ioEvent, kEventParamMenuItemType, itemType);
	SysEventParam::Get(ioEvent, kEventParamCGContextRef, cgContext);
										
	return DoMenuDrawItem(ioEvent, menu, currentBounds, item, itemBounds,
							virtualTop, virtualBottom, menuState,
							itemType, cgContext);
}


// ===========================================================================
//	MenuDrawItemContentDoer

OSStatus
MenuDrawItemContentDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef					menu;
	MenuItemIndexType		item;
	Rect					itemBounds;
	SInt16					deviceDepth;
	Boolean					deviceHasColor;
	CGContextRef			cgContext;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
	SysEventParam::Get(ioEvent, kEventParamMenuItemIndex, item);
	SysEventParam::Get(ioEvent, kEventParamMenuItemBounds, itemBounds);
	SysEventParam::Get(ioEvent, kEventParamDeviceDepth, deviceDepth);
	SysEventParam::Get(ioEvent, kEventParamDeviceColor, deviceHasColor);
	SysEventParam::Get(ioEvent, kEventParamCGContextRef, cgContext);
										
	return DoMenuDrawItemContent(ioEvent, menu, item, itemBounds, deviceDepth,
									deviceHasColor, cgContext);
}


// ===========================================================================
//	MenuDisposeDoer

OSStatus
MenuDisposeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	MenuRef		menu;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, menu);
										
	return DoMenuDispose(ioEvent, menu);
}


} // namespace PPx