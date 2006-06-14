// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxBundleUtils.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxBundleUtils.h>
#include <SysCFBundle.h>
#include <SysCFURL.h>

#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	GetResourceData
/**
	Returns the data from a named resource file
	
	@param	inBundle		Bundle containing resource files
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
BundleUtils::GetResourceData(
	CFBundleRef		inBundle,
	CFStringRef		inResourceName,
	CFStringRef		inResourceType,
	CFStringRef		inSubDirName)
{
	CFBundle	bundle(inBundle, retain_Yes);
	CFURL		url( bundle.GetResourceURL( inResourceName,
											inResourceType,
											inSubDirName ) );
											
		// CF routines return nil type refs to indicate failure.
		// So if url is invalid, we failed to get the URL for
		// the named resource. For all intents, this is the same
		// as a "resource file not found" Resource Manager error.
											
	if (not url.IsValid()) {
		PPx_ThrowOSErrorCode_(resFNotFound, "Resource URL not found");
	}

	OSStatus	status;
	CFDataRef	resDataRef = nil;
	::CFURLCreateDataAndPropertiesFromResource( nil, url, &resDataRef,
												nil, nil, &status );
	
		// The documentation for CFURLCreateDataAndPropertiesFromResource
		// says that the function may retrieve some data even if status
		// is an error. The caller is responsible for releasing the data ref
		// if it is non-null.
		//
		// So we put the data in a CFData, then throw an exception if we
		// failed to get all the data. This way, any partial data will be
		// released by the CFData destructor.
												
	CFData	resData(resDataRef, retain_No);
	
	PPx_ThrowIfOSError_(status, "CFURLCreateDataAndPropertiesFromResource failed");
	
	return resData;
}


// ---------------------------------------------------------------------------
//	GetResourceProperty
/**
	Returns a CFTypeRef for a property of a resource
	
	@param	inBundle		Bundle containing resource files
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
BundleUtils::GetResourceProperty(
	CFBundleRef		inBundle,
	CFStringRef		inPropertyName,
	CFStringRef		inResourceName,
	CFStringRef		inResourceType,
	CFStringRef		inSubDirName)
{
	CFBundle	bundle(inBundle, retain_Yes);
	CFURL		url( bundle.GetResourceURL( inResourceName,
											inResourceType,
											inSubDirName ) );

	OSStatus	status;
	CFTypeRef	propertyRef =
		::CFURLCreatePropertyFromResource( nil, url, inPropertyName, &status );
	
	PPx_ThrowIfOSError_(status, "CFURLCreateDataAndPropertiesFromResource failed");
	
	return propertyRef;
}


// ---------------------------------------------------------------------------
//	GetInfoDictionaryKeyString
/**
	Returns a CFString for the value of key in a bundle's info dictionary
	
	@param	inBundle	Bundle whose info dictionary to search
	@param	inKey		Dictionary key name
	
	@return	String containing the value of the key
	
	The is a wrapper for SysCFBundle's GetValueForInfoDictionaryKey function,
	which looks up a dictianary value (which is usually in the bundle's
	info.plist file). That function returns a generic CFTypeRef. This wrapper
	returns a CFString object for values that are CFStringRefs.
	
	@note Returns a CFString with a nil refernce if the key is not found.
	Throws an exception if key is found but the value is not a string.
*/

CFString
BundleUtils::GetInfoDictionaryKeyString(
	CFBundleRef		inBundle,
	CFStringRef		inKey)
{
	CFBundle	bundle(inBundle, retain_Yes);
	
	CFTypeRef	valueRef = bundle.GetValueForInfoDictionaryKey(inKey);
	
	if (valueRef != nil) {			// Verify that value is indeed a string
		if (::CFGetTypeID(valueRef) != ::CFStringGetTypeID()) {
			PPx_Throw_(RuntimeError, err_DynamicCast,
							"Info dictionary key value is not a string");
		}
	}
	
	return CFString(CFStringRef(valueRef));
}


} // namespace PPx