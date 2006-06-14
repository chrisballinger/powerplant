// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysWindow.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysWindow.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysWindow														  [public]
/**
	Constructor */

SysWindow::SysWindow()
{
	mWindowRef = nil;
}


// ---------------------------------------------------------------------------
//	SysWindow														  [public]
/**
	Constructs from an existing WindowRef									*/

SysWindow::SysWindow(
	WindowRef	inWindowRef)
	
	: mWindowRef(inWindowRef)
{
	::RetainWindow(inWindowRef);
}


// ---------------------------------------------------------------------------
//	SysWindow														  [public]
/**
	Contructs from window creating parameters
	
	@param	inWindClass		Window class
	@param	inWindAttrs		Window attributes
	@param	inContentBounds	Bounds of content region is global coordinates	*/

SysWindow::SysWindow(
	WindowClass			inWindClass,
	WindowAttributes	inWindAttrs,
	const Rect&			inContentBounds)
{
	mWindowRef = nil;
	MakeWindow(inWindClass, inWindAttrs, inContentBounds);
}


// ---------------------------------------------------------------------------
//	~SysWindow														  [public]
/**
	Destructor */

SysWindow::~SysWindow()
{
	if (mWindowRef != nil) {
		::ReleaseWindow(mWindowRef);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Adopt															  [public]
/**
	Adopts an existing WindowRef
	
	@param	inWindowRef		WindowRef to adopt
	
	Releases current WindowRef and takes ownership of input WindowRef		*/

void
SysWindow::Adopt(
	WindowRef	inWindowRef)
{
	if (inWindowRef != mWindowRef) {
	
		if (mWindowRef != nil) {
			::ReleaseWindow(mWindowRef);
		}
		
		mWindowRef = inWindowRef;
		
		if (inWindowRef != nil) {
			::RetainWindow(inWindowRef);
		}
	}
}


// ---------------------------------------------------------------------------
//	MakeWindow														  [public]
/**
	Makes a new Toolbox window from creation parameters
	
	@param	inWindClass		Window class
	@param	inWindAttrs		Window attributes
	@param	inContentBounds	Bounds of content region is global coordinates	*/

void
SysWindow::MakeWindow(
	WindowClass			inWindClass,
	WindowAttributes	inWindAttrs,
	const Rect&			inContentBounds)
{
	if (mWindowRef != nil) {
		PPx_Throw_(LogicError, err_Logic, "System Window already exists");
	}

	OSStatus	status = ::CreateNewWindow(inWindClass, inWindAttrs,
											&inContentBounds, &mWindowRef);
											
	PPx_ThrowIfOSError_(status, "CreateNewWindow failed");
}


// ---------------------------------------------------------------------------
//	GetWindowRef													  [public]
/**
	Returns the WindowRef for this Window
	
	@returns WindowRef for this Window										*/

WindowRef
SysWindow::GetWindowRef() const
{
	return mWindowRef;
}


// ---------------------------------------------------------------------------
//	BecomeCurrentPort												  [public]
/**
	Make the Window's GrafPort the current port								*/

void
SysWindow::BecomeCurrentPort() const
{
	::SetPortWindowPort(mWindowRef);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Select															  [public]
/**
	Brings a window to the front of its layer and activates it				*/

void
SysWindow::Select()
{
	::SelectWindow(mWindowRef);
}


// ---------------------------------------------------------------------------
//	IsVisible														  [public]
/**
	Returns whether the window is visible
	
	@return Whether the window is visible									*/

bool
SysWindow::IsVisible() const
{
	return ::IsWindowVisible(mWindowRef);
}


// ---------------------------------------------------------------------------
//	Show															  [public]
/**
	Makes the window visible												*/

void
SysWindow::Show()
{
	::ShowWindow(mWindowRef);
}


// ---------------------------------------------------------------------------
//	Hide															  [public]
/**
	Makes the window invisible												*/

void
SysWindow::Hide()
{
	::HideWindow(mWindowRef);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetTitle														  [public]
/**
	Sets the title of the window
	
	@param	inTitle		Title for the window								*/

void
SysWindow::SetTitle(
	 CFStringRef inTitle)
{
	::SetWindowTitleWithCFString(mWindowRef, inTitle);
}


// ---------------------------------------------------------------------------
//	GetTitle														  [public]
/**
	Returns the title of the window
	
	@return Title of the window												*/

CFString
SysWindow::GetTitle() const
{
	CFStringRef	titleRef;
	::CopyWindowTitleAsCFString(mWindowRef, &titleRef);
	return CFString(titleRef, retain_No);
}


// ---------------------------------------------------------------------------
//	SetBounds														  [public]
/**
	Sets the bounds for the specified region of the window
	
	@param	inBounds		Bounds in global coordinates
	@param	inRegionCode	Region of window to set the bounds of			*/

void
SysWindow::SetBounds(
	Rect 				inBounds,
	WindowRegionCode 	inRegionCode)
{
	::SetWindowBounds(mWindowRef, inRegionCode, &inBounds);
}

// ---------------------------------------------------------------------------
//	GetBounds														  [public]
/**
	Returns the bounds of a specified region of the window
	
	@param	inRegionCode	Window region whose bounds to return
	
	@return	Bounds of specified window region								*/

Rect
SysWindow::GetBounds(
	WindowRegionCode	inRegionCode) const
{
	Rect windowBounds;
	::GetWindowBounds(mWindowRef, inRegionCode, &windowBounds);
	return windowBounds;
}


// ---------------------------------------------------------------------------
//	GetWindowClass													  [public]
/**
	Returns the class of the Toolbox window
	
	@return Class of the Toolbox window										*/

WindowClass
SysWindow::GetWindowClass() const
{
	WindowClass	wClass;
	OSStatus	status = ::GetWindowClass(mWindowRef, &wClass);
	
	PPx_ThrowIfOSError_(status, "GetWindowClass failed");
	
	return wClass;
}


// ---------------------------------------------------------------------------
//	GetWindowAttributes												  [public]
/**
	Returns the window attributes
	
	@return Window attributes												*/

WindowAttributes
SysWindow::GetWindowAttributes() const
{
	WindowAttributes	wAttrs;
	OSStatus	status = ::GetWindowAttributes(mWindowRef, &wAttrs);
	
	PPx_ThrowIfOSError_(status, "GetWindowAttributes failed");
	
	return wAttrs;
}


// ---------------------------------------------------------------------------
//	SetProperty														  [public]
/**
	Sets a property for the window
	
	@param	inCreator		Creator code for identifying the property
	@param	inTag			Tag for identifying the property
	@param	inSize			Size of the property data
	@param	inPropertyPtr	Pointer to buffer of property data
	
	A creator code and tag identify a property								*/

void
SysWindow::SetProperty(
	OSType			inCreator,
	OSType			inTag,
	UInt32			inSize,
	const void*		inPropertyPtr)
{
	OSStatus	status = ::SetWindowProperty( mWindowRef, inCreator, inTag,
											  inSize, inPropertyPtr );
										   
	PPx_ThrowIfOSError_(status, "SetWindowProperty failed");
}


// ---------------------------------------------------------------------------
//	GetProperty														  [public]
/**
	Gets a property for the window
	
	@param	inCreator		Creator code for identifying the property
	@param	inTag			Tag for identifying the property
	@param	inBufferSize	Size of the property data
	@param	outBuffer		Pointer to buffer for property data
	@param	outSize			Actual number of bytes retrieved				*/

OSStatus
SysWindow::GetProperty(
	OSType			inCreator,
	OSType			inTag,
	UInt32			inBufferSize,
	void*			outBuffer,
	UInt32*			outSize) const
{
	return ::GetWindowProperty( mWindowRef, inCreator, inTag,
								inBufferSize, outSize, outBuffer );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	MoveStructureTo													  [public]
/**
	Move window's structure to the specified screen location
	
	@param	inHoriz		Horizontal location
	@param	inVert		Vertical location									*/

void
SysWindow::MoveStructureTo(
	SInt16	inHoriz,
	SInt16	inVert)
{
	::MoveWindowStructure(mWindowRef, inHoriz, inVert);
}


// ---------------------------------------------------------------------------
//	SetStructureBounds												  [public]
/**
	Sets the structure bounds of the Window
	
	@param	inBounds	New bounds for the window's structure				*/

void
SysWindow::SetStructureBounds(
	const Rect&	inBounds)				// Global coordinates
{
	::SetWindowBounds(mWindowRef, kWindowStructureRgn, &inBounds);
}


// ---------------------------------------------------------------------------
//	MoveContentTo													  [public]
/**
	Moves window's content to th specified screen location
	
	@param	inHoriz		Horizontal location
	@param	inVert		Vertical location									*/

void
SysWindow::MoveContentTo(
	SInt16	inHoriz,
	SInt16	inVert)
{
	::MacMoveWindow(mWindowRef, inHoriz, inVert, false);
}


// ---------------------------------------------------------------------------
//	SetContentBounds												  [public]
/**
	Sets the content bounds of the window
	
	@param	inBounds	New bounds the window's content						*/

void
SysWindow::SetContentBounds(
	const Rect&	inBounds)				// Global coordinates
{
	::SetWindowBounds(mWindowRef, kWindowContentRgn, &inBounds);
}


// ---------------------------------------------------------------------------
//	GetScratchWindow									 	 [static] [public]
/**
	Returns WindowRef for the scratch window
	
	@return WindowRef for the scratch window

	Some Toolbox calls require a valid WindowRef in order to work properly.
	In most cases, this is a bug/limitation in the OS. For such situations,
	we use a scratch window that has an empty bounds and is invisible.		*/

WindowRef
SysWindow::GetScratchWindow()
{
	static	Rect		bounds = { 0, 0, 0, 0 };
	static	SysWindow	sScratchWindow( kDocumentWindowClass,
										kWindowNoAttributes,
										bounds );
	
	return sScratchWindow.GetWindowRef();
}

} // namespace PPx