// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMNameOrOrdinalAttribute.cpp   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/21/97
//     $Date: 2006/04/12 08:47:44 $
//  $History: DMNameOrOrdinalAttribute.cpp $
//	
//	*****************  Version 2  *****************
//	User: scouten      Date: 3/06/97    Time: 11:07a
//	Updated in $/ConstructorWin/Windows/Source
//	Fixed a few silly bugs.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 03/06/97   Time: 10:58
//	Created in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added class.
//	
// ===========================================================================

#ifdef WINVER
	#include "ctor.h"
#endif

// ===========================================================================

#include "DMNameOrOrdinalAttribute.h"

	// Core : Data model : Exceptions
#include "DMRangeCheckException.h"

	// Core : Data model : Undo-redo
#include "DMAttributeSubAction.h"
#include "StDMTransactionBuilder.h"


// ===========================================================================

#pragma mark *** DMNameOrOrdinalAttribute ***

// ---------------------------------------------------------------------------
//		* DMNameOrOrdinalAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMNameOrOrdinalAttribute::DMNameOrOrdinalAttribute()
{
	mIsNumeric = false;
	mNumericValue = 0;
	mNumericValueSize = 32;
	mSigned = true;
}


// ---------------------------------------------------------------------------
//		* DMNameOrOrdinalAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[DMStringAttribute data]
//		Int32:			Default numeric value
//		Uint16:			Numeric value size (in bits)
//		Boolean:		True if numeric value can be signed
//		Boolean:		True if default value is numeric

DMNameOrOrdinalAttribute::DMNameOrOrdinalAttribute(
	LStream*	inStream)

: DMStringAttribute(inStream)

{

	// Read stream data.

	(*inStream)	>> mNumericValue
				>> mNumericValueSize
				>> mSigned
				>> mIsNumeric;

	// Sanity check: Value size must be in the range of 1 to 32 bits.

	SignalIf_(mNumericValueSize < 1);
	SignalIf_(mNumericValueSize > 32);

}


// ---------------------------------------------------------------------------
//		* DMNameOrOrdinalAttribute(DMNameOrOrdinalAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMNameOrOrdinalAttribute::DMNameOrOrdinalAttribute(
	const DMNameOrOrdinalAttribute&	inOriginal)

: DMStringAttribute(inOriginal)

{
	mIsNumeric = inOriginal.mIsNumeric;
	mNumericValue = inOriginal.mNumericValue;
	mNumericValueSize = inOriginal.mNumericValueSize;
	mSigned = inOriginal.mSigned;
}


// ---------------------------------------------------------------------------
//		* ~DMNameOrOrdinalAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMNameOrOrdinalAttribute::~DMNameOrOrdinalAttribute()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetIntValue
// ---------------------------------------------------------------------------
//	Return the numeric value of the attribute. Only valid of GetIsNumeric()
//	returns true.

Int32
DMNameOrOrdinalAttribute::GetIntValue() const
{
	return mNumericValue;
}


// ---------------------------------------------------------------------------
//		* SetIntValue
// ---------------------------------------------------------------------------
//	Set the attribute to a given numeric value.

void
DMNameOrOrdinalAttribute::SetIntValue(
	Int32	inValue)
{

	// Validate pointers.

	ValidateThis_();

	// If value is same as previous value, ignore it.

	if ((inValue == mNumericValue) && mIsNumeric)
		return;
	
	// Do validity check on value.
	
	CheckIntValue(inValue);

	// If there is a transaction active, make sure it knows about the property change.

	StDMTransactionBuilder* builder = StDMTransactionBuilder::GetSubActionHost();
	if (builder != nil) {
		builder->PostSubAction(new DMAttributeSubAction(this));
		MakeModified();
	}
	
	// Set the value.
	
	mNumericValue = inValue;
	mIsNumeric = true;
	
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
DMNameOrOrdinalAttribute::CheckIntValue(
	Int32	inValue)
{

	// Validate pointers.

	ValidateThis_();

	// Calculate minimum and maximum values for this field.
	
	Int32 maxValue = GetMaxValue();
	Int32 minValue = GetMinValue();

	// Test against these bounds. If it's invalid, we throw an exception which prevents
	// the value from being changed. If it's OK, we fall through.
	
	if ((inValue > maxValue) || (inValue < minValue))
		throw DMRangeCheckException(this, minValue, maxValue, inValue);

}


// ---------------------------------------------------------------------------
//		* GetMinValue
// ---------------------------------------------------------------------------
//	Return the minimum acceptable numeric value for this attribute.

Int32
DMNameOrOrdinalAttribute::GetMinValue() const
{

	// Validate pointers.

	ValidateThis_();

	// If this is a 32-bit field, there's no range checking.
	
	if (mNumericValueSize > 31)
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
//	Return the maximum acceptable numeric value for this attribute.

Int32
DMNameOrOrdinalAttribute::GetMaxValue() const
{

	// Validate pointers.

	ValidateThis_();

	// If this is a 31- or 32-bit field, there's no range checking.
	
	if (mNumericValueSize >= 31)
		return max_Int32;

	// Otherwise, calculate maximum value for this field from
	// bit size.
	
	Int32 maxValue = 1 << mNumericValueSize;
	if (mSigned)
		maxValue >>= 1;
	
	// Return an inclusive maximum value.
	
	return maxValue - 1;

}


// ---------------------------------------------------------------------------
//		* SetValueFromClone
// ---------------------------------------------------------------------------
//	Set the value of this attribute from another
//	DMNameOrOrdinalAttribute object.

void
DMNameOrOrdinalAttribute::SetValueFromClone(
	const DMAttribute*	inValue)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inValue);
	
	// Cast the "clone" object to DMNameOrOrdinalAttribute and copy its value.

	const DMNameOrOrdinalAttribute* valueScalar =
			dynamic_const_cast(inValue, DMAttribute*, DMNameOrOrdinalAttribute*);
	ValidateObject_(valueScalar);
	
	if (valueScalar->GetIsNumeric())
		SetIntValue(valueScalar->GetIntValue());
	else
		DMStringAttribute::SetValueFromClone(inValue);

}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Set the value of this attribute by reading the value from the stream.

void
DMNameOrOrdinalAttribute::ReadStreamDataSelf(
	LStream*	inStream)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// See if the value is numeric.
	
	Uint16 uniChar;
	(*inStream) >> uniChar;
	
	if (uniChar == 0xFFFF) {
	
		// It is numeric. The next word is the value.
	
		(*inStream) >> uniChar;
		mIsNumeric = true;

	}
	else {
	
		// It isn't numeric. Back up and read this character as
		// a Unicode string.
	
		inStream->SetMarker(-2, streamFrom_Marker);
		DMStringAttribute::ReadStreamDataSelf (inStream);

	}
}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf
// ---------------------------------------------------------------------------
//	Write the value of this attribute to the specified stream.

void
DMNameOrOrdinalAttribute::WriteStreamDataSelf(
	LStream*	inStream) const
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// See if the value is numeric.
	
	if (GetIsNumeric()) {
	
		// It is numeric. Write an invalid Unicode character, then the value.
		
		Uint16 flag = 0xFFFF;
		Uint16 value = mNumericValue;
		
		(*inStream)	>> flag
					>> value;
	
	}
	else {
	
		// Not numeric. Write the string value.
		
		DMStringAttribute::WriteStreamDataSelf(inStream);
	
	}
}
