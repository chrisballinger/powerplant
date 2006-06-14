// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysEventParam.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:34 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysEventParam.h
	@brief		Utility functions for getting and setting Carbon Event
				parameters
*/

#ifndef H_SysEventParam
#define H_SysEventParam
#pragma once

#include <PPxSysTypes.h>
#include <SysEventTypes.h>
#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysEventParam
/**
	Utility functions for getting and setting Carbon Event parameters */

namespace SysEventParam {

	// -----------------------------------------------------------------------
	/**	Traits for associating a C++ type with an EventParamType value.
		
		By using traits based on template specializations, we essentially
		create a table of (C++ Type, EventParamType) pairs. This table is
		a compile-time construct and does not use any runtime memory.
		
		Template functions use the traits to access the EventParamType
		for a C++ type.														*/

	template <typename TData> struct Traits;
	
		/** Macro for declaring Traits template specializations */
	
	#define	PPx_Declare_SysEventParam_Traits(DataT, ParamT)		\
		template <> struct Traits< DataT > {					\
			static const EventParamType type = ParamT;			\
		}


		// Boolean Type
		
		// There is no declaration for "bool". The system stores
		// parameters as Boolean values, and there is no guarantee
		// that sizeof(Boolean) is the same as sizeof(bool). You must
		// use Boolean rather than bool variables when getting/setting
		// values of typeBoolean.

	PPx_Declare_SysEventParam_Traits( Boolean,		typeBoolean );


		// Numeric Types
	
	PPx_Declare_SysEventParam_Traits( char,			typeChar );
	PPx_Declare_SysEventParam_Traits( SInt16,		typeSInt16 );
	PPx_Declare_SysEventParam_Traits( SInt32,		typeSInt32 );
	PPx_Declare_SysEventParam_Traits( UInt32,		typeUInt32 );
	PPx_Declare_SysEventParam_Traits( SInt64,		typeSInt64 );
	PPx_Declare_SysEventParam_Traits( Float32,		typeShortFloat );
	PPx_Declare_SysEventParam_Traits( Float64,		typeFloat );


		// High-level Toolbox Types
		
	PPx_Declare_SysEventParam_Traits( HIObjectRef,	typeHIObjectRef );
	PPx_Declare_SysEventParam_Traits( WindowRef,	typeWindowRef );
	PPx_Declare_SysEventParam_Traits( ControlRef,	typeControlRef );
	PPx_Declare_SysEventParam_Traits( MenuRef,		typeMenuRef );
	PPx_Declare_SysEventParam_Traits( ScrapRef,		typeScrapRef );
	PPx_Declare_SysEventParam_Traits( DragRef,		typeDragRef );
	PPx_Declare_SysEventParam_Traits( EventRef,		typeEventRef );
	
	PPx_Declare_SysEventParam_Traits( EventTargetRef,
													typeEventTargetRef );
	PPx_Declare_SysEventParam_Traits( HIToolbarRefType,
													typeHIToolbarRef );
	PPx_Declare_SysEventParam_Traits( HIToolbarItemRefType,
													typeHIToolbarItemRef );
	
	PPx_Declare_SysEventParam_Traits( HICommand,	typeHICommand );
	PPx_Declare_SysEventParam_Traits( ControlPartCodeType,
													typeControlPartCode );
	PPx_Declare_SysEventParam_Traits( ControlActionUPP,
													typeControlActionUPP );
	PPx_Declare_SysEventParam_Traits( EventHotKeyID,
													typeEventHotKeyID );
	PPx_Declare_SysEventParam_Traits( MouseTrackingRef,
													typeMouseTrackingRef );
	PPx_Declare_SysEventParam_Traits( EventMouseWheelAxisType,
													typeMouseWheelAxis );
	PPx_Declare_SysEventParam_Traits( MenuTrackingModeType,
													typeMenuTrackingMode );
	PPx_Declare_SysEventParam_Traits( MenuItemIndexType,
													typeMenuItemIndex );
	PPx_Declare_SysEventParam_Traits( MenuCommandType,
													typeMenuCommand );
	PPx_Declare_SysEventParam_Traits( MenuEventOptionsType,
													typeMenuEventOptions );
	PPx_Declare_SysEventParam_Traits( ThemeMenuStateType,
													typeThemeMenuState );
	PPx_Declare_SysEventParam_Traits( ThemeMenuItemTypeType,
													typeThemeMenuItemType );
	PPx_Declare_SysEventParam_Traits( WindowDefPartCodeType,
													typeWindowDefPartCode );
	PPx_Declare_SysEventParam_Traits( WindowRegionCodeType,
													typeWindowRegionCode );
	
	
		// Core Foundation Types
		
	PPx_Declare_SysEventParam_Traits( CFStringRef,	typeCFStringRef );
	PPx_Declare_SysEventParam_Traits( CFMutableStringRef,
													typeCFMutableStringRef );
	PPx_Declare_SysEventParam_Traits( CFTypeRef,	typeCFTypeRef );
	PPx_Declare_SysEventParam_Traits( CFMutableArrayRef,
													typeCFMutableArrayRef );
	PPx_Declare_SysEventParam_Traits( AXUIElementRef,
													typeCFTypeRef );

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

	PPx_Declare_SysEventParam_Traits( CFRange,		typeCFRange );

#endif // 10.3 or later
	
		// Core Graphics Types
		
	PPx_Declare_SysEventParam_Traits( CGContextRef,	typeCGContextRef );
	PPx_Declare_SysEventParam_Traits( HIPoint,		typeHIPoint );
	PPx_Declare_SysEventParam_Traits( HISize,		typeHISize );
	PPx_Declare_SysEventParam_Traits( HIRect,		typeHIRect );


		// Quickdraw Types
		
	PPx_Declare_SysEventParam_Traits( GrafPtr,		typeGrafPtr );
	PPx_Declare_SysEventParam_Traits( Point,		typeQDPoint );
	PPx_Declare_SysEventParam_Traits( Rect,			typeQDRectangle );
	PPx_Declare_SysEventParam_Traits( RgnHandle,	typeQDRgnHandle );
	
	
		// File Manager Types
		
	PPx_Declare_SysEventParam_Traits( FSRef,		typeFSRef );
	PPx_Declare_SysEventParam_Traits( FSVolumeRefNumType,
													typeFSVolumeRefNum );
		
	
		// Other Toolbox Types
		
	PPx_Declare_SysEventParam_Traits( OSStatusType,	typeOSStatus );
	PPx_Declare_SysEventParam_Traits( UniCharType,	typeUnicodeText );
	PPx_Declare_SysEventParam_Traits( OSTypeType,	typeType );
	PPx_Declare_SysEventParam_Traits( ProcessSerialNumber,
													typeProcessSerialNumber );
	

	// -----------------------------------------------------------------------
	/**	Set a parameter for a Carbon Event
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to set
		@param	inValue		Parameter value									*/
	
	template <typename TData>
	void
	Set(
		EventRef		inEvent,
		EventParamName	inName,
		const TData&	inValue)
	{
		typedef Traits<TData>	DataTraits;
		
		OSStatus	status = ::SetEventParameter( inEvent, inName,
												  DataTraits::type,
												  sizeof(TData),
												  &inValue );
												  
		PPx_ThrowIfOSError_(status, "SetEventParameter failed");
	}
	

	// -----------------------------------------------------------------------
	/**	Set a parameter for a Carbon Event
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to set
		@param	inType		Type ID of the parameter
		@param	inValue		Parameter value									*/
	
	template <typename TData>
	void
	Set(
		EventRef		inEvent,
		EventParamName	inName,
		EventParamType	inType,
		const TData&	inValue)
	{
		OSStatus	status = ::SetEventParameter( inEvent, inName,
												  inType,
												  sizeof(TData),
												  &inValue );
												  
		PPx_ThrowIfOSError_(status, "SetEventParameter failed");
	}


	// -----------------------------------------------------------------------
	/**	Get a parameter from a Carbon Event. Throws if there is an error
		getting the parameter.
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to get
		@param	outValue	Parameter value									*/

	template <typename TData>
	void
	Get(
		EventRef		inEvent,
		EventParamName	inName,
		TData&			outValue)
	{
		typedef Traits<TData>	DataTraits;
		
		OSStatus	status = ::GetEventParameter( inEvent, inName,
												  DataTraits::type, nil,
												  sizeof(TData), nil,
												  &outValue );
												  
		PPx_ThrowIfOSError_(status, "GetEventParameter failed");
	}


	// -----------------------------------------------------------------------
	/**	Get a parameter from a Carbon Event. Throws if there is an error
		getting the parameter.
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to get
		@param	inType		Type ID of the parameter
		@param	outValue	Parameter value									*/

	template <typename TData>
	void
	Get(
		EventRef		inEvent,
		EventParamName	inName,
		EventParamType	inType,
		TData&			outValue)
	{
		OSStatus	status = ::GetEventParameter( inEvent, inName,
												  inType, nil,
												  sizeof(TData), nil,
												  &outValue );
												  
		PPx_ThrowIfOSError_(status, "GetEventParameter failed");
	}


	// -----------------------------------------------------------------------
	/**	Get the size of a parameter from a Carbon Event. Throws if there is an
		error getting the parameter size.
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to get
		@param	outSize		The parameter size								*/

	inline
	void
	GetSize(
		EventRef		inEvent,
		EventParamName	inName,
		UInt32&			outSize)
	{
		OSStatus	status = ::GetEventParameter( inEvent, inName,
												  typeWildCard, nil,
												  0, &outSize, nil );
												  
		PPx_ThrowIfOSError_(status, "GetEventParameter failed");
	}


	// -----------------------------------------------------------------------
	/**	Get an optional parameter from a Carbon Event. Check the return
		value to determine if the parameter is present.
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to get
		@param	outValue	Parameter value
		
		@return		A Mac OS error code										*/
	
	template <typename TData>
	OSStatus
	GetOptional(
		EventRef		inEvent,
		EventParamName	inName,
		TData&			outValue)
	{
		typedef Traits<TData>	DataTraits;
		
		OSStatus	status = ::GetEventParameter( inEvent, inName,
												  DataTraits::type, nil,
												  sizeof(TData), nil,
												  &outValue );

		return status;
	}


	// -----------------------------------------------------------------------
	/**	Get an optional parameter from a Carbon Event. Check the return
		value to determine if the parameter is present.
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to get
		@param	inType		Type ID of the parameter
		@param	outValue	Parameter value
		
		@return		A Mac OS error code										*/
	
	template <typename TData>
	OSStatus
	GetOptional(
		EventRef		inEvent,
		EventParamName	inName,
		EventParamType	inType,
		TData&			outValue)
	{
		OSStatus	status = ::GetEventParameter( inEvent, inName,
												  inType, nil,
												  sizeof(TData), nil,
												  &outValue );

		return status;
	}


	// -----------------------------------------------------------------------
	/**	Get the size of an optional parameter from a Carbon Event. Check the
		return value to determine if the parameter is present.
		
		@param	inEvent		A reference to a Carbon Event
		@param	inName		Name of the parameter to get
		@param	outSize		The parameter size
		
		@return		A Mac OS error code										*/

	inline
	OSStatus
	GetSizeOptional(
		EventRef		inEvent,
		EventParamName	inName,
		UInt32&			outSize)
	{
		OSStatus	status = ::GetEventParameter( inEvent, inName,
												  typeWildCard, nil,
												  0, &outSize, nil );
												  
		return status;
	}
	

} // namespace SysEventParam


} // namespace PPx

#endif	// H_SysEventParam
