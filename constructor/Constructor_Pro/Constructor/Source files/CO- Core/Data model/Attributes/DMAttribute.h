// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMAttribute.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:30 $
//	$History: DMAttribute.h $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added PreValueChange method.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/07/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Removed ValidateThis_() in destructor. Possibly fixes a crashing bug
//	observed there.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/06/97    Time: 4:04p
//	Updated in $/ConstructorWin/Includes
//	Added include of GSharableString so MSVC can build templates
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:07
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Made CanDeleteThis const to avoid hiding DMElement's method.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/01/96   Time: 10:16
//	Updated in $/Constructor/Source files/Data model/Attributes
//	More work on enabled flag.
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

#pragma once

	// Core : Data model : Core objects
#include "DMElement.h"

	// Core : Utility classes
#include "GSharableString.h"


// ===========================================================================
//		* DMAttribute											[abstract]
// ===========================================================================
//
//	DMAttribute is an abstract base class for values or properties.
//	A DMAttribute is typically attached to a DMObject and represents
//	an attribute of that object. Attributes can be identified by a
//	four-byte ID ("key") or by title. DMAttribute contains common code
//	for finding, setting, and retriving the key or title. Since
//	attributes are commonly read from or written to a stream,
//	base methods are defined here for stream I/O.
//
//	NOTE: The value update message is defined at the end of this header file.
//
// ===========================================================================

class DMAttribute : public DMElement {

	// data model description

	DMClassID_				('attr', DMAttribute, DMElement);

	// constructor/destructor

protected:
							DMAttribute();
							DMAttribute(LStream* inStream);
							DMAttribute(const DMAttribute& inOriginal);
	virtual					~DMAttribute();

	// field identifiers

public:
	void					GetTitle(
									LStr255&			outTitle) const;
	void					SetTitle(
									ConstStringPtr		inTitle);
	
	inline FourCharCode		GetKey() const
									{ return mKey; }
	inline void				SetKey(FourCharCode inKey)
									{ mKey = inKey; }

	virtual DMAttribute*	FindAttributeByKey(
									DM_AttributeKey		inKey) const;
	virtual DMAttribute*	FindAttributeByTitle(
									ConstStringPtr		inTitle) const;

	// value accessors
	
	virtual void			GetAEValue(
									AEDesc&				outPropertyDesc) const;
	virtual void			SetAEValue(
									const AEDesc&		inValue);

	virtual void			GetTextValue(
									LStr255&			outValue) const;
	virtual void			SetTextValue(
									ConstStringPtr		inValue);
	
	virtual void			SetValueFromClone(
									const DMAttribute*	inValue);

	virtual void			ReadStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0);
	virtual void			WriteStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0) const;

	// streaming information
	
	inline Boolean			IncludeInStream() const
									{ return mInStream; }
	inline void				SetIncludeInStream(Boolean inIncludeInStream)
									{ mInStream = inIncludeInStream; }

	inline FourCharCode		GetStreamSelector() const
									{ return mStreamSelector; }
	inline void				SetStreamSelector(FourCharCode inSelector)
									{ mStreamSelector = inSelector; }
	
	// display information

	inline Boolean			IncludeInDisplay() const
									{ return mInDisplay; }
	inline void				SetIncludeInDisplay(Boolean inIncludeInDisplay)
									{ mInDisplay = inIncludeInDisplay; }
	virtual void			GetDisplayableName(
									LStr255&			outTitle) const;

	// data model changes

	virtual Boolean			CanDeleteThis() const;

	// update notifications

protected:
	virtual void			SendValueChangeMessage();
	virtual void			SendPreValueChangeMessage();


	// raw data streaming

	virtual void			ReadStreamDataSelf(
									LStream*			inStream);
	virtual void			WriteStreamDataSelf(
									LStream*			inStream) const;


	// data members

protected:
	GSharableStringPtr		mTitle;					// attribute title
	FourCharCode			mKey;					// attribute key

	Boolean					mInDisplay;				// include this attribute in property inspectors
	Boolean					mInStream;				// include this attribute in data streams
	FourCharCode			mStreamSelector;		// stream selector


	friend class DMAttributeSubAction;

};


// ===========================================================================
//		* Update messages
// ===========================================================================
//	An attribute (DMAttribute) is an LBroadcaster (via DMElement).
//	User interface objects which act on this attribute should make
//	themselves listeners to the attribute (or its parent object) so
//	they can be made aware of changes to the object.

// ---------------------------------------------------------------------------
//	* Attribute value message:
//		This message is sent just after an attribute changes its value.
//		Listeners can query the attribute object to get the new value when
//		this message is received.

const MessageT Attribute_ValueChange = 'val∆';
	// ioParam = DM_ValueChange*

struct DM_ValueChange {
	DMAttribute*	mAttribute;
	DMObject*		mObject;
};

// ---------------------------------------------------------------------------
//	* Attribute pre value message:
//		This message is sent just before an attribute changes its value.
//		Listeners can query the attribute object to get the old value.

const MessageT Attribute_PreValueChange = 'pre∆';
	// ioParam = DM_PreValueChange*

struct DM_PreValueChange {
	DMAttribute*	mAttribute;
	DMObject*		mObject;
};


// ===========================================================================
//		* Attribute accessor macros
// ===========================================================================
//
//	These macros should be used in the headers of DMObject subclases.
//	They provide accessors for each attribute.
//
//	For each attribute, you specify an attribute name and key.
//	The following symbols are then defined:
//
//		attrKey_<name>:				OSType attribute name
//		Get<name>Attribute:			Finds attribute, casts it to desired
//										types, does pointer validation.
//		Get<name>:					Returns current value of attribute
//		Set<name>:					Changes value of attribute (w/o transaction)
//	
// ===========================================================================

#define DMAttribute_(attrKey, attrName, attrType)							\
	enum { attrKey_##attrName = attrKey };									\
	inline DM##attrType##Attribute*		Get##attrName##Attribute() const	\
		{ return Find##attrType##Attribute(attrKey); }
