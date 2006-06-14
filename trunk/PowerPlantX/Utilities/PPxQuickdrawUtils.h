// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxQuickdrawUtils.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxQuickdrawUtils.h
	@brief		Utility classes and functions for working with Quickdraw
*/

#ifndef H_PPxQuickdrawUtils
#define H_PPxQuickdrawUtils
#pragma once

#include <PPxPrefix.h>
#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	GrafPortSaver
/**
	Saves, changes, and restores the current Quickdraw GrafPort */

class	GrafPortSaver {
public:
						GrafPortSaver( GrafPtr inPort );
						
						~GrafPortSaver();
private:
	GrafPtr		mSavedPort;
};


} // namespace PPx

#endif	// H_PPxQuickdrawUtils
