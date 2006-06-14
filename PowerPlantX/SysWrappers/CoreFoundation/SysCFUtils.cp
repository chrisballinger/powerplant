// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFUtils.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:31 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFUtils.h>
#include <SysCFString.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	GetEncodingFromScriptCode
/**
	Converts a ScriptCode to a CFStringEncoding. A CFStringEncoding is
	equivalent to a TextEncoding, so UpgradeScriptInfoToTextEncoding can be
	used for the conversion.
	
	@param	inScript		The ScriptCode to convert
	@param	inLanguage		The language of inScript.
								The default is kTextLanguageDontCare.
	@param	inRegion		The region of inScript.
								The default is kTextRegionDontCare.
	@param	inFontName		The name of the font used for converting inScript.
								The default is nil.

	@return	A CFStringEncoding equivalent to inScript						*/

CFStringEncoding
CFUtils::GetEncodingFromScriptCode(
	ScriptCode			inScript,
	LangCode			inLanguage,
	RegionCode			inRegion,
	ConstStr255Param	inFontName)
{
	CFStringEncoding	encoding;
    OSStatus err = ::UpgradeScriptInfoToTextEncoding(inScript, inLanguage,
    									inRegion, inFontName, &encoding);
    PPx_ThrowIfOSError_(err, "UpgradeScriptInfoToTextEncoding failed");
 
 	return encoding;
}


// ---------------------------------------------------------------------------
//	GetIndString
/**
	Returns a CFString from the contents of an item in a STR# resource
	
	@param	inSTRxID		Resource ID of a STR# resource.
	@param	inIndex			Index of a string within the STR# resource
								specified by inSTRxID.
	@param	inEncoding		Encoding used to convert the loaded string
								to unicode. The default is encoding_System.
	@param	inAllocator		Allocator for the CFString. The default is nil.
	
	@return	A CFString containing the contents of the STR# resource item	*/

CFString
CFUtils::GetIndString(
	ResIDT				inSTRxID,
	SInt16				inIndex,
	CFStringEncoding	inEncoding,
	CFAllocatorRef		inAllocator)
{
	Str255 resStr;
	resStr[0] = 0;

	::GetIndString(resStr, inSTRxID, inIndex);
	
	return CFString(resStr, inEncoding, inAllocator);
}


// ---------------------------------------------------------------------------
//	MakeValidRange
/**
	Returns whether the supplied range is valid for a CF container of the
	supplied length. Also, if the supplied range is a logical range rather
	than actual range, it will be converted to the corresponding actual range.
	
	@param	inMaxLength		The maximum possible length of the range.
	@param	ioRange			On input: the actual or logical range to validate.
								On output: a valid actual range if the input
								value was valid, otherwise an invalid range.
								
	@return	Whether ioRange is valid.										*/

bool
CFUtils::MakeValidRange(
	CFIndex		inMaxLength,
	CFRange&	ioRange)
{
	bool hasValidLoc = false;
	bool hasValidLength = false;

	if (inMaxLength > 0) {
	
		if (ioRange.location == cfIndex_Last) {
			ioRange.location = inMaxLength - 1;
		}

		hasValidLoc = ((ioRange.location >= 0) && (ioRange.location < inMaxLength));

		hasValidLength = false;
		if (hasValidLoc) {
			if (ioRange.length == cfIndex_Last) {
				ioRange.length = inMaxLength;
			}

			hasValidLength = ((ioRange.length > 0) &&
							  (ioRange.location + ioRange.length <= inMaxLength));
		}
	}

	return (hasValidLoc && hasValidLength);
}


// ---------------------------------------------------------------------------
//	VerifyRange
/**
	Throws an exception if the supplied range is invalid for a CF container
	of the supplied length. Also, if the supplied range is a logical range
	rather than actual range, it will be converted to the corresponding
	actual range.
	
	@param	inMaxLength		The maximum possible length of the range.
	@param	ioRange			On input: the actual or logical range to validate.
								On output: a valid actual range.			*/

void
CFUtils::VerifyRange(
	CFIndex		inMaxLength,
	CFRange&	ioRange)
{
	if (MakeValidRange(inMaxLength, ioRange) == false) {
		PPx_Throw_(LogicError, err_Logic, "Invalid CFRange");
	}
}


// ---------------------------------------------------------------------------
//	MakeValidIndex
/**
	Returns whether the supplied index is valid for a CF container of the
	supplied length. Also, if the supplied index is a logical range rather
	than actual index, it will be converted to the corresponding actual index.
	
	@param	inCount			Number of items. 0 <= index <= inCount - 1
	@param	ioIndex			On input: the actual or logical index to validate.
								On output: a valid actual index if the input
								value was valid, otherwise unchanged from the
								input value.
								
	@return	Whether ioIndex is valid										*/

bool
CFUtils::MakeValidIndex(
	CFIndex		inCount,
	CFIndex&	ioIndex)
{
	bool	isValid = false;

	if (ioIndex < inCount) {
		isValid = (inCount >= 0);
		
	} else if (ioIndex == cfIndex_Last) {
		isValid = (inCount > 0);
		
		if (isValid == true) {
			ioIndex = inCount - 1;
		}
	}

	return isValid;
}


// ---------------------------------------------------------------------------
//	VerifyIndex
/**
	Throws an exception if the supplied index is invalid for a CF container
	of the supplied length. Also, if the supplied index is a logical index
	rather than actual index, it will be converted to the corresponding
	actual index.
	
	@param	inCount			Number of items. 0 <= index <= inCount - 1
	@param	ioIndex			In: Index value to validate
								Out: Valid index value						*/

void
CFUtils::VerifyIndex(
	CFIndex		inCount,
	CFIndex&	ioIndex)
{
	if (MakeValidIndex(inCount, ioIndex) == false) {
		PPx_Throw_(LogicError, err_Logic, "Invalid CFIndex");
	}
}


// ---------------------------------------------------------------------------
//	MakeInsertIndex
/**
	Verifies an insertion index and adjusts it if it is a special flag
	indication the last index
	
	@param	inCount		Number of items in container
	@param	ioIndex		Index into container								*/	

bool
CFUtils::MakeInsertIndex(
	CFIndex		inCount,
	CFIndex&	ioIndex)
{
	bool	isValid = false;
	
	if (ioIndex <= inCount) {
		isValid = true;
		
	} else if (ioIndex == cfIndex_Last) {
		isValid = true;
		ioIndex = inCount;
	}
	
	return isValid;
}


// ---------------------------------------------------------------------------
//	VerifyInsertIndex
/**
	Throws an exception if an insertion index into a container is not valid
	
	@param	inCount		Number of items in container
	@param	ioIndex		Index into container								*/

void
CFUtils::VerifyInsertIndex(
	CFIndex		inCount,
	CFIndex&	ioIndex)
{
	if (MakeInsertIndex(inCount, ioIndex) == false) {
		PPx_Throw_(LogicError, err_Logic, "Invalid insertion CFIndex");
	}
}


} // namespace PPx