// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxExceptions.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:11 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxExceptions.h
	@brief		Exception classes
*/

#ifndef H_PPxExceptions
#define H_PPxExceptions
#pragma once

#include <PPxDebugging.h>

namespace PPx {

typedef		FourCharCode	ExceptionIDT;	/**< The "what" of an exception */

// ---------------------------------------------------------------------------
//	Exception IDs

const ExceptionIDT		err_MacOS				= 'oser';
const ExceptionIDT		err_Logic				= 'logc';
const ExceptionIDT		err_Runtime				= 'runt';
const ExceptionIDT		err_DataFormat			= 'data';

const ExceptionIDT		err_CFCreate			= 'cfcr';
const ExceptionIDT		err_CFStringConvert		= 'cfsc';
const ExceptionIDT		err_CFNilRef			= 'cfnr';
const ExceptionIDT		err_XMLFormat			= 'xmlf';
const ExceptionIDT		err_DynamicCast			= 'cast';
const ExceptionIDT		err_BadParam			= 'parm';
const ExceptionIDT		err_MissingData			= 'misd';


// ===========================================================================
//	Exception
/**
	Base class for PowerPlant X exceptions. Exception objects store an
	identifier indicating the kind of error, and, when the
	PPx_Debug_Exceptions preprocessor flag is set, an explanation string and
	source code location of the throw.
	
	The constructors and destructor are protected to enforce that Exception
	is a base class and never directly instantiated.						*/

class	Exception {
public:
	ExceptionIDT		What() const;
	
	virtual StringPtr	Why( Str255 outWhy ) const;
						
	const SourceLocation&
						Where() const;

protected:
						Exception(
								ExceptionIDT			inWnat,
								const char*				inWhy,
								const SourceLocation&	inWhere);
	
	virtual				~Exception();

private:
	ExceptionIDT		mExceptionID;	/**< The kind of error */
						
#if PPx_Debug_Exceptions			// Store "why" and "where" when debugging
	
	Str255				mWhyString;		/**< Description of the cause of the
												exception */
	
	SourceLocation		mLocation;		/**< Where in the source code the
												throw occurred */
	
#endif // PPx_Debug_Exceptions
};


// ===========================================================================
//	OSError
/**
	Exception class for a Mac OS error code. An OSError is thrown when a Mac
	Toolbox routine returns an error code.									*/

class	OSError : public Exception {
public:
						OSError(
								OSStatus				inOSError,
								const char*				inWhy,
								const SourceLocation&	inWhere);
	
	virtual StringPtr	Why( Str255 outWhy ) const;
								
	OSStatus			GetOSErrorCode() const;
	
	static void			Throw(	OSStatus				inOSError,
								const char*				inWhy,
								const SourceLocation&	inWhere);
								
										/** Custom throw function signature */
	typedef void (*ThrowFunc)(OSStatus, const char*, const SourceLocation&);
	
	static void			SetThrowFunc( ThrowFunc inThrowFunc );
	
private:
	OSStatus			mErrorCode;		/**< Mac error code */
	
	static ThrowFunc	sThrowFunc;		/**< Function to call before throwing
												an OSError */
};


// ===========================================================================
//	OSErrorCode
/**
	Template exception class for a specific Mac OS Error code. The template
	parameter is the actual error code integer value. This creates a
	unique type which you can use for catching exceptions.					*/

template <OSStatus status>
class	OSErrorCode : public OSError {
public:
						OSErrorCode(
								const char*				inWhy,
								const SourceLocation&	inWhere);
								
	static void			Throw(	const char*				inWhy,
								const SourceLocation&	inWhere);
};


// ---------------------------------------------------------------------------
//	OSErrorCode														  [public]
/**
	Constuctor
	
	@param	inWhy		A string describing the cause of the error
	@param	inWhere		Location within the source of the exception			*/

template <OSStatus status>
OSErrorCode<status>::OSErrorCode(
	const char*				inWhy,
	const SourceLocation&	inWhere)
	
	: OSError(status, inWhy, inWhere)
{
}


// ---------------------------------------------------------------------------
//	Throw													 [static] [public]
/**
	Throws an OSErrorCode exception
	
	@param	inWhy		A string describing the cause of the error
	@param	inWhere		Location within the source of the caller			*/

template <OSStatus status>
void
OSErrorCode<status>::Throw(
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	OSErrorCode	theException(inWhy, inWhere);
	
	Debugging::DebugException(theException);

	throw theException;
}

			
// ===========================================================================
//	LogicError
/**
	Exception class for a programming error. Logic errors are usually due to
	bugs in code, such as invalid function parameters or violations of assert
	conditions. Such errors should be found during testing.					*/

class	LogicError : public Exception {
public:
						LogicError(
								ExceptionIDT			inWhat,
								const char*				inWhy,
								const SourceLocation&	inWhere);

	static void			Throw(	ExceptionIDT			inWhat,
								const char*				inWhy,
								const SourceLocation&	inWhere);
};
			
			
// ===========================================================================
//	RuntimeError
/**
	Exception class for a runtime failure. Runtime errors are due to
	conditions external to the program, such as running out of memory or some
	other system resource.													*/

class	RuntimeError : public Exception {
public:
						RuntimeError(
								ExceptionIDT			inWhat,
								const char*				inWhy,
								const SourceLocation&	inWhere);
								
	static void			Throw(	ExceptionIDT			inWhat,
								const char*				inWhy,
								const SourceLocation&	inWhere);
};
			
			
// ===========================================================================
//	DataError
/**
	Exception class for bad input data. PPx programs are data-driven, using
	information stored in text files and Mac OS resources to describe user
	interface elements and program behavior. A DataError indicates a problem
	in interpreting that information. Such errors should be found during
	testing.																*/

class	DataError : public Exception {
public:
						DataError(
								ExceptionIDT			inWhat,
								const char*				inWhy,
								const SourceLocation&	inWhere);
								
	static void			Throw(	ExceptionIDT			inWhat,
								const char*				inWhy,
								const SourceLocation&	inWhere);
};


// ===========================================================================
//	Helper functions for throwing exceptions

// ---------------------------------------------------------------------------
//	ThrowException< TException >
/**
	Template function for throwing PPx Exception objects
	
	@param	inWhat		Exception ID
	@param	inWhy		A string describing the cause of the exception
	@param	inWhere		Location in the source code of the exception		*/

template <class TException>
inline void
ThrowException(
	ExceptionIDT			inWhat,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	TException::Throw(inWhat, inWhy, inWhere);
}


// ---------------------------------------------------------------------------
//	ThrowOSError
/**
	Throws a PPx::OSError exception
	
	@param	inErrorCode		A Mac OS error code
	@param	inWhy			A string describing the cause of the exception
	@param	inWhere			Location in the source code of the exception	*/

inline void
ThrowOSError(
	OSStatus				inErrorCode,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	OSError::Throw(inErrorCode, inWhy, inWhere);
}


// ---------------------------------------------------------------------------
//	ThrowOSErrorCode< OSStatus >
/**
	Template function which throws a PPx::OSErrorCode<> exception.
	Template parameter is a literal error code value.
	
	@param	inWhy		A string describing the cause of the exception
	@param	inWhere		Location in the source code of the exception		*/

template <OSStatus status>
inline void
ThrowOSErrorCode(
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	OSErrorCode<status>::Throw(inWhy, inWhere);
}


// ---------------------------------------------------------------------------
//	ThrowIfOSError
/**
	Throws a PPx::OSError exception if the error code is not noErr
	
	@param	inErrorCode		A Mac OS error code
	@param	inWhy			A string describing the cause of the exception
	@param	inWhere			Location in the source code of the exception	*/

inline void
ThrowIfOSError(
	OSStatus				inErrorCode,
	const char*				inWhy,
	const SourceLocation&	inWhere)
{
	if (inErrorCode != noErr) {
		ThrowOSError(inErrorCode, inWhy, inWhere);
	}
}


// ===========================================================================
//	Macros for throwing exceptions

// ---------------------------------------------------------------------------
//	PPx_Throw_( ExceptionClass, inWhat, inWhy )
/**
	Throws an exception
	
	@param	ExceptionClass	Name of the exception class
	@param	inWhat			Exception ID
	@param	inWhy			A string describing the cause of the exception	*/

#define	PPx_Throw_(ExceptionClass, inWhat, inWhy)					\
			PPx::ThrowException< ExceptionClass >(inWhat, inWhy, PPx_ExceptLoc_Here)
			
			
// ---------------------------------------------------------------------------
//	PPx_ThrowIf_( test, ExceptionClass, inWhat, inWhy )
/**
	Throws an exception if a boolean test condition is true
	
	@param	test			C++ code of boolean test condition
	@param	ExceptionClass	Name of the exception class
	@param	inWhat			Exception ID
	@param	inWhy			A string describing the cause of the exception	*/			
			
#define	PPx_ThrowIf_(test, ExceptionClass, inWhat, inWhy)			\
			if (test)												\
				PPx_Throw_(ExceptionClass, inWhat, inWhy)
				

// ---------------------------------------------------------------------------
//	PPx_ThrowIfNil_( inValue, ExceptionClass, inWhat, inWhy )
/**
	Throws an exception if a value is nil
	
	@param	inValue			A pointer type value
	@param	ExceptionClass	Name of the exception class
	@param	inWhat			Exception ID
	@param	inWhy			A string describing the cause of the exception	*/			

#define	PPx_ThrowIfNil_(inValue, ExceptionClass, inWhat, inWhy)		\
			if ((inValue) == nil)									\
				PPx_Throw_(ExceptionClass, inWhat, inWhy)
				

// ---------------------------------------------------------------------------
//	PPx_ThrowIfOSError_( inErrorCode, inWhy )
/**
	Throws a PPx::OSError exception if the error code is not noErr
	
	@param	inErrorCode		A Mac OS error code
	@param	inWhy			A string describing the cause of the exception	*/			

#define PPx_ThrowIfOSError_(inErrorCode, inWhy)						\
			PPx::ThrowIfOSError(inErrorCode, inWhy, PPx_ExceptLoc_Here)
				

// ---------------------------------------------------------------------------
//	PPx_ThrowOSError_( inErrorCode, inWhy )
/**
	Throws a PPx::OSError exception with the specified error code
	
	@param	inErrorCode		A Mac OS error code
	@param	inWhy			A string describing the cause of the exception	*/			

#define PPx_ThrowOSError_(inErrorCode, inWhy)						\
			PPx::ThrowOSError(inErrorCode, inWhy, PPx_ExceptLoc_Here)
				

// ---------------------------------------------------------------------------
//	PPx_ThrowOSErrorCode_( inErrorCode, inWhy )
/**
	Throws a PPx::OSErrorCode exception
	
	@param	inErrorCode		A literal Mac OS error code. Must be a constant
								value and not a variable.
	@param	inWhy			A string describing the cause of the exception	*/			

#define PPx_ThrowOSErrorCode_(inErrorCode, inWhy)					\
			PPx::ThrowOSErrorCode< inErrorCode >(inWhy, PPx_ExceptLoc_Here)


// ===========================================================================
//	Macros for verifying parameters

#if PPx_Verify_Parameters

// ---------------------------------------------------------------------------
//	PPx_BadParamIf_( test )
/**
	Throws a err_BadParam exception if a test condition is true */

#define	PPx_BadParamIf_(test)										\
			if (test)												\
				PPx::LogicError::Throw( PPx::err_BadParam,			\
										"Bad parameter: " #test,	\
										PPx_ExceptLoc_Here )
								
#else

	#define	PPx_BadParamIf_(test)
	
#endif // PPx_Verify_Parameters


// ---------------------------------------------------------------------------
//	PPx_BadParamIfNil_( nil )
/**
	Throws a err_BadParam exception if the parameter is nil */

#define PPx_BadParamIfNil_(inPtr)	PPx_BadParamIf_(inPtr == nil)


} // namespace PPx

#endif	// H_PPxExceptions
