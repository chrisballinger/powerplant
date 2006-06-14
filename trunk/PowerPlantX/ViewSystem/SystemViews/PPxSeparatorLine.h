// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSeparatorLine.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSeparatorLine.h
	@brief		A system separator line view
*/

#ifndef H_PPxSeparatorLine
#define H_PPxSeparatorLine
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	SeparatorLine
/**
	A system separator line view */

class	SeparatorLine : public View {
public:
						SeparatorLine();
						
	virtual				~SeparatorLine();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled);
	
protected:
	virtual void		InitState( const DataReader& inReader );

private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxSeparatorLine
