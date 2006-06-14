// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataFork.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:18 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDataFork.h
	@brief		Class for accessing the contents of a file's data fork
*/

#ifndef H_PPxDataFork
#define H_PPxDataFork
#pragma once

#include <PPxFileFork.h>
#include <SysCFData.h>

namespace PPx {

// ===========================================================================
//	DataFork
/**
	Wrapper class for the data fork of a file */

class	DataFork : public FileFork {
public:
						DataFork(
								SInt16	inRefNum,
								bool	inOwnsRefNum);
									
						DataFork(
								const FSRef&	inFile,
								SInt8			inPermissions = fsRdWrPerm);

	CFData				ReadContents();

	void				WriteContents( const CFData& inData );

	void				WriteContents(
								const void*	inBuffer,
								ByteCount	inBufferSize);

	void				ReadData(
								void*		inBuffer,
								ByteCount	inBufferSize,
								UInt16		inPositionMode = fsFromMark,
								SInt64		inOffset = 0);

	void				WriteData(
								const void*	inBuffer,
								ByteCount	inBufferSize,
								UInt16		inPositionMode = fsFromMark,
								SInt64		inOffset = 0);

	static
	const HFSUniStr255&	GetForkName();
};


} // namespace PPx

#endif	// H_PPxDataFork
