// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMAttribute.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:29 $
//	$History: DMAttribute.cpp $
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 03/11/97   Time: 10:56
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Removed CW Win32 template bug workaround. Got an update from Bob K that
//	fixes the problem.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 03/05/97   Time: 09:34
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added a workaround for CW Win32 compiler.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added PreValueChange method.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/07/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Removed ValidateThis_() in destructor. Possibly fixes a crashing bug
//	observed there.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/16/96   Time: 14:21
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Rolled in a bug fix from Clint to FindAttributeByTitle.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/16/96   Time: 12:46
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added code to ReadStreamData to ignore validation exceptions. (Bug fix
//	#1122.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/04/96   Time: 16:32
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added code in CanDeleteThis to allow attributes to be deleted when they
//	are contained in DMListAttribute objects.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:06
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Made CanDeleteThis const to avoid hiding DMElement's method.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/17/96   Time: 23:54
//	Checked in '$/Constructor/Source files/Data model/Attributes'
//	Comment: Removed Enabled flag. (It moved to DMElement.)
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 14:39
//	Checked in '$/Constructor/Source files/Data model/Attributes'
//	Comment: Added enabled flag to attributes.
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

#include "DMAttribute.h"

	// Core : Data model : Attributes
#include "DMListAttribute.h"

	// Core : Data model : Exceptions
#include "DMValidationException.h"


// ===========================================================================

#pragma mark *** DMAttribute ***

// ---------------------------------------------------------------------------
//		* DMAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMAttribute::DMAttribute()
{
	mKey = 0;
	mInDisplay = true;
	mInStream = true;
	mStreamSelector = 0;
}


// ---------------------------------------------------------------------------
//		* DMAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMAttribute is:
//
//		Pascal string:		Attribute title
//		OS Type:			Attribute key
//		Boolean:			Show attribute in property inspectors?
//		Boolean:			Write attribute to stream?
//		OS Type:			Stream selector code (0 for default)

DMAttribute::DMAttribute(
	LStream*	inStream)
{
	Str255 title;
	(*inStream)	>> title
				>> mKey
				>> mInDisplay
				>> mInStream
				>> mStreamSelector;

	SetTitle(title);
}


// ---------------------------------------------------------------------------
//		* DMAttribute(DMAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMAttribute::DMAttribute(
	const DMAttribute& inOriginal)

: DMElement(inOriginal)

{
	ValidateObject_(&inOriginal);
	mKey = inOriginal.mKey;
	mInDisplay = inOriginal.mInDisplay;
	mInStream = inOriginal.mInStream;
	mStreamSelector = inOriginal.mStreamSelector;
	mTitle = inOriginal.mTitle;
}


// ---------------------------------------------------------------------------
//		* ~DMAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMAttribute::~DMAttribute()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** field identifiers

// ---------------------------------------------------------------------------
//		* GetTitle
// ---------------------------------------------------------------------------
//	Return the title of the attribute.

void
DMAttribute::GetTitle(
	LStr255&	outTitle) const
{

	// Validate pointers.

	ValidateThis_();

	// Copy attribute title.

	if (mTitle != nil)
		outTitle = *((LString*) mTitle);		// cast is necessary, otherwise it takes it as a void*
	else
		outTitle[(UInt8)0] = 0;
}


// ---------------------------------------------------------------------------
//		* SetTitle
// ---------------------------------------------------------------------------
//	Change the title of the attribute. To save space, we use the shortest
//	possible string object.

void
DMAttribute::SetTitle(
	ConstStringPtr	inTitle)
{

	// Validate pointers.

	ValidateThis_();
	
	// Establish a string object for the string.

	SInt16 length = inTitle[0];
	if (length > 63)
		mTitle = new TSharableString<Str255>(inTitle);
	else if (length > 31)
		mTitle = new TSharableString<Str63>(inTitle);
	else if (length > 15)
		mTitle = new TSharableString<Str31>(inTitle);
	else if (length > 0)
		mTitle = new TSharableString<Str15>(inTitle);
	else
		mTitle = nil;
	
	if (length > 0)
		ValidateObject_(mTitle.GetObject());

}


// ---------------------------------------------------------------------------
//		* FindAttributeByKey
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its key. Checks to see if this attribute matches the desired key.

DMAttribute*
DMAttribute::FindAttributeByKey(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Check the attribute key.
	
	if ((inKey != 0) && (inKey == mKey))
		return (const_cast<DMAttribute*>(this));
	else
		return nil;

}


// ---------------------------------------------------------------------------
//		* FindAttributeByTitle
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its title. Checks to see if this attribute matches the desired title.

DMAttribute*
DMAttribute::FindAttributeByTitle(
	ConstStringPtr	inTitle) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Check the attribute title.
	
	if (mTitle != nil) {
		ValidateObject_(mTitle.GetObject());
		if (mTitle->CompareTo(inTitle) == 0)
			return (const_cast<DMAttribute*>(this));
	}

	// No match.

	return nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetAEValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute in an AppleEvent descriptor.
//	Should be overridden.

void
DMAttribute::GetAEValue(
	AEDesc& /* outPropertyDesc */) const
{
	SignalCStr_("DMAttribute::GetAEValue needs to be overridden");
}


// ---------------------------------------------------------------------------
//		* SetAEValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from an AppleEvent descriptor.
//	Should be overridden.

void
DMAttribute::SetAEValue(
	const AEDesc&	/* inValue */)
{
	SignalCStr_("DMAttribute::SetAEValue needs to be overridden");
}


// ---------------------------------------------------------------------------
//		* GetTextValue
// ---------------------------------------------------------------------------
//	Return the value of this attribute as a text string.
//	Should be overridden.

void
DMAttribute::GetTextValue(
	LStr255&	/* outValue */) const
{
	SignalCStr_("DMAttribute::GetTextValue needs to be overridden");
}


// ---------------------------------------------------------------------------
//		* SetTextValue
// ---------------------------------------------------------------------------
//	Set the value of this attribute from a text string.
//	Should be overridden.

void
DMAttribute::SetTextValue(
	ConstStringPtr	/* inValue */)
{
	SignalCStr_("DMAttribute::SetTextValue needs to be overridden");
}


// ---------------------------------------------------------------------------
//		* SetValueFromClone
// ---------------------------------------------------------------------------
//	Set the value of this attribute from an attribute object which was
//	created by calling Clone() on this attribute. Should be overridden.

void
DMAttribute::SetValueFromClone(
	const DMAttribute*	/* inValue */)
{
	SignalCStr_("DMAttribute::SetValueFromClone needs to be overridden");
}


// ---------------------------------------------------------------------------
//		* ReadStreamData
// ---------------------------------------------------------------------------
//	Set the value of this attribute by reading the value from the stream.
//	Should be overridden.

void
DMAttribute::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{

	// Validate pointers.

	ValidateThis_();

	// Check stream selector and include in stream flag.
	
	if ((inStreamSelector == mStreamSelector) && mInStream) {
		try {
			ReadStreamDataSelf(inStream);
		}
		catch(const DMValidationException&) {
			// swallow validation errors
		}
	}
}


// ---------------------------------------------------------------------------
//		* WriteStreamData
// ---------------------------------------------------------------------------
//	Write the value of this attribute to the specified stream.
//	Should be overridden.

void
DMAttribute::WriteStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector) const
{

	// Validate pointers.

	ValidateThis_();

	// Check stream selector and include in stream flag.
	
	if ((inStreamSelector == mStreamSelector) && mInStream)
		WriteStreamDataSelf(inStream);

}


// ===========================================================================

#pragma mark -
#pragma mark ** display information

// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Return the attribute's title as the display name.

void
DMAttribute::GetDisplayableName(
	LStr255&	outTitle) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return the attribute's title as the display name.
	
	GetTitle(outTitle);

}


// ===========================================================================

#pragma mark -
#pragma mark ** data model changes

// ---------------------------------------------------------------------------
//		* CanDeleteThis
// ---------------------------------------------------------------------------
//	Return true if it is permissible to delete this element.
//	In general, attributes are not deletable.

Boolean
DMAttribute::CanDeleteThis() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Attributes are generally not deletable unless the immediate
	// superelement is a dynamic list attribute.
	
	DMListAttribute* listAttr = (dynamic_cast<DMListAttribute*>(const_cast<DMAttribute*>(this)->mSuperElement));
	if (listAttr != nil)
		return IsEnabled();
	else
		return false;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendValueChangeMessage								[protected]
// ---------------------------------------------------------------------------
//	Send a message that indicates that the value of this attribute
//	has changed.

void
DMAttribute::SendValueChangeMessage()
{

	// Validate pointers.

	ValidateThis_();
	
	// Create a value change structure and broadcast it.

	DM_ValueChange msg = { this, GetAffectedObject() };
	SendUpdateMessage(Attribute_ValueChange, &msg, this);
	
}

// ---------------------------------------------------------------------------
//		* SendPreValueChangeMessage								[protected]
// ---------------------------------------------------------------------------
//	Send a message that indicates that the value of this attribute
//	is about to change.

void
DMAttribute::SendPreValueChangeMessage()
{

	// Validate pointers.

	ValidateThis_();
	
	// Create a value change structure and broadcast it.

	DM_PreValueChange msg = { this, GetAffectedObject() };
	SendUpdateMessage(Attribute_PreValueChange, &msg, this);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Override to read stream data for this data type.

void
DMAttribute::ReadStreamDataSelf(
	LStream*	/* inStream */)
{
	SignalCStr_("DMAttribute::ReadStreamDataSelf needs to be overridden");
}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Override to write stream data for this data type.

void
DMAttribute::WriteStreamDataSelf(
	LStream*	/* inStream */) const
{
	SignalCStr_("DMAttribute::WriteStreamDataSelf needs to be overridden");
}
