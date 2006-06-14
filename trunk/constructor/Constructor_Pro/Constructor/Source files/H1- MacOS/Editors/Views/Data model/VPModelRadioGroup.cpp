// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelRadioGroup.cpp		   © 1996 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/08/96
//     $Date: 2006/01/18 01:33:55 $
//  $History: VPModelRadioGroup.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/21/97   Time: 14:17
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Updated FinishCreateSelf to not remove radio buttons. (Bug fix #1194.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/08/96   Time: 19:42
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Finished baseline radio group support.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/08/96   Time: 18:02
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#include "VPModelRadioGroup.h"

	// Core : Data model : Attributes
#include "DMListAttribute.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"
#include "VPTypeEntry.h"


// ===========================================================================

#pragma mark *** VPModelRadioGroup ***

// ---------------------------------------------------------------------------
//		* VPModelRadioGroup()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelRadioGroup::VPModelRadioGroup()
{
}


// ---------------------------------------------------------------------------
//		* VPModelRadioGroup(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional data is read.

VPModelRadioGroup::VPModelRadioGroup(
	LStream*	inStream)

: VPModelObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VPModelRadioGroup(VPModelRadioGroup&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPModelRadioGroup::VPModelRadioGroup(
	const VPModelRadioGroup&	inOriginal)

: VPModelObject(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~VPModelRadioGroup
// ---------------------------------------------------------------------------
//	Destructor

VPModelRadioGroup::~VPModelRadioGroup()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* IsVisualObject
// ---------------------------------------------------------------------------
//	Return true if this is a visual object.

Boolean
VPModelRadioGroup::IsVisualObject() const
{
	return false;
}


// ===========================================================================

#pragma mark -
#pragma mark ** radio group management

// ---------------------------------------------------------------------------
//		* AddMember
// ---------------------------------------------------------------------------
//	Public method to add a radio button to this group. Called by
//	VPEditor::MakeRadioGroup.

void
VPModelRadioGroup::AddMember(
	VPModelObject*		inObject)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);
	ValidateObject_(mButtonList.GetObject());
	
	// Duplicate the button list's prototype and set it's value to the pane ID
	// of the new object.
	
	DMListAttribute* buttonList = (dynamic_cast<DMListAttribute*>(mButtonList.GetObject()));
	ValidateObject_(buttonList);
	
	DMAttributePtr newAttr;
	newAttr = (DMAttribute*) (buttonList->GetPrototype()->Clone());
	ValidateObject_(newAttr.GetObject());
	
	DMIntegerAttribute* idAttr = (dynamic_cast<DMIntegerAttribute*>(newAttr.GetObject()));
	ValidateObject_(idAttr);
	
	// Add this attribute to the radio group, then set the ID.
	
	buttonList->InsertElementAt(LArray::index_Last, idAttr, true);
	idAttr->SetIntValue(inObject->GetPaneID());
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Find the radio button ID list and make sure we listen for changes
//	from all buttons.

void
VPModelRadioGroup::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();

	// Do inherited initialization.
	
	VPModelObject::FinishCreateSelf();
	
	// Look for radio button IDs attribute.

	DMContainerAttribute* buttonList = FindContainerAttribute(pPaneRadioButtonList);
	ValidateObject_(buttonList);

	// Listen to each radio button.
	
	DMIterator iter(const_cast<LSharableArray&>(buttonList->GetSubElements()));
	while (iter.NextElement()) {
		
		DMIntegerAttribute* idAttr = (dynamic_cast<DMIntegerAttribute*>(iter.CurrentElement()));
		if (idAttr != nil) {
			ValidateObject_(idAttr);
			RadioButtonAdded(idAttr->GetIntValue());
		}
	}
	
	// Save pointer to radio button IDs. We don't do this earlier
	// because we want to disable RemoveRadioButton while finishing creation.
	
	mButtonList = buttonList;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** radio button management

// ---------------------------------------------------------------------------
//		* FindRadioButton										[protected]
// ---------------------------------------------------------------------------
//	Find the data model object that represent the specified pane ID.

VPModelObject*
VPModelRadioGroup::FindRadioButton(
	PaneIDT		inButtonID)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Look for parent object in runtime hierarchy.
	
	VPModelObject* parent = (dynamic_cast<VPModelObject*>(GetSuperModelObject()));
	if (parent == nil)
		return nil;
	
	ValidateObject_(parent);
	
	// Ask the parent for the first object that matches this ID.
	
	return parent->FindPaneByID(inButtonID);
	
}


// ---------------------------------------------------------------------------
//		* RadioButtonAdded										[protected]
// ---------------------------------------------------------------------------
//	Called when a radio button ID is added to the list. Make sure that we
//	listen for changes from this button.

Boolean
VPModelRadioGroup::RadioButtonAdded(
	PaneIDT		inButtonID)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Look for this radio button. If not found, we can't add this button.
	
	VPModelObject* button = FindRadioButton(inButtonID);
	if (button == nil)
		return false;
	
	ValidateObject_(button);
	
	// Now make sure this button is in fact a control.

	VPTypeEntry* type = button->GetVPTypeEntry();
	ValidateObject_(type);
	
	if (!type->TypeDerivesFrom('cntl'))
		return false;
	
	// Okay, it's legitimate. Listen for changes from this button.
	
	button->AddListener(this);
	return true;

}


// ---------------------------------------------------------------------------
//		* RadioButtonRemoved									[protected]
// ---------------------------------------------------------------------------
//	Called when a radio button ID is added to the list. Make sure that we
//	stop listening for changes from this button.

Boolean
VPModelRadioGroup::RadioButtonRemoved(
	PaneIDT		inButtonID)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Look for this radio button. Stop listening for changes from it.
	
	VPModelObject* button = FindRadioButton(inButtonID);
	if (button != nil) {
		ValidateObject_(button);
		button->RemoveListener(this);
		return true;
	}
	
	// No such button. Ignore.
	
	return false;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Intercept list-change and value-change messages. Call RadioButtonAdded
//	and RadioButtonRemoved as appropriate.

void
VPModelRadioGroup::SendUpdateMessage(
	MessageT	inMessage,
	void*		ioParam,
	DMElement*	inSubElement)
{

	// Validate pointers.

	ValidateThis_();

	// Intercept list change messages.

	if (inMessage == Container_ListChange) {
		
		// Make sure it's our button list that's affected.
		
		DM_ListChange* listMsg = (DM_ListChange*) ioParam;
		if (listMsg->mContainer == mButtonList.GetObject()) {
		
			// It is, see what object ID is being changed.
		
			DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(listMsg->mMovedElement));
			ValidateObject_(intAttr);
		
			if (listMsg->mElementAdded)
				RadioButtonAdded(intAttr->GetIntValue());		//! TEMPORARY: validity check here?
			else
				RadioButtonRemoved(intAttr->GetIntValue());
		
		}
	}

	// Intercept value change messages.
	
	if (inMessage == Attribute_ValueChange) {
	
		// See if the attribute is in our button list.
		
		DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
		DMAttribute* attr = valueMsg->mAttribute;
		ValidateObject_(attr);
		
		if (attr->GetKey() == pPaneRadioButtonID) {
		
			// It is, get the object ID and add it to the list.
			
			DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(attr));
			ValidateObject_(intAttr);
			
			RadioButtonAdded(intAttr->GetIntValue());
		
		}
	}

	// Relay message as usual.

	VPModelObject::SendUpdateMessage(inMessage, ioParam, inSubElement);

}


// ---------------------------------------------------------------------------
//		* DoSanityCheckSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure all the radio button IDs actually point to controls.

void
VPModelRadioGroup::DoSanityCheckSelf()
{

	// Validate pointers.
	
	ValidateThis_();

	// Make sure that each radio button is in fact a button.
	
	DMIterator iter(const_cast<LSharableArray&>(mButtonList->GetSubElements()));
	while (iter.NextElement()) {
		
		DMIntegerAttribute* idAttr = (dynamic_cast<DMIntegerAttribute*>(iter.CurrentElement()));
		if (idAttr != nil) {
			ValidateObject_(idAttr);
			if (!RadioButtonAdded(idAttr->GetIntValue()))
				mButtonList->RemoveElement(idAttr);
		}
	}
}


// ---------------------------------------------------------------------------
//		* GoingAway												[protected]
// ---------------------------------------------------------------------------
//	Overridden only to prevent a compiler warning.

void
VPModelRadioGroup::GoingAway()
{
	DMObject::GoingAway();
}


// ---------------------------------------------------------------------------
//		* GoingAway												[protected]
// ---------------------------------------------------------------------------
//	If one of the radio buttons we're referencing is deleted, also delete
//	its ID from the list.

void
VPModelRadioGroup::GoingAway(
	DMElement*		inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);

	// See if this is a view editor object.
	
	VPModelObject* object = (dynamic_cast<VPModelObject*>(inElement));
	if (object == nil)
		return;
	
	// It is, see if this object is one of our radio buttons.
	
	DMIterator iter(const_cast<LSharableArray&>(mButtonList->GetSubElements()));
	while (iter.NextElement()) {
		
		DMIntegerAttribute* idAttr = (dynamic_cast<DMIntegerAttribute*>(iter.CurrentElement()));
		if (idAttr != nil) {
			ValidateObject_(idAttr);
			if (object->GetPaneID() == idAttr->GetIntValue())
				mButtonList->RemoveElement(idAttr);
		}

	}
}
