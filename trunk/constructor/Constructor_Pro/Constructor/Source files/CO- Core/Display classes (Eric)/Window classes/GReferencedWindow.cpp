// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GReferencedWindow.cpp		© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/09/95
//     $Date: 2006/01/18 01:32:47 $
//	$History: GReferencedWindow.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 03/17/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Implemented option-click in close box to close all windows. (Suggestion
//	#1067.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Rolled in a couple of bug fixes from Clint. Non-zoomable windows
//	disable zoom command. Only one zoomer can install itself; subsequent
//	zoomers are ignored.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:57
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:28
//	Updated in $/Constructor/Source files/Display classes (Eric)/Window classes
//	Fixed a commenting error.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:27
//	Updated in $/Constructor/Source files/Display classes (Eric)/Window classes
//	Added AttemptClose override. It now treats Close as an essential
//	command, which means it can allocate memory when memory reserves are
//	low.
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

#include "GReferencedWindow.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Display classes (Robin) : Utilities
#include "UGAColorRamp.h"

	// Core : Resource mananger
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// Core : Utility classes
#include "UMemoryUtils.h"


// ===========================================================================

#pragma mark *** GReferencedWindow ***

// ---------------------------------------------------------------------------
//		* GReferencedWindow(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

GReferencedWindow::GReferencedWindow(
	LStream* inStream)

: LWindow(inStream)
{
	mZoomer = nil;
	mHasReference = false;
}


// ---------------------------------------------------------------------------
//		* GReferencedWindow(SWindowInfo&)
// ---------------------------------------------------------------------------
//	Procedural constructor

GReferencedWindow::GReferencedWindow(
	const SWindowInfo&	inWindowInfo)

: LWindow(inWindowInfo)
{
	mZoomer = nil;
	mHasReference = false;
}


// ---------------------------------------------------------------------------
//		* ~GReferencedWindow
// ---------------------------------------------------------------------------
//	Destructor

GReferencedWindow::~GReferencedWindow()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetReference
// ---------------------------------------------------------------------------
//	Set the object "class" and supermodel for AEOM references.

void
GReferencedWindow::SetReference(
	DescType		inWindowType,
	LModelObject*	inSuperModel)
{
	mHasReference = true;
	SetModelKind(inWindowType);
	SetSuperModel(inSuperModel);
}


// ---------------------------------------------------------------------------
//		* SetZoomer
// ---------------------------------------------------------------------------

void
GReferencedWindow::SetZoomer(
	GZoomer*		inZoomer)
{
	if (!mZoomer)
		mZoomer = inZoomer;
}


// ===========================================================================

#pragma mark -
#pragma mark ** zooming behavior

// ---------------------------------------------------------------------------
//		* CalcStandardBoundsForScreen
// ---------------------------------------------------------------------------

void
GReferencedWindow::CalcStandardBoundsForScreen(
	const Rect&		inScreenBounds,
	Rect&			outStdBounds) const
{

	// Do custom calculation of window size. After that's done, make sure the size
	// is reasonable given the minimum size specified in the PPob resource.

	if (mZoomer) {
		mZoomer->CalcStandardSize((GReferencedWindow*) this);
		
		Rect minMax;
		SDimension16 stdSize;
		
		GetMinMaxSize(minMax);
		(const_cast<GReferencedWindow*>(this))->GetStandardSize(stdSize);
		
		if (stdSize.width < minMax.left)
			stdSize.width = minMax.left;
		if (stdSize.height < minMax.top)
			stdSize.height = minMax.top;
		
		(const_cast<GReferencedWindow*>(this))->SetStandardSize(stdSize);
	}

	// Now do standard zooming behavior.

	LWindow::CalcStandardBoundsForScreen(inScreenBounds, outStdBounds);

}


// ---------------------------------------------------------------------------
//		* CalcStandardBounds
// ---------------------------------------------------------------------------
//	Override for bug fix.

Boolean
GReferencedWindow::CalcStandardBounds(
	Rect &outStdBounds) const
{

	// Find GDevice containing largest portion of window.
	
	GDHandle dominantDevice = UWindows::FindDominantDevice(
							  UWindows::GetWindowStructureRect(mMacWindowP));
	
	if (dominantDevice == nil)
		dominantDevice = ::GetMainDevice();		// ES addition 95-11-15
	
	// Compensate for MenuBar on the main screen.

	Rect screenRect = (**dominantDevice).gdRect;
	if (dominantDevice == ::GetMainDevice()) {
		screenRect.top += GetMBarHeight();
	}
	
	CalcStandardBoundsForScreen(screenRect, outStdBounds);
	Rect theRect = UWindows::GetWindowContentRect(mMacWindowP);
	return ::EqualRect(&outStdBounds, &theRect);

}


// ===========================================================================

#pragma mark -
#pragma mark ** zoom window command

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Overriden to implement zoom window command.

void
GReferencedWindow::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	switch (inCommand) {

		case cmd_ZoomWindow:
			outEnabled = ((mAttributes & windAttr_Zoomable) != 0);				//* J-1.0a3: BUG FIX #917: added command
			break;

		default:
			LWindow::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Overriden to implement zoom window command.

Boolean
GReferencedWindow::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	switch (inCommand) {
	
		case cmd_ZoomWindow:
			SendAESetZoom();				//* J-1.0a3: BUG FIX #917: added command
			return true;
	
		default:
			return LWindow::ObeyCommand(inCommand, ioParam);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** option-click

// ---------------------------------------------------------------------------
//		* ClickInGoAway
// ---------------------------------------------------------------------------
//	Overridden to implement option-click in go-away box => Close All.

void
GReferencedWindow::ClickInGoAway(
	const EventRecord&	inMacEvent)
{
	if (::TrackGoAway(mMacWindowP, inMacEvent.where)) {
		if (inMacEvent.modifiers & optionKey) {
			ProcessCommand(cmd_CloseAll);
		}
		else {
			ProcessCommand(cmd_Close);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** overrides to enforce resource context

// ---------------------------------------------------------------------------
//		* Activate
// ---------------------------------------------------------------------------

void
GReferencedWindow::Activate()
{
	StApplicationContext appContext;
	LWindow::Activate();
}


// ---------------------------------------------------------------------------
//		* Deactivate
// ---------------------------------------------------------------------------

void
GReferencedWindow::Deactivate()
{
	StApplicationContext appContext;
	LWindow::Deactivate();
}


// ---------------------------------------------------------------------------
//		* UpdatePort
// ---------------------------------------------------------------------------

void
GReferencedWindow::UpdatePort()
{
	StApplicationContext appContext;
	LWindow::UpdatePort();
}


// ---------------------------------------------------------------------------
//		* HandleClick
// ---------------------------------------------------------------------------

void
GReferencedWindow::HandleClick(
	const EventRecord&	inMacEvent,
	SInt16				inPart)
{
	StApplicationContext appContext;
	LWindow::HandleClick(inMacEvent, inPart);
}


// ===========================================================================

#pragma mark -
#pragma mark ** overrides to handle low-memory situations

// ---------------------------------------------------------------------------
//		* AttemptClose
// ---------------------------------------------------------------------------
//	Overridden to ensure that the close command doesn't fail because
//	memory is low.

void
GReferencedWindow::AttemptClose()
{
	StCriticalSection crit;
	LWindow::AttemptClose();
}


// ===========================================================================

#pragma mark -
#pragma mark ** AEOM behavior overrides

// ---------------------------------------------------------------------------
//		* HandleAppleEvent										[protected]
// ---------------------------------------------------------------------------
//	If we receive an Open event, just bring this window to the front.

void
GReferencedWindow::HandleAppleEvent(
	const AppleEvent&	inAppleEvent,
	AppleEvent&			outAEReply,
	AEDesc&				outResult,
	long				inAENumber)
{
	switch (inAENumber) {
		case ae_Open:						// sort of "re-opening" the window
			UDesktop::SelectDeskWindow(this);
			break;
		default:
			LWindow::HandleAppleEvent(inAppleEvent, outAEReply, outResult, inAENumber);
	}
}


// ---------------------------------------------------------------------------
//		* MakeSelfSpecifier										[protected]
// ---------------------------------------------------------------------------
//	Identify self. If the SetReference() method has been called, this
//	object will identify itself as a property of the supermodel.
//	Otherwise the default implementation from LWindow is used (references
//	by window title).

void
GReferencedWindow::MakeSelfSpecifier(
	AEDesc& inSuperSpecifier,
	AEDesc& outSelfSpecifier) const
{
	if (mHasReference) {
	
		// Make a descriptor as if this were a property.
			
		StAEDescriptor	keyData;
		DescType myType = GetModelKind();
		ThrowIfOSErr_(::AECreateDesc(typeType, (Ptr) &myType, sizeof (myType), keyData));
		ThrowIfOSErr_(::CreateObjSpecifier(cProperty, &inSuperSpecifier, formPropertyID,
									keyData, false, &outSelfSpecifier));
	}
	else
		LWindow::MakeSelfSpecifier(inSuperSpecifier, outSelfSpecifier);
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing overrides

// ---------------------------------------------------------------------------
//		* DrawSizeBox											[protected]
// ---------------------------------------------------------------------------
//	Overriden to frame the size box in gray if the window is inactive.
//	Done to match the grayscale appearance.

void
GReferencedWindow::DrawSizeBox()
{
	if (HasAttribute(windAttr_SizeBox)) {
	
			// The Toolbox trap DrawGrowIcon draws the size box in
			// the lower right corner of a Window, but it also outlines
			// the typical scroll bar areas at the right and bottom of
			// a Window. We want this routine to work for Windows that
			// don't necessarily have standard scroll bars, so we
			// temporarily change the clipping region to draw just the
			// size box.
	
		FocusDraw();
		
		Rect	sizeBox;
#if PP_Target_Carbon
		::GetWindowPortBounds(mMacWindowP, &sizeBox);
#else
		sizeBox = mMacWindowP->portRect;
#endif
		sizeBox.left = sizeBox.right - 15;
		sizeBox.top = sizeBox.bottom - 15;
		
		StClipRgnState	saveClip(sizeBox);
		if (IsActive())
			::DrawGrowIcon(mMacWindowP);
		else {
			Rect growRect = sizeBox;
			growRect.right++;
			growRect.bottom++;
			RGBColor theColor = UGAColorRamp::GetColor(10);
			::RGBForeColor(&theColor);
			::FrameRect(&growRect);
			::InsetRect(&growRect, 1, 1);
			::EraseRect(&growRect);
		}
	}
}
