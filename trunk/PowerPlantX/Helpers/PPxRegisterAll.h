// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRegisterAll.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxRegisterAll.h
	@brief		Helper functions for registering items related to the
				PPx persistence mechanism
*/

#ifndef H_PPxRegisterAll
#define H_PPxRegisterAll
#pragma once

#include <PPxPrefix.h>

namespace PPx {

	void		RegisterCommonXMLDecoders();

	void		RegisterCommonXMLEncoders();

	void		RegisterCommonNibDecoders();

} // namespace PPx


#endif	// H_PPxRegisterAll
