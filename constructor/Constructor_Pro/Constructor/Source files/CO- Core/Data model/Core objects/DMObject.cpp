// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMObject.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:36 $
//	$History: DMObject.cpp $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:01
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed a bug in SuperEnable that prevented subelements from being
//	enabled properly. (Bug fix #1088.)
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/07/97   Time: 18:47
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Rolled in an update from Clint 01/06/97.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:22
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:09
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Fixed a bug in FinishCreate that caused the FinishCreateSelf override
//	hook to get called twice.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:44
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added SendUpdateMessage override. Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:51
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Replaced ItemControl and CanMakeChanges with Enabled flag.
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

#include "DMObject.h"

	// Core : Data model : Attributes
#include "DMBitfieldAttribute.h"
#include "DMContainerAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMStringAttribute.h"
#include "DMRGBColorAttribute.h"


// ===========================================================================

#pragma mark *** DMObject ***

// ---------------------------------------------------------------------------
//		* DMObject()
// ---------------------------------------------------------------------------
//	Default constructor

DMObject::DMObject()

: DMContainerMixin((DMElement&) *this)
{
}


// ---------------------------------------------------------------------------
//		* DMObject(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. DMObject doesn't take any stream data.

DMObject::DMObject(
	LStream*	/* inStream */)

: DMContainerMixin((DMElement&) *this)
{
}


// ---------------------------------------------------------------------------
//		* DMObject(DMObject&)
// ---------------------------------------------------------------------------
//	Copy constructor. Clones attribute list.

DMObject::DMObject(
	const DMObject&		inOriginal)

: DMContainerMixin((DMElement&) *this, inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~DMObject
// ---------------------------------------------------------------------------
//	Destructor

DMObject::~DMObject()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* FindAttributeByKey
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its key. Recursively searches subelement (attribute) list.

DMAttribute*
DMObject::FindAttributeByKey(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Look through attribute list for this key.
	
	return DMContainerMixin::FindAttributeByKey(inKey);

}


// ---------------------------------------------------------------------------
//		* FindAttributeByTitle
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its title. Recursively searches subelement (attribute) list.

DMAttribute*
DMObject::FindAttributeByTitle(
	ConstStringPtr	inTitle) const
{
	
	// Validate pointers.

	ValidateThis_();
	
	// Look through attribute list for this title.

	return DMContainerMixin::FindAttributeByTitle(inTitle);

}


// ---------------------------------------------------------------------------
//		* FindBitfieldAttribute
// ---------------------------------------------------------------------------
//	A helper routine that finds an attribute using FindAttributeByKey
//	then casts it to DMBitfieldAttribute and checks the cast. Throws a
//	nil pointer exception if the attribute doesn't exist or isn't the
//	right type.

DMBitfieldAttribute*
DMObject::FindBitfieldAttribute(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();

	// Find the attribute.

	DMAttribute* attr = FindAttributeByKey(inKey);
	ValidateObject_(attr);
	
	// Cast it to container attribute.
	
	DMBitfieldAttribute* intAttr = (dynamic_cast<DMBitfieldAttribute*>(attr));
	ValidateObject_(intAttr);
	
	return intAttr;

}


// ---------------------------------------------------------------------------
//		* FindContainerAttribute
// ---------------------------------------------------------------------------
//	A helper routine that finds an attribute using FindAttributeByKey
//	then casts it to DMContainerAttribute and checks the cast. Throws a
//	nil pointer exception if the attribute doesn't exist or isn't the
//	right type.

DMContainerAttribute*
DMObject::FindContainerAttribute(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();

	// Find the attribute.

	DMAttribute* attr = FindAttributeByKey(inKey);
	ValidateObject_(attr);
	
	// Cast it to container attribute.
	
	DMContainerAttribute* intAttr = (dynamic_cast<DMContainerAttribute*>(attr));
	ValidateObject_(intAttr);
	
	return intAttr;

}


// ---------------------------------------------------------------------------
//		* FindIntegerAttribute
// ---------------------------------------------------------------------------
//	A helper routine that finds an attribute using FindAttributeByKey
//	then casts it to DMIntegerAttribute and checks the cast. Throws a
//	nil pointer exception if the attribute doesn't exist or isn't the
//	right type.

DMIntegerAttribute*
DMObject::FindIntegerAttribute(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();

	// Find the attribute.

	DMAttribute* attr = FindAttributeByKey(inKey);
	ValidateObject_(attr);
	
	// Cast it to integer attribute.
	
	DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(attr));
	ValidateObject_(intAttr);
	
	return intAttr;

}


// ---------------------------------------------------------------------------
//		* FindStringAttribute
// ---------------------------------------------------------------------------
//	A helper routine that finds an attribute using FindAttributeByKey
//	then casts it to DMStringAttribute and checks the cast. Throws a
//	nil pointer exception if the attribute doesn't exist or isn't the
//	right type.

DMStringAttribute*
DMObject::FindStringAttribute(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();

	// Find the attribute.

	DMAttribute* attr = FindAttributeByKey(inKey);
	ValidateObject_(attr);
	
	// Cast it to string attribute.
	
	DMStringAttribute* intAttr = (dynamic_cast<DMStringAttribute*>(attr));
	ValidateObject_(intAttr);
	
	return intAttr;

}

// ---------------------------------------------------------------------------
//		* FindRGBColorAttribute
// ---------------------------------------------------------------------------
//	A helper routine that finds an attribute using FindAttributeByKey
//	then casts it to DMRGBColorAttribute and checks the cast. Throws a
//	nil pointer exception if the attribute doesn't exist or isn't the
//	right type.

DMRGBColorAttribute*
DMObject::FindRGBColorAttribute(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();

	// Find the attribute.

	DMAttribute* attr = FindAttributeByKey(inKey);
	ValidateObject_(attr);
	
	// Cast it to string attribute.
	
	DMRGBColorAttribute* intAttr = (dynamic_cast<DMRGBColorAttribute*>(attr));
	ValidateObject_(intAttr);
	
	return intAttr;

}

// ===========================================================================

#pragma mark -
#pragma mark ** object data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamData
// ---------------------------------------------------------------------------
//	Read values for all the attributes from stream. For simple objects,
//	this should amount to a complete description of the data.

void
DMObject::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{

	// Validate pointers.

	ValidateThis_();
	
	// Use attribute list to read stream data.
	
	DMContainerMixin::ReadStreamData(inStream, inStreamSelector);

}


// ---------------------------------------------------------------------------
//		* WriteStreamData
// ---------------------------------------------------------------------------
//	Write values for all the attributes to stream. For simple objects,
//	this should amount to a complete description of the data.

void
DMObject::WriteStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Use attribute list to write stream data.
	
	DMContainerMixin::WriteStreamData(inStream, inStreamSelector);

}


// ===========================================================================

#pragma mark -
#pragma mark ** object hierarchy

// ---------------------------------------------------------------------------
//		* GetAffectedObject
// ---------------------------------------------------------------------------
//	This method can be used to find the object affected by a change.
//	Since DMObject is an object, we just "return this".

DMObject*
DMObject::GetAffectedObject() const
{

	// Validate pointers and return pointer to this.

	ValidateThis_();
	return (const_cast<DMObject*>(this));

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications/requests

// ---------------------------------------------------------------------------
//		* FinishCreate
// ---------------------------------------------------------------------------
//	Overriden to allow attributes to initialize themselves.

void
DMObject::FinishCreate()
{

	// Validate pointers.

	ValidateThis_();
	
	// Do post-constructor initialization on subelements.
	
	DMContainerMixin::FinishCreate();
	
}


// ---------------------------------------------------------------------------
//		* GoingAway
// ---------------------------------------------------------------------------
//	Public method to be called when the element is to be removed from
//	the visible data model. (This does not necessarily correspond to the
//	element actually being deleted.) Broadcasts a "going away" message
//	which should trigger display objects to remove any representation
//	of this element. If there are subelements, they are notified as well.
//
//	Subclasses which need to do special behavior when the object is going
//	away should override GoingAwaySelf().

void
DMObject::GoingAway()
{

	// Validate pointers.

	ValidateThis_();
	
	// Tell subelements that they're going away.
	
	DMContainerMixin::GoingAway();

}


// ---------------------------------------------------------------------------
//		* CollectChanges
// ---------------------------------------------------------------------------
//	Public method to be called when a file is about to be saved, or
//	pending changes need to be collected for any other reason. Broadcasts
//	a "collect changes" message which should trigger display objects
//	(especially the in-place editor) to record partially completed changes.
//	If there are subelements, they are notified as well.
//
//	Subclasses which need to do special behavior for collect changes
//	should override CollectChangesSelf().

void
DMObject::CollectChanges()
{

	// Validate pointers.

	ValidateThis_();
	
	// Ask subelements to collect changes.
	
	DMContainerMixin::CollectChanges();

}


// ===========================================================================

#pragma mark -
#pragma mark ** modified flag accessors

// ---------------------------------------------------------------------------
//		* IsSubModified
// ---------------------------------------------------------------------------
//	Returns true if any subelement has been modified. The "inTentative"
//	parameter checks to see if there are any pending changes which haven't
//	been recorded yet (appropriate for determining if the Save command
//	should be enabled).

Boolean
DMObject::IsSubModified(
	Boolean		inTentative) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Ask subelements if they're modified.
	
	return DMContainerMixin::IsSubModified(inTentative);

}


// ---------------------------------------------------------------------------
//		* ResetModified
// ---------------------------------------------------------------------------
//	Call this method after a file has been successfully saved. Clears the
//	mModified and mSubModified flag of this element and all subelements.

void
DMObject::ResetModified()
{

	// Validate pointers.

	ValidateThis_();
	
	// Reset modified flag for all subelements.
	
	DMContainerMixin::ResetModified();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** enabled flag accessors

// ---------------------------------------------------------------------------
//		* Enable
// ---------------------------------------------------------------------------
//	Make this element enabled (i.e. eligible for editing). Enabling works
//	like LPane's enabled flag: if the superelement is disabled, this element
//	is "latent" enabled.

void
DMObject::Enable()
{

	// Validate pointers.

	ValidateThis_();
	
	// Remember if we were disabled.
	
	Boolean wasEnabled = IsEnabled();
	
	// Enable this element.
	
	DMElement::Enable();
	
	// If we became enabled, enable subelements.
	
	if (!wasEnabled && IsEnabled())
		EnableSubs();
	
}


// ---------------------------------------------------------------------------
//		* Disable
// ---------------------------------------------------------------------------
//	Make this element disabled (i.e. ineligible for editing).
//	If this element has subelements, they are also marked as disabled.

void
DMObject::Disable()
{

	// Validate pointers.

	ValidateThis_();
	
	// If this attribute is currently enabled, disable all subelements.
	
	if (mEnabled == triState_On)
		DisableSubs();

	// Disable this element.

	DMElement::Disable();

}


// ===========================================================================

#pragma mark -
#pragma mark ** data model changes

// ---------------------------------------------------------------------------
//		* CanHostElement
// ---------------------------------------------------------------------------
//	Return true if "inNewElement" can be made a part of this element (i.e.
//	is an acceptable subelement). The "ioPlacement" parameter should be
//	modified to reflect the preferred placement of the new element.
//
//	In general, subclasses should override CanHostElementSelf.

Boolean
DMObject::CanHostElement(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{
	
	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);
	
	// Ask our attributes if we can host this element.
	
	return IsEnabled() && DMContainerMixin::CanHostElement(inNewElement, ioPlacement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** shallow cloning helper

// ---------------------------------------------------------------------------
//		* SetShallowClone										[protected]
// ---------------------------------------------------------------------------
//	Be sure that we don't copy subelements when cloning this object.

void
DMObject::SetShallowClone()
{
	DMContainerMixin::SetShallowClone();
}


// ===========================================================================

#pragma mark -
#pragma mark ** update requests from parent

// ---------------------------------------------------------------------------
//		* SuperEnable											[protected]
// ---------------------------------------------------------------------------
//	An element that refers to this element has been enabled. If it is
//	our super element, enable this element.

void
DMObject::SuperEnable(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	if (inSuperElement != nil)
		ValidateObject_(inSuperElement);
	
	// If this element is disabled, enabling its superelement
	// causes this and all subelements to become enabled if they
	// were previously latently enabled.
	
	if ((inSuperElement == mSuperElement) && (mEnabled == triState_Latent)) {

		// Remember if we were disabled.
		
		Boolean wasEnabled = IsEnabled();
	
		// Enable this element.
		
		DMElement::SuperEnable(inSuperElement);
		
		// If we became enabled, enable subelements.
		
		if (!wasEnabled && IsEnabled())
			EnableSubs();
		
	}
}


// ---------------------------------------------------------------------------
//		* SuperDisable											[protected]
// ---------------------------------------------------------------------------
//	An element that refers to this element has been disabled. If it is
//	our super element, disable this element.

void
DMObject::SuperDisable(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	if (inSuperElement != nil)
		ValidateObject_(inSuperElement);
	
	// If this element is enabled, disabling its superelement
	// causes this and all subelements to become latently enabled.
	
	if ((inSuperElement == mSuperElement) && (mEnabled == triState_On)) {
		DisableSubs();	
		DMElement::SuperDisable(inSuperElement);
	}
}


// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Overriden to ensure that update messages are *not* sent by superelements.
//	It is assumed that concerned parties will listen to an object, if not
//	the individual attributes used by that object.

void
DMObject::SendUpdateMessage(
	MessageT	inMessage,
	void*		ioParam,
	DMElement*	inSubElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Prevent this object from being disposed during method.

	StSharer lockObj(this);

	// Fill in updated element. Nil is a signal that we should
	// use this element.

	if (inSubElement == nil)
		inSubElement = this;
	
	// Send the change message.
	
	BroadcastMessage(inMessage, ioParam);

}
