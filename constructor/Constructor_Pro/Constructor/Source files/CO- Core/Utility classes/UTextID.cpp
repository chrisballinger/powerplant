// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UTextID.cpp					© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 02/12/95
//	   $Date: 2006/01/18 01:33:28 $
//	$History: UTextID.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:17
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 4   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:07
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge of changes for MSVC build
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:33
//	Checked in '$/Constructor/Source files/CO- Core/Utility classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/16/96   Time: 15:11
//	Checked in '$/Constructor/Source files/Utility classes'
//	Comment: Renamed class from UPaneID to UTextID.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "UTextID.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_UTextID				= 31100;
const SInt16	str_LeftSingleQuote		= 1;
const SInt16	str_RightSingleQuote 	= 2;


// ===========================================================================

#pragma mark *** UTextID ***

// ---------------------------------------------------------------------------
//		* StuffOptionalTextBox
// ---------------------------------------------------------------------------
//	Utility procedure to set an ID field to either textual values
//	or numeric values as specified by user preferences.

void
UTextID::StuffOptionalTextBox(
	UInt32			inValue,				// value to stuff
	LEditField*		inEditField,			// the edit field we're working with
	LStdCheckBox*	inCheckBox,				// the "Text" checkbox (can be nil)
	Boolean			inGuessText,			// true to guess whether ID is text or not
	Boolean			inUseTextID,			// true to use text IDs
	Boolean			inResizeBox,			// true to resize the text box
	SInt32			inFieldSize)			// data field size (in bits)
{

	// If requested, we'll guess whether this is text or not
	//	(but if value is zero, use default provided by caller).
	
	if (inGuessText && inValue != 0)
		inUseTextID = UTextID::IsLikelyTextValue(inValue, inFieldSize);

	// If requested, we can set the checkbox's value.
	
	if (inCheckBox)
		inCheckBox->SetValue(inUseTextID);

	// Decide how many characters can be in the edit field.
	
	SInt16 maxCharsText = 4;
	SInt16 maxCharsInt = 11;
	switch (inFieldSize) {
		case 8:
			maxCharsText = 1;
			maxCharsInt = 3;
			break;
		case -8:
			maxCharsText = 1;
			maxCharsInt = 4;
			break;
		case 16:
			maxCharsText = 2;
			maxCharsInt = 5;
			break;
		case -16:
			maxCharsText = 2;
			maxCharsInt = 6;
			break;
	}

	// Set the text box accordingly.
	
	if (inUseTextID) {
		if (inValue != 0) {
			Str15 theIDString;
			switch (inFieldSize) {
				case 8:
				case -8:
					inValue <<= 24;
					inValue &= 0xFF000000;
					inValue |= 0x00202020;
					break;
				case 16:
				case -16:
					inValue <<= 16;
					inValue &= 0xFFFF0000;
					inValue |= 0x00002020;
					break;
			}
			LString::FourCharCodeToPStr(inValue, theIDString);
			switch (inFieldSize) {
				case 8:
				case -8:
					theIDString[0] = 1;
					break;
				case 16:
				case -16:
					theIDString[0] = 2;
					break;
			}
			inEditField->SetDescriptor(theIDString);
		}
		else
			inEditField->SetDescriptor((StringPtr)"\001");		// zero value->null string
		inEditField->SetKeyFilter((TEKeyFilterFunc) UKeyFilters::PrintingCharField);
		inEditField->SetMaxChars(maxCharsText);
		if (inResizeBox)
			inEditField->ResizeFrameTo(44, 16, true);
	}
	else {
		inEditField->SetValue(inValue);
//		inEditField->SetKeyFilter((KeyFilterFunc) UKeyFilters::NegativeIntegerField);
		inEditField->SetMaxChars(maxCharsInt);
		if (inResizeBox)
			inEditField->ResizeFrameTo(66, 16, true);
	}
}


// ---------------------------------------------------------------------------
//		* ReadOptionalTextBox
// ---------------------------------------------------------------------------
//	Utility procedure to set an ID field to either textual values or numeric values
//	as specified by user preferences.

UInt32
UTextID::ReadOptionalTextBox(
	const LEditField*	inEditField,			// the edit field we're working with
	Boolean				inUseTextID,			// true to use text IDs
	SInt32				inFieldSize)			// size of field (in bits)
{
	if (inUseTextID) {
		Str255 theIDString;
		UInt32 theValue;
		inEditField->GetDescriptor(theIDString);
		switch (theIDString[0]) {
			case 0:
				return 0;				// null string -> zero value
			case 1:
				theIDString[2] = ' ';		// yes, we do mean to fall through here!
			case 2:
				theIDString[3] = ' ';
			case 3:
				theIDString[4] = ' ';
			default:
				LString::PStrToFourCharCode(theIDString, theValue);
				switch (inFieldSize) {
					case 16:
						theValue >>= 16;
						theValue &= 0xFFFF;
						break;
					case 8:
						theValue >>= 24;
						theValue &= 0xFF;
						break;
					case -16:
						theValue >>= 16;
						theValue &= 0xFFFF;
						if (theValue > 0x7FFF)
							theValue -= 0x10000;
						break;
					case -8:
						theValue >>= 24;
						theValue &= 0xFF;
						if (theValue > 0x7F)
							theValue -= 0x100;
						break;
				}
				return theValue;
		}
	}
	else
		return inEditField->GetValue();
}


// ---------------------------------------------------------------------------
//		* IsLikelyTextValue
// ---------------------------------------------------------------------------
//	Given a 32-bit value, determine whether it is likely to be
//	read as 4 characters or as an integer value.
//
//	Return true if text.

Boolean
UTextID::IsLikelyTextValue(
	UInt32		inValue,					// the ID in question
	SInt32		inFieldSize)				// size of the field in question
{

	// Shift data to compensate if less than 32 bits.
	
	switch (inFieldSize) {
		case 8:
		case -8:
			inValue <<= 24;
			inValue &= 0xFF000000;
			inValue |= 0x00202020;
			break;
		case 16:
		case -16:
			inValue <<= 16;
			inValue &= 0xFFFF0000;
			inValue |= 0x00002020;
			break;
	}

	// Extremely large integers are likely to be small negative integers.
	
	if (inValue > 0xFFFF0000)
		return false;
	
	// Anything less than all spaces is definitely integer.
	
	if (inValue < 0x20202020)
		return false;
	
	// Check all characters to see if they're printable.
		//! TEMPORARY: don't
	
	return true;
	
}


// ---------------------------------------------------------------------------
//		* MakePrintableID
// ---------------------------------------------------------------------------
//	Convert a pane ID to a printable string based on our best
//	guess as to whether it's a text value or not.

StringPtr
UTextID::MakePrintableID(
	UInt32		inValue,				// the value
	Str255		outString,				// string buffer
	Boolean		inUseQuotes,			// true to place single quotations around the string if it's text
	SInt32		inFieldSize)			// # bits in field
{
	StApplicationContext appContext;
	LStr255 returnStr;

	if (IsLikelyTextValue(inValue, inFieldSize)) {

		switch (inFieldSize) {
			case 8:
			case -8:
				inValue <<= 24;
				inValue &= 0xFF000000;
				inValue |= 0x00202020;
				break;
			case 16:
			case -16:
				inValue <<= 16;
				inValue &= 0xFFFF0000;
				inValue |= 0x00002020;
				break;
		}

		LStr255 tempStr = (OSType) inValue;
		switch (inFieldSize) {
			case 8:
			case -8:
				tempStr[(UInt8)0] = 1;
				break;
			case 16:
			case -16:
				tempStr[(UInt8)0] = 2;
				break;
		}

		if (inUseQuotes)
			returnStr += LStr255(STR_UTextID, str_LeftSingleQuote);
		returnStr += tempStr;
		if (inUseQuotes)
			returnStr += LStr255(STR_UTextID, str_RightSingleQuote);
	}
	else
		returnStr = (SInt32) inValue;

	return LString::CopyPStr(returnStr, outString);
}
