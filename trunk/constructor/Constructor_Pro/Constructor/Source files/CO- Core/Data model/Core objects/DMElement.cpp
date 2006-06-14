// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMElement.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/26/96
//	   $Date: 2006/01/18 01:32:35 $
//	$History: DMElement.cpp $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:21
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:05
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Changed IsEnabled to disable all elements if memory is low. This
//	effectively disables all non-essential commands.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/03/96   Time: 20:49
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Fixed bug in ResetModified. It wasn't sending the mod-flag changed
//	message.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/18/96   Time: 00:04
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Fixed const access bug in GetAffectedObject.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:43
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

#include "DMElement.h"

	// Core : Data model : Undo-redo
#include "DMModFlagSubAction.h"
#include "StDMTransactionBuilder.h"

	// Utility classes
#include "UMemoryUtils.h"


// ===========================================================================

#pragma mark *** DMElement ***

// ---------------------------------------------------------------------------
//		* DMElement()
// ---------------------------------------------------------------------------
//	Default constructor

DMElement::DMElement()
{
	InitElement();
}


// ---------------------------------------------------------------------------
//		* DMElement(const DMElement&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMElement::DMElement(
	const DMElement&	/* inOriginal */)
{
	InitElement();
	// don't clone modified or submodified values
}


// ---------------------------------------------------------------------------
//		* ~DMElement
// ---------------------------------------------------------------------------
//	Destructor

DMElement::~DMElement()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** object type information

// ---------------------------------------------------------------------------
//		* SetShallowClone
// ---------------------------------------------------------------------------
//	Be sure that we don't copy subelements when cloning this object.

void
DMElement::SetShallowClone()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Return a name for this object that is suitable for displaying
//	in an window title. This is an override hook.

void
DMElement::GetDisplayableName(
	LStr255&	outTitle) const
{
	outTitle[(UInt8)0] = 0;			// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** object hierarchy

// ---------------------------------------------------------------------------
//		* FinishCreate
// ---------------------------------------------------------------------------
//	Called by DMReanimator when the element has been placed in the object
//	hierarchy.

void
DMElement::FinishCreate()
{
	
	// Validate pointers.

	ValidateThis_();
	
	// Call override hook.
	
	FinishCreateSelf();

}


// ---------------------------------------------------------------------------
//		* SetSuperElement
// ---------------------------------------------------------------------------
//	Change the super element of this element.

void
DMElement::SetSuperElement(
	DMElement*	inSuperElement)
{

	// Validate input parameters.

	ValidateThis_();
	if (inSuperElement != nil)
		ValidateObject_(inSuperElement);

	// Change superelement.

	mSuperElement = inSuperElement;
	
	// Update enabled flag.
	
	if ((mSuperElement == nil) || (mSuperElement->IsEnabled())) {
	
		// No super element or super is enabled, we can be enabled.
		
		if (mEnabled == triState_Latent)
			SuperEnable(mSuperElement);
	
	}
	else {
	
		// New super element is disabled, we must not be enabled.
		
		if (mEnabled == triState_On)
			SuperDisable(mSuperElement);
	
	}
	
}


// ---------------------------------------------------------------------------
//		* SetDeleted
// ---------------------------------------------------------------------------
//	Notification that this object is considered deleted (or undeleted) from
//	the data model. This is an override hook.

void
DMElement::SetDeleted(
	Boolean		/* inDeleted */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* IsSubElementOf
// ---------------------------------------------------------------------------
//	Returns true if this element is a subelement of inSuperElement.

Boolean
DMElement::IsSubElementOf(
	const DMElement*	inSuperElement) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSuperElement);
	
	// Walk up the data model hierarchy. See if we find
	// the potential super element along the way.
	
	const DMElement* super = this;
	while (super != nil) {
		ValidateObject_(super);
		if (inSuperElement == super)
			return true;
		super = super->GetSuperElement();
	}
	
	return false;
}


// ---------------------------------------------------------------------------
//		* GetAffectedObject
// ---------------------------------------------------------------------------
//	This method can be used to find the object affected by a change.
//	DMElement just passes it up the element hierarchy. DMObject
//	overrides it to "return this".

DMObject*
DMElement::GetAffectedObject() const
{

	// Validate pointers.

	ValidateThis_();

	// Search up data model hierarchy until a DMObject is found.

	if (mSuperElement != nil) {
		ValidateObject_((const_cast<DMElement*>(this))->mSuperElement);
		return mSuperElement->GetAffectedObject();
	}
	else
		return nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* FindAttributeByKey
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its key. Since an element is not an attribute, returns nil.

DMAttribute*
DMElement::FindAttributeByKey(
	DM_AttributeKey		/* inKey */) const
{
	return nil;
}


// ---------------------------------------------------------------------------
//		* FindAttributeByTitle
// ---------------------------------------------------------------------------
//	Like PowerPlant's FindPaneByID, can be used to find an attribute
//	by its title. Since an element is not an attribute, returns nil.

DMAttribute*
DMElement::FindAttributeByTitle(
	ConstStringPtr	/* inTitle */) const
{
	return nil;
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications/requests

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
DMElement::GoingAway()
{

	// Validate pointers.

	ValidateThis_();
	
	// Prevent this object from being disposed during method.

	StSharer lockObj(this);

	// Inform listeners that this element is being deleted.	

	BroadcastMessage(Element_GoingAway, this);
	
	// Inform subclass of deletion.
	
	GoingAwaySelf();

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
DMElement::CollectChanges()
{
	
	// Validate pointers.

	ValidateThis_();

	// Prevent this object from being disposed during method.

	StSharer lockObj(this);
	
	// Ask any listeners to collect changes.

	BroadcastMessage(Element_CollectChanges, this);

	// Collect any changes that may be hidden by object.

	CollectChangesSelf();

}


// ===========================================================================

#pragma mark -
#pragma mark ** user interface accessor

// ---------------------------------------------------------------------------
//		* FindUserInterfaceObject
// ---------------------------------------------------------------------------
//	This method can be used to find a user interface object which is
//	portraying this element. Call with a identifier code (which should be
//	be provided by the UI object's class header). This method will return
//	a void* pointer which you may then cast to the type of the object which
//	you were expecting. Returns nil if no UI object was found.

void*
DMElement::FindUserInterfaceObject(
	FourCharCode	inObjectIdentifier) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Send out a message asking UI objects to identify themselves.

	DM_FindUIObject findMsg = { this, inObjectIdentifier, nil };
	(const_cast<DMElement*>(this))->BroadcastMessage(Element_FindUIObject, &findMsg);

	// Return a pointer to the last UI object to identify itself.

	return findMsg.mObjectPointer;

}


// ===========================================================================

#pragma mark -
#pragma mark ** modified flag accessors

// ---------------------------------------------------------------------------
//		* IsModified
// ---------------------------------------------------------------------------
//	Returns true if this element or any subelement has been modified.

Boolean
DMElement::IsModified() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return modified or submodified flag.
	
	return mModified || mSubModified;

}


// ---------------------------------------------------------------------------
//		* IsSubModified
// ---------------------------------------------------------------------------
//	Returns true if any subelement has been modified. The "inTentative"
//	parameter checks to see if there are any pending changes which haven't
//	been recorded yet (appropriate for determining if the Save command
//	should be enabled).

Boolean
DMElement::IsSubModified(
	Boolean		inTentative) const
{

	// Validate pointers.

	ValidateThis_();

	// Prevent this object from being disposed during method.

	StSharer lockObj((const_cast<DMElement*>(this)));

	// If we already know we're modified, just say so.
	
	if (mModified || mSubModified)
		return true;
	
	// Ask listeners for potential changes.

	if (inTentative) {
		DM_CollectTentativeChanges chgMsg = { (const_cast<DMElement*>(this)), false };

		(const_cast<DMElement*>(this))->
				BroadcastMessage(Element_CollectTentativeChanges, &chgMsg);

		return chgMsg.mIsChanged;
	}

	// Nope. We're not modified.

	return false;

}


// ---------------------------------------------------------------------------
//		* MakeModified
// ---------------------------------------------------------------------------
//	Should be called whenever any element is modified by user actions.
//	Sets the modified flag of this element and the sub-element modified
//	flag of all of its super elements.

void
DMElement::MakeModified()
{

	// Validate pointers.

	ValidateThis_();
	
	// Prevent this object from being disposed during method.

	StSharer lockObj(this);

	// If modified flag is off, turn it on now.

	if (!mModified) {

		// Set the modified flag.
	
		mModified = true;
	
		// If there is a transaction active, make sure it knows
		// about the mod-flag change.
	
		StDMTransactionBuilder* builder = StDMTransactionBuilder::GetSubActionHost();
		if (builder != nil)
			builder->PostSubAction(new DMModFlagSubAction(this, true, false));
		
		// Send modified flag changed messge.
		
		SendUpdateMessage(Element_ModifiedFlagChanged, this);
		
		// Make sure super element's sub modified flag gets set also.
		
		if (mSuperElement != nil) {
			ValidateObject_(mSuperElement);
			mSuperElement->SubModified();
		}
	}
}


// ---------------------------------------------------------------------------
//		* ResetModified
// ---------------------------------------------------------------------------
//	Call this method after a file has been successfully saved. Clears the
//	mModified and mSubModified flag of this element and all subelements.

void
DMElement::ResetModified()
{

	// Validate pointers.

	ValidateThis_();
	
	// If modified flag is set, reset it now.
	
	if (mModified || mSubModified) {
	
		// Clear flag.
		
		mModified = mSubModified = false;
		
		// Send modified flag changed messge.
		
		SendUpdateMessage(Element_ModifiedFlagChanged, this);
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** enabled flag accessors

// ---------------------------------------------------------------------------
//		* CanDeleteThis
// ---------------------------------------------------------------------------
//	Return true if it is permissible to delete this element.

Boolean
DMElement::CanDeleteThis() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Deletion is usually controlled by the enabled flag.
	// Some classes may override this behavior.
	
	return IsEnabled();

}


// ---------------------------------------------------------------------------
//		* IsEnabled
// ---------------------------------------------------------------------------
//	Return true if this element can be edited.

Boolean
DMElement::IsEnabled() const
{

	// Validate pointers.

	ValidateThis_();
	
	// If memory is low, all elements are disabled.
	
	if (UMemoryUtils::MemoryIsLow())
		return false;
	
	// Return enabled flag.
	
	return mEnabled == triState_On;

}


// ---------------------------------------------------------------------------
//		* Enable
// ---------------------------------------------------------------------------
//	Make this element enabled (i.e. eligible for editing). Enabling works
//	like LPane's enabled flag: if the superelement is disabled, this element
//	is "latent" enabled.

void
DMElement::Enable()
{

	// Validate pointers.

	ValidateThis_();
	
	// Enable only has an affect if we're disabled.

	if (mEnabled == triState_Off) {
	
		// Enabled is a hierarchical property, meaning that an element
		// can't be enabled unless its super element is enabled.
		// If there is no super element, it's OK to enable this directly.
	
		Boolean canEnable = false;
		if (mSuperElement == nil)
			canEnable = true;
		else {
			ValidateObject_(mSuperElement);
			canEnable = mSuperElement->IsEnabled();
		}
		
		if (canEnable) {
			mEnabled = triState_On;
			EnableSelf();
			SendEnabledChangeMessage();
		}		
		else
			mEnabled = triState_Latent;

	}
}


// ---------------------------------------------------------------------------
//		* Disable
// ---------------------------------------------------------------------------
//	Make this element disabled (i.e. ineligible for editing).
//	If this element has subelements, they are also marked as disabled.

void
DMElement::Disable()
{

	// Validate pointers.

	ValidateThis_();

	// Check state of Enabled flag, change it to off, then call
	// DisableSelf() if the flag was on originally. We change
	// the flag before calling DisableSelf() in case any code called
	// by that function checks the state of the flag.

	Boolean wasEnabled = (mEnabled == triState_On);
	mEnabled = triState_Off;
	if (wasEnabled) {
		DisableSelf();
		SendEnabledChangeMessage();
	}
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
DMElement::CanHostElement(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{
	
	// Validate pointers.

	ValidateThis_();

	// Hosting is only possible when the element is enabled.
	// CanHostElementSelf is an override hook which should be
	// used to enforce any other constraints.

	if (IsEnabled())
		return CanHostElementSelf(inNewElement, ioPlacement);
	else
		return false;

}


// ---------------------------------------------------------------------------
//		* HostElement
// ---------------------------------------------------------------------------
//	The element "inNewElement" has been dropped or pasted into this
//	element. Make any changes necessary to accomodate it.
//
//	In general, subclasses should override HostElementSelf.

void
DMElement::HostElement(
	DMElement*				inNewElement,
	const DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.

	ValidateThis_();

	// Prevent this object from being disposed during method.

	StSharer lockObj(this);

	// Hosting is only possible when the element is enabled.
	// HostElementSelf is an override hook which should be
	// used to implement the hosting behavior.

	if (IsEnabled()) {
		if (ioPlacement->mHostElement == this)
			HostElementSelf(inNewElement, ioPlacement);
		else
			SignalCStr_("HostElement message sent to wrong object");
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update requests from parent

// ---------------------------------------------------------------------------
//		* SuperGoingAway										[protected]
// ---------------------------------------------------------------------------
//	An element which refers to this element is going away. Verify that
//	that element is in fact our super element. If so, broadcast the going
//	away message for this element as well.

void
DMElement::SuperGoingAway(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Relay going away message only if it's from our superelement.

	if (inSuperElement == mSuperElement)
		GoingAway();

}


// ---------------------------------------------------------------------------
//		* SuperCollectChanges									[protected]
// ---------------------------------------------------------------------------
//	An element that refers to this element has received the collect changes
//	message. If that element is our super element, broadcast the collect
//	changes message also.

void
DMElement::SuperCollectChanges(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Collect changes only if it's from our superelement.

	if (inSuperElement == mSuperElement)
		CollectChanges();

}


// ---------------------------------------------------------------------------
//		* SuperResetModified									[protected]
// ---------------------------------------------------------------------------
//	An element that refers to this element has been told to reset its
//	modified flags. If that element is our super element, reset our
//	modified flag as well.

void
DMElement::SuperResetModified(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Reset modified flag only if it's from our superelement.

	if (inSuperElement == mSuperElement)
		ResetModified();
}


// ---------------------------------------------------------------------------
//		* SuperDeleted											[protected]
// ---------------------------------------------------------------------------
//	An element that refers to this element has been deleted. If it is
//	our super element, remove the reference to that element. We now
//	have no super element.

void
DMElement::SuperDeleted(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Clear super element pointer only if it's from our superelement.

	if (inSuperElement == mSuperElement)
		SetSuperElement(nil);
}


// ---------------------------------------------------------------------------
//		* SuperEnable											[protected]
// ---------------------------------------------------------------------------
//	An element that refers to this element has been enabled. If it is
//	our super element, enable this element.

void
DMElement::SuperEnable(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Enable self only if it's from our superelement.

	if (inSuperElement == mSuperElement) {

		// If an element is enabled, disabling its SuperView makes the Pane
		// latently enabled. We call DisableSelf() since a latently
		// enabled Pane is visually disabled.
		
		if (mEnabled == triState_Latent) {
			mEnabled = triState_On;
			EnableSelf();
			SendEnabledChangeMessage();
		}
	}

}


// ---------------------------------------------------------------------------
//		* SuperDisable											[protected]
// ---------------------------------------------------------------------------
//	An element that refers to this element has been disabled. If it is
//	our super element, disable this element.

void
DMElement::SuperDisable(
	DMElement*	inSuperElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Disable self only if it's from our superelement.

	if (inSuperElement == mSuperElement) {

		// If an element is enabled, disabling its super element
		// makes the element latently enabled. We call DisableSelf()
		// since a latently enabled element is visually disabled.
		
		if (mEnabled == triState_On) {
			mEnabled = triState_Latent;
			DisableSelf();
			SendEnabledChangeMessage();
		}
	}
}


// ---------------------------------------------------------------------------
//		* SuperCanHostElement									[protected]
// ---------------------------------------------------------------------------
//	If a super element cannot host a new element, it will ask its subelements
//	(typically attributes) if they can host it.

Boolean
DMElement::SuperCanHostElement(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inNewElement);

	// Use the normal CanHostElement method.

	return CanHostElement(inNewElement, ioPlacement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** modified flag implementation

// ---------------------------------------------------------------------------
//		* SubModified											[protected]
// ---------------------------------------------------------------------------
//	Called when a sub element has become modified. Causes this element
//	to set its "sub element modified" flag. (This technique is used so that
//	we can quickly determine whether the file needs to be saved or not.)

void
DMElement::SubModified()
{

	// Validate pointers.

	ValidateThis_();
	
	// Prevent this object from being disposed during method.

	StSharer lockObj(this);

	// If the sub-modified flag is off, turn it on now.

	if (!mSubModified) {

		// Set the sub object modified flag.
	
		mSubModified = true;
	
		// If there is a transaction active, make sure it knows about the mod-flag change.
	
		StDMTransactionBuilder* builder = StDMTransactionBuilder::GetSubActionHost();
		if (builder != nil)
			builder->PostSubAction(new DMModFlagSubAction(this, false, true));
		
		// Send modified flag changed messge.
		
		SendUpdateMessage(Element_ModifiedFlagChanged, this);
		
		// Keep propogating the change up the tree.
		
		if (mSuperElement != nil) {
			ValidateObject_(mSuperElement);
			mSuperElement->SubModified();
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** enabled flag implementation

// ---------------------------------------------------------------------------
//		* EnableSelf											[protected]
// ---------------------------------------------------------------------------
//	Override hook. This element has become enabled.

void
DMElement::EnableSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Override hook. This element has become disabled.

void
DMElement::DisableSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* SendEnabledChangeMessage								[protected]
// ---------------------------------------------------------------------------
//	Broadcast a message to signal that this element's enabled flag has
//	changed.

void
DMElement::SendEnabledChangeMessage()
{

	// Validate pointers.

	ValidateThis_();
	
	// Broadcast change message.

	SendUpdateMessage(Element_EnabledChange, this, this);

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notification

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Common code to broadcast an update message and trigger the broadcast
//	from parent objects as well.

void
DMElement::SendUpdateMessage(
	MessageT		inMessage,
	void*			ioParam,
	DMElement*		inUpdatedElement)
{

	// Validate pointers.

	ValidateThis_();
	
	// Prevent this object from being disposed during method.

	StSharer lockObj(this);

	// Fill in updated element. Nil is a signal that we should
	// use this element.

	if (inUpdatedElement == nil)
		inUpdatedElement = this;
	
	// Send the change message.
	
	BroadcastMessage(inMessage, ioParam);

	// Ask superelement to relay the same message.

	if (mSuperElement != nil) {
		ValidateObject_(mSuperElement);
		mSuperElement->SendUpdateMessage(inMessage, ioParam, inUpdatedElement);
	}

}


// ===========================================================================

#pragma mark -
#pragma mark ** override hooks for update notifications/requests

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook to be called when the element has been placed in
//	the object hierarchy.

void
DMElement::FinishCreateSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* GoingAwaySelf											[protected]
// ---------------------------------------------------------------------------
//	Override hook to be called when the element is to be removed from
//	the visible data model. (This does not necessarily correspond to the
//	element actually being deleted.)

void
DMElement::GoingAwaySelf()
{
	// override hook: default does nothing
}


// ---------------------------------------------------------------------------
//		* CollectChangesSelf									[protected]
// ---------------------------------------------------------------------------
//	Override hook to be called when a file is about to be saved, or
//	pending changes need to be collected for any other reason.

void
DMElement::CollectChangesSelf()
{
	// override hook: default does nothing
}


// ---------------------------------------------------------------------------
//		* CanHostElementSelf									[protected]
// ---------------------------------------------------------------------------
//	Override hook. Return true if "inNewElement" can be made a part of
//	this element (i.e. is an acceptable subelement). The "ioPlacement" parameter
//	should be modified to reflect the preferred placement of the new element.

Boolean
DMElement::CanHostElementSelf(
	const DMElement*	/* inNewElement */,
	DM_DragPlacement*	/* ioPlacement */)
{
	return false;				// override hook
}


// ---------------------------------------------------------------------------
//		* HostElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook to be called when an element ("inNewElement") has
//	been dropped or pasted into this element. Make any changes necessary
//	to accomodate it.

void
DMElement::HostElementSelf(
	DMElement*			 	/* inNewElement */,
	const DM_DragPlacement*	/* ioPlacement */)
{
	SignalCStr_("DMElement::HostElementSelf needs to be overridden");
}


// ===========================================================================

#pragma mark -
#pragma mark ** common initialization

// ---------------------------------------------------------------------------
//		* InitElement											[private]
// ---------------------------------------------------------------------------
//	Common initialization used by the DMElement constructors.

void
DMElement::InitElement()
{
	mSuperElement = nil;
	mModified = false;
	mSubModified = false;
	mEnabled = triState_On;
}
