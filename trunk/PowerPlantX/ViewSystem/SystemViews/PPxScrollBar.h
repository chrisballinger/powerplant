// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxScrollBar.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxScrollBar.h
	@brief		A system scroll bar control
*/

#ifndef H_PPxScrollBar
#define H_PPxScrollBar
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	ScrollBar
/**
	A system scroll bar control */

class	ScrollBar : public View {
public:
						ScrollBar();
						
	virtual				~ScrollBar();
	
	void				Initialize(
								View*				inSuperView,
								const HIRect&		inFrame,
								bool				inVisible,
								bool				inEnabled,
								SInt32				inInitialValue,
								SInt32				inMinValue,
								SInt32				inMaxValue,
								SInt32				inViewSize,
								bool				inHasLiveTracking);
								
	void				SetViewSize( SInt32 inViewSize );
	
	SInt32				GetViewSize() const;
	
	void				SetShowsArrowsFlag( bool inShowsArrows );
	
	bool				GetShowsArrowsFlag() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxScrollBar
