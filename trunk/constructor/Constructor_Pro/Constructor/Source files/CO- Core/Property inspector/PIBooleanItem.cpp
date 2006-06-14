// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIBooleanItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/24/96
//     $Date: 2006/01/18 01:33:20 $
//	$History: PIBooleanItem.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 17:44
//	Updated in $/Constructor/Source files/Property inspector
//	Changed resource ID allocation to avoid conflict with Display classes
//	(Eric) folder. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:20
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

#include "PIBooleanItem.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// Core : Data model : Undo-redo
#include "StDMAttributeSetter.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"
#include "LCheckBox.h"

// ===========================================================================

#pragma mark *** PIBooleanItem ***

// ---------------------------------------------------------------------------
//		* PIBooleanItem()
// ---------------------------------------------------------------------------
//	Constructor

PIBooleanItem::PIBooleanItem()
{
	mCheckBox = nil;
	mAttributeInteger = nil;
}


// ---------------------------------------------------------------------------
//		* ~PIBooleanItem
// ---------------------------------------------------------------------------
//	Destructor

PIBooleanItem::~PIBooleanItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** display synchronization

// ---------------------------------------------------------------------------
//		* SetValueFromAttributeSelf								[protected]
// ---------------------------------------------------------------------------
//	Update the checkbox's value.

void
PIBooleanItem::SetValueFromAttributeSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mCheckBox);
	ValidateObject_(mAttributeInteger);

	// Update checkbox display.
	
	mCheckBox->SetValue(mAttributeInteger->GetIntValue());

}


// ---------------------------------------------------------------------------
//		* SetValueFromDisplaySelf								[protected]
// ---------------------------------------------------------------------------
//	Update the attribute's value from the checkbox.

void
PIBooleanItem::SetValueFromDisplaySelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mCheckBox);
	ValidateObject_(mOVTable);
	ValidateObject_(mAttribute.GetObject());

	// Update attribute from checkbox's value.
	
	StDMAttributeSetter setter(mAttribute);
	setter.SetUndoerHost(mOVTable);
	setter.SetSelection(mOVTable->GetSelection());
	setter.SetIntValue(mCheckBox->GetValue());

}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Disable the checkbox.

void
PIBooleanItem::DisableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mCheckBox);
	
	// Disable the checkbox.

	mCheckBox->Disable();

}


// ---------------------------------------------------------------------------
//		* EnableSelf											[protected]
// ---------------------------------------------------------------------------
//	Enable the checkbox.

void
PIBooleanItem::EnableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mCheckBox);
	
	// Enable the checkbox.

	mCheckBox->Enable();

}


// ===========================================================================

#pragma mark -
#pragma mark ** display configuration

// ---------------------------------------------------------------------------
//		* FinishCreate											[protected]
// ---------------------------------------------------------------------------
//	Create the checkbox.

void
PIBooleanItem::FinishCreate()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	ValidateObject_(mAttributeInteger);
	
	// Load checkbox from PPob resource.
	
	STableCell cell(FindRowForItem(), 2);
	mCheckBox = (dynamic_cast<LCheckBox*>(CreatePaneForCell(cell, 21100)));
	ValidateObject_(mCheckBox);

	// Configure title.

	LStr255	attrTitle;
	mAttribute->GetTitle(attrTitle);
	mCheckBox->SetDescriptor(attrTitle);
	
	// Configure value.
	
	mCheckBox->SetValue(mAttributeInteger->GetIntValue());

	// Set up listener relationship.
	
	mCheckBox->AddListener(this);
	mCheckBox->SetValueMessage(msg_DisplayValueChanged);
	
}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this is an integer attribute.

void
PIBooleanItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have an integer attribute.

	mAttributeInteger = (dynamic_cast<DMIntegerAttribute*>(inElement));
	ValidateObject_(mAttributeInteger);

	PIAttributeItem::SetElementSelf(inElement);

}