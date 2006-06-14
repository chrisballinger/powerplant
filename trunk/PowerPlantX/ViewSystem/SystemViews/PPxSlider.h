// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSlider.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSlider.h
	@brief		A system slider control
*/

#ifndef H_PPxSlider
#define H_PPxSlider
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	Slider
/**
	A system slider control */

class	Slider : public View {
public:
						Slider();
						
	virtual				~Slider();
	
	void				Initialize(
								View*						inSuperView,
								const HIRect&				inFrame,
								bool						inVisible,
								bool						inEnabled,
								SInt32						inInitialValue,
								SInt32						inMinValue,
								SInt32						inMaxValue,
								ControlSliderOrientation	inOrientation,
								UInt16						inTickMarksCount,
								bool						inHasLiveTracking);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:
	ControlSliderOrientation	mOrientation;		/**< Direction of indicator */
	UInt16						mTickMarksCount;	/**< Number of tick marks */
};


} // namespace PPx

#endif	// H_PPxSlider
