// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxProgressBar.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxProgressBar.h
	@brief		A system progress bar control
*/

#ifndef H_PPxProgressBar
#define H_PPxProgressBar
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	ProgressBar
/**
	A system progress bar control */

class	ProgressBar : public View {
public:
						ProgressBar();
						
	virtual				~ProgressBar();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								SInt32			inInitialValue,
								SInt32			inMinValue,
								SInt32			inMaxValue,
								bool			inIsIndeterminate);
	
	void				SetIndeterminate( bool inIsIndeterminate );
	
	bool				IsIndeterminate() const;
	
	void				SetAnimating( bool inIsAnimating );
	
	bool				IsAnimating() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxProgressBar
