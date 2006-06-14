// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxResourceFork.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:19 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxResourceFork.h
	@brief		Class for accessing a file's resource fork
*/

#ifndef H_PPxResourceFork
#define H_PPxResourceFork
#pragma once

#include <PPxFileFork.h>

namespace PPx {

// ===========================================================================
/**
	Wrapper class for the resource fork of a file */

class	ResourceFork : public FileFork {
public:
						ResourceFork(
								SInt16		inRefNum,
								bool		inOwnsRefNum);

						ResourceFork(
								const FSRef&	inFile,
								SInt8			inPermissions = fsRdWrPerm);

	static
	const HFSUniStr255&	GetForkName();
};

} // namespace PPx

#endif	// H_PPxResourceFork
