// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	Constructor.h				© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//	Prefix file for Constructor sources. Typically included in the
//	precompile header sources.
//
//	 Created: 03/30/95
//	   $Date: 2006/04/12 08:47:44 $
//	$History: Constructor.h $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 03/18/97   Time: 13:54
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Changed #ifndef Constructor_WindowsHost to #if
//	!Constructor_WindowsHost.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/01/97   Time: 14:40
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Added Constructor_UseRF and Constructor_UseRW macros.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/01/97   Time: 13:54
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Added Windows target flags.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/22/97   Time: 19:45
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Added #include <new> to avoid preprocessor problems with #define new
//	NEW.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/06/96   Time: 00:27
//	Updated in $/Constructor/Source files/+ Headers
//	Fixed a bug in ValidateObject_ for Java target.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/05/96   Time: 00:23
//	Updated in $/Constructor/Source files/+ Headers
//	Fixed bug in ValidateSimpleObject_ for const objects.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/24/96   Time: 01:28
//	Checked in '$/Constructor/Source files/+ Headers'
//	Comment: Removed ValidateThis_ macro for shipping builds.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:28
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

// ===========================================================================
//		* Constructor build flags
// ===========================================================================
//	Set these switches to 1 or 0, but do not delete.

#ifndef Constructor_ForMacOS
	#define Constructor_ForMacOS	1			// build PowerPlant target of Constructor
#endif

#ifndef Constructor_ForWindows
	#define Constructor_ForWindows	0			// build Windows target of Constructor
#endif

#ifndef Constructor_ForJava
	#define Constructor_ForJava		0			// build Java target of Constructor
#endif

#ifndef Constructor_ForPilot
	#define Constructor_ForPilot	0			// build Pilot target of Constructor
#endif

#ifndef WINVER
	#define Constructor_MacOSHost	1			// build MacOS-hosted Constructor
	#define Constructor_WindowsHost	0
#else
	#define Constructor_MacOSHost	0
	#define Constructor_WindowsHost	1			// build Windows-hosted Constructor
#endif

#ifndef Constructor_Debug
	#define Constructor_Debug		0
#endif

#ifndef Constructor_Debug
	#define Constructor_Carbon		0			// build non-Carbon Constructor
#endif

// ===========================================================================
//		* PowerPlant configuration
// ===========================================================================
/*
#undef Debug_Throw								// no need for exception debugging:
												//	MW debugger does this already
*/

#if Constructor_Debug
	#ifndef Debug_Throw
	#define Debug_Throw
	#endif
	#if !Constructor_WindowsHost
	    #define	Debug_Signal							// assertion debugging is useful
	#endif
#endif


	// Target flags
#ifndef __GNUC__
	#define PP_Target_Carbon		Constructor_Carbon

	#define PP_Target_Classic		(!PP_Target_Carbon)

	#define TARGET_API_MAC_CARBON	PP_Target_Carbon
	#define TARGET_API_MAC_OS8		PP_Target_Classic
#endif

// JCD 980608 - set up PP Debugging classes

#if Constructor_Debug
	#define PP_DEBUG						1
	#define PP_Debug						1
	#define	PP_USE_MOREFILES
	#define	PP_SPOTLIGHT_SUPPORT			PP_Target_Classic
	#define PP_Spotlight_Support			PP_Target_Classic
	#define	PP_QC_SUPPORT					PP_Target_Classic
	#define PP_QC_Support					PP_Target_Classic
	#define PP_DEBUGNEW_SUPPORT				1
	#define PP_DebugNew_Support				1
	#define DEBUG_NEW						DEBUG_NEW_LEAKS
#else
	#define PP_DEBUG						0
	#define PP_Debug						0
	#define	PP_USE_MOREFILES
	#define	PP_SPOTLIGHT_SUPPORT			0
	#define PP_Spotlight_Support			0
	#define	PP_QC_SUPPORT					0
	#define PP_QC_Support					0
	#define PP_DEBUGNEW_SUPPORT				0
	#define PP_DebugNew_Support				0
	#define DEBUG_NEW						DEBUG_NEW_OFF
#endif

// ---------------------------------------------------------------------------
//	Options
	
#ifndef __GNUC__
	#define PP_Uses_PowerPlant_Namespace	0
	#define PP_Supports_Pascal_Strings		1
	#define PM_USE_SESSION_APIS				0 // icw -- Turning this on requires CarbonLib 1.2 or greater

	#if PP_Target_Carbon
		#define PP_StdDialogs_Option			PP_StdDialogs_NavServicesOnly
	#else
		#define PP_StdDialogs_Option			PP_StdDialogs_Conditional
	#endif

	#define PP_Uses_Old_Integer_Types		0
	#define PP_Obsolete_AllowTargetSwitch	0
	#define PP_Obsolete_ThrowExceptionCode	0
	#define PP_Warn_Obsolete_Classes		1

	#define PP_Suppress_Notes_22			1

	#include <MSLCarbonPrefix.h>
#endif
#include <cmath>

// ---------------------------------------------------------------------------
//	Classic mode helpers to make it look more like Carbon
//	(I dunno why these aren't in the Universal Interfaces...  they should be.)
	
#if !PP_Target_Carbon
#define GetWindowFromPort(thePort) thePort
#endif

#include <PP_DebugMacros.h>

#include <new>
using namespace std;

#ifndef __GNUC__
	#include <PP_ClassHeaders.cp>					// include most class definitions
#endif

#include <fp.h>

#define SKIPOMPARSE 1


#define Constructor_Mercutio	PP_Target_Classic

// ===========================================================================
//		* Resource file configuration macros
// ===========================================================================

#if Constructor_ForWindows
	#define Constructor_UseRF		0
	#define Constructor_UseRW		1
#else
	#define Constructor_UseRF		1
	#define Constructor_UseRW		0
#endif


// ===========================================================================
//		* Portability macros
// ===========================================================================

	// -----------------------------------------------------------------------
	//		* dynamic_const_cast macro
	// -----------------------------------------------------------------------
	// The Microsoft compiler currently being used to compile the Constructor
	// code on windows will not allow a const object of a base class to be
	// cast to a const object of a derived type via dynamic_cast. Hence,
	// for the Windows version, const-ness is first removed and then the
	// dynamic cast is done. dynamic_cast under the Metrowerks compiler does
	// allow this usage, so the casting macro simply uses dynamic_cast.
	
#ifndef Constructor_WindowsHost
	#define dynamic_const_cast(obj,fromType,toType) (dynamic_cast<const toType>(obj))
#else
	#define dynamic_const_cast(obj,fromType,toType) (dynamic_cast<toType>(const_cast<fromType>(obj)))
#endif


// ===========================================================================
//		* Debugging infrastructure
// ===========================================================================

// JCD 980608 - commented out most of these debugging macros as they already
// exist in the PP Debugging Classes.

#if Constructor_Debug
	
	// Turn off optimizations that make it impossible to set breakpoints
	// in the MW debugger.

	#pragma optimize_for_size off
	#pragma peephole off
	
	// Define pointer/handle/object validation routines.
/*	
	#if Constructor_MacOSHost
		#define DEBUG_NEW DEBUG_NEW_LEAKS
		#define DebugNewValidatePtr_(x) DebugNewValidatePtr(x)
	#else
		#define DEBUG_NEW DEBUG_NEW_BASIC
		#define DebugNewValidatePtr_(x) (void) 0
	#endif
	

	// Draw in debugging tools suite.

	#include "DebugNew.h"
	#include "SpotlightAPI.h"
	#include "QCAPI.h"
	

	// -----------------------------------------------------------------------
	//		* ValidatePointer_
	// -----------------------------------------------------------------------
	//	ValidatePointer_ should be called before dereferencing any pointer.
	//	It may be used with Memory Manager Ptr pointers only.
	
	#define ValidatePtr_(ptr)											\
		do {																\
			SignalIf_(ptr == nil);											\
			ThrowIf_(ptr == nil);											\
			QCVerifyPtr(ptr);												\
		} while (false)


	// -----------------------------------------------------------------------
	//		* ValidateHandle_
	// -----------------------------------------------------------------------
	//	ValidateHandle_ should be called before dereferencing any handle.
	//	It may be used with Memory Manager Handles only.
	
	#define ValidateHandle_(handle)											\
		do {																\
			SignalIf_(handle == nil);										\
			ThrowIf_(handle == nil);										\
			SignalIf_((*handle) == nil);									\
			QCVerifyHandle(handle);											\
		} while (false)


	// -----------------------------------------------------------------------
	//		* ValidateObject_
	// -----------------------------------------------------------------------
	//	ValidateObject_ should be called before dereferencing any
	//	C++ object. ValidateObject_ must NOT be used for stack-based
	//	objects.

	#define ValidateObject_(obj)											\
		do {																\
			SignalIf_(obj == nil);											\
			ThrowIf_(obj == nil);											\
			SLDisable();													\
			DebugNewValidatePtr_(const_cast<void*>							\
					(dynamic_cast<const void*>(obj)));						\
			SLEnable();														\
		} while (false)


	// -----------------------------------------------------------------------
	//		* ValidateSimpleObject_
	// -----------------------------------------------------------------------
	//	ValidateSimpleObject_ is a variant of ValidateObject_ for
	//	objects which do not have virtual members.
	
	#define ValidateSimpleObject_(obj)										\
		do {																\
			SignalIf_(obj == nil);											\
			ThrowIf_(obj == nil);											\
			SLDisable();													\
			DebugNewValidatePtr_((void*) obj);								\
			SLEnable();														\
		} while (false)
	

	// -----------------------------------------------------------------------
	//		* ValidateThis_
	// -----------------------------------------------------------------------
	//	ValidateThis_ is a shortcut for ValidateObject_(this).
	//	It should be placed at the top of object methods where practical.
	//
	//	ValidateThis_ must NOT be used for stack-based classes.

	#define ValidateThis_() ValidateObject_(this)

*/
	// -----------------------------------------------------------------------
	//		* operator new
	// -----------------------------------------------------------------------
	//	In the MacOS target, we can coerce all occurrences of "new"
	//	to DebugNew's placement new syntax with this preprocessor

	#if Constructor_MacOSHost
		#define new NEW
	#endif
#else

// ===========================================================================
//		* Disabled debugging infrastructure
// ===========================================================================
//	This is a non-debug target (shipping or profiling).
//	Deactivate or simplify all debugging infrastructure.

/*
	// Define simple pointer/handle/object validation routines.

	#define ValidatePtr_(ptr)	 		ThrowIf_(ptr == nil)
	#define ValidateHandle_(handle)		ThrowIf_(handle == nil)
	#define ValidateObject_(obj)		ThrowIf_(obj == nil)
	#define ValidateSimpleObject_(obj)	ThrowIf_(obj == nil)
	#define ValidateThis_()				(void) 0
*/

#endif
