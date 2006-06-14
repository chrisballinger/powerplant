// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMEditAttachment.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/04/96
//	   $Date: 2006/01/18 01:32:40 $
//	$History: DMEditAttachment.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:23
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:29
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Updated for new undo/redo menu string architecture.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 18:56
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Fixed bugs in Copy and Paste.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 17:49
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Updated source format.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMPointers.h"


// ===========================================================================
//		* DMEditAttachment
// ===========================================================================
//
//	DMEditAttachment provides most of the standard menu behavior for
//	views which display selectable elements. This includes cut, copy,
//	clear (but not paste), and also mapping the backspace/delete key
//	to clear. Paste is not implemented here since the placement of
//	pasted items is too tied to the receiving view.
//
//	Menu title customization is provided for most items in the Edit menu,
//	even if the enable/disable decision is made elsewhere.
//
// ===========================================================================

class DMEditAttachment : public LAttachment {

public:
							DMEditAttachment(
									LCommander*			inActionHost,
									DMSelection*		inSelection,
									ResIDT				inEditMenuStrings);
	virtual					~DMEditAttachment();

	// event dispatching

protected:
	virtual void			ExecuteSelf(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			FindCommandStatus(
									SCommandStatus*		inCommandStatus);
	virtual void			HandleKeyEvent(
									const EventRecord*	inEvent);

	// clipboard primitives
	
	virtual void			CopySelection();
	virtual void			DeleteSelection(CommandT	inMenuCommand);

	
	// data members

protected:
	LCommander*				mActionHost;				// host which receives transactions
														//	(presumably there is an LUndoer there)
	DMSelectionPtr			mSelection;					// selection for this view
	ResIDT					mEditMenuStrings;			// standard edit menu strings (cut, copy, paste, etc.)

};
