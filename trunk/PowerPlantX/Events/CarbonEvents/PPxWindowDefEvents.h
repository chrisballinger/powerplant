// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowDefEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:16 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxWindowDefEvents.h
	@brief		Event handlers for window definition Carbon Events */

#ifndef H_PPxWindowDefEvents
#define H_PPxWindowDefEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Draws a window's structure */

class	WindowDrawFrameDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowDrawFrame> {
protected:
	virtual OSStatus	DoWindowDrawFrame(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Draws a specific part of a window's structure */

class	WindowDrawPartDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowDrawPart> {
protected:
	virtual OSStatus	DoWindowDrawPart(
								SysCarbonEvent&		ioEvent,
								WindowRef			inWindow,
								WindowDefPartCode	inPart) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns a specified region of a window */

class	WindowGetRegionDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowGetRegion> {
protected:
	virtual OSStatus	DoWindowGetRegion(
								SysCarbonEvent&		ioEvent,
								WindowRef			inWindow,
								WindowRegionCode	inRegionCode,
								RgnHandle			ioRegion) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the window part hit by a specified mouse location */

class	WindowHitTestDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowHitTest> {
protected:
	virtual OSStatus	DoWindowHitTest(
								SysCarbonEvent&		ioEvent,
								WindowRef			inWindow,
								const Point&		inGlobalPoint,
								WindowDefPartCode&	outPartHit) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Initializes a window definition */

class	WindowInitDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowInit> {
protected:
	virtual OSStatus	DoWindowInit(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								UInt32&			outFeatures) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Disposes a window definition */

class	WindowDisposeDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowDispose> {
protected:
	virtual OSStatus	DoWindowDispose(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles drag hiliting for a window */

class	WindowDragHiliteDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowDragHilite> {
protected:
	virtual OSStatus	DoWindowDragHilite(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								bool			inDrawHilite) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles change in modified state of a window */

class	WindowModifiedDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowModified> {
protected:
	virtual OSStatus	DoWindowModified(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								bool			inIsModified) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles creating a drag image for a window's proxy icon */

class	WindowSetupProxyDragImageDoer : public SpecificEventDoer<
													kEventClassWindow,
													kEventWindowSetupProxyDragImage> {
protected:
	virtual OSStatus	DoWindowSetupProxyDragImage(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								RgnHandle		ioImageClipRgn,
								RgnHandle		ioDragOutline,
								GWorldPtr&		outImage) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Handles change in window state */

class	WindowStateChangedDoer : public SpecificEventDoer<
										kEventClassWindow,
										kEventWindowStateChanged> {
protected:
	virtual OSStatus	DoWindowStateChanged(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								UInt32			inStateFlags) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the width of a window's title area */

class	WindowMeasureTitleDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowMeasureTitle> {
protected:
	virtual OSStatus	DoWindowMeasureTitle(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								SInt16&			outFullWidth,
								SInt16&			outTextWidth) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Draws a window's grow box */

class	WindowDrawGrowBoxDoer : public SpecificEventDoer<
											kEventClassWindow,
											kEventWindowDrawGrowBox> {
protected:
	virtual OSStatus	DoWindowDrawGrowBox(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Returns the outline for a window being resized */

class	WindowGetGrowImageRegionDoer : public SpecificEventDoer<
												kEventClassWindow,
												kEventWindowGetGrowImageRegion> {
protected:
	virtual OSStatus	DoWindowGetGrowImageRegion(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow,
								const Rect&		inGlobalBounds,
								RgnHandle		ioImageRgn) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Paints a window */

class	WindowPaintDoer : public SpecificEventDoer<
									kEventClassWindow,
									kEventWindowPaint> {
protected:
	virtual OSStatus	DoWindowPaint(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


} // namespace PPx

#endif	// H_PPxWindowDefEvents
