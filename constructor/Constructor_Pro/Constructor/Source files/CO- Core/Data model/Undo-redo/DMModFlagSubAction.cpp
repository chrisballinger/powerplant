// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMModFlagSubAction.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/02/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: DMModFlagSubAction.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:21
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Undo-redo'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:32
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMModFlagSubAction.h"


// ===========================================================================

#pragma mark *** DMModFlagSubAction ***

// ---------------------------------------------------------------------------
//		* DMModFlagSubAction(...)
// ---------------------------------------------------------------------------
//	Constructor

DMModFlagSubAction::DMModFlagSubAction(
	DMElement*	inElement,
	Boolean		inSetModified,
	Boolean		inSetSubModified)
{
	ValidateObject_(inElement);
	
	mElement = inElement;
	ValidateObject_(mElement.GetObject());
	
	mSetModified = inSetModified;
	mSetSubModified = inSetSubModified;
}


// ---------------------------------------------------------------------------
//		* ~DMModFlagSubAction
// ---------------------------------------------------------------------------
//	Destructor

DMModFlagSubAction::~DMModFlagSubAction()
{
	ValidateThis_();
}


// ---------------------------------------------------------------------------
//		* RedoSelf
// ---------------------------------------------------------------------------
//	Turn the modified or sub-modified flag on. Broadcast change message.

void
DMModFlagSubAction::RedoSelf()
{
	ValidateThis_();
	ValidateObject_(mElement.GetObject());
	
	if (mSetModified)
		mElement->mModified = true;
	if (mSetSubModified)
		mElement->mSubModified = true;
	mElement->SendUpdateMessage(Element_ModifiedFlagChanged, (DMElement*) mElement);
}


// ---------------------------------------------------------------------------
//		* UndoSelf
// ---------------------------------------------------------------------------
//	Turn the modified or sub-modified flag off. Broadcast change message.

void
DMModFlagSubAction::UndoSelf()
{
	ValidateThis_();
	ValidateObject_(mElement.GetObject());
	
	if (mSetModified)
		mElement->mModified = false;
	if (mSetSubModified)
		mElement->mSubModified = false;
	mElement->SendUpdateMessage(Element_ModifiedFlagChanged, (DMElement*) mElement);
}
