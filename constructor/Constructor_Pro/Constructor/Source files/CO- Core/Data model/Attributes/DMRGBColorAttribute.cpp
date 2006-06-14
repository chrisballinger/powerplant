// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMRGBColorAttribute.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMRGBColorAttribute.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added ValuesOnly() accessors.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:20
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Attributes'
//	Comment: Fixed CR/LF problem
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

#include "DMRGBColorAttribute.h"

	// Core : Data model : Exceptions
#include "DMRangeCheckException.h"

	// Core : Data model : Undo-redo
#include "DMAttributeSubAction.h"
#include "StDMTransactionBuilder.h"

	// PowerPlant : AppleEvent classes
#include <UAppleEventsMgr.h>
#include <UExtractFromAEDesc.h>

	// PowerPlant : File & stream classes
#include <LStream.h>


// ===========================================================================

#pragma mark *** DMRGBColorAttribute ***

// ---------------------------------------------------------------------------
//		* DMRGBColorAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMRGBColorAttribute::DMRGBColorAttribute()
{
	mValue.red = mValue.green = mValue.blue = 0;		// default to black
}


// ---------------------------------------------------------------------------
//		* DMRGBColorAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMRGBColorAttribute is:
//
//		[DMAttribute data]
//		RGBColor:			Initial/default value

DMRGBColorAttribute::DMRGBColorAttribute(
	LStream* inStream)

: DMAttribute(inStream)

{
	*inStream >> mValue.red;
	*inStream >> mValue.green;
	*inStream >> mValue.blue;
}


// ---------------------------------------------------------------------------
//		* DMRGBColorAttribute(DMRGBColorAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMRGBColorAttribute::DMRGBColorAttribute(
	const DMRGBColorAttribute& inOriginal)

: DMAttribute(inOriginal)

{
	mValue = inOriginal.mValue;
}


// ---------------------------------------------------------------------------
//		* ~DMRGBColorAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMRGBColorAttribute::~DMRGBColorAttribute()
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
DMRGBColorAttribute::GetAEValue(
	AEDesc& outPropertyDesc) const
{
	ValidateThis_();
	::AECreateDesc(typeRGBColor, (Ptr) &mValue, sizeof (RGBColor), &outPropertyDesc);
}


// ---------------------------------------------------------------------------
//		* SetAEValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from an AppleEvent descriptor.

void
DMRGBColorAttribute::SetAEValue(
	const AEDesc& inValue)
{

	ValidateThis_();

	// Extract the new color.

	RGBColor newColor;
	UExtractFromAEDesc::TheRGBColor(inValue, mValue);
	
	// Set the color.

	SetRGBColorValue(newColor);

}


// ---------------------------------------------------------------------------
//		* GetRGBColorValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute as an RGB color.

RGBColor
DMRGBColorAttribute::GetRGBColorValue() const
{
	ValidateThis_();
	return mValue;
}


// ---------------------------------------------------------------------------
//		* SetRGBColorValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from an RGB color. This method
//	is a bottleneck which must be called by all the Set___Value
//	methods.

void
DMRGBColorAttribute::SetRGBColorValue(
	const RGBColor& inValue)
{

	ValidateThis_();

	// If value is same as previous value, ignore it.
	
	if ((inValue.red == mValue.red) && (inValue.green == mValue.green) && (inValue.blue == mValue.blue))
		return;
	
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
//		* SetValueFromClone
// ---------------------------------------------------------------------------
//	Set the value of this attribute from another DMRGBColorAttribute object.

void
DMRGBColorAttribute::SetValueFromClone(
	const DMAttribute* inValue)
{
	ValidateThis_();

	const DMRGBColorAttribute* valueRGB =
				(dynamic_const_cast(inValue, DMAttribute*, DMRGBColorAttribute*));
	
	if (valueRGB != nil) {
		ValidateObject_(valueRGB);
		SetRGBColorValue(valueRGB->GetRGBColorValue());
	}
	else
		SignalPStr_("\pDMRGBColorAttribute::SetValueFromPeer - peer wasn't scalar");
}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf
// ---------------------------------------------------------------------------
//	Set the value of this attribute by reading the value from the stream.

void
DMRGBColorAttribute::ReadStreamDataSelf(
	LStream* inStream)
{
	
	ValidateThis_();

	// Read the new value.
	
	RGBColor newColor;
	*inStream >> newColor.red;
	*inStream >> newColor.green;
	*inStream >> newColor.blue;

	// Set the value. SetRGBColorValue is a bottleneck.
	
	SetRGBColorValue(newColor);

}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf
// ---------------------------------------------------------------------------
//	Write the value of this attribute to the specified stream.

void
DMRGBColorAttribute::WriteStreamDataSelf(
	LStream* inStream) const
{
	ValidateThis_();
	*inStream << mValue.red;
	*inStream << mValue.green;
	*inStream << mValue.blue;
}
