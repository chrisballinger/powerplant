// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxThemeTextBox.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxThemeTextBox.h
	@brief		View for drawing text using a theme font inside a bounding box
*/

#ifndef H_PPxThemeTextBox
#define H_PPxThemeTextBox
#pragma once

#include <PPxBaseView.h>

namespace PPx {

// ===========================================================================
//	ThemeTextBox
/**
	View for drawing text using a theme font inside a bounding box */

class	ThemeTextBox : public BaseView,
					   public ControlDrawDoer {
public:
						ThemeTextBox();
						
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inText,
								ThemeFontID		inThemeFontID = kThemeSystemFont,
								SInt16			inJustification = teFlushDefault,
								bool			inOneLineOnly = false);
								
	void				SetText( CFStringRef inText );
	
	CFString			GetText() const;
	
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
	CFString			mText;				/**< Text to display */
	ThemeFontID			mThemeFontID;		/**< Theme font */
	SInt16				mJustification;		/**< Text justification */
	bool				mOneLineOnly;		/**< Put all text on one line? */
};


} // namespace PPx

#endif	// H_PPxThemeTextBox
