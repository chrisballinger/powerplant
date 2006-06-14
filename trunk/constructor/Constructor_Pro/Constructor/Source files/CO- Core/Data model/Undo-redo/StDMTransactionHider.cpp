// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMTransactionHider.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/05/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: StDMTransactionHider.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:06
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge of changes for MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:22
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Undo-redo'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:33
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "StDMTransactionHider.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"


// ===========================================================================

#pragma mark *** StDMTransactionHider ***

// ---------------------------------------------------------------------------
//		* StDMTransactionHider
// ---------------------------------------------------------------------------
//	Constructor

StDMTransactionHider::StDMTransactionHider()
{
	mSavedBuilder = StDMTransactionBuilder::sCurrentBuilder;
	StDMTransactionBuilder::sCurrentBuilder = nil;
}


// ---------------------------------------------------------------------------
//		* ~StDMTransactionHider
// ---------------------------------------------------------------------------
//	Destructor

StDMTransactionHider::~StDMTransactionHider()
{
	StDMTransactionBuilder::sCurrentBuilder = mSavedBuilder;
}
