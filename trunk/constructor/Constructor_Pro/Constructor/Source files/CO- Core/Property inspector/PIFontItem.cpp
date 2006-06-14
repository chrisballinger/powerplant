// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIFontItem.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/31/96
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIFontItem.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed SetValueFromAttributeSelf to redraw text label.
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:21
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/26/96   Time: 17:58
//	Updated in $/Constructor/Source files/Property inspector
//	Changed resource ID allocation to avoid conflict with Display classes
//	(Eric) folder.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/25/96   Time: 19:03
//	Updated in $/Constructor/Source files/Property inspector
//	Improved placement of font name popup. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:27
//	Updated in $/Constructor/Source files/Property inspector
//	Added history information.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 16:24
//	Created
//	Comment: Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIFontItem.h"

	// Core : Data model : Attribute classes
#include "DMFontStringAttribute.h"

	// Core : Data model : Undo/redo
#include "StDMAttributeSetter.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Property inspector
#include "PIFontPopup.h"

	// PowerPlant : CA : Outline table
#include <LOutlineTable.h>


// ===========================================================================

#pragma mark *** PIFontItem ***

// ---------------------------------------------------------------------------
//		* PIFontItem()
// ---------------------------------------------------------------------------
//	Constructor

PIFontItem::PIFontItem()
{
	mAttributeFont = nil;
	mPopup = nil;
}


// ---------------------------------------------------------------------------
//		* ~PIFontItem
// ---------------------------------------------------------------------------
//	Destructor

PIFontItem::~PIFontItem()
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
PIFontItem::FinishCreate()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOutlineTable);
	ValidateObject_(mAttributeFont);

	// Load popup from PPob resource.
	
	STableCell cell(FindRowForItem(), 2);
	mPopup = (dynamic_cast<PIFontPopup*>(CreatePaneForCell(cell, 21106)));
	ValidateObject_(mPopup);

	// Place mPopup in right edge of cell.

	Rect bounds;	
	mOutlineTable->GetLocalCellRect(cell, bounds);
	mPopup->SetAttribute(mAttributeFont);

	// Set up listener relationship.
	
	mPopup->AddListener(this);
	mPopup->SetValueMessage(msg_DisplayValueChanged);

}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this is a labelled integer attribute.

void
PIFontItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have an integer attribute.

	mAttributeFont = (dynamic_cast<DMFontStringAttribute*>(inElement));
	ValidateObject_(mAttributeFont);

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
PIFontItem::GetDrawContentsSelf(
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
PIFontItem::ConfigureInPlaceEdit(
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
//	Font name has changed. Ensure that attribute reflects this change.

void
PIFontItem::SetValueFromAttributeSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mPopup);
	ValidateObject_(mAttributeFont);
	
	// Update IPE display.

	PIStringItem::SetValueFromAttributeSelf();
	
	// Update popup menu.
	
	mPopup->SetValueFromAttribute();
	
	// Redraw the cell also.
	
	RefreshCell(2);
	
}


// ---------------------------------------------------------------------------
//		* SetValueFromDisplaySelf								[protected]
// ---------------------------------------------------------------------------
//	Pull value from popup menu.

void
PIFontItem::SetValueFromDisplaySelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	ValidateObject_(mOVTable);
	ValidateObject_(mPopup);

	// Get the font name from the popup.

	LStr255 fontName;
	mPopup->GetFontName(fontName);

	// Send it to the attribute.

	StDMAttributeSetter setter(mAttribute);
	setter.SetUndoerHost(mOVTable);
	setter.SetSelection(mOVTable->GetSelection());
	setter.SetTextValue(fontName);
	
}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Disable the label popup.

void
PIFontItem::DisableSelf()
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
PIFontItem::EnableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mPopup);

	// Enable the label popup.	

	mPopup->Enable();

}
