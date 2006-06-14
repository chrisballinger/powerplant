// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LWindowAppAttachment.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "LWindowAppAttachment.h"
#include "LWindowMenuAttachment.h"


LWindowAppAttachment* LWindowAppAttachment::sWindowAppAttachment = nil;


// ===========================================================================
//		* SWindowMenuInfo
// ===========================================================================
//	(Used internally by window menu code.)

struct SWindowMenuInfo {
	LWindow*				mWindow;
	CommandT				mCommand;
	LWindowMenuAttachment*	mAttachment;
	Boolean					mDividerLine;
};


// ===========================================================================

#pragma mark *** LWindowAppAttachment ***

// ---------------------------------------------------------------------------
//		* LWindowAppAttachment()
// ---------------------------------------------------------------------------
//	Constructor

LWindowAppAttachment::LWindowAppAttachment()

: mWindowMenuInfoList(sizeof (SWindowMenuInfo))
{
	mNextCommandNumber = cmd_FirstWindow;
	mWindowMenuID = 0;
	mWindowMenuStart = 0;
	
	if (sWindowAppAttachment == nil)
		sWindowAppAttachment = this;
}


// ---------------------------------------------------------------------------
//		* ~LWindowAppAttachment
// ---------------------------------------------------------------------------
//	Destructor

LWindowAppAttachment::~LWindowAppAttachment()
{
	if (sWindowAppAttachment == this)
		sWindowAppAttachment = nil;
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu adders/removers

// ---------------------------------------------------------------------------
//		* AddWindow												[static]
// ---------------------------------------------------------------------------
//	Called by LWindowMenuAttachment::InstallMenu to place a new window
//	in the Windows menu.

void
LWindowAppAttachment::AddWindow(
	LWindow*				inWindow,
	LWindowMenuAttachment*	inAttachment,
	LWindow*				inAfterWindow,
	Boolean					inAddDividerLine)
{
	if (sWindowAppAttachment != nil) {
		sWindowAppAttachment->InternalAddWindow(
									inWindow,
									inAttachment,
									inAfterWindow,
									inAddDividerLine);
	}
}


// ---------------------------------------------------------------------------
//		* RemoveWindow											[static]
// ---------------------------------------------------------------------------
//	Called by LWindowMenuAttachment::RemoveMenu to remove a window
//	from the Windows menu once it is closed.


void
LWindowAppAttachment::RemoveWindow(
	LWindow*	inWindow)
{
	if (sWindowAppAttachment != nil)
		sWindowAppAttachment->InternalRemoveWindow(inWindow);
}


// ---------------------------------------------------------------------------
//		* SetWindowMenu											[static]
// ---------------------------------------------------------------------------
//	Should be called by the application during initialization.
//	Tells the window menu code which menu is the Windows menu.

void
LWindowAppAttachment::SetWindowMenu(
	SInt16	inMenuID)
{
	if (sWindowAppAttachment != nil) {
		LMenu* windowMenu = LMenuBar::GetCurrentMenuBar()->FetchMenu(inMenuID);
		if (windowMenu != nil) {
			MenuHandle windowMenuH = windowMenu->GetMacMenuH();
			if (windowMenuH != nil)
				sWindowAppAttachment->mWindowMenuStart = ::CountMenuItems(windowMenuH);
		}
		sWindowAppAttachment->mWindowMenuID = inMenuID;
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu adders/removers

// ---------------------------------------------------------------------------
//		* ExecuteSelf											[protected]
// ---------------------------------------------------------------------------
//	Intercept command and command-status messages.

void
LWindowAppAttachment::ExecuteSelf(
	MessageT	inMessage,
	void*		ioParam)
{

	SetExecuteHost(true);

	if (inMessage == msg_CommandStatus) {
	
		// Update menu commands.
	
		FindCommandStatus((SCommandStatus*) ioParam);
		
	}
	else if ((inMessage >= cmd_FirstWindow) && (inMessage <= cmd_LastWindow)) {
	
		// It's a window command, bring the window to front.
	
		LWindow* window = CommandToWindow(inMessage);
		if (window != nil) {
			UDesktop::SelectDeskWindow(window);
			SetExecuteHost(false);
		}
	}
}


// ---------------------------------------------------------------------------
//		* FindCommandStatus										[protected]
// ---------------------------------------------------------------------------
//	Dispatch the menu status message to the window menu attachment.
//	It may want to customize the behavior.

void
LWindowAppAttachment::FindCommandStatus(
	SCommandStatus*	inCommandStatus)
{
	
	// First, see if there's a modal dialog active. If so, ignore the request.
	
	if (UDesktop::FrontWindowIsModal()) {
		*(inCommandStatus->usesMark) = true;
		*(inCommandStatus->mark) = 0;
		*(inCommandStatus->enabled) = false;
		SetExecuteHost(false);
		return;
	}
	
	// Find the window menu attachment corresponding to this command.
	
	CommandT command = inCommandStatus->command;
	if ((command >= cmd_FirstWindow) && (command <= cmd_LastWindow)) {
	
		// It's a window command, enable it (and check it if frontmost window).
		// Allow the window menu attachment to handle it.
	
		LWindowMenuAttachment* windowAtt = CommandToAttachment(command);
		if (windowAtt != nil) {
			windowAtt->FindCommandStatus(inCommandStatus);
			SetExecuteHost(false);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu command helpers

// ---------------------------------------------------------------------------
//		* CommandToWindow										[protected]
// ---------------------------------------------------------------------------
//	Given a command number, find the window object that it represents.

LWindow*
LWindowAppAttachment::CommandToWindow(
	CommandT	inCommand)
{
	LArrayIterator iter(mWindowMenuInfoList);
	SWindowMenuInfo info;
	while (iter.Next(&info)) {
		if (info.mCommand == inCommand)
			return info.mWindow;
	}
	
	return nil;
}


// ---------------------------------------------------------------------------
//		* CommandToAttachment									[protected]
// ---------------------------------------------------------------------------
//	Given a command number, find the window menu attacahment for that window.

LWindowMenuAttachment*
LWindowAppAttachment::CommandToAttachment(
	CommandT	inCommand)
{
	LArrayIterator iter(mWindowMenuInfoList);
	SWindowMenuInfo info;
	while (iter.Next(&info)) {
		if (info.mCommand == inCommand)
			return info.mAttachment;
	}
	
	return nil;
}

// ---------------------------------------------------------------------------
//		* WindowToCommand										[protected]
// ---------------------------------------------------------------------------
//	Given a window object, find the command number for the Windows menu
//	item that will select it.

CommandT
LWindowAppAttachment::WindowToCommand(
	const LWindow*	inWindow)
{
	LArrayIterator iter(mWindowMenuInfoList);
	SWindowMenuInfo info;
	while (iter.Next(&info)) {
		if (info.mWindow == inWindow)
			return info.mCommand;
	}
	
	return 0;
}


// ===========================================================================

#pragma mark -
#pragma mark ** internal menu adders/removers

// ---------------------------------------------------------------------------
//		* InternalAddWindow										[protected]
// ---------------------------------------------------------------------------
//	Internal implementation of AddWindow.

void
LWindowAppAttachment::InternalAddWindow(
	LWindow*				inWindow,
	LWindowMenuAttachment*	inAttachment,
	LWindow*				inAfterWindow,
	Boolean					inAddDividerLine)
{
	
	// Sanity check: Make sure pointers are valid. Ignore request if not.
	
	if ((inWindow == nil) || (inAttachment == nil))
		return;
	
	// Find the window menu. Ignore request if not found.
	
	LMenu* menu = nil;
	if (mWindowMenuID != 0)
		menu = LMenuBar::GetCurrentMenuBar()->FetchMenu(mWindowMenuID);
	if (menu == nil)
		return;
	
	MenuHandle menuH = menu->GetMacMenuH();
	if (menuH == nil)
		return;
	
	// Build info structure for this window.
	
	SWindowMenuInfo info;
	info.mWindow = inWindow;
	info.mCommand = mNextCommandNumber++;
	info.mAttachment = inAttachment;
	info.mDividerLine = inAddDividerLine;
	
	// Find placement for this window. By default, we just add
	// it to the end of the menu.
	
	SInt16 insertAfter = 16000;
	if (inAfterWindow != nil) {
		
		// Caller has requested that this follow a specific window
		// in the menu. Find that window.
		
		CommandT command = WindowToCommand(inAfterWindow);
		insertAfter = menu->IndexFromCommand(command);
		if (insertAfter == 0)
			insertAfter = 16000;
	
	}
	
	// Insert divider line if so requested (or if no previous items existed).
	
	if ((mWindowMenuStart > 0) && (::CountMenuItems(menuH) == mWindowMenuStart)) {
		info.mDividerLine = false;
		inAddDividerLine = true;
	}

	if (inAddDividerLine)
		menu->InsertCommand( (StringPtr)"\001-", cmd_Nothing, insertAfter++ );
	
	// Insert menu item. FindCommandStatus will fill in the actual window title
	// later, so we can just use filler text.
	
	menu->InsertCommand( (StringPtr)"\001x", info.mCommand, insertAfter );

	// Add this window to our internal window list.

	mWindowMenuInfoList.InsertItemsAt(1, LArray::index_Last, &info);
	
}


// ---------------------------------------------------------------------------
//		* InternalRemoveWindow									[protected]
// ---------------------------------------------------------------------------
//	Internal implementation of RemoveWindow.

void
LWindowAppAttachment::InternalRemoveWindow(
	LWindow*	inWindow)
{
	
	// Sanity check: Make sure pointers are valid. Ignore request if not.
	
	if (inWindow == nil)
		return;
	
	// Find the window menu. Ignore request if not found.
	
	LMenu* menu = nil;
	if (mWindowMenuID != 0)
		menu = LMenuBar::GetCurrentMenuBar()->FetchMenu(mWindowMenuID);
	if (menu == nil)
		return;
	
	MenuHandle menuH = menu->GetMacMenuH();

	// Find the item. See if it has a divider line.

	LArrayIterator iter(mWindowMenuInfoList);
	SWindowMenuInfo info;
	SInt32 arrayIndex = 0;
	while (iter.Next(&info)) {
		if (info.mWindow == inWindow) {
			arrayIndex = iter.GetCurrentIndex();
			break;
		}
	}

	if (arrayIndex == 0)
		return;

	// Remove the item.
	
	SInt16 index = menu->IndexFromCommand(info.mCommand);
	if (index < mWindowMenuStart)
		return;
	
	menu->RemoveItem(index);
	mWindowMenuInfoList.Remove(&info);

	// Remove the divider line (if one existed).
	
	Boolean hadDivider = info.mDividerLine;
	if ((mWindowMenuStart > 0) && (::CountMenuItems(menuH) == mWindowMenuStart + 1))
		hadDivider = true;
	if (hadDivider)
		menu->RemoveItem(--index);

	//! TEMPORARY: remove divider for following item...

}
