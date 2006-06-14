// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEnvironment.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxEnvironment.h
	@brief		Utility functions for determining the environment (hardware
				and software) in which a program is running.
*/

#ifndef H_PPxEnvironment
#define H_PPxEnvironment
#pragma once

#include <PPxPrefix.h>

namespace PPx {


// ===========================================================================
//	GestaltValue
/**
	Wrapper class for finding and storing a Toolbox Gestalt value */

class	GestaltValue {
public:
	explicit			GestaltValue( OSType inSelector );
	
	SInt32				GetValue() const	{ return mValue; }
	
private:
	SInt32				mValue;			/**< Gestalt response value */
};


// ===========================================================================
//	Environment
/**
	Utility functions for determining the environment (hardware and software)
	in which a program is running											*/

namespace Environment {

	SInt16		GetOSVersion();
				
} // namespace Environment


} // namespace PPx

#endif	// H_PPxEnvironment