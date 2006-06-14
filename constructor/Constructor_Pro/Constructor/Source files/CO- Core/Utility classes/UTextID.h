// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UTextID.h					© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 02/12/95
//	   $Date: 2006/01/18 01:33:28 $
//	$History: UTextID.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:17
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:33
//	Checked in '$/Constructor/Source files/CO- Core/Utility classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/16/96   Time: 15:12
//	Checked in '$/Constructor/Source files/Utility classes'
//	Comment: Renamed class from UPaneID to UTextID.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : PP headers
#include <PP_Types.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LEditField;
class LStdCheckBox;


// ===========================================================================
//		* UTextID
// ===========================================================================
//	This utility class provides some stuff that we use throughout
//	constructor for deciphering pane IDs, message codes, and the like.
//
//	This is where the logic of deciding whether a 32-bit quantity
//	should be displayed as a numeric value or as text.

class UTextID {

public:
	static void			StuffOptionalTextBox(
								UInt32				inValue,
								LEditField*			inEditField,
								LStdCheckBox*		inCheckBox,
								Boolean				inGuessText = true,
								Boolean				inUseTextID = false,
								Boolean				inResizeBox = true,
								SInt32				inFieldSize = 32);

	static UInt32			ReadOptionalTextBox(
								const LEditField*	inEditField,
								Boolean				inUseTextID,
								SInt32				inFieldSize = 32);
	
	static Boolean			IsLikelyTextValue(
								UInt32				inValue,
								SInt32				inFieldSize = 32);
	
	static StringPtr		MakePrintableID(
								UInt32				inValue,
								Str255				outString,
								Boolean				inUseQuotes,
								SInt32				inFieldSize = 32);
	
};
