// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysHIView.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:36 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysHIView.h
	@brief		Wrapper class for a Mac Toolbox HIView
*/

#ifndef H_SysHIView
#define H_SysHIView
#pragma once

#include <SysCFString.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysHIView
/**
	Wrapper class for a Mac Toolbox HIView */

class	SysHIView {
public:
						SysHIView();
					
						SysHIView( HIViewRef inViewRef );
					
						~SysHIView();
						
	void				Adopt( HIViewRef inViewRef );

	EventTargetRef		GetSysEventTarget() const;
	
	HIViewRef			GetSysView() const;
	
	void				AddSubView( HIViewRef inSubview );
	
	void				RemoveFromSuperView();
	
	HIViewRef			GetSuperView() const;
	
	bool				IsVisible() const;
	
	void				SetVisible( bool inMakeVisible );
	
	bool				IsActive() const;
	
	void				SetActive( bool inActivate );
	
	bool				IsEnabled() const;
	
	void				SetEnabled( bool inEnable );
	
	void				SetValue( SInt32 inValue );
	
	SInt32				GetValue() const;
	
	void				SetMinValue( SInt32 inMinValue );
	
	SInt32				GetMinValue() const;
	
	void				SetMaxValue( SInt32 inMaxValue );
	
	SInt32				GetMaxValue() const;
	
	void				SetViewSize( SInt32 inViewSize );
	
	SInt32				GetViewSize() const;
	
	void				SetTitle( CFStringRef inTitle );
	
	CFString			GetTitle() const;
	
	void				SetCommandID( UInt32 inCommandID );
	
	UInt32				GetCommandID() const;
	
	void				SetDataTag(
								SInt16			inPartCode,
								FourCharCode	inTag,
								Size			inDataSize,
								const void*		inDataPtr);

	OSStatus			GetDataTag(
								SInt16			inPartCode,
								FourCharCode	inTag,
								Size			inBufferSize,
								void*			inBuffer,
								Size*			outDataSize = nil) const;
								
	void				SetProperty(
								OSType			inCreator,
								OSType			inTag,
								UInt32			inSize,
								const void*		inPropertyPtr);
								
	OSStatus			GetProperty(
								OSType			inCreator,
								OSType			inTag,
								UInt32			inBufferSize,
								void*			inBuffer,
								UInt32*			outSize = nil) const;

	void				GetBounds( HIRect& outBounds ) const;

	void				GetFrame( HIRect& outFrame ) const;
	
	void				SetFrame( const HIRect& inFrame );
	
	void				MoveFrameBy(
								float	inDeltaX,
								float	inDeltaY);
	
	void				PlaceFrameAt(
								float	inLeftX,
								float	inTopY);
								
	bool				GetNeedsDisplay() const;
								
	void				SetNeedsDisplay( bool inNeedsDisplay );
	
	void				SetNeedsDisplayInRegion(
								RgnHandle	inRegion,
								bool		inNeedsDisplay);
								
	void				CreateOffscreenImage(
								HIRect&			outFrame,
								CGImageRef&		outImage);
								
	// -----------------------------------------------------------------------
	//	Static class interface

	static void			RegisterSysViewClass(
								CFStringRef		inClassID,
								CFStringRef		inBaseClassID);
								
	static HIViewRef	CreateSysView(
								CFStringRef		inClassID,
								OptionBits		inFeatures);
								
private:								// Forbid copy and assignment
						SysHIView( const SysHIView& );
	SysHIView&			operator = ( const SysHIView& );

private:
	HIViewRef		mHIViewRef;			/**< System HIView reference */
};


} // namespace PPx

#endif	// H_SysHIView
