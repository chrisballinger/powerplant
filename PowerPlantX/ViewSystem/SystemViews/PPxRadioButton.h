// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRadioButton.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxRadioButton.h
	@brief		A system radio button control
*/

#ifndef H_PPxRadioButton
#define H_PPxRadioButton
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	RadioButton
/**
	A system radio button control */

class	RadioButton : public View {
public:
						RadioButton();
						
	virtual				~RadioButton();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inTitle,
								SInt32			inInitialValue,
								bool			inAutoToggle);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	bool		mAutoToggle;
};


} // namespace PPx

#endif	// H_PPxRadioButton
