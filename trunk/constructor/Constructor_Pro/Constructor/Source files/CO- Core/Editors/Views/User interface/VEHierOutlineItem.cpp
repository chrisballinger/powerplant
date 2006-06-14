// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEHierOutlineItem.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/13/96
//     $Date: 2006/01/18 01:33:16 $
//  $History: VEHierOutlineItem.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:03
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Updated GetDrawContentsSelf to use
//	VEModelObject::GetDisplayableObjectType.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:10
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Now draws non-visual objects (attachments, etc.) in non-bold.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:23
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added support for dynamic columns in hierarchy window.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:41
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added class. (Refactoring hierarchy view to generic view editor.)
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEHierOutlineItem.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Editors : Views : User interface
#include "VEHierTable.h"

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"


// ===========================================================================

#pragma mark *** VEHierOutlineItem ***

// ---------------------------------------------------------------------------
//		* VEHierOutlineItem()
// ---------------------------------------------------------------------------
//	Constructor

VEHierOutlineItem::VEHierOutlineItem()
{
	mModelObject = nil;
}


// ---------------------------------------------------------------------------
//		* ~VEHierOutlineItem
// ---------------------------------------------------------------------------
//	Destructor

VEHierOutlineItem::~VEHierOutlineItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Return the attribute key that is drawn in a given cell.

DM_AttributeKey
VEHierOutlineItem::GetAttributeKeyForCell(
	TableIndexT		inColumn) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_((const_cast<VEHierOutlineItem*>(this))->mOVTable);
	
	// Ask the table for the attribute key.
	
	VEHierTable* hierTable = (dynamic_cast<VEHierTable*>((const_cast<VEHierOutlineItem*>(this))->mOVTable));
	ValidateObject_(hierTable);

	return hierTable->GetAttributeKeyForColumn(inColumn);

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing setup

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Return the information that is to be drawn for a given cell.

void
VEHierOutlineItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(mModelObject);
	
	// Create a temporary string.

	LStr255 tempStr;

	// Dispatch column number.

	switch (inCell.col) {
	
		// Column 1: Class icon and name.
	
		case 1: {
			VETypeEntry* type = mModelObject->GetTypeEntry();
			ValidateObject_(type);
			ioDrawContents.outIconSuite = type->GetClassIcon();

			mModelObject->GetDisplayableObjectType(tempStr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			if (mModelObject->IsVisualObject())
				ioDrawContents.outTextTraits.style |= bold;

			ioDrawContents.outHasIcon = true;
			ioDrawContents.outShowSelection = true;
			break;
		}
		
		// Other columns: Dynamically built.
		
		default:
			VEHierTable* hierTable = (dynamic_cast<VEHierTable*>(mOVTable));
			ValidateObject_(hierTable);
			
			DMAttribute* attr = mModelObject->FindAttributeByKey(
									hierTable->GetAttributeKeyForColumn(inCell.col));
			if (attr != nil) {
				attr->GetTextValue(tempStr);
				LString::CopyPStr(tempStr, ioDrawContents.outTextString);
				ioDrawContents.outCanDoInPlaceEdit = true;
			}
			break;
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Accept the pointer to the data model object we're drawing.

void
VEHierOutlineItem::SetElementSelf(
	DMElement*	inElement)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a VEModelObject.

	mModelObject = (dynamic_cast<VEModelObject*>(inElement));
	ValidateObject_(mModelObject);

	// Specify container for submodels.
	
	DMElement* subPanes = mModelObject->GetSubObjectList();
	if (subPanes != nil) {
		ValidateObject_(subPanes);
		mExpanded = true;
		SetContainer(subPanes);
	}
}
