// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRoundButton.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxRoundButton.h
	@brief		A system round button control
*/

#ifndef H_PPxRoundButton
#define H_PPxRoundButton
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	RoundButton
/**
	A system round button control */

class	RoundButton : public View {
public:
						RoundButton();
						
	virtual				~RoundButton();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								ControlRoundButtonSize			inButtonSize,
								const ControlButtonContentInfo&	inContent);
	
	void				SetContentInfo( const ControlButtonContentInfo& inContent );
	
	void				GetContentInfo( ControlButtonContentInfo& outContent ) const;
	
	void				SetButtonSize( ControlRoundButtonSize inButtonSize );
	
	ControlRoundButtonSize
						GetButtonSize() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxRoundButton
