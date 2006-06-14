// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMRGBColorAttribute.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:33 $
//	$History: DMRGBColorAttribute.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/01/96   Time: 10:19
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added attribute accessor macros.
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
//		* DMRGBColorAttribute
// ===========================================================================
//
//	DMRGBColorAttribute is a concrete attribute class for all integer values.
//
// ===========================================================================

class DMRGBColorAttribute : public DMAttribute {

	DMClassID_				('rgbc', DMRGBColorAttribute, DMAttribute);

public:
							DMRGBColorAttribute();
							DMRGBColorAttribute(LStream* inStream);
							DMRGBColorAttribute(const DMRGBColorAttribute& inOriginal);
	virtual					~DMRGBColorAttribute();

	// value accessors
	
	virtual void			GetAEValue(
									AEDesc&				outPropertyDesc) const;
	virtual void			SetAEValue(
									const AEDesc&		inValue);

	virtual RGBColor		GetRGBColorValue() const;
	
	virtual void			SetRGBColorValue(
									const RGBColor&		inValue);
	
	virtual void			SetValueFromClone(
									const DMAttribute*	inValue);

	// raw data streaming

protected:
	virtual void			ReadStreamDataSelf(
									LStream*			inStream);
	virtual void			WriteStreamDataSelf(
									LStream*			inStream) const;

	// data members

protected:
	RGBColor				mValue;						// current value of attribute
	
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

#define DMRGBColorAttribute_(attrKey, attrName)								\
	DMAttribute_(attrKey, attrName, RGBColor)								\
	inline RGBColor	Get##attrName() const									\
		{ return Get##attrName##Attribute()->GetRGBColorValue(); }			\
	inline void		Set##attrName(const RGBColor& inValue) const			\
		{ Get##attrName##Attribute()->SetRGBColorValue(inValue); }
