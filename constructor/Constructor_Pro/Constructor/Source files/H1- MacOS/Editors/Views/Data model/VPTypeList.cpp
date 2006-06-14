// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPTypeList.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/04/12 08:48:30 $
//	$History: VPTypeList.cpp $
//	
//	*****************  Version 18  *****************
//	User: scouten      QDate: 01/30/97   Time: 15:26
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed ScanCurrentResourceFile and ScanResourceMap so that CTYP
//	descriptions take precedence over CPPb descriptions. (Bug fix #1232.)
//	
//	*****************  Version 17  *****************
//	User: scouten      QDate: 01/30/97   Time: 13:24
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed MakeModelObjectForClass so that LPrintout gets a VPModelView.
//	This fixes the "LPrintout doesn't show hierarchy in hiearchy window"
//	bug. (Bug fix #1244.)
//	
//	*****************  Version 16  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 12/21/96   Time: 18:30
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Reenabled LGATabPanel drawing agent.
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 12/18/96   Time: 13:35
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Temporarily disabled LGATabPanel drawing agent.
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 12/17/96   Time: 14:24
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added support for LGALittleArrows and LGATabPanel.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 12/16/96   Time: 13:58
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added HandleResourceDataChanged override to allow dynamic updating of
//	type list as CTYPs are edited. (Bug fix #1098.)
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 12/08/96   Time: 18:04
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added support for VPModelRadioGroup.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 12/08/96   Time: 16:47
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added LListBox drawing agent.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 12/08/96   Time: 11:29
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added support for CTYP descriptions of custom types.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/06/96   Time: 16:45
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed FixRuntimeParent to know about the grayscale "standard" controls.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/05/96   Time: 14:11
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Updated ReadItemListFromCPPb to know about DMStringAttribute's
//	SetAcceptCR flag.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:50
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed a bug in ReadItemListFromCPPb. It was reading even-padded strings
//	as odd-padded strings.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/25/96   Time: 00:38
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Changed subpane and attachment lists to use the new
//	VESubModelContainer.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/22/96   Time: 16:04
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added support for VPModelScroller and VPModelDialogBox.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:43
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added support for VPModelView. Corrected some bugs in
//	ReadItemListFromCPPb.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:11
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added support for GA classes. Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#include "VPTypeList.h"

	// Core : Data model : Attributes
#include "DMBitfieldAttribute.h"
#include "DMContainerAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMRGBColorAttribute.h"
#include "DMStringAttribute.h"
#include "DMToolboxBooleanAttribute.h"
#include "DMValueLabel.h"

	// Core : Data model : Core classes
#include "DMReanimator.h"

	// Core : Data model : Undo/redo
#include "StDMTransactionHider.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Editors : Views : Data model
#include "VESubModelContainer.h"

	// Core : Utility classes
#include "UIconUtilities.h"

	// MacOS : Editors : Views : Data model
#include "VPTypeEntry.h"
#include "VPDataModel.h"
#include "VPModelAttachment.h"
#include "VPModelDialogBox.h"
#include "VPModelObject.h"
#include "VPModelPane.h"
#include "VPModelRadioGroup.h"
#include "VPModelScroller.h"
#include "VPModelView.h"
#include "VPModelWindow.h"

	// MacOS : Editors : Views : Drawing agents
#include "VPLButton.h"
#include "VPLCaption.h"
#include "VPLCicnButton.h"
#include "VPLControl.h"
#include "VPLEditField.h"
#include "VPLGABox.h"
#include "VPLGACheckbox.h"
#include "VPLGAColorSwatch.h"
#include "VPLGADisclosureTriangle.h"
#include "VPLGAEditField.h"
#include "VPLGAFocusBorder.h"
#include "VPLGAIconButton.h"
#include "VPLGAIconButtonPopup.h"
#include "VPLGAIconSuite.h"
#include "VPLGAIconSuiteControl.h"
#include "VPLGAIconTextButton.h"
#include "VPLGALittleArrows.h"
#include "VPLGAPopup.h"
#include "VPLGAPrimaryBox.h"
#include "VPLGAPushButton.h"
#include "VPLGARadioButton.h"
#include "VPLGASecondaryBox.h"
#include "VPLGASeparator.h"
#include "VPLGATabPanel.h"
#include "VPLGATextButton.h"
#include "VPLGroupBox.h"
#include "VPLListBox.h"
#include "VPLPane.h"
#include "VPLPicture.h"
#include "VPLPrintout.h"
#include "VPLScroller.h"
#include "VPLStdControl.h"
#include "VPLStdPopupMenu.h"
#include "VPLTextButton.h"
#include "VPLToggleButton.h"
#include "VPLView.h"
#include "VPLWindow.h"

#include "VPLBevelButton.h"
#include "LCmdBevelButton.h"
#include "VPLChasingArrows.h"
#include "VPLCheckBox.h"
#include "VPLCheckBoxGroupBox.h"
#include "VPLDisclosureTriangle.h"
#include "VPLEditText.h"
#include "VPLIconControl.h"
#include "VPLImageWell.h"
#include "VPLLittleArrows.h"
#include "VPLPictureControl.h"
#include "VPLPlacard.h"
#include "VPLPopupButton.h"
#include "VPLPopupGroupBox.h"
#include "VPLProgressBar.h"
#include "VPLPushButton.h"
#include "VPLRadioButton.h"
#include "VPLScrollBar.h"
#include "VPLSeparatorLine.h"
#include "VPLSlider.h"
#include "VPLStaticText.h"
#include "VPLTabsControl.h"
#include "VPLTextGroupBox.h"
#include "VPLWindowHeader.h"

#include "VPLPageController.h"
#include "VPLScrollerView.h"

	// MacOS : Editors : Views : User interface
#include "VPEditor.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	DSPC_VPTypeList		= 3050;


// ===========================================================================

#pragma mark *** VPTypeList ***

// ---------------------------------------------------------------------------
//		* VPTypeList(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional stream data is read.

VPTypeList::VPTypeList(
	LStream*	inStream)

: VETypeList(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VPTypeList(VPTypeList&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPTypeList::VPTypeList(
	const VPTypeList&	inOriginal)

: VETypeList(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~VPTypeList
// ---------------------------------------------------------------------------
//	Destructor

VPTypeList::~VPTypeList()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** type list accessor

// ---------------------------------------------------------------------------
//		* CreateGlobalTypeList									[static]
// ---------------------------------------------------------------------------
//	Create the global type list for PowerPlant classes if one doesn't
//	already exist. Usually called only by CAMacOSTarget::MakeTypeList.

void
VPTypeList::CreateGlobalTypeList()
{

	// If a global type list already exists, don't create another one.

	if (sTypeList != nil)
		return;

	// Doesn't exist, need to set one up.
	// Register PowerPlant view editor classes.

	VPEditor::RegisterVPClasses();

	// Use data model reanimator to read type list.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	DMElementPtr tempContainer;
	reanimator->ElementsFromResource('DSPC', DSPC_VPTypeList, tempContainer);
	
	sTypeList = (dynamic_cast<VETypeList*>((DMElement*) tempContainer));
	ValidateObject_(sTypeList.GetObject());
	
	sTypeList->ScanAtLaunch();

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource file scanning

// ---------------------------------------------------------------------------
//		* ScanCurrentResourceFile								[protected]
// ---------------------------------------------------------------------------
//	Scan the current file for CPPb and CLSS resources.

void
VPTypeList::ScanCurrentResourceFile(
	const FSSpec&	inSourceFile)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for CPPb and CLSS resources.

	MakeSpecsFromFile('CTYP', inSourceFile);
	MakeSpecsFromFile('CPPb', inSourceFile);
//	MakeSpecsFromFile('CLSS', inSourceFile);

}


// ---------------------------------------------------------------------------
//		* ScanResourceMap										[protected]
// ---------------------------------------------------------------------------
//	Scan the named resource map for CLSS and CPPb resources.

void
VPTypeList::ScanResourceMap(
	const FSSpec&	inSourceFile,
	RFMap&			inRFMap)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for CTYP, CPPb and CLSS resources in this document.

	MakeSpecsFromMap('CTYP', inSourceFile, inRFMap);
	MakeSpecsFromMap('CPPb', inSourceFile, inRFMap);
//	MakeSpecsFromMap('CLSS', inSourceFile, inRFMap);

}


// ---------------------------------------------------------------------------
//		* ScanFolderForPlugIns									[protected]
// ---------------------------------------------------------------------------
//	Scan folder for Constructor documents containing class descriptions.

void
VPTypeList::ScanFolderForPlugIns(
	SInt16	inVRefNum,
	SInt32	inDirID)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for Constructor documents in the folder.

	MakeSpecsFromFolder(inVRefNum, inDirID, 'MWC2', 'rsrc');

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource -> data model conversion

// ---------------------------------------------------------------------------
//		* MakeTypeSpec											[protected]
// ---------------------------------------------------------------------------
//	Given a resource type, choose the appropriate routine to make a
//	class specification. Return true if a spec was successfully built.

Boolean
VPTypeList::MakeTypeSpec(
	LStream&		inStream,
	ResType			inResType,
	const FSSpec&	inSourceFile)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Be sure this doesn't trigger undo/redo.

	StDMTransactionHider hide;

	// Build the description.

	if (inResType == 'CTYP')
		return MakeSpecFromCTYP(inStream, inSourceFile);
	else if (inResType == 'CPPb')
		return MakeSpecFromCPPb(inStream, inSourceFile);
	else if (inResType == 'CLSS')
		return MakeSpecFromCLSS(inStream, inSourceFile);
	else
		return false;

}


// ---------------------------------------------------------------------------
//		* MakeSpecFromCPPb										[protected]
// ---------------------------------------------------------------------------
//	Given a CPPb resource, make a type specification. Return true if
//	successful.

Boolean
VPTypeList::MakeSpecFromCPPb(
	LStream&		inStream,
	const FSSpec&	inSourceFile)
{

	// we  no longer support CPPb's
	
	DoNoCPPbSupportDialog(inSourceFile.name);
	return false;	
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Read CPPb header.
	
	ClassIDT classID;
	ClassIDT dataModelParentID;
	ResIDT iconID;
	ResIDT ppobID;
	Str255 className;
	UInt8 displayFlags;

	inStream	>> classID
				>> dataModelParentID
				>> iconID
				>> ppobID
				>> className
				>> displayFlags;

	// If reading the application file, ignore non-PowerPlant types.
	
	if (mTypesAreBuiltIn) {
		ClassIDT tempID = classID;
		SInt16 bytes = 4;
		while (bytes--) {
			char thisChar = tempID & 0xFF;
			if ((thisChar >= 'A') && (thisChar <= 'Z'))
				return false;
			tempID >>= 8;
		}
	}

	// Build class entry.
	
	VPTypeEntryPtr thisEntry;
	thisEntry = new VPTypeEntry;
	ValidateObject_(thisEntry.GetObject());

	thisEntry->FinishCreate();
	thisEntry->mClassID = classID;
	thisEntry->mClassName = className;
	thisEntry->mParentClassID = dataModelParentID;

	// Place this class in the type list hierarchy.
	
	VETypeEntry* previousEntry = nil;
	VETypeEntry* dataModelParent = nil;
	VETypeEntry* runtimeParent = nil;

	PrepareTypeEntry(thisEntry, dataModelParent, runtimeParent, previousEntry);

	// Read field descriptions.
	
	ReadItemListFromCPPb(thisEntry, inStream);

	// Read fields that got tacked on after the field descriptions.

	ResIDT defaultPPobID;
	inStream	>> thisEntry->mDragWidth
				>> thisEntry->mDragHeight
				>> defaultPPobID;

	// If there's a class icon, grab it now. Detach resources so we don't
	// get toasted when the resource file goes away.
	
	if (iconID != 0) {
		thisEntry->mClassIcon =
			UIconUtilities::Get1DetachedIconSuite(UIconUtilities::GetRemappedIconID(iconID), svAllSmallData);
#if PP_Target_Carbon
		ThrowIfNil_(thisEntry->mClassIcon);
#else
		ValidateHandle_(thisEntry->mClassIcon);
#endif
	}

	// If there's a default PPob, use the prototype to read its values.

	if (defaultPPobID != 0) {
		Handle defaultPPob = nil;
		defaultPPob = ::Get1Resource('PPob', defaultPPobID);
		if (defaultPPob != nil) {
			ValidateHandle_(defaultPPob);
			::HNoPurge(defaultPPob);
			::DetachResource(defaultPPob);
			LHandleStream ppobStream(defaultPPob);
			ppobStream.SetMarker(14, streamFrom_Start);
			thisEntry->mPrototype->ReadStreamData(&ppobStream, 'PPob');
		}
	}

	// Determine if this class must be at the root.

	switch (classID) {
		case LWindow::class_ID:
		case LPrintout::class_ID:
		case LGrafPortView::class_ID:
			thisEntry->mMustBeRoot = true;
	}

	// Put this into the type list.
	
	InstallTypeEntry(thisEntry, runtimeParent, previousEntry);

	// Signal a successful build.

	return true;

}


// ---------------------------------------------------------------------------
//		* MakeSpecFromCLSS										[protected]
// ---------------------------------------------------------------------------
//	Given a CLSS resource (Constructor 1.0 format), make a type spec.
//	Return true if successful.

Boolean
VPTypeList::MakeSpecFromCLSS(
	LStream&		inStream,
	const FSSpec&	inSourceFile)
{
	#pragma unused (inStream)
	#pragma unused (inSourceFile)
	
	return false;			//! TEMPORARY: don't
}


// ===========================================================================

#pragma mark -
#pragma mark ** item list readers

// ---------------------------------------------------------------------------
//		* ReadItemListFromCPPb									[protected]
// ---------------------------------------------------------------------------
//	Read item list from a CPPb type description.

void
VPTypeList::ReadItemListFromCPPb(
	VPTypeEntry*	inEntry,
	LStream&		inStream)
{

	// we no longer support 'CPPb' resources
	
	DoNoCPPbSupportDialog();
	return;

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inEntry);
	ValidateObject_(inEntry->mPrototype.GetObject());

	// See if we need to create a subpanes list and/or attachments list.
	
	Boolean needsSubpanes = false;
	Boolean needsAttachments = false;
	
	switch (inEntry->GetClassID()) {
		case LPane::class_ID:
			needsAttachments = true;
			break;
		case LView::class_ID:
		case LScroller::class_ID:
			needsSubpanes = true;
			break;
		case LWindow::class_ID:
		case LPrintout::class_ID:
			needsAttachments = needsSubpanes = true;
			break;
	}

	// See how many items there are. If none, we can quit now.
	
	SInt16 numItems;
	inStream >> numItems;
	if ((numItems == 0) && !(needsAttachments || needsSubpanes))
		return;
	
	// Create a container for the attributes.
	
	TSharablePtr<DMBitfieldAttribute> bitContainer;
	SInt16 bitBits = 0;

	DMContainerAttributePtr container;
	container = MakeContainerAttribute(inEntry);
	ValidateObject_(container.GetObject());
	
	// Create attachments list (if required).
	
	if (needsAttachments) {
		DMContainerAttribute* attr = new VESubModelContainer;
		ValidateObject_(attr);
		attr->SetTitle("\pAttachments");						//! TEMPORARY: grab from STR#
		attr->SetKey(pPaneAttachmentsList);
		attr->SetIncludeInDisplay(false);
		attr->SetSubObjectList(true);
		attr->SetMemberClassID(VPModelAttachment::class_ID);
		inEntry->mPrototype->InsertElementAt(LArray::index_Last, attr, true);
	}
	
	// Create subpanes list (if required).
	
	if (needsSubpanes) {
		DMContainerAttribute* attr = new VESubModelContainer;
		ValidateObject_(attr);
		attr->SetTitle("\pSubpanes");							//! TEMPORARY: grab from STR#
		attr->SetKey(pPaneSubViewList);
		attr->SetIncludeInDisplay(false);
		attr->SetSubObjectList(true);
		attr->SetMemberClassID(VPModelPane::class_ID);			//! TEMPORARY: grab from VPModelPane
		inEntry->mPrototype->InsertElementAt(LArray::index_Last, attr, true);
	}
	
	// Read the container descriptions.

	FourCharCode streamSelector = 'PPob';
	while (numItems--) {
	
		// Read title, field key, and type from stream.
		
		Str255 theItemTitle;
		FourCharCode theItemType;
		FourCharCode theItemFieldKey;

		inStream	>> theItemTitle
					>> theItemFieldKey
					>> theItemType;

		Boolean isBitfield = ((theItemType & 0xFFFF0000) == 0x42420000);
		
		// The next field is a switch. If on, there's a display specification.
		// If off, there's a 16-bit default value.
		
		SInt16 numValueLabels = 0;
		SInt32 theItemDefaultValue = 0;
		UInt32 theItemDisplayFlags = 0;

		SInt16 displaySpecSwitch;
		inStream >> displaySpecSwitch;

		switch (displaySpecSwitch) {

			case 1:
				inStream	>> theItemDefaultValue
							>> theItemDisplayFlags
							>> numValueLabels;
				break;

			case 0:
				SInt16 temp;
				inStream >> temp;
				theItemDefaultValue = temp;
				break;

			default:
				Throw_(1);				// probably an old CPPb, but we don't know for sure
		}
		
		// Close out a bitfield if appropriate.
		
		if ((bitContainer != nil) && (!isBitfield || (bitBits > 31))) {
			ValidateObject_(bitContainer.GetObject());
			bitContainer->SetStreamSelector(streamSelector);
			bitContainer->SetValueSize((bitBits + 7) >> 3);
			container->InsertElementAt(LArray::index_Last, bitContainer, true);
			bitContainer = nil;
			bitBits = 0;
		}
		
		// See if this is a "switch streams" flag.
		
		if (theItemType == '~str') {
			streamSelector = theItemDefaultValue;
			continue;
		}
		
		// Create the attribute.
		
		DMAttributePtr attr;
		switch (theItemType) {
			
			case 'TBXB':
			case 'TBX2': {
				DMToolboxBooleanAttribute* tbxAttr = new DMToolboxBooleanAttribute;
				ValidateObject_(tbxAttr);
				attr = tbxAttr;
				tbxAttr->SetValueSize(1);
				tbxAttr->SetSigned(false);
				tbxAttr->SetValueBytes(theItemType == 'TBX2' ? 2 : 1);
				break;
			}
			
			case 'COLR':
				attr = new DMRGBColorAttribute;
				break;

			case 'PSTR':
			case 'ESTR': {
				DMStringAttribute* strAttr = new DMStringAttribute;
				ValidateObject_(strAttr);
				attr = strAttr;
				if (theItemType == 'ESTR')
					strAttr->SetEvenPadded(true);
				if (theItemDisplayFlags & 0x100)
					strAttr->SetAcceptCR(true);
				break;
			}
			
			case 'DBYT':
			case 'UBYT':
			case 'DWRD':
			case 'UWRD':
			case 'DLNG':
			case 'ULNG':
			case 'TNAM':
			case 'BOOL': {
				DMIntegerAttribute* intAttr = new DMIntegerAttribute;
				ValidateObject_(intAttr);
				attr = intAttr;
				intAttr->SetIntValue(theItemDefaultValue);
				switch (theItemType) {
					case 'DBYT':
						intAttr->SetValueSize(8);
						intAttr->SetSigned(true);
						break;
					case 'UBYT':
						intAttr->SetValueSize(8);
						intAttr->SetSigned(false);
						break;
					case 'DWRD':
						intAttr->SetValueSize(16);
						intAttr->SetSigned(true);
						break;
					case 'UWRD':
						intAttr->SetValueSize(16);
						intAttr->SetSigned(false);
						break;
					case 'DLNG':
						intAttr->SetValueSize(32);
						intAttr->SetSigned(true);
						break;
						
					case 'TNAM':
						intAttr->SetAlwaysText(true);
						// fall thru;
					case 'ULNG':
						intAttr->SetValueSize(32);
						intAttr->SetSigned(false);
						break;
					case 'BOOL':
						intAttr->SetValueSize(1);
						intAttr->SetSigned(false);
						break;
				}
				
				if (theItemDisplayFlags & 0x10) {
					intAttr->SetCanBeText(true);
					intAttr->SetAlwaysText(false);
				}
			}
		}
				
		// See if this is a bitfield.

		if (isBitfield) {
		
			// It is, make sure we have a bitfield container attribute.
		
			if (bitContainer == nil) {
				bitContainer = new DMBitfieldAttribute;
				ValidateObject_(bitContainer.GetObject());
			}
			
			// The item type may specify a bit depth (in the form 'BBnn' where nn > 0 and nn < 32).
			
			UInt16 bitDepth = 1;
			char itemTypeByte3 = (theItemType >> 8) & 0xFF;
			char itemTypeByte4 = theItemType & 0xFF;
			
			if ((itemTypeByte3 >= '0') && (itemTypeByte3 <= '3') && (itemTypeByte4 >= '0') && (itemTypeByte4 <= '9')) {
				bitDepth = ((itemTypeByte3 - '0') * 10) + (itemTypeByte4 - '0');
				if (bitDepth < 1)
					bitDepth = 1;
				if (bitDepth > 31)
					bitDepth = 31;
			}
			bitBits += bitDepth;
			
			// Okay, now create an attribute for this bit.
			
			DMIntegerAttribute* intAttr = new DMIntegerAttribute;
			ValidateObject_(intAttr);
			attr = intAttr;
			intAttr->SetIntValue(theItemDefaultValue);
			intAttr->SetValueSize(bitDepth);
			intAttr->SetSigned(false);
			
		}
		
		// Finish configuring the attribute.
		
		ValidateObject_(attr.GetObject());
		attr->SetKey(theItemFieldKey);
		attr->SetStreamSelector(streamSelector);
		attr->SetTitle(theItemTitle);
		
		// Check display flags for this attribute.
		
		if (theItemDisplayFlags & 0x80)		// not in stream
			attr->SetIncludeInStream(false);
		if (theItemDisplayFlags & 0x20)
			attr->SetIncludeInDisplay(false);
		
		// Read value labels.
		
		if (numValueLabels > 0) {
			
			// Make a container for value labels.
			
			DMContainerAttributePtr valContainer;
			valContainer = new DMContainerAttribute;
			ValidateObject_(valContainer.GetObject());
			
			// Read value labels.
			
			while (numValueLabels--) {
				SInt32 value;
				SInt32 paneID;				// now ignored
				Str255 label;

				inStream	>> value
							>> paneID		// read and discard
							>> label;
				
				TSharablePtr<DMValueLabel> dmLabel;
				dmLabel = new DMValueLabel;
				ValidateObject_(dmLabel.GetObject());
				
				dmLabel->SetValue(value);
				dmLabel->SetLabel(label);
				
				valContainer->InsertElementAt(LArray::index_Last, dmLabel, true);
			}
			
			// Now attach the label container to the attribute.
			
			DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>((DMAttribute*) attr));
			if (intAttr != nil) {
				ValidateObject_(intAttr);
//				intAttr->SetValueLabels(valContainer);
			}
		}

		// Install it in container attribute.
		
		if (!isBitfield)
			container->InsertElementAt(LArray::index_Last, attr, true);
		else if (bitContainer != nil)
			bitContainer->InsertElementAt(LArray::index_Last, attr, true);
		
	}

	// Close out a bitfield if appropriate.
	
	if (bitContainer != nil) {
		bitContainer->SetStreamSelector(streamSelector);
		bitContainer->SetValueSize((bitBits + 7) >> 3);
		container->InsertElementAt(LArray::index_Last, bitContainer, true);
		bitContainer = nil;
		bitBits = 0;
	}
}


// ---------------------------------------------------------------------------
//		* ReadItemListFromCLSS									[protected]
// ---------------------------------------------------------------------------
//	Read item list from a CLSS resource.

void
VPTypeList::ReadItemListFromCLSS(
	VPTypeEntry*	inEntry,
	LStream&		inStream)
{
	#pragma unused (inEntry)
	#pragma unused (inStream)
	
	//! TEMPORARY: don't
}


// ===========================================================================

#pragma mark -
#pragma mark ** data model connection

// ---------------------------------------------------------------------------
//		* MakeModelObjectForClass								[protected]
// ---------------------------------------------------------------------------
//	Create a prototype data model object for this class.

VEModelObject*
VPTypeList::MakeModelObjectForClass(
	ClassIDT	inClassID)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Most classes use VPModelObject. A few use other classes, where
	// some specialization is required.

	switch (inClassID) {

		case LPane::class_ID:
			return new VPModelPane;

		case LRadioGroup::class_ID:
			return new VPModelRadioGroup;

		case 'atch':						// LAttachment::class_ID
		case LTabGroup::class_ID:
			return new VPModelAttachment;

		case LView::class_ID:
		case LPrintout::class_ID:
			return new VPModelView;

		case LScroller::class_ID:
		case 'sclv':	// ScrollerView
			return new VPModelScroller;
		
		case LWindow::class_ID:
			return new VPModelWindow;
		
		case LDialogBox::class_ID:
			return new VPModelDialogBox;

		default:
			return nil;
	}
}


// ---------------------------------------------------------------------------
//		* FixRuntimeParent										[protected]
// ---------------------------------------------------------------------------
//	Some classes have different runtime parents than is
//	reflected in the data model. We fix those discrepancies here.

void
VPTypeList::FixRuntimeParent(
	ClassIDT	inClassID,
	ClassIDT&	ioRuntimeParent)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Some classes have different runtime parents than is
	// reflected in the data model. We fix any discrepancies here.

	switch (inClassID) {

		case LWindow::class_ID:
		case LPrintout::class_ID:
		case LScroller::class_ID:
		case 'sclv':		// ScrollerView
			ioRuntimeParent = LView::class_ID;
			break;
			
		case LStdButton::class_ID:
		case LStdCheckBox::class_ID:
		case LStdPopupMenu::class_ID:
		case LStdRadioButton::class_ID:
			ioRuntimeParent = LStdControl::class_ID;
			break;
		
		case LGACheckbox::class_ID:
		case LGATextButton::class_ID:
		case LGAPopup::class_ID:
		case LGAPushButton::class_ID:
		case LGARadioButton::class_ID:
		case LTextButton::class_ID:
/*
		case 'bbut':	// BevelButton
		case 'chbx':	// CheckBox
		case 'clck':	// Clock
		case 'dtri':	// DisclosureTriangle
		case 'ictl':	// IconControl
		case 'popb':	// PopupButton
		case 'push':	// PushButton
		case 'rdbt':	// RadioButton
		case 'etxt':	// EditText
*/
			ioRuntimeParent = 'cntl';
			break;

		// JCD 980617 - place all Appearance controls under the new
		// Appearance page.
		
		case 'winh':	// LWindowHeader
		case 'tgbx':	// LTextGroupBox
		case 'tabs':	// LTabsControl
		case 'stxt':	// LStaticText
//		case 'slid':	// LSlider
		case 'sepl':	// LSeparatorLine
//		case 'sbar':	// LScrollBar
		case 'rdbt':	// LRadioButton
		case 'push':	// LPushButton
//		case 'pbar':	// LProgressBar
		case 'pgbx':	// LPopupGroupBox
		case 'popb':	// LPopupButton
		case 'plcd':	// LPlacard
		case 'picd':	// LPictureControl
//		case 'larr':	// LLittleArrows
		case 'iwel':	// LImageWell
		case 'ictl':	// LIconControl
		case 'etxt':	// LEditText
		case 'dtri':	// LDisclosureTriangle
		case 'cbbt':	// LCmdBevelButton
		case 'clck':	// LClock
		case 'cbgb':	// LCheckBoxGroupBox
		case 'chbx':	// LCheckBox
		case 'carr':	// LChasingArrows
		case 'bbut':	// LBevelButton
			ioRuntimeParent = 'cpan';	// LControlPane
			break;

	}
}


// ---------------------------------------------------------------------------
//		* ChooseDrawingAgent									[protected]
// ---------------------------------------------------------------------------
//	Create the appropriate drawing agent for a given PowerPlant class.

#ifndef __GNUC__
#pragma warn_illtokenpasting off
#endif

VE_DrawAgentCreatorFunc
VPTypeList::ChooseDrawingAgent(
	ClassIDT	inClassID)
{

	#define _AgentFunc(classID) \
		case classID :: class_ID: \
			return VP##classID::CreateAgent;

	switch (inClassID) {
		
		_AgentFunc(LButton);
		_AgentFunc(LCaption);
		_AgentFunc(LCicnButton);
		_AgentFunc(LEditField);
		_AgentFunc(LGABox);
		_AgentFunc(LGACheckbox);
		_AgentFunc(LGAColorSwatch);
		_AgentFunc(LGADisclosureTriangle);
		_AgentFunc(LGAEditField);
		_AgentFunc(LGAFocusBorder);
		_AgentFunc(LGAIconButton);
		_AgentFunc(LGAIconButtonPopup);
		_AgentFunc(LGAIconSuite);
		_AgentFunc(LGAIconSuiteControl);
		_AgentFunc(LGAIconTextButton);
		_AgentFunc(LGALittleArrows);
		_AgentFunc(LGAPopup);
		_AgentFunc(LGAPrimaryBox);
		_AgentFunc(LGAPushButton);
		_AgentFunc(LGARadioButton);
		_AgentFunc(LGASecondaryBox);
		_AgentFunc(LGASeparator);
		_AgentFunc(LGATabPanel);
		_AgentFunc(LGATextButton);
		_AgentFunc(LGroupBox);
		_AgentFunc(LListBox);
		_AgentFunc(LPane);
		_AgentFunc(LPicture);
		_AgentFunc(LPrintout);
		_AgentFunc(LScroller);
		_AgentFunc(LStdControl);
		_AgentFunc(LStdPopupMenu);
		_AgentFunc(LTextButton);
		_AgentFunc(LToggleButton);
		_AgentFunc(LView);
		_AgentFunc(LWindow);
		
		_AgentFunc(LBevelButton);
		_AgentFunc(LChasingArrows);
		_AgentFunc(LCheckBox);
		_AgentFunc(LCheckBoxGroupBox);
		_AgentFunc(LDisclosureTriangle);
		_AgentFunc(LEditText);
		_AgentFunc(LIconControl);
		_AgentFunc(LImageWell);
		_AgentFunc(LLittleArrows);
		_AgentFunc(LPictureControl);
		_AgentFunc(LPlacard);
		_AgentFunc(LPopupButton);
		_AgentFunc(LPopupGroupBox);
		_AgentFunc(LProgressBar);
		_AgentFunc(LPushButton);
		_AgentFunc(LRadioButton);
		_AgentFunc(LScrollBar);
		_AgentFunc(LSeparatorLine);
		_AgentFunc(LSlider);
		_AgentFunc(LStaticText);
		_AgentFunc(LTabsControl);
		_AgentFunc(LTextGroupBox);
		_AgentFunc(LWindowHeader);
		
		_AgentFunc(LPageController);
		_AgentFunc(LScrollerView);
		
		case LCmdBevelButton::class_ID:	// Same as regular BevelButton
			return VPLBevelButton::CreateAgent;

			// special cases
		case 'cntl':					// special case, since LControl doesn't have a class ID
			return VPLControl::CreateAgent;

		case LStdButton::class_ID:		// special case, since CPPbs don't follow normal inheritance paths
		case LStdCheckBox::class_ID:
		case LStdRadioButton::class_ID:
			return VPLStdControl::CreateAgent;
		
		default:
			return nil;
	}
}


// ---------------------------------------------------------------------------
//		* HandleResourceDataChanged								[protected]
// ---------------------------------------------------------------------------
//	Called whenever a resource's data changes. If the resource is a CTYP,
//	we may update the global type list.

void
VPTypeList::HandleResourceDataChanged(
	const RMMap_ChangeMessage*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find the affected resource.
	
	RFResource* rsrc = (dynamic_cast<RFResource*>(inMessage->mResource));
	ValidateSimpleObject_(rsrc);
	
	// See if it's a CTYP. If not, ignore it.
	
	if (rsrc->GetResType() != 'CTYP')
		return;
	
	// It is. Update the type list.
	
	StDMTransactionHider hideThis;
	
	// Get the resource data.
	
	LHandleStream resDataStream(rsrc->GetResData());
	
	// Get the file containing this resource.
	
	FSSpec spec = { 0, 0, "\p" };
	LFile* file = rsrc->GetRFMap().GetMainFile();
	if (file != nil) {
		ValidateObject_(file);
		file->GetSpecifier(spec);
	}
	
	// Create the new type spec.

	try {
		MakeTypeSpec(resDataStream, 'CTYP', spec);
	}
	catch(...) {
		// ignore errors
	}
}