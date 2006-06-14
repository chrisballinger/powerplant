// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMObject.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:36 $
//	$History: DMObject.h $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:01
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed a bug in SuperEnable that prevented subelements from being
//	enabled properly. (Bug fix #1088.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/07/97   Time: 18:47
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Rolled in an update from Clint 01/06/97.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:22
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:45
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added SendUpdateMessage override. Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:51
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Replaced ItemControl and CanMakeChanges with Enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMElement.h"
#include "DMContainerMixin.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMBitfieldAttribute;
class DMContainerAttribute;
class DMIntegerAttribute;
class DMStringAttribute;
class DMRGBColorAttribute;


// ===========================================================================
//		* DMObject
// ===========================================================================
//
//	DMObject represents a visible object in the data model. An object
//	has a list of attributes, one or more of which may be a list of
//	subobjects. Subclasses of DMObject may be created to encapsulate
//	domain-specific knowledge, though this is not necessary.
//
// ===========================================================================

class DMObject :	public DMElement,
					public DMContainerMixin {

	// data model description

	DMClassID_				('obj ', DMObject, DMElement);

	// constructors/destructor

public:
							DMObject();
							DMObject(LStream* inStream);
							DMObject(const DMObject& inOriginal);
	virtual					~DMObject();

	// attribute accessors

	virtual DMAttribute*	FindAttributeByKey(
									DM_AttributeKey		inKey) const;
	virtual DMAttribute*	FindAttributeByTitle(
									ConstStringPtr		inTitle) const;

	DMBitfieldAttribute*	FindBitfieldAttribute(
									DM_AttributeKey		inKey) const;
	DMContainerAttribute*	FindContainerAttribute(
									DM_AttributeKey		inKey) const;
	DMIntegerAttribute*		FindIntegerAttribute(
									DM_AttributeKey		inKey) const;
	DMStringAttribute*		FindStringAttribute(
									DM_AttributeKey		inKey) const;
	DMRGBColorAttribute*	FindRGBColorAttribute(
									DM_AttributeKey		inKey) const;
	
	const LSharableArray&	GetAttributes() const
									{ return mSubElements; }

	// object data streaming
	
	virtual void			ReadStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0);
	virtual void			WriteStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0) const;

	// object hierarchy

	virtual DMObject*		GetAffectedObject() const;

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

	// data model changes

	virtual Boolean			CanHostElement(
									const DMElement*	inNewElement,
									DM_DragPlacement*	ioPlacement);

	// shallow cloning helper

	virtual void			SetShallowClone();

	// update requests from parent

protected:
	virtual void			SuperEnable(DMElement*		inSuperElement);
	virtual void			SuperDisable(DMElement*		inSuperElement);

	virtual void			SendUpdateMessage(
									MessageT			inMessage,
									void*				ioParam,
									DMElement*			inSubElement = nil);

};
