// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEnvironment.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxEnvironment.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	GestaltValue
/**
	Constructor
	
	@param	inSelector		Gestalt selector
	
	Stores the gestalt value for the specified selector						*/

GestaltValue::GestaltValue(
	OSType	inSelector)
{
	OSErr	err = ::Gestalt(inSelector, &mValue);
	
	PPx_ThrowIfOSError_(err, "Gestalt failed");
}


// ---------------------------------------------------------------------------
//	GetOSVersion
/**
	@return	OS version as a hexadecimal-encoded number
	
	For example, OS version 10.2.8 is represented as 0x1028					*/

SInt16
Environment::GetOSVersion()
{
										// OS Version is in the low word
										//   of the gestalt value
	static	SInt16	sOSVersion = static_cast<SInt16>( 0x0000FFFF &
							GestaltValue(gestaltSystemVersion).GetValue() );
	
	return sOSVersion;
}


} // namespace PPx