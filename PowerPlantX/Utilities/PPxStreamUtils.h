// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxStreamUtils.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxStreamUtils.h
	@brief		Utility functions for working with standard streams
*/

#ifndef H_PPxStreamUtils
#define H_PPxStreamUtils
#pragma once

#include <PPxPrefix.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ostream>

// ---------------------------------------------------------------------------
//	Global stream output operators for common Mac types.

std::ostream&	operator << ( std::ostream&		inStream, 
							  ConstStringPtr	inPascalStr );
							  
std::ostream&	operator << ( std::ostream&		inStream,
							  CFStringRef		inCFString );

std::ostream&	operator << ( std::ostream&		inStream,
							  Point				inPoint );

std::ostream&	operator << ( std::ostream&		inStream,
							  const Rect&		inRect );

namespace PPx {

// ===========================================================================
//	StreamUtils
/**
	Utility functions for working with the standard IOStream library */

namespace StreamUtils {

void		WriteLinesOfText(
				std::ostream&	inOutputStream,
				const char*		inTextPtr,
				long			inTextLength,
				char			inLineEndChar);
				
} // namespace StreamUtils

} // namespace PPx

#endif	// H_PPxStreamUtils