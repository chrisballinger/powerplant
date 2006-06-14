// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindow.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:49 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxWindow.h
	@brief		Window for displaying data on screen
*/

#ifndef H_PPxWindow
#define H_PPxWindow
#pragma once

#include <PPxEventTarget.h>
#include <PPxAttachable.h>
#include <PPxWindowEvents.h>
#include <SysWindow.h>

#include <memory>

namespace PPx {

class	View;
class	WindowContentView;

// ===========================================================================
//	Window
/**
	Window for displaying data on screen */

class	Window : public EventTarget,
				 public Attachable,
				 public WindowCloseDoer {
public:
						Window();
						
	virtual				~Window();
	
	void				Initialize(
								WindowClass			inWindowClass,
								WindowAttributes	inWindAttrs,
								const Rect&			inContentBounds,
								CFStringRef			inTitle);
	
	WindowRef			GetSysWindow() const;
	
	View*				GetContentView() const;
	
	void				AddSubView( View* inSubView );
	
	void				Select();
	
	bool				IsVisible() const;
	
	void				Show();
	
	void				Hide();
	
	void				SetTitle( CFStringRef inTitle );
	
	CFString			GetTitle() const;
	
	void				Close();
	
							// Static interface
	
	static Window*		GetWindowObject( WindowRef inWindowRef );
	
	static void			SetDefaultAttributes( WindowAttributes inWindAttrs );
	
	static WindowAttributes
						GetDefaultAttributes();

protected:
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;

	virtual OSStatus	DoWindowClose(
								SysCarbonEvent&	ioEvent,
								WindowRef		inWindow);
private:
	virtual CFStringRef	ClassName() const;
	
	virtual
	EventTargetRef		SysEventTarget() const;
	
											// Forbid copy and assignment
						Window( const Window& );
	Window&				operator = ( const Window& );
	
private:
	SysWindow				mSysWindow;			/**< System window reference */
	std::auto_ptr<WindowContentView>
							mContentView;		/**< Top of view hierarchy */
};


} // namespace PPx

#endif	// H_PPxWindow
