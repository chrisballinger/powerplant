// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StKeyDialogHandler.h		© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 08/09/95
//	   $Date: 2006/01/18 01:32:41 $
//	$History: StKeyDialogHandler.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Dialog handlers
//	Improved commenting.
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

#pragma once

	// PowerPlant : Utility classes
#include <UModalDialogs.h>

// ===========================================================================
//		* StKeyDialogHandler
// ===========================================================================
//
//	StKeyDialogHandler is a variant of PowerPlant's StDialogHandler
//	that adds support for keyboard shortcuts.
//
// ===========================================================================

class StKeyDialogHandler : public StDialogHandler {

public:
							StKeyDialogHandler(
									ResIDT				inDialogResID,
									LCommander*			inSuper);
	virtual					~StKeyDialogHandler();

protected:
	virtual void			EventKeyDown(
									const EventRecord&	inMacEvent);
	virtual Boolean			HandlePushButtonKey(
									const EventRecord&	inMacEvent);

};
