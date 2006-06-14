// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFolder.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:18 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxFolder.h
*/

#ifndef H_PPxFolder
#define H_PPxFolder
#pragma once

#include <PPxFSObject.h>

namespace PPx {

// ===========================================================================
//	Folder
/**
	Encapsulates a Mac OS file system folder.
	
	Provides functions for creating a folder on disk and obtaining
	information about an existing folder.									*/

class	Folder {
public:
						Folder();

						Folder( const FSObject& inLocation );

						Folder(
								FSVolumeRefNum	inVolume,
								SInt32			inDirID);
						
						Folder( const Folder& inOriginal );
						
	Folder&				operator = ( const Folder& inOriginal );

	bool				IsEqualTo(const Folder& inOther ) const;

	const FSObject&		GetLocation() const;

	FSVolumeRefNum		GetVolume() const;

	SInt32				GetDirID() const;

	void				UpdateLocation();
	
	void				Invalidate();

	void				CreateOnDisk(
							FSCatalogInfoBitmap		inCatInfoFlags,
							const FSCatalogInfo&	inCatInfo,
							bool					inReplace);

	void				DeleteOnDisk();

	void				DeleteContents();
	
private:
	void				SetVolAndDirIDFromLocation();

private:
	FSVolumeRefNum		mVolume;	/**< Volume where folder resides */
	SInt32				mDirID;		/**< Directory ID of folder */
	FSObject			mLocation;	/**< File system object for accessing
											the folder */
};


// -----------------------------------------------------------------------
//	GetLocation
/**
	Returns FSObject specifying the Folders' location
	
	@return	FSObject specifying the Folders' location					*/
	
inline const FSObject&
Folder::GetLocation() const
{
	return mLocation;
}


// -----------------------------------------------------------------------
//	GetVolume
/**
	Returns the Folder's volume reference number
	
	@return Folder's volume reference number							*/

inline FSVolumeRefNum
Folder::GetVolume() const
{
	return (mVolume);
}


// -----------------------------------------------------------------------
//	GetDirID
/**
	Returns the Folder's directory ID
	
	@return Folder's directory ID										*/

inline SInt32
Folder::GetDirID() const
{
	return (mDirID);
}


} // namespace PPx

#endif	// H_PPxFolder
