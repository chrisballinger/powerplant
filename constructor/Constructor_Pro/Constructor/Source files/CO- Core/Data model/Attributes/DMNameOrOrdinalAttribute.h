// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMNameOrOrdinalAttribute.h	   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/21/97
//     $Date: 2006/01/18 01:32:32 $
//  $History: DMNameOrOrdinalAttribute.h $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 03/06/97   Time: 10:58
//	Created in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMStringAttribute.h"


// ===========================================================================
//		* DMNameOrOrdinalAttribute
// ===========================================================================
//
//	An attribute that represents the "name or ordinal" data member of
//	the Win32 resource format.
//
// ===========================================================================

class DMNameOrOrdinalAttribute : public DMStringAttribute {

	// data model description

	DMClassID_				('nord', DMNameOrOrdinalAttribute, DMStringAttribute);

	// constructors/destructor

public:
							DMNameOrOrdinalAttribute();
							DMNameOrOrdinalAttribute(LStream* inStream);
							DMNameOrOrdinalAttribute(const DMNameOrOrdinalAttribute& inOriginal);
	virtual					~DMNameOrOrdinalAttribute();

	// value accessors

	virtual Int32			GetIntValue() const;
	virtual void			SetIntValue(Int32 inValue);
	virtual void			CheckIntValue(Int32 inValue);

	virtual Int32			GetMinValue() const;
	virtual Int32			GetMaxValue() const;

	virtual void			SetValueFromClone(const DMAttribute* inValue);
	
	// display information

	inline Boolean			GetIsNumeric() const
									{ return mIsNumeric; }
	inline Boolean			GetSigned() const
									{ return mSigned; }
	inline void				SetSigned(Boolean inSigned)
									{ mSigned = inSigned; }

	// raw data streaming

protected:
	virtual void			ReadStreamDataSelf(LStream* inStream);
	virtual void			WriteStreamDataSelf(LStream* inStream) const;


	// data members

protected:
	Int32					mNumericValue;			// current value of attribute
	Uint16					mNumericValueSize;		// maximum size of value (in bits)
	Boolean					mSigned;				// true if value is signed
	Boolean					mIsNumeric;				// true if numeric value is valid;
													// false to use string value

};
