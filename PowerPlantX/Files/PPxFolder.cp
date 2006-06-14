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

#include <PPxFolder.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Folder															  [public]
/**
	Default constructor */

Folder::Folder()
{
	mVolume = kFSInvalidVolumeRefNum;
	mDirID	= dirID_Invalid;
}


// ---------------------------------------------------------------------------
//	Folder															  [public]
/**
	Constructs from an FSObject specifying the folder location
	
	@param	inLocation	FSObject specifying the folder location				*/

Folder::Folder(
	const FSObject&	inLocation)
	
	: mLocation(inLocation)
{
	SetVolAndDirIDFromLocation();
}


// ---------------------------------------------------------------------------
//	Folder															  [public]
/**
	Constructs from a volume and directory ID
	
	@param	inVolume	Volume reference number
	@param	inDirID		Directory ID of folder								*/
	
Folder::Folder(
	FSVolumeRefNum	inVolume,
	SInt32			inDirID)
{
	mVolume = inVolume;
	mDirID	= inDirID;
	
	UpdateLocation();
}


// ---------------------------------------------------------------------------
//	Folder															  [public]
/**
	Copy constructor */
	
Folder::Folder(
	const Folder&	inOriginal)
	
	: mVolume(inOriginal.mVolume),
	  mDirID(inOriginal.mDirID),
	  mLocation(inOriginal.mLocation)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */
	
Folder&
Folder::operator = (
	const Folder&	inSource)
{
	mVolume		= inSource.mVolume;
	mDirID		= inSource.mDirID;
	mLocation	= inSource.mLocation;
	
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	IsEqualTo														  [public]
/**
	Returns whether the Folder refers to the same on-disk folder as
	another Folder object
	
	@param	inOther		Folder with which to test equality
	
	@return	Whether the Folder objects refer to the same folder				*/
	
bool
Folder::IsEqualTo(
	const Folder&	inOther) const
{
	return (mLocation.IsEqualTo(inOther.mLocation));
}


// ---------------------------------------------------------------------------
//	UpdateLocation													  [public]
/**
	Updates Folder's location to reflect its on-disk state
	
	Call if you think the Folder may have been moved or deleted via external
	means, such as user actions in the Finder								*/

void
Folder::UpdateLocation()
{
	if ( (mVolume != kFSInvalidVolumeRefNum) and
		 (mDirID != dirID_Invalid) ) {
		 							// Get FSRef from volume and dir ID
		FSRef		updatedRef;
		FSRefParam	parentParams;
		::BlockZero(&parentParams, sizeof(FSRefParam));
		
		parentParams.ioNamePtr	= pString_Empty;
		parentParams.ioVRefNum	= mVolume;
		parentParams.ioDirID	= mDirID;
		parentParams.newRef		= &updatedRef;

		OSStatus err = ::PBMakeFSRefSync(&parentParams);
		PPx_ThrowIfOSError_(err, "PBMakeFSRefSync failed");
		
		mLocation = updatedRef;
		
	} else {						// Update location and get volume
		mLocation.Update();			//   and dir ID from its FSRef
		SetVolAndDirIDFromLocation();
	}
}


// ---------------------------------------------------------------------------
//	Invalidate														  [public]
/**
	Invalidates the internal state of the Folder object. Until you
	respecify the folder location via the assigment operator, any
	future attempt to perform a non-const operation on the Folder will
	cause an exception.
	
	Call if you know that the Folder no longer exists through some means
	external to this object, such as user actions in the Finder.			*/

void
Folder::Invalidate()
{
	mVolume	= kFSInvalidVolumeRefNum;
	mDirID	= dirID_Invalid;
	mLocation.Invalidate();
}


// ---------------------------------------------------------------------------
//	CreateOnDisk													  [public]
/**
	Creates folder on disk with the specified catalog information
	
	@param	inCatInfoFlags	Bit flags specifying which information to set
	@param	inCatInfo		Catalog informatin to set
	@param	inReplace		Whether it's OK to replace an existing folder	*/


void
Folder::CreateOnDisk(
	FSCatalogInfoBitmap		inCatInfoFlags,
	const FSCatalogInfo&	inCatInfo,
	bool					inReplace)
{
								// Save parent and name in case we're
								// replacing an existing folder.
	FSRef parentRef;
	mLocation.GetParent(parentRef);

	HFSUniStr255 fileName;
	mLocation.GetName(fileName);

	OSStatus err;

	if (mLocation.Exists()) {
		if (inReplace) {		// Delete the existing folder.
								//   This will throw an exception if 
								//   mLocation really points to a file.
			err = ::FSDeleteContainer(&mLocation.UseRef()); 
			PPx_ThrowIfOSError_(err, "FSDeleteContainer failed");
			
		} else {
			PPx_Throw_(LogicError, err_Logic,
						"Folder exists and inReplace is false");
		}
	}
								// Create new folder on disk
	FSRef	folderRef;
	err = ::FSCreateDirectoryUnicode(&parentRef, fileName.length,
				fileName.unicode, inCatInfoFlags, &inCatInfo, &folderRef,
				nil, reinterpret_cast<UInt32 *>(&mDirID));
	PPx_ThrowIfOSError_(err, "FSCreateDirectoryUnicode failed");

	mLocation	= folderRef;
	mVolume		= mLocation.GetVolume();
}


// ---------------------------------------------------------------------------
//	DeleteOnDisk													  [public]
/**
	Deletes folder from disk
	
	@note The Folder object is still valid afterwards, so you can recreate
	the folder by calling CreateOnDisk at a later time.						*/

void
Folder::DeleteOnDisk()
{
	mLocation.DeleteContainer();
	mVolume = kFSInvalidVolumeRefNum;
	mDirID	= dirID_Invalid;
}


// ---------------------------------------------------------------------------
//	DeleteContents													  [public]
/**
	Deletes on disk the files and directories within the Folder				*/

void
Folder::DeleteContents()
{
	mLocation.DeleteContainerContents();
}


// ---------------------------------------------------------------------------
//	SetVolAndDirIDFromLocation										 [private]
/**
	Sets the volume and directory ID from the location FSObject. Used
	internally to synch the state of the object.							*/

void
Folder::SetVolAndDirIDFromLocation()
{
	if (mLocation.Exists()) {
	
			// Get volume and directoy ID for from catalog info
			// of location
	
		FSCatalogInfoBitmap		catInfoBM = kFSCatInfoVolume |
											kFSCatInfoNodeID |
											kFSCatInfoNodeFlags;
		FSCatalogInfo	catInfo;
		mLocation.GetCatalogInfo(catInfoBM, catInfo);
		
		if ((catInfo.nodeFlags & kFSNodeIsDirectoryMask) == 0) {
			PPx_ThrowOSError_(errFSNotAFolder, "File object is not a folder");
		}

		mVolume	= catInfo.volume;
		mDirID	= catInfo.nodeID;
		
	} else {
		mVolume = kFSInvalidVolumeRefNum;
		mDirID	= dirID_Invalid;
	}
}


} // namespace PPx