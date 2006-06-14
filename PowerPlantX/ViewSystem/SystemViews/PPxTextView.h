// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTextView.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxTextView.h
	@brief		A system text view
*/

#ifndef H_PPxTextView
#define H_PPxTextView
#pragma once

#include <PPxView.h>

// The system text view is only available on Mac OS 10.3 or later
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

namespace PPx {

// ===========================================================================
//	TextView
/**
	A system text view */

class	TextView : public View {
public:
						TextView();
						
	virtual				~TextView();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								OptionBits		inOptions,
								TXNFrameOptions	inFrameOptions);
	
	TXNObject			GetTextObject() const;
	
	void				SetText( CFStringRef inText );
	
	CFString			GetText() const;

protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif // 10.3 or later

#endif	// H_PPxTextView
