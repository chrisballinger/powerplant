// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMContainerAttribute.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/02/96
//	   $Date: 2006/01/18 01:32:30 $
//	$History: DMContainerAttribute.h $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/18/97   Time: 14:58
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed a bug in SuperEnable that prevented subelements from being
//	enabled properly. (Bug fix #1088.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/08/96   Time: 15:52
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added mCopySubsOnClone flag and associated behaviors.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:21
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added ShallowClone method to DMElement.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/01/96   Time: 01:58
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Fixed a bug in FinishCreate. Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:58
//	Checked in '$/Constructor/Source files/Data model/Attributes'
//	Comment: Updated to use new enabled flag.
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

	// Core : Data model : Core objects
#include "DMContainerMixin.h"


// ===========================================================================
//		* DMContainerAttribute
// ===========================================================================
//
//	DMContainerAttribute is an attribute that acts as a container
//	for other attributes or objects.
//
// ===========================================================================

class DMContainerAttribute :	public DMAttribute,
								public DMContainerMixin {

	DMClassID_				('cont', DMContainerAttribute, DMAttribute);

public:
							DMContainerAttribute();
							DMContainerAttribute(LStream* inStream);
							DMContainerAttribute(const DMContainerAttribute& inOriginal);
	virtual					~DMContainerAttribute();

	// subelement accessors

	const LSharableArray&	GetSubElements() const
									{ return mSubElements; }
	inline FourCharCode		GetMemberClassID() const
									{ return mMemberClassID; }
	inline void				SetMemberClassID(ClassIDT inMemberClassID)
									{ mMemberClassID = inMemberClassID; }

	inline Boolean			GetCopySubsOnClone() const
									{ return mCopySubsOnClone; }
	inline void				SetCopySubsOnClone(Boolean inCopySubs)
									{ mCopySubsOnClone = inCopySubs; }

	// attribute accessors

	virtual DMAttribute*	FindAttributeByKey(
									DM_AttributeKey		inKey) const;
	virtual DMAttribute*	FindAttributeByTitle(
									ConstStringPtr		inTitle) const;

	// update notifications/requests

	virtual void			FinishCreate();

	virtual void			GoingAway();
	virtual void			CollectChanges();
	
	// modified flag accessors

	virtual Boolean			IsSubModified(
									Boolean				inTentative = false) const;
	virtual void			ResetModified();

	// enabled flag accessors
	
	virtual void			Enable();
	virtual void			Disable();

	// value accessors

	virtual void			ReadStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0);
	virtual void			WriteStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0) const;

	// data model changes

	virtual Boolean			CanHostElement(
									const DMElement*	inNewElement,
									DM_DragPlacement*	ioPlacement);

protected:
	virtual Boolean			CanHostElementSelf(
									const DMElement*	inNewElement,
									DM_DragPlacement*	ioPlacement);
	virtual void			HostElementSelf(
									DMElement*				inNewElement,
									const DM_DragPlacement*	ioPlacement);

	// update requests from parent

	virtual void			SuperEnable(
									DMElement*			inSuperElement);
	virtual void			SuperDisable(
									DMElement*			inSuperElement);


	// shallow cloning helper

	virtual void			SetShallowClone();
	


	// data members

protected:
	FourCharCode			mMemberClassID;				// data model class ID of subelements
														//	(used by CanHostElementSelf)
	Boolean					mCopySubsOnClone;			// true to clone subobjects when this
														//	object is cloned (normally true)

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

#define DMContainerAttribute_(attrKey, attrName)							\
	DMAttribute_(attrKey, attrName, Container)								\
	inline DMContainerAttribute*		Get##attrName() const				\
		{ return Get##attrName##Attribute(); }
