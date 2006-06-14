// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSignature.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:28 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxSignature.h>
#include <PPxPrimaryBundle.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Local declarations

namespace Signature {

	const OSType	signature_Default		= 'PPxS';
	const CFString	propListKey_Signature	= CFSTR("CFBundleSignature");

	OSType			sSignature = 0;
	
	void			SetSignatureFromBundle();
}


// ---------------------------------------------------------------------------
//	Set
/**
	Sets the signature for the program
	
	@param	inSignature		Signature for the program						*/

void
Signature::Set(
	OSType	inSignature)
{
	sSignature = inSignature;
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns the signature for the program
	
	@return Signature for the program
	
	If you don't call Set, we try to get the signature from the info.plist
	of the PrimaryBundle													*/

OSType
Signature::Get()
{
	if (sSignature == 0) {
		SetSignatureFromBundle();
	}
		
	return sSignature;
}


// ---------------------------------------------------------------------------
//	SetSignatureFromBundle
/**
	Tries to get signature for the program from its primary bundle */

void
Signature::SetSignatureFromBundle()
{
									// Try to get type and creator of
									//   primary bundle's package
	OSType	theType;
	PPx::PrimaryBundle::Instance().GetPackageInfo(theType, sSignature);
	
	if (sSignature == 0) {			// Couldn't get creator from Bundle
									//   Use a default value
		sSignature = signature_Default;
	}
}


} // namespace PPx