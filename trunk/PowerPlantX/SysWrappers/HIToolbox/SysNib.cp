// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysNib.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysNib.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysNib															  [public]
/**
	Constructs from the name of a Nib file in the current Bundle
	
	@param	inNibName	Name of nib file									*/

SysNib::SysNib(
	CFStringRef	inNibName)
{
	OSStatus	status = ::CreateNibReference(inNibName, &mNibRef);
	
	PPx_ThrowIfOSError_(status, "CreateNibReference failed");
}


// ---------------------------------------------------------------------------
//	SysNib															  [public]
/**
	Constructs from the name of a Nib file in the specified Bundle
	
	@param	inBundle	CFBundle containing the nib file
	@param	inNibName	Name of nib file									*/

SysNib::SysNib(
	CFBundleRef	inBundle,
	CFStringRef	inNibName)
{
	OSStatus	status = ::CreateNibReferenceWithCFBundle(
									inBundle, inNibName, &mNibRef);
	
	PPx_ThrowIfOSError_(status, "CreateNibReferenceWithCFBundle failed");
}


// ---------------------------------------------------------------------------
//	~SysNib															  [public]
/**
	Destructor */

SysNib::~SysNib()
{
	::DisposeNibReference(mNibRef);
}


// ---------------------------------------------------------------------------
//	CreateWindow													  [public]
/**
	Creates a window from Nib data
	
	@param	inWindowName	Name of window instance in Nib file
	
	@return	WindowRef for the newly created window							*/

WindowRef
SysNib::CreateWindow(
	CFStringRef	inWindowName)
{
	WindowRef	window;

	OSStatus	status = ::CreateWindowFromNib(mNibRef, inWindowName, &window);
	
	PPx_ThrowIfOSError_(status, "CreateWindowFromNib failed");
	
	return window;
}


// ---------------------------------------------------------------------------
//	CreateMenu														  [public]
/**
	Creates a menu from Nib data
	
	@param	inMenuName	Name of menu instance in Nib file
	
	@return	MenuRef for the newly created menu								*/

MenuRef
SysNib::CreateMenu(
	CFStringRef	inMenuName)
{
	MenuRef		menu;

	OSStatus	status = ::CreateMenuFromNib(mNibRef, inMenuName, &menu);
	
	PPx_ThrowIfOSError_(status, "CreateMenuFromNib failed");
	
	return menu;
}


// ---------------------------------------------------------------------------
//	CreateMenuBar													  [public]
/**
	Creates a menu bar from Nib data
	
	@param	inMenuBarName	Name of menu bar instance in Nib file
	
	@return	Handle to the newly created menu bar							*/

Handle
SysNib::CreateMenuBar(
	CFStringRef	inMenuBarName)
{
	Handle		menuBar;

	OSStatus	status = ::CreateMenuBarFromNib(
								mNibRef, inMenuBarName, &menuBar);
	
	PPx_ThrowIfOSError_(status, "CreateMenuBarFromNib failed");
	
	return menuBar;
}


// ---------------------------------------------------------------------------
//	SetMenuBar														  [public]
/**
	Sets the current menu bar from Nib data
	
	@param	inMenuBarName	Name of menu bar instance in Nib file			*/

void
SysNib::SetMenuBar(
	CFStringRef	inMenuBarName)
{
	OSStatus	status = ::SetMenuBarFromNib(mNibRef, inMenuBarName);
	
	PPx_ThrowIfOSError_(status, "SetMenuBarFromNib failed");
}


} // namespace PPx