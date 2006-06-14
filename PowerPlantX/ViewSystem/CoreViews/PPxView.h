// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxView.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:38 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxView.h
	@brief		Abstract base class for a visual element
*/

#ifndef H_PPxView
#define H_PPxView
#pragma once

#include <PPxEventTarget.h>
#include <PPxIdentifiable.h>
#include <PPxAttachable.h>
#include <PPxViewEvents.h>
#include <SysCGImage.h>
#include <SysHIView.h>

#include <vector>

namespace PPx {

class	FrameAdapter;

// ===========================================================================
//	View
/**
	Abstract base class for a visual element */

class	View : public EventTarget,
			   public Identifiable,
			   public Attachable,
			   public ControlBoundsChangedDoer {
public:
	virtual				~View();

	HIViewRef			GetSysView() const;
	
								// View hierarchy
	
	void				AddSubView( View* inSubView );
	
	void				RemoveFromSuperView();
	
	View*				GetSuperView() const;
	
	WindowRef			GetSysWindow() const;
	
	SInt32				SubViewCount() const;
	
	View*				GetSubViewByIndex( SInt32 inIndex ) const;
	
	View*				FindViewByID( ObjectIDT inID );

	const View*			FindConstViewByID( ObjectIDT inID ) const;
	
								// Frame
	
	void				SetFrameAdapter( FrameAdapter* inAdapter );
	
	void				SetFrame( const HIRect& inFrame );
	
	void				GetFrame( HIRect& outFrame ) const;
	
	void				GetLocalFrame( HIRect& outFrame ) const;
	
								// Display

	bool				GetNeedsDisplay() const;
								
	void				SetNeedsDisplay( bool inNeedsDisplay );
	
	void				SetNeedsDisplayInRegion(
								RgnHandle	inRegion,
								bool		inNeedsDisplay);
								
	CGImage				GetOffscreenImage( HIRect& outFrame );

								// State Information
	
	bool				IsVisible() const;
	
	void				SetVisible( bool inMakeVisible );
	
	bool				IsActive() const;
	
	void				SetActive( bool inActivate );
	
	bool				IsEnabled() const;
	
	void				SetEnabled( bool inEnable );
	
								// Values
	
	void				SetValue( SInt32 inValue );
	
	SInt32				GetValue() const;
	
	void				SetMinValue( SInt32 inMinValue );
	
	SInt32				GetMinValue() const;
	
	void				SetMaxValue( SInt32 inMaxValue );
	
	SInt32				GetMaxValue() const;
	
								// Properties
	
	void				SetTitle( CFStringRef inTitle );
	
	CFString			GetTitle() const;
	
	void				SetDataTag(
								SInt16				inPartCode,
								FourCharCode		inTag,
								Size				inDataSize,
								const void*			inDataPtr);

	OSStatus			GetDataTag(
								SInt16				inPartCode,
								FourCharCode		inTag,
								Size				inBufferSize,
								void*				inBuffer,
								Size*				outDataSize = nil) const;
								
	static View*		GetViewObject( HIViewRef inViewRef );
	
protected:
						View();
	
	void				Initialize(
								HIViewRef		inViewRef,
								const HIRect&	inFrame);
	
	void				Initialize(
								HIViewRef		inViewRef,
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled);
	
	void				RemoveSubView( View* inSubView );
	
	void				ReadViewState(
								const DataReader&	inReader,
								View*&				outSuperView,
								HIRect&				outFrame,
								bool&				outVisible,
								bool&				outEnabled);
	
	void				InitViewState(
								HIViewRef			inViewRef,
								const DataReader&	inReader);
								
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
	void				WriteViewHierarchy( DataWriter& ioWriter) const;

	void				AdoptSysView( HIViewRef inViewRef );

	virtual OSStatus	DoControlBoundsChanged(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								UInt32			inChangeAttributes,
								const HIRect&	inOriginalBounds,
								const HIRect&	inCurrentBounds);
								
	virtual void		AdaptToSuperFrameSize(
								const HIRect&	inOldSuperFrame,
								const HIRect&	inNewSuperFrame);
	
private:
	void				SetSuperView( View* inSuperView );
	
	virtual void		InitState( const DataReader& ) { }

	virtual
	EventTargetRef		SysEventTarget() const;
	
private:
	SysHIView			mSysHIView;
	View*				mSuperView;
	std::vector<View*>*	mSubViews;
	std::auto_ptr<FrameAdapter>	mFrameAdapter;
	
private:							// Copy and assignment not implemented
						View( const View& );
	View&				operator = ( const View& );
};


// -----------------------------------------------------------------------
//	Key names for persistent data common to several View subclasses

extern const CFStringRef	key_InitValue;
extern const CFStringRef	key_MinValue;
extern const CFStringRef	key_MaxValue;
extern const CFStringRef	key_ViewTitle;
extern const CFStringRef	key_AutoToggle;
extern const CFStringRef	key_IsPrimaryGroup;
extern const CFStringRef	key_TextContent;
extern const CFStringRef	key_TextJustification;
extern const CFStringRef	key_ContentType;
extern const CFStringRef	key_ContentResID;

} // namespace PPx

#endif	// H_PPxView
