// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDebugging.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDebugging.h>
#include <PPxExceptions.h>

#include <Carbon/Carbon.h>

#if PPx_Debug_Exceptions || PPx_Debug_Signals
	#include <iostream>
#endif

namespace PPx {

// ---------------------------------------------------------------------------
//	Global variables for specifying the action to take for
//	a Throw and a Signal

#if PPx_Debug_Exceptions
Debugging::EDebugAction	Debugging::gDebugThrowAction  = debugAction_Nothing;
#endif

#if PPx_Debug_Signals
Debugging::EDebugAction	Debugging::gDebugSignalAction = debugAction_Nothing;
#endif


// ---------------------------------------------------------------------------
//	Constant for empty source location when debugging options are off

#if !PPx_Debug_Exceptions || !PPx_Debug_Signals
const SourceLocation	sourceLocation_Nothing(nil, nil, 0);
#endif


namespace {	// unnamed

	// -----------------------------------------------------------------------
	//	Alert resources and item numbers of buttons

	const	ResIDT	ALRT_Exception	= 253;
	const	ResIDT	ALRT_Signal		= 254;

	const	SInt16	button_Continue	= 1;
	const	SInt16	button_Abort	= 4;
	const	SInt16	button_Debugger	= 5;
	const	SInt16	button_Quiet	= 6;

} // unnamed namespace


// ===========================================================================
//	SourceLocation

// ---------------------------------------------------------------------------
//	SourceLocation						Default Constructor

SourceLocation::SourceLocation()

	: function(nil),
	  file(nil),
	  line(0)
{
}


// ---------------------------------------------------------------------------
//	SourceLocation						Parameterized Constructor

SourceLocation::SourceLocation(
	const char*		inFunction,
	const char*		inFile,
	int				inLine)
	
	: function(inFunction),
	  file(inFile),
	  line(inLine)
{
}

#pragma mark -

// ===========================================================================
//	Debugging
// ===========================================================================

#if PPx_Debug_Exceptions

// ---------------------------------------------------------------------------
//	DebugException
/**
	Takes action when an exception is thrown
	
	@param	inException		PPx exception object							*/

void
Debugging::DebugException(
	const Exception&	inException)
{
	switch (gDebugThrowAction) {
	
		case debugAction_Alert:
			ExceptionAlert(inException);
			break;
			
		case debugAction_Debugger: {
			Str255	whyString;
			::DebugStr(inException.Why(whyString));
			break;
		}
		
		case debugAction_Console: {
			Str255	whyString;
			inException.Why(whyString);
			std::cerr.write((char*) &whyString[1], whyString[0]) << std::endl;
			std::cerr << "  function: " << inException.Where().function;
			std::cerr << "  file: " << inException.Where().file;
			std::cerr << "  line: " << inException.Where().line << std::endl;
			break;
		}
	}
}


// ---------------------------------------------------------------------------
//	ExceptionAlert
/**
	Displays an alert with information about an exception
	
	@param	inException		PPx exception object							*/

void
Debugging::ExceptionAlert(
	const Exception&	inException)
{
	Str255	whyString;
	inException.Why(whyString);

	const SourceLocation	where = inException.Where();
	
	Str63	funcStr;
	LoadPStrFromCStr(funcStr, 63, where.function);
	
	Str63	fileStr;
	LoadPStrFromCStr(fileStr, 63, where.file);
	
	Str15	lineStr;
	::NumToString(where.line, lineStr);
	
	::ParamText(whyString, funcStr, fileStr, lineStr);
	
	SInt16	button = ::StopAlert(ALRT_Exception, nil);

	switch (button) {

//		case button_Continue:		// Do nothing
//			break;

		case button_Abort:
			::ExitToShell();
			break;

		case button_Debugger:
			::Debugger();
			break;

		case button_Quiet:
			gDebugThrowAction  = debugAction_Nothing;
			break;
	}
}

#endif	// PPx_Debug_Exceptions


#if PPx_Debug_Signals

// ---------------------------------------------------------------------------
//	DebugSignal
/**
	Takes action when a signal is raised
	
	@param	inMessage	C string message to display
	@param	inWhere		Source location where signal occurred				*/

void
Debugging::DebugSignal(
	const char*				inMessage,
	const SourceLocation&	inWhere)
{
	switch (gDebugSignalAction) {
	
		case debugAction_Alert:
			SignalAlert(inMessage, inWhere);
			break;
			
		case debugAction_Debugger: {
			Str255	msgString;
			LoadPStrFromCStr(msgString, 255, inMessage);
			::DebugStr(msgString);
		}
		
		case debugAction_Console: {
			std::cerr << inMessage << std::endl;
			std::cerr << "  function: " << inWhere.function;
			std::cerr << "  file: " << inWhere.file;
			std::cerr << "  line: " << inWhere.line << std::endl;
		}
	}
}


// ---------------------------------------------------------------------------
//	SignalAlert
/**
	Displays an alert with information about a signal
	
	@param	inMessage	C string message to display
	@param	inWhere		Source location where signal occurred				*/

void
Debugging::SignalAlert(
	const char*				inMessage,
	const SourceLocation&	inWhere)
{
	Str255	msgStr;
	LoadPStrFromCStr(msgStr, 255, inMessage);
	
	Str63	funcStr;
	Str63	fileStr;
	Str15	lineStr;
	
	LoadPStrFromCStr(funcStr, 63, inWhere.function);
	LoadPStrFromCStr(fileStr, 63, inWhere.file);
	::NumToString(inWhere.line, lineStr);
	
	::ParamText(msgStr, funcStr, fileStr, lineStr);
	
	SInt16	button = ::StopAlert(ALRT_Signal, nil);

	switch (button) {

//		case button_Continue:		// Do nothing
//			break;

		case button_Abort:
			::ExitToShell();
			break;

		case button_Debugger:
			::Debugger();
			break;

		case button_Quiet:
			gDebugSignalAction  = debugAction_Nothing;
			break;
	}
}

#endif	// PPx_Debug_Signals

#pragma mark -

// ---------------------------------------------------------------------------
//	CopyPStr
/**
	Copies a Pascal string
	
	@param	inSourceString	String to copy
	@param	outDestString	Destination string in which to put copy
	@param	inDestSize		Maximum size of destination string, including
								the length byte
								
	@note inSourceString may be nil, in which case outDestString is set
	to a zero length string.												*/

StringPtr
Debugging::CopyPStr(
	ConstStringPtr	inSourceString,
	StringPtr		outDestString,
	SInt16			inDestSize)
{
	SInt16	dataLen = 1;

	if (inSourceString != nil) {
		dataLen = (SInt16) (inSourceString[0] + 1);
		if (dataLen > inDestSize) {
			dataLen = inDestSize;
		}

		::BlockMoveData(inSourceString, outDestString, dataLen);
	}

	outDestString[0] = (UInt8) (dataLen - 1);		// Store length byte

	return outDestString;
}


// ---------------------------------------------------------------------------
//	LoadPStrFromCStr
/**
	Copies contents of a C string into a Pascal string
	
	@param	outPStr		Output Pascal string to which to copy
	@param	inMaxChars	Maximum size of Pascal string
	@param	inCStr		C String to copy
	
	@return	Pointer to the Pascal string									*/

unsigned char*
Debugging::LoadPStrFromCStr(
	Str255			outPStr,
	UInt8			inMaxChars,
	const char*		inCStr)
{
		// Find length of C string by searching for the terminating
		// null character. Limit length to inMaxChars.

	unsigned char	strLength = 0;
	
	if (inCStr != nil) {

		while ( (strLength < inMaxChars)  &&  (inCStr[strLength] != '\0') ) {
			strLength += 1;
		}
										// Copy characters into Pascal string
		::BlockMoveData(inCStr, outPStr + 1, strLength);
	}

	outPStr[0] = strLength;				// Store length byte

	return outPStr;
}


// ---------------------------------------------------------------------------
//	AppendPStr
/**
	Append two Pascal strings. The first string becomes the combination
	of the first and second strings.
	
	@param	ioBaseString	Base string. On exit, the appended string
	@param	inAppendString	String to append
	@param	inDestSize		Maximum size of appended string, including the
								length byte
								
	@return	Pointer to the appended string									*/

StringPtr
Debugging::AppendPStr(
	Str255				ioBaseString,
	ConstStringPtr		inAppendString,
	SInt16				inDestSize)
{
								// Limit combined string to inDestSize chars
    SInt16	charsToCopy = inAppendString[0];
    if (ioBaseString[0] + charsToCopy > inDestSize - 1) {
    	charsToCopy = (SInt16) (inDestSize - 1 - ioBaseString[0]);
    }

								// Copy second to end of first string
    ::BlockMoveData(inAppendString + 1,  ioBaseString + ioBaseString[0] + 1,
    					charsToCopy);
    							// Set length of combined string
    ioBaseString[0] += (UInt8) charsToCopy;

    return ioBaseString;
}

#pragma mark -

// ===========================================================================
//	MenuDebugStr

namespace {
	const MenuID			menuID_DebugStr = 19876;
}


// ---------------------------------------------------------------------------
//	Display
/**
	Temporarily displays a Pascal string in the menu bar
	
	@param	inString	Pascal string
	@param	inDelay		Duration in ticks for displaying the string			*/

void
MenuDebugStr::Display(
	ConstStringPtr	inString,
	unsigned long	inDelay)
{
	MenuRef		menu;
	OSStatus	status = ::CreateNewMenu( menuID_DebugStr, 0, &menu);
	
	if (status == noErr) {
		
		::SetMenuTitle(menu, inString);
		::MacInsertMenu(menu, 0);
		::MacDrawMenuBar();
		
		unsigned long	ticks;
		::Delay(inDelay, &ticks);
		
		::MacDeleteMenu(menuID_DebugStr);
		::ReleaseMenu(menu);
		::MacDrawMenuBar();
	}
}


// ---------------------------------------------------------------------------
//	Display
/**
	Temporarily displays a Pascal string in the menu bar
	
	@param	inCString	C string
	@param	inDelay		Duration in ticks for displaying the string			*/

void
MenuDebugStr::Display(
	const char*		inCString,
	unsigned long	inDelay)
{
		// Find length of C string by searching for the terminating
		// null character. However, don't bother to look past 255
		// characters, since that's all that a Pascal string can hold.

	unsigned char	strLength = 0;

	while ( (strLength < 255)  &&  (inCString[strLength] != '\0') ) {
		strLength += 1;
	}
	
	Str255	pStr;
	pStr[0] = strLength;
	::BlockMoveData(inCString, pStr + 1, strLength);
	
	Display(pStr, inDelay);
}


// ---------------------------------------------------------------------------
//	Display
/**
	Temporarily displays a number in the menu bar
	
	@param	inNumber	Signed integer number
	@param	inDelay		Duration in ticks for displaying the string			*/

void
MenuDebugStr::Display(
	long			inNumber,
	unsigned long	inDelay)
{
	Str15	str;
	::NumToString(inNumber, str);
	Display(str, inDelay);
}


} // namespace PPx