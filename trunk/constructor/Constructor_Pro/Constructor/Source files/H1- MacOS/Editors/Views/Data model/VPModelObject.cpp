// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelObject.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/14/96
//	   $Date: 2006/01/18 01:33:54 $
//	$History: VPModelObject.cpp $
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 12/22/96   Time: 14:19
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed a bug in DoSanityCheck that caused some resources to be
//	unopenable. (Bug fix #1165.)
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:27
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added ReadStreamData override.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/06/96   Time: 17:59
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetDisplayableObjectType method.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/06/96   Time: 16:34
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added BashNegativeSize method. Updated MoveBy and ResizeBy to call it.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/06/96   Time: 15:12
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Updated BashPaneID to use new exception classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/25/96   Time: 15:20
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetDescriptor method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/24/96   Time: 19:39
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Finished AdaptToSuperFrameSize.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/22/96   Time: 15:59
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added DoSanityCheck, DoSanityCheckSelf, and BashSubPaneID methods.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:58
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetPrintableID and IsSizeAttribute methods.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#include "VPModelObject.h"

	// Data model : Utilties
#include "DMIterator.h"

	// Editors : Views : Generic view editor : User interfaces
#include "VEDrawingAgent.h"

	// Editors : Views : PowerPlant : Data model
#include "VPDataModel.h"
#include "VPDuplicatePaneIDException.h"
#include "VPTypeEntry.h"
#include "VPWrongClassIDException.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_ClassAliasStrings	= 3005;
const SInt16	str_LeftParen			= 1;
const SInt16	str_RightParen			= 2;


// ===========================================================================

#pragma mark *** VPModelObject ***

// ---------------------------------------------------------------------------
//		* VPModelObject()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelObject::VPModelObject()
{
	mReadingStreamData = false;
}


// ---------------------------------------------------------------------------
//		* VPModelObject(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional data is read.

VPModelObject::VPModelObject(
	LStream*	inStream)

: VEModelObject(inStream)

{
	mReadingStreamData = false;
}


// ---------------------------------------------------------------------------
//		* VPModelObject(VPModelObject&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPModelObject::VPModelObject(
	const VPModelObject&	inOriginal)

: VEModelObject(inOriginal)

{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&inOriginal);
	
	// Copy type entry reference.

	mVPTypeEntry = inOriginal.mVPTypeEntry;

	// Clear reading stream data flag.

	mReadingStreamData = false;

}


// ---------------------------------------------------------------------------
//		* ~VPModelObject
// ---------------------------------------------------------------------------
//	Destructor

VPModelObject::~VPModelObject()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** pane ID utilities

// ---------------------------------------------------------------------------
//		* GetPaneID
// ---------------------------------------------------------------------------
//	Return the object's PowerPlant pane ID (if any).

SInt32
VPModelObject::GetPaneID() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for pane ID attribute. If we find it, return its value.
	// Otherwise return zero as a default.

	DMIntegerAttribute* attr = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneID)));
	if (attr != nil)
		return attr->GetIntValue();
	else
		return 0;

}


// ---------------------------------------------------------------------------
//		* FindPaneByID
// ---------------------------------------------------------------------------
//	Find the pane which has the specified ID. Searches this object, then
//	searches all panes contained within this object, not just direct
//	subpanes. Returns nil if no pane with the target ID is found.

VPModelObject*
VPModelObject::FindPaneByID(
	PaneIDT		inPaneID) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// First, check to see if the pane ID matches this object.

	if (GetPaneID() == inPaneID)
		return (const_cast<VPModelObject*>(this));

	// It didn't. Check subpane list.

	DMContainerAttribute* subPanes = GetSubPaneList();
	if (subPanes != nil) {

		// Make sure subpane pointer is valid.

		ValidateObject_(subPanes);
		
		// Iterate through subpanes.
	
		DMFastIterator iter(subPanes->GetSubElements());
		while (iter.NextElement()) {
			VPModelObject* elementVP = (dynamic_cast<VPModelObject*>(iter.CurrentElement()));
			if (elementVP != nil) {
				ValidateObject_(elementVP);
				VPModelObject* foundObj = elementVP->FindPaneByID(inPaneID);
				if (foundObj != nil)
					return foundObj;
			}
		}
	}

	// No such pane found.
	
	return nil;

}


// ---------------------------------------------------------------------------
//		* FindPaneByIDBackwards
// ---------------------------------------------------------------------------
//	Find the pane which has the specified ID. Searches in reverse order:
//	i.e. through the subpane list backwards, then this object. Searches
//	all panes contained within this object, not just direct subpanes.
//	Returns nil if no pane with the target ID is found.

VPModelObject*
VPModelObject::FindPaneByIDBackwards(
	PaneIDT		inPaneID) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// First check subpanes (in reverse order).

	DMContainerAttribute* subPanes = GetSubPaneList();
	if (subPanes != nil) {
	
		// Validate pointer to subpanes container.
	
		ValidateObject_(subPanes);
		
		// Iterate backwards through subpanes.
		
		DMFastIterator iter(subPanes->GetSubElements(), LArrayIterator::from_End);
		while (iter.PreviousElement()) {
			VPModelObject* elementVP = (dynamic_cast<VPModelObject*>(iter.CurrentElement()));
			if (elementVP != nil) {
				ValidateObject_(elementVP);
				VPModelObject* foundObj = elementVP->FindPaneByIDBackwards(inPaneID);
				if (foundObj != nil)
					return foundObj;
			}
		}
	}
	
	// None of the subpanes matched the ID, see if this pane does.
	
	if (GetPaneID() == inPaneID)
		return (const_cast<VPModelObject*>(this));

	// Nope. There is no such pane.

	return nil;
	
}


// ---------------------------------------------------------------------------
//		* CheckForDuplicateID
// ---------------------------------------------------------------------------
//	Returns true if two or more panes have the same ID. Searches all panes
// 	contained within this object, not just direct subpanes.

Boolean
VPModelObject::CheckForDuplicateID(
	PaneIDT		inPaneID) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for pane ID forwards, then backwards.
	// If there is only one pane with this ID, then
	// the two calls should return the same result.

	VPModelObject* fromStart = FindPaneByID(inPaneID);
	VPModelObject* fromEnd = FindPaneByIDBackwards(inPaneID);

	return (fromStart != fromEnd);

}


// ---------------------------------------------------------------------------
//		* GetDescriptor											[protected]
// ---------------------------------------------------------------------------
//	Return the object's title if one exists.

void
VPModelObject::GetDescriptor(
	LStr255&	outTitle) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for pane descriptor attribute. If we find it, return its value.
	// Otherwise return empty string as a default.

	DMAttribute* attr = FindAttributeByKey(pPaneDescriptor);
	if (attr != nil)
		attr->GetTextValue(outTitle);
	else
		outTitle[0] = 0;

}


// ---------------------------------------------------------------------------
//		* GetPrintableID										[protected]
// ---------------------------------------------------------------------------
//	Return the object's ID in a format suitable for printing. In this case,
//	we return the pane ID which can be interpreted as either text or numeric
//	values.

void
VPModelObject::GetPrintableID(
	LStr255&	outIDString) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for component name attribute.

	DMAttribute* idAttr = FindAttributeByKey(pPaneID);
	if (idAttr != nil)
		idAttr->GetTextValue(outIDString);
	else
		outIDString[0] = 0;

}


// ===========================================================================

#pragma mark -
#pragma mark ** subobject list

// ---------------------------------------------------------------------------
//		* GetSubPaneList
// ---------------------------------------------------------------------------
//	If there is a subpane list for this object, return it.

DMContainerAttribute*
VPModelObject::GetSubPaneList() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for a subpane list. It's OK if we don't have one,
	// so we don't validate this pointer.

	return (dynamic_cast<DMContainerAttribute*>(FindAttributeByKey(pPaneSubViewList)));

}


// ===========================================================================

#pragma mark -
#pragma mark ** class information accessors

// ---------------------------------------------------------------------------
//		* SetTypeEntry
// ---------------------------------------------------------------------------
//	Tell this object which type entry describes its view class.

void
VPModelObject::SetTypeEntry(
	VETypeEntry*	inTypeEntry)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inTypeEntry);
	
	// Do inherited method.

	VEModelObject::SetTypeEntry(inTypeEntry);

	// Make sure we got a VPTypeEntry.

	mVPTypeEntry = (dynamic_cast<VPTypeEntry*>(inTypeEntry));
	ValidateObject_(mVPTypeEntry.GetObject());

}


// ---------------------------------------------------------------------------
//		* GetDisplayableObjectType
// ---------------------------------------------------------------------------
//	Return a printable/displayable name for the object type. Overridden to
//	add the class alias if there is one.

void
VPModelObject::GetDisplayableObjectType(
	LStr255&	outTitle) const
{

	// Get the class name.

	VEModelObject::GetDisplayableObjectType(outTitle);
	
	// If there is a class alias, tack it on at the end of the name.

	VETypeEntry* paneType = GetTypeEntry();
	ValidateObject_(paneType);
	
	ClassIDT paneClass = paneType->GetClassID();
	ClassIDT classAlias = GetClassAlias();

	if ((classAlias != paneClass) && (classAlias != 0)) {

		outTitle += LStr255(STR_ClassAliasStrings, str_LeftParen) +
					LStr255((FourCharCode) classAlias) +
					LStr255(STR_ClassAliasStrings, str_RightParen);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** handy pane moving/sizing utilities

// ---------------------------------------------------------------------------
//		* MoveBy
// ---------------------------------------------------------------------------
//	Move the object by the designated number of pixels right and down.
//	(Use negative numbers to move left or up.)

void
VPModelObject::MoveBy(
	SInt32	inHorizDelta,
	SInt32	inVertDelta)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Clear negative sizes.
	
	BashNegativeSize();
	
	// Look for left & top attributes.

	DMIntegerAttribute* left = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneLeftOrigin)));
	DMIntegerAttribute* top = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneTopOrigin)));
	
	// If we have them, do the arithmetic.
	
	if (left != nil) {
		ValidateObject_(left);
		left->SetIntValue(left->GetIntValue() + inHorizDelta);
	}
	if (top != nil) {
		ValidateObject_(top);
		top->SetIntValue(top->GetIntValue() + inVertDelta);
	}
}


// ---------------------------------------------------------------------------
//		* ResizeBy
// ---------------------------------------------------------------------------
//	Resize the object (if it has a size) by the designated number of pixels.
//	This corresponds to the PowerPlant method ResizeFrameBy.

void
VPModelObject::ResizeBy(
	SInt32	inHorizDelta,
	SInt32	inVertDelta)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Clear negative sizes.
	
	BashNegativeSize();
	
	// Look for width & height attributes.

	DMIntegerAttribute* width = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneWidth)));
	DMIntegerAttribute* height = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneHeight)));

	// If we find them, do the arithmetic.
	
	if (width != nil) {
		ValidateObject_(width);
		width->SetIntValue(width->GetIntValue() + inHorizDelta);
	}
	if (height != nil) {
		ValidateObject_(height);
		height->SetIntValue(height->GetIntValue() + inVertDelta);
	}
}


// ---------------------------------------------------------------------------
//		* ResizeTo
// ---------------------------------------------------------------------------
//	Resize the object (if it has a size) to the designated number of pixels.
//	This corresponds to the PowerPlant method ResizeFrameTo.

void
VPModelObject::ResizeTo(
	SInt32	inHorizSize,
	SInt32	inVertSize)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for width and height attributes.

	DMIntegerAttribute* width = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneWidth)));
	DMIntegerAttribute* height = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneHeight)));

	// If we have them, resize the pane.
	
	if (width != nil) {
		ValidateObject_(width);
		width->SetIntValue(inHorizSize);
	}
	if (height != nil) {
		ValidateObject_(height);
		height->SetIntValue(inVertSize);
	}
}


// ---------------------------------------------------------------------------
//		* IsSizeAttribute										[protected]
// ---------------------------------------------------------------------------
//	Return true if the attribute key is either width or height.

Boolean
VPModelObject::IsSizeAttribute(
	FourCharCode	inAttributeKey) const
{
	switch (inAttributeKey) {
		case pPaneWidth:
		case pPaneHeight:
			return true;
		default:
			return false;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** data model consistency checks

// ---------------------------------------------------------------------------
//		* DoSanityCheck
// ---------------------------------------------------------------------------
//	Allow this object and all subobjects to make sure that pane IDs, etc.,
//	are valid.

void
VPModelObject::DoSanityCheck()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Do own sanity check.
	
	{
		StValueChanger<Boolean> noThrow(mReadingStreamData, true);
		DoSanityCheckSelf();
	}
	
	// Do sanity checks for subobjects (if there are any).
	
	DMContainerAttribute* subObjects = GetSubObjectList();
	if (subObjects != nil) {
	
		ValidateObject_(subObjects);
		
		DMIterator iter(const_cast<LSharableArray&>(subObjects->GetSubElements()));
		while (iter.NextElement()) {
			
			VPModelObject* subVP = (dynamic_cast<VPModelObject*>(iter.CurrentElement()));
			ValidateObject_(subVP);
			
			subVP->DoSanityCheck();
			
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamData
// ---------------------------------------------------------------------------
//	Overridden to prevent validation exceptions from being thrown while
//	building up the data model.

void
VPModelObject::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Remember that we're reading stream data.
	
	StValueChanger<Boolean> reading(mReadingStreamData, true);

	// Go ahead and read the data.

	VEModelObject::ReadStreamData(inStream, inStreamSelector);

}


// ===========================================================================

#pragma mark -
#pragma mark ** pane resizing helper

// ---------------------------------------------------------------------------
//		* AdaptToSuperFrameSize									[protected]
// ---------------------------------------------------------------------------
//	Called by VPModelView::FrameSizeChanged whenever the superview's
//	size changes.

void
VPModelObject::AdaptToSuperFrameSize(
	SInt32	/* inSurrWidthDelta */,
	SInt32	/* inSurrHeightDelta */)
{
	// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** data model consistency checks

// ---------------------------------------------------------------------------
//		* DoSanityCheckSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook. Make sure that pane IDs, etc., are valid.

void
VPModelObject::DoSanityCheckSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* BashSubPaneID											[protected]
// ---------------------------------------------------------------------------
//	Given an attribute key, make sure that attribute contains an ID that
//	points to a valid pane (i.e. the ID is not duplicated within the PPob
//	and is of the required class).

void
VPModelObject::BashSubPaneID(
	FourCharCode	inAttributeKey,
	ClassIDT		inRequiredSubClass)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find the pane ID attribute.
	
	DMIntegerAttribute* idAttr = FindIntegerAttribute(inAttributeKey);
	ValidateObject_(idAttr);
	
	// If ID is the magic value -1, ignore it.
	// Otherwise we could infinitely recurse.
	
	PaneIDT currentID = idAttr->GetIntValue();
	if (currentID == -1)
		return;
	
	// Look for the pane ID.
	
	VPModelObject* object = FindPaneByID(currentID);
	if (object == nil)
		return;
	
	// Look for duplicate ID.
	
	if (CheckForDuplicateID(currentID)) {
		if (mReadingStreamData)
			idAttr->SetIntValue(-1);
		else
			throw VPDuplicatePaneIDException(currentID);
	}
	
	// Look for an object that's not the right type.
	
	if (!(object->GetVPTypeEntry()->TypeDerivesFrom(inRequiredSubClass))) {
		if (mReadingStreamData)
			idAttr->SetIntValue(-1);
		else
			throw VPWrongClassIDException(idAttr, currentID, inRequiredSubClass);
	}
	
}


// ---------------------------------------------------------------------------
//		* BashNegativeSize										[protected]
// ---------------------------------------------------------------------------
//	Called by MoveBy and ResizeBy to eliminate negative width & height
//	values. (These are used by PowerPlant to signal "expand to fit.")

void
VPModelObject::BashNegativeSize()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// See if this object has negative size. In PowerPlant-speak,
	// this means the pane will be expanded to fit the object.
	
	DMIntegerAttribute* widthAttr = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneWidth)));
	DMIntegerAttribute* heightAttr = (dynamic_cast<DMIntegerAttribute*>(FindAttributeByKey(pPaneHeight)));
	
	if ((widthAttr == nil) || (heightAttr == nil))
		return;
	
	ValidateObject_(widthAttr);
	ValidateObject_(heightAttr);

	SInt32 width = widthAttr->GetIntValue();
	SInt32 height = heightAttr->GetIntValue();

	if ((width >= 0) && (height >= 0))
		return;

	// Okay, we have negative value. Look for drawing agent.
	
	VEDrawingAgent* agent = FindDrawingAgent();
	if (agent == nil)
		return;
	
	ValidateObject_(agent);
	
	Rect frame;
	if (!agent->CalcPortFrameRect(frame))
		return;
	
	if (width < 0)
		widthAttr->SetIntValue(frame.right - frame.left);
	
	if (height < 0)
		heightAttr->SetIntValue(frame.bottom - frame.top);
	
}
