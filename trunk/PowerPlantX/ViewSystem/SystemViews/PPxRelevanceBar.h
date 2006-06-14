// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRelevanceBar.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxRelevanceBar.h
	@brief		A system relevance bar control
*/

#ifndef H_PPxRelevanceBar
#define H_PPxRelevanceBar
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	RelevanceBar
/**
	A system relevance bar control */

class	RelevanceBar : public View {
public:
						RelevanceBar();
						
	virtual				~RelevanceBar();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								SInt32			inInitialValue,
								SInt32			inMinValue,
								SInt32			inMaxValue);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxRelevanceBar
