// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIHexStringItem.h			   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/18/97
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIHexStringItem.h $
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

#pragma once

	// Core : Property inspector
#include "PIStringItem.h"

class LCheckBox;

// ===========================================================================
//		* PIStringItem
// ===========================================================================
//
//	PIHexStringItem is an attribute item for string values that need to offer
//  Hex input.  It displays a checkbox for switching between Hex and Ascii input
//
// ===========================================================================

class PIHexStringItem : public PIStringItem {

public:
							PIHexStringItem() { }
	virtual					~PIHexStringItem() { }
	
	virtual void			FinishCreate();
	
	// configuration
	
	virtual void			UpdateRowSize();
	virtual void			ListenToMessage(
									MessageT		inMessage,
									void*			ioParam);

	// update notifications

protected:

	virtual void			DisableSelf();
	virtual void			EnableSelf();

	// drawing

	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);
	// in-place editing implementation
	
	virtual void			ConfigureInPlaceEdit(
									const STableCell&		inCell,
									ResIDT&					outTextTraitsID,
									SInt16&					outMaxChars,
									UInt8&					outAttributes,
									TEKeyFilterFunc&		outKeyFilter);
	virtual ResIDT			ConfigureTextTraits(
									TextTraitsRecord&		outTextTraits);

	virtual void			IPEChanged();

	// key filter

	static EKeyStatus		HexFilter(TEHandle			inMacTEH,
									   UInt16			inKeyCode,
									   UInt16			&ioCharCode,
									   EventModifiers	inModifiers);

	// hex support
	
	Boolean					IsHex();
	Boolean					NeedsHex();

	void					ConvertFromHex(LStr255& inOutString);
	void					ConvertToHex(LStr255& inOutString);


	// data members

protected:	
	LCheckBox*				mHexBox;

};