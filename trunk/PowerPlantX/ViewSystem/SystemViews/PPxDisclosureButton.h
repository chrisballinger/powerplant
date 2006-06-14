// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDisclosureButton.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDisclosureButton.h
	@brief		A system disclosure button control
*/

#ifndef H_PPxDisclosureButton
#define H_PPxDisclosureButton
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	DisclosureButton
/**
	A system disclosure button control */

class	DisclosureButton : public View {
public:
						DisclosureButton();
						
	virtual				~DisclosureButton();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								SInt32			inInitialValue,
								bool			inAutoToggle);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:
	bool		mAutoToggle;	/**< Toggle between open/closed when clicked */
};


} // namespace PPx

#endif	// H_PPxDisclosureButton
