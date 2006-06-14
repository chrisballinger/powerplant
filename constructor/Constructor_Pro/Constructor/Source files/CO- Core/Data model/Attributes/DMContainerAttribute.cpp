// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMContainerAttribute.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/02/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMContainerAttribute.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/18/97   Time: 14:57
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed a bug in SuperEnable that prevented subelements from being
//	enabled properly. (Bug fix #1088.)
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/08/96   Time: 15:53
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Added mCopySubsOnClone flag and associated behaviors.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:20
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMContainerAttribute.h"


// ===========================================================================

#pragma mark *** DMContainerAttribute ***

// ---------------------------------------------------------------------------
//		* DMContainerAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMContainerAttribute::DMContainerAttribute()

: DMContainerMixin((DMElement&) *this)

{

	// Clear variables.

	mMemberClassID = 0x3f3f3f3f;
	mCopySubsOnClone = true;

}


// ---------------------------------------------------------------------------
//		* DMContainerAttribute(DMContainerAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor. Creates a clone of all elements in the list.

DMContainerAttribute::DMContainerAttribute(
	const DMContainerAttribute&		inOriginal)

: DMAttribute(inOriginal),
  DMContainerMixin((DMElement&) *this, inOriginal)

{

	// Copy member fields.

	mMemberClassID = inOriginal.mMemberClassID;
	mCopySubsOnClone = inOriginal.mCopySubsOnClone;
	
	if (!mCopySubsOnClone)
		SetShallowClone();
	
}


// ---------------------------------------------------------------------------
//		* DMContainerAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMContainerAttribute is:
//
//		[DMAttribute data]
//		Boolean:				True if container is "sub-object" list.

DMContainerAttribute::DMContainerAttribute(
	LStream*	inStream)

: DMAttribute(inStream),
  DMContainerMixin((DMElement&) *this)

{

	// Read stream data.

	*inStream >> mIsSubObjectList;
	*inStream >> mMemberClassID;

	// Reset other variables.
	
	mCopySubsOnClone = true;

}


// ---------------------------------------------------------------------------
//		* ~DMContainerAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMContainerAttribute::~DMContainerAttribute()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* FindAttributeByKey
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its key. Checks to see if this attribute matches the desired key.
//	If not, checks all elements in the subelement list.

DMAttribute*
DMContainerAttribute::FindAttributeByKey(
	DM_AttributeKey		inKey) const
{

	// Validate pointers.

	ValidateThis_();
	
	// See if this is the key we're looking for. If not, ask
	// subelements if they match the attribute key.

	DMAttribute* attr = DMAttribute::FindAttributeByKey(inKey);
	if (attr != nil) {
		ValidateObject_(attr);
		return attr;
	}
	else
		return DMContainerMixin::FindAttributeByKey(inKey);
}


// ---------------------------------------------------------------------------
//		* FindAttributeByTitle
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its title. Checks to see if this attribute matches the desired title.
//	If not, checks all elements in the subelement list.

DMAttribute*
DMContainerAttribute::FindAttributeByTitle(
	ConstStringPtr	inTitle) const
{

	// Validate pointers.

	ValidateThis_();
	
	// See if this is the title we're looking for. If not, ask
	// subelements if they match the attribute title.

	DMAttribute* attr = DMAttribute::FindAttributeByTitle(inTitle);
	if (attr != nil) {
		ValidateObject_(attr);
		return attr;
	}
	else
		return DMContainerMixin::FindAttributeByTitle(inTitle);

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications/requests

// ---------------------------------------------------------------------------
//		* FinishCreate
// ---------------------------------------------------------------------------
//	Overriden to allow subelements to initialize themselves.

void
DMContainerAttribute::FinishCreate()
{

	// Validate pointers.

	ValidateThis_();
	
	// Do post-constructor initialization on subelements.
	
	DMContainerMixin::FinishCreate();
	
}


// ---------------------------------------------------------------------------
//		* GoingAway
// ---------------------------------------------------------------------------
//	Pass the "going away" message along to all the subelements.

void
DMContainerAttribute::GoingAway()
{

	// Validate pointers.

	ValidateThis_();
	
	// Tell subelements they're going away.
	
	DMContainerMixin::GoingAway();
	
}


// ---------------------------------------------------------------------------
//		* CollectChanges
// ---------------------------------------------------------------------------
//	Pass the "collect changes" message along to all the subelements.

void
DMContainerAttribute::CollectChanges()
{

	// Validate pointers.

	ValidateThis_();
	
	// Collect changes for subelements.
	
	DMContainerMixin::CollectChanges();

}


// ===========================================================================

#pragma mark -
#pragma mark ** modified flag accessors

// ---------------------------------------------------------------------------
//		* IsSubModified
// ---------------------------------------------------------------------------
//	See if any of the sub elements are modified.

Boolean
DMContainerAttribute::IsSubModified(
	Boolean		inTentative) const
{

	// Validate pointers.

	ValidateThis_();
	
	// See if any of the subelements are modified.
	
	return DMContainerMixin::IsSubModified(inTentative);

}


// ---------------------------------------------------------------------------
//		* ResetModified
// ---------------------------------------------------------------------------
//	Clear modified flags for all subelements.

void
DMContainerAttribute::ResetModified()
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
DMContainerAttribute::Enable()
{

	// Validate pointers.

	ValidateThis_();
	
	// Remember if we were disabled.
	
	Boolean wasEnabled = IsEnabled();
	
	// Enable this element.
	
	DMAttribute::Enable();
	
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
DMContainerAttribute::Disable()
{

	// Validate pointers.

	ValidateThis_();
	
	// If this attribute is currently enabled, disable all subelements.
	
	if (mEnabled == triState_On)
		DisableSubs();

	// Disable this element.

	DMAttribute::Disable();

}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* ReadStreamData
// ---------------------------------------------------------------------------
//	Read the data into each contained attribute.

void
DMContainerAttribute::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{

	// Validate pointers.

	ValidateThis_();
	
	// Read stream data for all subelements.
	
	DMContainerMixin::ReadStreamData(inStream, inStreamSelector);

}


// ---------------------------------------------------------------------------
//		* WriteStreamData
// ---------------------------------------------------------------------------
//	Write the data from each contained attribute.

void
DMContainerAttribute::WriteStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Write stream data for all subelements.
	
	DMContainerMixin::WriteStreamData(inStream, inStreamSelector);

}


// ===========================================================================

#pragma mark -
#pragma mark ** data model changes

// ---------------------------------------------------------------------------
//		* CanHostElement
// ---------------------------------------------------------------------------
//	Ask subelements if they want to host the element.

Boolean
DMContainerAttribute::CanHostElement(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{
	
	// Validate pointers.

	ValidateThis_();
	
	// Ask subelements if they can host this element.
	
	return IsEnabled() && DMContainerMixin::CanHostElement(inNewElement, ioPlacement);

}


// ---------------------------------------------------------------------------
//		* CanHostElementSelf									[protected]
// ---------------------------------------------------------------------------
//	We can accept any element which is derived from the named class ID.
//	This covers most containing needs.

Boolean
DMContainerAttribute::CanHostElementSelf(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);

	// Accept the element if it is derived from our required class ID
	// and it is a superelement of this element.

	if (inNewElement->IsDerivedFrom(mMemberClassID) && !IsSubElementOf(inNewElement)) {
		ioPlacement->mHostElement = this;
		if (ioPlacement->mInsertionIndex == 0)
			ioPlacement->mInsertionIndex = LArray::index_Last;
		return true;
	}
	else
		return false;

}


// ---------------------------------------------------------------------------
//		* HostElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Add the new element to our subelement list.

void
DMContainerAttribute::HostElementSelf(
	DMElement*				inNewElement,
	const DM_DragPlacement* ioPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);

	// Add this item to the subelement list.

	DMContainerMixin::HostElementSelf(inNewElement, ioPlacement);

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
DMContainerAttribute::SuperEnable(
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
		
		DMAttribute::SuperEnable(inSuperElement);
		
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
DMContainerAttribute::SuperDisable(
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
		DMAttribute::SuperDisable(inSuperElement);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** shallow cloning helper

// ---------------------------------------------------------------------------
//		* SetShallowClone										[protected]
// ---------------------------------------------------------------------------
//	Be sure that we don't copy subelements when cloning this object.

void
DMContainerAttribute::SetShallowClone()
{
	DMContainerMixin::SetShallowClone();
}
