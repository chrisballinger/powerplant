// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxConstants.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxConstants.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CoreFoundation string encodings

		// $Issue$ Can the system encoding change at runtime?

const CFStringEncoding	encoding_System = ::CFStringGetSystemEncoding();


// ---------------------------------------------------------------------------
//	CoreFoundation strings

const CFStringRef		cfString_Empty = CFSTR("");
const unsigned char		pString_Empty[]	= { 0 };


// ---------------------------------------------------------------------------
//	CoreFoundation sizes, positions, and ranges

const CFRange	cfRange_All		= { 0, cfIndex_Last };
const CFRange	cfRange_Empty	= { 0, 0 };


} // namespace PPx