// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibDecoder.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:22 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#ifndef H_PPxNibDecoder
#define H_PPxNibDecoder
#pragma once

#include <PPxNibReader.h>
#include <PPxNibConstants.h>
#include <PPxNibDecoderUtils.h>
#include <PPxWindow.h>
#include <PPxXMLDecoder.h>
#include <SysCFString.h>
#include <SysCFXMLNode.h>
#include <SysCFXMLTree.h>

namespace PPx {

namespace NibDecoder {

	Window*				CreateWindowFromNib(
								CFStringRef	inNibName,
								CFStringRef	inWindowName);

	template <typename TWindow>
	TWindow*
						CreateWindowFromNib(
								CFStringRef	inNibName,
								CFStringRef	inWindowName);

	void				GetWindowTreeData(
							CFStringRef	inNibName,
							CFStringRef	inWindowName,
							CFXMLTree&	outWindowTree,
							CFXMLTree&	outRootControlTree);

// ---------------------------------------------------------------------------
//	CreateWindowFromNib<TWindow>
/**
	This is the template version of CreateWindowFromNib() that will create a
	new window of type TWindow.  TWindow should be subclassed from PPx::Window.
	
	@param	inNibName		Name of the nib file (without the .nib extension)
							to load the window from
	@param	inWindowName	Name of the window object's name as created in
							Interface Builder
	
	@return	Pointer to newly created TWindow								*/

template <typename TWindow>
TWindow*
CreateWindowFromNib(
	CFStringRef	inNibName,
	CFStringRef	inWindowName)
{
	CFXMLTree windowTree;
	CFXMLTree rootControlTree;

	GetWindowTreeData(inNibName, inWindowName, windowTree, rootControlTree);

	std::auto_ptr<TWindow> window;

	// create the window
	OSStatus status;
	NibReader windowReader(windowTree);
	
	WindowClass windowClass = kDocumentWindowClass;
	windowReader.ReadOptionalInt(CFSTR("carbonWindowClass"), windowClass);

	CFString title("");
	windowReader.ReadOptionalString(NibConstants::tag_title, title);

	Rect bounds;
	CFString boundsString;
	windowReader.ReadRequiredString(CFSTR("windowRect"), boundsString);
	NibDecoderUtils::SetQDRectFromString(boundsString, bounds);

	WindowAttributes attributes = 0;

	bool receiveActivates = true;
	windowReader.ReadOptionalBool(CFSTR("receiveActivates"), receiveActivates);
	if (not receiveActivates)
		attributes |= kWindowNoActivatesAttribute;
	
	bool receiveUpdates = true;
	windowReader.ReadOptionalBool(CFSTR("receiveUpdates"), receiveUpdates);
	if (not receiveUpdates)
		attributes |= kWindowNoUpdatesAttribute;
	
	bool hasCollapseBox = true;
	windowReader.ReadOptionalBool(CFSTR("hasCollapseBox"), hasCollapseBox);
	if (hasCollapseBox)
		attributes |= kWindowCollapseBoxAttribute;
	
	bool hasHZoom = true;
	windowReader.ReadOptionalBool(CFSTR("hasHorizontalZoom"), hasHZoom);
	if (hasHZoom)
		attributes |= kWindowHorizontalZoomAttribute;
	
	bool hasVZoom = true;
	windowReader.ReadOptionalBool(CFSTR("hasVerticalZoom"), hasVZoom);
	if (hasVZoom)
		attributes |= kWindowVerticalZoomAttribute;
	
	bool isConstrained = true;
	windowReader.ReadOptionalBool(CFSTR("isConstrained"), isConstrained);
	if (not isConstrained)
		attributes |= kWindowNoConstrainAttribute;
	
	bool isResizable = true;
	windowReader.ReadOptionalBool(CFSTR("isResizable"), isResizable);
	if (isResizable)
		attributes |= kWindowResizableAttribute;
	
	bool standardHandler = true;
	windowReader.ReadOptionalBool(CFSTR("standardHandler"), standardHandler);
	if (standardHandler)
		attributes |= kWindowStandardHandlerAttribute;
	
	bool hasCloseBox = true;
	windowReader.ReadOptionalBool(CFSTR("hasCloseBox"), hasCloseBox);
	if (hasCloseBox)
		attributes |= kWindowCloseBoxAttribute;
	
	bool liveResize = false;
	windowReader.ReadOptionalBool(CFSTR("liveResize"), liveResize);
	if (liveResize)
		attributes |= kWindowLiveResizeAttribute;
	
	bool hasSideTitle = false;
	windowReader.ReadOptionalBool(CFSTR("hasSideTitle"), hasSideTitle);
	if (hasSideTitle)
		attributes |= kWindowSideTitlebarAttribute;
	
	bool isMetal = false;
	windowReader.ReadOptionalBool(CFSTR("metal"), isMetal);
	if (isMetal)
		attributes |= kWindowMetalAttribute;
	
	bool isCompositing = false;
	windowReader.ReadOptionalBool(CFSTR("compositing"), isCompositing);
	if (isCompositing)
		attributes |= kWindowCompositingAttribute;
	else
		PPx_Throw_(PPx::DataError, PPx::err_XMLFormat, 
			"The window must use kWindowCompositingAttribute");
	
	bool hasToolbarButton = false;
	windowReader.ReadOptionalBool(CFSTR("toolbarButton"), hasToolbarButton);
	if (hasToolbarButton)
		attributes |= kWindowToolbarButtonAttribute;
	
	// ### We need to rethink the SheetWindow/DrawerWindow stuff now that we
	// have a template version of CreateWindowFromNib().
	
	// ### It would be nice to also create SheetWindow or DrawerWindow here
/*	if ((windowClass == kSheetWindowClass) or (windowClass == kSheetAlertWindowClass))
	{
		std::auto_ptr<SheetWindow> sheetWindow(new SheetWindow);
		sheetWindow->Initialize(attributes, bounds, nil);
		window.reset(sheetWindow.release());
	}
	// ### PPx::DrawerWindow needs a pointer to an existing parent window.  We
	    might need to change that to allow a nil pointer.
	else if (windowClass == kDrawerWindowClass)
	{
		std::auto_ptr<DrawerWindow> drawerWindow(new DrawerWindow);
		drawerWindow->Initialize(attributes, nil);
		window.reset(drawerWindow.release());
	}
	else*/
	{
		window.reset(new TWindow);
		window->Initialize(windowClass, attributes, bounds, title);
	}

	SInt32 windowPosition = 0;
	if (windowReader.ReadOptionalInt(CFSTR("windowPosition"), windowPosition))
	{
		WindowRef parentWindow = NULL;
		if ((windowPosition == kWindowCenterOnParentWindow) or
			(windowPosition == kWindowCenterOnParentWindowScreen) or
			(windowPosition == kWindowCascadeOnParentWindow) or
			(windowPosition == kWindowCascadeOnParentWindowScreen) or
			(windowPosition == kWindowCascadeStartAtParentWindowScreen) or
			(windowPosition == kWindowAlertPositionOnParentWindow) or
			(windowPosition == kWindowAlertPositionOnParentWindowScreen))
		{
			parentWindow = ::GetNextWindowOfClass(window->GetSysWindow(), kAllWindowClasses, true);
		}
		status = ::RepositionWindow(window->GetSysWindow(), parentWindow, windowPosition);
		PPx_ThrowIfOSError_(status, "RepositionWindow failed");
	}

	SInt32 themeBrush = 0;
	if (windowReader.ReadOptionalInt(CFSTR("themeBrush"), themeBrush) and
		not (attributes & kWindowMetalAttribute))
	{
		status = ::SetThemeWindowBackground(window->GetSysWindow(), themeBrush, true);
		PPx_ThrowIfOSError_(status, "SetThemeWindowBackground failed");
	}
	else if (not (attributes & kWindowMetalAttribute))
	{
		// Determine the default theme brush based on the window class
		switch (windowClass)
		{
			case kAlertWindowClass:
			case kMovableAlertWindowClass:
				themeBrush = kThemeBrushAlertBackgroundActive;
				break;
			case kModalWindowClass:
				// ### IB does not allow for Modal windows with
				// Compositing turned on so this will not look correct.
				themeBrush = kThemeBrushDialogBackgroundActive;
				break;
			case kMovableModalWindowClass:
				themeBrush = kThemeBrushMovableModalBackground;
				break;
			case kFloatingWindowClass:
				// IB does not appear to make any changes for floating windows.
				break;
			case kUtilityWindowClass:
				themeBrush = kThemeBrushUtilityWindowBackgroundActive;
				break;
			case kSheetWindowClass:
			case kSheetAlertWindowClass:
				themeBrush = kThemeBrushSheetBackgroundTransparent;
				break;
			case kDrawerWindowClass:
				themeBrush = kThemeBrushDrawerBackground;
				break;
		}
		
		if (themeBrush != 0)
		{
			status = ::SetThemeWindowBackground(window->GetSysWindow(), themeBrush, true);
			PPx_ThrowIfOSError_(status, "SetThemeWindowBackground failed");
		}
	}

	if (rootControlTree.IsValid())
	{
		NibReader rootControlReader(rootControlTree);
		
		rootControlReader.ReadOptionalSubViews(window->GetContentView());
	}

	window->FinishInitPersistent();

	return window.release();
}

} // namespace NibDecoder

} // namespace PPx

#endif	// H_PPxNibDecoder

