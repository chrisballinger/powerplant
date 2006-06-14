// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSignature.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:28 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSignature.h
	@brief		Functions getting and setting the signature of a program
*/

#ifndef H_PPxSignature
#define H_PPxSignature
#pragma once

#include <PPxPrefix.h>

namespace PPx {

// ===========================================================================
//	Signature
/**
	Sets/Gets the four-character code signature for the program.
	
	In addition to its common use as the creator code for files,
	the Toolbox also uses a signature to identify user-supplied data
	stored by the OS. For example, properties associated with Controls.
	
	If you don't call Set, the Get function will try to get the
	signature from info.plist of the PrimaryBundle							*/

namespace	Signature {

	void	Set( OSType inSignature );
	
	OSType	Get();
}


} // namespace PPx

#endif	// H_PPxSignature
