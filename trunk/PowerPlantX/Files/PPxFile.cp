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

#include <PPxFile.h>
#include <PPxExceptions.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	File															  [public]
/**
	Default constructor */

File::File()
{
}


// ---------------------------------------------------------------------------
//	File															  [public]
/**
	Constructs from a FSObject
	
	@param	inLocation	FSObject specifying the file location				*/

File::File(
	const FSObject&	inLocation)
	
	: mLocation(inLocation)
{
}


// ---------------------------------------------------------------------------
//	File															  [public]
/**
	Copy constructor
	
	The data and resource fork objects are not copied. The File
	has both forks closed after construction.								*/

File::File(
	const File&	inOriginal)
	
	: mLocation(inOriginal.mLocation)
{
	// mDataFork and mResourceFork are not copied
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator. The data resource forks are not assigned.
	The File has both forks after assignment.								*/

File&
File::operator = (
	const File&	inSource)
{
	mDataFork.reset();
	mResourceFork.reset();

	mLocation = inSource.mLocation;
	
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	IsEqualTo														  [public]
/**
	Returns whether the File represents the same disk file as another
	File object
	
	@param	inOther		File object with which to test equality
	
	@return	Whether the File objects refer to the same file					*/
	
bool
File::IsEqualTo(
	const File&		inOther) const
{
	return mLocation.IsEqualTo(inOther.mLocation);
}


// ---------------------------------------------------------------------------
//	GetLocation														  [public]
/**
	Returns a reference to a FSObject specifying the file's location
	
	@return Reference to FSObject specifying the file's location			*/

const FSObject&
File::GetLocation() const
{
	return mLocation;
}


// ---------------------------------------------------------------------------
//	UpdateLocation													  [public]
/**
	Updates File's location to reflect its on-disk state
	
	Call if you think the File may have benn moved or deleted via external
	means, such as user actions in the Finder								*/

void
File::UpdateLocation()
{
	if (mDataFork.get() != nil) {
		mDataFork->GetFSObject(mLocation);
		
	} else if (mResourceFork.get() != nil) {
		mResourceFork->GetFSObject(mLocation);
		
	} else if (mLocation.IsValid() == true) {
		mLocation.Update();
	}
}


// ---------------------------------------------------------------------------
//	Invalidate														  [public]
/**
	Invalidates the internal state of the File object. Until you respecify
	the file location via the assignment operator, any future attempt to
	perform a non-const operation on the File will cause an exception.
	
	Call if you know that the File no longer exists through some
	means external to this object, such as user actions in the Finder.		*/

void
File::Invalidate()
{
	CloseDataFork();
	CloseResourceFork();
	mLocation.Invalidate();
}


// ---------------------------------------------------------------------------
//	CreateOnDisk													  [public]
/**
	Creates file on disk with the specifed catalog information
	
	@param	inCatInfoFlags	Bit flags specifying which information to set
	@param	inCatInfo		Catalog informatin to set
	@param	inReplace		Whether it's OK to replace an existing file		*/

void
File::CreateOnDisk(
	FSCatalogInfoBitmap 	inCatInfoFlags,
	const FSCatalogInfo&	inCatInfo,
	bool					inReplace)
{
	if (mLocation.Exists()) {
		if (inReplace) {
			mLocation.Delete();
		} else {
			PPx_Throw_(LogicError, err_Logic, "File exists and inReplace is false");
		}
	}

	FSRef parentRef;
	mLocation.GetParent(parentRef);

	HFSUniStr255 fileName;
	mLocation.GetName(fileName);

	FSRef fileRef;
	OSStatus err = ::FSCreateFileUnicode(&parentRef, fileName.length,
											fileName.unicode, inCatInfoFlags,
		&inCatInfo, &fileRef, NULL);
	PPx_ThrowIfOSError_(err, "FSCreateFileUnicode failed");
	
	mLocation = fileRef;
}


// ---------------------------------------------------------------------------
//	CreateOnDisk													  [public]
/**
	Creates file on disk with the specifed type and creator
	
	@param	inFileType		File type
	@param	inCreator		Creator code
	@param	inReplace		Whether it's OK to replace an existing file		*/

void
File::CreateOnDisk(
	OSType		inFileType,
	OSType		inCreator,
	bool		inReplace)
{
	FSCatalogInfo	catInfo;
	FInfo*			finderInfo = (FInfo *) &catInfo.finderInfo;
	
	::BlockZero(finderInfo, sizeof(FInfo));
	
	finderInfo->fdType		= inFileType;
	finderInfo->fdCreator	= inCreator;

	CreateOnDisk(kFSCatInfoFinderInfo, catInfo, inReplace);
}


// ---------------------------------------------------------------------------
//	DeleteOnDisk													  [public]
/**
	Deletes file from disk
	
	@note The File object is still valid afterwards, so you can recreate
	the file by calling CreateOnDisk at a later time.						*/

void
File::DeleteOnDisk()
{
	CloseDataFork();
	CloseResourceFork();
	mLocation.Delete();
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetTotalForkSizes												  [public]
/**
	Passes back the total logical size, physical size, and number of
	forks of a file
	
	@param	outLogicalSize	Sum of all fork logical sizes
	@param	outPhysicalSize	Sum of all fork physical sizes
	@param	outForkCount	Number of file forks
	
	@note Pass nil for any of the parameters if you do not wish to obtain
	that piece of information												*/

void
File::GetTotalForkSizes(
	UInt64*		outLogicalSize,
	UInt64*		outPhysicalSize,
	ItemCount*	outForkCount) const
{
	OSStatus err = ::FSGetTotalForkSizes(&mLocation.UseRef(), outLogicalSize,
											outPhysicalSize, outForkCount);
	PPx_ThrowIfOSError_(err, "FSGetTotalForkSizes failed");
}


// ---------------------------------------------------------------------------
//	OpenDataFork													  [public]
/**
	Opens a File's data fork
	
	@param	inPermissions	Access permissions for the data fork
	
	@return	DataFork object for the File
	
	@note If the data fork is already open, the access permissions are
	not changed																*/

DataFork*
File::OpenDataFork(
	SInt8 inPermissions)
{
	if (mDataFork.get() == nil) {
		mDataFork.reset( new DataFork(mLocation.UseRef(), inPermissions) );
	}

	return mDataFork.get();
}


// ---------------------------------------------------------------------------
//	CloseDataFork													  [public]
/**
	Closes a File's data fork												*/

void	
File::CloseDataFork()
{
	mDataFork.reset();
}


// ---------------------------------------------------------------------------
//	OpenResourceFork												  [public]
/**
	Opens a File's resource fork
	
	@param	inPermissions	Access permissions for the resource fork
	
	@return	ResourceFork object for the File
	
	@note If the resource fork is already open, the access permissions are
	not changed																*/

ResourceFork*
File::OpenResourceFork(
	SInt8	inPermissions)
{
	if (mResourceFork.get() == nil) {
		mResourceFork.reset( new ResourceFork(mLocation.UseRef(), inPermissions) );
	}

	return mResourceFork.get();
}


// ---------------------------------------------------------------------------
//	CloseResourceFork												  [public]
/**
	Closes a File's resource fork											*/

void
File::CloseResourceFork()
{
	mResourceFork.reset();
}


} // namespace PPx