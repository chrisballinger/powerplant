// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSearchField.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSearchField.h
	@brief		A system search field
*/

#ifndef H_PPxSearchField
#define H_PPxSearchField
#pragma once

#include <PPxView.h>

// The system search field is only available on Mac OS 10.3 or later
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

namespace PPx {

// ===========================================================================
//	SearchField
/**
	A system search field */

class	SearchField : public View {
public:
						SearchField();
						
	virtual				~SearchField();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								OptionBits		inAttributes,
								MenuRef			inSearchMenu,
								CFStringRef		inDescriptiveText);
	
	void				SetSearchMenu( MenuRef inSearchMenu );
	
	MenuRef				GetSearchMenu() const;
	
	void				ChangeAttributes(
								OptionBits	inAttributesToSet,
								OptionBits	inAttributesToClear);
	
	OptionBits			GetAttributes() const;
	
	void				SetDescriptiveText( CFStringRef inText );
	
	CFString			GetDescriptiveText() const;

protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif // 10.3 or later

#endif	// H_PPxSearchField
