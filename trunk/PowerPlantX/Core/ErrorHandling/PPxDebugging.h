// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDebugging.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDebugging.h
	@brief		Debugging Utilities
*/

#ifndef H_PPxDebugging
#define H_PPxDebugging
#pragma once

#include <PPxOptions.h>
#include <PPxTypes.h>

namespace PPx {

class	Exception;

// ===========================================================================
//	SourceLocation
/**
	Location within a source file */

struct SourceLocation {

	const char*				function;
	const char*				file;
	int						line;

	SourceLocation();

	SourceLocation(
			const char*		inFunction,
			const char*		inFile,
			int				inLine);
};


// ---------------------------------------------------------------------------
//	Macros for specifying the source location. For non-debug builds, the
//	locations are nil values.

							/** Nil source code location. Used when debugging
								options are off. */
#if !PPx_Debug_Exceptions || !PPx_Debug_Signals
	extern const SourceLocation		sourceLocation_Nothing;
#endif

							/** Location within source code. Set to a nil
								location when debugging exceptions is off. */
#if PPx_Debug_Exceptions
	#define PPx_ExceptLoc_Here	PPx::SourceLocation(__FUNCTION__, __FILE__, __LINE__)
#else
	#define PPx_ExceptLoc_Here	PPx::sourceLocation_Nothing
#endif

							/** Location within source code. Set to a nil
								location when debugging signals is off. */
#if PPx_Debug_Signals
	#define PPx_SignalLoc_Here	PPx::SourceLocation(__FUNCTION__, __FILE__, __LINE__)
#else
	#define PPx_SignalLoc_Here	PPx::sourceLocation_Nothing
#endif


// ===========================================================================
//	Debugging
/**
	Utility functions for debugging exceptions and signals */

namespace Debugging {

// ---------------------------------------------------------------------------
/** Possible actions for Throw and Signal debugging */

typedef enum {
	debugAction_Nothing				= 0,
	debugAction_Alert				= 1,
	debugAction_Debugger			= 2,
	debugAction_Console				= 3
} EDebugAction;


// ---------------------------------------------------------------------------
//	Debugging exceptions

#if PPx_Debug_Exceptions

	extern EDebugAction		gDebugThrowAction;
	
	void	DebugException( const Exception& inException );
	
	void	ExceptionAlert( const Exception& inException );
	
#else

	inline void	DebugException( const Exception& ) { }
	
	inline void ExceptionAlert( const Exception& ) { }
	
#endif


// ---------------------------------------------------------------------------
//	SetDebugThrowAction
/**
	Sets debugging action to take when throwing an exception
	
	@param	inAction	Action to take. May be debugAction_Noting,
						debugAction_Alert, debugAction_Debugger,
						or debugAction_Console								*/

inline void
SetDebugThrowAction(
	EDebugAction	inAction)
{
	#if PPx_Debug_Exceptions
		gDebugThrowAction = inAction;
	#else
		#pragma unused(inAction)
	#endif
}


// ---------------------------------------------------------------------------
//	PPx_SetDebugThrow_Nothing_()
/**
	Sets option to do nothing extra when throwing an exception */

#define	PPx_SetDebugThrow_Nothing_()										\
			PPx::Debugging::SetDebugThrowAction(							\
								PPx::Debugging::debugAction_Nothing)


// ---------------------------------------------------------------------------
//	PPx_SetDebugThrow_Alert_()
/**
	Sets option to display a modal alert when throwing an exception */

#define	PPx_SetDebugThrow_Alert_()											\
			PPx::Debugging::SetDebugThrowAction(							\
								PPx::Debugging::debugAction_Alert)


// ---------------------------------------------------------------------------
//	PPx_SetDebugThrow_Debugger_()
/**
	Sets option to break into the debugger when throwing an exception */

#define	PPx_SetDebugThrow_Debugger_()										\
			PPx::Debugging::SetDebugThrowAction(							\
								PPx::Debugging::debugAction_Debugger)


// ---------------------------------------------------------------------------
//	PPx_SetDebugThrow_Console_()
/**
	Sets option to write to the console when throwing an exception */

#define	PPx_SetDebugThrow_Console_()										\
			PPx::Debugging::SetDebugThrowAction(							\
								PPx::Debugging::debugAction_Console)


// ---------------------------------------------------------------------------

#if PPx_Debug_Signals

	extern EDebugAction		gDebugSignalAction;

	void	DebugSignal(
					const char*				inMessage,
					const SourceLocation&	inWhere);

	void	SignalAlert(
					const char*				inMessage,
					const SourceLocation&	inWhere);


	// -----------------------------------------------------------------------
	//	PPx_SignalString_( str )
	/**
		Raises a signal with the specified C string */
	
	#define	PPx_SignalString_(str)									\
				PPx::Debugging::DebugSignal(str, PPx_SignalLoc_Here)


	// -----------------------------------------------------------------------
	//	PPx_SignalIf_( test )
	/**
		Raises a signal if the test condition is true */
	
	#define PPx_SignalIf_(test)										\
				if (test)											\
					PPx_SignalString_(#test);


	// -----------------------------------------------------------------------
	//	PPx_SignalIfNot_( test )
	/**
		Raises a signal if the test condition is false */
	
	#define PPx_SignalIfNot_(test)		PPx_SignalIf_(not (test))
	
#else

	#define PPx_SignalString_(str)
	#define PPx_SignalIf_(test)
	#define PPx_SignalIfNot_(test)
	
#endif


// ---------------------------------------------------------------------------
//	SetDebugSignalAction
/**
	Sets debugging action to take when raising a PPx signal
	
	@param	inAction	Action to take. May be debugAction_Noting,
						debugAction_Alert, debugAction_Debugger,
						or debugAction_Console								*/

inline void
SetDebugSignalAction(
	EDebugAction	inAction)
{
	#if PPx_Debug_Signals
		gDebugSignalAction = inAction;
	#else
		#pragma unused(inAction)
	#endif
}


// ---------------------------------------------------------------------------
//	PPx_SetDebugSignal_Nothing_()
/**
	Sets option to do nothing when raising a PPx signal */

#define	PPx_SetDebugSignal_Nothing_()										\
			PPx::Debugging::SetDebugSignalAction(							\
								PPx::Debugging::debugAction_Nothing)


// ---------------------------------------------------------------------------
//	PPx_SetDebugSignal_Alert_()
/**
	Sets option to display a modal alert when raising a PPx signal */

#define	PPx_SetDebugSignal_Alert_()											\
			PPx::Debugging::SetDebugSignalAction(							\
								PPx::Debugging::debugAction_Alert)


// ---------------------------------------------------------------------------
//	PPx_SetDebugSignal_Debugger_()
/**
	Sets option to break into the debugger when raising a PPx signal */

#define	PPx_SetDebugSignal_Debugger_()										\
			PPx::Debugging::SetDebugSignalAction(							\
								PPx::Debugging::debugAction_Debugger)


// ---------------------------------------------------------------------------
//	PPx_SetDebugSignal_Console_()
/**
	Sets option to write to the console when raising a PPx signal */

#define	PPx_SetDebugSignal_Console_()										\
			PPx::Debugging::SetDebugSignalAction(							\
								PPx::Debugging::debugAction_Console)


// ---------------------------------------------------------------------------
//	Pascal string utilities

StringPtr		CopyPStr(
						ConstStringPtr	inSourceString,
						StringPtr		outDestString,
						SInt16			inDestSize = sizeof(Str255));

StringPtr		LoadPStrFromCStr(
						Str255			outPStr,
						UInt8			inMaxChars,
						const char*		inCStr);

StringPtr		AppendPStr(
						Str255			ioBaseString,
						ConstStringPtr	inAppendString,
						SInt16			inDestSize = sizeof(Str255));			
	
	
} // namespace debugging


// ===========================================================================
//	MenuDebugStr
/**
	Debugging utility functions for displaying information in the menu bar.
	
	Stopping at a breakpoint in the source debugger or posting an alert
	may disturb the state of a program by obscuring windows or generating
	window deactivate/activate events. This may hamper debugging of code
	dealing with window drawing and event handling. In such cases, you can
	use these routines to display debugging information in the menu bar.	*/

namespace MenuDebugStr {

	void	Display( ConstStringPtr inString, unsigned long inDelay = 120 );
	
	void	Display( const char* inCString, unsigned long inDelay = 120 );
	
	void	Display( long inNumber, unsigned long inDelay = 120 );

}

} // namespace PPx

#endif	// H_PPxDebugging
