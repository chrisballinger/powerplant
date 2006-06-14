// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMValidationException.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/29/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMValidationException.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:31
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Exceptions
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/04/96   Time: 15:04
//	Updated in $/Constructor/Source files/Data model/Exceptions
//	Added ShowValidationFailedAlert method. Inlined constructor and
//	destructor.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMValidationException.h"
#include <LStaticText.h>

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : Utility classes
#include <UModalDialogs.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_ValidationFailure		= 23900;
const PaneIDT	Pane_Reason					= 'RSN ';


// ===========================================================================

#pragma mark *** DMValidationException ***

// ---------------------------------------------------------------------------
//		* ShowValidationFailedAlert
// ---------------------------------------------------------------------------
//	Show an alert that indicates the reason for the validation failure.

void
DMValidationException::ShowValidationFailedAlert() const
{
	
	// Load the can't open dialog.
	
	StApplicationContext appContext;
	StDialogHandler dialog(PPob_ValidationFailure, LCommander::GetTopCommander());
	ValidateObject_(dialog.GetDialog());
	
	// Fill in the reason field in the dialog.
	
	LStaticText* reasonCaption = dynamic_cast<LStaticText*>(dialog.GetDialog()->FindPaneByID(Pane_Reason));
	ValidateObject_(reasonCaption);
	
	LStr255 reasonStr;
	GetErrorString(reasonStr);
	
	reasonCaption->SetDescriptor(reasonStr);
	
	// Show the dialog.

	dialog.GetDialog()->ProcessCommand(cmd_PositionWindow);
	dialog.GetDialog()->Show();

	// Wait until OK is hit.

	MessageT message;
	do {
		message = dialog.DoDialog();
	} while (message == msg_Nothing);

	// Remember dialog's new position.

	dialog.GetDialog()->ProcessCommand(cmd_RecordPosition);

}