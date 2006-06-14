// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDrawerWindow.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDrawerWindow.h>
#include <PPxSerializer.h>

namespace PPx {

namespace {	// unnamed

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_PreferredEdge		= CFSTR("preferred edge");
	const CFStringRef	key_LeadingOffset		= CFSTR("leading offset");
	const CFStringRef	key_TrailingOffset		= CFSTR("trailing offset");
}


// ---------------------------------------------------------------------------
//	DrawerWindow													  [public]
/**
	Default Constructor */

DrawerWindow::DrawerWindow()
{
}


// ---------------------------------------------------------------------------
// Initialize
/**
	Initializes from parameters
	
	@param	inWindAttrs		Toolbox window attributes
	@param	inParentWindow	Window to which drawer is attached
	
	See <MacWindows.h> for information about window attributes				*/

void
DrawerWindow::Initialize(
	WindowAttributes	inWindAttrs,
	Window*				inParentWindow)
{
	Rect	bounds = { 0, 0, 1, 1};
	
	Window::Initialize(kDrawerWindowClass, inWindAttrs, bounds, nil);
	
	SetParentWindow(inParentWindow);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
DrawerWindow::ClassName() const
{
	return CFSTR("PPx::DrawerWinndow");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
DrawerWindow::InitState(
	const DataReader&	inReader)
{
	Window::InitState(inReader);
	
	OptionBits	preferredEdge	= kWindowEdgeDefault;
	float		leadingOffset	= kWindowOffsetUnchanged;
	float		trailingOffset	= kWindowOffsetUnchanged;
	
	inReader.ReadOptional(key_PreferredEdge, preferredEdge);
	inReader.ReadOptional(key_LeadingOffset, leadingOffset);
	inReader.ReadOptional(key_TrailingOffset, trailingOffset);
	
	SetPreferredEdge(preferredEdge);
	SetDrawerOffsets(leadingOffset, trailingOffset);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
DrawerWindow::WriteState(
	DataWriter&	ioWriter) const
{
	Window::WriteState(ioWriter);
	
	OptionBits	preferredEdge = GetPreferredEdge();
	
	float		leadingOffset;
	float		trailingOffset;
	GetDrawerOffsets(leadingOffset, trailingOffset);
	
	if (preferredEdge != kWindowEdgeDefault) {
		ioWriter.WriteValue(key_PreferredEdge, preferredEdge);
	}
	
	ioWriter.WriteValue(key_LeadingOffset, leadingOffset);
	ioWriter.WriteValue(key_TrailingOffset, trailingOffset);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetParentWindow
/**
	Sets the parent window for the drawer
	
	@param	inParent	Parent window for the drawer						*/

void
DrawerWindow::SetParentWindow(
	Window*		inParent)
{
	OSStatus	status = ::SetDrawerParent( GetSysWindow(),
											inParent->GetSysWindow() );
	PPx_ThrowIfOSError_(status, "SetDrawerParent failed");
}


// ---------------------------------------------------------------------------
//	GetParentWindow
/**
	Returns the parent window for the drawer
	
	@return	Parent window for the drawer									*/

Window*
DrawerWindow::GetParentWindow() const
{
	return GetWindowObject(::GetDrawerParent(GetSysWindow()));
}


// ---------------------------------------------------------------------------
//	SetPreferredEdge
/**
	Set the preferred edge from which the drawer slides
	
	@param	inPreferredEdge		Preferred edge for the drawer
	
	Values for the preferred edge are KWindowEdgeDefault, kWindowEdgeTop,
	kWindowEdgeLeft, kWindowEdgeBottom, and kWindowEdgeRight				*/

void
DrawerWindow::SetPreferredEdge(
	OptionBits	inPreferredEdge)
{
	OSStatus	status = ::SetDrawerPreferredEdge( GetSysWindow(),
												   inPreferredEdge );
	PPx_ThrowIfOSError_(status, "SetDrawerPreferredEdge failed");
}


// ---------------------------------------------------------------------------
//	GetPreferredEdge
/**
	Returns the preferred edge from which the drawer slides
	
	@return	Preferred edge for the drawer									*/

OptionBits
DrawerWindow::GetPreferredEdge() const
{
	return ::GetDrawerPreferredEdge( GetSysWindow() );
}


// ---------------------------------------------------------------------------
//	GetCurrentEdge
/**
	Returns the edge on which a drawer is or would be displayed
	
	@return	Current edge of the drawer										*/

OptionBits
DrawerWindow::GetCurrentEdge() const
{
	return ::GetDrawerCurrentEdge( GetSysWindow() );
}


// ---------------------------------------------------------------------------
//	GetDrawerState
/**
	Returns the current state of the drawer
	
	@return	Current state of the drawer
	
	The drawer state may be kWindowDrawerOpening, kWindowDrawerOpen,
	kWindowDrawerClosing, or kWindowDrawerClosed							*/

WindowDrawerState
DrawerWindow::GetDrawerState() const
{
	return ::GetDrawerState( GetSysWindow() );
}


// ---------------------------------------------------------------------------
//	SetDrawerOffsets
/**
	Sets the offsets of the drawer's size from its parent's size
	
	@param	inLeadingEdge	Offset of leading edge (top or left)
	@param	inTrailingEdge	Offset of trailing edge (bottom or right)
	
	A drawer starts with the same width or height of its parent window,
	depending on which edge it opens. The offsets specify how much to
	indent the drawer from the edges of the parent window.					*/

void
DrawerWindow::SetDrawerOffsets(
	float	inLeadingEdge,
	float	inTrailingEdge)
{
	OSStatus	status = ::SetDrawerOffsets( GetSysWindow(), inLeadingEdge,
											 inTrailingEdge );
	PPx_ThrowIfOSError_(status, "SetDrawerOffsets failed");
}


// ---------------------------------------------------------------------------
//	GetDrawerOffsets
/**
	Passes back the offsets of the drawer's size from its parent's size
	
	@param	outLeadingEdge	Offset of leading edge (top or left)
	@param	outTrailingEdge	Offset of trailing edge (bottom or right)
	
	A drawer starts with the same width or height of its parent window,
	depending on which edge it opens. The offsets specify how much to
	indent the drawer from the edges of the parent window.					*/

void
DrawerWindow::GetDrawerOffsets(
	float&	outLeadingEdge,
	float&	outTrailingEdge) const
{
	OSStatus	status = ::GetDrawerOffsets( GetSysWindow(), &outLeadingEdge,
											 &outTrailingEdge );
	PPx_ThrowIfOSError_(status, "GetDrawerOffsets failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
/**
	Toggles the drawer from open to closes, or from closed to open			*/

void
DrawerWindow::Toggle()
{
	::ToggleDrawer( GetSysWindow() );
}


// ---------------------------------------------------------------------------
/**
	Opens the drawer
	
	@param	inPreferredEdge		Preferred edge on which to open
	@param	inAsync				Whether to open the drawer async or sync
	
	With async opening, the system installs a timer to handle drawer
	opening and returns immediately. With sysn opening, the draw is fully
	opened before this function returns.
	
	Programs will have better performance using async opening and handling
	the kEventWindowDrawerOpened CarbonEvent that informs when the drawer
	is fully opened.														*/

void
DrawerWindow::OpenDrawer(
	OptionBits	inPreferredEdge,
	bool		inAsync)
{
	OSStatus	status = ::OpenDrawer( GetSysWindow(), inPreferredEdge,
									   inAsync );
	PPx_ThrowIfOSError_(status, "OpenDrawer failed");
}


// ---------------------------------------------------------------------------
//	CloseDrawer
/**
	Close the drawer
	
	@param	inAsync		Whether to close the drawer async or sync
	
	With async opening, the system installs a timer to handle drawer
	closing and returns immediately. With sysn opening, the draw is fully
	closed before this function returns.
	
	Programs will have better performance using async closing and handling
	the kEventWindowDrawerClosed CarbonEvent that informs when the drawer
	is fully closed.														*/

void
DrawerWindow::CloseDrawer(
	bool	inAsync)
{
	OSStatus	status = ::CloseDrawer( GetSysWindow(), inAsync );
	PPx_ThrowIfOSError_(status, "CloseDrawer failed");
}


} // namespace PPx
