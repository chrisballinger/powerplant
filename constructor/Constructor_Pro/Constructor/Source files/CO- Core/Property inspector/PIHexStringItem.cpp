// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIHexStringItem.cpp			   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/18/97
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIHexStringItem.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/28/97   Time: 14:44
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	More updates for VC++ build environment.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 02/28/97   Time: 14:39
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Updated for VC++ build environment.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 02/28/97   Time: 11:50
//	Created
//	Comment: Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIHexStringItem.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"
#include "DMStringAttribute.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// PowerPlant : CA : Text editing
#include <LInPlaceEditField.h>

	// PowerPlant : CA : Utility classes
#include <UNewTextDrawing.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>
#include <LCheckBox.h>

// ===========================================================================

#pragma mark *** PIHexStringItem ***


// ---------------------------------------------------------------------------
//		* FinishCreate											[protected]
// ---------------------------------------------------------------------------
//	Create hex checkbox if necessary

void
PIHexStringItem::FinishCreate()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOutlineTable);
	
	// Load button from PPob resource.
	
	STableCell cell(FindRowForItem(), 2);
	mHexBox = (dynamic_cast<LCheckBox*>(CreatePaneForCell(cell, 21109)));
	ValidateObject_(mHexBox);
	mHexBox->MoveBy(0, 1, false);
	
	// Place mButton in right edge of cell.

	Rect bounds;	
	mOutlineTable->GetLocalCellRect(cell, bounds);
	
	// Set up listener relationship.
	
	mHexBox->AddListener(this);
}

// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* UpdateRowSize
// ---------------------------------------------------------------------------
//	Override just to insert the "maxSize.right -= 45" line, so that we calculate
//  the correct number of rows for our shortened space

void
PIHexStringItem::UpdateRowSize()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(mAttribute.GetObject());
	
	// Measure the size of this string.
	
	TextTraitsRecord txtr;
	ConfigureTextTraits(txtr);
	
	mOVTable->FocusDraw();
	SInt16 justification = UTextTraits::SetPortTextTraits(&txtr);

	Rect maxSize = { 0, 0, 32767, mOVTable->GetColWidth(2) - 6 };
	maxSize.right -= 45;
	Rect textSize;
	
	LStr255 string;
	mAttribute->GetTextValue(string);
	
	if (string.Length() == 0)
		string = "\001 ";
	
	UNewTextDrawing::MeasureWithJustification(
			(Ptr) &string[(UInt8) 1],
			string[(UInt8) 0],
			maxSize,
			justification,
			textSize);

	// Set vertical size based on font info.

	SInt16 height = textSize.bottom + 3;
	if (height < 15)
		height = 15;
	
	TableIndexT row = FindRowForItem();
	if (mOVTable->GetRowHeight(row) != height) {
		mOVTable->SetRowHeight(height, row, row);

		STableCell topLeftCell(row, 1);
		STableCell botRightCell;
		mOVTable->GetTableSize(botRightCell.row, botRightCell.col);
	
		mOVTable->RefreshCellRange(topLeftCell, botRightCell);
	}
}




// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Override to convert the text to hex if necessary, and to shorten the bounds.

void
PIHexStringItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());

	PIStringItem::GetDrawContentsSelf(inCell,ioDrawContents);
	if (inCell.col == 2)
	{
		if (IsHex())
		{
			LStr255 tempStr;
			mAttribute->GetTextValue(tempStr);
			if (IsHex())
				ConvertToHex(tempStr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
		}
		ioDrawContents.ioCellBounds.right -= 40;
	}
}

#pragma mark -
#pragma mark ** interaction

// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Disable the button.

void
PIHexStringItem::DisableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mHexBox);
	
	// Disable the button.

	mHexBox->Disable();

}


// ---------------------------------------------------------------------------
//		* EnableSelf											[protected]
// ---------------------------------------------------------------------------
//	Enable the button.

void
PIHexStringItem::EnableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mHexBox);
	
	// Enable the button.

	mHexBox->Enable();

}

	
// ---------------------------------------------------------------------------
//		* ListenToMessage											[protected]
// ---------------------------------------------------------------------------
//	We listen for the hex box's message

void
PIHexStringItem::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	ValidateThis_();
	if (inMessage == 'Hex ')
	{
		SetValueFromAttribute();
		STableCell cell(FindRowForItem(), 2);
		mOVTable->RefreshCell(cell);
	}
	else
		PIStringItem::ListenToMessage(inMessage,ioParam);	
}



// ===========================================================================

#pragma mark -
#pragma mark ** in-place editing implementation

// ---------------------------------------------------------------------------
//		* ConfigureInPlaceEdit									[protected]
// ---------------------------------------------------------------------------
//	Override to set the hex filter and double the maxChars if we are doing hex

void
PIHexStringItem::ConfigureInPlaceEdit(
	const STableCell&		inCell,
	ResIDT&					outTextTraitsID,
	SInt16&					outMaxChars,
	UInt8&					outAttributes,
	TEKeyFilterFunc&		outKeyFilter)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());

	PIStringItem::ConfigureInPlaceEdit(
		inCell,outTextTraitsID,outMaxChars,outAttributes,outKeyFilter);
	
	// String attribute?
	
	DMStringAttribute* stringAttr = (dynamic_cast<DMStringAttribute*>(mAttribute.GetObject()));
	if (stringAttr != nil) 
	{
		ValidateObject_(stringAttr);
		if (IsHex())
		{
			outKeyFilter = HexFilter;
			outMaxChars <<= 1; //twice as many hex digits for the string
		}
	}	
}


// ---------------------------------------------------------------------------
//		* ConfigureTextTraits									[protected]
// ---------------------------------------------------------------------------
//	Override to use the default ttxr if we are in hex mode

ResIDT
PIHexStringItem::ConfigureTextTraits(
	TextTraitsRecord&	ioTextTraits)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	
	if (IsHex())
	{
		// Just load the default text traits into the return record.
	
		TextTraitsH defaultTxtr = UTextTraits::LoadTextTraits(21000);
		ValidateHandle_((Handle) defaultTxtr);
		ioTextTraits = **defaultTxtr;
		return 21000;
	}	
	else
		return PIStringItem::ConfigureTextTraits(ioTextTraits);
}

// ---------------------------------------------------------------------------
//		* IPEChanged											[protected]
// ---------------------------------------------------------------------------
//	Overriden to translate from hex if necessary

void
PIHexStringItem::IPEChanged()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mEditField);
	ValidateObject_(mOVTable);

	if (IsHex())
	{	
		LStr255 ipeText;
		mEditField->GetDescriptor(ipeText);
		
		//convert from hex
		ConvertFromHex(ipeText);
		
		mEditField->SetDescriptor(ipeText);
	}	
	
	//let OVItem do the rest
	PIStringItem::IPEChanged();
}

// ===========================================================================

#pragma mark -
#pragma mark ** Hex Support

Boolean
PIHexStringItem::NeedsHex()
{
	ValidateObject_(mAttribute.GetObject());
	DMStringAttribute* stringAttr = (dynamic_cast<DMStringAttribute*>(mAttribute.GetObject()));
	return ((stringAttr != 0)&&(stringAttr->GetNeedsHex()));
}

Boolean
PIHexStringItem::IsHex()
{
	return (NeedsHex() && (mHexBox->GetValue() != 0));
}


void
PIHexStringItem::ConvertToHex(
	LStr255& inOutString)
{
	LStr255 temp;
	UInt8 size = inOutString.Length();
	for (short i = 1; i <= size; i++)
	{	
		UInt8 high = inOutString[(UInt8) i] >> 4;
		if (high > 9)
			temp += (UInt8) (high + '7');
		else 
			temp += (UInt8) (high + '0');
			
		UInt8 low = inOutString[(UInt8) i] & 0xf;
		if (low > 9)
			temp += (UInt8) (low + '7');
		else 
			temp += (UInt8) (low + '0');
	}
	inOutString = temp;
}

void
PIHexStringItem::ConvertFromHex(
	LStr255& inOutString)
{
	LStr255 temp;
	SInt8 size = inOutString.Length();
	for (short i = 1; i <= size; i++)
	{	
		UInt8 high = inOutString[(UInt8) i];
		i++;
		
		if (high > '9')
			high -= '7';
		else
			high -= '0';
		
		UInt8 low = (i <= size) ? inOutString[(UInt8) i] : 0;
		if (low > '9')
			low -= '7';
		else
			low -= '0';

		temp += (UInt8) ((high << 4) | (low & 0xf));
	}
	inOutString = temp;
}

EKeyStatus
PIHexStringItem::HexFilter(
	TEHandle		/* inMacTEH */,
	UInt16			inKeyCode,
	UInt16			&ioCharCode,
	EventModifiers	/* inModifiers */)
{
	EKeyStatus	theKeyStatus = keyStatus_PassUp;
	
	if (UKeyFilters::IsTEDeleteKey(inKeyCode)) {
		theKeyStatus = keyStatus_TEDelete;
	} else if (UKeyFilters::IsTECursorKey(inKeyCode)) {
		theKeyStatus = keyStatus_TECursor;
	} else if (UKeyFilters::IsExtraEditKey(inKeyCode)) {
		theKeyStatus = keyStatus_ExtraEdit;
	} else if (UKeyFilters::IsPrintingChar(ioCharCode)) {
		if ((UKeyFilters::IsNumberChar(ioCharCode))||((ioCharCode >= 'A') && (ioCharCode <= 'F')))  {
			theKeyStatus = keyStatus_Input;
		} else if ((ioCharCode >= 'a') && (ioCharCode <= 'f')){
			theKeyStatus = keyStatus_Input;
			ioCharCode -= 32;  // Convert to upper case
		}
		else {
			theKeyStatus = keyStatus_Reject;
		}
		
	}	

	return theKeyStatus;
}