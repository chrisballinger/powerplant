// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIStringItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/20/96
//     $Date: 2006/01/18 01:33:24 $
//	$History: PIStringItem.cpp $
//	
//	*****************  Version 18  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:49
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Hacked ConfigureTextTraits for Pilot fonts.
//	
//	*****************  Version 17  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 16  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 02/01/97   Time: 15:06
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Isolated Txtr references behind Constructor_ForMacOS macro.
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 01/29/97   Time: 14:03
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed a bug in ConfigureTextTraits WRT font size = 0. (Bug fix #1227.)
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 01/28/97   Time: 19:56
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Moved multi-line text support to LOutlineItem.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 01/28/97   Time: 17:46
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Upgraded to handle word-wrapping appropriately. (Bug fix #1211.)
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 12/21/96   Time: 15:47
//	Updated in $/Constructor/Source files/Property inspector
//	Added code to ConfigureTextTraits to constrain size, color, and drawing
//	mode. (Bug fix #1144.)
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 12/17/96   Time: 16:50
//	Updated in $/Constructor/Source files/Property inspector
//	Added code to ValueChanged to make sure string redraws when text traits
//	changes. (Bug fix #1139.)
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/17/96   Time: 00:37
//	Updated in $/Constructor/Source files/Property inspector
//	Updated UpdateRowSize to trigger refresh when row size changes. (Bug
//	fix #1138.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/08/96   Time: 16:26
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed a bug in ConfigureTextTraits that occurred when the text traits
//	attribute pointed to a deleted Txtr resource.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/05/96   Time: 17:28
//	Updated in $/Constructor/Source files/Property inspector
//	Updated to display and edit multi-line strings.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/05/96   Time: 12:50
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed ConfigureInPlaceEdit to use DMIntegerAttribute's new GetMinValue
//	and GetMaxValue accessors.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/04/96   Time: 20:32
//	Updated in $/Constructor/Source files/Property inspector
//	Updated to display PowerPlant pane titles in the style of their text
//	traits.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 09:43
//	Updated in $/Constructor/Source files/Property inspector
//	Moved ValueChanged method from PIAttributeItem (to reduce flicker in
//	other attribute items). Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:42
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for attribute's enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIStringItem.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"
#include "DMStringAttribute.h"

#if Constructor_ForPilot
	#include "DMPilotStringAttribute.h"
#endif

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Core : PowerPlant : CA : Utility classes
#include "UNewTextDrawing.h"

	// PowerPlant : GA : Grayscale utility classes
#include <UGAColorRamp.h>


	//! TEMPORARY includes
#include "RFMap.h"
#include "RFResource.h"
#include "RSResource.h"


// ===========================================================================

#pragma mark *** PIStringItem ***

// ---------------------------------------------------------------------------
//		* PIStringItem()
// ---------------------------------------------------------------------------
//	Constructor

PIStringItem::PIStringItem()
{
}


// ---------------------------------------------------------------------------
//		* ~PIStringItem
// ---------------------------------------------------------------------------
//	Destructor

PIStringItem::~PIStringItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* UpdateRowSize
// ---------------------------------------------------------------------------
//	Make sure the row is large enough to display the full text of this
// 	attribute.

void
PIStringItem::UpdateRowSize()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Calculate height of attribute text.
	
	TableIndexT row = FindRowForItem();
	STableCell cell(row, 2);
	SInt16 height = CalcRowHeightForCell(cell);

	// Set vertical size based on font info.

	if (height < 15)
		height = 15;
	
	if (mOVTable->GetRowHeight(row) != height) {

		mOVTable->SetRowHeight(height, row, row);

		STableCell topLeftCell(row, 1);
		STableCell botRightCell;
		mOVTable->GetTableSize(botRightCell.row, botRightCell.col);
		mOVTable->RefreshCellRange(topLeftCell, botRightCell);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Called when a data model element changes. If it is the attribute
//	we are displaying, then update the display.

void
PIStringItem::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inMessage->mAttribute);

	// See if this is our attribute.
	
	if (inMessage->mAttribute == mAttribute) {

		ValidateObject_(mAttribute.GetObject());
		ValidateObject_(mOVTable);

		SetValueFromAttribute();
		STableCell cell(FindRowForItem(), 2);
		mOVTable->RefreshCell(cell);
	}
#if Constructor_ForMacOS
	else if (inMessage->mAttribute->GetKey() == 'pTXR') {		//! TEMPORARY: ugly target-specific knowledge
		SetValueFromAttributeSelf();
	}
#elif Constructor_ForPilot
	else if (inMessage->mAttribute->GetKey() == 'pFNT') {		//! TEMPORARY: ugly target-specific knowledge
		SetValueFromAttributeSelf();
	}
#endif

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Draw the attribute's value in the right-hand column.

void
PIStringItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());

	// Dispatch column number.

	switch (inCell.col) {

		case 2: {
		
			// Show text value of attribute in column 2.
		
			LStr255 tempStr;
			mAttribute->GetTextValue(tempStr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			ioDrawContents.outTextBaseline++;
			ioDrawContents.outCanDoInPlaceEdit = mAttribute->IsEnabled();
			ioDrawContents.ioCellBounds.left += 2;

			ConfigureTextTraits(ioDrawContents.outTextTraits);

			// If attribute is disabled, dim value.
			// But... if object is enabled, don't dim title.
			
			if (!mAttribute->IsEnabled()) {

				DMObject* object = mAttribute->GetAffectedObject();
				ValidateObject_(object);
				
				if (object->IsEnabled())
					ioDrawContents.outTextTraits.mode = grayishTextOr;

			}
			
			// See if the attribute requires multi-line text.
			
			DMStringAttribute* stringAttr = (dynamic_cast<DMStringAttribute*>(mAttribute.GetObject()));
			if (stringAttr != nil)
				ioDrawContents.outMultiLineText = stringAttr->GetAcceptCR();
			
			break;
		}
		
		case 1:
		default:
			PIAttributeItem::GetDrawContentsSelf(inCell, ioDrawContents);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** clicking

// ---------------------------------------------------------------------------
//		* SingleClick											[protected]
// ---------------------------------------------------------------------------
//	Overriden to start in-place editing immediately, without any delay.

void
PIStringItem::SingleClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	
	// Start in-place editing, without typical 1-second delay.

	if ((inCell.col == 2) && (mAttribute->IsEnabled()))
		StartInPlaceEdit(inCell);

}


// ---------------------------------------------------------------------------
//		* TrackEmptyClick										[protected]
// ---------------------------------------------------------------------------
//	Overriden to do in-place editing, regardless of whether the text value
//	display was hit.

void
PIStringItem::TrackEmptyClick(
	const STableCell&			inCell,
	const SMouseDownEvent&		inMouseDown,
	const SOutlineDrawContents&	inDrawContents)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(mAttribute.GetObject());
	
	// Make entire cell sensitive for in-place editing.

	if ((inCell.col == 2) && (mAttribute->IsEnabled())) {
		mOVTable->UnselectAllCells();
		mOVTable->SelectCell(inCell);
		StartInPlaceEdit(inCell);
	}
	else
		LOutlineItem::TrackEmptyClick(inCell, inMouseDown, inDrawContents);

}


// ===========================================================================

#pragma mark -
#pragma mark ** display synchronization

// ---------------------------------------------------------------------------
//		* SetValueFromAttributeSelf								[protected]
// ---------------------------------------------------------------------------
//	Force a redisplay of the attribute's value.

void
PIStringItem::SetValueFromAttributeSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Check the size of this row.
	
	UpdateRowSize();
	
	// Trigger a redraw in the value column.
	
	STableCell cell(FindRowForItem(), 2);
	mOVTable->RefreshCell(cell);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** in-place editing implementation

// ---------------------------------------------------------------------------
//		* ConfigureInPlaceEdit									[protected]
// ---------------------------------------------------------------------------
//	Configure the in-place editing for the value cell. If it's an integer
//	attribute, disallow alphabetic entries.

void
PIStringItem::ConfigureInPlaceEdit(
	const STableCell&		/* inCell */,
	ResIDT&					outTextTraitsID,
	SInt16&					outMaxChars,
	UInt8&					outAttributes,
	TEKeyFilterFunc&		outKeyFilter)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());

	// String attribute?
	
	DMStringAttribute* stringAttr = (dynamic_cast<DMStringAttribute*>(mAttribute.GetObject()));
	if (stringAttr != nil) {
		
		// Validate string attribute pointer.

		ValidateObject_(stringAttr);
		
		// Yes, configure maximum length and key filters. 
		
		TextTraitsRecord foo;
		outTextTraitsID = ConfigureTextTraits(foo);
		outMaxChars = stringAttr->GetValueSize();
		return;
		
	}
	
	// Integer attribute?
	
	DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(mAttribute.GetObject()));
	if (intAttr != nil) {
	
		// Validate integer attribute pointers.
	
		ValidateObject_(intAttr);
	
		// Turn off word wrapping.
		
		outAttributes &= ~editAttr_WordWrap;
	
		// Yes, see if it's a text field.
		
		if (intAttr->GetAlwaysText()) {
		
			// Configure it for printing characters.
			
			outKeyFilter = UKeyFilters::PrintingCharField;
			outMaxChars = (intAttr->GetValueSize() + 7) / 8;
			
		}
		else {
		
			// It's at least optionally a decimal value, configure max length & key filter.
			
			LStr255 maxValueStr(intAttr->GetMaxValue());
			LStr255 minValueStr(intAttr->GetMinValue());
			
			outMaxChars = maxValueStr.Length();
			if (minValueStr.Length() > outMaxChars)
				outMaxChars = minValueStr.Length();
			
			if (intAttr->GetSigned())
				outKeyFilter = UKeyFilters::NegativeIntegerField;
			else
				outKeyFilter = UKeyFilters::IntegerField;

			// If this field can be read as a text field, config may change.
			
			if (intAttr->GetCanBeText()) {
				outKeyFilter = UKeyFilters::PrintingCharField;
				SInt16 minSize = (intAttr->GetValueSize() + 7) / 8;
				if (minSize > outMaxChars)
					outMaxChars = minSize;
			}

		}
	}
}


// ---------------------------------------------------------------------------
//		* ConfigureTextTraits									[protected]
// ---------------------------------------------------------------------------
//	Load the correct text traits for this attribute.

ResIDT
PIStringItem::ConfigureTextTraits(
	TextTraitsRecord&	ioTextTraits)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	
	// Just load the default text traits into the return record.
	
	TextTraitsH defaultTxtr = UTextTraits::LoadTextTraits(21000);
	ValidateHandle_((Handle) defaultTxtr);
	
	ioTextTraits = **defaultTxtr;

	// Skanky hack #1: Test for a specific attribute key.
	// This knowledge really belongs in the view editor.
#if Constructor_ForMacOS
	if (mAttribute->GetKey() != 'pDES')
		return 21000;
#elif Constructor_ForPilot
	if (!dynamic_cast<DMPilotStringAttribute*>(mAttribute.GetObject()))
		return 21000;
#else
	return 21000;
#endif

#if Constructor_ForMacOS	

	// Okay, this is a text description inside of a PPob resource.
	// Look for the corresponding Txtr resource.
	
	// Skanky hack #2: Look up for an RSContainer.
	// We shouldn't have this knowledge, but...
			
	RSResource* resource = nil;
	DMElement* ancestor = mAttribute;
	
	while (ancestor != nil) {
		ValidateObject_(ancestor);
		resource = (dynamic_cast<RSResource*>(ancestor));
		if (resource != nil)
			break;
		ancestor = ancestor->GetSuperElement();
	}
	
	DMObject* object = mAttribute->GetAffectedObject();

	if ((ancestor == nil) || (object == nil))
		return 21000;
	
	ValidateObject_(ancestor);
	ValidateObject_(object);
	
	// Skanky hack #3: Look for text traits attribute. This invoves
	// yet another lookup based on a specific attribute key.
	// Again, this knowledge belongs in the view editor.


	DMIntegerAttribute* txtrAttr =
			(dynamic_cast<DMIntegerAttribute*>(object->FindAttributeByKey('pTXR')));
	if (txtrAttr == nil)
		return 21000;
	
	ValidateObject_(txtrAttr);
	
	txtrAttr->AddListener(this);
	ResIDT txtr = txtrAttr->GetIntValue();
	
	// Okay, all the numbers add up. Load the text traits resource.

	RFResource* txtrRes = nil;
	if (txtr != 0) {
		RFMap* map = resource->GetRFMap();
		ValidateObject_(map);
		txtrRes = map->FindResource('Txtr', txtr, false);

		if (txtrRes != nil) {
			ValidateSimpleObject_(txtrRes);
			if (txtrRes->IsDeleted())
				txtrRes = nil;
		}
	}
	
	if (txtrRes != nil) {
	
		// Text traits resource found, copy it back to caller.
		
		StApplicationContext appContext;
		Handle dummyTxtrRes = ::Get1Resource('Txtr', 21010);
		ValidateHandle_(dummyTxtrRes);
		
		TextTraitsH txtrData = (TextTraitsH) txtrRes->GetResData();
		ValidateHandle_((Handle) txtrData);
		
		(*txtrData)->justification = teFlushLeft;
		(*txtrData)->color = UGAColorRamp::GetBlackColor();
		(*txtrData)->mode = srcOr;
		
		SInt16 size = (*txtrData)->size;
		if (size < 9)
			(*txtrData)->size = 9;
		if (size > 14)
			(*txtrData)->size = 14;
		
		::BlockMove(*txtrData, &ioTextTraits, sizeof (TextTraitsRecord));
		::SetHandleSize(dummyTxtrRes, ::GetHandleSize((Handle) txtrData));
		ThrowIfMemError_();

		::BlockMoveData(*txtrData, *dummyTxtrRes, ::GetHandleSize((Handle) txtrData));
		::DisposeHandle((Handle) txtrData);
		
		return 21010;
	
	}
	else {
		
		// Text traits resource not found, or ID = 0: Use system settings.
		
		UTextTraits::LoadSystemTraits(ioTextTraits);
		return 0;
	
	}
#elif Constructor_ForPilot


	StApplicationContext appContext;
	DMIntegerAttribute* font = (dynamic_cast<DMIntegerAttribute*>(
						mAttribute->GetAffectedObject()->FindAttributeByKey('pFNT')));
	Int32 ttxrID = 0;
	if (font != nil)
	{
		ttxrID = 4000 + font->GetIntValue();
		font->AddListener(this);
	}
	else if ((mAttribute->GetKey() == 'pTTL')||(mAttribute->GetKey() == 'MESG'))
		ttxrID = 4001; //bold for alert title and message
	else
		ttxrID = 4000; //standard for alert buttons
	
	TextTraitsH handle = UTextTraits::LoadTextTraits(ttxrID);
	ValidateHandle_((Handle)handle);
	// JCD 980629 - we can't move sizeof(TextTraitsRecord) lest suffer
	// the chance of overwriting (it happens). Let's just write what
	// we need.
//	::BlockMoveData((char*)*handle, &ioTextTraits, sizeof (TextTraitsRecord));
	::BlockMoveData((char*)*handle, &ioTextTraits, ::GetHandleSize((Handle)handle));
	::ReleaseResource((Handle)handle);
	return ttxrID;
#endif
}
