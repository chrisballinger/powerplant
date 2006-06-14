// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowHeader.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxWindowHeader.h
	@brief		A system window header view
*/

#ifndef H_PPxWindowHeader
#define H_PPxWindowHeader
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	WindowHeader
/**
	A system window header view */

class	WindowHeader : public View {
public:
						WindowHeader();
						
	virtual				~WindowHeader();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								bool			inIsListHeader);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	bool			mIsListHeader;		/**< Use list header variant? */
};


} // namespace PPx

#endif	// H_PPxWindowHeader
