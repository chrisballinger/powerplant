// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StKeyDialogHandler.cpp		© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 08/09/95
//	   $Date: 2006/01/18 01:32:41 $
//	$History: StKeyDialogHandler.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Dialog handlers
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:06
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge of changes for MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:23
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Dialog handlers'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:44
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "StKeyDialogHandler.h"

	// PowerPlant : Array classes
#include <LArrayIterator.h>

	// PowerPlant : Pane classes
#include <LDialogBox.h>
#include <LStdControl.h>

	// PowerPlant : Utility classes
#include <UKeyFilters.h>

#include <LPushButton.h>


// ===========================================================================

#pragma mark *** StKeyDialogHandler ***

// ---------------------------------------------------------------------------
//		* StKeyDialogHandler(ResIDT, LCommander*)
// ---------------------------------------------------------------------------
//	Constructor

StKeyDialogHandler::StKeyDialogHandler(
	ResIDT			inDialogResID, 
	LCommander*		inSuper)

: StDialogHandler(inDialogResID, inSuper)
{
}


// ---------------------------------------------------------------------------
//		* ~StKeyDialogHandler
// ---------------------------------------------------------------------------
//	Destructor

StKeyDialogHandler::~StKeyDialogHandler()
{
}


// ---------------------------------------------------------------------------
//		* EventKeyDown
// ---------------------------------------------------------------------------
//	See if the key is a pushbutton key equivalent. This overrides
//	menu equivalents while the dialog handler is the target.

void
StKeyDialogHandler::EventKeyDown(
	const EventRecord& inMacEvent)
{
	if (!HandlePushButtonKey(inMacEvent))
		LEventDispatcher::EventKeyDown(inMacEvent);
}


// ---------------------------------------------------------------------------
//		* HandlePushButtonKey
// ---------------------------------------------------------------------------
//	If there is a pushbutton with the same first letter as this key character,
//	simulate a mouse-down on that button. Otherwise return false which lets
//	the key event pass through normally.

Boolean
StKeyDialogHandler::HandlePushButtonKey(
	const EventRecord& inMacEvent)
{
	
	// First make sure the command key is down. If not, it's definitely not a key equiv.
	// Also we reject command-period since LDialogBox will do that for us.
	
	if (!(inMacEvent.modifiers & cmdKey) || UKeyFilters::IsCmdPeriod(inMacEvent))
		return false;

	// Okay, it's a command key. Scan the buttons for a matching title.

	char theChar = inMacEvent.message & charCodeMask;
	::UppercaseText(&theChar, 1, smSystemScript);
	
	LArrayIterator iter(mDialog->GetSubPanes());
	LPane* thePane;
	while (iter.Next(&thePane)) {
		
		LPushButton* theButton = (dynamic_cast<LPushButton*>(thePane));
		if (theButton != nil) {

			Str255 title;
			theButton->GetDescriptor(title);
			if (title[1] == theChar) {
				
				// Got a match. Fake a mouse-down on the button.
				
				theButton->SimulateHotSpotClick(kControlButtonPart);
				return true;
				
			}
		}
	}
	
	return false;
}