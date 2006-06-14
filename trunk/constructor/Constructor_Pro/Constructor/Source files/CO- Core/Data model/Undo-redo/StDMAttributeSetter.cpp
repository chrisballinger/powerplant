// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMAttributeSetter.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/03/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: StDMAttributeSetter.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "StDMAttributeSetter.h"

	// Core : Data model : Attributes
#include "DMAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMRGBColorAttribute.h"

	// Core : Data model : Undo-redo
#include "StDMTransactionBuilder.h"
#include "DMTransaction.h"


// ===========================================================================

#pragma mark *** StDMAttributeSetter ***

// ---------------------------------------------------------------------------
//		* StDMAttributeSetter(DMAttribute*)
// ---------------------------------------------------------------------------
//	Constructor, specifying the attribute to be changed

StDMAttributeSetter::StDMAttributeSetter(
	DMAttribute*	inAttribute)
{
	ValidateObject_(inAttribute);

	mAttribute = inAttribute;
	ValidateObject_(mAttribute.GetObject());
	
	mUndoerHost = nil;
}


// ---------------------------------------------------------------------------
//		* ~StDMAttributeSetter
// ---------------------------------------------------------------------------
//	Destructor

StDMAttributeSetter::~StDMAttributeSetter()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute value setters

// ---------------------------------------------------------------------------
//		* SetTextValue
// ---------------------------------------------------------------------------
//	Set up the transaction, set the value, commit the transaction.

void
StDMAttributeSetter::SetTextValue(
	ConstStringPtr inTextValue)
{

	// Validate pointers.

	ValidateObject_(mAttribute.GetObject());

	// Build a transaction.

	StDMTransactionBuilder trans(mUndoerHost, mSelection);
	MakeDescriptionString(trans);

	// Set the value.
	
	mAttribute->SetTextValue(inTextValue);
	
	// Commit transaction.
	
	trans.Commit();

}


// ---------------------------------------------------------------------------
//		* SetIntValue
// ---------------------------------------------------------------------------
//	Set up the transaction, set the value, commit the transaction.

void
StDMAttributeSetter::SetIntValue(
	SInt32	inIntValue)
{

	// Validate pointers.

	ValidateObject_(mAttribute.GetObject());

	// Build a transaction.

	StDMTransactionBuilder trans(mUndoerHost, mSelection);
	MakeDescriptionString(trans);

	// Set the value.

	DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(mAttribute.GetObject()));
	ValidateObject_(intAttr);

	intAttr->SetIntValue(inIntValue);
	
	// Commit transaction.
	
	trans.Commit();

}

// ---------------------------------------------------------------------------
//		* SetRGBColorValue
// ---------------------------------------------------------------------------
//	Set up the transaction, set the value, commit the transaction.

void
StDMAttributeSetter::SetRGBColorValue(
	const RGBColor&		inColorValue)
{

	// Validate pointers.

	ValidateObject_(mAttribute.GetObject());

	// Build a transaction.

	StDMTransactionBuilder trans(mUndoerHost, mSelection);
	MakeDescriptionString(trans);

	// Set the value.
	
	DMRGBColorAttribute* rgbAttr = (dynamic_cast<DMRGBColorAttribute*>(mAttribute.GetObject()));
	ValidateObject_(rgbAttr);

	rgbAttr->SetRGBColorValue(inColorValue);
	
	// Commit transaction.
	
	trans.Commit();

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Set the selection for this transaction. If the transaction is
//	undone or redone, this selection will be asked to select the
//	affected objects.

void
StDMAttributeSetter::SetSelection(
	DMSelection* inSelection)
{
	if (inSelection != nil)
		ValidateObject_(inSelection);
	mSelection = inSelection;
}


// ---------------------------------------------------------------------------
//		* SetUndoerHost
// ---------------------------------------------------------------------------
//	Set the commander which will receive the action object.

void
StDMAttributeSetter::SetUndoerHost(
	LCommander* inUndoerHost)
{
	ValidateObject_(inUndoerHost);
	mUndoerHost = inUndoerHost;
}


// ===========================================================================

#pragma mark -
#pragma mark ** description string

// ---------------------------------------------------------------------------
//		* MakeDescriptionString									[protected]
// ---------------------------------------------------------------------------
//	Make the description string that will be used for the Undo/Redo
//	menu command.

void
StDMAttributeSetter::MakeDescriptionString(
	StDMTransactionBuilder&	inTransaction)
{

	// Validate input parameters.
	
	ValidateObject_(mAttribute.GetObject());

	// Set the undo/redo string.
	
	LStr255 description(STR_UndoRedoStrings, str_SetSpace);
	LStr255 tempStr;
	
	mAttribute->GetTitle(tempStr);
	description += tempStr;
	
	inTransaction.SetDescription(description);

}
