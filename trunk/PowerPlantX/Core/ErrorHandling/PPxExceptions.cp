// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxExceptions.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:11 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxExceptions.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Exception													   [protected]
/**
	Constructor
	
	@param	inWhat		Kind of exception
	@param	inWhy		C string describing why the exception occurred
	@param	inWhere		Source code location where exception was thrown
	
	@note When the option PPx_Debug_Exceptiond is false, the why and
	where are not stored.													*/

Exception::Exception(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	mExceptionID = inWhat;

#if PPx_Debug_Exceptions

	Debugging::LoadPStrFromCStr(mWhyString, 255, inWhy);
	mLocation = inWhere;
	
#else
	#pragma unused(inWhy)
	#pragma unused(inWhere)
#endif
}


// ---------------------------------------------------------------------------
//	~Exception													   [protected]
/**
	Destructor */

Exception::~Exception()
{
}


// ---------------------------------------------------------------------------
//	What															  [public]
/**
	Returns the kind of an exception */

ExceptionIDT
Exception::What() const
{
	return mExceptionID;
}


// ---------------------------------------------------------------------------
//	Why																  [oublic]
/**
	Returns a Pascal string describing why an exception was thrown
	
	@param	outWhy	Pascal string in which to store the description
	
	@note If PPx_Debug_Exceptions is false, description is an empty string	*/

StringPtr
Exception::Why(
	Str255	outWhy) const
{
#if PPx_Debug_Exceptions

	::BlockMoveData(mWhyString, outWhy, mWhyString[0] + 1);
	
#else

	outWhy[0] = 0;

#endif
	
	return outWhy;
}


// ---------------------------------------------------------------------------
//	Where															  [public]
/**
	Returns the source code location where the exception was thrown
	
	@return	Source code location where exception was thrown	
	
	Location specifies the source file name, function name, and line
	number.
	
	@note Location fields are nil if PPx_Debug_Exceptions is false.			*/

const SourceLocation&
Exception::Where() const
{
#if PPx_Debug_Exceptions

	return mLocation;
	
#else

	return sourceLocation_Nothing;
	
#endif
}

#pragma mark -

OSError::ThrowFunc		OSError::sThrowFunc = nil;

// ---------------------------------------------------------------------------
//	OSError															  [public]
/**
	Constructor
	
	@param	inOSError	OS error code
	@param	inWhy		C string describing why the exception occurred
	@param	inWhere		Source code location where exception was thrown
	
	@note If PPx_Debug_Exceptions is false, the why and where are not
	stored.																	*/

OSError::OSError(
	OSStatus				inOSError,
	const char*				inWhy,
	const SourceLocation&	inWhere)
	
	: Exception(err_MacOS, inWhy, inWhere)
{
	mErrorCode = inOSError;
}


// ---------------------------------------------------------------------------
//	Why																  [public]
/**
	Returns a Pascal string describing why an exception was thrown
	
	@param	outWhy	Pascal string in which to store the description
	
	@note If PPx_Debug_Exceptions is false, description is an empty string	*/

StringPtr
OSError::Why(
	Str255	outWhy) const
{
#if PPx_Debug_Exceptions

		// Prepend error code number to the description

	Debugging::CopyPStr("\pMac OS Error = ", outWhy);
	
	Str15	errNumStr;
	::NumToString(mErrorCode, errNumStr);
	
	Debugging::AppendPStr(outWhy, errNumStr);
	Debugging::AppendPStr(outWhy, "\p\r");
	
	Str255	baseWhyStr;
	Debugging::AppendPStr(outWhy, Exception::Why(baseWhyStr));
	
	return outWhy;
	
#else

	return Exception::Why(outWhy);
	
#endif
}


// ---------------------------------------------------------------------------
//	GetOSErrorCode													  [public]
/**
	Returns the OS error code which caused the exception
	
	@return	OS error code													*/

OSStatus
OSError::GetOSErrorCode() const
{
	return mErrorCode;
}


// ---------------------------------------------------------------------------
//	Throw													 [static] [public]
/**
	Throws an OSError exception
	
	@param	inOSError	OS error code
	@param	inWhy		C string description of why the exception was thrown
	@param	inWhere		Source location where exception was throw			*/

void
OSError::Throw(
	OSStatus				inOSError,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	if (sThrowFunc != nil) {
		(*sThrowFunc)(inOSError, inWhy, inWhere);
	}
	
	OSError	theException(inOSError, inWhy, inWhere);
	
	Debugging::DebugException(theException);

	throw theException;
}


// ---------------------------------------------------------------------------
//	SetThrowFunc											 [static] [public]
/**
	Specifies the hook function to call when throwing an OSError exception
	
	@param	inThrowFunc		Pointer to custom throw function
	
	Before throwing an exception when a Toolbox function returns an
	unexpected error code, PPx calls a user-supplied function which
	should throw an explicit instantiation of the template class
	PPx::OSErrorCode<OSStatus> for each OS error code which you wish
	to catch. See <MacErrors.h> for the list of OS error codes.
	For example,
	
	@code
	
	void
	MyThrowOSErrorCode(
		OSStatus					inErrorCode,
		const char*					inWhy,
		const PPx::SourceLocation&	inWhere)
	{
		switch (inErrorCode) {
		
			case fnfErr:	// File not found
				PPx::ThrowOSErrorCode<fnfErr>(inWhy, inWhere);
				break;
				
			case opWrErr:	// File already open with write access
				PPx::ThrowOSErrorCode<opWrErr>(inWhy, inWhere);
				break;
		}
	}
	
	@endcode
		
	Then, somewhere in your code, register your custom throw function,
	and enter a try/catch block.
	
	@code
	
	PPx::OSError::SetThrowFunc( MyThrowOSErrorCode );
	
	try {
		// Do something that may throw exceptions
		
	}
	
	catch ( const PPx::OSErrorCode<fnfErr>& inErr ) {
		// File not found
		// Take specific recovery action for this kind of error
	}
	
	catch ( const PPx::OSErrorCode<opWrErr>& inErr ) {
		// File already open with write permission
		// Take specific recovery action for this kind of error
	}
	
	catch (...) {
		// Some other exceptions
		// Take generic recovery action
	}

	@endcode

	This allows you to take specific recovery actions for certain
	OS errors.																*/
	
void
OSError::SetThrowFunc(
	ThrowFunc	inThrowFunc)
{
	sThrowFunc = inThrowFunc;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	LogicError														  [public]
/**
	Constructor
	
	@param	inWhat		Kind of logic error
	@param	inWhy		C string describing why the exception occurred
	@param	inWhere		Source code location where exception was thrown
	
	@note If PPx_Debug_Exceptions is false, the why and where are not
	stored.																	*/

LogicError::LogicError(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
	
	: Exception(inWhat, inWhy, inWhere)
{
}


// ---------------------------------------------------------------------------
//	Throw													 [static] [public]
/**
	Throws an LogicError exception
	
	@param	inWhat		Kind of logic error
	@param	inWhy		C string description of why the exception was thrown
	@param	inWhere		Source location where exception was throw			*/

void
LogicError::Throw(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	LogicError	theException(inWhat, inWhy, inWhere);
	
	Debugging::DebugException(theException);

	throw theException;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	RuntimeError													  [public]
/**
	Constructor
	
	@param	inWhat		Kind of runtime error
	@param	inWhy		C string describing why the exception occurred
	@param	inWhere		Source code location where exception was thrown
	
	@note If PPx_Debug_Exceptions is false, the why and where are not
	stored.																	*/

RuntimeError::RuntimeError(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
	
	: Exception(inWhat, inWhy, inWhere)
{
}


// ---------------------------------------------------------------------------
//	Throw													 [static] [public]
/**
	Throws a RuntimeError exception
	
	@param	inWhat		Kind of logic error
	@param	inWhy		C string description of why the exception was thrown
	@param	inWhere		Source location where exception was throw			*/

void
RuntimeError::Throw(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	RuntimeError	theException(inWhat, inWhy, inWhere);
	
	Debugging::DebugException(theException);

	throw theException;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	DataError														  [public]
/**
	Constructor
	
	@param	inWhat		Kind of data error
	@param	inWhy		C string describing why the exception occurred
	@param	inWhere		Source code location where exception was thrown
	
	@note If PPx_Debug_Exceptions is false, the why and where are not
	stored.																	*/

DataError::DataError(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
	
	: Exception(inWhat, inWhy, inWhere)
{
}


// ---------------------------------------------------------------------------
//	Throw													 [static] [public]
/**
	Throws a DataError exception
	
	@param	inWhat		Kind of data error
	@param	inWhy		C string description of why the exception was thrown
	@param	inWhere		Source location where exception was throw			*/

void
DataError::Throw(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	DataError	theException(inWhat, inWhy, inWhere);
	
	Debugging::DebugException(theException);

	throw theException;
}


} // namespace PPx