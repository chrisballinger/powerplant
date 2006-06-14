// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMTransactionHider.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/05/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: StDMTransactionHider.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
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

#pragma once


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class StDMTransactionBuilder;


// ===========================================================================
//		* StDMTransactionHider
// ===========================================================================
//
//	StDMTransactionHider is a helper class which disables the
//	StDMTransactionBuilder temporarily. Use it if, in the course of
//	executing the command, you make changes to the data model that
//	should not be undone or redone.
//
// ===========================================================================

class StDMTransactionHider {

public:
							StDMTransactionHider();
	virtual					~StDMTransactionHider();

protected:
	StDMTransactionBuilder*	mSavedBuilder;

};
