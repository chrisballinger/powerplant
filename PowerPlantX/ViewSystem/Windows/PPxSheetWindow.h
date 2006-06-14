// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSheetWindow.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSheetWindow.h
	@brief		Classes for a sheet window and a sheet alert
*/

#ifndef H_PPxSheetWindow
#define H_PPxSheetWindow
#pragma once

#include <PPxCorrespondent.h>
#include <PPxCommandEvents.h>
#include <PPxWindow.h>

namespace PPx {

// ===========================================================================
//	SheetWindow
/**
	A window displayed as a sheet in a parent window */

class	SheetWindow : public Window {
public:
						SheetWindow();
						
	void				Initialize(
								WindowAttributes	inWindAttrs,
								const Rect&			inBounds,
								Window*				inParent);
	
	void				Show( Window* inParent );
	
	void				Hide();
	
	WindowRef			GetParentWindow() const;
	
private:
	virtual CFStringRef	ClassName() const;
	
											// Forbid copy and assignment
						SheetWindow( const SheetWindow& );
	SheetWindow&		operator = ( const SheetWindow& );
};


// ===========================================================================
//	SheetAlert
/**
	An alert displayed as a sheet in a parent window */

class	SheetAlert : public Correspondent,
					 public CommandProcessDoer {
public:
						SheetAlert();
						
	virtual				~SheetAlert();
	
	void				Initialize(
								CFStringRef		inErrorMessage,
								CFStringRef		inExplanation = nil,
								AlertType		inAlertType = kAlertNoteAlert,
								const AlertStdCFStringAlertParamRec* inParams = nil);
								
	void				Initialize(
								CFStringRef		inErrorMessage,
								CFStringRef		inExplanation,
								AlertType		inAlertType,
								CFStringRef		inOKLabel,
								CFStringRef		inCancelLabel,
								CFStringRef		inOtherLabel);
	
	void				Show(	WindowRef		inParent,
								EventTargetRef	inNotifyTarget = nil,
								CommandIDT		inOKCommand = 0,
								CommandIDT		inCancelCommand = 0,
								CommandIDT		inOtherCommand = 0);
								
	void				Close();
	
protected:
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
	virtual OSStatus	DoCommandProcess(
								SysCarbonEvent&	ioEvent,
								HICommand		inCommand,
								UInt32			inKeyModifiers,
								UInt32			inMenuContext);

private:
	virtual CFStringRef	ClassName() const;
	
											// Forbid copy and assignment
						SheetAlert( const SheetAlert& );
	SheetAlert&			operator = ( const SheetAlert& );
								
private:
	DialogRef		mSysDialogRef;
	EventTargetRef	mNotifyTarget;
	CommandIDT		mOKCommand;
	CommandIDT		mCancelCommand;
	CommandIDT		mOtherCommand;
};


} // namespace PPx

#endif // H_PPxSheetWindow