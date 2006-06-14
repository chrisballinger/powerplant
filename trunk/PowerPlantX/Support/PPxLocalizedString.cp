// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxLocalizedString.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxLocalizedString.h>
#include <PPxPrimaryBundle.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	LocalizedString
/**
	Constructor
	
	@param	inKeyString			Key string for the localized string
	@param	inTableName			File name of lookup table
	
	The table name should not inlude the .strings extension. Pass nil
	to use the default Localizable.strings table.							*/

LocalizedString::LocalizedString(
	CFStringRef		inKeyString,
	CFStringRef		inTableName)
	
	: mKeyString(inKeyString),
	  mTableName(inTableName)
{
}


// ---------------------------------------------------------------------------
//	GetLocalizedString
/*
	Returns the localized version of the string
	
	@return	Localized version of the string									*/

CFString
LocalizedString::GetLocalizedString() const
{
	if (not mLocalizedString.IsValid()) {
	
		mLocalizedString = PrimaryBundle::GetLocalizedString(
									mKeyString, mTableName.GetRefValue());
	}
	
	return mLocalizedString;
}


} // namespace PPx