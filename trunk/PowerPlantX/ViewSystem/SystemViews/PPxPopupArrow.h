// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPopupArrow.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPopupArrow.h
	@brief		A system popup arrow view
*/

#ifndef H_PPxPopupArrow
#define H_PPxPopupArrow
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	PopupArrow
/**
	A system popup arrow view */

class	PopupArrow : public View {
public:
						PopupArrow();
						
	virtual				~PopupArrow();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								ControlPopupArrowOrientation	inOrientation,
								ControlPopupArrowSize			inArrowSize);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	ControlPopupArrowOrientation	mOrientation;	/**< Direction that arrow points */
	ControlPopupArrowSize			mArrowSize;		/**< Normal or small arrow size */
};


} // namespace PPx

#endif	// H_PPxPopupArrow
