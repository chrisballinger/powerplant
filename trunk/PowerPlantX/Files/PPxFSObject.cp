// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFSObject.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:19 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxFSObject.h>
#include <PPxExceptions.h>
#include <PPxFolder.h>
#include <PPxFSUtils.h>
#include <SysCFURL.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Default constructor */

FSObject::FSObject()
{
	mState = ref_Invalid;
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Constructs from a file system FSRef
	
	@param	inFSRef		File system file reference							*/

FSObject::FSObject(
	const FSRef&	inFSRef)
	
	: mFSRef(inFSRef),
	  mState(ref_Exists)
{
	Update();
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Constructs from a parent FSRef and CFString item name
	
	@param	inParentRef		FSRef for parent directory
	@param	inName			Name of item as a CFString
	
	@note The entity referred to by the parent and name does not have to
	currently exist															*/

FSObject::FSObject(
	const FSRef&	inParentRef,
	const CFString&	inName)
	
	: mFSRef(inParentRef),
	  mName(inName),
	  mState(ref_ParentAndName)
{
	Update();
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Constructs from a parent FSRef and HFSUniStr255 item name
	
	@param	inParentRef		FSRef for parent directory
	@param	inName			Name of item as a HFSUniStr255
	
	@note The entity referred to by the parent and name does not have to
	currently exist															*/

FSObject::FSObject(
	const FSRef&		inParentRef,
	const HFSUniStr255&	inName)
	
	: mFSRef(inParentRef),
	  mName(inName),
	  mState(ref_ParentAndName)
{
	Update();
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Contructs from a volume refnum, parent directory ID,
	and CFString item name
	
	@param	inVRefNum		Volume reference number
	@param	inParentDirID	Parent directory ID
	@param	inName			Name of the item as a CFString
	
	@note The entity referred to by the parent and name does not have to
	currently exist															*/

FSObject::FSObject(
	FSVolumeRefNum		inVRefNum,
	SInt32				inParentDirID,
	const CFString&		inName)
{
	HFSUniStr255	hfsName;
	FSUtils::StringToHFSUniStr(inName, hfsName);
	
	InitFromVolDirName(inVRefNum, inParentDirID, hfsName);
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Contructs from a volume refnum, parent directory ID,
	and HFSUniStr255 item name
	
	@param	inVRefNum		Volume reference number
	@param	inParentDirID	Parent directory ID
	@param	inName			Name of the item as a HFSUniStr255
	
	@note The entity referred to by the parent and name does not have to
	currently exist															*/

FSObject::FSObject(
	FSVolumeRefNum			inVRefNum,
	SInt32					inParentDirID,
	const HFSUniStr255&		inName)
{
	InitFromVolDirName(inVRefNum, inParentDirID, inName);
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Constructs from a FSSpec
	
	@param	inFSSpec		FSSpec for the file system object
	@param	inNameEncoding	Encoding system for file name in the FSSpec		*/
	
FSObject::FSObject(
	const FSSpec&		inFSSpec,
	CFStringEncoding	inNameEncoding)
{
	mState = ref_Invalid;
								// First try to convert inFSSpec to an FSRef
	OSStatus err = ::FSpMakeFSRef(&inFSSpec, &mFSRef);
	
	if (err == noErr) {	
		mState = ref_Exists;	// That succeeded, so we have a valid FSRef
		
	} else if (inFSSpec.name[0] > 0) {
								// File object doesn't exist. Check if the
								//   parent directory in the FSSpec is valid.
		FSRefParam parentParams;
		::BlockZero(&parentParams, sizeof(FSRefParam));
		
		parentParams.ioNamePtr	= pString_Empty;
		parentParams.ioVRefNum	= inFSSpec.vRefNum;
		parentParams.ioDirID	= inFSSpec.parID;
		parentParams.newRef		= &mFSRef;

		err = ::PBMakeFSRefSync(&parentParams);
		
		if (err == noErr) {		// Our FSRef is now a valid parent
								//   directory. Copy name from the FSSpec
			mName = CFString(inFSSpec.name, inNameEncoding);
			mState = ref_ParentAndName;
		}
	}
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Constructs from a Core Foundation URL reference
	
	@param	inURL	CF URL descrbing a file system entity					*/

FSObject::FSObject(
	CFURLRef	inURL)
{
	InitFromURL(inURL);
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Constructs from an absolute path name
	
	@param	inAbsolutePath	Absolute path name to a file system item
	@param	inPathStyle		OS Path Style for URL (POSIX, HFS, Windows)		*/

FSObject::FSObject(
	const CFString&		inAbsolutePath,
	CFURLPathStyle		inPathStyle)
{
	mState = ref_Invalid;
									// Convert absolute path to a URL
	CFURL	theURL(inAbsolutePath, false, nil, inPathStyle, nil);
	
	InitFromURL(theURL.UseRef());
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Constructs from a relative path name
	
	@param	inRelativePath		Relative path name to a file system item
	@param	inBaseDirectory		Base directory of path
	@param	inPathStyle			OS Path Style for URL (POSIX, HFS, Windows)	*/

FSObject::FSObject(
	const CFString&		inRelativePath,
	const FSRef&		inBaseDirectory,
	CFURLPathStyle		inPathStyle)
{
	mState = ref_Invalid;
									// Convert base directory to a URL
	CFURL	baseURL(inBaseDirectory);
									
									// Convert relative path and base URL
									//   tp another URL
	CFURL	theURL(inRelativePath, false, baseURL, inPathStyle, nil);
	
	InitFromURL(theURL.UseRef());
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Copy constructor */

FSObject::FSObject(
	const FSObject&	inOriginal)
	
	: mFSRef(inOriginal.mFSRef),
	  mName(inOriginal.mName),
	  mState(inOriginal.mState)
{
}


// ---------------------------------------------------------------------------
//	FSObject														  [public]
/**
	Destructor */

FSObject::~FSObject()
{
}

#pragma mark -

// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */

FSObject&
FSObject::operator = (
	const FSObject&	inOther)
{
	mFSRef	= inOther.mFSRef;
	mName	= inOther.mName;
	mState	= inOther.mState;
	
	return *this;
}


// ---------------------------------------------------------------------------
//	operator = ( FSRef )											  [public]
/**
	Assigns FSObject from a FSRef
	
	@param	inFSRef		FSRef from which to assign
	
	@return	Reference to this FSObject									*/

FSObject&
FSObject::operator = (
	const FSRef&	inFSRef)
{
	mName.FreeRef();
	mFSRef	= inFSRef;
	mState	= ref_Exists;

	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	UseRef															  [public]
/**
	Returns a const reference to a FSRef. Throws if the FSRef is not valid.	*/

const FSRef&
FSObject::UseRef() const
{
	if (not Exists())
		PPx_ThrowOSError_(errFSBadFSRef, "File object has no FSRef");

	return mFSRef;
}


// ---------------------------------------------------------------------------
//	CompareTo														  [public]
/**
	Compares the FSObject with another FSObject
	
	@param	inOther		FSObject to which to compare
	
	@return	Whether the FSObjects are equivalent
	
	@retval	noErr	The FSObject are equivalent. Any other value means
					not equivalent.											*/

OSStatus
FSObject::CompareTo(
	const FSObject&	inOther) const
{
	if (mState != inOther.mState) {		// States are different
		return errFSRefsDifferent;
	}
	
	if (mState == ref_Invalid) {		// Both states are invalid, so
		return noErr;					//   we consider them equivalent
	}
	
	OSStatus	result = ::FSCompareFSRefs(&mFSRef, &inOther.mFSRef);
	
	if ( (result == noErr) and (mState == ref_ParentAndName) ) {
										// For parent & name, the names
										// must match as well
		if (not FSUtils::FSNamesAreEqual(mName, inOther.mName)) {
			result = errFSRefsDifferent;
		}
	}

	return result;
}


// ---------------------------------------------------------------------------
//	CompareTo														  [public]
/**
	Compares the FSObject with an FSRef
	
	@param	inFSRef		FSRef to which to compare
	
	@return Whether the FSObject refers to the same item as the FSRef
	
	@retval	noErr	The items are equivalent. Any other value means
					not equivalent.											*/

OSStatus
FSObject::CompareTo(
	const FSRef&	inFSRef) const
{
	return (mState == ref_Exists) ? ::FSCompareFSRefs(&mFSRef, &inFSRef)
								  : errFSRefsDifferent;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetName															  [public]
/**
	Returns the name of the item of the file system item
	
	@return	Name of the file system item									*/

CFString
FSObject::GetName() const
{
	CFString fileName;

	if (mState == ref_Exists) {
		HFSUniStr255 hfsName;
		OSStatus err = ::FSGetFileDirName(&mFSRef, &hfsName);
		PPx_ThrowIfOSError_(err, "FSGetFileDirName failed");

		fileName = CFString(hfsName.unicode, hfsName.length);
		
	} else if (mState == ref_ParentAndName) {
		fileName = mName;
		
	} else if (mState == ref_Invalid) {
		PPx_ThrowOSError_(errFSBadFSRef, "File object has an invalid FSRef");
	}

	return fileName;
}


// ---------------------------------------------------------------------------
//	GetName															  [public]
/**
	Passes back the name of the file system item in a HFSUniStr255 variable
	
	@param	outName		Name of the file system item
*/

void
FSObject::GetName(HFSUniStr255 &outName) const
{
	if (mState == ref_Exists) {
		OSStatus err = ::FSGetFileDirName(&mFSRef, &outName);
		PPx_ThrowIfOSError_(err, "FSGetFileDirName failed");
		
	} else if (mState == ref_ParentAndName) {
		FSUtils::StringToHFSUniStr(mName, outName);
		
	} else {
		PPx_ThrowOSError_(errFSBadFSRef, "File object has no FSRef");
	}
}


// ---------------------------------------------------------------------------
//	GetPath															  [public]
/**
	Returns the path name for the file system item
	
	@param	inPathStyle		OS Path Style for URL (POSIX, HFS, Windows)
	
	@return	Path name for the file system item								*/

CFString
FSObject::GetPath(
	CFURLPathStyle	inPathStyle) const
{
	CFURL	fileURL = GetURL();
	
	return fileURL.GetFileSystemPath(inPathStyle);
}


// ---------------------------------------------------------------------------
//	GetVolume														  [public]
/**
	Returns the volume reference number for where the file system item resides
	
	@return Volume reference number for where the item resides
*/

FSVolumeRefNum
FSObject::GetVolume() const
{
	FSVolumeRefNum vRefNum = kFSInvalidVolumeRefNum;

	OSStatus err = ::FSGetVRefNum(&UseRef(), &vRefNum);
	PPx_ThrowIfOSError_(err, "FSGetVRefNum failed");

	return vRefNum;
}


// ---------------------------------------------------------------------------
//	GetParent														  [public]
/**
	Passes back the FSRef for the parent directory of the file system item
	
	@param	outParentRef	FSRef for the parent directory of the item		*/

void
FSObject::GetParent(
	FSRef&	outParentRef) const
{
	if (mState == ref_Exists) {
		OSStatus err = ::FSGetParentRef(&mFSRef, &outParentRef);
		PPx_ThrowIfOSError_(err, "FSGetParentRef failed");
		
	} else if (mState == ref_ParentAndName) {
		outParentRef = mFSRef;
		
	} else {
		PPx_ThrowOSError_(errFSBadFSRef, "File object has no FSRef");
	}
}


// ---------------------------------------------------------------------------
//	GetParent														  [public]
/**
	Passes back an FSObject for the parent directory of the file system item
	
	@param	outParent	FSObject for the parent of the file system item
*/

void
FSObject::GetParent(
	FSObject&	outParent) const
{
	FSRef parentRef;
	GetParent(parentRef);

	outParent.mFSRef = parentRef;
	outParent.mState = ref_Exists;
}


// ---------------------------------------------------------------------------
//	GetParentDirID													  [public]
/**
	Returns the directory ID of the parent of the file system item
	
	@return Direcctor ID of the parent of the item
*/

SInt32
FSObject::GetParentDirID() const
{
	FSRef parentRef;
	GetParent(parentRef);

	FSCatalogInfo catInfo;
	OSStatus err = ::FSGetCatalogInfo(&parentRef, kFSCatInfoNodeID,
										&catInfo, nil, nil, nil);
	PPx_ThrowIfOSError_(err, "FSGetCatalogInfo failed");

	return catInfo.nodeID;
}


// ---------------------------------------------------------------------------
//	GetFSSpec														  [public]
/**
	Passes back an FSSpec for the file system item
	
	@param	outFSSpec		FSSpec for the item
	@param	inNameEncoding	Encoding to use for the file name
*/

void
FSObject::GetFSSpec(
	FSSpec&				outFSSpec,
	CFStringEncoding	inNameEncoding) const
{
	if (mState == ref_Exists) {
		OSStatus err = ::FSGetCatalogInfo(&mFSRef, kFSCatInfoNone, nil,
											nil, &outFSSpec, nil);
		PPx_ThrowIfOSError_(err, "FSGetCatalogInfo failed");
		
	} else if (mState == ref_ParentAndName) {
		FSCatalogInfoBitmap catInfoBM = kFSCatInfoVolume | kFSCatInfoNodeID;
		FSCatalogInfo catInfo;
		GetCatalogInfo(catInfoBM, catInfo);

		mName.GetPascalString(outFSSpec.name, sizeof(outFSSpec.name),
								inNameEncoding);
		outFSSpec.vRefNum = catInfo.volume;
		outFSSpec.parID   = catInfo.nodeID;
		
	} else {
		PPx_ThrowOSError_(errFSBadFSRef, "File object has no FSRef");
	}
}


// ---------------------------------------------------------------------------
//	GetURL															  [public]
/**
	Returns the URL for the FSObject
	
	@return The URL for the FSObject
	
	@note The URL will have a nil reference (be invalid) if the FSObject
	is invalid																*/

CFURL
FSObject::GetURL() const
{
	CFURL	theURL;
	
	if (mState == ref_Exists) {
		theURL = CFURL(mFSRef);
		
	} else if (mState == ref_ParentAndName) {
		CFURL	baseURL(mFSRef);
		theURL = CFURL(mName, baseURL);
	}
	
	return theURL;
}


// ---------------------------------------------------------------------------
//	GetCatalogInfo													  [public]
/**
	Pass back file system catalog information for the item
	
	@param	inWhichInfo		Bit flags specifying which information to get
	@param	outCatInfo		FSCatalogInfo struct filled in with requested info
*/

void
FSObject::GetCatalogInfo(
	FSCatalogInfoBitmap		inWhichInfo,
	FSCatalogInfo&			outCatInfo) const
{
	OSStatus err = ::FSGetCatalogInfo(&UseRef(), inWhichInfo, &outCatInfo,
										nil, nil, nil);
	PPx_ThrowIfOSError_(err, "FSGetCatalogInfo failed");
}


// ---------------------------------------------------------------------------
//	SetCatalogInfo													  [public]
/**
	Sets file system catalog information for the item
	
	@param	inWhichInfo		Bit flags specifying which information to set
	@param	inCatInfo		FSCatalogInfo struct containing data to set		*/

void
FSObject::SetCatalogInfo(
	FSCatalogInfoBitmap		inWhichInfo,
	const FSCatalogInfo&	inCatInfo)
{
	OSStatus err = ::FSSetCatalogInfo(&GetRef(), inWhichInfo, &inCatInfo);
	PPx_ThrowIfOSError_(err, "FSSetCatalogInfo failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	IsFile															  [public]
/**
	Returns whether the item is a file
	
	@return Whether the item is a file
*/

bool
FSObject::IsFile() const
{
	FSCatalogInfo catInfo;
	GetCatalogInfo(kFSCatInfoNodeFlags, catInfo);

	return ((catInfo.nodeFlags & kFSNodeIsDirectoryMask) == 0);
}


// ---------------------------------------------------------------------------
//	IsFolder														  [public]
/**
	Returns whether the item is a folder
	
	@return Whether the item is a folder
*/

bool
FSObject::IsFolder() const
{
	FSCatalogInfo catInfo;
	GetCatalogInfo(kFSCatInfoNodeFlags, catInfo);

	return ((catInfo.nodeFlags & kFSNodeIsDirectoryMask) != 0);
}


// ---------------------------------------------------------------------------
//	CheckLock														  [public]
/**
	Returns the locked state of the file system item
	
	@return Locked state of the file system item
	
	@retval	noErr		Unlocked
	@retval	fLckdErr	Item is locked
	@retval	wPrErr		Item is on a read-only value
	@retval	vLckdErr	Item is on a locked volume
*/

OSStatus
FSObject::CheckLock() const
{
	OSStatus lockStatus = ::FSCheckLock(&UseRef());

				// If the result is not one of the recognized lock states,
				// throw an exception
	if ( (lockStatus != noErr) and
		 (lockStatus != fLckdErr) and
		 (lockStatus != wPrErr) and
		 (lockStatus != vLckdErr) ) {
		PPx_Throw_(OSError, lockStatus, "FSCheckLock failed");
	}

	return lockStatus;
}


// ---------------------------------------------------------------------------
//	SetIsLocked														  [public]
/**
	Set the locked state of the file system item
	
	@param	inLock	Whether to lock or unlock the item
*/

void
FSObject::SetIsLocked(
	bool inLock)
{
									// Get all of the node flags
	FSCatalogInfo catInfo;
	GetCatalogInfo(kFSCatInfoNodeFlags, catInfo);

									// Decide if locked state differs
									//   from the desired locked state
	bool needsChange = false;
	if (inLock) {
		needsChange = ((catInfo.nodeFlags & kFSNodeLockedMask) == 0);
		if (needsChange) {
									// The item is unlocked and needs to
									//   be locked
			catInfo.nodeFlags |= kFSNodeLockedMask;
		}
			
	} else {
		needsChange = ((catInfo.nodeFlags & kFSNodeLockedMask) != 0);
		if (needsChange) {
									// The item is locked and needs to
									//   be unlocked
			catInfo.nodeFlags &= ~kFSNodeLockedMask;
		}
	}

	if (needsChange) {				// Change locked state
		SetCatalogInfo(kFSCatInfoNodeFlags, catInfo);
	}
}

#pragma mark -


// ---------------------------------------------------------------------------
//	GetFinderInfo													  [public]
/**
	Psses back the Finder information for the item
	
	@param	outFinderInfo		Pointer to FinderInfo struct
	@param	outExtFinderInfo	Pointer to ExtendedFinderInfo struct
	@param	outIsFolder			Whether the item is a folder
	
	For any of the parameters, pass nil if you do not want that piece
	of infomration.															*/
	
void
FSObject::GetFinderInfo(
	FinderInfo*				outFinderInfo,
	ExtendedFinderInfo*		outExtFinderInfo,
	bool*					outIsFolder)  const
{
								// Boolean vs bool semantics
	Boolean		isFolder;
	Boolean*	isFolderPtr = (outIsFolder == nil) ? nil : &isFolder;

	OSStatus err = ::FSGetFinderInfo(&UseRef(), outFinderInfo,
										outExtFinderInfo, isFolderPtr);
	PPx_ThrowIfOSError_(err, "FSGetFinderInfo failed");
	
	if (outIsFolder != nil) {
		*outIsFolder = isFolder;
	}
}


// ---------------------------------------------------------------------------
//	SetFinderInfo													  [public]
/**
	Sets the Finder information for the item
	
	@param	inFinderInfo		Pointer to FinderInfo struct
	@param	inExtFinderInfo		Pointer to ExtendedFinderInfo struct
	
	For any of the parameters, pass nil if you do not want to set that
	piece of infomration.													*/

void
FSObject::SetFinderInfo(
	const FinderInfo*			inFinderInfo,
	const ExtendedFinderInfo*	inExtFinderInfo)
{
	OSStatus err = ::FSSetFinderInfo(&GetRef(), inFinderInfo, inExtFinderInfo);
	PPx_ThrowIfOSError_(err, "FSSetFinderInfo failed");
}


// ---------------------------------------------------------------------------
//	GetFinderFlags													  [public]
/**
	Returns the Finder flags for the item
	
	@return Finder flags for the item										*/

UInt16
FSObject::GetFinderFlags() const
{
	FinderInfo finderInfo;
	GetFinderInfo(&finderInfo);

	return finderInfo.file.finderFlags;
}


// ---------------------------------------------------------------------------
//	ChangeFinderFlags												  [public]
/**
	Changes the Finder flags for the item
	
	@param	inSetFlags			If true, set the designated flags.
								If false, clear the designate flags
	@param	inFlagsToChange		Bit mask designating which flags to change	*/

void
FSObject::ChangeFinderFlags(
	bool		inSetFlags,
	UInt32		inFlagsToChange)
{
	OSStatus err =  ::FSChangeFinderFlags(&GetRef(), inSetFlags, inFlagsToChange);
	PPx_ThrowIfOSError_(err, "FSChangeFinderFlags failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Rename															  [public]
/**
	Changes the name of the file system item
	
	@param	inName			New name for the item as a HFSUniStr255
	@param	inEncodingHint	Suggested text encoding to use when converting
								the name from unicode to another encoding	*/

void
FSObject::Rename(
	const HFSUniStr255&		inName,
	TextEncoding			inEncodingHint)
{
	if (mState == ref_Exists) {
		FSRef renamedRef;
		OSStatus err = ::FSRenameUnicode(&mFSRef, inName.length,
								inName.unicode, inEncodingHint, &renamedRef);
		PPx_ThrowIfOSError_(err, "FSRenameUnicode failed");

		mFSRef = renamedRef;
		
	} else if (mState == ref_ParentAndName) {
		mName = CFString(inName.unicode, inName.length);
		
	} else {
		PPx_ThrowOSError_(errFSBadFSRef, "File object has no FSRef");
	}
}


// ---------------------------------------------------------------------------
//	Rename															  [public]
/**
	Changes the name of the file system item
	
	@param	inName			New name for the item as a CFString
	@param	inEncodingHint	Suggested text encoding to use when converting
								the name from unicode to another encoding	*/

void
FSObject::Rename(
	const CFString&		inName,
	TextEncoding		inEncodingHint)
{
	HFSUniStr255	newName;
	FSUtils::StringToHFSUniStr(inName, newName);
	Rename(newName, inEncodingHint);	
}


// ---------------------------------------------------------------------------
//	Delete															  [public]
/**
	Deletes the file system item
	
	@note The actual file system item is deleted, but the FSObject remains
	valid so you can use it recreate the item on disk at a later time		*/

void
FSObject::Delete()
{
	FSRef*	fsRef = &GetRef();		// Throws if FSRes does not exist

									// Save the parent and name
	HFSUniStr255	fileName;
	FSRef			parentRef;
	OSStatus err = ::FSGetCatalogInfo(fsRef, kFSCatInfoNone, nil,
										&fileName, nil, &parentRef);
	PPx_ThrowIfOSError_(err, "FSGetCatalogInfo failed");

									// Delete the item on disk
	err = ::FSDeleteObject(fsRef);
	PPx_ThrowIfOSError_(err, "FSDeleteObject failed");
	
	mFSRef	= parentRef;			// Internal state is now parent/name
	mName	= CFString(fileName);
	mState	= ref_ParentAndName;
}


// ---------------------------------------------------------------------------
//	DeleteContainer													  [public]
/**
	Deletes the file system item and all its contained files and folders
	
	@note Throws an exception if the item is not a directory
	
	@note The actual file system item is deleted, but the FSObject remains
	valid so you can use it recreate the item on disk at a later time		*/

void
FSObject::DeleteContainer()
{
	FSRef*	fsRef = &GetRef();		// Throws if FSRes does not exist

									// Save the parent and name
	HFSUniStr255	fileName;
	FSRef			parentRef;
	OSStatus err = ::FSGetCatalogInfo(fsRef, kFSCatInfoNone, nil,
										&fileName, nil, &parentRef);
	PPx_ThrowIfOSError_(err, "FSGetCatalogInfo failed");

									// Delete the container on disk
	err = ::FSDeleteContainer(fsRef);
	PPx_ThrowIfOSError_(err, "FSDeleteContainer failed");
	
	mFSRef	= parentRef;			// Internal state is now parent/name
	mName	= CFString(fileName);
	mState	= ref_ParentAndName;
}


// ---------------------------------------------------------------------------
//	DeleteContainerContents											  [public]
/**
	Deletes the files and folders contained within the item
	
	@note Throws an exception if the item is not a directory
*/

void
FSObject::DeleteContainerContents()
{
	OSStatus err = ::FSDeleteContainerContents(&GetRef());
	PPx_ThrowIfOSError_(err, "FSDeleteContainerContents failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Update															  [public]
/**
	Queries the file system to update the internal state of the FSObject
	
	Call if you think that the file system item may have been deleted or
	created by means external to this object. For example, via the actions
	of the user in the Finder.												*/

void
FSObject::Update()
{
	bool	isInvalid = false;
	OSStatus err;

	if (mState == ref_Exists) {
	
			// The fastest way to check if an FSRef still exists is to
			// call FSGetCatalogInfo with no parameters
			
		err = ::FSGetCatalogInfo(&mFSRef, kFSCatInfoNone, nil, nil, nil, nil);
		if (err != noErr) {
			Invalidate();
		}
		
	} else if (mState == ref_ParentAndName) {
		HFSUniStr255	hfsName;
		FSUtils::StringToHFSUniStr(mName, hfsName);
		
		if (hfsName.length == 0) {		// Zero lenght name is invalid
			Invalidate();
			
		} else {						// Make FSRef for parent/name
			FSRef	newRef;
			err = ::FSMakeFSRefUnicode(&mFSRef, hfsName.length,
						hfsName.unicode, kCFStringEncodingUnicode, &newRef);
						
			if (err == noErr) {			// Success. Changes state from
				mFSRef = newRef;		//   parent/name to exists
				mName.FreeRef();
				mState = ref_Exists;
				
			} else {					// Validate parent FSRef
			
				err = ::FSGetCatalogInfo(&mFSRef, kFSCatInfoNone,
											nil, nil, nil, nil);
				if (err != noErr) {
					Invalidate();
				}
			}
		}
	}
}


// -----------------------------------------------------------------------
//	Invalidate													 [private]
/**
	Invalidates the state of the FSObject
	
	Call if you know that the file system item no longer exists through
	some means external to this object. For example, if you delete the
	directory containing the item.										*/

void
FSObject::Invalidate()
{
	mName.FreeRef();
	mState = ref_Invalid;
}

#pragma mark -

// -----------------------------------------------------------------------
//	GetRef														 [private]
/**
	Returns a non-const reference to the FSRef. Throws if the FSRef
	does not exist.
	
	@return	Reference to FSRef for the FSObject							*/

FSRef&
FSObject::GetRef()
{
	if (mState != ref_Exists) {
		PPx_ThrowOSError_(errFSBadFSRef, "File object has no FSRef");
	}

	return mFSRef;
}


// -----------------------------------------------------------------------
//	InitFromVolDirName											 [private]
/**
	Initializes from a volume refnum, parent directory ID,
	and HFSUniStr255 item name
	
	@param	inVRefNum		Volume reference number
	@param	inParentDirID	Parent directory ID
	@param	inName			Name of the item as a HFSUniStr255
	
	@note The entity referred to by the parent and name does not have to
	currently exist															*/

void
FSObject::InitFromVolDirName(
	FSVolumeRefNum			inVRefNum,
	SInt32					inParentDirID,
	const HFSUniStr255&		inName)
{
	mState = ref_Invalid;

	if (inName.length == 0) return;
	
		// Convert inVRefNum and inParentDirID to a parent FSRef
		
	FSRef		parentRef;
	FSRefParam	parentParams;
	::BlockZero(&parentParams, sizeof(FSRefParam));
	
	parentParams.ioNamePtr	= pString_Empty;
	parentParams.ioVRefNum	= inVRefNum;
	parentParams.ioDirID	= inParentDirID;
	parentParams.newRef		= &parentRef;
	
	OSStatus err = ::PBMakeFSRefSync(&parentParams);
	
	if (err == noErr) {					// Conversion succeeded. Check for
										// a file or folder named inName
										// in the parent ref.
		err = ::FSMakeFSRefUnicode(&parentRef, inName.length, inName.unicode,
									kCFStringEncodingUnicode, &mFSRef);

		if (err == noErr) {				// A file or folder with the name
										//   was found. We have a valid
										//   FSRef.
			mState = ref_Exists;
			
		} else {						// No item with the name, but the
										//   parent FSRef is valid
			mName = CFString(inName);
			mState = ref_ParentAndName;
			mFSRef = parentRef;
		}
	}
}


// ---------------------------------------------------------------------------
//	InitFromURL														 [private]
/**
	Initializes from a Core Foundation URL reference
	
	@param	inURLRef	CF URL descrbing a file system entity				*/

void
FSObject::InitFromURL(
	CFURLRef	inURLRef)
{
	mState = ref_Invalid;
	
	if (inURLRef == nil) return;
	
	CFURL	theURL(inURLRef, retain_Yes);
	
									// Try to convert the URL to an FSRef
	if (theURL.GetFSRef(mFSRef)) {
	
		mState = ref_Exists;		// Success. Our FSRef is valid.
	
	} else {						// Check if URL is a valid parent/name
	
									// Item name is the last part of the URL
		mName = theURL.GetLastPathComponent();
	
		if (mName.IsValid()) {		// Strip off item name from URL and see
									//   if what's left is a valid directory
			theURL.DeleteLastPathComponent();
			
			if (theURL.GetFSRef(mFSRef)) {	// We have a valid parent
				mState = ref_ParentAndName;
			}
		}
	}
}


} // namespace PPx