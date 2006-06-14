// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMSubAction.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: DMSubAction.cpp $
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

#include "DMSubAction.h"


// ===========================================================================

#pragma mark *** DMSubAction ***

// ---------------------------------------------------------------------------
//		* DMSubAction(...)
// ---------------------------------------------------------------------------
//	Constructor

DMSubAction::DMSubAction()
{
	mIsDone = true;
}


// ---------------------------------------------------------------------------
//		* ~DMSubAction
// ---------------------------------------------------------------------------
//	Destructor

DMSubAction::~DMSubAction()
{
	ValidateThis_();
}


// ---------------------------------------------------------------------------
//		* Commit
// ---------------------------------------------------------------------------
//	Called by StDMTransactionBuilder::Commit when this subaction is
//	added to a transaction. This is an override hook for the subaction
//	to gather additional information.

void
DMSubAction::Commit()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* GetAffectedObject
// ---------------------------------------------------------------------------
//	Called by DMTransaction::SelectAffectedObjects. Should return the
//	object which was affected by this subaction, or nil if none.

DMObject*
DMSubAction::GetAffectedObject() const
{
	return nil;
}
