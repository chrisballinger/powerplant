// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPrimaryBundle.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:27 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPrimaryBundle.h
	@brief		Utility functions for working with the primary bundle
				for a program
*/

#ifndef H_PPxPrimaryBundle
#define H_PPxPrimaryBundle
#pragma once

#include <PPxPrefix.h>

#include <SysCFBundle.h>
#include <SysCFString.h>

namespace PPx {

// ===========================================================================
//	PrimaryBundle
/**
	Utility functions for working with the primary bundle for a program */

namespace PrimaryBundle {
	
	CFBundle&	Instance();

	void		Set( CFBundleRef inBundle );
	
	CFData		GetResourceData(
						CFStringRef		inResourceName,
						CFStringRef		inResourceType,
						CFStringRef		inSubDirName = nil);
						
	CFTypeRef	GetResourceProperty(
						CFStringRef		inPropertyName,
						CFStringRef		inResourceName,
						CFStringRef		inResourceType,
						CFStringRef		inSubDirName = nil);
						
	CFString	GetLocalizedString( CFStringRef inKey );
	
	CFString	GetLocalizedString(
						CFStringRef		inKey,
						CFStringRef		inTable);
	
	CFString	GetLocalizedString(
						CFStringRef		inKey,
						CFStringRef		inDefaultValue,
						CFStringRef		inTable);
}


} // namespace PPx

#endif	// H_PPxPrimaryBundle
