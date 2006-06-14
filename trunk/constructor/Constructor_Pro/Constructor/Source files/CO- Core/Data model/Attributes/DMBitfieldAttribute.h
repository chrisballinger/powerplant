// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMBitfieldAttribute.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/09/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMBitfieldAttribute.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:19
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Attributes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"


// ===========================================================================
//		* DMBitfieldAttribute
// ===========================================================================
//
//	DMBitfieldAttribute is an attribute that contains other integer
//	attributes and writes them to and from stream as a packed bitfield.
//
// ===========================================================================

class DMBitfieldAttribute : public DMContainerAttribute {

	DMClassID_				('bitf', DMBitfieldAttribute, DMContainerAttribute);

public:
							DMBitfieldAttribute();
							DMBitfieldAttribute(const DMBitfieldAttribute& inOriginal);
							DMBitfieldAttribute(LStream* inStream);
	virtual					~DMBitfieldAttribute();

	// value accessors
	
	virtual SInt32			GetIntValue() const;
	virtual void			SetIntValue(SInt32 inValue);

	inline UInt16			GetValueSize() const
									{ return mValueSize; }
	inline void				SetValueSize(UInt16 inValueSize)
									{ mValueSize = inValueSize; }

	virtual void			ReadStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0);
	virtual void			WriteStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0) const;

	// update requests

protected:
	virtual Boolean			CanHostElementSelf(
									const DMElement*	inNewElement,
									DM_DragPlacement*	ioPlacement);
	virtual void			HostElementSelf(
									DMElement*				inNewElement,
									const DM_DragPlacement*	inPlacement);

	// update notification

	virtual void			SendUpdateMessage(
									MessageT			inMessage,
									void*				ioParam,
									DMElement*			inSubElement = nil);

	// raw data streaming

	virtual void			ReadStreamDataSelf(
									LStream*			inStream);
	virtual void			WriteStreamDataSelf(
									LStream*			inStream) const;




	UInt16					mValueSize;				// size of bitfield (in bytes): maximum 4

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

#define DMBitfieldAttribute_(attrKey, attrName)								\
	DMAttribute_(attrKey, attrName, Bitfield)								\
	inline SInt32	Get##attrName() const									\
		{ return Get##attrName##Attribute()->GetIntValue(); }				\
	inline void		Set##attrName(SInt32 inValue) const						\
		{ Get##attrName##Attribute()->SetIntValue(inValue); }
