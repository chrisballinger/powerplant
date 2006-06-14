// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMAttributeSetter.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/03/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: StDMAttributeSetter.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/02/96   Time: 18:19
//	Updated in $/Constructor/Source files/Data model/Undo-redo
//	Updated for new undo/redo menu string architecture.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:33
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMPointers.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class StDMTransactionBuilder;


// ===========================================================================
//		* StDMAttributeSetter
// ===========================================================================
//
//	An StDMAttributeSetter can be used to set the value of an
//	attribute from a text string. It creates a transaction with an
//	appropriate undo/redo string.
//
// ===========================================================================

class StDMAttributeSetter {

public:
							StDMAttributeSetter(
									DMAttribute*		inAttribute);
							~StDMAttributeSetter();
	
	// attribute value setters
	
	void					SetTextValue(
									ConstStringPtr		inTextValue);
	void					SetIntValue(
									SInt32				inIntValue);
	void					SetRGBColorValue(
									const RGBColor&		inColorValue);

	// configuration
	
	void					SetSelection(
									DMSelection*		inSelection);
	void					SetUndoerHost(
									LCommander*			inUndoerHost);

	// description string

protected:
	void					MakeDescriptionString(
									StDMTransactionBuilder&	inTransaction);


	// data members

protected:
	DMAttributePtr			mAttribute;					// attribute whose value we're setting
	DMSelectionPtr			mSelection;					// selection whose object has the attribute
	LCommander*				mUndoerHost;				// where to post the transaction

};
