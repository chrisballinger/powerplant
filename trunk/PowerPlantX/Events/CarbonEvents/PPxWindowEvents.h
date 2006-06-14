// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:17 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxWindowEvents.h
	@brief		Event handlers for window Carbon Events
*/

#ifndef H_PPxWindowEvents
#define H_PPxWindowEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Handles a low-level window update event */

class	WindowUpdateDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowUpdate> {
protected:
	virtual OSStatus	DoWindowUpdate(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles drawing the contents of a window */

class	WindowDrawContentDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDrawContent> {
protected:
	virtual OSStatus	DoWindowDrawContent(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window being activated */

class	WindowActivatedDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowActivated> {
protected:
	virtual OSStatus	DoWindowActivated(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window being deactivated */

class	WindowDeactivatedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDeactivated> {
protected:
	virtual OSStatus	DoWindowDeactivated(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window being activated by a mouse click */

class	WindowGetClickActivationDoer : public SpecificEventDoer<
												kEventClassWindow,
												kEventWindowGetClickActivation> {
protected:
	virtual OSStatus	DoWindowGetClickActivation(
								SysCarbonEvent&			ioEvent,
								WindowRef				inWindow,
								const HIPoint&			inMouseLocation,
								UInt32					inKeyModifiers,
								WindowPartCode			inWindowPart,
								ControlRef				inControlHit,
								ClickActivationResult	&outResult) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window being shown */

class	WindowShowingDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowShowing> {
protected:
	virtual OSStatus	DoWindowShowing(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window being hidden */

class	WindowHidingDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowHiding> {
protected:
	virtual OSStatus	DoWindowHiding(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window after being shown */

class	WindowShownDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowShown> {
protected:
	virtual OSStatus	DoWindowShown(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window after being hidden */

class	WindowHiddenDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowHidden> {
protected:
	virtual OSStatus	DoWindowHidden(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to collapse a window */

class	WindowCollapseDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowCollapse> {
protected:
	virtual OSStatus	DoWindowCollapse(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to collapse all windows */

class	WindowCollapseAllDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowCollapseAll> {
protected:
	virtual OSStatus	DoWindowCollapseAll(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window about to be collapsed */

class	WindowCollapsingDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowCollapsing> {
protected:
	virtual OSStatus	DoWindowCollapsing(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window after being collapsed */

class	WindowCollapsedDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowCollapsed> {
protected:
	virtual OSStatus	DoWindowCollapsed(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to expand a window */

class	WindowExpandDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowExpand> {
protected:
	virtual OSStatus	DoWindowExpand(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to expand all windows */

class	WindowExpandAllDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowExpandAll> {
protected:
	virtual OSStatus	DoWindowExpandAll(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window about to be expanded */

class	WindowExpandingDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowExpanding> {
protected:
	virtual OSStatus	DoWindowExpanding(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window after being expanded */

class	WindowExpandedDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowExpanded> {
protected:
	virtual OSStatus	DoWindowExpanded(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to zoom a window */

class	WindowZoomDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowZoom> {
protected:
	virtual OSStatus	DoWindowZoom(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to zoom all windows */

class	WindowZoomAllDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowZoomAll> {
protected:
	virtual OSStatus	DoWindowZoomAll(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window after being zoomed */

class	WindowZoomedDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowZoomed> {
protected:
	virtual OSStatus	DoWindowZoomed(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window being moved and/or resized */

class	WindowBoundsChangingDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowBoundsChanging> {
protected:
	virtual OSStatus	DoWindowBoundsChanging(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								UInt32			inAttributes,
								const Rect&		inOriginalBounds,
								const Rect&		inPreviousBounds,
								Rect&			ioCurrentBounds) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window having been moved and/or resized */

class	WindowBoundsChangedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowBoundsChanged> {
protected:
	virtual OSStatus	DoWindowBoundsChanged(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								UInt32			inAttributes,
								const Rect&		inOriginalBounds,
								const Rect&		inPreviousBounds,
								const Rect&		inCurrentBounds) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window starting to be resized */

class	WindowResizeStartedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowResizeStarted> {
protected:
	virtual OSStatus	DoWindowResizeStarted(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window finishing being resized */

class	WindowResizeCompletedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowResizeCompleted> {
protected:
	virtual OSStatus	DoWindowResizeCompleted(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window starting to be dragged */

class	WindowDragStartedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDragStarted> {
protected:
	virtual OSStatus	DoWindowDragStarted(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window finishing being dragged */

class	WindowDragCompletedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDragCompleted> {
protected:
	virtual OSStatus	DoWindowDragCompleted(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to close a window */

class	WindowCloseDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowClose> {
protected:
	virtual OSStatus	DoWindowClose(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to close all windows */

class	WindowCloseAllDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowCloseAll> {
protected:
	virtual OSStatus	DoWindowCloseAll(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window about to be disposed */

class	WindowClosedDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowClosed> {
protected:
	virtual OSStatus	DoWindowClosed(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles changing the cursor when the mouse is inside a window */

class	WindowCursorChangeDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowCursorChange> {
protected:
	virtual OSStatus	DoWindowCursorChange(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								Point			inMouseLocation,
								UInt32			inKeyModifiers) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a click in a window intended to invoke a contextual menu */

class	WindowContextualMenuSelectDoer : public SpecificEventDoer<
													kEventClassWindow,
													kEventWindowContextualMenuSelect> {
protected:
	virtual OSStatus	DoWindowContextualMenuSelect(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a request to select from the window path popup menu */

class	WindowPathSelectDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowPathSelect> {
protected:
	virtual OSStatus	DoWindowPathSelect(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the ideal size of a window's content region */

class	WindowGetIdealSizeDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowGetIdealSize> {
protected:
	virtual OSStatus	DoWindowGetIdealSize(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								Point&			outIdealSize) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the minimum size of a window's content region */

class	WindowGetMinimumSizeDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowGetMinimumSize> {
protected:
	virtual OSStatus	DoWindowGetMinimumSize(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								Point&			outMinimumSize) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the maximum size of a window's content region */

class	WindowGetMaximumSizeDoer : public SpecificEventDoer<
												kEventClassWindow,
												kEventWindowGetMaximumSize> {
protected:
	virtual OSStatus	DoWindowGetMaximumSize(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								Point&			outMaximumSize) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles notification that the available window area has changed */

class	WindowConstrainDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowConstrain> {
protected:
	virtual OSStatus	DoWindowConstrain(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a click in a window. The click is not a contextual menu click
	and is not within any subview.											*/

class	WindowHandleContentClickDoer : public SpecificEventDoer<
												kEventClassWindow,
												kEventWindowHandleContentClick> {
protected:
	virtual OSStatus	DoWindowHandleContentClick(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window acquiring the focus */

class	WindowFocusAcquiredDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowFocusAcquired> {
protected:
	virtual OSStatus	DoWindowFocusAcquired(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a window relinquishing the focus */

class	WindowFocusRelinquishDoer : public SpecificEventDoer<
												kEventClassWindow,
												kEventWindowFocusRelinquish> {
protected:
	virtual OSStatus	DoWindowFocusRelinquish(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a setting the focus to the main view of a window */

class	WindowFocusContentDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowFocusContent> {
protected:
	virtual OSStatus	DoWindowFocusContent(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a setting the focus to the toolbar of a window */

class	WindowFocusToolbarDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowFocusContent> {
protected:
	virtual OSStatus	DoWindowFocusToolbar(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drawer starting to open */

class	WindowDrawerOpeningDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDrawerOpening> {
protected:
	virtual OSStatus	DoWindowDrawerOpening(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drawer being fully open */

class	WindowDrawerOpenedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDrawerOpened> {
protected:
	virtual OSStatus	DoWindowDrawerOpened(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drawer starting to close */

class	WindowDrawerClosingDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDrawerClosing> {
protected:
	virtual OSStatus	DoWindowDrawerClosing(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles a drawer being fully closed */

class	WindowDrawerClosedDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDrawerClosed> {
protected:
	virtual OSStatus	DoWindowDrawerClosed(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxWindowEvents
