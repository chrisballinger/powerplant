// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPrimaryBundle.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:27 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxPrimaryBundle.h>
#include <PPxBundleUtils.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Instance
/**
	Returns a reference to the primary bundle object
	
	@note Implements the Singleton design pattern by using a static local
	variable. The default value is the application's main bundle.			*/

CFBundle&
PrimaryBundle::Instance()
{
								// Default value is the main bundle
	static CFBundle		sPrimaryBundle(::CFBundleGetMainBundle(), retain_Yes);
	
	return sPrimaryBundle;
}


// ---------------------------------------------------------------------------
//	Set
/**
	Specify the primary bundle
	
	@param	inBundle	Bundle reference to designate as the primary bundle
	
	@note Plug-ins or other external code modules may wish to set the
	primary bundle to their bundle in order access their resources rather
	than those of the host application.										*/

void
PrimaryBundle::Set(
	CFBundleRef	inBundle)
{
	Instance().AttachRef(inBundle, retain_Yes);
}


// ---------------------------------------------------------------------------
//	GetResourceData
/**
	Returns the data from a named resource file
	
	@param	inResourceName	Name of the resource
	@param	inResourceType	Type of the resource
	@param	inSubDirName	Subdirectory of bundle in which to start search
	
	@return	CFData object containing the resource data
	
	Bundled programs typically store resources as separate files within
	subdirectories of the bundle. To get the data in a file called
	MyPicture.jpg, you would pass "MyPicture" and the resource name
	and "jpg" as the resource type. Note that the "dot" in the file
	name is not included in the name or type.
	
	You may pass nil for the subdirectory name. If so, the system
	uses its default search algorithm, which tries to get the correct
	localized version of the resource. Read the Mac OS documentation
	on bundles and url access for more information.							*/

CFData
PrimaryBundle::GetResourceData(
	CFStringRef		inResourceName,
	CFStringRef		inResourceType,
	CFStringRef		inSubDirName)
{
	return BundleUtils::GetResourceData( Instance(), inResourceName,
										 inResourceType, inSubDirName );
}


// ---------------------------------------------------------------------------
//	GetResourceProperty
/**
	Returns a CFTypeRef for a property of a resource
	
	@param	inPropertyName	Name of the property
	@param	inResourceName	Name of the resource
	@param	inResourceType	Type of the resource
	@param	inSubDirName	Subdirectory of bundle in which to start search
	
	@return	CFTypeRef for the property value
	
	See the comments for BundleUtils::GetResourceData for information
	about resource names, resource types, and subdirectories.
	
	The kind of CFTypeRef returned depends on the property. See
	<CFURLAccess.h> and Apple's Core Foundation documentation for
	information about resource properties.									*/

CFTypeRef
PrimaryBundle::GetResourceProperty(
	CFStringRef		inPropertyName,
	CFStringRef		inResourceName,
	CFStringRef		inResourceType,
	CFStringRef		inSubDirName)
{
	return BundleUtils::GetResourceProperty( Instance(), inPropertyName,
											 inResourceName, inResourceType, 
											 inSubDirName );
}


// ---------------------------------------------------------------------------
//	GetLocalizedString
/**
	Returns the localized string for a key using the default localized
	string file
	
	@param	inKey		Key for string
	
	@return Localized string for key
	
	Returns the key as the localized string if the key is not in the
	Localizable.strings file												*/

CFString
PrimaryBundle::GetLocalizedString(
	CFStringRef		inKey)
{
	return Instance().GetLocalizedString(inKey, inKey, nil);
}


// ---------------------------------------------------------------------------
//	GetLocalizedString
/**
	Returns the localized string for a key using a specified strings
	table file
	
	@param	inKey		Key for string
	@param	inTable		Name of .strings file to use as the look-up table
	
	@return Localized string for key
	
	Returns the key as the localized string if the key is not in the
	look-up table															*/
	
CFString
PrimaryBundle::GetLocalizedString(
	CFStringRef		inKey,
	CFStringRef		inTable)
{
	return Instance().GetLocalizedString(inKey, inKey, inTable);
}


// ---------------------------------------------------------------------------
//	GetLocalizedString
/**
	Returns the localized string for a key using a specified strings
	table file and default value
	
	@param	inKey			Key for string
	@param	inDefaultValue	String to return if key is not in table
	@param	inTable			Name of .strings file to use as the look-up table
	
	@return Localized string for key										*/

CFString
PrimaryBundle::GetLocalizedString(
	CFStringRef		inKey,
	CFStringRef		inDefaultValue,
	CFStringRef		inTable)
{
	return Instance().GetLocalizedString(inKey, inDefaultValue, inTable);
}


} // namespace PPx