// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFSUtils.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:19 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxFSUtils.h>
#include <SysCFString.h>

#include <CoreServices/CoreServices.h>

// ---------------------------------------------------------------------------

namespace {

	// -----------------------------------------------------------------------
	//	Options for comparing file names

	const UCCollateOptions		collateOpts_HFSFileNames =
										kUCCollateComposeInsensitiveMask	|
										kUCCollateWidthInsensitiveMask		|
										kUCCollateCaseInsensitiveMask		|
										kUCCollateDigitsOverrideMask		|
										kUCCollateDigitsAsNumberMask		|
										kUCCollatePunctuationSignificantMask;
}


// ---------------------------------------------------------------------------
//	Eauality operator for FSRef
/**
	Compares two FSRefs for equality
	
	@param	inRight		Right hand side FSRef
	@param	inLeft		Left hand side FSRef
	
	@returns Whether the FSRefs are equal									*/

bool
operator == (
	const FSRef&	inRight,
	const FSRef&	inLeft)
{
	return (::FSCompareFSRefs(&inRight, &inLeft) == noErr);
}


// ---------------------------------------------------------------------------
//	Inequality operator for FSRef
/**
	Compares two FSRefs for inequality
	
	@param	inRight		Right hand side FSRef
	@param	inLeft		Left hand side FSRef
	
	@returns Whether the FSRefs are unequal									*/

bool
operator != (const FSRef &inRight, const FSRef &inLeft)
{
	return (::FSCompareFSRefs(&inRight, &inLeft) != noErr);
}


namespace PPx {

// ---------------------------------------------------------------------------
//	CompareFSNames
/**
	Compares file entity HFSUniStr255 names for sorting purposes. Uses the
	same sorting logic as the Mac OS X Finder
	
	@param	inNameOne	Right hand side operand
	@param	inNameTwo	Left hand side operand
	
	@return	Result of comparing inNameOne to inNameTwo
	
	@retval	Zero	Names are equivalent
	@retval > 0		inNameOne is greater than inNameTwo
	@retval < 0		inNameOne is less than inNameTwo
*/

SInt32
FSUtils::CompareFSNames(
	const HFSUniStr255&	inNameOne,
	const HFSUniStr255&	inNameTwo)
{
	SInt32 compareResult = 0;
	
	OSStatus status = ::UCCompareTextDefault( collateOpts_HFSFileNames,
											  inNameOne.unicode, inNameOne.length,
											  inNameTwo.unicode, inNameTwo.length,
											  nil, &compareResult );
	PPx_ThrowIfOSError_(status, "UCCompareTextDefault failed");

	return compareResult;
}


// ---------------------------------------------------------------------------
//	CompareFSNames
/**
	Compares file entity CFString names for sorting purposes. Uses the same
	sorting logic as the Mac OS X Finder
	
	@param	inNameOne	Right hand side operand
	@param	inNameTwo	Left hand side operand
	
	@return	Result of comparing inNameOne to inNameTwo
	
	@retval	Zero	Names are equivalent
	@retval > 0		inNameOne is greater than inNameTwo
	@retval < 0		inNameOne is less than inNameTwo
*/

SInt32
FSUtils::CompareFSNames(
	const CFString&	inNameOne,
	const CFString&	inNameTwo)
{
	HFSUniStr255 nameOne;
	StringToHFSUniStr(inNameOne, nameOne);

	HFSUniStr255 nameTwo;
	StringToHFSUniStr(inNameTwo, nameTwo);

	return (CompareFSNames(nameOne, nameTwo));
}


// ---------------------------------------------------------------------------
//	FSNamesAreEqual
/**
	Compares file entity HFSUniStr255 names for equivalence
	
	@param	inNameOne	Right hand side operand
	@param	inNameTwo	Left hand side operand
	
	@return	Whether the two strings are equivalent
	
	@note Comparing specifically for equality may be significantly faster
	than the general comparison performed by FSUtils::CompareFSNames
*/

bool
FSUtils::FSNamesAreEqual(
	const HFSUniStr255&	inNameOne,
	const HFSUniStr255&	inNameTwo)
{
	Boolean equivalent = false;

	OSStatus status = ::UCCompareTextDefault( collateOpts_HFSFileNames,
											  inNameOne.unicode, inNameOne.length,
											  inNameTwo.unicode, inNameTwo.length,
											  &equivalent, nil);
	PPx_ThrowIfOSError_(status, "UCCompareTextDefault failed");

	return equivalent;
}


// ---------------------------------------------------------------------------
//	FSNamesAreEqual
/**
	Compares file entity CFString names for equivalence
	
	@param	inNameOne	Right hand side operand
	@param	inNameTwo	Left hand side operand
	
	@return	Whether the two strings are equivalent
	
	@note Comparing specifically for equality may be significantly faster
	than the general comparison performed by FSUtils::CompareFSNames
*/

bool
FSUtils::FSNamesAreEqual(
	const CFString&	inNameOne,
	const CFString&	inNameTwo)
{
	HFSUniStr255 nameOne;
	StringToHFSUniStr(inNameOne, nameOne);

	HFSUniStr255 nameTwo;
	StringToHFSUniStr(inNameTwo, nameTwo);

	return FSNamesAreEqual(nameOne, nameTwo);
}


// ---------------------------------------------------------------------------
//	StringToHFSUniStr
/**
	Converts a CFString to a HFS unicode string
	
	@param	inString		Input CFString
	@param	outHFSUniStr	String converted to HFSUniStr255				*/

void
FSUtils::StringToHFSUniStr(
	const CFString&	inString,
	HFSUniStr255&	outHFSUniStr)
{
	outHFSUniStr.length = inString.GetLength();
	
	if (outHFSUniStr.length > 255) {
		PPx_Throw_(RuntimeError, err_CFStringConvert,
					"CFString too long to convert to HFSUniStr255");
	}
	
	if (outHFSUniStr.length > 0) {
		inString.GetSubstring( ::CFRangeMake(0, outHFSUniStr.length),
							   outHFSUniStr.unicode );
	}
}


} // namespace PPx