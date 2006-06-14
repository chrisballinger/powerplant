// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFUtils.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:31 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFUtils.h
	@brief		Utility functions for working with CoreFoundation
*/

#ifndef H_SysCFUtils
#define H_SysCFUtils
#pragma once

#include <PPxPrefix.h>

#include <Carbon/Carbon.h>
#include <sstream>

// ===========================================================================
//	Global comparison operators for CFRange

// ---------------------------------------------------------------------------
/**	Equality operator for CFRange

	@param	inLeft		Left side of == operator
	@param	inRight		Right side of == operator
	
	@return	Whether the two CFRanges are equal								*/

inline bool
operator == (
	const CFRange&	inLeft,
	const CFRange&	inRight)
{
	return ((inLeft.location == inRight.location) and
			(inLeft.length == inRight.length));
}


// ---------------------------------------------------------------------------
/**	Inequality operator for CFRange

	@param	inLeft		Left side of != operator
	@param	inRight		Right side of != operator
	
	@return	Whether the two CFRanges are not equal							*/

inline bool
operator != (
	const CFRange&	inLeft,
	const CFRange&	inRight)
{
	return ((inLeft.location != inRight.location) or
			(inLeft.length != inRight.length));
}



namespace PPx {

class CFString;

// ===========================================================================
/**	Utility functions for working with Core Foundation */

namespace CFUtils {

	CFStringEncoding	GetEncodingFromScriptCode(
								ScriptCode			inScript,
								LangCode			inLanguage = kTextLanguageDontCare,
								RegionCode			inRegion = kTextRegionDontCare,
								ConstStr255Param	inFontName = nil);

	CFString			GetIndString(
								ResIDT				inSTRxID,
								SInt16				inIndex,
								CFStringEncoding	inEncoding = encoding_System,
								CFAllocatorRef		inAllocator = nil);

	bool				MakeValidRange(
								CFIndex		inMaxLength,
								CFRange&	ioRange);

	void				VerifyRange(
								CFIndex		inMaxLength,
								CFRange&	ioRange);

	bool				MakeValidIndex(
								CFIndex		inCount,
								CFIndex&	ioIndex);

	void				VerifyIndex(
								CFIndex		inCount,
								CFIndex&	ioIndex);
								
	bool				MakeInsertIndex(
								CFIndex		inCount,
								CFIndex&	ioIndex);				
								
	void				VerifyInsertIndex(
								CFIndex		inCount,
								CFIndex&	ioIndex);				

} // namespace CFUtils


} // namespace PPx


#endif // H_SysCFUtils
