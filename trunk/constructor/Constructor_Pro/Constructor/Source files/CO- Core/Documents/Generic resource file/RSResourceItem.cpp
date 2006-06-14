// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSResourceItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:33:01 $
//  $History: RSResourceItem.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:18
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/05/96   Time: 12:36
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Updated text traits ID for in-place editing.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:57
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSResourceItem.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Documents : Generic resource document
#include "RSResource.h"

	// Core : Editors : Generic editor
#include "REEditor.h"
#include "RETypeEntry.h"


// ===========================================================================

#pragma mark *** RSResourceItem ***

// ---------------------------------------------------------------------------
//		* RSResourceItem()
// ---------------------------------------------------------------------------
//	Constructor

RSResourceItem::RSResourceItem()
{
	mRSResource = nil;
	mTypeEntry = nil;
	mHasEditor = false;
}


// ---------------------------------------------------------------------------
//		* ~RSResourceItem
// ---------------------------------------------------------------------------
//	Destructor

RSResourceItem::~RSResourceItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Column 1 = resource name, 2 = resource ID.

DM_AttributeKey
RSResourceItem::GetAttributeKeyForCell(
	TableIndexT inColumn) const
{
	switch (inColumn) {
		case 1:
			return RSResource::attrKey_ResourceName;
		case 2:
			return RSResource::attrKey_ResourceID;
		default:
			return 0;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing setup

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Show resource name and type icon in column 1, resource ID in two.

void
RSResourceItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mRSResource);

	switch (inCell.col) {
	
		// Column 1: Resource type icon and resource name.
	
		case 1:
			ValidateObject_(mTypeEntry);
			ioDrawContents.outShowSelection = true;
			ioDrawContents.outHasIcon = true;
			ioDrawContents.outIconSuite = mTypeEntry->GetResTypeIconSuite();
			if (mHasEditor)
				ioDrawContents.outIconTransform |= kTransformOpen;
			ioDrawContents.outCanDoInPlaceEdit = true;
			mRSResource->GetResName(ioDrawContents.outTextString);
			if (ioDrawContents.outTextString[0] == 0)
#ifdef WINVER
				LString::CopyPStr((StringPtr)"\001-", ioDrawContents.outTextString);
#else
				LString::CopyPStr("\pÑ", ioDrawContents.outTextString);
#endif

			break;
		
		// Column 2: Resource ID.

		case 2: 
			if (mRSResource->GetResIDsSignificant()) {
				ioDrawContents.ioCellBounds.right -= 8;
				ioDrawContents.outTextTraits.justification = teJustRight;
				ioDrawContents.outCanDoInPlaceEdit = true;
				LStr255 resIDStr((SInt32) mRSResource->GetResID());
				LString::CopyPStr(resIDStr, ioDrawContents.outTextString);
			}
			break;

	}
}


// ---------------------------------------------------------------------------
//		* ConfigureInPlaceEdit									[protected]
// ---------------------------------------------------------------------------
//	Set the text traits to match the actual drawing.

void
RSResourceItem::ConfigureInPlaceEdit(
	const STableCell&	inCell,
	ResIDT&				outTextTraitsID,
	SInt16&				outMaxChars,
	UInt8&				outAttributes,
	TEKeyFilterFunc&	outKeyFilter)
{

	// Validate pointers.

	ValidateThis_();
	
	// Turn off word wrapping.
	
	outAttributes &= ~editAttr_WordWrap;

	// If editing resource ID, make sure it's properly configured
	// for editing numbers.

	if (inCell.col == 2) {
		outTextTraitsID = 15002;
		outKeyFilter = UKeyFilters::NegativeIntegerField;
		outMaxChars = 6;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource editing

// ---------------------------------------------------------------------------
//		* DoubleClick											[protected]
// ---------------------------------------------------------------------------
//	Pass the double-click to the table as a menu command.

void
RSResourceItem::DoubleClick(
	const STableCell&			/* inCell */,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{
	
	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Edit the selected resources.
	
	mOVTable->ProcessCommand(cmd_EditItem);

}


// ===========================================================================

#pragma mark -
#pragma mark ** has editor flag

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for "has editor" messages from the resource.

void
RSResourceItem::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Decode message.
	
	switch (inMessage) {
	
		case msg_HasEditor:
			RSResource* resource;
			resource = (RSResource*) ioParam;
			if (resource == mRSResource)
				CheckHasEditor();
			break;
		
		default:
			OVItem::ListenToMessage(inMessage, ioParam);
	
	}
}


// ---------------------------------------------------------------------------
//		* CheckHasEditor										[protected]
// ---------------------------------------------------------------------------
//	Ask the resource if it currently has an editor.

void
RSResourceItem::CheckHasEditor()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRSResource);
	
	// Ask the resource for its editor.
	
	void* editor = mRSResource->FindUserInterfaceObject(REEditor::objID_Editor);
	Boolean newHasEditor = (editor != nil);
	
	// If status changed, trigger a redraw.
	
	if ((newHasEditor != mHasEditor) && (mOutlineTable != nil))
		RefreshCell(1);
	
	mHasEditor = newHasEditor;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Cast the element to a PRResource* and choose the appropriate icon.

void
RSResourceItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a RSResource.

	mRSResource = (dynamic_cast<RSResource*>(inElement));
	ValidateObject_(mRSResource);
	CheckHasEditor();

	// Okay, it's cool. Find the resource type entry for this resource.
	
	mTypeEntry = RETypeEntry::FindResTypeEntry(mRSResource->GetResType());
	ValidateObject_(mTypeEntry);

}
