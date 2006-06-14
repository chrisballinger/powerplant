// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxIconPushButton.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxIconPushButton.h
	@brief		A system push button with icon control
*/

#ifndef H_PPxIconPushButton
#define H_PPxIconPushButton
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	IconPushButton
/**
	A system push button with icon control */

class	IconPushButton : public View {
public:
						IconPushButton();
						
	virtual				~IconPushButton();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inTitle,
								const ControlButtonContentInfo&	inContent,
								ControlPushButtonIconAlignment	inAlignment);
	
	void				SetDefaultFlag( bool inIsDefault );
	
	bool				GetDefaultFlag() const;
	
	void				SetCancelFlag( bool inIsCancel );
	
	bool				GetCancelFlag() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	ControlButtonContentInfo		mContent;		/**< Source of icon data */
	ControlPushButtonIconAlignment	mIconAlignment;	/**< Icon on left or right */
};


} // namespace PPx

#endif	// H_PPxIconPushButton
