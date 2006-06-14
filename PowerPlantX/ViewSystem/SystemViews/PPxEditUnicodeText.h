// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEditUnicodeText.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxEditUnicodeText.h
	@brief		A system edit unicode text control
*/

#ifndef H_PPxEditUnicodeText
#define H_PPxEditUnicodeText
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	EditUnicodeText
/**
	A system edit unicode text control */

class	EditUnicodeText : public View {
public:
						EditUnicodeText();
						
	virtual				~EditUnicodeText();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inText,
								bool			inIsPassword,
								const ControlFontStyleRec*	inStyle);
								
	virtual void		SetText( CFStringRef inText );
	
	virtual CFString	GetText() const;
	
	void				SetThemeFontID( ThemeFontID inFontID );
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:
	bool		mIsPassword;		/**< Whether field is for a password */
};


} // namespace PPx

#endif	// H_PPxEditUnicodeText
