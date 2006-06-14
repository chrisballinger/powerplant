// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFileFork.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:18 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxFileFork.h
	@brief		Class for accessing a fork of a file
*/

#ifndef H_PPxFileFork
#define H_PPxFileFork
#pragma once

#include <PPxPrefix.h>
#include <CoreServices/CoreServices.h>

namespace PPx {

class FSObject;

// ===========================================================================
//	FileFork
/**
	Wrapper class for a fork of a file */

class	FileFork {
public:
						FileFork(
								const FSRef&		inFile,
								SInt8				inPermissions,
								const HFSUniStr255&	inForkName);

						FileFork(
								SInt16	inRefNum,
								bool	inOwnsRefNum);

	virtual				~FileFork();

	SInt16				UseRefNum() const;
	
	bool				IsOpen() const;

	SInt64				GetSize() const;

	void				SetSize(
								SInt64	inSize,
								UInt16	inPositionMode = fsFromStart);

	SInt64				GetPosition() const;

	void				SetPosition(
								SInt64	inOffset,
								UInt16	inPositionMode = fsFromStart);

	void				GetFSRef( FSRef& outRef ) const;

	void				GetFSObject( FSObject& outFSObject ) const;

	void				GetForkName( HFSUniStr255& outForkName ) const;

	void				GetForkInfo( FSForkInfo& outForkInfo ) const;

	SInt16				Open(	const FSRef&		inFile,
								SInt8				inPermissions,
								const HFSUniStr255&	inForkName);		

	void				Close();

	void				Flush();

private:
	SInt16			mRefNum;			/**< Open file fork referece number */
	bool			mOwnsRefNum;		/**< Whether to close fork upon destruction */

private:									// Forbid copy and assignment
						FileFork( const FileFork& );
	FileFork&			operator = ( const FileFork& );
};

} // namespace PPx

#endif	// H_PPxFileFork
