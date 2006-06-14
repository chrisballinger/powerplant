// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFSUtils.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:19 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxFSUtils.h
	@brief		Utility functions for working with files and folders
*/

#ifndef H_PPxFSUtils
#define H_PPxFSUtils
#pragma once

#include <PPxPrefix.h>

#include <CoreServices/CoreServices.h>

// ---------------------------------------------------------------------------
//	Global comparison operators for FSRef

bool	operator == (const FSRef& inRight, const FSRef& inLeft);

bool	operator != (const FSRef& inRight, const FSRef& inLeft);


// ---------------------------------------------------------------------------

namespace PPx {

class CFString;


// ===========================================================================
//	FSUtils
/**
	Utility functions for working with files and folders */

namespace FSUtils {

	SInt32			CompareFSNames(
							const HFSUniStr255&	inNameOne,
							const HFSUniStr255&	inNameTwo);

	SInt32			CompareFSNames(
							const CFString&	inNameOne,
							const CFString&	inNameTwo);

	bool			FSNamesAreEqual(
							const HFSUniStr255&	inNameOne,
							const HFSUniStr255&	inNameTwo);

	bool			FSNamesAreEqual(
							const CFString&	inNameOne,
							const CFString&	inNameTwo);
	
	void			StringToHFSUniStr(
							const CFString&	inString,
							HFSUniStr255&	outHFSUniStr);

} // namespace FSUtils


} // namespace PPx

#endif // H_PPxFSUtils
