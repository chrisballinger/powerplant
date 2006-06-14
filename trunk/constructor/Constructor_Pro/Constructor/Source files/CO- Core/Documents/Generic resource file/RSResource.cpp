// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSResource.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/28/96
//	   $Date: 2006/01/18 01:33:01 $
//	$History: RSResource.cpp $
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 02/28/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Rolled in Clint's fixes for RM/RF classes.
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 02/18/97   Time: 18:09
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed some errors introduced by merging with 2.4.1.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:54
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Added ResourceDeleted method to keep links current when primary
//	resource changes.
//	
//	*****************  Version 11  *****************
//	User: Andrew       Date: 2/06/97    Time: 11:42a
//	Updated in $/ConstructorWin/Sources
//	Initial mods for compiling with MSVC
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/28/97   Time: 15:31
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed a problem with the "resource IDs significant" flag. (Bug fix
//	#1137.)
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/22/96   Time: 17:47
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Make sure wctb resources are always marked non-purgeable. (Bug fix
//	#1162.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/16/96   Time: 15:30
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Removed signal in PrimSetResName which was triggered by undoing a
//	create new resource. (Bug fix #1130.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/05/96   Time: 18:09
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Fixed a bug in GetResName which caused a Signal_ alert when the
//	resource had just been deleted.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/25/96   Time: 21:33
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Changed UpdateResIDData to call RESession::GetEditorForResource with
//	error dialogs turned off.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/06/96   Time: 13:50
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Fixed bugs in SetRFResource. Now functions properly if some attributes
//	don't exist (as in Java target).
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:56
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:55
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSResource.h"

	// Core : Documents : Generic resource file
#include "RSContainer.h"
#include "RSDuplicateResIDException.h"
#include "RSType.h"

	// Core : Editors : Generic editor
#include "REEditor.h"
#include "RESession.h"
#include "RETypeEntry.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// MacOS : Resource manager
#include "RFMap.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_TitleStrings		= 15200;
const SInt16	str_Space				= 1;
const SInt16	str_CommaLeftQuote		= 2;
const SInt16	str_RightQuote			= 3;


// ===========================================================================

#pragma mark *** RSResource ***

// ---------------------------------------------------------------------------
//		* RSResource(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

RSResource::RSResource(
	LStream*	inStream)

: PDResource(inStream)

{
	InitRSResource();
}


// ---------------------------------------------------------------------------
//		* RSResource(RSResource&)
// ---------------------------------------------------------------------------
//	Copy constructor

RSResource::RSResource(
	const RSResource& inOriginal)

: PDResource(inOriginal)

{
	InitRSResource();
}


// ---------------------------------------------------------------------------
//		* ~RSResource
// ---------------------------------------------------------------------------
//	Destructor

RSResource::~RSResource()
{

	// Validate pointers.

	ValidateThis_();
	
	// Release cached resource data (if any).

	if (mDeletedResData != nil) {
		ValidateHandle_(mDeletedResData);
		::DisposeHandle(mDeletedResData);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource info accessors

// ---------------------------------------------------------------------------
//		* GetResID
// ---------------------------------------------------------------------------
//	Return the resource ID of this resource. If possible, we get it from the
//	low-level resource map. Otherwise, we may have cached it while changing
//	IDs or if we were deleted.

ResIDT
RSResource::GetResID() const
{

	// Validate pointers.

	ValidateThis_();

	// See if we're in the midst of changing resource IDs.

	if (mChangingID) {
	
		// We are. Grab the ID from the data model. In some cases, there may
		// not be a resource ID attribute. If so, we're hosed. Just say "zero."
	
		DMAttribute* resID = FindAttributeByKey(attrKey_ResourceID);
		if (resID == nil)
			return 0;

		// Okay, there is an ID attribute. Grab its value.
		
		ValidateObject_(resID);
		
		DMIntegerAttribute* resIDInt = (dynamic_cast<DMIntegerAttribute*>(resID));
		ValidateObject_(resIDInt);
		return resIDInt->GetIntValue();

	}
	else if (mRMResource != nil) {
	
		// Not changing IDs now. Grab the resource ID from the low-level
		// resource map.

		RFResource *rfRes = GetRFResource();
		ValidateObject_(rfRes);
		return rfRes->GetResID();
		
	}
	else if (mDeletedResData != nil) {
	
		// This resource is currently deleted. Grab the resource ID from
		// the deleted resource data.
	
		ValidateHandle_(mDeletedResData);
		StHandleLocker lockData(mDeletedResData);

		// Ugly, fragile, but quick way of getting data.

		ResIDT* resIDPtr = *((ResIDT**) mDeletedResData);
		resIDPtr += 2;
		return *resIDPtr;

	}
	else {
	
		// As last resort, we're about to unpack resource data.
		// Return the resource ID that we'll have once that's done.
	
		return mUnpackResID;
	
	}
}


// ---------------------------------------------------------------------------
//		* GetResType
// ---------------------------------------------------------------------------
//	Return the ResType of this resource (or the primary resource). If possible,
//	we get it from the low-level resource map. Otherwise it comes from cached
//	resource data if deleted.

ResType
RSResource::GetResType() const
{

	// Validate pointers.

	ValidateThis_();

	// See if we have a resource map entry.

	if (mRMResource != nil) {
	
		// JCD 980629 - added this validation in hopes of squashing
		// a bug (MW06759).
		
		ValidateObject_(mRMResource);
		
		// We do, use it to get the resource type.
	
		RFResource *rfRes = GetRFResource();
		ValidateObject_(rfRes);
		return rfRes->GetResType();

	}
	else if (mDeletedResData != nil) {
	
		// No current map entry, try the deleted resource data.
		
		ValidateHandle_(mDeletedResData);
		StHandleLocker lockData(mDeletedResData);
		
		// Ugly, fragile, but quick way of getting data.
		
		ResType* resTypePtr = *((ResType**) mDeletedResData);
		return *resTypePtr;
		
	}
	else {
	
		// There really isn't any way of getting the res type now. Bummer.
	
		SignalCStr_("Can't get res type now.");
		return 0;
		
	}
}


// ---------------------------------------------------------------------------
//		* GetResAttrs
// ---------------------------------------------------------------------------
//	Return the resource attributes. Resource attributes are not available if
//	the resource is currently deleted.

SInt16
RSResource::GetResAttrs() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Get attributes from resource map entry only.

	if (mRMResource != nil) {
	
		RFResource *rfRes = GetRFResource();
		ValidateObject_(rfRes);
		return rfRes->GetResAttrs();
		
	}
	else {
	
		SignalCStr_("Can't get res attributes now.");
		return 0;

	}
}


// ---------------------------------------------------------------------------
//		* GetResTypeEntry
// ---------------------------------------------------------------------------
//	Return the resource type description object for this resource's type.

RETypeEntry*
RSResource::GetResTypeEntry() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Search the global type list for a description of this
	// resource type.

	RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(GetResType());
	ValidateObject_(typeEntry);
	
	return typeEntry;
	
}


// ---------------------------------------------------------------------------
//		* GetResName
// ---------------------------------------------------------------------------
//	Return the resource name. Resource name is not available if the resource
//	is currently deleted.

StringPtr
RSResource::GetResName(
	Str255	outResName) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Use low-level resource map to get resource name if possible.
	
	if (mRMResource != nil) {
		
		ValidateObject_(mRMResource);
		return mRMResource->GetResName(outResName);
		
	}
	else {
		LStr255 name;
		GetResourceName(name);
		return LString::CopyPStr(name, outResName);
	}
}


// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Return a name for this resource that can be used for
//	window titles and property inspector labels. The name is formed
//	from the resource type, ID (if appropriate), and name.

void
RSResource::GetDisplayableName(
	LStr255&	outTitle) const
{

	// Validate pointers.

	ValidateThis_();

	// Start with resource type description.
	
	RETypeEntry* typeEntry = GetResTypeEntry();
	ValidateObject_(typeEntry);

	typeEntry->GetResTypeName(outTitle);
	outTitle += LStr255(STR_TitleStrings, str_Space);

	// Add resource number, but only if IDs have meaning here.

	if (GetResIDsSignificant()) {
		LStr255 tempStr((SInt32) GetResID());
		outTitle += tempStr;
	}
	
	// Add resource name (if there is one).

	LStr255 tempStr;
	GetResName(tempStr);
	if (tempStr.Length()) {
		outTitle += LStr255(STR_TitleStrings, str_CommaLeftQuote);
		outTitle += tempStr;
		outTitle += LStr255(STR_TitleStrings, str_RightQuote);
	}
}


// ---------------------------------------------------------------------------
//		* GetRelatedResources
// ---------------------------------------------------------------------------
//	Fill the provided array with a list of related resources (if any).
//	The pointers are of type RFResource*.

void
RSResource::GetRelatedResources(
	LArray&		outRMResources) const
{

	// Validate pointers.
	
	ValidateThis_();

	// Find the resource type entry.
	
	RETypeEntry* typeEntry = GetResTypeEntry();
	ValidateObject_(typeEntry);
	
	// Find the resource map.
	
	RFMap* map = GetRFMap();
	ValidateObject_(map);

	// Iterate through related resource type list.
	// Find the resource that matches this resource type
	// and ID, and add it to the list.
	
	ResType resType;
	LFastArrayIterator iter(typeEntry->GetResTypes());

	while (iter.Next(&resType)) {

		// We have a primary resource type. Find the low-level
		// resource map entry for it.

		RMResource* theRsrc = map->FindResource(resType, GetResID(), false);
		if (theRsrc != nil) {
		
			// Okay, a resource was found. Make sure it's not the
			// primary resource. If it isn't, add it to the array.
		
			ValidateObject_(theRsrc);
			if (theRsrc != GetRMResource())
				outRMResources.AddItem(&theRsrc);

		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource map accessors

// ---------------------------------------------------------------------------
//		* GetRSContainer
// ---------------------------------------------------------------------------
//	Scan up through the data model looking for the resource container object.

RSContainer*
RSResource::GetRSContainer() const
{

	// Validate pointers.
	
	ValidateThis_();

	// Look for resource container by walking up the data model. This is ugly. *sigh*
	
	RSContainer* container = nil;
	DMElement* super = GetSuperElement();

	while (super != nil) {
		ValidateObject_(super);
		container = (dynamic_cast<RSContainer*>(super));
		if (container != nil)
			return container;
		super = super->GetSuperElement();
	}

	// Couldn't find container.
	
	return nil;

}

// ---------------------------------------------------------------------------
//		* GetRSType
// ---------------------------------------------------------------------------
//	Scan up through the data model looking for the resource type object.

RSType*
RSResource::GetRSType() const
{

	// Validate pointers.
	
	ValidateThis_();

	// Look for resource type by walking up the data model. This is ugly. *sigh*
	
	RSType* type = nil;
	DMElement* super = GetSuperElement();

	while (super != nil) {
		ValidateObject_(super);
		type = (dynamic_cast<RSType*>(super));
		if (type != nil)
			return type;
		super = super->GetSuperElement();
	}

	// Couldn't find type.
	
	return nil;

}


// ---------------------------------------------------------------------------
//		* GetRFMap
// ---------------------------------------------------------------------------
//	Return a pointer to the low-level resource map for this
//	resource (if any).

RFMap*
RSResource::GetRFMap() const
{

	// Validate pointers.

	ValidateThis_();
	
	// If we have a low-level resource, we can find the resource map.

	if (mRMResource != nil) {
		RFResource *rfRes = GetRFResource();
		ValidateObject_(rfRes);
		return &(rfRes->GetRFMap());
	}
	else
		return nil;
}


// ---------------------------------------------------------------------------
//		* GetRFType
// ---------------------------------------------------------------------------
//	Return a pointer to the low-level resource type entry for
//	this resource (if any).

RFType*
RSResource::GetRFType() const
{

	// Validate pointers.

	ValidateThis_();
	
	// If we have a low-level resource, we can find the resource type.

	if (mRMResource != nil) {
		RFResource *rfRes = GetRFResource();
		ValidateObject_(rfRes);
		return &(rfRes->GetRFType());
	}
	else
		return nil;
}


// ---------------------------------------------------------------------------
//		* SetRFResource
// ---------------------------------------------------------------------------
//	Attach this data model object to a low-level resource. Should be
//	called only once as the data model is being built. Syncs the data
//	model attributes with the low-level resource map.

void
RSResource::SetRMResource(
	RMResource*		inRMResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inRMResource);

	// Record the resource pointer.

	PDResource::SetRMResource(inRMResource);
	
	// Cast the resource to RFResource.
	
	mRMResource = (dynamic_cast<RFResource*>(inRMResource));
	ValidateObject_(mRMResource);

	// Sync up data model with resource map.
	// Note that some attributes may not exist, depending on
	// the target platform.
	
	RFResource *rfRes = GetRFResource();
	ValidateObject_(rfRes);

	if (FindAttributeByKey(attrKey_ResourceName) != nil) {
		LStr255 resName;
		rfRes->GetResName(resName);
		SetResourceName(resName);
	}

	if (FindAttributeByKey(attrKey_ResourceID) != nil)
		SetResourceID(rfRes->GetResID());

	if (FindAttributeByKey(attrKey_ResourceAttributes) != nil)
		SetResourceAttributes(rfRes->GetResAttrs());

	if (FindAttributeByKey(attrKey_ResourceType) != nil) {
		SetResourceType(rfRes->GetResType());
		GetResourceTypeAttribute()->Disable();
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource deletion

// ---------------------------------------------------------------------------
//		* UnpackResData
// ---------------------------------------------------------------------------
//	Convert resource data created by PackResData back into resource map
//	entries.

void
RSResource::UnpackResData()
{

	// Validate pointers.

	ValidateThis_();
	ValidateHandle_(mDeletedResData);
	SignalIf_(mRMResource != nil);

	// Find the resource map.

	RSContainer* container = GetRSContainer();
	ValidateObject_(container);
	
	RFMap* map = container->GetRFMap();
	ValidateObject_(map);
	
	// Okay. Grab the handle.

	ValidateHandle_(mDeletedResData);
	LHandleStream packData(mDeletedResData);
	mDeletedResData = nil;
	
	// Unpack resources as long as there's data.
	
	while (!packData.AtEnd()) {
		ResType type;
		SInt16 resID;
		LStr255 resName;
		SInt16 attrs;
		Handle data;
	
		packData	>> type
					>> resID
					>> ((StringPtr) resName)
					>> attrs
					>> data;
		
		if (data != nil)
			ValidateHandle_(data);
		
		if ((mRMResource == nil) && !mUnpackChangeID) {
			mUnpackResID = resID;
		}

		RFResource* rsrc = map->FindResource(type, mUnpackChangeID ? mUnpackResID : resID, true);
		if (rsrc == nil)
			continue;
		
		ValidateObject_(rsrc);

		if (mRMResource == nil)
			mRMResource = rsrc;
	
		rsrc->SetResName(resName);
		rsrc->SetResAttrs(attrs);

		if (data != nil)
			rsrc->SetResData(data);
		
		::DisposeHandle(data);

	}
	
	// Update resource data if ID is stored inside the resource.
	
	ResIDT newResID = GetResID();
	UpdateResIDData(newResID, newResID);
	
	// Reset unpack flags.
	
	mUnpackChangeID = false;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Intercept value change messages and ensure that the low-level resource
//	map follows these changes.

void
RSResource::SendUpdateMessage(
	MessageT		inMessage,
	void*			ioParam,
	DMElement*		inSubElement)
{

	// Validate pointers.

	ValidateThis_();

	// Intercept change messages from resource ID, attribute, and name.

	if (inMessage == Attribute_ValueChange) {
		
		// Make sure it's this object that's affected.
		
		DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
		if (valueMsg->mObject == this) {
		
			ValidateObject_(valueMsg->mAttribute);
		
			// It is. Check attribute key.
			
			switch (valueMsg->mAttribute->GetKey()) {

				case attrKey_ResourceName: {
					LStr255 newName;
					valueMsg->mAttribute->GetTextValue(newName);
					PrimSetResName(newName);
					break;
				}
				
				case attrKey_ResourceID: {
					DMIntegerAttribute* resIDAttr =
							(dynamic_cast<DMIntegerAttribute*>(valueMsg->mAttribute));

					if (resIDAttr != nil) {
						ValidateObject_(resIDAttr);
						SInt32 newResID = resIDAttr->GetIntValue();
						PrimSetResID(newResID);
					}
					break;
				}

				case attrKey_ResourceAttributes:
					PrimSetResAttrs(((DMBitfieldAttribute*) valueMsg->mAttribute)->GetIntValue());
					break;
			}
		}	
	}

	// Relay message as usual.

	DMObject::SendUpdateMessage(inMessage, ioParam, inSubElement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** low-level resource info setters

// ---------------------------------------------------------------------------
//		* PrimSetResID											[protected]
// ---------------------------------------------------------------------------
//	Change the resource ID of this resource. Since the low-level resource map
//	doesn't provide a SetResID method, we have to duplicate all related
//	resources and delete the originals.

void
RSResource::PrimSetResID(
	ResIDT	inResID)
{

	// Validate pointers.

	ValidateThis_();

	// Sanity check: Make sure we have a resource.
	
	if (mRMResource == nil) {
		mUnpackChangeID = true;
		mUnpackResID = inResID;
		return;
	}
	ValidateObject_(mRMResource);	

	if (inResID == GetResID())
		return;
		
	// Check to make sure that we aren't creating a duplicate ID.
	
	PreflightResID(inResID);
	
	RFResource *rfRes = GetRFResource();
	ValidateObject_(rfRes);

	// Save old resource ID.
	
	ResIDT oldResID = rfRes->GetResID();

	// Cache related resources.

	LArray relatedRsrcs(sizeof (RFResource*));
	GetRelatedResources(relatedRsrcs);

	// Duplicate primary resource.

	StValueChanger<Boolean> changeID(mChangingID, true);

	RFResource* dupRsrc = rfRes->DuplicateResource(GetResType(), inResID);
	ValidateObject_(dupRsrc);
	
	// Tell any resource editor we may have that the primary resource is changing.
	
	REEditor* editor = (REEditor*) FindUserInterfaceObject(REEditor::objID_Editor);
	if (editor != nil) {
	
		ValidateObject_(editor);
		if (editor->GetPrimaryResource() == mRMResource)
			editor->SetPrimaryResource(dupRsrc);
	
	}
	
	// Delete the old resource.
		
	mRMResource->DeleteResource();
	mRMResource = dupRsrc;
	
	// Duplicate related resources.

	LFastArrayIterator iter(relatedRsrcs);
	RFResource* rsrc;
	while (iter.Next(&rsrc)) {
		ValidateObject_(rsrc);
		rsrc->DuplicateResource(rsrc->GetResType(), inResID);
		rsrc->DeleteResource();
	}
	
	// Change resource data as appropriate.
	
	UpdateResIDData(oldResID, inResID);
	
}


// ---------------------------------------------------------------------------
//		* PrimSetResName										[protected]
// ---------------------------------------------------------------------------
//	Change the resource name for this and all related resources.

void
RSResource::PrimSetResName(
	ConstStringPtr	inResName)
{

	// Validate pointers.

	ValidateThis_();
	ThrowIfNil_(inResName);

	// Make sure we have a resource map entry. If not, ignore the request.

	if (mRMResource == nil)
		return;

	ValidateObject_(mRMResource);
	
	// Optimization: Don't change name if we're changing it to the same thing.
	
	LStr255 oldResName;
	mRMResource->GetResName(oldResName);
	if (oldResName.CompareTo(inResName) == 0)
		return;

	// Name is different. Set primary resource's name.
		
	mRMResource->SetResName(inResName);

	// Set resource name for related resources.
	
	LArray relatedRsrcs(sizeof (RFResource*));
	GetRelatedResources(relatedRsrcs);
	
	LFastArrayIterator iter(relatedRsrcs);
	RFResource* rsrc;
	while (iter.Next(&rsrc)) {
		ValidateObject_(rsrc);
		rsrc->SetResName(inResName);
	}
}


// ---------------------------------------------------------------------------
//		* PrimSetResAttrs										[protected]
// ---------------------------------------------------------------------------
//	Change the resource attributes for this and all related resources.

void
RSResource::PrimSetResAttrs(
	SInt32	inResAttrs)
{

	// Validate pointers.

	ValidateThis_();

	if (mRMResource == nil)
		return;
	ValidateObject_(mRMResource);	

	// Don't set attributes if they're already this value.

	if (inResAttrs == GetResAttrs())
		return;

	RFResource *rfRes = GetRFResource();
	ValidateObject_(rfRes);

	// Set resource attributes in primary resource.

	rfRes->SetResAttrs(inResAttrs);

	// Set resource attributes for related resources.

	LArray relatedRsrcs(sizeof (RFResource*));
	GetRelatedResources(relatedRsrcs);
	
	LFastArrayIterator iter(relatedRsrcs);
	RFResource* rsrc;
	while (iter.Next(&rsrc)) {
		ValidateObject_(rsrc);
		if (rsrc->GetResType() == 'wctb')		// ugly quick hack to prevent purgable wctbs
			rsrc->SetResAttrs(0);
		else
			rsrc->SetResAttrs(inResAttrs);
	}
}


// ---------------------------------------------------------------------------
//		* PreflightResID										[protected]
// ---------------------------------------------------------------------------
//	We're about to change a resource ID. Ensure that no existing resource
//	has the new ID. If there is a conflict, throw a validation exception to
//	prevent the change from occurring.

void
RSResource::PreflightResID(
	ResIDT&		inResID)
{

	// Validate input parameters.

	ValidateThis_();

	// If setting to our own ID, ignore check.
	
	if (inResID == GetResID())
		return;

	// Find the resource type entry.
	
	RSType* type = GetRSType();
	if (type == nil)
		return;
	ValidateObject_(type);
	
	// Found the type. Make sure there isn't already a
	// resource of this type and ID. If there is, throw
	// a validation exception.
	
	RSResource* oldRsrc = type->FindResource(GetResType(), inResID, false);
	if (oldRsrc != nil) {
		
		// We have a conflict. See what the appropriate response is.
		
		if (GetResIDsSignificant())
			throw RSDuplicateResIDException();
		else
			SignalCStr_("Setting resource IDs when ID isn't significant?");
	
	}
}


// ---------------------------------------------------------------------------
//		* UpdateResIDData										[protected]
// ---------------------------------------------------------------------------
//	The resource's ID has changed. If needed, update the resource data
//	to reflect the data. (Some resource formats, such as MENU require this
//	updating.)

void
RSResource::UpdateResIDData(
	ResIDT		inOldResID,
	ResIDT		inNewResID)
{

	// Validate pointers.

	ValidateThis_();
	
	// Find resource type entry.
	
	RETypeEntry* typeEntry = GetResTypeEntry();
	ValidateObject_(typeEntry);

	// Change resource data as appropriate.
	
	if (typeEntry->UpdateResDataOnIDChange()) {

		// Find data model container.
		
		RSContainer* container = GetRSContainer();
		ValidateObject_(container);

		// Find editor session.
	
		RESession* session = container->GetSession();
		ValidateObject_(session);

		TSharablePtr<REEditor> editor;
		editor = session->GetEditorForResource(GetResType(), GetResID(), true, true);
		if (editor != nil) {

			// Got the editor, ask it to update the resource ID.
			
			ValidateObject_(editor.GetObject());
			editor->UpdateResID(GetRFResource(), inOldResID, inNewResID);
		
		}
	}
}


// ---------------------------------------------------------------------------
//		* PackOneResData										[protected]
// ---------------------------------------------------------------------------
//	Called by PackResData to pack the resource data for a single resource.

void
RSResource::PackOneResData(
	LHandleStream&		inStream,
	const RMResource*	inResource) const
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inResource);
	
	// Cast this resource to RFResource.
	
	const RFResource* theResRF = dynamic_const_cast(inResource,RMResource*,RFResource*);
	ValidateObject_(theResRF);
	
	// Get resource data and attributes.

	ResType type = theResRF->GetResType();
	SInt16 resID = theResRF->GetResID();

	LStr255 resName;
	theResRF->GetResName(resName);

	SInt16 attrs = theResRF->GetResAttrs();
	Handle data = (const_cast<RFResource*>(theResRF))->GetResData();

	// Write this information to the stream.

	inStream	<< type
				<< resID
				<< ((StringPtr) resName)
				<< attrs
				<< data;

	// Release our copy of the resource data.
	
	::DisposeHandle(data);

}


// ---------------------------------------------------------------------------
//		* ResourceDeleted										[protected]
// ---------------------------------------------------------------------------
//	A low-level resource was deleted. Make sure the primary resource is
//	updated to reflect the change.

void
RSResource::ResourceDeleted(
	RFResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inResource);
	
	// See if the deleted resource is our primary resource.
	// If not, we don't care about this notification.
	
	if (inResource != mRMResource)
		return;
	
	// It is our primary resource. See if there are other "related"
	// resources that we can bump up to primary status.
	
	LArray relatedRsrcs(sizeof (RFResource*));
	GetRelatedResources(relatedRsrcs);

	LFastArrayIterator iter(relatedRsrcs);
	RFResource* rsrc;
	while (iter.Next(&rsrc)) {
		ValidateObject_(rsrc);
		if (!rsrc->IsDeleted() && rsrc != mRMResource) {
			mRMResource = rsrc;
			return;
		}
	}
	
	// No other related resources. Delete this object.
	
	ValidateObject_(mSuperElement);
	DMContainerMixin* superContainer = (dynamic_cast<DMContainerMixin*>(mSuperElement));
	ValidateObject_(superContainer);
	
	superContainer->RemoveElement(this);

}


// ===========================================================================

#pragma mark -
#pragma mark ** common initialization

// ---------------------------------------------------------------------------
//		* InitRSResource										[private]
// ---------------------------------------------------------------------------
//	Common initialization for RSResource constructors.

void
RSResource::InitRSResource()
{
	mRMResource = nil;
	mChangingID = false;
	mUnpackChangeID = false;
	mUnpackResID = 0;
}
