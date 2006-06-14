// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFString.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:30 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFString.h>
#include <SysCFUtils.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Default constructor	*/

CFString::CFString()
{
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Construct from an immutable CFStringRef

	@param	inStringRef		CF string reference
	@param	inRetain		Whether to retain the CF string
	
	@note Toolbox functions which return a CFStringRef and have the word
	"copy" or "create" in their name return a string with a retain count
	of one. Pass false for inRetain when constructing from such a string.
	The CFString won't retain the string, but will release the string in
	its destructor.
	
	Toolbox functions which return a CFStringRef and have the word "get"
	in their name return a string without incrementing its retain count.
	Pass true for inRetain when constructing from such a string.
	The CFString will retain the string, and later release it.				*/

CFString::CFString(
	CFStringRef	inStringRef,
	bool		inRetain)
	
	: BaseT(inStringRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Constructs from an existing mutable CF string

	@param	inStringRef		CF string reference
	@param	inRetain		Whether to retain the CF string
	
	@note Pass false for inRetain if you are transferring ownership of the
	CF string, which is normally the case if you obtained it from a
	call to a Toolbox function which copies a CF string.
	
	Pase true for inRetain if the caller wants to maintain ownership
	of the CF string. In this case, we make a copy, so that multiple
	clients aren't changing the same data.									*/

CFString::CFString(
	CFMutableStringRef	inStringRef,
	bool				inRetain)
{
	AttachMutableRef(inStringRef, inRetain);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Constructs from a Pascal string
	
	@param	inPascalString	Pascal string to copy
	@param	inEncoding		Encoding of Pascal string
	@param	inAllocator		CF Allocator									*/

CFString::CFString(
	ConstStringPtr		inPascalString,
	CFStringEncoding	inEncoding,
	CFAllocatorRef		inAllocator)
{
	CFStringRef	ref = ::CFStringCreateWithPascalString(inAllocator,
							inPascalString, inEncoding);
	PPx_ThrowIfCFCreateFailed_(ref, "CFStringCreateWithPascalString");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Constructs from a C string
	
	@param	inCString		C string to copy
	@param	inEncoding		Encoding of C string
	@param	inAllocator		CF Allocator									*/

CFString::CFString(
	const char*			inCString,
	CFStringEncoding	inEncoding,
	CFAllocatorRef		inAllocator)
{
	CFStringRef	ref = ::CFStringCreateWithCString(inAllocator,
							inCString, inEncoding);
	PPx_ThrowIfCFCreateFailed_(ref, "CFStringCreateWithCString");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Constructs from a buffer of unicode characters
	
	@param	inUniChars		Array of unicode characters
	@param	inCharCount		Number of unicode characters to copy
	@param	inAllocator		CF Allocator									*/

CFString::CFString(
	const UniChar*		inUniChars,
	CFIndex				inCharCount,
	CFAllocatorRef		inAllocator)
{
	CFStringRef	ref = ::CFStringCreateWithCharacters(inAllocator,
							inUniChars, inCharCount);
	PPx_ThrowIfCFCreateFailed_(ref, "CFStringCreateWithCharacters");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Constructs from a file system unicode string
	
	@param	inHFSUniStr		A HFSUniStr255 string
	@param	inAllocator		CF Allocator									*/

CFString::CFString(
	const HFSUniStr255&	inHFSUniStr,
	CFAllocatorRef		inAllocator)
{
	CFStringRef	ref = ::CFStringCreateWithCharacters(inAllocator,
							inHFSUniStr.unicode, inHFSUniStr.length);
	PPx_ThrowIfCFCreateFailed_(ref, "CFStringCreateWithCharacters");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Constructs from a buffer of bytes
	
	@param	inBuffer			Pointer to a buffer
	@param	inByteCount			Number of bytes to copy from buffer
	@param	inEncoding			Encoding of bytes in buffer
	@param	inIsExternalRep		Whether the bytes come from an external
									representation (where there may be
									a BOM (btye order mark) character
	@param	inAllocator			CF Allocator								*/

CFString::CFString(
	const void*			inBuffer,
	CFIndex				inByteCount,
	CFStringEncoding	inEncoding,
	bool				inIsExternalRep,
	CFAllocatorRef		inAllocator)
{
	CFStringRef	ref = ::CFStringCreateWithBytes(inAllocator,
							(UInt8*) inBuffer, inByteCount, inEncoding,
							inIsExternalRep);
	PPx_ThrowIfCFCreateFailed_(ref, "CFStringCreateWithBytes");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Constructs from a std::string
	
	@param	inString		std::string from which to copy
	@param	inEncoding		Encoding of text in std::string
	@param	inAllocator		CF Allocator									*/

CFString::CFString(
	const std::string&	inString,
	CFStringEncoding	inEncoding,
	CFAllocatorRef		inAllocator)
{
	CFStringRef	ref = ::CFStringCreateWithCString(inAllocator,
							inString.c_str(), inEncoding);
	PPx_ThrowIfCFCreateFailed_(ref, "CFStringCreateWithCString");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Copy constructor */

CFString::CFString(
	const CFString&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment Operator */

CFString&
CFString::operator = (
	const CFString&	inSource)
{
	AssignObject(inSource);
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetLength														  [public]
/**
	Returns number of unicode characters in the string
	
	@return Number of unicode characters in the string						*/

CFIndex
CFString::GetLength() const
{
	return ::CFStringGetLength( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetByteLength													  [public]
/**
	Returns the byte length of a range of characters if they were converted
	to the specified encoding
	
	@param	inRange			Range in string
	@param	inEncoding		Encoding for which to determine byte length
	@param	inLossByte		Byte for characters that can't be converted
								to encoding
	@param	inIsExternalRep	Whether bytes are intended for external storage
	
	@return	Bytes length of a range of characters if they were converted
			to the specified encoding										*/
				
CFIndex
CFString::GetByteLength(
	CFRange				inRange,
	CFStringEncoding	inEncoding,
	UInt8				inLossByte,
	bool				inIsExternalRep) const
{
	CFUtils::VerifyRange(GetLength(), inRange);

	CFIndex	byteLength;
	::CFStringGetBytes(UseRef(), inRange, inEncoding, inLossByte,
						inIsExternalRep, nil, 0, &byteLength);

	return byteLength;
}


// ---------------------------------------------------------------------------
//	GetByteRange													  [public]
/**
	Fills in buffer with bytes converted from a range in the string to
	the specified encoding
	
	@param	inBufferSize	Size of buffer
	@param	outBuffer		Pointer to buffer
	@param	inRange			Range of characters in the string
	@param	inEncoding		Encoding to use for bytes
	@param	inLossByte		Byte to use for characters that can't be
								converted to the encoding
	@param	inIsExternalRep	Whether bytes are intended for external storage	*/

CFIndex
CFString::GetByteRange(
	CFIndex				inBufferSize,
	UInt8*				outBuffer,
	CFRange				inRange,
	CFStringEncoding	inEncoding,
	UInt8				inLossByte,
	bool				inIsExternalRep) const
{
	CFUtils::VerifyRange(GetLength(), inRange);

	return ::CFStringGetBytes(UseRef(), inRange, inEncoding, inLossByte,
							inIsExternalRep, outBuffer, inBufferSize, nil);
}


// ---------------------------------------------------------------------------
//	GetCharacterAt													  [public]
/**
	Returns the unicode character at the given index in the string
	
	@param	inIndex		Index in string
	
	@return	Unicode character at the given index							*/

UniChar
CFString::GetCharacterAt(
	CFIndex	inIndex) const
{
	CFUtils::VerifyIndex(GetLength(), inIndex);
	return ::CFStringGetCharacterAtIndex(UseRef(), inIndex);
}


// ---------------------------------------------------------------------------
//	operator []														  [public]
/**
	Returns the unicode character at the given index in the string
	
	@param	inIndex		Index in string
	
	@return	Unicode character at the given index							*/

UniChar
CFString::operator [] (
	CFIndex	inIndex) const
{
	return GetCharacterAt(inIndex);
}


// ---------------------------------------------------------------------------
//	GetSubstring													  [public]
/**
	Passes back a range of the string in a unicode characater buffer
	
	@param	inRange		Range of string to copy
	@param	outBuffer	Pointer to unicode character buffer					*/

void
CFString::GetSubstring(
	CFRange		inRange,
	UniChar*	outBuffer) const
{
	CFUtils::VerifyRange(GetLength(), inRange);
	::CFStringGetCharacters(UseRef(), inRange, outBuffer);
}


// ---------------------------------------------------------------------------
//	GetUniStringPtr													  [public]
/**
	Returns a pointer to a UniChar string
	
	Returns nil if the internal storage format is not compatible
	with an array of UniChar characters										*/

const UniChar*
CFString::GetUniStringPtr() const
{
	return ::CFStringGetCharactersPtr( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetPascalString													  [public]
/**
	Passes back the CFString as a Pascal string
	
	@param	outBuffer		Pointer to a Pascal string buffer
	@param	inBufferSize	Size of Pascal string buffer
	@param	inEncoding		Encoding to use to convert string
	
	@return	Whether the conversion to a Pascal string was successful		*/

bool
CFString::GetPascalString(
	StringPtr			outBuffer,
	CFIndex				inBufferSize,
	CFStringEncoding	inEncoding) const
{
	return ::CFStringGetPascalString(UseRef(), outBuffer, inBufferSize,
										inEncoding);
}


// ---------------------------------------------------------------------------
//	GetPascalStringPtr												  [public]
/**
	Returns a pointer to a Pascal string
	
	@param	inEncoding	Encoding to use to convert string
	
	@return	Pascal string pointer to internal buffer of the CFString
	
	Returns nil if the internal storage format is not compatible
	with a Pascal string													*/

ConstStringPtr
CFString::GetPascalStringPtr(
	CFStringEncoding	inEncoding) const
{
	return ::CFStringGetPascalStringPtr(UseRef(), inEncoding);
}


// ---------------------------------------------------------------------------
//	GetCString														  [public]
/**
	Passes back the CFString as a C string
	
	@param	outBuffer		Pointer to a C string buffer
	@param	inBufferSize	Size of the C striing buffer
	@param	inEncoding		Encoding to use to convert characters
	
	@return	Whether the conversion to a C string was successful				*/

bool
CFString::GetCString(
	char*				outBuffer,
	CFIndex				inBufferSize,
	CFStringEncoding	inEncoding) const
{
	return ::CFStringGetCString(UseRef(), outBuffer, inBufferSize,
										inEncoding);
}


// ---------------------------------------------------------------------------
//	GetCStringPtr													  [public]
/**
	Returns pointer to a C string
	
	@param	inEncoding		Encoding to use to convert string
	
	@return	C string pointer to internal buffer of the CFString
	
	Returns nil if the internal storage format is not compatible with
	a C string																*/

const char*
CFString::GetCStringPtr(
	CFStringEncoding	inEncoding) const
{
	return ::CFStringGetCStringPtr(UseRef(), inEncoding);
}


// ---------------------------------------------------------------------------
//	GetString														  [public]
/**
	Pass back a copy of the string as a std::string
	
	@param	outString		std::string in which to copy string
	@param	inEncoding		Encoding to use to convert string				*/

void
CFString::GetString(
	std::string&		outString,
	CFStringEncoding	inEncoding) const
{
	CFIndex	byteLength = GetByteLength(cfRange_All, inEncoding);
	
	if (byteLength > 0) {
		outString.resize(byteLength);
		UInt8*	buffer = reinterpret_cast<UInt8 *>(&outString[0]);
		CFRange	allRange = { 0, GetLength() };
		
		::CFStringGetBytes(UseRef(), allRange, inEncoding, 0, false,
							buffer, byteLength, &byteLength);
	
	} else {
		outString.clear();
	}
}


// ---------------------------------------------------------------------------
//	Get4CharCodeValue												  [public]
/**
	Returns FourCharCode respresented by the string
	
	@return FourCharCode respresented by the string							*/

FourCharCode
CFString::Get4CharCodeValue() const
{
	FourCharCode code = 0;

	CFIndex actualLength;
	::CFStringGetBytes(UseRef(), ::CFRangeMake(0, sizeof(FourCharCode)),
						kCFStringEncodingMacRoman, 0, false,
						(UInt8 *)&code, sizeof(FourCharCode), &actualLength);

	return code;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Append															  [public]
/**
	Appends a CFStringRef to the string
	
	@param	inString	String to append									*/

void
CFString::Append(
	CFStringRef	inString)
{
	::CFStringAppend(UseMutableRef(), inString);
}


// ---------------------------------------------------------------------------
//	Append															  [public]
/**
	Appends an array of unicode characters to the string
	
	@param	inChars		Pointer to unicode character array
	@param	inCharCount	Number of unicdoe characters to append				*/

void
CFString::Append(
	const UniChar*	inChars,
	CFIndex			inCharCount)
{
	::CFStringAppendCharacters(UseMutableRef(), inChars, inCharCount);
}


// ---------------------------------------------------------------------------
//	Append															  [public]
/**
	Appends a Pascal string to the string
	
	@param	inPascalString		Pascal string pointer
	@param	inEncoding			Encoding of Pascal string					*/

void
CFString::Append(
	ConstStringPtr		inPascalString,
	CFStringEncoding	inEncoding)
{
	::CFStringAppendPascalString(UseMutableRef(), inPascalString, inEncoding);
}


// ---------------------------------------------------------------------------
//	Append															  [public]
/**
	Appends a C string to the string
	
	@param	inCString		C string pointer
	@param	inEncoding		Encoding of C string							*/

void
CFString::Append(
	const char*			inCString,
	CFStringEncoding	inEncoding)
{
	::CFStringAppendCString(UseMutableRef(), inCString, inEncoding);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Insert															  [public]
/**
	Insert a CFStringRef into the string
	
	@param	inIndex		Index at which to insert
	@param	inString	CFStringRef for string to insert					*/

void
CFString::Insert(
	CFIndex			inIndex,
	CFStringRef		inString)
{
	CFUtils::VerifyInsertIndex(GetLength(), inIndex);
	::CFStringInsert(UseMutableRef(), inIndex, inString);
}


// ---------------------------------------------------------------------------
//	Pad																  [public]
/**
	Pads or reduces a string to the specified length
	
	@param	inPadString		String of padding characters
	@param	inLength		Set string to this character length
	@param	inPadPosition	Start position in pad string
	
	If inLength is larger than the current string size, string grows to
	inLength and the extra characters are filled with characters from the
	pad string, starting at inPodPosition. If there are more extra characters
	than in the pad string, it wraps around to the start of the pad string.
	
	If inLength is smaller tha the current string size, string is truncated
	to inLength characters.													*/

void
CFString::Pad(
	CFStringRef		inPadString,
	CFIndex			inLength,
	CFIndex			inPadPosition)
{
	::CFStringPad(UseMutableRef(), inPadString, inLength, inPadPosition);
}


// ---------------------------------------------------------------------------
//	Delete															  [public]
/**
	Deletes a range of characters from the string
	
	@param inRange		Range of characters to delete						*/

void
CFString::Delete(
	CFRange	inRange)
{
	CFUtils::VerifyRange(GetLength(), inRange);
	::CFStringDelete(UseMutableRef(), inRange);
}


// ---------------------------------------------------------------------------
//	Replace															  [public]
/**
	Replaces a range of characters in the string with another string
	
	@param	inRange			Range of characters to replace
	@param	inReplacement	String to put in place of character range
	
	String grows or shrinks if range and replacment string have different
	lengths.																*/

void
CFString::Replace(
	CFRange		inRange,
	CFStringRef	inReplacement)
{
	CFUtils::VerifyRange(GetLength(), inRange);
	::CFStringReplace(UseMutableRef(), inRange, inReplacement);
}


// ---------------------------------------------------------------------------
//	ReplaceAll														  [public]
/**
	Replaces entires contents of string with another string
	
	@param	inReplacement	Replacement string								*/

void
CFString::ReplaceAll(
	CFStringRef	inReplacement)
{
	::CFStringReplaceAll(UseMutableRef(), inReplacement);
}


// ---------------------------------------------------------------------------
//	Assign4CharCode													  [public]
/**
	Sets contents of string to a four-character code
	
	@param	inCode			Four-character code
	@param	inAllocator		CF Allocator									*/

void
CFString::Assign4CharCode(
	FourCharCode	inCode,
	CFAllocatorRef	inAllocator)
{
	AssignObject( CFString(&inCode, sizeof(FourCharCode),
							kCFStringEncodingMacRoman, false, inAllocator) );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	CompareTo														  [public]
/**
	Compares string to a CFStringRef
	
	@param	inStringRef		String to which to compare
	@param	inOptions		Comparison options
	
	@return Comparison result
	
	@retval	kCFCompareLessThan		String is less than input string
	@retval	kCFCompareEqualTo		String is equal to input string
	@retval	kCFCompareGreaterThan	String is greater than input string		*/

CFComparisonResult
CFString::CompareTo(
	CFStringRef		inStringRef,
	CFOptionFlags	inOptions) const
{
	return ::CFStringCompare(UseRef(), inStringRef, inOptions);
}


// ---------------------------------------------------------------------------
//	FindInRange														  [public]
/**
	Finds a search string within a range of the string
	
	@param	inRange			Range to search
	@param	inSearchStr		String to search for
	@param	inOptions		String comparison flags
	
	@return	Range of where search string is in the String					*/

CFRange
CFString::FindInRange(
	CFRange			inRange,
	CFStringRef		inSearchStr,
	CFOptionFlags	inOptions) const
{
	CFUtils::VerifyRange(GetLength(), inRange);
	
	CFRange		foundRange;
	if (::CFStringFindWithOptions(UseRef(), inSearchStr, inRange,
									inOptions, &foundRange) == false) {
		foundRange.length = 0;
	}
	
	return foundRange;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	MutableCopy														 [private]
/**
	Returns a CFMutableStringRef containing a copy of the input string
	
	@param	inRefToCopy		String to copy
	
	@retval	Mutable string copy of the input string							*/

CFMutableStringRef
CFString::MutableCopy(
	CFStringRef	inRefToCopy) const
{
	CFMutableStringRef	copyRef = ::CFStringCreateMutableCopy(
										::CFGetAllocator(inRefToCopy),
										cfSize_Unlimited, inRefToCopy);
	PPx_ThrowIfCFCreateFailed_(copyRef, "CFStringCreateMutableCopy");
	
	return copyRef;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	CFString + CFString

CFString
operator + (
	const CFString&	inLeft,
	const CFString&	inRight)
{
	CFString	result(inLeft);
	result.Append(inRight);
	return result;
}


// ---------------------------------------------------------------------------
//	CFString + CFStringRef

CFString
operator + (
	const CFString&	inLeft,
	CFStringRef		inRight)
{
	CFString	result(inLeft);
	result.Append(inRight);
	return result;
}


// ---------------------------------------------------------------------------
//	CFStringRef + CFString

CFString
operator + (
	CFStringRef		inLeft,
	const CFString&	inRight)
{
	CFString	result(inLeft);
	result.Append(inRight);
	return result;
}


// ---------------------------------------------------------------------------
//	CFString + Pascal String

CFString
operator + (
	const CFString&	inLeft,
	ConstStringPtr	inRight)
{
	CFString	result(inLeft);
	result.Append(inRight);
	return result;
}


// ---------------------------------------------------------------------------
//	Pascal String + CFString

CFString
operator + (
	ConstStringPtr	inLeft,
	const CFString&	inRight)
{
	CFString	result(inLeft);
	result.Append(inRight);
	return result;
}


// ---------------------------------------------------------------------------
//	CFString + C String

CFString
operator + (
	const CFString&	inLeft,
	const char*		inRight)
{
	CFString	result(inLeft);
	result.Append(inRight);
	return result;
}


// ---------------------------------------------------------------------------
//	C String + CFString

CFString
operator + (
	const char*		inLeft,
	const CFString&	inRight)
{
	CFString	result(inLeft);
	result.Append(inRight);
	return result;
}


} // namespace PPx