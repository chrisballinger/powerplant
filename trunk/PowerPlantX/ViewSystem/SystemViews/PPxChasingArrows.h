// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxChasingArrows.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxChasingArrows.h
	@brief		A system chasing arrows activity indicator
*/

#ifndef H_PPxChasingArrows
#define H_PPxChasingArrows
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	ChasingArrows
/**
	A system chasing arrows activity indicator */

class	ChasingArrows : public View {
public:
						ChasingArrows();
						
	virtual				~ChasingArrows();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled);
	
	void				SetAnimating( bool inIsAnimating );
	
	bool				IsAnimating() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxChasingArrows
