// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxBundleUtils.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxBundleUtils.h
	@brief		Utility functions for working with Bundles
*/

#ifndef H_PPxBundleUtils
#define H_PPxBundleUtils
#pragma once

#include <PPxPrefix.h>
#include <SysCFData.h>
#include <SysCFString.h>

#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	BundleUtils
/**
	Utility functions for working with Bundles */

namespace BundleUtils {

	CFData		GetResourceData(
						CFBundleRef		inBundle,
						CFStringRef		inResourceName,
						CFStringRef		inResourceType,
						CFStringRef		inSubDirName = nil);
						
	CFTypeRef	GetResourceProperty(
						CFBundleRef		inBundle,
						CFStringRef		inPropertyName,
						CFStringRef		inResourceName,
						CFStringRef		inResourceType,
						CFStringRef		inSubDirName = nil);
						
	CFString	GetInfoDictionaryKeyString(
						CFBundleRef		inBundle,
						CFStringRef		inKey);
}


} // namespace PPx

#endif	// H_PPxBundleUtils
