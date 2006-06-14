// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxClockControl.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxClockControl.h
	@brief		A system clock control
*/

#ifndef H_PPxClockControl
#define H_PPxClockControl
#pragma once

#include <PPxView.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	ClockControl
/**
	A system clock control */

class	ClockControl : public View {
public:
						ClockControl();
						
	virtual				~ClockControl();
	
	void				Initialize(
								View*				inSuperView,
								const HIRect&		inFrame,
								bool				inVisible,
								bool				inEnabled,
								ControlClockType	inClockType,
								ControlClockFlags	inClockFlags);
	
	void				SetLongDate( const LongDateRec& inLongDate );
	
	void				GetLongDate( LongDateRec& outLongDate );
	
	void				SetThemeFontID( ThemeFontID inFontID );

	void				SetAnimating( bool inIsAnimating );
	
	bool				IsAnimating() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	ControlClockType	mClockType;		/**< Type of information (time or date) */
	ControlClockFlags	mClockFlags;	/**< Editable and live options */
};


} // namespace PPx

#endif	// H_PPxClockControl
