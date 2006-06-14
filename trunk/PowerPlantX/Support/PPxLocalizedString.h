// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxLocalizedString.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxLocalizedString.h
	@brief		A string whose localized value is looked up from a table
*/

#ifndef H_PPxLocalizedString
#define H_PPxLocalizedString
#pragma once

#include <PPxPrefix.h>

#include <SysCFString.h>

namespace PPx {

// ===========================================================================
//	LocalizedString
/**
	A string whose localized value is looked up from a table */

class	LocalizedString {
public:
						LocalizedString(
								CFStringRef		inKeyString,
								CFStringRef		inTableName = nil);
							
	CFString			GetKeyString() const;
	
	CFString			GetTableName() const;
	
	CFString			GetLocalizedString() const;

private:
	CFString			mKeyString;
	CFString			mTableName;
	mutable CFString	mLocalizedString;
};


// ---------------------------------------------------------------------------
//	GetKeyString
/**
	Returns the key string for the localized string
	
	@return Key string for the localized string								*/

inline CFString
LocalizedString::GetKeyString() const
{
	return mKeyString;
}


// ---------------------------------------------------------------------------
//	GetTableName
/**
	Returns the file name of the lookup table for the localized string
	
	@return File name of the lookup table for the localized string
	
	The name does not include the .strings extension. A nil table name
	means that the default Localizable.strings file is used.				*/

inline CFString
LocalizedString::GetTableName() const
{
	return mTableName;
}


} // namespace PPx

#endif	// H_PPxLocalizedString
