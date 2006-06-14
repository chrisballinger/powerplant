// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEModelObject.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/02/96
//     $Date: 2006/01/18 01:33:12 $
//	$History: VEModelObject.cpp $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 03/19/97   Time: 14:52
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Eliminated dangling pointers in VETypeEntry. (Bug fix #1310.)
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/30/97   Time: 13:09
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Added CanDeleteThis override to prevent deleting the root object of a
//	visual hierarchy. (Bug fix #1246.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:02
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added GetDisplayableObjectType method.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/25/96   Time: 15:18
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added GetDisplayableName and GetDescriptor methods.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:12
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added IsVisualObject and GetSubObjectList methods.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:32
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added GetPrintableID and IsSizeAttribute methods.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEModelObject.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"

	// Core : Editors : Views : User interface
#include "VEDrawingAgent.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_VEMiscStrings		= 2007;
const SInt16	str_SpaceIDSpace		= 2;
const SInt16	str_CommaLeftQuote		= 3;
const SInt16	str_RightQuote			= 4;


// ===========================================================================

#pragma mark *** VEModelObject ***

// ---------------------------------------------------------------------------
//		* VEModelObject()
// ---------------------------------------------------------------------------
//	Default constructor

VEModelObject::VEModelObject()
{
}


// ---------------------------------------------------------------------------
//		* VEModelObject(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VEModelObject::VEModelObject(
	LStream* inStream)

: DMObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VEModelObject(VEModelObject&)
// ---------------------------------------------------------------------------
//	Copy constructor

VEModelObject::VEModelObject(
	const VEModelObject& inOriginal)

: DMObject(inOriginal)

{
	mTypeEntry = inOriginal.mTypeEntry;
}


// ---------------------------------------------------------------------------
//		* ~VEModelObject
// ---------------------------------------------------------------------------
//	Destructor

VEModelObject::~VEModelObject()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* IsVisible
// ---------------------------------------------------------------------------
//	Return true if the model object is considered visible. Should be
//	overriden, since visibility is a framework-specific concept.

Boolean
VEModelObject::IsVisible() const
{
	return true;				// override hook
}


// ---------------------------------------------------------------------------
//		* IsVisualObject
// ---------------------------------------------------------------------------
//	Return true if the model object is considered a visual object (i.e.
//	a PowerPlant pane). This is the default. Override and return false
//	for non-visual objects (i.e. PowerPlant attachments).

Boolean
VEModelObject::IsVisualObject() const
{
	return true;			// override hook
}


// ---------------------------------------------------------------------------
//		* FindDrawingAgent
// ---------------------------------------------------------------------------
//	Return the drawing agent for this model object (if there is one).

VEDrawingAgent*
VEModelObject::FindDrawingAgent() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Broadcast message looking for drawing agent.

	return (VEDrawingAgent*) FindUserInterfaceObject(VEDrawingAgent::object_ID);

}


// ---------------------------------------------------------------------------
//		* GetSuperModelObject
// ---------------------------------------------------------------------------
//	Find the super object in the runtime hierarchy.

VEModelObject*
VEModelObject::GetSuperModelObject() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Search up the data model hierarchy until we find another
	// VEModelObject.

	DMElement* super = GetSuperElement();
	while (super != nil) {
		ValidateObject_(super);
		VEModelObject* superVE = (dynamic_cast<VEModelObject*>(super));
		if (superVE != nil) {
			ValidateObject_(superVE);
			return superVE;
		}
		super = super->GetSuperElement();
	}

	// None found.

	return nil;

}


// ---------------------------------------------------------------------------
//		* CanDeleteThis
// ---------------------------------------------------------------------------
//	Overridden to disallow deleting the root object in the visual hierarchy.

Boolean
VEModelObject::CanDeleteThis() const
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// If we're the root object, we can't be deleted.

	VEModelObject* super = GetSuperModelObject();
	if (super == nil)
		return false;
	
	// Otherwise, let inherited handle this.

	return DMObject::CanDeleteThis();

}


// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Return a string which can be used to identify this object in
//	property inspectors, etc.

void
VEModelObject::GetDisplayableName(
	LStr255&	outTitle) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mTypeEntry.GetObject());
	
	// Build displayable name from class name and object ID.
	
	mTypeEntry->GetShortClassName(outTitle);
	
	// Tack on object ID (if any).
	
	LStr255 idString;
	GetPrintableID(idString);

	if (idString.Length() > 0) {
		StApplicationContext appContext;
		outTitle += LStr255(STR_VEMiscStrings, str_SpaceIDSpace);
		outTitle += idString;
	}
	
	// If the object has a title or caption, add that in, too.
	
	LStr255 descriptor;
	GetDescriptor(descriptor);
	
	if (descriptor.Length() > 0) {
		StApplicationContext appContext;
		outTitle += LStr255(STR_VEMiscStrings, str_CommaLeftQuote);
		outTitle += descriptor;
		outTitle += LStr255(STR_VEMiscStrings, str_RightQuote);
	}
}


// ---------------------------------------------------------------------------
//		* GetDisplayableObjectType
// ---------------------------------------------------------------------------
//	Return a printable/displayable name for the object type.

void
VEModelObject::GetDisplayableObjectType(
	LStr255&	outTitle) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mTypeEntry.GetObject());
	
	// Ask type entry for its name.

	mTypeEntry->GetShortClassName(outTitle);

}


// ---------------------------------------------------------------------------
//		* GetPrintableID
// ---------------------------------------------------------------------------
//	Return a name for this object that is suitable for display as an
//	object ID (i.e. pane ID or component name).

void
VEModelObject::GetPrintableID(
	LStr255&	outIDString) const
{
	outIDString[(UInt8)0] = 0;			// override hook
}


// ---------------------------------------------------------------------------
//		* GetDescriptor
// ---------------------------------------------------------------------------
//	Get the title of the visual object.

void
VEModelObject::GetDescriptor(
	LStr255&	outTitle) const
{
	outTitle[(UInt8)0] = 0;			// override hook
}


// ---------------------------------------------------------------------------
//		* GetSubObjectList
// ---------------------------------------------------------------------------
//	Return a pointer to the container attribute that should be used
//	as the sub-object list in the hierarchy view.

DMContainerAttribute*
VEModelObject::GetSubObjectList() const
{
	return GetSubPaneList();
}


// ===========================================================================

#pragma mark -
#pragma mark ** type definition

// ---------------------------------------------------------------------------
//		* SetTypeEntry
// ---------------------------------------------------------------------------
//	Set the type entry object that defines the characteristics of this object.

void
VEModelObject::SetTypeEntry(
	VETypeEntry*	inTypeEntry)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inTypeEntry);
	
	// Replace pointer to type entry.
	
	mTypeEntry = inTypeEntry;
	
	// Listen for change messages.
	
	mTypeEntry->AddListener(this);
	
}


// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Catch notifications of the type entry changing.

void
VEModelObject::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam) 
{

	switch (inMessage) {

		case TypeEntry_Replaced:
			VE_TypeEntryChange* changeMsg = (VE_TypeEntryChange*) ioParam;
			TypeEntryChanged(changeMsg);
			break;
		
		default:
			DMListener::ListenToMessage(inMessage, ioParam);
		
	}
}


// ---------------------------------------------------------------------------
//		* TypeEntryChanged										[protected]
// ---------------------------------------------------------------------------
//	A type entry has changed. If it's ours, make sure we follow the change.

void
VEModelObject::TypeEntryChanged(
	VE_TypeEntryChange*	inChangeMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See what type object changed. If it was ours, replace it.
	
	if (inChangeMessage->mOldType == mTypeEntry) {
		mTypeEntry = inChangeMessage->mNewType;
		mTypeEntry->AddListener(this);
		
		//! TEMPORARY: should change data members to match...
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** data model changes

// ---------------------------------------------------------------------------
//		* CanHostElementSelf									[protected]
// ---------------------------------------------------------------------------
//	Overriden to see if subpane container can host elements.

Boolean
VEModelObject::CanHostElementSelf(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inNewElement);
	
	// See if we have subpanes. If so, ask them if they can host this element.

	DMContainerAttribute* subPanes = GetSubPaneList();
	if (subPanes != nil) {
		ValidateObject_(subPanes);
		return subPanes->CanHostElement(inNewElement, ioPlacement);
	}
	else
		return DMElement::CanHostElementSelf(inNewElement, ioPlacement);

}
