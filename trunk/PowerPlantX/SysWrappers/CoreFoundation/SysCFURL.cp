// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFURL.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:31 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFURL.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Default constructor */

CFURL::CFURL()
{
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Constructs from a CFURLRef
	
	@param	inURLRef		CFURLRef to use for this object
	@param	inRetain		Whether to retain the input CFURLRef			*/

CFURL::CFURL(
	CFURLRef	inURLRef,
	bool		inRetain)
	
	: BaseT(inURLRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Constructs from a FSRef
	
	@param	inFSRef			FSRef which specifies an item in the file system
	@param	inAllocator		CF Allocator									*/

CFURL::CFURL(
	const FSRef&	inFSRef,
	CFAllocatorRef	inAllocator)
{
	CFURLRef	ref = ::CFURLCreateFromFSRef(inAllocator, &inFSRef);
	PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateFromFSRef");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Constructs from a string and base URL
	
	@param	inString		String part of URL
	@param	inBaseURL		Base URL for this URL
	@param	inAllocator		CF Allocator									*/

CFURL::CFURL(
	CFStringRef		inString,
	CFURLRef		inBaseURL,
	CFAllocatorRef	inAllocator)
{
	CFURLRef	ref = ::CFURLCreateWithString(inAllocator, inString, inBaseURL);
	PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateWithString");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Constructs from text in a buffer and a base URL
	
	@param	inBuffer		Pointer to text buffer
	@param	inBufferLength	Size of buffer
	@param	inEncoding		Encoding of characters in buffer
	@param	inBaseURL		Base URL for this URL
	@param	inAllocator		CF Allocator									*/

CFURL::CFURL(
	const void*			inBuffer,
	CFIndex				inBufferLength,
	CFStringEncoding	inEncoding,
	CFURLRef			inBaseURL,
	CFAllocatorRef		inAllocator)
{
	CFURLRef	ref = ::CFURLCreateWithBytes(inAllocator,
								static_cast<const UInt8*>(inBuffer),
								inBufferLength, inEncoding, inBaseURL);
	PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateWithBytes");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Constructs from a file system path and a base URL
	
	@param	inFilePath		File path
	@param	inIsDirectory	Whether the item is a directory
	@param	inBaseURL		Base URL for this URL
	@param	inPathStyle		OS Path Style for URL (POSIX, HFS, Windows)
	@param	inAllocator		CF Allocator									*/
	
CFURL::CFURL(
	CFStringRef		inFilePath,
	bool			inIsDirectory,
	CFURLRef		inBaseURL,
	CFURLPathStyle	inPathStyle,
	CFAllocatorRef	inAllocator)
{
	CFURLRef	ref;
	
	if (inBaseURL == nil) {
		ref = ::CFURLCreateWithFileSystemPath(inAllocator, inFilePath,
												inPathStyle, inIsDirectory);
		PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateWithFileSystemPath");
												
	} else {
		ref = ::CFURLCreateWithFileSystemPathRelativeToBase(inAllocator,
							inFilePath, inPathStyle, inIsDirectory, inBaseURL);
		PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateWithFileSystemPathRelativeToBase");
	}
		
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Constructs from a file system representation and a base URL
	
	@param	inPathBuffer	Pointer to buffer containing path
	@param	inBufferLength	Length of path buffer
	@param	inIsDirectory	Whether the item is a directory
	@param	inBaseURL		Base URL for this URL
	@param	inAllocator		CF Allocator									*/

CFURL::CFURL(
	const void*		inPathBuffer,
	CFIndex			inBufferLength,
	bool			inIsDirectory,
	CFURLRef		inBaseURL,
	CFAllocatorRef	inAllocator)
{
	CFURLRef	ref;
	
	if (inBaseURL == nil) {
		ref = ::CFURLCreateFromFileSystemRepresentation(inAllocator,
									static_cast<const UInt8*>(inPathBuffer),
									inBufferLength, inIsDirectory);
		PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateFromFileSystemRepresentation");
												
	} else {
		ref = ::CFURLCreateFromFileSystemRepresentationRelativeToBase(inAllocator,
									static_cast<const UInt8 *>(inPathBuffer),
									inBufferLength, inIsDirectory, inBaseURL);
		PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateFromFileSystemRepresentationRelativeToBase");
	}
		
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Copy constructor */

CFURL::CFURL(
	const CFURL&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	CFURL															  [public]
/**
	Assignment operator */

CFURL&
CFURL::operator = (
	const CFURL&	inSource)
{
	AssignObject(inSource);
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetAsData														  [public]
/**
	Extracts content of the URL into a CFDataRef
	
	@param	inEscapeWhitespace	Whether to escape whitespace characters
	@param	inEncoding			Encoding for characters
	@param	inAllocator			CF Allocator
	
	@return CFData object containing the content of the URL					*/

CFData
CFURL::GetAsData(
	bool				inEscapeWhitespace,
	CFStringEncoding	inEncoding,
	CFAllocatorRef		inAllocator) const
{
	return CFData( ::CFURLCreateData(inAllocator, UseRef(),
									 inEncoding, inEscapeWhitespace),
				   retain_No );
}


// ---------------------------------------------------------------------------
//	GetString														  [public]
/**
	Returns the string component of the URL
	
	@return String component of the URL											*/

CFString
CFURL::GetString() const
{
	return CFString( ::CFURLGetString(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetBaseURL														  [public]
/**
	Returns the Base URL of this URL
	
	@return	Base URL of this URL											*/

CFURL
CFURL::GetBaseURL() const
{
	return CFURL( ::CFURLGetBaseURL(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	CanBeDecomposed													  [public]
/**
	Returns whether the URL can be decomposed into separate pieces
	
	@return Whether the URL can be decomposed into separate pieces

	If it can be decomposed, you can get the scheme, net location,
	path and resource specifier as separate strings.
	
	If it cannot be deomposed, you can get the scheme and resource
	specifier, but the net location and path will be nil.					*/

bool
CFURL::CanBeDecomposed() const
{
	return ::CFURLCanBeDecomposed( UseRef() );
}


// ---------------------------------------------------------------------------
//	HasDirectoryPath												  [public]
/**
	Returns whether the URL represents a directory
	
	@return Whether the URL represents a directory							*/

bool
CFURL::HasDirectoryPath() const
{
	return ::CFURLHasDirectoryPath( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetPortNumber													  [public]
/**
	Returns URL's port number

	@return	Port number of URL

	@retval	-1		No port number specified								*/

SInt32
CFURL::GetPortNumber() const
{
	return ::CFURLGetPortNumber( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetFSRef														  [public]
/**
	Passes back the FSRef corresponding to the URL
	
	@param	outFSRef	FSRef corresponding to the URL

	@return	Whether there is a FSRef corresponding to the URL				*/

bool
CFURL::GetFSRef(
	FSRef&	outFSRef) const
{
	return ::CFURLGetFSRef( UseRef(), &outFSRef );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetScheme														  [public]
/**
	Returns scheme portion of the URL
	
	@return	Scheme portion of the URL
	
	The scheme is the transport type, such as http or ftp					*/

CFString
CFURL::GetScheme() const
{
	return CFString( ::CFURLCopyScheme(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetNetLocation													  [public]
/**
	Returns the net location protion of the URL
	
	@return Net location portion of the URL
	
	The net location contains the host name or IP address, and username
	and password															*/

CFString
CFURL::GetNetLocation() const
{
	return CFString( ::CFURLCopyNetLocation(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetPath															  [public]
/**
	Returns the path of the URL
	
	@return	Path of the URL													*/

CFString
CFURL::GetPath() const
{
	return CFString( ::CFURLCopyPath(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetStrictPath													  [public]
/**
	Returns the strict path of the URL
	
	@param	outIsAbsolute	Whether the path is a asboute
	
	@return Strict path of the URL											*/

CFString
CFURL::GetStrictPath(
	bool	outIsAbsolute) const
{
	Boolean	isAbsolute;			// Toolbox call has a Boolean* parameter.
								// Boolean and bool may have different sizes,
								//   so we use a Boolean and copy the result.
	CFStringRef	stringRef = ::CFURLCopyStrictPath( UseRef(), &isAbsolute );
	
	outIsAbsolute = isAbsolute;
	
	return CFString(stringRef, retain_No);
}


// ---------------------------------------------------------------------------
//	GetFileSystemPath												  [public]
/**
	Returns the file system path of the URL
	
	@param	inPathStyle		OS Path Style for URL (POSIX, HFS, Windows)
	
	@result	File system path of the URL										*/

CFString
CFURL::GetFileSystemPath(
	CFURLPathStyle	inPathStyle) const
{
	return CFString( ::CFURLCopyFileSystemPath(UseRef(), inPathStyle),
					 retain_No );
}


// ---------------------------------------------------------------------------
//	GetResourceSpecifier											  [public]
/**
	Returns the resource specifier of the URL
	
	@return Resource specifier of the URL									*/

CFString
CFURL::GetResourceSpecifier() const
{
	return CFString( ::CFURLCopyResourceSpecifier(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetHostName														  [public]
/**
	Returns the host name of the URL
	
	@return Host name of the URL											*/

CFString
CFURL::GetHostName() const
{
	return CFString( ::CFURLCopyHostName(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetUserName														  [public]
/**
	Returns the user name of the URL
	
	@return User name of the URL											*/

CFString
CFURL::GetUserName() const
{
	return CFString( ::CFURLCopyUserName(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetPassword														  [public]
/**
	Returns the password of the URL
	
	@return Password of the URL												*/

CFString
CFURL::GetPassword() const
{
	return CFString( ::CFURLCopyPassword(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetParameterString												  [public]
/**
	Returns the parameter string of the URL
	
	@return Parameter string of the URL										*/

CFString
CFURL::GetParameterString(
	CFStringRef	inEscapedChars) const
{
	return CFString( ::CFURLCopyParameterString(UseRef(), inEscapedChars),
					 retain_No );
}


// ---------------------------------------------------------------------------
//	GetQueryString													  [public]
/**
	Returns the query string of the URL
	
	@return Query string of the URL											*/

CFString
CFURL::GetQueryString(
	CFStringRef	inEscapedChars) const
{
	return CFString( ::CFURLCopyQueryString(UseRef(), inEscapedChars),
					 retain_No );
}


// ---------------------------------------------------------------------------
//	GetFragment														  [public]
/**
	Returns the fragment of the URL
	
	@return Fragment of the URL
	
	The fragment is the text following a # character, generally used
	to indicate locations in a single file									*/

CFString
CFURL::GetFragment(
	CFStringRef	inEscapedChars) const
{
	return CFString( ::CFURLCopyFragment(UseRef(), inEscapedChars),
					 retain_No );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetLastPathComponent											  [public]
/**
	Returns the last path component of the URL
	
	@return Last path component of the URL									*/

CFString
CFURL::GetLastPathComponent() const
{
	return CFString( ::CFURLCopyLastPathComponent(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetPathExtension												  [public]
/**
	Returns the path extension of the URL
	
	@return Path extension of the URL										*/

CFString
CFURL::GetPathExtension() const
{
	return CFString( ::CFURLCopyPathExtension(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	AppendPathComponent												  [public]
/**
	Appends a path component to the URL
	
	@param	inPathComponent		Path component to append
	@param	inIsDirectory		Whether the path compontent is a directory	*/

void
CFURL::AppendPathComponent(
	CFStringRef		inPathComponent,
	bool			inIsDirectory)
{
	CFURLRef	ref = ::CFURLCreateCopyAppendingPathComponent(
									GetAllocator(), UseRef(),
									inPathComponent, inIsDirectory);
	
	PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateCopyAppendingPathComponent");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	DeleteLastPathComponent											  [public]
/**
	Deletes the last path component from the URL							*/

void
CFURL::DeleteLastPathComponent()
{
	CFURLRef	ref = ::CFURLCreateCopyDeletingLastPathComponent(
									GetAllocator(), UseRef());
	
	PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateCopyDeletingLastPathComponent");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	AppendPathExtension												  [public]
/**
	Appends a path extension to the URL
	
	@param	inExtension		Extension to append								*/

void
CFURL::AppendPathExtension(
	CFStringRef		inExtension)
{
	CFURLRef	ref = ::CFURLCreateCopyAppendingPathExtension(
									GetAllocator(), UseRef(),
									inExtension);
	
	PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateCopyAppendingPathExtension");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	DeletePathExtension												  [public]
/**
	Deletes the path extension from the URL									*/

void
CFURL::DeletePathExtension()
{
	CFURLRef	ref = ::CFURLCreateCopyDeletingPathExtension(
									GetAllocator(), UseRef());
	
	PPx_ThrowIfCFCreateFailed_(ref, "CFURLCreateCopyDeletingPathExtension");
	
	AttachRef(ref, retain_No);
}


} // namespace PPx