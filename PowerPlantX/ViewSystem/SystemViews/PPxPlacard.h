// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPlacard.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPlacard.h
	@brief		A system placard view
*/

#ifndef H_PPxPlacard
#define H_PPxPlacard
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	Placard
/**
	A system placard view */

class	Placard : public View {
public:
						Placard();
						
	virtual				~Placard();
	
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

#endif	// H_PPxPlacard
