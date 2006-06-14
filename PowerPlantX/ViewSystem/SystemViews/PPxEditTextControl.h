// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEditTextControl.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxEditTextControl.h
	@brief		A system edit text control
	
	@note Do not use this control. Use PPx::EditUnicodeText instead.
	This control does not work properly as an HIView in compositing mode.
	This is a system limitation noted in Apple's "Introducing HIView"
	documentation.
	
	This class is here in case future versions of Mac OS X or PPx add
	support for this control.												*/

#ifndef H_PPxEditTextControl
#define H_PPxEditTextControl
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	EditTextControl
/**
	A system edit text control.
	
	@note Do not use this control.											*/

class	EditTextControl : public View {
public:
						EditTextControl();
						
	virtual				~EditTextControl();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inText,
								bool			inIsPassword,
								bool			inUseInlineInput,
								const ControlFontStyleRec*	inStyle);
	
	void				SetText( CFStringRef inText );
	
	CFString			GetText() const;
	
	void				SetThemeFontID( ThemeFontID inFontID );

protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:
	bool		mIsPassword;		/**< Whether field is for a password */
	bool		mUseInlineInput;	/**< Whether to use inline input */
};


} // namespace PPx

#endif	// H_PPxEditTextControl
