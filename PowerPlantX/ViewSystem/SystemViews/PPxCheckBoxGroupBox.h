// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCheckBoxGroupBox.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxCheckBoxGroupBox.h
	@brief		A system group box with a check box title
*/

#ifndef H_PPxCheckBoxGroupBox
#define H_PPxCheckBoxGroupBox
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	CheckBoxGroupBox
/**
	A system group box with a check box title */

class	CheckBoxGroupBox : public View {
public:
						CheckBoxGroupBox();
						
	virtual				~CheckBoxGroupBox();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inTitle,
								SInt32			inInitialValue,
								bool			inIsPrimary,
								bool			inAutoToggle);
	
	void				GetTitleRect( Rect& outTitleRect ) const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:
	bool		mAutoToggle;		/**< Toggle between on/off when clicked */
	bool		mIsPrimaryGroup;	/**< Primary (true) or Secondary (false) group */
};


} // namespace PPx

#endif	// H_PPxCheckBoxGroupBox
