// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCheckBox.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxCheckBox.h
	@brief		A system check box control
*/

#ifndef H_PPxCheckBox
#define H_PPxCheckBox
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	CheckBox
/**
	A system check box control */

class	CheckBox : public View {
public:
						CheckBox();
						
	virtual				~CheckBox();
	
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
	bool		mAutoToggle;	/**< Toggle between on/off when clicked */
};


} // namespace PPx

#endif	// H_PPxCheckBox
