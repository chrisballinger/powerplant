// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StAboutDialogHandler.cpp	© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/05/96
//	   $Date: 2006/01/18 01:32:41 $
//	$History: StAboutDialogHandler.cpp $
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

#include "StAboutDialogHandler.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : PP headers
#include <PP_Messages.h>

	// Universal headers
#include <Sound.h>


// ===========================================================================

#pragma mark *** StAboutDialogHandler ***

// ---------------------------------------------------------------------------
//		* StAboutDialogHandler
// ---------------------------------------------------------------------------
//	Constructor

StAboutDialogHandler::StAboutDialogHandler(
	ResIDT			inDialogResID,
	LCommander*		inSuper)

: StDialogHandler(inDialogResID, inSuper)
{
	mBeanCount = 0;
}


// ---------------------------------------------------------------------------
//		* ~StAboutDialogHandler
// ---------------------------------------------------------------------------
//	Destructor

StAboutDialogHandler::~StAboutDialogHandler()
{
}


// ---------------------------------------------------------------------------
//		* EventMouseDown
// ---------------------------------------------------------------------------
//	Ignore all mouse-down events, but close the dialog box.

void
StAboutDialogHandler::EventMouseDown(
	const EventRecord& /* inMacEvent */)
{
	ListenToMessage(cmd_Close, nil);
}


// ---------------------------------------------------------------------------
//		* EventKeyDown
// ---------------------------------------------------------------------------
//	This ain't the chicken; it's the egg. :-)

void
StAboutDialogHandler::EventKeyDown(
	const EventRecord& inMacEvent)
{
	unsigned char theChar = inMacEvent.message & charCodeMask;
	
	mBeanCount <<= 8;
	mBeanCount += theChar;

	if (mBeanCount == 'moof') {
		StApplicationContext appContext;
		Handle eggSound = ::Get1Resource('THNG', 1450);
		if (eggSound != nil)
			::SndPlay(nil, (SndListHandle) eggSound, false);
		::ReleaseResource(eggSound);
	}
}
