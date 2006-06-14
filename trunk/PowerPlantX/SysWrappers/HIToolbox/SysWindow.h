// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysWindow.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysWindow.h
	@brief		Wrapper class for a Mac Toolbox Window
*/

#ifndef H_SysWindow
#define H_SysWindow
#pragma once

#include <SysCFString.h>
#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysWindow
/**
	Wrapper class for a Mac Toolbox Window */

class	SysWindow {
public:
						SysWindow();
						
						SysWindow( WindowRef inWindowRef );
	
						SysWindow(
								WindowClass			inWindClass,
								WindowAttributes	inWindAttrs,
								const Rect&			inContentBounds);
						
						~SysWindow();
	
	void				Adopt( WindowRef inWindowRef );
	
	void				MakeWindow(
								WindowClass			inWindClass,
								WindowAttributes	inWindAttrs,
								const Rect&			inContentBounds);
						
	WindowRef			GetWindowRef() const;
	
	void				BecomeCurrentPort() const;
	
	void				Select();
	
	bool				IsVisible() const;
						
	void				Show();
	
	void				Hide();
	
	void				SetTitle( CFStringRef inTitle );
	
	CFString			GetTitle() const;

	void				SetBounds(
								Rect				inBounds,
								WindowRegionCode	regionCode
												= kWindowContentRgn);

	Rect				GetBounds( WindowRegionCode regionCode
												= kWindowContentRgn ) const;
	
	WindowClass			GetWindowClass() const;
	
	WindowAttributes	GetWindowAttributes() const;
								
	void				SetProperty(
								OSType			inCreator,
								OSType			inTag,
								UInt32			inSize,
								const void*		inPropertyPtr);
								
	OSStatus			GetProperty(
								OSType			inCreator,
								OSType			inTag,
								UInt32			inBufferSize,
								void*			outBuffer,
								UInt32*			outSize = nil) const;
	
	void				MoveStructureTo(
								SInt16		inHoriz,
								SInt16		inVert);
								
	void				SetStructureBounds( const Rect& inBounds );
	
	void				MoveContentTo(
								SInt16		inHoriz,
								SInt16		inVert);
	
	void				SetContentBounds( const Rect& inBounds );
								
	static WindowRef	GetScratchWindow();

private:									// Forbid copy and assignment
						SysWindow( const SysWindow& );
	SysWindow&			operator = ( const SysWindow& );

private:
	WindowRef			mWindowRef;			/**< System window reference */
};


} // namespace PPx

#endif	// H_SysWindow
