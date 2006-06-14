// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LWindowPositioner.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "LWindowPositioner.h"
#include <UResourceMgr.h>


LArray LWindowPositioner::sWindowPositioners(sizeof (LWindowPositioner*));


// ===========================================================================

#pragma mark *** LWindowPositioner ***

// ---------------------------------------------------------------------------
//		* CreateWindowPositionerStream							[static]
// ---------------------------------------------------------------------------
//	Stream creation function

void*
LWindowPositioner::CreateWindowPositionerStream(
	LStream*	inStream)
{
	return new LWindowPositioner(inStream);
}


// ---------------------------------------------------------------------------
//		* LWindowPositioner(...)
// ---------------------------------------------------------------------------
//	Procedural constructor, specifying the window to attach to,
//	the WSPC resource ID, stagger amounts, and whether to adjust
//	window position and/or size.

LWindowPositioner::LWindowPositioner(
	LWindow*		inWindow,
	ResIDT			inResID,
	SInt16			inHorizStagger,
	SInt16			inVertStagger,
	Boolean			inAdjustPosition,
	Boolean			inAdjustSize)
{

	// Do common initialization.

	InitWindowPositioner();

	// Fill in fields.
	
	mWindow = inWindow;
	ThrowIfNil_(mWindow);

	mWindow->AddAttachment(this);
	mResID = inResID;
	mWindowStagger.width = inHorizStagger;
	mWindowStagger.height = inVertStagger;
	mAdjustPosition = inAdjustPosition;
	mAdjustSize = inAdjustSize;

}


// ---------------------------------------------------------------------------
//		* LWindowPositioner(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for LWindowPositioner is:
//
//		[LAttachment data]
//		Int16:		WSPC resource ID
//		Int16:		Horizontal window stagger amount
//		Int16:		Vertical window stagger amount
//		Boolean:	True to adjust window position
//		Boolean:	True to adjust window size

LWindowPositioner::LWindowPositioner(
	LStream*		inStream)

: LAttachment(inStream)

{

	// Do common initialization.

	InitWindowPositioner();
	
	// Read stream data.

	(*inStream)	>> mResID
				>> mWindowStagger.width
				>> mWindowStagger.height
				>> mAdjustPosition
				>> mAdjustSize;

	// Figure out who the window is.
	
	mWindow = (dynamic_cast<LWindow*>(mOwnerHost));
	ThrowIfNil_(mWindow);

}


// ---------------------------------------------------------------------------
//		* ~LWindowPositioner
// ---------------------------------------------------------------------------
//	Destructor

LWindowPositioner::~LWindowPositioner()
{

	// Remove self from list of positioners.
	
	sWindowPositioners.Remove(this);

}


// ===========================================================================

#pragma mark -
#pragma mark ** message dispatch

// ---------------------------------------------------------------------------
//		* ExecuteSelf											[protected]
// ---------------------------------------------------------------------------
//	Catch the cmd_PositionWindow and cmd_Close messages.

void
LWindowPositioner::ExecuteSelf(
	MessageT	inMessage,
	void*		/* ioParam */)
{
	switch (inMessage) {

		case cmd_PositionWindow:
			DoWindowPosition();
			break;

		case cmd_RecordPosition:
			DoRecordWindowPosition();
			break;

		case cmd_Close:
			if (mRecordWindowInfo) {
				mRecordWindowInfo = false;
				DoRecordWindowPosition();
			}
			break;

	}
}


// ---------------------------------------------------------------------------
//		* DoWindowPosition										[protected]
// ---------------------------------------------------------------------------
//	Place the window at a reasonable location. If a window position
//	was recorded previously, try to place the window at the same location
//	again. Be sure that the window is completely visible on one display.

void
LWindowPositioner::DoWindowPosition()
{

	// Only do window position once.
	
	if (mHasSetPosition)
		return;
	mHasSetPosition = true;

	// Determine if we record window position. If a window positioner
	// already exists for this resource ID (i.e. window kind), it takes
	// precedence. Otherwise, we'll record the window position when
	// this window closes.
	
	LWindowPositioner* existingPositioner = FindPositionerByKind(mResID);
	mRecordWindowInfo = (existingPositioner == nil);

	// Read window position.
	
	SWindowPositionH positionH = ReadWindowPosition(mResID);
	
	// Place window accordingly.
	
	if (mAdjustSize)
		ChooseWindowSize(positionH);
	if (mAdjustPosition)
		ChooseWindowPosition(positionH);
	
}


// ---------------------------------------------------------------------------
//		* DoRecordWindowPosition								[protected]
// ---------------------------------------------------------------------------
//	When a window is closed, we may want to record its position so that
//	future windows of this type are placed at the same location.

void
LWindowPositioner::DoRecordWindowPosition()
{
	
	// Create the window position record.
	
	SWindowPositionH positionH = RecordWindowPosition();
	
	// Save it to the appropriate place.
	
	if (positionH != nil) {
		WriteWindowPosition(mResID, positionH);
		::DisposeHandle((Handle) positionH);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** window placement

// ---------------------------------------------------------------------------
//		* ChooseWindowPosition									[protected]
// ---------------------------------------------------------------------------
//	Place the window at an appropriate location, given the available
//	displays.

void
LWindowPositioner::ChooseWindowPosition(
	SWindowPositionH	inWindowPositionH)
{

	// Choose the location that will serve as a seed for the
	// window position. If a window position record existed,
	// use its information; otherwise, use the existing bounds
	// rect of the window (which probably comes from the PPob).

	Rect strucRect = UWindows::GetWindowStructureRect(GetWindowFromPort(mWindow->GetMacPort()));
	Rect contRect = UWindows::GetWindowContentRect(GetWindowFromPort(mWindow->GetMacPort()));
	
	Rect border;
	border.left = contRect.left - strucRect.left;
	border.right = strucRect.right - contRect.right;
	border.top = contRect.top - strucRect.top;
	border.bottom = strucRect.bottom - contRect.bottom;

	if (inWindowPositionH != nil) {
		
		// Window position record was found, see where it
		// thinks this window should be.
	
		Point seedPt = (**inWindowPositionH).windowPosition;
		::OffsetRect(&strucRect, seedPt.h - contRect.left, seedPt.v - contRect.top);
		::OffsetRect(&contRect, seedPt.h - contRect.left, seedPt.v - contRect.top);
	}
	
	// Choose display device for this window.

	GDHandle deviceH = ChooseDisplayForWindow(strucRect);

	// Make sure window will fit on this device.
	
	CropWindowToDisplay(strucRect, deviceH);
	
	// Choose location for window on this device.
	
	PlaceWindowOnDisplay(strucRect, deviceH);

	// Move window to that location.
	
	contRect = strucRect;
	contRect.left += border.left;
	contRect.top += border.top;
	contRect.right -= border.right;
	contRect.bottom -= border.bottom;
	mWindow->DoSetBounds(contRect);

}


// ---------------------------------------------------------------------------
//		* ChooseWindowSize										[protected]
// ---------------------------------------------------------------------------
//	Called only if mAdjustSize is true. Restore the window size to the
//	size that was previously recorded.

void
LWindowPositioner::ChooseWindowSize(
	SWindowPositionH	inWindowPositionH)
{
	
	// Set window size to whatever was recorded in the settings
	// resource (if there is one). If not, we just use the
	// existing size.
	
	if (inWindowPositionH != nil) {
	
		// For now we don't worry about whether this size is
		// acceptable on the user's displays. ChooseWindowPosition
		// will shrink the window if there's a problem.
	
		SDimension16 size = (**inWindowPositionH).windowSize;
		
		Rect bounds = UWindows::GetWindowContentRect(GetWindowFromPort(mWindow->GetMacPort()));
		bounds.right = bounds.left + size.width;
		bounds.bottom = bounds.top + size.height;
		
		mWindow->DoSetBounds(bounds);
	
	}
}


// ---------------------------------------------------------------------------
//		* RecordWindowPosition									[protected]
// ---------------------------------------------------------------------------
//	Write the current window position and size to a handle for storage
//	in the preferences file.

SWindowPositionH
LWindowPositioner::RecordWindowPosition()
{

	// Record window position and size.

	LHandleStream positionH;

	// Grab the window's position and size.
	
	Rect bounds = UWindows::GetWindowContentRect(GetWindowFromPort(mWindow->GetMacPort()));

	// JCD 980612 - 
	// we will tweak this information to ensure the bounds of the window
	// are within the minimum and maximum size boundries. I know, seems
	// silly, how could this happen, right? Well, at least for the minimum
	// size, try closing a Windowshaded window. The size will be very
	// small when it reopens and this will cause very odd behavior.
	//
	// This will only be effective if the window does specify minimum
	// and maximum sizes (i.e. no negative numbers for them).
	//
	// This fixes bug MW00502.

	Rect minMaxSize;
	mWindow->GetMinMaxSize(minMaxSize);
	
	// copy to some more descriptive variables
	
	SInt16 minWidth = minMaxSize.left;
	SInt16 minHeight = minMaxSize.top;
	SInt16 maxWidth = minMaxSize.right;
	SInt16 maxHeight = minMaxSize.bottom;
	
	// top/left will be our anchor, since the Mac drawing system uses
	// that as origin. So when we adjust, we'll add/subtract from the
	// right and bottom.

	if ( (minWidth >= 0) && ((bounds.right - bounds.left) < minWidth) ) {
		bounds.right += (minWidth - (bounds.right - bounds.left));
	}
	
	if ( (minHeight >= 0) && ((bounds.bottom - bounds.top) < minHeight) ) {
		bounds.bottom += (minHeight - (bounds.bottom - bounds.top));
	}
	
	if ( (maxWidth >= 0) && ((bounds.right - bounds.left) > maxWidth) ) {
		bounds.right -= ((bounds.right - bounds.left) - maxWidth);
	}
	
	if ( (maxHeight >= 0) && ((bounds.bottom - bounds.top) > maxHeight) ) {
		bounds.bottom -= ((bounds.bottom - bounds.top) - maxHeight);
	}

	// Record this information.
	
	positionH.WriteData(&bounds.top, sizeof(bounds.top));	// Point is (v,h)
	positionH.WriteData(&bounds.left, sizeof(bounds.left));
	SInt16 width = bounds.right - bounds.left;
	positionH.WriteData(&width, sizeof(width));				// SDimension16 is (h,v)
	SInt16 height = bounds.bottom - bounds.top;
	positionH.WriteData(&height, sizeof(height));
	
	// Return this handle.

	return (SWindowPositionH) (positionH.DetachDataHandle());

}


// ---------------------------------------------------------------------------
//		* ChooseDisplayForWindow								[protected]
// ---------------------------------------------------------------------------
//	Decide which device will be used to display this window.

GDHandle
LWindowPositioner::ChooseDisplayForWindow(
	const Rect&	inWindowBounds)
{

	// If the window is already at least partially on one display,
	// leave it there.
	
	GDHandle curDevice = UWindows::FindDominantDevice(inWindowBounds);
	if (curDevice != nil)
		return curDevice;
	
	// Not on any device at all, just put it on the main screen.
	
	return ::GetMainDevice();

}


// ---------------------------------------------------------------------------
//		* CropWindowToDisplay									[protected]
// ---------------------------------------------------------------------------
//	The display has been chosen. Make sure that the window will fit
//	on this display. If the window is larger in either dimension than
//	the display, resize it so that it will fit.

void
LWindowPositioner::CropWindowToDisplay(
	Rect&		ioWindowBounds,
	GDHandle	inDeviceH)
{
	
	// Calculate size of window and device.
	
	SDimension16 devSize;
	Rect devBounds;
	CalcUsableDisplayArea(inDeviceH, devBounds, devSize);

	SDimension16 winSize = { ioWindowBounds.right - ioWindowBounds.left,
							 ioWindowBounds.bottom - ioWindowBounds.top };

	// Reduce window size as necessary.
	
	if (winSize.width > devSize.width)
		ioWindowBounds.right -= (winSize.width - devSize.width);
	if (winSize.height > devSize.height)
		ioWindowBounds.bottom -= (winSize.height - devSize.height);
	
}


// ---------------------------------------------------------------------------
//		* PlaceWindowOnDisplay									[protected]
// ---------------------------------------------------------------------------
//	Choose a location for this window within the given display. Make sure
//	that it doesn't land directly on top of another window.

void
LWindowPositioner::PlaceWindowOnDisplay(
	Rect&			ioWindowBounds,
	GDHandle		inDeviceH)
{
	
	// Calculate the rectangle in which the upper-left corner of
	// the window must be placed.
	
	SDimension16 devSize;
	Rect devBounds;
	CalcUsableDisplayArea(inDeviceH, devBounds, devSize);

	SDimension16 winSize = { ioWindowBounds.right - ioWindowBounds.left,
							 ioWindowBounds.bottom - ioWindowBounds.top };
	
	Rect ulBounds = devBounds;
	ulBounds.right -= winSize.width;
	ulBounds.bottom -= winSize.height;

	// Make sure there's at least a 1-pixel wide/high area for window.
	
	if (ulBounds.right <= ulBounds.left)
		ulBounds.right = ulBounds.left + 1;
	if (ulBounds.bottom <= ulBounds.top)
		ulBounds.bottom = ulBounds.top + 1;

	// Now that we know the window placement bounds, make sure the proposed location
	// is within that area.
	
	Point newHome = topLeft(ioWindowBounds);
	
	if (newHome.h < ulBounds.left)
		newHome.h = ulBounds.left;
	if (newHome.h >= ulBounds.right)
		newHome.h = ulBounds.right - 1;
	if (newHome.v < ulBounds.top)
		newHome.v = ulBounds.top;
	if (newHome.v >= ulBounds.bottom)
		newHome.v = ulBounds.bottom - 1;
	
	Point firstHome = newHome;

	// Now test locations until we exhaust the possibilities...

	while (!ValidateWindowPosition(newHome, ulBounds)) {
	
		// Position wasn't valid, stagger window position.
		
		newHome.h += mWindowStagger.width;
		while (newHome.h >= ulBounds.right) {
			newHome.h -= (ulBounds.right - ulBounds.left);
		}
		
		newHome.v += mWindowStagger.height;
		while (newHome.v >= ulBounds.bottom) {
			newHome.v -= (ulBounds.bottom - ulBounds.top);
		}
		
		// See if we've cycled back to the original location.
		// If so, stop the loop. We're not going to find a happy home
		// for this window.
		
		if ((newHome.h == firstHome.h) && (newHome.v == firstHome.v))
			break;

	}
	
	// We've decided where the upper left corner is, bump other
	// bounds to match it.
	
	::OffsetRect(&ioWindowBounds, newHome.h - ioWindowBounds.left, newHome.v - ioWindowBounds.top);
	
}


// ---------------------------------------------------------------------------
//		* CalcUsableDisplayArea									[protected]
// ---------------------------------------------------------------------------
//	Calculate the usable display area with the given device.
//	Leave room for the usual 4-pixel border at the edges of the device,
//	and (if appropriate) the menu bar at the top of the device.

void
LWindowPositioner::CalcUsableDisplayArea(
	GDHandle			inDeviceH,
	Rect&				outDeviceBounds,
	SDimension16&		outDeviceSize)
{

	// Get device's boundaries.

	outDeviceBounds = (**inDeviceH).gdRect;
	
	// Adjust for rounded borders and menu bar (if appropriate).
	
	outDeviceBounds.left += 4;
	outDeviceBounds.right -= 4;
	outDeviceBounds.top += 4;
	outDeviceBounds.bottom -= 4;
	
	if (inDeviceH == ::GetMainDevice())
		outDeviceBounds.top += ::GetMBarHeight();
	
	
	// Calculate size. This is redundant information, but it
	// is handy for the places we call this function.
	
	outDeviceSize.width = outDeviceBounds.right - outDeviceBounds.left;
	outDeviceSize.height = outDeviceBounds.bottom - outDeviceBounds.top;
	
}


// ---------------------------------------------------------------------------
//		* ValidateWindowPosition								[protected]
// ---------------------------------------------------------------------------
//	Called by PlaceWindowOnDisplay. Return true if this tentative window
//	position is acceptable.

Boolean
LWindowPositioner::ValidateWindowPosition(
	Point&		inStructUL,
	Rect&		inULBounds)
{

	// First (obvious) check: See if UL corner is within the
	// acceptable bounds.
	
	if (!::PtInRect(inStructUL, &inULBounds))
		return false;
	
	// Second, see if any other window already has this location.
	// Iterate over the window list.
	
	WindowPtr macWindowP = ::FrontWindow();
	while (macWindowP != nil) {
	
		// Grab the PowerPlant window object for this window.
	
		LWindow* theWindow = LWindow::FetchWindowObject(macWindowP);
		
		// Make sure that it is a valid window object, and that
		// it points to a regular, visible window.
		
		if (	(theWindow != nil)
			 && (theWindow != mWindow)
			 && (theWindow->HasAttribute(windAttr_Regular))
			 && (theWindow->IsVisible())) {

			// Okay, this is a regular, visible window, check its bounds.
			
			Rect winStruct = UWindows::GetWindowStructureRect(macWindowP);
			if ((winStruct.top == inStructUL.v) && (winStruct.left == inStructUL.h))
				return false;

		}
		
		// Move ahead to next window.
		
		macWindowP = MacGetNextWindow(macWindowP);
	
	}

	// It's in acceptable bounds and it doesn't conflict with another window,
	// this position is OK.
	
	return true;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** window position resource

// ---------------------------------------------------------------------------
//		* ReadWindowPosition									[protected]
// ---------------------------------------------------------------------------
//	Read the window position from a WSPC resource if available.
//	Returns nil if no such resource was found. Override if you want
//	to read from a specific resource fork (such as a preferences file).

SWindowPositionH
LWindowPositioner::ReadWindowPosition(
	ResIDT	inResID)
{
	Handle positionH = ::Get1Resource(ResType_WindowPosition, inResID);
	if (positionH != nil)
		::DetachResource(positionH);
	
	return (SWindowPositionH) positionH;
}


// ---------------------------------------------------------------------------
//		* WriteWindowPosition									[protected]
// ---------------------------------------------------------------------------
//	Write the window position to a WSPC resource. Override if you want
//	to write to a specific resource fork (such as a preferences file).

void
LWindowPositioner::WriteWindowPosition(
	ResIDT				inResID,
	SWindowPositionH	inWindowPositionH)
{
	StNewResource newPosResource(ResType_WindowPosition, inResID);
	::SetHandleSize((Handle) newPosResource, ::GetHandleSize((Handle) inWindowPositionH));
	::BlockMoveData(*inWindowPositionH, *newPosResource, ::GetHandleSize((Handle) inWindowPositionH));
//	::HandAndHand((Handle) inWindowPositionH, (Handle) newPosResource);
	newPosResource.Write();
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessor

// ---------------------------------------------------------------------------
//		* FindPositionerByKind									[protected]
// ---------------------------------------------------------------------------
//	See if another window positioner exists for this resource kind.

LWindowPositioner*
LWindowPositioner::FindPositionerByKind(
	ResIDT	inResID)
{
	LArrayIterator iter(sWindowPositioners, LArrayIterator::from_End);
	LWindowPositioner* pos;
	
	while (iter.Previous(&pos)) {
		if ((pos->mResID == inResID) && (pos != this))
			return pos;
	}
	
	return nil;
}


// ===========================================================================

#pragma mark -
#pragma mark ** common initialization

// ---------------------------------------------------------------------------
//		* InitWindowPositioner									[private]
// ---------------------------------------------------------------------------
//	Common initialization for LWindowPositioner constructors.

void
LWindowPositioner::InitWindowPositioner()
{
	static int hasRegisteredFlipper = false;
	
	// Register our flipper.
	if (!hasRegisteredFlipper) {
		OSStatus theErr = ::CoreEndianInstallFlipper(kCoreEndianResourceManagerDomain, ResType_WindowPosition,
	                                                 LWindowPositioner::FlipWindowPosition, NULL);
		ThrowIfOSStatus_(theErr);
		hasRegisteredFlipper = true;
	}
	
	// Clear variables.
	
	mHasSetPosition = false;
	mRecordWindowInfo = false;
	mWindowStagger.width = mWindowStagger.height = 16;
	mWindow = nil;
	mResID = 0;

	// Always pass attachable messages through to owner.

	SetExecuteHost(true);
	
	// Make sure that this attachment responds to all messages.
	
	SetMessage(msg_AnyMessage);
	
	// Add self to list of positioners.
	
	sWindowPositioners.InsertItemsAt(1, LArray::index_Last, &*this);
	
}

// ---------------------------------------------------------------------------
//		* FlipWindowPosition									[private]
// ---------------------------------------------------------------------------
//	Resource flipper for ResType_WindowPosition.

OSStatus
LWindowPositioner::FlipWindowPosition(
	OSType	dataDomain,
	OSType	dataType,
	short	id,
	void	*dataPtr,
	UInt32	dataSize,
	Boolean	currentlyNative,
	void	*refcon)
{
	if (dataSize != sizeof(SWindowPosition)) {
		return paramErr;
	}
	SWindowPosition *position = (SWindowPosition *) dataPtr;
	position->windowPosition.v = Endian16_Swap(position->windowPosition.v);
	position->windowPosition.h = Endian16_Swap(position->windowPosition.h);
	position->windowSize.width = Endian16_Swap(position->windowSize.width);
	position->windowSize.height = Endian16_Swap(position->windowSize.height);
	return noErr;
}
