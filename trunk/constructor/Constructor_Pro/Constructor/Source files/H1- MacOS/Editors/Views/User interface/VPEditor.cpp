// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPEditor.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/04/12 08:48:30 $
//	$History: VPEditor.cpp $
//	
//	*****************  Version 19  *****************
//	User: scouten      QDate: 03/19/97   Time: 12:15
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Disabled Arrange menu items when document is disabled. (Bug fix #1311.)
//	
//	*****************  Version 18  *****************
//	User: scouten      QDate: 02/18/97   Time: 16:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Fixed a rather silly bug in ReadObjectFromPPobTags that caused it to
//	lose class aliases.
//	
//	*****************  Version 17  *****************
//	User: scouten      QDate: 02/07/97   Time: 13:29
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Rolled in fixed from 2.4.1.
//	
//	*****************  Version 16  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:55
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 01/29/97   Time: 18:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Fixed ReadObjectFromPPobTags so that it safely recovers from undershoot
//	when reading stream data. (Bug fix #1233.)
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 01/21/97   Time: 14:58
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Updated WritePPobTagsForSubs to write radio groups at end of PPob list.
//	(Bug fix #1194.)
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 12/08/96   Time: 19:36
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added support for radio groups.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:44
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Changed WriteResourceDataSelf to force RidL generation for all layouts.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 11/25/96   Time: 21:36
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Updated to use the new mSuppressErrors flag in REEditor.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/25/96   Time: 20:35
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Improved handling of missing custom types.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/22/96   Time: 15:55
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added support for VPModelObject's DoSanityCheck call.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/22/96   Time: 09:20
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Finished code in UpdateResIDSelf.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/21/96   Time: 11:30
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added Make Tab Group command and hooks for implementing Make Radio
//	Group.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/20/96   Time: 13:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added code to generate RidL and wctb resources.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:31
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added support for dynamic columns in hierarchy window.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/19/96   Time: 12:19
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added special case for tab groups.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:48
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added PPob generator code. Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#include "VPEditor.h"
#include <LStaticText.h>

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMRGBColorAttribute.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Display classes (Eric) : Dialog handlers
#include "StKeyDialogHandler.h"

	// Core : Editors : Generic editor	
#include "RESession.h"

	// Core : Editors : Views : User interface
#include "VEHierTable.h"

	// Core : Resource manager
#include "RMResource.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Core : Utility classes
#include "UMemoryUtils.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"
#include "VPModelRadioGroup.h"
#include "VPTypeEntry.h"

	// MacOS : Resource manager
#include "RFResource.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_MissingCTInfo		= 3050;
const PaneIDT	Pane_ClassID			= 'ID  ';

const ResIDT	STR_ArrangeMenuStrings	= 3007;
const SInt16	str_MakeRadioGroup		= 1;
const SInt16	str_MakeTabGroup		= 2;

const CommandT	cmd_MakeRadioGroup		= 'radg';
const CommandT	cmd_MakeTabGroup		= 'tabg';


// ===========================================================================
//		* PPob resource format description
// ===========================================================================
//	Adapted from UReanimator.cp.

typedef	SInt32 TagID;

enum {
	tag_ObjectData		= 'objd',
	tag_BeginSubs		= 'begs',
	tag_EndSubs			= 'ends',
	tag_Include			= 'incl',
	tag_UserObject		= 'user',
	tag_ClassAlias		= 'dopl',
	tag_Comment			= 'comm',
	tag_End				= 'end.',
	
	object_Null			= 'null'
};


// ===========================================================================

#pragma mark *** VPEditor ***

// ---------------------------------------------------------------------------
//		* VPEditor(...)
// ---------------------------------------------------------------------------
//	Constructor

VPEditor::VPEditor(
	LCommander*		inSuper,
	RESession&		inSession)

: VEEditor(inSuper, inSession)

{

	// Make sure we have a valid type list.

	mTypeList = (dynamic_cast<VPTypeList*>(VETypeList::GetTypeList()));
	ValidateObject_(mTypeList.GetObject());

	// Reset parsing flags.

	mDeleteUnknownPanes = false;

}


// ---------------------------------------------------------------------------
//		* ~VPEditor
// ---------------------------------------------------------------------------
//	Destructor

VPEditor::~VPEditor()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** commander behaviors

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Overridden to enable the Make Radio Group and Make Tab Group.

void
VPEditor::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	
	// Dispatch command.

	switch (inCommand) {

		case cmd_MakeRadioGroup:
			if (mSelection->GetRootElement()->IsEnabled() && !UMemoryUtils::MemoryIsLow()) {
				outEnabled = mSelection->GetSelectedElements().GetCount() > 1;
			}
			break;
			
		case cmd_MakeTabGroup:
			if (mSelection->GetRootElement()->IsEnabled() && !UMemoryUtils::MemoryIsLow()) {
				outEnabled = true;
			}
			break;
		
		default:
			VEEditor::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);

	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Overridden to respond to the Make Radio Group and Make Tab Group commands.

Boolean
VPEditor::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Dispatch command.

	switch (inCommand) {

		case cmd_MakeRadioGroup:
			MakeRadioGroup();
			return true;
			
		case cmd_MakeTabGroup:
			MakeTabGroup();
			return true;
		
		default:
			return VEEditor::ObeyCommand(inCommand, ioParam);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* RegisterVPClasses										[static]
// ---------------------------------------------------------------------------
//	Register view and data model classes used by PP view editor.

void
VPEditor::RegisterVPClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;
	
	// Register generic view editor classes.
	
	VEEditor::RegisterVEClasses();

	// Register PP view editor data model classes.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->RegisterClass(VPTypeEntry::class_ID, VPTypeEntry::CreateFromStream);
	reanimator->RegisterClass(VPTypeList::class_ID, VPTypeList::CreateFromStream);

}


// ===========================================================================

#pragma mark -
#pragma mark ** editor override hooks

// ---------------------------------------------------------------------------
//		* ReadResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read data from PPob and WIND resources.

void
VPEditor::ReadResourceDataSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(mPrimaryResource);
	
	// Sanity check: Be sure we have a primary resource that's a PPob.
	
	RFResource* primaryRF = (dynamic_cast<RFResource*>(mPrimaryResource));
	ValidateObject_(primaryRF);
	
	ThrowIf_(primaryRF->GetResType() != 'PPob');
		
	// See if there's a 'WIND' resource also.
	
	RMResource* windRsrc = mSession.GetResource('WIND', primaryRF->GetResID(), false);

	// Build input streams for the two resources.
	
	LHandleStream ppobStream(mPrimaryResource->GetResData());
	LHandleStream windStream(windRsrc != nil ? windRsrc->GetResData() : ::NewHandle(0));

	// Make sure the PPob version is correct.
	
	SInt16 ppobVersion = 0;
	ppobStream >> ppobVersion;
	if (ppobVersion != 2)
		return;
	
	// Parse the stream.
	
	VPModelObjectPtr rootObject;
	ReadObjectFromPPobTags(rootObject, nil, ppobStream, windStream);
	
	ValidateObject_(rootObject.GetObject());
	
	// Check for 'end.' tag.
	
	FourCharCode endTag;
	ppobStream >> endTag;
	if (endTag != tag_End)
		return;
	
	// See if there is a 'wctb' resource.
	
	RMResource* wctbRsrc = mSession.GetResource('wctb', primaryRF->GetResID(), false);
	if (wctbRsrc != nil) {
		
		// There is, parse it.
		
		ValidateSimpleObject_(wctbRsrc);
		ReadWctbResource(wctbRsrc, rootObject);
		
	}

	// Let data model objects do a sanity check.
	
	rootObject->DoSanityCheck();
	
	// Okay, it was valid. Return it.

	mRootObject = rootObject;
	mDataModelRoot = rootObject;
	
	// Get a selection for this menu editor.
	
	mSession.GetSelectionForEditor(this, mSelection);
	ValidateObject_(mSelection.GetObject());

	if (mSelection->GetRootElement() == nil)
		mSelection->SetRootElement(mDataModelRoot);

}


// ---------------------------------------------------------------------------
//		* WriteResourceData										[protected]
// ---------------------------------------------------------------------------
//	Spill the view editor back out to PPob and WIND resources.

void
VPEditor::WriteResourceDataSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());
	ValidateSimpleObject_(mPrimaryResource);
	
	// Sanity check: Be sure we have a primary resource that's a PPob.
	
	RFResource* primaryRF = (dynamic_cast<RFResource*>(mPrimaryResource));
	ValidateObject_(primaryRF);
	
	ThrowIf_(primaryRF->GetResType() != 'PPob');
	
	// Let data model objects do a sanity check.
	
	mRootObject->DoSanityCheck();
	
	// See what other resources we have.
	
	RMResource* windRsrc = mSession.GetResource('WIND', primaryRF->GetResID(), false);
	RMResource* ridlRsrc = mSession.GetResource('RidL', primaryRF->GetResID(), false);
	RMResource* wctbRsrc = mSession.GetResource('wctb', primaryRF->GetResID(), false);

	// Build output streams for the two resources.
	
	LHandleStream ppobStream;
	LHandleStream windStream;
	LHandleStream ridlStream;
	LHandleStream wctbStream;
	
	// Write the PPob version number.
	
	SInt16 ppobVersion = 2;
	ppobStream << ppobVersion;
	
	// Write the PPob tab format.
	
	WritePPobTags(mRootObject, ppobStream, windStream);

	// Update the RidL resource.
	
	SInt16 itemCount = 0;
	ridlStream << itemCount;

	WriteRidLData(mRootObject, ridlStream);
	
	itemCount = (ridlStream.GetLength() - 2) / 4;
	ridlStream.SetMarker(0, streamFrom_Start);
	ridlStream << itemCount;
	
	// Update the wctb resource.
	
	if (wctbRsrc != nil) {
		ValidateSimpleObject_(wctbRsrc);
		wctbStream.SetDataHandle(wctbRsrc->GetResData());
	}
	WriteWctbData(mRootObject, wctbStream);
	
	// Write the 'end.' tag.
	
	FourCharCode endTag = tag_End;
	ppobStream << endTag;

	// Spill the streams to resources.
	
	mPrimaryResource->SetResData(ppobStream.GetDataHandle());
	
	if (windStream.GetLength()) {
		if (windRsrc == nil)
			windRsrc = mSession.GetResource('WIND', primaryRF->GetResID(), true);
		ValidateSimpleObject_(windRsrc);
		windRsrc->SetResData(windStream.GetDataHandle());
	}
	else if (windRsrc != nil) {
		ValidateSimpleObject_(windRsrc);
		windRsrc->DeleteResource();
	}

	if (ridlStream.GetLength()) {
		if (ridlRsrc == nil)
			ridlRsrc = mSession.GetResource('RidL', primaryRF->GetResID(), true);
		ValidateSimpleObject_(ridlRsrc);
		ridlRsrc->SetResData(ridlStream.GetDataHandle());
	}
	else if (ridlRsrc != nil) {
		ValidateSimpleObject_(ridlRsrc);
		ridlRsrc->DeleteResource();
	}

	if (wctbStream.GetLength()) {
		if (wctbRsrc == nil)
			wctbRsrc = mSession.GetResource('wctb', primaryRF->GetResID(), true);
		ValidateSimpleObject_(wctbRsrc);
		wctbRsrc->SetResData(wctbStream.GetDataHandle());
	}
	else if (wctbRsrc != nil) {
		ValidateSimpleObject_(wctbRsrc);
		wctbRsrc->DeleteResource();
	}
}


// ---------------------------------------------------------------------------
//		* UpdateResIDSelf										[protected]
// ---------------------------------------------------------------------------
//	The resource ID for this PPob has changed. Change the reference to
//	the WIND resource that's contained inside the LWindow data (if any).

void
VPEditor::UpdateResIDSelf(
	RMResource*		/* inResource */,
	ResIDT			/* inOldResID */,
	ResIDT			inNewResID)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());
	
	// See if there is a WIND ID attribute.
	
	DMIntegerAttribute* windAttr =
				(dynamic_cast<DMIntegerAttribute*>(mRootObject->FindAttributeByKey(pPaneWINDResourceID)));

	if (windAttr != nil) {
	
		// There is one, change the ID.
		
		windAttr->SetIntValue(inNewResID);

		// Rewrite the resource data.

		WriteResourceData();
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** editor windows

// ---------------------------------------------------------------------------
//		* AddDefaultHierColumns									[protected]
// ---------------------------------------------------------------------------
//	Add columns for pane ID and descriptor to the hierarchy table.

void
VPEditor::AddDefaultHierColumns(
	VEHierTable*	inHierTable)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inHierTable);
	
	// Add the default columns.
	
	inHierTable->InsertColumn(1, pPaneID, 70);
	inHierTable->InsertColumn(2, pPaneDescriptor, 160);

}



// ===========================================================================

#pragma mark -
#pragma mark ** PPob parser

// ---------------------------------------------------------------------------
//		* ReadObjectFromPPobTags								[protected]
// ---------------------------------------------------------------------------
//	Called by ReadResourceDataSelf. Read a single object (and its subobjects)
//	from a PPob input stream.

void
VPEditor::ReadObjectFromPPobTags(
	VPModelObjectPtr&	inObjectPtr,
	VPModelObject*		inParentObject,
	LStream&			inPPobStream,
	LStream&			inWindStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mTypeList.GetObject());

	// First, try to read this object.
	
	inObjectPtr = nil;

	FourCharCode tag = 0;
	FourCharCode alias = 0;
	Boolean stop = false;
	
	while (inObjectPtr == nil) {
		
		// Read the tag.
	
		inPPobStream >> tag;
		switch (tag) {
			
			// User object: Constructor 1.0 used this info, we don't.
			
			case tag_UserObject: {
				ClassIDT superClassID;
				inPPobStream >> superClassID;
				break;
			}
			
			// Alias: Record class ID.
		
			case tag_ClassAlias:
				inPPobStream >> alias;
				break;
		
			// Comment: Ignore for now. We'll use it later.
			
			case tag_Comment: {
				SInt32 commentLength = 0;
				inPPobStream >> commentLength;
				inPPobStream.SetMarker(commentLength, streamFrom_Marker);
				break;
			}
			
			// Object data: Try to build an object.

			case tag_ObjectData: {

				// Get the data length and record the mark.

				SInt32 dataLength;
				inPPobStream >> dataLength;
				SInt32 dataStart = inPPobStream.GetMarker();
				
				// Get class ID.
				
				ClassIDT classID;
				inPPobStream >> classID;
				
				// See if we know anything about this type.
				
				VPTypeEntry* typeEntry = nil;
				if (alias != 0) {
					typeEntry = (dynamic_cast<VPTypeEntry*>(mTypeList->FindTypeByID(alias)));
					if (typeEntry != nil) {
						classID = alias;
						alias = 0;
					}
				}
				if (typeEntry == nil)
					typeEntry = (dynamic_cast<VPTypeEntry*>(mTypeList->FindTypeByID(classID)));

				if (typeEntry == nil) {

					if (!mDeleteUnknownPanes)
						ComplainAboutUnknownType(classID);
					
					if (dataLength >= sizeof (SViewInfo) + sizeof (SPaneInfo) + 4)
						typeEntry = (dynamic_cast<VPTypeEntry*>(mTypeList->FindTypeByID(LView::class_ID)));
					else
						typeEntry = (dynamic_cast<VPTypeEntry*>(mTypeList->FindTypeByID(LPane::class_ID)));

				}

				ValidateObject_(typeEntry);
				
				// We do, build an instance of it.
				
				TSharablePtr<VEModelObject> tempObject;
				typeEntry->CreateInstance(tempObject);
				inObjectPtr = (dynamic_cast<VPModelObject*>((VEModelObject*) tempObject));
				ValidateObject_(inObjectPtr.GetObject());
				
				// Configure the alias.
				
				inObjectPtr->SetClassAlias(alias != 0 ? alias : classID);

				// Read object data.
				
				inObjectPtr->ReadStreamData(&inPPobStream, 'PPob');
				inObjectPtr->ReadStreamData(&inWindStream, 'WIND');
				
				// See if there's any leftover data. We ignore it if so.
				
				SInt32 leftoverSize = (dataStart + dataLength) - inPPobStream.GetMarker();
				if (leftoverSize != 0)
					inPPobStream.SetMarker(leftoverSize, streamFrom_Marker);
//				if (leftoverSize < 0) {
//					inObjectPtr = nil;
//					return;
//				}
				
				break;
			}
		
			// Anything else: Unread the tag and exit.
			
			default:
				inPPobStream.SetMarker(-4, streamFrom_Marker);
				stop = true;
		}
	
		if (stop)
			break;

	}
	
	// Got the object, see if there are subobjects.
	
	inPPobStream >> tag;
	if (tag != tag_BeginSubs) {
		inPPobStream.SetMarker(-4, streamFrom_Marker);
		return;
	}
	
	// There are subobjects. Read them.
	
	if (inObjectPtr == nil)
		inObjectPtr = inParentObject;

	ValidateObject_(inObjectPtr.GetObject());
	
	if (inObjectPtr->GetVPTypeEntry()->TypeDerivesFrom(LTabGroup::class_ID))
		ReadSubObjectsFromPPobTags(inParentObject, inPPobStream, inWindStream);
	else
		ReadSubObjectsFromPPobTags(inObjectPtr, inPPobStream, inWindStream);

}


// ---------------------------------------------------------------------------
//		* ReadSubObjectsFromPPobTags							[protected]
// ---------------------------------------------------------------------------
//	Read the tags for subobjects from the PPob stream. Installs these objects
//	in the specified parent object.

void
VPEditor::ReadSubObjectsFromPPobTags(
	VPModelObject*	inParentObject,
	LStream&		inPPobStream,
	LStream&		inWindStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inParentObject);
	ValidateObject_(mTypeList.GetObject());

	// Read tags until the end tag is reached.

	FourCharCode tag = 0;
	
	while (1) {
	
		// Read tags for subobjects.
	
		inPPobStream >> tag;
		switch (tag) {
		
			// End subs: Go back up in nesting level.
		
			case tag_EndSubs:
				return;
			
			// Any object tag: Read a subobject and see where it fits in.
			
			case tag_ObjectData:
			case tag_UserObject:
			case tag_ClassAlias: {
				inPPobStream.SetMarker(-4, streamFrom_Marker);

				VPModelObjectPtr subObject;
				ReadObjectFromPPobTags(subObject, inParentObject, inPPobStream, inWindStream);
				
				if (subObject == nil)
					break;
				
				DMContainerAttribute* host = nil;
				if ((subObject->GetVPTypeEntry()->TypeDerivesFrom('atch'))
				 || (subObject->GetVPTypeEntry()->TypeDerivesFrom(LTabGroup::class_ID)))
					host = inParentObject->GetAttachmentList();
				else
					host = inParentObject->GetSubPaneList();
				
				if (host != nil) {
					ValidateObject_(host);
					host->InsertElementAt(LArray::index_Last, subObject, true);
				}

				break;
			}

			// Comment: Ignore for now. We'll use it later.
			
			case tag_Comment: {
				SInt32 commentLength = 0;
				inPPobStream >> commentLength;
				inPPobStream.SetMarker(commentLength, streamFrom_Marker);
				break;
			}
			
			// Anything else: Unread the tag and exit.
			
			default:
				inPPobStream.SetMarker(-4, streamFrom_Marker);
				return;
		}
	}
}


// ---------------------------------------------------------------------------
//		* ReadWctbResource										[protected]
// ---------------------------------------------------------------------------
//	A 'wctb' resource exists for this layout. Read it and get its content
//	color. If there is a window at the root of this layout (and there should
//	be), set its content color attribute accordingly.

void
VPEditor::ReadWctbResource(
	RMResource*		inWctbResource,
	VPModelObject*	inRootObject)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inRootObject);
	ValidateSimpleObject_(inWctbResource);
	
	// Look for "has wctb" attribute. If none such, ignore the resource contents.
	
	DMIntegerAttribute* hasWctbAttr =
		(dynamic_cast<DMIntegerAttribute*>(inRootObject->FindAttributeByKey(pPaneWindowHasWCTB)));
	ValidateObject_(hasWctbAttr);
	
	// Make sure resource isn't deleted.
	
	if (inWctbResource->IsDeleted())
		return;
	
	// Yes, it has a 'wctb'. Set default content color in case none is specified.

	hasWctbAttr->SetIntValue(1);

	DMRGBColorAttribute* contentColorAttr =
		(dynamic_cast<DMRGBColorAttribute*>(inRootObject->FindAttributeByKey(pPaneWindowContentColor)));
	ValidateObject_(contentColorAttr);

	RGBColor color = UGAColorRamp::GetWhiteColor();
	contentColorAttr->SetRGBColorValue(color);

	// Now read the resource data.
	
	LHandleStream wctbData(inWctbResource->GetResData());
	wctbData.SetMarker(6, streamFrom_Start);
	
	SInt16 numEntries;
	// ReadData is right here -- the resource is already swapped.
	wctbData.ReadData(&numEntries, sizeof (numEntries));
	numEntries++;
	
	// Read each wctb entry.

	while (numEntries--) {

		SInt16 partID;
		
		// ReadData is right here -- the resource is already swapped.
		wctbData.ReadData(&partID, sizeof (partID));
		wctbData.ReadData(&color, sizeof (color));

		// Ignore all color entries except for content color.

		if (partID == 0)
			contentColorAttr->SetRGBColorValue(color);

	}
}


// ---------------------------------------------------------------------------
//		* ComplainAboutUnknownType								[protected]
// ---------------------------------------------------------------------------
//	Show a dialog informing the user that no custom type description was
//	found for one of the elements in this resource.

void
VPEditor::ComplainAboutUnknownType(
	ClassIDT	inUnknownType)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// If dialog is suppressed, just assume user hit cancel.
	
	if (mSuppressErrors)
		Throw_(err_CantOpenSilent);

	// Set up the dialog.

	StApplicationContext appContext;
	StKeyDialogHandler dialog(PPob_MissingCTInfo, LCommander::GetTarget());

	LWindow* window = dialog.GetDialog();
	ValidateObject_(window);

	// Set the class name field to the four-character code of the missing class.

	LStaticText* className = dynamic_cast<LStaticText*>(window->FindPaneByID(Pane_ClassID));
	ValidateObject_(className);
	className->SetDescriptor(LStr255((FourCharCode) inUnknownType));

	// Run the dialog.

	dialog.GetDialog()->Show();	
	MessageT message = msg_Nothing;
	while (message == msg_Nothing) {
		message = dialog.DoDialog();
	}
	
	// Force the menus to be redrawn.
	
	LCommander::SetUpdateCommandStatus(true);

	// If user clicked Cancel, throw an exception that gets us out of reading the PPob.

	if (message == msg_Cancel)
		LException::Throw(err_CantOpenSilent);

	// Otherwise, remember that we should delete unknown pane types.

	mDeleteUnknownPanes = true;

}


// ===========================================================================

#pragma mark -
#pragma mark ** PPob generator

// ---------------------------------------------------------------------------
//		* WritePPobTags											[protected]
// ---------------------------------------------------------------------------
//	Called by WriteResourceDataSelf. Generates the appropriate PPob tags
//	for an object in this layout, and any subobjects it may have.

void
VPEditor::WritePPobTags(
	VPModelObject*		inObject,
	LHandleStream&		inPPobStream,
	LHandleStream&		inWindStream)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// See if this object has a class alias.
	
	VPTypeEntry* paneType = inObject->GetVPTypeEntry();
	ValidateObject_(paneType);
	
	ClassIDT paneClass = paneType->GetClassID();
	ClassIDT classAlias = inObject->GetClassAlias();

	if ((classAlias != paneClass) && (classAlias != 0)) {
		TagID aliasTag = tag_ClassAlias;
		inPPobStream	<< aliasTag
						<< classAlias;
	}

	// Write the data for this object.
	// The stream must be random access since we back up to write the length.
	
	TagID theTag = tag_ObjectData;
	inPPobStream << theTag;
	
	SInt32 length = 0;
	inPPobStream << length;

	SInt32 dataStart = inPPobStream.GetMarker();
	inPPobStream << paneClass;

	inObject->WriteStreamData(&inPPobStream, 'PPob');
	inObject->WriteStreamData(&inWindStream, 'WIND');

	length = inPPobStream.GetMarker() - dataStart;
	inPPobStream.SetMarker(dataStart - sizeof (length), streamFrom_Start);
	inPPobStream << length;
	inPPobStream.SetMarker(dataStart + length, streamFrom_Start);

	// Now write stream data for any attachments or subpanes we may have.
	
	DMContainerAttribute* attachments =
			(dynamic_cast<DMContainerAttribute*>(inObject->FindAttributeByKey(pPaneAttachmentsList)));
	DMContainerAttribute* subPanes =
			(dynamic_cast<DMContainerAttribute*>(inObject->FindAttributeByKey(pPaneSubViewList)));
	
	WritePPobTagsForSubs(attachments, subPanes, inPPobStream, inWindStream);

}


// ---------------------------------------------------------------------------
//		* WritePPobTagsForSubs									[protected]
// ---------------------------------------------------------------------------
//	Called by WritePPobTags. Writes the PPob data for subobjects of a
//	member of this layout.

void
VPEditor::WritePPobTagsForSubs(
	DMContainerAttribute*	inAttachments,
	DMContainerAttribute*	inSubPanes,
	LHandleStream&			inPPobStream,
	LHandleStream&			inWindStream)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// See if there are any subobjects to write home about. :-)
	
	Boolean hasSubs = false;
	
	if (inAttachments != nil) {
		ValidateObject_(inAttachments);
		hasSubs |= inAttachments->GetSubElements().GetCount() > 0;
	}

	if (inSubPanes != nil) {
		ValidateObject_(inSubPanes);
		hasSubs |= inSubPanes->GetSubElements().GetCount() > 0;
	}
	
	if (!hasSubs)
		return;
	
	// We have subobjects. Write the begin subs tag.
	
	TagID theTag = tag_BeginSubs;
	inPPobStream << theTag;
	
	// Iterate through the attachments first.
	
	if (inAttachments != nil) {
		DMFastIterator attachments(inAttachments->GetSubElements());
		while (attachments.NextElement()) {
			
			// Get the next attachment.
			
			VPModelObject* attachment = (dynamic_cast<VPModelObject*>(attachments.CurrentElement()));
			ValidateObject_(attachment);

			// BUG FIX #1194: Added this clause.
			// Special case: If writing a radio group "attachment,"
			// put it at the end of the PPob list, not the beginning.
			
			if (attachment->GetVPTypeEntry()->TypeDerivesFrom(LRadioGroup::class_ID))
				continue;

			// Not a radio group, write the PPob data for this object now.

			WritePPobTags(attachment, inPPobStream, inWindStream);

			// Special case: If writing a tab group "attachment,"
			// write all of its parent's subpanes as subobjects of the attachment.
			
			if (attachment->GetVPTypeEntry()->TypeDerivesFrom(LTabGroup::class_ID)) {
				WritePPobTagsForSubs(nil, inSubPanes, inPPobStream, inWindStream);
				inSubPanes = nil;
			}
		}
	}
	
	// Then iterate through the subpanes.
	
	if (inSubPanes != nil) {
		DMFastIterator subPanes(inSubPanes->GetSubElements());
		while (subPanes.NextElement()) {
		
			// Get the next subpane.
		
			VPModelObject* subPane = (dynamic_cast<VPModelObject*>(subPanes.CurrentElement()));
			ValidateObject_(subPane);
			
			// Write the PPob data for this pane.
			
			WritePPobTags(subPane, inPPobStream, inWindStream);

		}
	}
	
	// BUG FIX #1194: Added this section.
	// Iterate through the attachments again, looking for radio groups.
	
	if (inAttachments != nil) {
		DMFastIterator attachments(inAttachments->GetSubElements());
		while (attachments.NextElement()) {
			
			// Get the next attachment.
			
			VPModelObject* attachment = (dynamic_cast<VPModelObject*>(attachments.CurrentElement()));
			ValidateObject_(attachment);

			// Special case: If writing a radio group "attachment,"
			// put it at the end of the PPob list, not the beginning.
			
			if (attachment->GetVPTypeEntry()->TypeDerivesFrom(LRadioGroup::class_ID))
				WritePPobTags(attachment, inPPobStream, inWindStream);

		}
	}
	
	// Write the end subs tag.
	
	theTag = tag_EndSubs;
	inPPobStream << theTag;
	
}


// ---------------------------------------------------------------------------
//		* WriteRidLData											[protected]
// ---------------------------------------------------------------------------
//	Called by WriteResourceDataSelf. Writes the ID of any LControl derivative
//	to the RidL resource stream. Calls itself recursively to write IDs of
//	subobjects.

void
VPEditor::WriteRidLData(
	VPModelObject*		inObject,
	LHandleStream&		inRidLStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);
	
	// Look for reasons not to write this object to the RidL stream.	

	do {
	
		// Is it an LControl?
	
		VPTypeEntry* typeEntry = inObject->GetVPTypeEntry();
		ValidateObject_(typeEntry);
		if (!typeEntry->TypeDerivesFrom('cntl'))
			break;

		// Is the pane ID unique within in the PPob?
		
		SInt32 paneID = inObject->GetPaneID();

		VPModelObject* rootVP = (dynamic_cast<VPModelObject*>(mDataModelRoot.GetObject()));
		ValidateObject_(rootVP);
		
		if (rootVP->CheckForDuplicateID(paneID))
			break;

		// Passes both tests, include it in the RidL.
		
		inRidLStream << paneID;

	} while (0);
	
	// See if this object has any subpanes.
	
	DMContainerAttribute* subPanes = inObject->GetSubPaneList();
	if (subPanes != nil) {
	
		// It does, see if any of them should be written to the RidL.
		
		ValidateObject_(subPanes);
		
		DMFastIterator iter(subPanes->GetSubElements());
		while (iter.NextElement()) {
		
			VPModelObject* object = (dynamic_cast<VPModelObject*>(iter.CurrentElement()));
			ValidateObject_(object);
			WriteRidLData(object, inRidLStream);

		}
	}
}


// ---------------------------------------------------------------------------
//		* WriteWctbData											[protected]
// ---------------------------------------------------------------------------
//	Called by WriteResourceDataSelf. Writes the window's content color to
//	the 'wctb' resource. Copies over any other color table entries that may
//	exist.

void
VPEditor::WriteWctbData(
	VPModelObject*		inRootObject,
	LHandleStream&		inWctbStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inRootObject);

	// See if the root object knows about content color.
	
	DMRGBColorAttribute* contentColorAttr =
			(dynamic_cast<DMRGBColorAttribute*>(inRootObject->FindAttributeByKey(pPaneWindowContentColor)));
	
	if (contentColorAttr == nil)
		return;
	
	
	// It does. Extract that color from the attribute.

	RGBColor contentColor = contentColorAttr->GetRGBColorValue();

	// If a wctb already exists, save its data to the side.
	// We write a new version.

	LHandleStream oldWctb(inWctbStream.DetachDataHandle());
	oldWctb.SetMarker(8, streamFrom_Start);

	// Write wctb header (eight bytes of zero).
	// We'll fill in # of entries later.
	
	SInt16 partCount = -1;
	SInt16 partID = 0;
	SInt32 zero = 0;
	inWctbStream.WriteData(&zero, sizeof (zero));
	inWctbStream.WriteData(&zero, sizeof (zero));
	
	// Write content color.

	partID = 0;
	inWctbStream.WriteData(&partID, sizeof (partID));
	inWctbStream.WriteData(&contentColor, sizeof (contentColor));
	partCount++;
	
	// Copy other wctb elements over.
	
	while (oldWctb.GetMarker() < oldWctb.GetLength()) {
	
		RGBColor partColor;
		oldWctb.ReadData(&partID, sizeof (partID));
		oldWctb.ReadData(&partColor, sizeof (partColor));
		
		if (partID != 0) {
			inWctbStream.WriteData(&partID, sizeof (partID));
			inWctbStream.WriteData(&partColor, sizeof (partColor));
			partCount++;
		}
	}
	
	// Update item count.
	
	inWctbStream.SetMarker(6, streamFrom_Start);
	inWctbStream.WriteData(&partCount, sizeof (partCount));

	// See if we're supposed to write a WCTB.

	DMIntegerAttribute* writeWctbAttr = inRootObject->FindIntegerAttribute(pPaneWindowHasWCTB);
	ValidateObject_(writeWctbAttr);
	
	Boolean writeWCTB = writeWctbAttr->GetIntValue() != 0;
	if (!writeWCTB)
		inWctbStream.SetLength(0);

}


// ===========================================================================

#pragma mark -
#pragma mark ** radio and tab group commands

// ---------------------------------------------------------------------------
//		* PutOnDuty												[protected]
// ---------------------------------------------------------------------------
//	Add the Make Tab Group and Make Radio Group to the Arrange menu.

void
VPEditor::PutOnDuty( LCommander *inNewTarget)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Remember if the menu resources are going to be loaded.
	
	Boolean addGroupItems = (sArrangeMenu == nil);
	
	// Load the menu resources if this hasn't been done already.

	VEEditor::PutOnDuty(inNewTarget);
	
	// Now add the "Make Tab Group" and "Make Radio Group" menu items.

	if (addGroupItems) {
		
		StApplicationContext appContext;

		ValidateSimpleObject_(sArrangeMenu);
		sArrangeMenu->InsertCommand("\p-", 0, 32766);
		sArrangeMenu->InsertCommand(LStr255(STR_ArrangeMenuStrings, str_MakeRadioGroup), cmd_MakeRadioGroup, 32766);
		sArrangeMenu->InsertCommand(LStr255(STR_ArrangeMenuStrings, str_MakeTabGroup), cmd_MakeTabGroup, 32766);
		
		SInt16 theRadioIndex = sArrangeMenu->IndexFromCommand(cmd_MakeRadioGroup);
		SInt16 theTabIndex = sArrangeMenu->IndexFromCommand(cmd_MakeTabGroup);
#if PP_Target_Carbon
		::SetMenuItemModifiers(sArrangeMenu->GetMacMenuH(), theRadioIndex, kMenuShiftModifier);
		::SetMenuItemModifiers(sArrangeMenu->GetMacMenuH(), theTabIndex, kMenuShiftModifier);
#else
		// JWW - Underline attribute really means use a shift modifier with Mercutio
		::SetItemStyle(sArrangeMenu->GetMacMenuH(), theRadioIndex, underline);
		::SetItemStyle(sArrangeMenu->GetMacMenuH(), theTabIndex, underline);
#endif
	}
}


// ---------------------------------------------------------------------------
//		* MakeRadioGroup										[protected]
// ---------------------------------------------------------------------------
//	Form a radio group from the selected panes, which are presumably
//	radio buttons. The radio group will automatically reject any ineligible
//	objects.

void
VPEditor::MakeRadioGroup()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mTypeList.GetObject());
	
	// First, make sure root object can host attachments.
	// (We now treat radio groups as though they are attachments.)

	VPModelObject* rootVP = (dynamic_cast<VPModelObject*>(mDataModelRoot.GetObject()));
	ValidateObject_(rootVP);
	
	DMContainerAttribute* attachments =
				(dynamic_cast<DMContainerAttribute*>(rootVP->FindAttributeByKey(pPaneAttachmentsList)));
	if (attachments == nil)
		return;

	// Build a transaction to record this activity.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_MakeRadioGroup);
	
	// Create a new radio group.
	
	VETypeEntry* radioGroupType = mTypeList->FindTypeByID(LRadioGroup::class_ID);
	ValidateObject_(radioGroupType);
	
	VEModelObjectPtr radioGroupPtr;
	radioGroupType->CreateInstance(radioGroupPtr);
	
	VPModelRadioGroup* radioGroup = (dynamic_cast<VPModelRadioGroup*>(radioGroupPtr.GetObject()));
	ValidateObject_(radioGroup);
	
	// Add all of the selected elements that can be added.
	
	{
		DMFastIterator iter(mSelection->GetSelectedElements());
		while (iter.NextElement()) {
			
			// Make sure this is a visual object.
			
			VPModelObject* object = (dynamic_cast<VPModelObject*>(iter.CurrentElement()));
			if (object == nil)
				continue;
			
			// Make sure it's an LControl.
			
			VPTypeEntry* objType = object->GetVPTypeEntry();
			ValidateObject_(objType);
			
			if (!objType->TypeDerivesFrom('cntl'))
				continue;
			
			// It is an LControl, add it to the group.
			
			radioGroup->AddMember(object);
			
		}
	}

	// Insert it in the data model.
	
	attachments->InsertElementAt(LArray::index_Last, radioGroup, true);
	
	// Select the new radio group.
	
	mSelection->SelectSimple(radioGroup);
	
	// Commit the transaction.
	
	trans.Commit();

}


// ---------------------------------------------------------------------------
//		* MakeTabGroup											[protected]
// ---------------------------------------------------------------------------
//	Create a default tab group.

void
VPEditor::MakeTabGroup()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());
	ValidateObject_(mTypeList.GetObject());
	
	// First, make sure root object can host attachments.
	// (We now treat tab groups as though they are attachments.)

	VPModelObject* rootVP = (dynamic_cast<VPModelObject*>(mDataModelRoot.GetObject()));
	ValidateObject_(rootVP);
	
	DMContainerAttribute* attachments =
				(dynamic_cast<DMContainerAttribute*>(rootVP->FindAttributeByKey(pPaneAttachmentsList)));
	if (attachments == nil)
		return;

	// Build a transaction to record this activity.
	
	StDMTransactionBuilder trans(this, mSelection, cmd_MakeTabGroup);
	
	// Scan root object's attachments list for tab groups.
	// Delete them if found.
	
	DMIterator iter(const_cast<LSharableArray&>(attachments->GetSubElements()));
	while (iter.NextElement()) {
	
		VPModelObject* attachment = (dynamic_cast<VPModelObject*>(iter.CurrentElement()));
		ValidateObject_(attachment);
		
		if (attachment->GetVPTypeEntry()->TypeDerivesFrom(LTabGroup::class_ID))
			attachments->RemoveElement(attachment);
	
	}

	// Create a new tab group.
	
	VETypeEntry* tabGroupType = mTypeList->FindTypeByID(LTabGroup::class_ID);
	ValidateObject_(tabGroupType);
	
	VEModelObjectPtr tabGroup;
	tabGroupType->CreateInstance(tabGroup);
	
	// Insert it in the data model.
	
	attachments->InsertElementAt(LArray::index_Last, tabGroup, true);
	
	// Select the new tab group.
	
	mSelection->SelectSimple(tabGroup);
	
	// Commit the transaction.
	
	trans.Commit();

}