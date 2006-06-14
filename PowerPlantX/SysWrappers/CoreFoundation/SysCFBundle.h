// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFBundle.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:28 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFBundle.h
	@brief		Wrapper class for Core Foundation Bundle
*/

#ifndef H_SysCFBundle
#define H_SysCFBundle
#pragma once

#include <SysCFObject.h>
#include <SysCFArray.h>
#include <SysCFDictionary.h>
#include <SysCFString.h>
#include <SysCFURL.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFBundle
/**
	Wrapper class for Core Foundation Bundle */

class	CFBundle : public CFObject<CFBundleRef> {
public:
									// Constructors
						CFBundle();
						
						CFBundle(
								CFBundleRef	inBundleRef,
								bool		inRetain);
						
	explicit			CFBundle( CFStringRef inBundleID );
						
	explicit			CFBundle(
								CFURLRef		inBundleURL,
								CFAllocatorRef	inAllocator = nil);
								
						CFBundle( const CFBundle& inOriginal );
						
	CFBundle&			operator = ( const CFBundle& inSource );
	
									// Accessors
	CFURL				GetBundleURL() const;
	
	CFTypeRef			GetValueForInfoDictionaryKey( CFStringRef inKey ) const;
	
	CFDictionary<CFStringRef, CFStringRef>
						GetGlobalInfoDictionary() const;
	
	CFDictionary<CFStringRef, CFStringRef>
						GetLocalInfoDictionary() const;
	
	void				GetPackageInfo(
								UInt32&		outPackageType,
								UInt32&		outPackageCreator) const;
								
	CFString			GetIdentifier() const;
	
	UInt32				GetVersionNumber() const;
	
	CFString			GetDevelopmentRegion() const;
	
	CFURL				GetSupportFilesDirectoryURL() const;
	
	CFURL				GetResourcesDirectoryURL() const;
	
	CFURL				GetPrivateFrameworksURL() const;
	
	CFURL				GetSharedFrameworksURL() const;
	
	CFURL				GetSharedSupportURL() const;
	
	CFURL				GetBuiltInPlugInsURL() const;
	
									// Resource Handling
	CFURL				GetResourceURL(
								CFStringRef	inResourceName,
								CFStringRef	inResourceType = nil,
								CFStringRef	inSubDirName = nil) const;
	
	CFArray<CFURLRef>	GetResourceURLsOfType(
								CFStringRef	inResourceType,
								CFStringRef	inSubDirName = nil) const;
								
	CFString			GetLocalizedString(
								CFStringRef	inKey,
								CFStringRef	inDefaultValue,
								CFStringRef	inTableName) const;
								
									// Localization Resource Handling
	CFArray<CFStringRef>
						GetBundleLocalizations() const;
	
	CFArray<CFStringRef>
						GetPreferredLocalizations() const;
	
									// Resource Manager functions
	SInt16				OpenResourceMap() const;
	
	void				CloseResourceMap( SInt16 inRefNum ) const;
								
private:
	typedef CFObject<CFBundleRef>	BaseT;
};

} // namespace PPx

#endif // H_SysCFBundle
