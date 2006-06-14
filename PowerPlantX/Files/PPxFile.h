// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFile.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:18 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxFile.h
	@brief		Class for a file on disk
*/

#ifndef H_PPxFile
#define H_PPxFile
#pragma once

#include <PPxDataFork.h>
#include <PPxFSObject.h>
#include <PPxResourceFork.h>

namespace PPx {

// ===========================================================================
//	File
/**
	A file on disk. Provides functions for creating a file an accessing
	its data and resource forks
	
	File does not provide I/O operations. It creates DataFork and
	ResourceFork classes which implement reading and writing.				*/

class	File {
public:
						File();

						File( const FSObject &inLocation );
						
						File ( const File& inOriginal );
						
	File&				operator = (const File& inSource );

	bool				IsEqualTo( const File &inOther ) const;

	const FSObject&		GetLocation() const;

	void				UpdateLocation();
	
	void				Invalidate();

	void				CreateOnDisk(
								FSCatalogInfoBitmap		inCatInfoFlags,
								const FSCatalogInfo&	inCatInfo,
								bool					inReplace);

	void				CreateOnDisk(
								OSType		inFileType,
								OSType		inCreator,
								bool		inReplace);
								
	void				DeleteOnDisk();

	void				GetTotalForkSizes(
								UInt64*		outLogicalSize,
								UInt64*		outPhysicalSize = nil,
								ItemCount*	outForkCount = nil) const;

								// Data Fork operations
	DataFork*			GetDataFork();

	bool				IsDataForkOpen() const;

	DataFork*			OpenDataFork(SInt8 inPermissions = fsRdWrPerm);

	void				CloseDataFork();

								// Resource Fork operations
	ResourceFork*		GetResourceFork();

	bool				IsResourceForkOpen() const;

	ResourceFork*		OpenResourceFork(SInt8 inPermissions = fsRdWrPerm);

	void				CloseResourceFork();

private:
	FSObject					mLocation;
	std::auto_ptr<DataFork>		mDataFork;
	std::auto_ptr<ResourceFork>	mResourceFork;
};


// ---------------------------------------------------------------------------
//	GetDataFork
/**
	Returns DataFork object for the File
	
	@return DataFork object for the File									*/

inline DataFork*
File::GetDataFork()
{
	return mDataFork.get();
}


// ---------------------------------------------------------------------------
//	IsDataForkOpen
/**
	Returns whether the File's data fork is open
	
	@return Whether the File's data fork is open							*/

inline bool
File::IsDataForkOpen() const
{
	return (mDataFork.get() != nil);
}


// ---------------------------------------------------------------------------
//	GetResourceFork
/**
	Returns ResourceFork object for the File
	
	@return ResourceFork object for the File								*/

inline ResourceFork *
File::GetResourceFork()
{
 	return mResourceFork.get();
}


// ---------------------------------------------------------------------------
//	IsResourceForkOpen
/**
	Returns whether the File's resource fork is open
	
	@return Whether the File's resource fork is open						*/

inline bool
File::IsResourceForkOpen() const
{
	return (mResourceFork.get() != nil);
}


} // namespace PPx


#endif // H_PPxFile
