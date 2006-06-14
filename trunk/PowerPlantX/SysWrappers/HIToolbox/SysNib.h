// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysNib.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysNib.h
	@brief		Wrapper class for a Nib file
*/

#ifndef H_SysNib
#define H_SysNib
#pragma once

#include <SysCFString.h>
#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysNib
/**
	Wrapper class for a Nib file */

class	SysNib {
public:
						SysNib( CFStringRef inNibName );
						
						SysNib(	CFBundleRef	inBundle,
								CFStringRef	inNibName);
								
						~SysNib();
						
	WindowRef			CreateWindow( CFStringRef inWindowName );
	
	MenuRef				CreateMenu( CFStringRef inMenuName );
	
	Handle				CreateMenuBar( CFStringRef inMenuBarName );
	
	void				SetMenuBar( CFStringRef inMenuBarName );

private:
	IBNibRef	mNibRef;
};


} // namespace PPx

#endif	// H_SysNib
