// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxStaticText.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxStaticText.h
	@brief		A system static text control
*/

#ifndef H_PPxStaticText
#define H_PPxStaticText
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	StaticText
/**
	A system static text control */

class	StaticText : public View {
public:
						StaticText();
						
	virtual				~StaticText();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inText,
								const ControlFontStyleRec*	inStyle);
								
	void				SetText( CFStringRef inText );
	
	CFString			GetText() const;
	
	void				SetThemeFontID( ThemeFontID inFontID );

	void				SetFontStyle( const ControlFontStyleRec& inStyleRec );
	
	void				GetFontStyle( ControlFontStyleRec& outStyleRec );
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxStaticText
