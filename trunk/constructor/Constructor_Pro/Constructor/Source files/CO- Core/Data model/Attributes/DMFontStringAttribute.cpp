// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMFontStringAttribute.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:32:31 $
//	$History: DMFontStringAttribute.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added PreValueChange method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 10/31/96   Time: 18:12
//	Created in $/Constructor/Source files/Data model/Attributes
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMFontStringAttribute.h"

	// Core : Data model : Undo-redo
#include "DMAttributeSubAction.h"
#include "StDMTransactionBuilder.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : Utility classes
#include <UTextTraits.h>


// ===========================================================================

#pragma mark *** DMFontStringAttribute ***

// ---------------------------------------------------------------------------
//		* DMFontStringAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMFontStringAttribute::DMFontStringAttribute()
{
	mFontNumber = UTextTraits::fontNumber_Unknown;
	mIncludeSystemFont = true;
	mIncludeApplicationFont = true;
	mIncludeHiddenFonts = false;
	mWriteFontNumber = false;
}


// ---------------------------------------------------------------------------
//		* DMFontStringAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[DMStringAttribute data]
//		Int16:			Default font number
//		Boolean:		True to write font number before string
//		Boolean:		True to include "System font" in popup menu
//		Boolean:		True to include "Application font" in popup menu
//		Boolean:		True to include hidden fonts (% and . prefix)

DMFontStringAttribute::DMFontStringAttribute(
	LStream* inStream)

: DMStringAttribute(inStream)

{
	(*inStream)	>> mFontNumber
				>> mWriteFontNumber
				>> mIncludeSystemFont
				>> mIncludeApplicationFont
				>> mIncludeHiddenFonts;

	mFontNumber = UTextTraits::fontNumber_Unknown;
}


// ---------------------------------------------------------------------------
//		* DMFontStringAttribute(DMFontStringAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMFontStringAttribute::DMFontStringAttribute(
	const DMFontStringAttribute& inOriginal)

: DMStringAttribute(inOriginal)

{
	mFontNumber = inOriginal.mFontNumber;
	mWriteFontNumber = inOriginal.mWriteFontNumber;
	mIncludeSystemFont = inOriginal.mIncludeSystemFont;
	mIncludeApplicationFont = inOriginal.mIncludeApplicationFont;
	mIncludeHiddenFonts = inOriginal.mIncludeHiddenFonts;
}


// ---------------------------------------------------------------------------
//		* ~DMFontStringAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMFontStringAttribute::~DMFontStringAttribute()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetTextValue
// ---------------------------------------------------------------------------
//	Overriden to return the "System font" or "Application font" string
//	if the font number is used, or to look up the font's name if possible.

void
DMFontStringAttribute::GetTextValue(
	LStr255&	outValue) const
{

	// Validate input parameters.

	ValidateThis_();
	
	// If font number isn't being used, just use the string value.
	
	switch (mFontNumber) {
	
		case UTextTraits::fontNumber_Unknown:
			DMStringAttribute::GetTextValue(outValue);
			break;
		
		case systemFont:					// "System font"
		case applFont: {					// "Application font"
			StApplicationContext appContext;
			::GetIndString(outValue, STR_FontStringAttribute,
							mFontNumber == 0 ? str_SystemFont : str_ApplicationFont);
			break;
		}
		
		default:						// get font's name
			::GetFontName(mFontNumber, outValue);
			break;
	
	}
}


// ---------------------------------------------------------------------------
//		* SetTextValue
// ---------------------------------------------------------------------------
//	Match "System font" and "Application font" strings if possible.

void
DMFontStringAttribute::SetTextValue(
	ConstStringPtr		inValue)
{

	// Validate input parameters.

	ValidateThis_();
	
	// Make sure font name doesn't match "System font" or "Application font".
	
	{
		StApplicationContext appContext;
		LStr255 sysFontStr(STR_FontStringAttribute, str_SystemFont);
		LStr255 appFontStr(STR_FontStringAttribute, str_ApplicationFont);
		
		if (sysFontStr.CompareTo(inValue) == 0) {
			SetFontNumber(systemFont);
			return;
		}
		
		if (appFontStr.CompareTo(inValue) == 0) {
			SetFontNumber(applFont);
			return;
		}
	}
	
	// Font name is something else, record it as a text value.

	DMStringAttribute::SetTextValue(inValue);
	mFontNumber = UTextTraits::fontNumber_Unknown;

}


// ---------------------------------------------------------------------------
//		* SetFontNumber
// ---------------------------------------------------------------------------
//	Set the font name by referencing the font number.

void
DMFontStringAttribute::SetFontNumber(
	SInt16	inFontNumber)
{
	
	ValidateThis_();
	
	// If value is same as previous value, ignore it.
	
	if (mFontNumber == inFontNumber)
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
	
	mFontNumber = inFontNumber;
	mValue[(UInt8)0] = 0;
	
	// Let listeners know that value has changed.
	
	SendValueChangeMessage();
	
}


// ---------------------------------------------------------------------------
//		* SetValueFromClone
// ---------------------------------------------------------------------------
//	Copy value from another font string attribute.

void
DMFontStringAttribute::SetValueFromClone(
	const DMAttribute*	inValue)
{

	// Validate input parameters.

	ValidateThis_();
	ValidateObject_(inValue);

	// Make sure we're copying from a string.

	const DMFontStringAttribute* valueFont = dynamic_const_cast(inValue,DMAttribute*,DMFontStringAttribute*);
	ValidateObject_(valueFont);
	
	// It's cool. Copy the value.
	// If it was storing the font number, use that... otherwise copy the string.
	
	SInt16 fontNum = valueFont->GetFontNumber();
	
	if (fontNum == UTextTraits::fontNumber_Unknown)
		DMStringAttribute::SetValueFromClone (inValue);
	else
		SetFontNumber(fontNum);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read font number and name from data stream.

void
DMFontStringAttribute::ReadStreamDataSelf(
	LStream*	inStream)
{
	
	ValidateThis_();

	// Read font number (if so requested).

	if (mWriteFontNumber)
		*inStream >> mFontNumber;
	else
		mFontNumber = UTextTraits::fontNumber_Unknown;

	// Now read the font string.

	DMStringAttribute::ReadStreamDataSelf(inStream);

}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read font number and name from data stream.

void
DMFontStringAttribute::WriteStreamDataSelf(
	LStream*	inStream) const
{

	ValidateThis_();
	
	// Write font number (if so requested).
	
	if (mWriteFontNumber) {
	
		*inStream << mFontNumber;
		
		// Also make sure font name is blocked out if we're using
		// the font number. It should be blocked out anyway...
		
		if (mFontNumber != UTextTraits::fontNumber_Unknown)
			mValue == (StringPtr)"\001";
	
	}

	// Now write the font name.

	DMStringAttribute::WriteStreamDataSelf(inStream);

}
