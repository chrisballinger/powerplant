// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSysTypes.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSysTypes.h
	@brief		Wrapper classes for Toolbox integer types
	
	PPx uses function overloading and template argument deduction based
	on the type of parameters. This requires that different kinds of
	parameters have unique types.
	
	The Toolbox uses typedefs to define descriptive type names for what
	are actually integer types. For example, OSStatus is a signed long.
	However, a typedef declares an alias and not a unique type. So, to
	the C++ compiler, OSStatus and signed long are the same type, and
	can't be used to distinguish between different overloaded functions.
	
	The structs in this file are wrappers for integer types so that they
	have a unique type. Each struct has a single data member, a constructor
	with a default value, and coercion operators to the underlying type.
	The name of each struct is the same as the name of the Toolbox typedef
	with the word "Type" appended.
*/

#ifndef H_PPxSysTypes
#define H_PPxSysTypes
#pragma once

#include <PPxPrefix.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
/**	Wrapper for FourCharCode */

struct FourCharCodeStruct { };
typedef IntegerType<FourCharCodeStruct, FourCharCode>	FourCharCodeType;


// ===========================================================================
/**	Wrapper for UniChar */

struct UniCharStruct { };
typedef IntegerType<UniCharStruct, UniChar>		UniCharType;


// ===========================================================================
/**	Wrapper for OSStatus */

struct OSStatusStruct { };
typedef IntegerType<OSStatusStruct, OSStatus>	OSStatusType;


// ===========================================================================
/**	Wrapper for ControlPartCode */

struct ControlPartCodeStruct { };
typedef IntegerType<ControlPartCodeStruct, ControlPartCode>
				ControlPartCodeType;


// ===========================================================================
/**	Wrapper for EventMouseWheelAxis */

struct EventMouseWheelAxisStruct { };
typedef IntegerType<EventMouseWheelAxisStruct, EventMouseWheelAxis>
				EventMouseWheelAxisType;


// ===========================================================================
/**	Wrapper for MenuTrackingMode */

struct MenuTrackingModeStruct { };
typedef IntegerType<MenuTrackingModeStruct, MenuTrackingMode>
				MenuTrackingModeType;


// ===========================================================================
/**	Wrapper for MenuItemIndex */

struct MenuItemIndexStruct { };
typedef IntegerType<MenuItemIndexStruct, MenuItemIndex>	MenuItemIndexType;


// ===========================================================================
/**	Wrapper for MenuCommand */

struct MenuCommandStruct { };
typedef IntegerType<MenuCommandStruct, MenuCommand>	MenuCommandType;


// ===========================================================================
/**	Wrapper for MenuEventOptions */

struct MenuEventOptionsStruct { };
typedef IntegerType<MenuEventOptionsStruct, MenuEventOptions>
				MenuEventOptionsType;


// ===========================================================================
/**	Wrapper for ThemeMenuState */

struct ThemeMenuStateStruct { };
typedef IntegerType<ThemeMenuStateStruct, ThemeMenuState>
				ThemeMenuStateType;


// ===========================================================================
/**	Wrapper for ThemeMenuItemType */

struct ThemeMenuItemTypeStruct { };
typedef IntegerType<ThemeMenuItemTypeStruct, ThemeMenuItemType>
				ThemeMenuItemTypeType;


// ===========================================================================
/**	Wrapper for WindowClass */

struct WindowClassStruct { };
typedef IntegerType<WindowClassStruct, WindowClass> WindowClassType;


// ===========================================================================
/**	Wrapper for WindowAttributes */

struct WindowAttributesStruct { };
typedef IntegerType<WindowAttributesStruct, WindowAttributes>
				WindowAttributesType;


// ===========================================================================
/**	Wrapper for WindowDefPartCode */

struct WindowDefPartCodeStruct { };
typedef IntegerType<WindowDefPartCodeStruct, WindowDefPartCode>
				WindowDefPartCodeType;


// ===========================================================================
/**	Wrapper for WindowRegionCode */

struct WindowRegionCodeStruct { };
typedef IntegerType<WindowRegionCodeStruct, WindowRegionCode>
				WindowRegionCodeType;


// ===========================================================================
/**	Wrapper for OSType */

struct OSTypeStruct { };
typedef IntegerType<OSTypeStruct, OSType>	OSTypeType;


// ===========================================================================
/**	Wrapper for FSVolumeRefNum */

struct FSVolumeRefNumStruct { };
typedef IntegerType<FSVolumeRefNumStruct, FSVolumeRefNum>
				FSVolumeRefNumType;


} // namespace PPx

#endif	// H_PPxSysTypes
