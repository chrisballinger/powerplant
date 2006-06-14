// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRadioGroup.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxRadioGroup.h
	@brief		A system radio group control
*/

#ifndef H_PPxRadioGroup
#define H_PPxRadioGroup
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	RadioGroup
/**
	A system radio group control */

class	RadioGroup : public View {
public:
						RadioGroup();
						
	virtual				~RadioGroup();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled);
								
	View*				GetCurrentButton() const;
	
protected:
	virtual void		InitState( const DataReader& inReader );

private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxRadioGroup
