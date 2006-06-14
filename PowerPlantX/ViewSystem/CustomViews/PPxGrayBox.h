// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxGrayBox.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxGrayBox.h
	@brief		View which draws a gray box
*/

#ifndef H_PPxGrayBox
#define H_PPxGrayBox
#pragma once

#include <PPxBaseView.h>
#include <PPxViewEvents.h>

namespace PPx {

// ===========================================================================
//	GrayBox
/**
	View which draws a gray box */

class	GrayBox : public BaseView,
				  public ControlDrawDoer {
public:
						GrayBox();

	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								float			inStrokeGray,
								float			inStrokeAlpha,
								float			inFillGray,
								float			inFillAlpha,
								OptionBits		inFeatures = features_None);

protected:
	virtual void		InitState( const DataReader& inReader );
	
	virtual void		WriteState( DataWriter& ioWriter ) const;

	virtual OSStatus	DoControlDraw(
								SysCarbonEvent&		ioEvent,
								ControlRef			inControl,
								ControlPartCode		inPartCode,
								RgnHandle			inClipRgn,
								CGContextRef		inContext);

private:
	virtual CFStringRef	ClassName() const;
	
	virtual void		FinishInit();
	
private:
	float		mStrokeGray;
	float		mStrokeAlpha;
	float		mFillGray;
	float		mFillAlpha;
};


} // namespace PPx

#endif	// H_PPxGrayBox
