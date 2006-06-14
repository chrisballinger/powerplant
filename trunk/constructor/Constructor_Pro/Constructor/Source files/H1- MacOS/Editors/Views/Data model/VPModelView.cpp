// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelView.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/20/96
//     $Date: 2006/01/18 01:33:56 $
//  $History: VPModelView.cpp $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/17/96   Time: 16:27
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed some bugs in FinishCreate that caused duplicate command to
//	misbehave. (Bug fix #1055.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/08/96   Time: 15:08
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added FinishCreate method. Fixed a bug in SyncSubObjectList that caused
//	it to forget subobjects in some cases.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:09
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Changed InitModelView to use the new StKeepSharableAlive class.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:47
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added exception handling to InitModelView.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/25/96   Time: 00:38
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Changed subobject list to use the new VESubModelContainer.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/24/96   Time: 19:42
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added FrameSizeChanged method.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:24
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#include "VPModelView.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"
#include "StDMTransactionHider.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Editors : Views : Data model
#include "VESubModelContainer.h"

	// Core : Utilities
#include "StKeepSharableAlive.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPModelView ***

// ---------------------------------------------------------------------------
//		* VPModelView()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelView::VPModelView()
{
	InitModelView();
}


// ---------------------------------------------------------------------------
//		* VPModelView(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No extra stream data is read.

VPModelView::VPModelView(
	LStream*	inStream)

: VPModelPane(inStream)

{
	InitModelView();
}


// ---------------------------------------------------------------------------
//		* VPModelView(VPModelView&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPModelView::VPModelView(
	const VPModelView&	inOriginal)

: VPModelPane(inOriginal)

{
	InitModelView();
}


// ---------------------------------------------------------------------------
//		* ~VPModelView
// ---------------------------------------------------------------------------
//	Destructor

VPModelView::~VPModelView()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* GetSubObjectList
// ---------------------------------------------------------------------------
//	Return a pointer to the hybrid subobject list.

DMContainerAttribute*
VPModelView::GetSubObjectList() const
{
	return mSubObjects;
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Overridden to catch list-changed notifications.

void
VPModelView::SendUpdateMessage(
	MessageT	inMessage,
	void*		ioParam,
	DMElement*	inSubElement)
{

	// Validate pointers.
	
	ValidateThis_();

	// See what kind of message this is.

	if (inMessage == Container_ListChange) {
		
		// It's a list change message. See what it says.
		
		DM_ListChange changeMsg = *((DM_ListChange*) ioParam);
		if (!mHandlingUpdateMessage) {
			StValueChanger<Boolean> noUpdates(mHandlingUpdateMessage, true);
			SyncSubObjectList(changeMsg);
		}
	}
	
	// See if width or height are changing.
	
	if (inMessage == Attribute_ValueChange) {
	
		DM_ValueChange valueMsg = *((DM_ValueChange*) ioParam);
		ValidateObject_(valueMsg.mAttribute);
		
		switch (valueMsg.mAttribute->GetKey()) {
			
			case pPaneWidth:
			case pPaneHeight:
				FrameSizeChanged(valueMsg.mAttribute);
				break;
			
		}
	}

	// Relay the message as usual.

	VPModelPane::SendUpdateMessage(inMessage, ioParam, inSubElement);

}


// ---------------------------------------------------------------------------
//		* SyncSubObjectList										[protected]
// ---------------------------------------------------------------------------
//	Called by SendUpdateMessage whenever a list-changed message is about
//	to be sent. Make sure that the hybrid sub-object list is kept in sync
//	with the attachment and subpane lists.

void
VPModelView::SyncSubObjectList(
	DM_ListChange&	inChangeMessage)
{

	// Validate pointers.
	
	ValidateThis_();

	// See what list was affected.

	DMContainerAttribute* container = (dynamic_cast<DMContainerAttribute*>(inChangeMessage.mContainer));
	if (container == nil)
		return;
	
	ValidateObject_(container);
	
	if (container->GetAffectedObject() != this)
		return;
		
	DMElement* element = inChangeMessage.mMovedElement;
	ValidateObject_(element);
	
	// Which list changed?

	DMContainerAttribute* syncList = nil;
	Boolean syncBeSuperElement = false;
	SInt32 syncIndex = container->GetSubElements().FetchIndexOf(&element);
	
	switch (container->GetKey()) {
	
		case pPaneAttachmentsList:
			syncList = mSubObjects;
			break;

		case pPaneSubViewList:
			syncList = mSubObjects;
			
			DMContainerAttribute* attachments = FindContainerAttribute(pPaneAttachmentsList);
			ValidateObject_(attachments);
			
			syncIndex += attachments->GetSubElements().GetCount();
			break;

		case pPaneSubObjectList:
			if ((dynamic_cast<VPModelPane*>(element)) != nil) {
				syncList = FindContainerAttribute(pPaneSubViewList);
				
				DMContainerAttribute* attachments = FindContainerAttribute(pPaneAttachmentsList);
				ValidateObject_(attachments);
				
				syncIndex -= attachments->GetSubElements().GetCount();
			}
			else
				syncList = FindContainerAttribute(pPaneAttachmentsList);

			syncBeSuperElement = true;
			break;

		default:
			return;
			
	}

	// Update the other list accordingly.
	
	if (syncList != nil) {
	
		// Validate "other list" pointer.
	
		ValidateObject_(syncList);
		
		// Hide this transaction detail.
		
		StDMTransactionHider hideThis;
		
		// Insert or remove the item in the other list as requested.
		
		if (inChangeMessage.mElementAdded)
			syncList->InsertElementAt(syncIndex, element, syncBeSuperElement);
		else
			syncList->RemoveElement(element);
	}
}


// ---------------------------------------------------------------------------
//		* FrameSizeChanged										[protected]
// ---------------------------------------------------------------------------
//	Called whenever the width or height attribute changes value.
//	Asks the subpanes to adapt to this change in size.

void
VPModelView::FrameSizeChanged(
	DMAttribute*	inAttribute)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inAttribute);
	
	// Cast the attribute to integer attribute.
	
	DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(inAttribute));
	ValidateObject_(intAttr);

	// Cache the new size.
	
	SDimension32 oldSize = mFrameSize;
	
	switch (inAttribute->GetKey()) {
	
		case pPaneWidth:
			mFrameSize.width = intAttr->GetIntValue();
			break;
		
		case pPaneHeight:
			mFrameSize.height = intAttr->GetIntValue();
			break;
		
		default:
			SignalCStr_("Unexpected attribute");
	
	}
	
	// If no pending transaction, stop now.
	
	if (StDMTransactionBuilder::GetSubActionHost() == nil)
		return;
	
	// Figure the delta between old and new.
	
	SInt32 widthDelta = 0;
	SInt32 heightDelta = 0;
	
	if ((oldSize.width != -1) && (mFrameSize.width != -1))
		widthDelta = mFrameSize.width - oldSize.width;
	
	if ((oldSize.height != -1) && (mFrameSize.height != -1))
		heightDelta = mFrameSize.height - oldSize.height;
	
	// Get subpanes to adapt to this change in size.
	
	DMContainerAttribute* subPanes = GetSubPaneList();
	ValidateObject_(subPanes);
	
	DMFastIterator iter(subPanes->GetSubElements());
	while (iter.NextElement()) {
		VPModelObject* object = (dynamic_cast<VPModelObject*>(iter.CurrentElement()));
		ValidateObject_(object);
		object->AdaptToSuperFrameSize(widthDelta, heightDelta);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** subobject list helper

// ---------------------------------------------------------------------------
//		* FinishCreate
// ---------------------------------------------------------------------------

void
VPModelView::FinishCreate()
{

	// Validate pointers.

	ValidateThis_();
	
	// Do standard initialization.

	VPModelPane::FinishCreate();
	
	// See if there was an extra mosh subobject list created.
	// If so, destroy it.
	
	DMIterator iter(mSubElements);
	while (iter.NextElement()) {
	
		DMAttribute* attr = (dynamic_cast<DMAttribute*>(iter.CurrentElement()));
		ValidateObject_(attr);
		
		if (attr->GetKey() == pPaneSubObjectList) {
			if (attr != mSubObjects.GetObject())
				mSubElements.RemoveItem(attr);
		}
	}
	
	// Make sure the remaining subobject list is correctly formed.
	
	ValidateObject_(mSubObjects.GetObject());
	
	StKeepSharableAlive keepIt(this);
	
	DMContainerAttribute* attachments =
				(dynamic_cast<DMContainerAttribute*>(FindAttributeByKey(pPaneAttachmentsList)));
	DMContainerAttribute* subPanes =
				(dynamic_cast<DMContainerAttribute*>(FindAttributeByKey(pPaneSubViewList)));

	if ((subPanes != nil) && (attachments != nil)) {

		ValidateObject_(attachments);
		ValidateObject_(subPanes);
	
		StDMTransactionHider hideThis;
		StValueChanger<Boolean> noUpdates(mHandlingUpdateMessage, true);
	
		(const_cast<LSharableArray&>(mSubObjects->GetSubElements())).
				RemoveItemsAt(mSubObjects->GetSubElements().GetCount(), 1);
		
		DMFastIterator attachIter(attachments->GetSubElements());
		while (attachIter.NextElement()) {
			DMElement* element = attachIter.CurrentElement();
			mSubObjects->InsertElementAt(LArray::index_Last, element, false);
		}
	
		DMFastIterator subPaneIter(subPanes->GetSubElements());
		while (subPaneIter.NextElement()) {
			DMElement* element = subPaneIter.CurrentElement();
			mSubObjects->InsertElementAt(LArray::index_Last, element, false);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* InitModelView											[private]
// ---------------------------------------------------------------------------
//	Common initialization used by the VPModelView constructors.

void
VPModelView::InitModelView()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Reset flags.
	
	mHandlingUpdateMessage = false;
	
	mFrameSize.width = -1;
	mFrameSize.height = -1;
	
	// Manually twiddle the use count field so we don't
	// get zapped by InsertItemAt.

	StKeepSharableAlive keepThis(this);

	// Create a fake attribute that will hold the combined
	// attributes and subpanes lists.

	DMContainerAttributePtr attr;
	attr = (dynamic_cast<DMContainerAttribute*>(FindAttributeByKey(pPaneSubObjectList)));
	
	if (attr == nil) {
		attr = new VESubModelContainer;
		ValidateObject_(attr.GetObject());
		attr->SetKey(pPaneSubObjectList);
		attr->SetIncludeInDisplay(false);
		attr->SetSubObjectList(true);
		attr->SetMemberClassID(VPModelObject::class_ID);
		attr->SetCopySubsOnClone(false);
		InsertElementAt(LArray::index_Last, attr, true);
	}

	mSubObjects = attr;

}
