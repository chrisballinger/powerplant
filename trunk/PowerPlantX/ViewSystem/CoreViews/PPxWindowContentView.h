// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowContentView.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:38 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxWindowContentView.h
	@brief		Top-level view for the contents of a window
*/

#ifndef H_PPxWindowContentView
#define H_PPxWindowContentView
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	WindowContentView
/**
	Top-level view for the contents of a window */

class	WindowContentView : public View {
public:
						WindowContentView();
						
	virtual				~WindowContentView();
	
	void				Initialize( WindowRef inSysWindow );
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
	
	virtual OSStatus	DoControlBoundsChanged(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl,
								UInt32			inChangeAttributes,
								const HIRect&	inOriginalBounds,
								const HIRect&	inCurrentBounds);
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxWindowContentView
