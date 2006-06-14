// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPushButton.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPushButton.h
	@brief		A system push button control
*/

#ifndef H_PPxPushButton
#define H_PPxPushButton
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
/**	A system push button control */

class	PushButton : public View {
public:
						PushButton();
						
	virtual				~PushButton();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inTitle);
	
	void				SetDefaultFlag( bool inIsDefault );
	
	bool				GetDefaultFlag() const;
	
	void				SetCancelFlag( bool inIsCancel );
	
	bool				GetCancelFlag() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxPushButton
