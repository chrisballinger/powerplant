// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSSourceStatusButton.cpp	© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 03/12/95
//	   $Date: 2006/01/18 01:33:02 $
//	$History: RSSourceStatusButton.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/05/97    Time: 2:23p
//	Updated in $/ConstructorWin/Sources
//	Added #include for MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:00
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
	#include "ctor.h"
#endif

// ===========================================================================

#include "RSSourceStatusButton.h"

	// Core : Documents : Project document
#include "PDSourceControl.h"

	// PowerPlant : Utility classes
#include <UDrawingState.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	ICN_CKIDReadOnlyIcon		= 15151;
const ResIDT	ICN_CKIDWriteableIcon		= 15152;
const ResIDT	ICN_CKIDModifyReadOnlyIcon	= 15153;
const ResIDT	ICN_FileLockedIcon			= 15154;
const ResIDT	ICN_FileReadOnlyIcon		= 15155;


// ===========================================================================

#pragma mark *** RSSourceStatusButton ***

// ---------------------------------------------------------------------------
//		* RSSourceStatusButton(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

RSSourceStatusButton::RSSourceStatusButton(
	LStream*	inStream)

: LControl(inStream)

{
	mSettings = nil;
}


// ---------------------------------------------------------------------------
//		* ~RSSourceStatusButton
// ---------------------------------------------------------------------------
//	Destructor

RSSourceStatusButton::~RSSourceStatusButton()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* AttachSourceControl
// ---------------------------------------------------------------------------
//	Tell the button where the source control object is.

void
RSSourceStatusButton::AttachSourceControl(
	PDSourceControl*	inSettings)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inSettings);
	
	// Listen to messages from SCC object.
	
	mSettings = inSettings;
	mSettings->AddListener(this);
	
	// Trigger redraw.
	
	Refresh();

}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Draw the current status.

void
RSSourceStatusButton::DrawSelf()
{

	// Validate pointers.

	ValidateThis_();
	
	// Be sure pen is normal.
	
	StColorPenState::Normalize();

	// Decide which graphic to draw.
	
	ResIDT theID = resID_Undefined;
	if (mSettings != nil) {
		if (mSettings->IsLocked())
			theID = ICN_FileLockedIcon;
		else if (mSettings->HasSCCInfo()) {
			if (mSettings->IsReadOnly())
				theID = ICN_CKIDReadOnlyIcon;
			else if (mSettings->IsModifyReadOnly())
				theID = ICN_CKIDModifyReadOnlyIcon;
			else
				theID = ICN_CKIDWriteableIcon;
		}
		else
			theID = ICN_CKIDWriteableIcon;

	}

	// Draw the graphic.

	Rect frame;
	CalcLocalFrameRect(frame);

	::EraseRect(&frame);
	if (theID != resID_Undefined) {
		Rect iconFrame = frame;
		if (iconFrame.bottom - iconFrame.top > 16) {
			iconFrame.top += (iconFrame.bottom - iconFrame.top - 16) / 2;
			iconFrame.bottom = iconFrame.top + 16;
		}
		if (iconFrame.right - iconFrame.left > 16) {
			iconFrame.left += (iconFrame.right - iconFrame.left - 16) / 2;
			iconFrame.right = iconFrame.left + 16;
		}
		if (mEnabled != triState_Off)
			::PlotIconID(&iconFrame, atNone, ttNone, theID);
		else
			::PlotIconID(&iconFrame, atNone, ttDisabled, theID);
	}

	// Draw a black frame around the button.

	::FrameRect(&frame);

}


// ===========================================================================

#pragma mark -
#pragma mark ** mouse-down behaviors

// ---------------------------------------------------------------------------
//		* HotSpotResult											[protected]
// ---------------------------------------------------------------------------
//	If this document is marked read-only, change it to modified read-only.

void
RSSourceStatusButton::HotSpotResult(
	SInt16	/* inHotSpot */)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// If we have a source-control setting, make file MRO.

	if (mSettings != nil) {
		if (mSettings->IsReadOnly()) {
			mSettings->ModifyReadOnly();
			StatusBlink();
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** source status updates

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for source code control messages.

void
RSSourceStatusButton::ListenToMessage(
	MessageT	inMessage,
	void*		/* ioParam */)
{

	// Validate pointers.

	ValidateThis_();
	
	// Listen for SCC messages.

	switch (inMessage) {

		case msg_SCC_Changed:
			Refresh();
			break;
			
		case msg_ReadOnlyViolation:
			if (IsActive())
				StatusBlink();
			break;
	}
}


// ---------------------------------------------------------------------------
//		* StatusBlink											[protected]
// ---------------------------------------------------------------------------
//	Blink the status indicator, usually as an indication of access control
//	violation.

void
RSSourceStatusButton::StatusBlink()
{

	// Validate pointers.

	ValidateThis_();

	// Force redraw first.

	Draw(nil);
	
	// Now blink the framerect three times.
	
	Rect frame;
	CalcLocalFrameRect(frame);
	::InsetRect(&frame, 1, 1);
	
#if PP_Target_Carbon
	SInt16 menuFlash = 3 << 1;
#else
	SInt16 menuFlash = LMGetMenuFlash() << 1;			//* 2.2d9: BUG FIX #592: flash to match menu blink
#endif
	if (menuFlash < 2)
		menuFlash = 2;

	for (SInt16 i = 0; i < menuFlash; i++) {
		UInt32 junk;
		::InvertRect(&frame);
		::Delay(3, &junk);
	}
	
	// Make sure it looks right again.
	
	DrawSelf();

}
