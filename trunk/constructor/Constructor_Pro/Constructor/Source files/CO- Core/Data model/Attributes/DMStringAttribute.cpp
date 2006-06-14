// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMStringAttribute.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMStringAttribute.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added PreValueChanged method.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/05/97    Time: 5:51p
//	Updated in $/ConstructorWin/Sources
//	Fixes for compiling with MSVC
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:56
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Upgraded to handle word-wrapping appropriately. (Bug fix #1211.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/05/96   Time: 14:10
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added mAcceptCR flag and accessors.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:59
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 18:14
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Improved pointer validation.
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

#include "DMStringAttribute.h"

	// Core : Data model : Exceptions
#include "DMLengthCheckException.h"

	// Core : Data model : Undo/redo
#include "DMAttributeSubAction.h"
#include "StDMTransactionBuilder.h"


// ===========================================================================

#pragma mark *** DMStringAttribute ***

// ---------------------------------------------------------------------------
//		* DMStringAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMStringAttribute::DMStringAttribute()
{
	mValueSize = 255;
	mEvenPadded = mOddPadded = mAcceptCR = mIsCString = false;
}


// ---------------------------------------------------------------------------
//		* DMStringAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMStringAttribute is:
//
//		[DMAttribute data]
//		Pascal string:		Initial/default value
//		Uint16:				Value size (in characters, not including length byte)
//		Boolean:			True if string is padded to an even length in stream
//		Bitfield:				
//			$01					True if string is padded to an odd length in
//								  stream (Note: Padding DOES include length byte.)
//			$02					True if string is null terminated (C style
//								  rather than P style)
//			$04					True if string can accept carraige-return
//								  characters
//			$08					True if string should offer a hex entry option
//			$10					True if string is expressed as Unicode text

DMStringAttribute::DMStringAttribute(
	LStream*	inStream)

: DMAttribute(inStream)

{

	// Read stream data.

	inStream->ReadPString(mValue);
	*inStream >> mValueSize;
	*inStream >> mEvenPadded;
	
	SInt8 flags;
	*inStream >> flags;

	mOddPadded = (flags & 0x01) != 0;
	mIsCString = (flags & 0x02) != 0;
	mAcceptCR = (flags & 0x04) != 0;
	mNeedsHex  = (flags & 0x08) != 0;

	// Do validity checks on data.
	
	SignalIf_(mValueSize < 1);
	SignalIf_(mValueSize > 255);

}


// ---------------------------------------------------------------------------
//		* DMStringAttribute(DMStringAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMStringAttribute::DMStringAttribute(
	const DMStringAttribute&	inOriginal)

: DMAttribute(inOriginal)

{

	// Validate pointers.

	ValidateObject_(&inOriginal);
	
	// Copy values from original.
	
	mValue = inOriginal.mValue;
	mValueSize = inOriginal.mValueSize;
	mEvenPadded = inOriginal.mEvenPadded;
	mOddPadded = inOriginal.mOddPadded;
	mAcceptCR = inOriginal.mAcceptCR;
	mIsCString = inOriginal.mIsCString;
	mNeedsHex = inOriginal.mNeedsHex;
}


// ---------------------------------------------------------------------------
//		* ~DMStringAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMStringAttribute::~DMStringAttribute()
{
	ValidateThis_();
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetAEValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute in an AppleEvent descriptor.

void
DMStringAttribute::GetAEValue(
	AEDesc&		outPropertyDesc) const
{
	ValidateThis_();
	::AECreateDesc(typeChar, (Ptr) &mValue[(UInt8)1], mValue[(UInt8)0], &outPropertyDesc);
}


// ---------------------------------------------------------------------------
//		* SetAEValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from an AppleEvent descriptor.

void
DMStringAttribute::SetAEValue(
	const AEDesc&	inValue)
{
	ValidateThis_();

	LStr255 tempStr;
	StAEDescriptor temp;
	ThrowIfOSErr_(::AECoerceDesc(&inValue, typeChar, &(temp.mDesc)));
	UExtractFromAEDesc::ThePString(temp.mDesc, tempStr);
	SetTextValue(tempStr);
}


// ---------------------------------------------------------------------------
//		* GetTextValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute as a text string.

void
DMStringAttribute::GetTextValue(
	LStr255&	outValue) const
{
	ValidateThis_();
	outValue = mValue;
}


// ---------------------------------------------------------------------------
//		* SetTextValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute as a text string. This method
//	is a bottleneck which must be called by all the Set___Value
//	methods.

void
DMStringAttribute::SetTextValue(
	ConstStringPtr	inValue)
{

	ValidateThis_();

	// If value is same as previous value, ignore it.
	
	if (mValue.CompareTo(inValue) == 0)
		return;
	
	// Do validity check on value.
	
	CheckTextValue(inValue);

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
//		* CheckTextValue
// ---------------------------------------------------------------------------
//	Called from SetTextValue to verify that a proposed new value for this
//	attribute is valid. Throws a DMLengthCheckException if value is too
//	long as defined by the maximum character length. May be overriden if the
//	attribute has unusual validation requirements.

void
DMStringAttribute::CheckTextValue(
	ConstStringPtr	inValue)
{
	ValidateThis_();

	SInt32 attemptedLength = inValue[0];
	if (attemptedLength > mValueSize)
		throw DMLengthCheckException(this, mValueSize, attemptedLength);
}


// ---------------------------------------------------------------------------
//		* SetValueFromClone
// ---------------------------------------------------------------------------
//	Set the value of this attribute from another DMStringAttribute object.

void
DMStringAttribute::SetValueFromClone(
	const DMAttribute*	inValue)
{
	
	// Validate input parameters.
	
	ValidateThis_();
	ValidateObject_(inValue);

	// Make sure we're copying from a string.

	const DMStringAttribute* valueString = dynamic_const_cast(inValue,DMAttribute*,DMStringAttribute*);
	ValidateObject_(valueString);
	
	// It's cool. Copy the value.

	LStr255 tempStr;
	valueString->GetTextValue(tempStr);
	SetTextValue(tempStr);

}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf
// ---------------------------------------------------------------------------
//	Set the value of this attribute by reading the value from the stream.

void
DMStringAttribute::ReadStreamDataSelf(
	LStream*	inStream)
{

	ValidateThis_();

	// Read string.

	if (mIsCString)
	{
		mValue[(UInt8)0] = 0; 
		UInt8 byte;
		*inStream >> byte;
		while (byte != 0) 
		{
			mValue[++mValue[(UInt8)0]] = byte;
			*inStream >> byte;
		} 
	}
	else
		inStream->ReadPString(mValue);

	// Read padding byte (if applicable).

	UInt8 padByte;
	if (mEvenPadded) {
		if ((mValue.Length() & 1) == 0)
			*inStream >> padByte;
	}
	else if (mOddPadded) {
		if ((mValue.Length() & 1) == 1)
			*inStream >> padByte;
	}
}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf
// ---------------------------------------------------------------------------
//	Write the value of this attribute to the specified stream.

void
DMStringAttribute::WriteStreamDataSelf(
	LStream*	inStream) const
{

	ValidateThis_();

	// Write string data.

	if (mIsCString)
	{
		
		inStream->WriteBlock(&mValue[(UInt8)1],mValue[(UInt8)0]);
		UInt8 terminate = 0;
		*inStream << terminate;
	}
	else
		inStream->WritePString(mValue);

	// Write pad byte (if applicable).

	UInt8 padByte = 0;
	if (mEvenPadded) {
		if ((mValue.Length() & 1) == 0)
			*inStream << padByte;
	}
	else if (mOddPadded) {
		if ((mValue.Length() & 1) == 1)
			*inStream << padByte;
	}
}
