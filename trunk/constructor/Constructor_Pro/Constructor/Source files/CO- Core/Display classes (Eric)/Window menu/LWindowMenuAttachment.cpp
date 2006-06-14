// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LWindowMenuAttachment.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "LWindowMenuAttachment.h"
#include "LWindowAppAttachment.h"

#include <PP_Messages.h>


// ===========================================================================

#pragma mark *** LWindowMenuAttachment ***

// ---------------------------------------------------------------------------
//		* LWindowMenuAttachment()
// ---------------------------------------------------------------------------
//	Default constructor

LWindowMenuAttachment::LWindowMenuAttachment()
{
	InitWindowMenuAttachment();
}


// ---------------------------------------------------------------------------
//		* LWindowMenuAttachment(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Takes no additional stream data.

LWindowMenuAttachment::LWindowMenuAttachment(
	LStream*	inStream)

: LAttachment(inStream)
{
	InitWindowMenuAttachment();
}


// ---------------------------------------------------------------------------
//		* ~LWindowMenuAttachment
// ---------------------------------------------------------------------------
//	Destructor, ensures that the menu item for this window is removed.

LWindowMenuAttachment::~LWindowMenuAttachment()
{
	RemoveMenu();
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu installation

// ---------------------------------------------------------------------------
//		* ExecuteSelf											[protected]
// ---------------------------------------------------------------------------
//	Dispatch attachment message. The window's attachment (this object)
//	only needs to process attachment messages once, immediately after
//	the window is created. When this happens, it calls InstallMenu()
//	to create the menu item for this window, then ceases to listen to
//	messages.

void
LWindowMenuAttachment::ExecuteSelf(
	MessageT	/* inMessage */,
	void*		/* ioParam */)
{
	if (!mAddedMenu) {
		SetMessage(msg_Nothing);
		mAddedMenu = true;
		InstallMenu();
	}
}



// ---------------------------------------------------------------------------
//		* InstallMenu											[protected]
// ---------------------------------------------------------------------------
//	Called by ExecuteSelf() the first time this attachment receives a
//	command message. Installs the menu command for this window at the
//	bottom of the window list. Override if you wish to control the
//	placement or text of the menu item.

void
LWindowMenuAttachment::InstallMenu()
{
	if (mWindow != nil)
		LWindowAppAttachment::AddWindow(mWindow, this, nil, false);
}


// ---------------------------------------------------------------------------
//		* RemoveMenu											[protected]
// ---------------------------------------------------------------------------
//	Called from destructor. Removes the menu command for this window.

void
LWindowMenuAttachment::RemoveMenu()
{
	if (mWindow != nil)
		LWindowAppAttachment::RemoveWindow(mWindow);
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu installation

// ---------------------------------------------------------------------------
//		* FindCommandStatus										[protected]
// ---------------------------------------------------------------------------
//	Called from LWindowAppAttachment::FindCommandStatus.
// 	Updates the menu information for this window.

void
LWindowMenuAttachment::FindCommandStatus(
	SCommandStatus*		inCommandStatus)
{
	if (mWindow != nil) {
		*(inCommandStatus->enabled) = true;
		*(inCommandStatus->usesMark) = true;
		*(inCommandStatus->mark) = mWindow->IsActive() ? checkMark : 0;
		
		// JCD 980611 - If the name of the window begins with a hyphen
		// (ASCII 0x2D), this will cause the window title to be displayed
		// as a divider line in the window menu instead of the actual
		// title (this is how the Menu Manager works). So to fake out the
		// Menu Manager, we'll replace the hyphen with an opt-hyphen
		// (ASCII 0xD0) so it will display correctly. This fixes bug MW07434.
		// This is only necessary on Mac.
	
	#if Constructor_ForMacOS	

		Str255 tempName;
		mWindow->GetDescriptor(tempName);
		if ( tempName[0] && (tempName[1] == 0x2D) ) {
			tempName[1] = 0xD0;
		}
		
		LString::CopyPStr( tempName, inCommandStatus->name, tempName[0] + 1 );
	
	#else // Constructor_ForMacOS
	
		mWindow->GetDescriptor(inCommandStatus->name);
	
	#endif // Constructor_ForMacOS

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu installation

// ---------------------------------------------------------------------------
//		* InitWindowMenuAttachment								[private]
// ---------------------------------------------------------------------------
//	Called from both constructors. Sets the message filter to accept any
//	message. This is used temporarily until InstallMenu() is called; then
//	this object ceases to listen for messages.

void
LWindowMenuAttachment::InitWindowMenuAttachment()
{
	SetMessage(msg_AnyMessage);
	mWindow = (dynamic_cast<LWindow*>(mOwnerHost));
	mAddedMenu = false;
}
