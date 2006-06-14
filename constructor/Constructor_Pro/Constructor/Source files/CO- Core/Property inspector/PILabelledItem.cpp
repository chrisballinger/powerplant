// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PILabelledItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 09/27/96
//     $Date: 2006/01/18 01:33:23 $
//	$History: PILabelledItem.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:38
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed a bug in SetValueFromAttributeSelf that could allow cell text to
//	be out of sync with attribute value.
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:21
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/21/96   Time: 16:12
//	Updated in $/Constructor/Source files/Property inspector
//	Switched from LGAPopup to LGAMiniArrowPopup in PI window. (Bug fix
//	#1132.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/26/96   Time: 18:00
//	Updated in $/Constructor/Source files/Property inspector
//	Changed resource ID allocation to avoid conflict with Display classes
//	(Eric) folder.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/25/96   Time: 19:08
//	Updated in $/Constructor/Source files/Property inspector
//	Improved placement of value label popup. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:38
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for attribute's enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:08
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PILabelledItem.h"

	// Core : Data model : Attributes
#include "DMLabelledIntegerAttribute.h"

	// Core : Data model : Undo-redo
#include "StDMAttributeSetter.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Property inspector
#include "PILabelPopup.h"

	// PowerPlant : CA : Outline table
#include <LOutlineTable.h>


// ===========================================================================

#pragma mark *** PILabelledItem ***

// ---------------------------------------------------------------------------
//		* PILabelledItem()
// ---------------------------------------------------------------------------
//	Constructor

PILabelledItem::PILabelledItem()
{
	mPopup = nil;
	mAttributeLab = nil;
}


// ---------------------------------------------------------------------------
//		* ~PILabelledItem
// ---------------------------------------------------------------------------
//	Destructor

PILabelledItem::~PILabelledItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* FinishCreate											[protected]
// ---------------------------------------------------------------------------
//	Create popup menu for labels.

void
PILabelledItem::FinishCreate()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOutlineTable);
	ValidateObject_(mAttributeLab);

	// Load popup from PPob resource.
	
	STableCell cell(FindRowForItem(), 2);
	mPopup = (dynamic_cast<PILabelPopup*>(CreatePaneForCell(cell, 21104)));
	ValidateObject_(mPopup);
	mPopup->MoveBy(0, 1, false);

	// Place mPopup in right edge of cell.

	Rect bounds;	
	mOutlineTable->GetLocalCellRect(cell, bounds);
	mPopup->SetAttribute(mAttributeLab);

	// Set up listener relationship.
	
	mPopup->AddListener(this);
	mPopup->SetValueMessage(msg_DisplayValueChanged);

}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this is a labelled integer attribute.

void
PILabelledItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have an integer attribute.

	mAttributeLab = (dynamic_cast<DMLabelledIntegerAttribute*>(inElement));
	ValidateObject_(mAttributeLab);

	PIStringItem::SetElementSelf(inElement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing & editing

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Overriden to cut the label text short so there's room for the
//	label popup.

void
PILabelledItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();

	// Do normal draw contents...

	PIStringItem::GetDrawContentsSelf(inCell, ioDrawContents);
	
	// ... except that we truncate the label text to make space for
	// the label popup.
	
	if (inCell.col == 2)
		ioDrawContents.ioCellBounds.right -= 28;
	
}


// ---------------------------------------------------------------------------
//		* ConfigureInPlaceEdit									[protected]
// ---------------------------------------------------------------------------
//	Overriden to allow printing characters.

void
PILabelledItem::ConfigureInPlaceEdit(
	const STableCell&		/* inCell */,
	ResIDT&					/* outTextTraitsID */,
	SInt16&					/* outMaxChars */,
	UInt8&					outAttributes,
	TEKeyFilterFunc&		outKeyFilter)
{
	outAttributes &= ~editAttr_WordWrap;
	outKeyFilter = UKeyFilters::PrintingCharField;
}


// ---------------------------------------------------------------------------
//		* SetValueFromAttributeSelf								[protected]
// ---------------------------------------------------------------------------
//	Update display to match attribute's value.

void
PILabelledItem::SetValueFromAttributeSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mPopup);
	ValidateObject_(mAttributeLab);
	
	// Update IPE display.

	PIStringItem::SetValueFromAttributeSelf();
	
	// Update popup menu.
	
	mPopup->SetAttributeValue(mAttributeLab->GetIntValue());
	
	// Redraw the cell also.
	
	RefreshCell(2);
	
}


// ---------------------------------------------------------------------------
//		* SetValueFromDisplaySelf								[protected]
// ---------------------------------------------------------------------------
//	Pull value from popup menu.

void
PILabelledItem::SetValueFromDisplaySelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	ValidateObject_(mOVTable);
	ValidateObject_(mPopup);
	
	// Copy value from popup menu to attribute.
	
	StDMAttributeSetter setter(mAttribute);
	setter.SetUndoerHost(mOVTable);
	setter.SetSelection(mOVTable->GetSelection());
	setter.SetIntValue(mPopup->GetAttributeValue());
}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Disable the label popup.

void
PILabelledItem::DisableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mPopup);
	
	// Disable the label popup.

	mPopup->Disable();

}


// ---------------------------------------------------------------------------
//		* EnableSelf											[protected]
// ---------------------------------------------------------------------------
//	Enable the label popup.

void
PILabelledItem::EnableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mPopup);
	
	// Enable the label popup.

	mPopup->Enable();

}


// Includes added for building under MSVC++

#include "OVItemFactory.h"
#include "DMContainerAttribute.h"
