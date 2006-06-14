// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTabView.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxTabView.h
	@brief		A system tab view
*/

#ifndef H_PPxTabView
#define H_PPxTabView
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	TabView
/**
	A system tab view */

class	TabView : public View {
public:
						TabView();
						
	virtual				~TabView();
	
	void				Initialize(
								View*					inSuperView,
								const HIRect&			inFrame,
								bool					inVisible,
								bool					inEnabled,
								ControlTabSize			inTabSize,
								ControlTabDirection		inTabDirection,
								UInt16					inTabCount,
								const ControlTabEntry*	inTabEntries);
	
	void				SetThemeFontID( ThemeFontID inFontID );
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	ControlTabSize		mTabSize;
	ControlTabDirection	mTabDirection;
};


} // namespace PPx

#endif	// H_PPxTabView
