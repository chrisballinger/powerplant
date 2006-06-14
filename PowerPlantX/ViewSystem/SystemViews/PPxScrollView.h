// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxScrollView.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxScrollView.h
	@brief		A system scroll view
*/

#ifndef H_PPxScrollView
#define H_PPxScrollView
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	ScrollView
/**
	A system scroll view */

class	ScrollView : public View {
public:
						ScrollView();
						
	virtual				~ScrollView();
	
	void				Initialize(
								View*				inSuperView,
								const HIRect&		inFrame,
								bool				inVisible,
								bool				inEnabled,
								OptionBits			inOptions);
								
	void				SetAutoHideScrollBars( bool inAutoHide );
	
	bool				GetAutoHideScrollBars() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	OptionBits		mScrollOptions;
};


} // namespace PPx

#endif	// H_PPxScrollView
