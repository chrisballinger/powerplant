// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMBitfieldAttribute.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMBitfieldAttribute.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 4   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:00
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:19
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Attributes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/16/96   Time: 23:08
//	Checked in '$/Constructor/Source files/Data model/Attributes'
//	Comment: Fixed validation bug in SendUpdateMessage.
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

#include "DMBitfieldAttribute.h"
	
	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// PowerPlant : File & stream classes
#include <LStream.h>


// ===========================================================================

#pragma mark *** DMBitfieldAttribute ***

// ---------------------------------------------------------------------------
//		* DMBitfieldAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMBitfieldAttribute::DMBitfieldAttribute()
{
	mValueSize = 4;
}


// ---------------------------------------------------------------------------
//		* DMBitfieldAttribute(DMBitfieldAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMBitfieldAttribute::DMBitfieldAttribute(
	const DMBitfieldAttribute& inOriginal)

: DMContainerAttribute(inOriginal)

{
	ValidateObject_(&inOriginal);
	mValueSize = inOriginal.mValueSize;
}


// ---------------------------------------------------------------------------
//		* DMBitfieldAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMBitfieldAttribute is:
//
//		[DMContainerAttribute data]
//		Uint16:				Number of bytes occupied by this bitfield

DMBitfieldAttribute::DMBitfieldAttribute(
	LStream* inStream)

: DMContainerAttribute(inStream)

{
	ValidateThis_();
	*inStream >> mValueSize;
	
	SignalIf_(mValueSize < 1);
	SignalIf_(mValueSize > 4);
}


// ---------------------------------------------------------------------------
//		* ~DMBitfieldAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMBitfieldAttribute::~DMBitfieldAttribute()
{
	ValidateThis_();
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetIntValue
// ---------------------------------------------------------------------------
//	Return the value of this field as it will be stored in the stream.

SInt32
DMBitfieldAttribute::GetIntValue() const
{

	ValidateThis_();

	// Collect values from each member attribute.

	SInt32 outValue = 0;
	
	DMFastIterator iter(mSubElements);
	while (iter.NextElement()) {

		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);

		DMIntegerAttribute* objIA = (dynamic_cast<DMIntegerAttribute*>(objDM));
		ValidateObject_(objIA);

		UInt32 mask = (1 << objIA->GetValueSize()) - 1;
		UInt32 orValue = objIA->GetIntValue() & mask;
		outValue <<= objIA->GetValueSize();
		outValue |= orValue;
	}
	
	return outValue;

}


// ---------------------------------------------------------------------------
//		* SetIntValue
// ---------------------------------------------------------------------------
//	Set the value of all subattributes by unpacking a bitfield.

void
DMBitfieldAttribute::SetIntValue(
	SInt32 inValue)
{

	ValidateThis_();

	// Parcel this out among the contained integer fields.
	
	DMFastIterator iter(mSubElements, LArrayIterator::from_End);
	while (iter.PreviousElement()) {

		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		
		DMIntegerAttribute* objIA = (dynamic_cast<DMIntegerAttribute*>(objDM));
		ValidateObject_(objIA);

		// Figure out how many bits we're supposed to read.

		UInt16 valueSize = objIA->GetValueSize();
		UInt32 mask = (1 << valueSize) - 1;
		
		// Decode the value from the bitstream. If value is signed,
		// convert sign bit to a real (32-bit) negative number.

		SInt32 value = inValue & mask;
		if (objIA->GetSigned() && (value > (1 << (valueSize-1))))
			value -= mask+1;
		
		// Set the value.
		
		objIA->SetIntValue(value);

		// Shift bits over and read the next value.
		
		inValue >>= objIA->GetValueSize();

	}
}


// ---------------------------------------------------------------------------
//		* ReadStreamData
// ---------------------------------------------------------------------------
//	Overriden to bypass the typical container attribute stream behavior.

void
DMBitfieldAttribute::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{
	ValidateThis_();
	DMAttribute::ReadStreamData(inStream, inStreamSelector);
		// bypass DMContainerAttribute
}


// ---------------------------------------------------------------------------
//		* WriteStreamData
// ---------------------------------------------------------------------------
//	Overriden to bypass the typical container attribute stream behavior.

void
DMBitfieldAttribute::WriteStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector) const
{
	ValidateThis_();
	DMAttribute::WriteStreamData(inStream, inStreamSelector);
		// bypass DMContainerAttribute
}


// ===========================================================================

#pragma mark -
#pragma mark ** update requests

// ---------------------------------------------------------------------------
//		* CanHostElementSelf									[protected]
// ---------------------------------------------------------------------------
//	A bitfield attribute can only accept integer attributes as members.

Boolean
DMBitfieldAttribute::CanHostElementSelf(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{
	ValidateThis_();
	ValidateObject_(inNewElement);

	const DMIntegerAttribute* intAttr = dynamic_const_cast(inNewElement,DMElement*,DMIntegerAttribute*);
	if (intAttr != nil) {
		ValidateObject_(intAttr);
		ioPlacement->mHostElement = this;
		return true;
	}
	else
		return false;
}


// ---------------------------------------------------------------------------
//		* HostElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Accept integer attribute and apply it to this value.

void
DMBitfieldAttribute::HostElementSelf(
	DMElement*				inNewElement,
	const DM_DragPlacement*	inPlacement)
{
	ValidateThis_();
	ValidateObject_(inNewElement);

	const DMIntegerAttribute* intAttr = (dynamic_cast<const DMIntegerAttribute*>(inNewElement));
	if (intAttr != nil) {
		ValidateObject_(intAttr);
		SetIntValue(intAttr->GetIntValue());
	}
	else
		DMContainerAttribute::HostElementSelf(inNewElement, inPlacement);
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notification

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Overriden to indicate a value change for the bitfield as a whole.

void
DMBitfieldAttribute::SendUpdateMessage(
	MessageT		inMessage,
	void*			ioParam,
	DMElement*		inSubElement)
{

	ValidateThis_();
	
	if (inSubElement != nil)
		ValidateObject_(inSubElement);

	// Send update message as usual.

	DMElement::SendUpdateMessage(inMessage, ioParam, inSubElement);

	// If a subitem is saying it's changed, say that we've changed also.

	if (inMessage == Attribute_ValueChange) {
		
		DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
		if (valueMsg->mAttribute != this)
			SendValueChangeMessage();
		
	}

}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read data from the stream and decompress it into individual values.

void
DMBitfieldAttribute::ReadStreamDataSelf(
	LStream* inStream)
{

	ValidateThis_();

	// Figure out how many bytes to read for this value.

	UInt32 tempValue;
	switch (mValueSize) {
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

	// Parcel this out among the contained integer fields.
	
	SetIntValue(tempValue);

}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Pack the data from the individual attributes and compress it into the
//	bitfield format.

void
DMBitfieldAttribute::WriteStreamDataSelf(
	LStream* inStream) const
{

	ValidateThis_();

	// Collect values from each member attribute.

	SInt32 outValue = GetIntValue();
	
	// Figure out how many bytes will be read for this value.

	switch (mValueSize) {
		case 1:
			UInt8 value8;
			value8 = outValue;
			*inStream << value8;
			break;
		case 2:
			UInt16 value16;
			value16 = outValue;
			*inStream << value16;
			break;
		case 3:
			value8 = outValue >> 16;
			value16 = outValue;
			*inStream << value8;
			*inStream << value16;
			break;
		case 4:
			*inStream << outValue;
			break;
		default:
			SignalCStr_("Bad value size.");
	}
}
