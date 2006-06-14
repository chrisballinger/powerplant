// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTextGroupBox.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxTextGroupBox.h
	@brief		A system group box with a text title
*/

#ifndef H_PPxTextGroupBox
#define H_PPxTextGroupBox
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	TextGroupBox
/**
	A system group box with a text title */

class	TextGroupBox : public View {
public:
						TextGroupBox();
						
	virtual				~TextGroupBox();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inTitle,
								bool			inIsPrimary);
	
	void				SetThemeFontID( ThemeFontID inFontID );

	void				GetTitleRect( Rect& outTitleRect ) const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	bool		mIsPrimaryGroup;	/**< Primary (true) or Secondary (false) group */
};


} // namespace PPx

#endif	// H_PPxTextGroupBox
