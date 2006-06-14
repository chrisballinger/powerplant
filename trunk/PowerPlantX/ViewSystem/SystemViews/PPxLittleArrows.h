// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxLittleArrows.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxLittleArrows.h
	@brief		A system little arrows control
*/

#ifndef H_PPxLittleArrows
#define H_PPxLittleArrows
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	LittleArrows
/**
	A system little arrows control */

class	LittleArrows : public View {
public:
						LittleArrows();
						
	virtual				~LittleArrows();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								SInt32			inInitialValue,
								SInt32			inMinValue,
								SInt32			inMaxValue,
								SInt32			inIncrement);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:
	SInt32		mIncrement;		/**< Amount to change value when clicked */
};


} // namespace PPx

#endif	// H_PPxLittleArrows
