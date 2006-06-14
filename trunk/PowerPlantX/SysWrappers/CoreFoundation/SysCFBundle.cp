// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFBundle.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:28 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFBundle.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFBundle														  [public]
/**
	Default constructor	*/

CFBundle::CFBundle()
{
}


// ---------------------------------------------------------------------------
//	CFBundle														  [public]
/**
	Constructs from a CFBundleRef
	
	@param	inBundleRef		CFBundleRef to use
	@param	inRetain		Whether to retain the CFBundleRef				*/

CFBundle::CFBundle(
	CFBundleRef	inBundleRef,
	bool		inRetain)
	
	: BaseT(inBundleRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFBundle														  [public]
/**
	Constructs from a Bundle identifier
	
	@param	inBundleID		Bundle identifier								*/

CFBundle::CFBundle(
	CFStringRef	inBundleID)
{
	CFBundleRef	ref = ::CFBundleGetBundleWithIdentifier(inBundleID);
	PPx_ThrowIfCFCreateFailed_(ref, "CFBundleGetBundleWithIdentifier");
		
	AttachRef(ref, retain_Yes);
}


// ---------------------------------------------------------------------------
//	CFBundle														  [public]
/**
	Constructs from a URL
	
	@param	inBundleURL		URL for a Bundle
	@param	inAllocator		CF Allocator									*/

CFBundle::CFBundle(
	CFURLRef		inBundleURL,
	CFAllocatorRef	inAllocator)
{
	CFBundleRef	ref = ::CFBundleCreate(inAllocator, inBundleURL);
	PPx_ThrowIfCFCreateFailed_(ref, "CFBundleCreate");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFBundle														  [public]
/**
	Copy constructor */

CFBundle::CFBundle(
	const CFBundle&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */

CFBundle&
CFBundle::operator = (
	const CFBundle&	inSource)
{
	AssignObject(inSource);
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetBundleURL													  [public]
/**
	Returns a CFURL object for the Bundle's location
	
	@return CFURL object for the Bundle's location							*/

CFURL
CFBundle::GetBundleURL() const
{
	return CFURL( ::CFBundleCopyBundleURL(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetValueForInfoDictionaryKey									  [public]
/**
	Returns a CFTypeRef for a value in the Bundle's information dictionary
	
	@param	inKey	Key for the value
	
	@return	CTTypeRef for the value with the specified key					*/

CFTypeRef
CFBundle::GetValueForInfoDictionaryKey(
	CFStringRef	inKey) const
{
	return ::CFBundleGetValueForInfoDictionaryKey( UseRef(), inKey );
}


// ---------------------------------------------------------------------------
//	GetGlobalInfoDictionary											  [public]
/**
	Returns a CFDictionary containing the Bundle's global information
	dictionary
	
	@return CFDictionary containing the Bundle's global information
	dictionary																*/

CFDictionary<CFStringRef, CFStringRef>
CFBundle::GetGlobalInfoDictionary() const
{
	return CFDictionary<CFStringRef, CFStringRef>
				( ::CFBundleGetInfoDictionary(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetLocalInfoDictionary											  [public]
/**
	Returns a CFDictionary object containing the Bundle's local information
	dictionary
	
	@return CFDictionary object containing the Bundle's local information
	dictionary																*/

CFDictionary<CFStringRef, CFStringRef>
CFBundle::GetLocalInfoDictionary() const
{
	return CFDictionary<CFStringRef, CFStringRef>
				( ::CFBundleGetLocalInfoDictionary(UseRef()),  retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetPackageInfo													  [public]
/**
	Passes back the type and creator codes for the Bundle's package
	
	@param	outPackageType		Type code of the Bundle's package
	@param	outPackageCreator	Creator code of the Bundle's package		*/

void
CFBundle::GetPackageInfo(
	UInt32&		outPackageType,
	UInt32&		outPackageCreator) const
{
	::CFBundleGetPackageInfo(UseRef(), &outPackageType,  &outPackageCreator);
}


// ---------------------------------------------------------------------------
//	GetIdentifier													  [public]
/**
	Returns a CFString containing the Bundle's identifier
	
	@return CFString containing the Bundle's identifier						*/

CFString
CFBundle::GetIdentifier() const
{
	return CFString( ::CFBundleGetIdentifier(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetVersionNumber												  [public]
/**
	Returns the version number of the Bundle
	
	@return Version number of the Bundle									*/

UInt32
CFBundle::GetVersionNumber() const
{
	return ::CFBundleGetVersionNumber( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetDevelopmentRegion											  [public]
/**
	Returns a CFString containing the Bundle's development region
	
	@return CFString containing the Bundle's development region				*/

CFString
CFBundle::GetDevelopmentRegion() const
{
	return CFString( ::CFBundleGetDevelopmentRegion(UseRef()), retain_Yes );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetSupportFilesDirectoryURL									 	 [public]
/**
	Returns CRURL for the location of the Bundle's support files directory
	
	@return CFURL for the location of the Bundle's support files directory	*/

CFURL
CFBundle::GetSupportFilesDirectoryURL() const
{
	return CFURL( ::CFBundleCopySupportFilesDirectoryURL(UseRef()),
				  retain_No );
}


// ---------------------------------------------------------------------------
//	GetResourcesDirectoryURL										  [public]
/**
	Returns CFURL for the location of the Bundle's resources directory
	
	@return CFURL for the location of the Bundle's resources directory		*/

CFURL
CFBundle::GetResourcesDirectoryURL() const
{
	return CFURL( ::CFBundleCopyResourcesDirectoryURL(UseRef()),
				  retain_No );
}


// ---------------------------------------------------------------------------
//	GetPrivateFrameworksURL											  [public]
/**
	Returns CFURL for the location of the Bundle's private frameworks
	
	@return CFURL for the location of the Bundle's private frameworks		*/

CFURL
CFBundle::GetPrivateFrameworksURL() const
{
	return CFURL( ::CFBundleCopyPrivateFrameworksURL(UseRef()),
				  retain_No );
}


// ---------------------------------------------------------------------------
//	GetSharedFrameworksURL											  [public]
/**
	Returns CFURL for the location of the Bundle's shared frameworks
	
	@return CFURL for the location of the Bundle's shared frameworks		*/

CFURL
CFBundle::GetSharedFrameworksURL() const
{
	return CFURL( ::CFBundleCopySharedFrameworksURL(UseRef()),
				  retain_No );
}


// ---------------------------------------------------------------------------
//	GetSharedSupportURL											  [public]
/**
	Returns CFURL for the location of the Bundle's shared support files
	directory
	
	@return CFURL for the location of the Bundle's shared support files
	directory																*/

CFURL
CFBundle::GetSharedSupportURL() const
{
	return CFURL( ::CFBundleCopySharedSupportURL(UseRef()),
				  retain_No );
}


// ---------------------------------------------------------------------------
//	GetBuiltInPlugInsURL											  [public]
/**
	Returns CFURL for the location of the Bundle's built-in plug-ins
	
	@return CFURL for the location of the Bundle's built-in plug-ins		*/

CFURL
CFBundle::GetBuiltInPlugInsURL() const
{
	return CFURL( ::CFBundleCopyBuiltInPlugInsURL(UseRef()),
				  retain_No );
}


// ---------------------------------------------------------------------------
//	GetResourceURL													  [public]
/**
	Returns CFURL for the location of a resource file within the Bundle
	
	@param	inResourceName	Name of resource file
	@param	inResourceType	Type of resource file
	@param	inSubDirName	Subdirectory which to search. Pass nil to use
								the standard search locations
	
	@return CFURL for the location of the resource file						*/

CFURL
CFBundle::GetResourceURL(
	CFStringRef	inResourceName,
	CFStringRef	inResourceType,
	CFStringRef	inSubDirName) const
{
	return CFURL( ::CFBundleCopyResourceURL(UseRef(), inResourceName,
										inResourceType, inSubDirName),
				  retain_No );
}


// ---------------------------------------------------------------------------
//	GetResourceURLsOfType											  [public]
/**
	Returns a CFArray of CFURLs for the locations of all resources of
	a specified type
	
	@param	inResourceType	Type of resource file
	@param	inSubDirName	Subdirectory which to search. Pass nil to use
								the standard search locations

	@return CFArray of CFURLRefs for the locations of all resources of
	a specified type														*/

CFArray<CFURLRef>
CFBundle::GetResourceURLsOfType(
	CFStringRef	inResourceType,
	CFStringRef	inSubDirName) const
{
	return CFArray<CFURLRef>( ::CFBundleCopyResourceURLsOfType(
									UseRef(), inResourceType, inSubDirName),
							  retain_No );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetLocalizedString												  [public]
/**
	Returns localized string by performing a table look-up. The table
	is a .strings file which contains (key, value) pairs of strings.
	If the key string is in the table, function returns the corresponding
	value string. If the key is not in the table, function returns the
	default value string.
	
	@param	inKey			Key string for performing table look-up
	@param	inDefaultValue	String returned if key is not found
	@param	inTableName		Name of .strings file containing look-up table.
							If nil, uses the default Localizable.strings file.
	
	@return	CFString with localized text
*/

CFString
CFBundle::GetLocalizedString(
	CFStringRef	inKey,
	CFStringRef	inDefaultValue,
	CFStringRef	inTableName) const
{
	return CFString( ::CFBundleCopyLocalizedString(
							UseRef(), inKey, inDefaultValue, inTableName ),
					 retain_No );
}


// ---------------------------------------------------------------------------
//	GetBundleLocalizations											  [public]
/**
	Returns a CFArray of CFStringRefs of all the Bundle's localizations
	
	@return CFArray of CFStringRefs of all the Bundle's localizations		*/

CFArray<CFStringRef>
CFBundle::GetBundleLocalizations() const
{
	return CFArray<CFStringRef>
				( ::CFBundleCopyBundleLocalizations(UseRef()), retain_No );
}


// ---------------------------------------------------------------------------
//	GetPreferredLocalizations										  [public]
/**
	Returns a CFArray of CFStringRefs of all the Bundle's preferred
	localizations
	
	@return CFArray of CFStringRefs of all the Bundle's preferred
	localizations															*/

CFArray<CFStringRef>
CFBundle::GetPreferredLocalizations() const
{
	CFArrayRef	preferredLocs = nil;

	CFArray<CFStringRef>	allLocs(GetBundleLocalizations());

	if (allLocs.IsValid()) {
		preferredLocs = ::CFBundleCopyPreferredLocalizationsFromArray(allLocs);
	}
	
	return CFArray<CFStringRef>(preferredLocs, retain_No );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	OpenResourceMap													  [public]
/**
	Opens the Bundle's resource map and returns its reference number
	
	@return	Reference number for Bundle's open resource map					*/

SInt16
CFBundle::OpenResourceMap() const
{
	return ::CFBundleOpenBundleResourceMap( UseRef() );
}


// ---------------------------------------------------------------------------
//	CloseResourceMap												  [public]
/**
	Closes Bundle's resource map
	
	@param	inRefNum	Reference number of Bundle's resource map, previously
						obtained from OpenResourceMap()						*/

void
CFBundle::CloseResourceMap(
	SInt16	inRefNum) const
{
	::CFBundleCloseBundleResourceMap( UseRef(), inRefNum );
}


} // namespace PPx