// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMLTEView.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxMLTEView.h
	@brief		Text editing view based on MLTE
*/

//	$$$ Incomplete $$$	Class is still under construction

#ifndef H_PPxMLTEView
#define H_PPxMLTEView
#pragma once

#include <PPxBaseView.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	MLTEView
/**
	Text edit view base on MLTE
	
	@note Still under construction. Only displays text.						*/

class	MLTEView : public BaseView,
				   public ControlDrawDoer {
public:
						MLTEView();
						
	virtual				~MLTEView();
	
	void				Initialize(
								View*				inSuperView,
								const HIRect&		inFrame,
								bool				inVisible,
								bool				inEnabled,
								TXNFrameOptions		inFrameOptions,
								TXNFrameType		inFrameType,
								TXNFileType			inFileType,
								TXNPermanentTextEncodingType	inEncodingType);
								
	TXNObject			GetTextObject() const;
	
	void				SetText( CFStringRef inText );
	
	CFString			GetText() const;
	
	void				SetOneControlTag(
								TXNControlTag		inTag,
								TXNControlData		inData);
								
	void				SetCGContext( CGContextRef inCGContext );
	
	void				SetOneTypeAttribute(
								TXNTypeAttributes*	inAttribute,
								TXNOffset			inStartOffset,
								TXNOffset			inEndOffset);
								
	void				SetOneTypeAttributeForSelection( TXNTypeAttributes* inAttribute );
								
	void				SetFontName( ConstStringPtr inName );
	
	void				SetFontSize( Fixed inSize );
	
	void				SetFontStyle( Style inStyle );
	
	void				SetFontColor( const RGBColor& inColor );
	
	static void			InitializeSystem(
								TXNInitOptions							inOptions = 0,
								const TXNMacOSPreferredFontDescription*	inFonts = nil,
								ItemCount								inFontCount = 0);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;

	virtual OSStatus	DoControlDraw(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								ControlPartCode		inPartCode,
								RgnHandle			inClipRgn,
								CGContextRef		inContext);
								
private:
	virtual CFStringRef	ClassName() const;
	
	virtual void		FinishInit();

private:
	TXNObject			mTXNObject;
	TXNFrameID			mTXNFrameID;
	TXNFrameOptions		mFrameOptions;
	TXNFrameType		mFrameType;
	TXNFileType			mFileType;
	TXNPermanentTextEncodingType
						mEncodingType;
};


} // namespace PPx

#endif	// H_PPxMLTEView
