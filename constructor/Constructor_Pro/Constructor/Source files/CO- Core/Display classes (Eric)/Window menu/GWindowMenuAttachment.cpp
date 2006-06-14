// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GWindowMenuAttachment.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/14/96
//     $Date: 2006/01/18 01:32:47 $
//	$History: GWindowMenuAttachment.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:58
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window menu
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:25
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Window menu'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "GWindowMenuAttachment.h"

	// Core : Display classes (Eric) : Window menu
#include "LWindowAppAttachment.h"


// ===========================================================================
//		* Internal message IDs
// ===========================================================================

const MessageT msg_GetNestLevel = 'gnlv';


// ===========================================================================
//		* Ugly hack to give me access to subcommander list. Ugh!
// ===========================================================================

class GCommander : public LCommander {
	friend class GWindowMenuAttachment;
};


// ===========================================================================

#pragma mark *** GWindowMenuAttachment ***

// ---------------------------------------------------------------------------
//		* GWindowMenuAttachment()
// ---------------------------------------------------------------------------
//	Constructor

GWindowMenuAttachment::GWindowMenuAttachment()
{
	mTopLevelWindow = true;
	mNestingLevel = 0;
}


// ---------------------------------------------------------------------------
//		* GWindowMenuAttachment(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		Boolean:		true if window is top-level window
//							(i.e. project window)

GWindowMenuAttachment::GWindowMenuAttachment(
	LStream*	inStream)

: LWindowMenuAttachment(inStream)
{
	(*inStream)	>> mTopLevelWindow;
	mNestingLevel = 0;
}


// ---------------------------------------------------------------------------
//		* ~GWindowMenuAttachment
// ---------------------------------------------------------------------------
//	Destructor

GWindowMenuAttachment::~GWindowMenuAttachment()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu configuration

// ---------------------------------------------------------------------------
//		* FindCommandStatus										[protected]
// ---------------------------------------------------------------------------
//	Overriden to insert spaces in the menu title when the window is
//	considered subordinate to another window.

void
GWindowMenuAttachment::FindCommandStatus(
	SCommandStatus*		inCommandStatus)
{

	// Do standard menu item.

	LWindowMenuAttachment::FindCommandStatus(inCommandStatus);

	// Insert spaces to represent location in hierarchy.

	if (mNestingLevel > 0) {
	
		LStr255 menuItem;
		SInt16 nest = mNestingLevel;
		while (nest--) {
			menuItem += (StringPtr)"\002  ";
		}
	
		menuItem += inCommandStatus->name;
		LString::CopyPStr(menuItem, inCommandStatus->name);
	}
}


// ---------------------------------------------------------------------------
//		* InstallMenu											[protected]
// ---------------------------------------------------------------------------
//	Overriden to determine the appropriate nesting level (i.e. where
//	does this window sit in the window hierarchy).

void
GWindowMenuAttachment::InstallMenu()
{

	// Sanity check: Make sure we have a window.
	
	if (mWindow == nil)
		return;

	// Kind of ugly hack to find the window that's our parent
	// in the command hierarchy.
	
	LWindow* afterWindow = nil;

	if (!mTopLevelWindow) {
		GCommander* super = (GCommander*) mWindow->GetSuperCommander();
		
		while (super != nil) {
			
			// Work through this commander's subcommander list. See if there are
			// any other windows.
			
			LArrayIterator iter(super->mSubCommanders);
			LCommander* theSub;
			while (iter.Next(&theSub)) {
				LWindow* window = (dynamic_cast<LWindow*>(theSub));
				if ((window != nil) && (window != mWindow)) {
					afterWindow = window;
					break;
				}
			}
			
			if (afterWindow != nil)
				break;
			
			// Move on to next super.
			
			super = (GCommander*) super->GetSuperCommander();
			
		}
	}

	// Found a window to insert after, see what its indent level is.
	
	if (afterWindow != nil) {
		SInt16 nestLevel = 0;
		afterWindow->ExecuteAttachments(msg_GetNestLevel, &nestLevel);
		mNestingLevel = nestLevel + 1;
	}

	// Okay, now we know our position in the hierarchy. Insert menu item.

	LWindowAppAttachment::AddWindow(mWindow, this, afterWindow, false);

	// Make sure we respond to get nesting level messages.

	SetMessage(msg_GetNestLevel);

}


// ---------------------------------------------------------------------------
//		* ExecuteSelf											[protected]
// ---------------------------------------------------------------------------
//	Overriden to allow one window menu attachment to query another window
//	for its nesting level.

void
GWindowMenuAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{
	if (inMessage == msg_GetNestLevel) {
		SInt16* outNestLevel = (SInt16*) ioParam;
		*outNestLevel = mNestingLevel;
	}
	else
		LWindowMenuAttachment::ExecuteSelf(inMessage, ioParam);
}
