// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMIntegerAttribute.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:31 $
//	$History: DMIntegerAttribute.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/07/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added #include <string.h> so strcpy would work.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/05/96   Time: 12:42
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added GetMinValue and GetMaxValue methods.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/22/96   Time: 14:52
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Fixed a bug in SetTextValue which caused it to read negative numbers as
//	text values in optional text fields.
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
//		* DMIntegerAttribute
// ===========================================================================
//
//	DMIntegerAttribute is a concrete attribute class for all integer values.
//
// ===========================================================================

class DMIntegerAttribute : public DMAttribute {

	DMClassID_				('intv', DMIntegerAttribute, DMAttribute);

public:
							DMIntegerAttribute();
							DMIntegerAttribute(LStream* inStream);
							DMIntegerAttribute(const DMIntegerAttribute& inOriginal);
	virtual					~DMIntegerAttribute();

	// value accessors
	
	virtual void			GetAEValue(
									AEDesc&				outPropertyDesc) const;
	virtual void			SetAEValue(
									const AEDesc&		inValue);

	virtual void			GetTextValue(
									LStr255&			outValue) const;
	virtual void			SetTextValue(
									ConstStringPtr		inValue);

	virtual SInt32			GetIntValue() const;
	virtual void			SetIntValue(
									SInt32				inValue);
	virtual void			CheckIntValue(
									SInt32				inValue);

	virtual SInt32			GetMinValue() const;
	virtual SInt32			GetMaxValue() const;

	virtual void			SetValueFromClone(
									const DMAttribute*	inValue);
	
	// display information
	
	inline UInt16			GetValueSize() const
									{ return mValueSize; }
	inline void				SetValueSize(UInt16 inValueSize)
									{ mValueSize = inValueSize; }

	inline Boolean			GetSigned() const
									{ return mSigned; }
	inline void				SetSigned(Boolean inSigned)
									{ mSigned = inSigned; }

	inline Boolean			GetCanBeText() const
									{ return mCanBeText; }
	inline void				SetCanBeText(Boolean inCanBeText)
									{ mCanBeText = inCanBeText; }

	inline Boolean			GetAlwaysText() const
									{ return mAlwaysText; }
	inline void				SetAlwaysText(Boolean inAlwaysText)
									{ mAlwaysText = inAlwaysText; }

	// raw data streaming

protected:
	virtual void			ReadStreamDataSelf(
									LStream*		inStream);
	virtual void			WriteStreamDataSelf(
									LStream*		inStream) const;


	// data members

protected:
	SInt32					mValue;					// current value of attribute
	UInt16					mValueSize;				// maximum size of value (in bits)
	Boolean					mSigned;				// true if value is signed
	Boolean					mCanBeText;				// true if value can be interpreted as characters (OSType)
	Boolean					mAlwaysText;			// true if value is always interpreted as characters
	
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

#define DMIntegerAttribute_(attrKey, attrName)								\
	DMAttribute_(attrKey, attrName, Integer)								\
	inline SInt32	Get##attrName() const									\
		{ return Get##attrName##Attribute()->GetIntValue(); }				\
	inline void		Set##attrName(SInt32 inValue) const						\
		{ Get##attrName##Attribute()->SetIntValue(inValue); }

