// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMStringAttribute.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/28/96
//	   $Date: 2006/01/18 01:32:33 $
//	$History: DMStringAttribute.h $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added hex option behavior.
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
//	User: scouten      QDate: 12/05/96   Time: 14:09
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added mAcceptCR flag and accessors.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:59
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Fixed a bug in SetEvenPadded. It was setting the odd-padded flag.
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 18:15
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Fixed access control for protected members (was public).
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMAttribute.h"


// ===========================================================================
//		* DMStringAttribute
// ===========================================================================
//
//	DMStringAttribute is a concete attribute class for all
//	string (text) values.
//
// ===========================================================================

class DMStringAttribute : public DMAttribute {

	// data model description

	DMClassID_				('pstr', DMStringAttribute, DMAttribute);

	// constructors/destructor

public:
							DMStringAttribute();
							DMStringAttribute(LStream* inStream);
							DMStringAttribute(const DMStringAttribute& inOriginal);
	virtual					~DMStringAttribute();

	// value accessors
	
	virtual void			GetAEValue(
									AEDesc&				outPropertyDesc) const;
	virtual void			SetAEValue(
									const AEDesc&		inValue);

	virtual void			GetTextValue(
									LStr255&			outValue) const;
	virtual void			SetTextValue(
									ConstStringPtr		inValue);
	virtual void			CheckTextValue(
									ConstStringPtr		inValue);

	virtual void			SetValueFromClone(
									const DMAttribute*	inValue);
	
	// display information
	
	inline UInt16			GetValueSize() const
									{ return mValueSize; }
	inline void				SetValueSize(UInt16 inValueSize)
									{ mValueSize = inValueSize; }

	inline Boolean			GetEventPadded() const
									{ return mEvenPadded; }
	inline void				SetEvenPadded(Boolean inEvenPadded)
									{ mEvenPadded = inEvenPadded; }

	inline Boolean			GetOddPadded() const
									{ return mOddPadded; }
	inline void				SetOddPadded(Boolean inOddPadded)
									{ mOddPadded = inOddPadded; }
	
	inline Boolean			GetAcceptCR() const
									{ return mAcceptCR; }
	inline void				SetAcceptCR(Boolean inAcceptCR)
									{ mAcceptCR = inAcceptCR; }
	
	inline Boolean			GetNeedsHex() const
									{ return mNeedsHex; }
	inline void				SetNeedsHex(Boolean inNeedsHex)
									{ mNeedsHex = inNeedsHex; }
	// raw data streaming

protected:
	virtual void			ReadStreamDataSelf(
									LStream*			inStream);
	virtual void			WriteStreamDataSelf(
									LStream*			inStream) const;


	// data members

protected:
	LStr255					mValue;					// current value of attribute
	UInt16					mValueSize;				// maximum size of value (in bytes)
	Boolean					mEvenPadded;			// true if value is even-length padded
	Boolean					mOddPadded;				// true if value is odd-length padded
	Boolean					mIsCString;				// true if this is a null-terminated string
	Boolean					mNeedsHex;				// true if this string should allow hex entry
	Boolean					mAcceptCR;				// true if carraige return can be accepted
	
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

#define DMStringAttribute_(attrKey, attrName)								\
	DMAttribute_(attrKey, attrName, String)									\
	inline void		Get##attrName(LStr255& outValue) const					\
		{ Get##attrName##Attribute()->GetTextValue(outValue); }				\
	inline void		Set##attrName(ConstStringPtr inValue) const				\
		{ Get##attrName##Attribute()->SetTextValue(inValue); }
