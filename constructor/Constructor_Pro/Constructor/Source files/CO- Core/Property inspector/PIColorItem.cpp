// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIColorItem.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/02/96
//     $Date: 2006/01/18 01:33:20 $
//	$History: PIColorItem.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/13/96   Time: 18:23
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed a bug in FinishCreate that caused the color popup to fail.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:34
//	Updated in $/Constructor/Source files/Property inspector
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 17:51
//	Updated in $/Constructor/Source files/Property inspector
//	Changed resource ID allocation to avoid conflict with Display classes
//	(Eric) folder. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:22
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

#include "PIColorItem.h"

	// Core : Data model : Attributes
#include "DMRGBColorAttribute.h"

	// Core : Data model : Undo-redo
#include "StDMAttributeSetter.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// MacOS : Editors : Bitmaps : PT popups
#include "SUColorPane.h"

	// MacOS : Editors : Bitmaps : PT graphics utils
#include "SUColorUtils.h"


// ===========================================================================

#pragma mark *** PIColorItem ***

// ---------------------------------------------------------------------------
//		* PIColorItem()
// ---------------------------------------------------------------------------
//	Constructor

PIColorItem::PIColorItem()
{
	mAttributeRGB = nil;
	mColorPane = nil;
}


// ---------------------------------------------------------------------------
//		* ~PIColorItem
// ---------------------------------------------------------------------------
//	Destructor

PIColorItem::~PIColorItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** display synchronization


// ---------------------------------------------------------------------------
//		* SetValueFromAttributeSelf								[protected]
// ---------------------------------------------------------------------------
//	Update the color popup from the attribute.

void
PIColorItem::SetValueFromAttributeSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttributeRGB);
	ValidateObject_(mColorPane);

	// Get color from attribute; send it to popup.

	RGBColor color = mAttributeRGB->GetRGBColorValue();
	mColorPane->SetColor(SUColorUtils::RGBToColor32(color));

}


// ---------------------------------------------------------------------------
//		* SetValueFromDisplaySelf								[protected]
// ---------------------------------------------------------------------------
//	Update the attribute from the color popup.

void
PIColorItem::SetValueFromDisplaySelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	ValidateObject_(mOVTable);
	ValidateObject_(mColorPane);

	// Get color from popup; set attribute's value to match it.

	StDMAttributeSetter setter(mAttribute);
	setter.SetUndoerHost(mOVTable);
	setter.SetSelection(mOVTable->GetSelection());
	setter.SetRGBColorValue(SUColorUtils::Color32ToRGB(mColorPane->GetColor()));

}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Disable the color popup.

void
PIColorItem::DisableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mColorPane);
	
	// Disable the popup.
	
	mColorPane->Disable();

}


// ---------------------------------------------------------------------------
//		* EnableSelf											[protected]
// ---------------------------------------------------------------------------
//	Enable the color popup.

void
PIColorItem::EnableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mColorPane);
	
	// Enable the color popup.
	
	mColorPane->Enable();
	
}


// ---------------------------------------------------------------------------
//		* FinishCreate											[protected]
// ---------------------------------------------------------------------------
//	Create the color popup pane.

void
PIColorItem::FinishCreate()
{

	// Validate pointers.

	ValidateThis_();
	
	// Load checkbox from PPob resource.
	
	STableCell cell(FindRowForItem(), 2);
	mColorPane = (dynamic_cast<SUColorPane*>(CreatePaneForCell(cell, 21102)));
	ValidateObject_(mColorPane);

	mColorPane->MoveBy(0, -1, false);

	// Configure popup's color table. Currently hard-wired for 256 colors.

	#if Constructor_ForJava
		CTabHandle ctab = SUColorUtils::GetColorTable(128);
	#else
		CTabHandle ctab = SUColorUtils::GetColorTable(8);
	#endif
	ValidateHandle_((Handle) ctab);

	mColorPane->SetColorTable(ctab);

	// Configure value.
	
	SetValueFromAttributeSelf();

	// Set up listener relationship.
	
	mColorPane->AddListener(this);
	mColorPane->SetPaneID(msg_DisplayValueChanged);		// since SUColorPane broadcasts its pane ID
//	mColorPane->SetValueMessage(msg_DisplayValueChanged);
	
}

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this is a color attribute.

void
PIColorItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// Sanity check: Make sure we have an RGB color attribute.

	mAttributeRGB = (dynamic_cast<DMRGBColorAttribute*>(inElement));
	ValidateObject_(mAttributeRGB);

	PIAttributeItem::SetElementSelf(inElement);

}

#include "OVItemFactory.h"
