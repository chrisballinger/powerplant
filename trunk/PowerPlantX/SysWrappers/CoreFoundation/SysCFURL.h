// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFURL.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:31 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFURL.h
	@brief		Wrapper class for Core Foundation URL
*/

#ifndef H_SysCFURL
#define H_SysCFURL
#pragma once

#include <SysCFObject.h>
#include <SysCFData.h>
#include <SysCFString.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFURL
/**
	Wrapper class for Core Foundation URL */

class	CFURL : public CFObject<CFURLRef> {
public:
									// Constructors
						CFURL();
						
						CFURL(	CFURLRef	inURLRef,
								bool		inRetain);
						
	explicit			CFURL(	const FSRef&	inFSRef,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFURL(	CFStringRef		inString,
								CFURLRef		inBaseURL = nil,
								CFAllocatorRef	inAllocator = nil);
								
						CFURL(	const void*			inBuffer,
								CFIndex				inBufferLength,
								CFStringEncoding	inEncoding = encoding_System,
								CFURLRef			inBaseURL = nil,
								CFAllocatorRef		inAllocator = nil);
								
						CFURL(	CFStringRef		inFilePath,
								bool			inIsDirectory,
								CFURLRef		inBaseURL = nil,
								CFURLPathStyle	inPathStyle = kCFURLPOSIXPathStyle,
								CFAllocatorRef	inAllocator = nil);
								
						CFURL(	const void*		inPathBuffer,
								CFIndex			inBufferLength,
								bool			inIsDirectory,
								CFURLRef		inBaseURL = nil,
								CFAllocatorRef	inAllocator = nil);
								
						CFURL( const CFURL& inOriginal );
						
	CFURL&				operator = ( const CFURL& inSource );
	
									// Accessors
	CFData				GetAsData(
								bool				inEscapeWhitespace,
								CFStringEncoding	inEncoding = encoding_System,
								CFAllocatorRef		inAllocator = nil) const;
								
	CFString			GetString() const;

	CFURL				GetBaseURL() const;
	
	bool				CanBeDecomposed() const;
	
	bool				HasDirectoryPath() const;
	
	SInt32				GetPortNumber() const;
	
	bool				GetFSRef( FSRef &outFSRef ) const;
	
									// Getting copies of string data
	CFString			GetScheme() const;
								
	CFString			GetNetLocation() const;
	
	CFString			GetPath() const;
	
	CFString			GetStrictPath( bool inIsAbsolute ) const;
	
	CFString			GetFileSystemPath( CFURLPathStyle inPathStyle
												= kCFURLPOSIXPathStyle ) const;
	
	CFString			GetResourceSpecifier() const;
	
	CFString			GetHostName() const;
	
	CFString			GetUserName() const;
	
	CFString			GetPassword() const;
	
	CFString			GetParameterString( CFStringRef inEscapedChars = nil ) const;
	
	CFString			GetQueryString( CFStringRef inEscapedChars = nil ) const;
	
	CFString			GetFragment( CFStringRef inEscapedChars = nil ) const;
	
	CFString			GetLastPathComponent() const;
	
	CFString			GetPathExtension() const;
	
									// Changing path component and extension
	void				AppendPathComponent(
								CFStringRef		inPathComponent,
								bool			inIsDirectory);
								
	void				DeleteLastPathComponent();
	
	void				AppendPathExtension( CFStringRef inExtension );
	
	void				DeletePathExtension();

private:
	typedef CFObject<CFURLRef>	BaseT;
};

} // namespace PPx

#endif // H_SysCFURL
