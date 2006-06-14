// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMIntegerAttribute.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMIntegerAttribute.cpp $
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added PreValueChange method.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/18/97   Time: 18:01
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Switched #ifdef Constructor_WindowsHost to #if.
//	
//	*****************  Version 10  *****************
//	User: Andrew       Date: 2/17/97    Time: 4:27p
//	Updated in $/ConstructorWin/Sources
//	ifdef's inclusion of string.h for compiling with MSVC/altura
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/07/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added #include <string.h> so strcpy would work.
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/05/96   Time: 12:42
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added GetMinValue and GetMaxValue methods.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:11
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Fixed a bug in ReadStreamDataSelf that caused signed 16-bit integers to
//	be read incorrectly.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/22/96   Time: 14:52
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Fixed a bug in SetTextValue which caused it to read negative numbers as
//	text values in optional text fields.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/24/96   Time: 00:10
//	Checked in '$/Constructor/Source files/Data model/Attributes'
//	Comment: Improved handling of text values.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/18/96   Time: 00:27
//	Checked in '$/Constructor/Source files/Data model/Attributes'
//	Comment: Added support for character string values.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
	#include "ctor.h"
#endif

// ===========================================================================

#include "DMIntegerAttribute.h"

	// Core : Data model : Exceptions
#include "DMRangeCheckException.h"

	// Core : Data model : Undo-redo
#include "DMAttributeSubAction.h"
#include "StDMTransactionBuilder.h"

	// Core : Utilities
#include "UTextID.h"

	// PowerPlant : AppleEvent classes
#include <UAppleEventsMgr.h>
#include <UExtractFromAEDesc.h>

	// MSL
#if Constructor_WindowsHost
	#include <strings.h>
#else
	#include <string.h>
#endif


// ===========================================================================

#pragma mark *** DMIntegerAttribute ***

// ---------------------------------------------------------------------------
//		* DMIntegerAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMIntegerAttribute::DMIntegerAttribute()
{
	mValue = 0;
	mValueSize = 32;
	mSigned = true;
	mCanBeText = false;
	mAlwaysText = false;
}


// ---------------------------------------------------------------------------
//		* DMIntegerAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMIntegerAttribute is:
//
//		[DMAttribute data]
//		Int32:			Initial/default value
//		Uint16:			Value size (in bits)
//		Boolean:		True if value is signed, false if unsigned
//		Boolean:		True if value can be interpreted as text (OSType)
//		Boolean:		True if value is always interpreted as text

DMIntegerAttribute::DMIntegerAttribute(
	LStream*	inStream)

: DMAttribute(inStream)

{
	
	// Read stream data.

	(*inStream)	>> mValue
				>> mValueSize
				>> mSigned
				>> mCanBeText
				>> mAlwaysText;

	// Sanity check: Value size must be in the range of 1 to 32 bits.

	SignalIf_(mValueSize < 1);
	SignalIf_(mValueSize > 32);

}


// ---------------------------------------------------------------------------
//		* DMIntegerAttribute(DMIntegerAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMIntegerAttribute::DMIntegerAttribute(
	const DMIntegerAttribute&	inOriginal)

: DMAttribute(inOriginal)

{
	
	// Validate pointers.

	ValidateObject_(&inOriginal);
	
	// Copy original object's values.

	mValue = inOriginal.mValue;
	mValueSize = inOriginal.mValueSize;
	mSigned = inOriginal.mSigned;
	mCanBeText = inOriginal.mCanBeText;
	mAlwaysText = inOriginal.mAlwaysText;

}


// ---------------------------------------------------------------------------
//		* ~DMIntegerAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMIntegerAttribute::~DMIntegerAttribute()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetAEValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute in an AppleEvent descriptor.

void
DMIntegerAttribute::GetAEValue(
	AEDesc&		outPropertyDesc) const
{

	// Validate pointers.

	ValidateThis_();

	// Create AppleEvent descriptor with value.

	::AECreateDesc(typeLongInteger, (Ptr) &mValue, sizeof (SInt32), &outPropertyDesc);

}


// ---------------------------------------------------------------------------
//		* SetAEValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from an AppleEvent descriptor.

void
DMIntegerAttribute::SetAEValue(
	const AEDesc&	inValue)
{

	// Validate pointers.

	ValidateThis_();
	
	// Dispatch descriptor type.

	switch (inValue.descriptorType) {

		case typeBoolean:
			Boolean tempBool;
			UExtractFromAEDesc::TheBoolean(inValue, tempBool);
			SetIntValue(tempBool);
			break;

		case typeChar:
			Str255 tempStr;
			strcpy( (char*)tempStr, "\004    ");
			UExtractFromAEDesc::ThePString(inValue, tempStr);
			UInt32 tempStrType;
			LString::PStrToFourCharCode(tempStr, tempStrType);
			if (mValueSize >= 8)
				tempStrType >>= 24;
			else if (mValueSize >= 16)
				tempStrType >>= 16;
			SetIntValue(tempStrType);
			break;

		case typeType:
			OSType tempType;
			UExtractFromAEDesc::TheType(inValue, tempType);
			if (mValueSize >= 8)
				tempType >>= 24;
			else if (mValueSize >= 16)
				tempType >>= 16;
			SetIntValue(tempType);
			break;

		default:
			SInt32 tempInt32;
			StAEDescriptor tempD;
			ThrowIfOSErr_(::AECoerceDesc(&inValue, typeLongInteger, &(tempD.mDesc)));
#if __PowerPlant__ >= 0x02112001
			// icw - PowerPlant 2.1.1 changed the name of this routine
			UExtractFromAEDesc::TheSInt32(tempD.mDesc, tempInt32);
#else
			UExtractFromAEDesc::TheInt32(tempD.mDesc, tempInt32);
#endif
			SetIntValue(tempInt32);
	}
}


// ---------------------------------------------------------------------------
//		* GetTextValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute as a text string.

void
DMIntegerAttribute::GetTextValue(
	LStr255&	outValue) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Determine if this value should be read as a series of characters.
	
	Boolean textValue = mAlwaysText;
	if (!textValue && mCanBeText)
		textValue = UTextID::IsLikelyTextValue(mValue, mValueSize);
	
	if (textValue) {
		if (mValue == 0) {
		
			// If value is zero and we must use text values,
			// return an empty string.
		
			outValue[(UInt8)0] = 0;

		}
		else {
	
			// Yes, this is a likely text value. Write a character stream.
			// How many characters we write depends on the value size.
		
			if (mValueSize <= 8) {
				outValue[(UInt8)0] = 1;
				outValue[(UInt8)1] = mValue;
			}
			else if (mValueSize <= 16) {
				outValue[(UInt8)0] = 2;
				outValue[(UInt8)1] = mValue >> 8;
				outValue[(UInt8)2] = mValue;
			}
			else {
				outValue[(UInt8)0] = 4;
				outValue[(UInt8)1] = mValue >> 24;
				outValue[(UInt8)2] = mValue >> 16;
				outValue[(UInt8)3] = mValue >> 8;
				outValue[(UInt8)4] = mValue;
			}
			
			// If it's an optional text value, bracket with quotes.
			
			if (mCanBeText && !mAlwaysText) {
				LStr255 tempStr;
				tempStr += '\'';
				tempStr += outValue;
				tempStr += '\'';
				outValue = tempStr;
			}
			
		}
	}
	else {
	
		// No, it's not a likely text value. Write a decimal number.
	
		outValue = mValue;

	}
}


// ---------------------------------------------------------------------------
//		* SetTextValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from a text string.

void
DMIntegerAttribute::SetTextValue(
	ConstStringPtr	inValue)
{

	// Validate pointers.

	ValidateThis_();
	
	// Read the value. Determine if it's to be read as
	// a text value or as a sequence of decimal digits.
	// This can be tricky if it's an optional text value.
	
	SInt32 value;
	LStr255 tempString(inValue);
	Boolean readAsText = mAlwaysText;
	
	if (mCanBeText && !mAlwaysText) {
		
		// If string is empty, treat it as text.
	
		if (tempString.Length() == 0)
			readAsText = true;
		else {
		
			// String is not empty, see if it has any non-digits in it.
		
			SInt16 position = 0;
			while (++position <= tempString.Length()) {
				if ((!UKeyFilters::IsNumberChar(tempString[(UInt8)position]))
				  && (tempString[(UInt8)position] != '-')) {
					readAsText = true;
					break;
				}
			}
		}
	}
	
	// Read the value.
	
	if (readAsText) {
	
		// Read as a string. Shortcut for empty string
		// which is read as a zero value.
		
		if (tempString.Length() == 0)
			value = 0;
		else {
			
			// Determine if we should strip single quotes (')
			// surrounding the value. Only do this if enough
			// characters would remain afterwards to form a
			// complete value.
			
			Boolean stripQuotes = (tempString[(UInt8)1] == '\'');
			if (stripQuotes && (tempString.Length() < 2))
				stripQuotes = false;
			
			// If required, strip the quotes.
			
			if (stripQuotes) {
				tempString.Remove(1, 1);
				if (tempString[tempString.Length()] == '\'')
					tempString.Remove(tempString.Length(), 1);
			}
			
			// Pad the string with spaces until it is at least 4 characters.
			
			tempString.Append( (StringPtr)"\004    ");
			
			// Truncate the string so that it is no longer than the value size.

			tempString[(UInt8)0] = (mValueSize + 7) / 8;
			
			// Read the remaining characters.
			
			value = 0;
			SInt32 index = 0;
			
			while (++index <= tempString.Length()) {
				value *= 256;
				value += tempString[(UInt8)index];
			}
		}
	}
	else {
	
		// Read as a decimal number.
	
		::StringToNum(inValue, &value);
	
	}
	
	// Set the value.
	
	SetIntValue(value);
	
}


// ---------------------------------------------------------------------------
//		* GetIntValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute as an integer.

SInt32
DMIntegerAttribute::GetIntValue() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return value.
	
	return mValue;

}


// ---------------------------------------------------------------------------
//		* SetIntValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from an integer. This method
//	is a bottleneck which must be called by all the Set___Value
//	methods.

void
DMIntegerAttribute::SetIntValue(
	SInt32	inValue)
{

	// Validate pointers.

	ValidateThis_();

	// If value is same as previous value, ignore it.
	
	if (inValue == mValue)
		return;
	
	// Do validity check on value.
	
	CheckIntValue(inValue);

	// If there is a transaction active, make sure it knows about the property change.

	StDMTransactionBuilder* builder = StDMTransactionBuilder::GetSubActionHost();
	if (builder != nil) {
		builder->PostSubAction(new DMAttributeSubAction(this));
		MakeModified();
	}
	
	// Let listeners know that value is about to change.

	SendPreValueChangeMessage();
	
	// Set the value.
	
	mValue = inValue;
	
	// Let listeners know that value has changed.
	
	SendValueChangeMessage();
	
}


// ---------------------------------------------------------------------------
//		* CheckIntValue
// ---------------------------------------------------------------------------
//	Called from SetIntValue to verify that a proposed new value for this
//	attribute is valid. Throws a DMRangeCheckException if value is out
//	of range as defined by the bitfield width. May be overriden if the
//	attribute has unusual bounds.

void
DMIntegerAttribute::CheckIntValue(
	SInt32	inValue)
{

	// Validate pointers.

	ValidateThis_();

	// Calculate minimum and maximum values for this field.
	
	SInt32 maxValue = GetMaxValue();
	SInt32 minValue = GetMinValue();

	// Test against these bounds. If it's invalid, we throw an exception which prevents
	// the value from being changed. If it's OK, we fall through.
	
	if ((inValue > maxValue) || (inValue < minValue))
		throw DMRangeCheckException(this, minValue, maxValue, inValue);

}


// ---------------------------------------------------------------------------
//		* GetMinValue
// ---------------------------------------------------------------------------
//	Return the minimum acceptable value for this attribute.

SInt32
DMIntegerAttribute::GetMinValue() const
{

	// Validate pointers.

	ValidateThis_();

	// If this is a 32-bit field, there's no range checking.
	
	if (mValueSize > 31)
		return min_Int32;

	// Otherwise, calculate minimum value for this field from
	// bit size and signed flag.

	if (mSigned)
		return (-GetMaxValue()) - 1;
	else
		return 0;

}


// ---------------------------------------------------------------------------
//		* GetMaxValue
// ---------------------------------------------------------------------------
//	Return the maximum acceptable value for this attribute.

SInt32
DMIntegerAttribute::GetMaxValue() const
{

	// Validate pointers.

	ValidateThis_();

	// If this is a 31- or 32-bit field, there's no range checking.
	
	if (mValueSize >= 31)
		return max_Int32;

	// Otherwise, calculate maximum value for this field from
	// bit size.
	
	SInt32 maxValue = 1 << mValueSize;
	if (mSigned)
		maxValue >>= 1;
	
	// Return an inclusive maximum value.
	
	return maxValue - 1;

}


// ---------------------------------------------------------------------------
//		* SetValueFromClone
// ---------------------------------------------------------------------------
//	Set the value of this attribute from another DMIntegerAttribute object.

void
DMIntegerAttribute::SetValueFromClone(
	const DMAttribute*	inValue)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inValue);
	
	// Cast the "clone" object to DMIntegerAttribute and copy its value.

	const DMIntegerAttribute* valueScalar = dynamic_const_cast( inValue, DMAttribute*, DMIntegerAttribute* );
	ValidateObject_(valueScalar);
	SetIntValue(valueScalar->GetIntValue());

}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf
// ---------------------------------------------------------------------------
//	Set the value of this attribute by reading the value from the stream.

void
DMIntegerAttribute::ReadStreamDataSelf(
	LStream*	inStream)
{

	// Validate pointers.

	ValidateThis_();

	// Figure out how many bytes to read for this value.

	SInt32 valueBytes = (mValueSize + 7) >> 3;
	SInt32 tempValue;
	
	switch (valueBytes) {
		case 1:
			UInt8 value8;
			*inStream >> value8;
			tempValue = value8;
			break;
		case 2:
			UInt16 value16;
			*inStream >> value16;
			tempValue = value16;
			break;
		case 3:
			*inStream >> value8;
			*inStream >> value16;
			tempValue = (value8 << 16) + value16;
			break;
		case 4:
			*inStream >> tempValue;
			break;
		default:
			SignalCStr_("Bad value size.");
	}

	// If value is negative, make sure it looks negative even when expanded to 32 bits.
	
	if (mSigned && (mValueSize < 32)) {
		SInt32 sizeComplement = 1 << (mValueSize - 1);
		if (tempValue >= sizeComplement) {
			while (tempValue >= 0) {
				tempValue -= sizeComplement;
			}
		}
	}

	// Set the value. SetIntValue is a bottleneck.
	
	SetIntValue(tempValue);

}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf
// ---------------------------------------------------------------------------
//	Write the value of this attribute to the specified stream.

void
DMIntegerAttribute::WriteStreamDataSelf(
	LStream*	inStream) const
{

	// Validate pointers.

	ValidateThis_();

	// Figure out how many bytes will be read for this value.

	SInt32 valueBytes = (mValueSize + 7) >> 3;
	switch (valueBytes) {
		case 1:
			UInt8 value8;
			value8 = mValue;
			*inStream << value8;
			break;
		case 2:
			UInt16 value16;
			value16 = mValue;
			*inStream << value16;
			break;
		case 3:
			value8 = mValue >> 16;
			value16 = mValue;
			*inStream << value8;
			*inStream << value16;
			break;
		case 4:
			*inStream << mValue;
			break;
		default:
			SignalCStr_("Bad value size.");
	}
}
