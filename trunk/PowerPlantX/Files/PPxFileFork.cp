// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFileFork.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:18 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxFileFork.h>
#include <PPxFSObject.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	FileFork														  [public]
/**
	Constructs a FileFork object for a file and opens the fork

	@param	inFile			FSRef for the file
	@param	inPermissions	Access permissions
	@param	inForkName		Name of the fork								*/

FileFork::FileFork(
	const FSRef&		inFile,
	SInt8				inPermissions,
	const HFSUniStr255&	inForkName)
	
	: mRefNum(refNum_Invalid),
	  mOwnsRefNum(true)
{
	Open(inFile, inPermissions, inForkName);
}


// ---------------------------------------------------------------------------
//	FileFork														  [public]
/**
	Constructs from an already open file fork
	
	@param	inRefNum		Reference number for an open fork
	@param	inOwnsRefNum	Whether to close the fork upon destruction		*/

FileFork::FileFork(
	SInt16	inRefNum,
	bool	inOwnsRefNum)
	
	: mRefNum(inRefNum),
	  mOwnsRefNum(inOwnsRefNum)
{
}


// ---------------------------------------------------------------------------
//	~FileFork														  [public]
/**
	Destructor. Closes the fork if we own it.								*/

FileFork::~FileFork()
{
	Close();
}


// ---------------------------------------------------------------------------
//	UseRefNum														  [public]
/**
	Returns the fork's reference number
	
	@return	Fork's reference number											*/

SInt16
FileFork::UseRefNum() const
{
	PPx_SignalIf_(mRefNum == refNum_Invalid);

	return mRefNum;
}


// ---------------------------------------------------------------------------
//	IsOpen															  [public]
/**
	Returns whether the fork is open
	
	@return	Whether the fork is open										*/

bool
FileFork::IsOpen() const
{
	return (mRefNum != refNum_Invalid);
}


// ---------------------------------------------------------------------------
//	GetSize															  [public]
/**
	Returns the size of a file
	
	@return	Size of the file, in bytes										*/

SInt64
FileFork::GetSize() const
{
	SInt64		forkSize;
	OSStatus	err = ::FSGetForkSize(UseRefNum(), &forkSize);
	PPx_ThrowIfOSError_(err, "FSGetForkSize failed");

	return forkSize;
}


// ---------------------------------------------------------------------------
//	SetSize															  [public]
/**
	Sets the size of a file
	
	@param	inSize			Size in bytes from the reference location
	@param	inPositionMode	Reference location in fork						*/

void
FileFork::SetSize(
	SInt64		inSize,
	UInt16		inPositionMode)
{
	OSStatus	err = ::FSSetForkSize(UseRefNum(), inPositionMode, inSize);
	PPx_ThrowIfOSError_(err, "FSSetForkSize failed");
}


// ---------------------------------------------------------------------------
//	GetPosition														  [public]
/**
	Returns the current position in the fork
	
	@return Current position in the fork
	
	@note The reference location fsFromMark	refers to the current position	*/

SInt64
FileFork::GetPosition() const
{
	SInt64 forkPos;
	OSStatus err = ::FSGetForkPosition(UseRefNum(), &forkPos);
	PPx_ThrowIfOSError_(err, "FSGetForkPosition failed");

	return (forkPos);
}


// ---------------------------------------------------------------------------
//	SetPosition														  [public]
/**
	Sets the current position in the fork
	
	@param	inOffset		Bytes offset from the reference location
	@param	inPositionMode	Reference location within the file
	
	@note The reference location fsFromMark	refers to the current position	*/

void
FileFork::SetPosition(
	SInt64	inOffset,
	UInt16	inPositionMode)
{
	OSStatus err = ::FSSetForkPosition(UseRefNum(),inPositionMode, inOffset);
	PPx_ThrowIfOSError_(err, "FSSetForkPosition failed");
}


// ---------------------------------------------------------------------------
//	GetFSRef														  [public]
/**
	Passes back the FSRef for the file containing the fork
	
	@param	outRef		FSRef for the fork's file							*/

void
FileFork::GetFSRef(
	FSRef&	outRef) const
{
	OSStatus err = ::FSGetForkCBInfo(UseRefNum(), 0, nil, nil, nil,
									 &outRef, nil);
	PPx_ThrowIfOSError_(err, "FSGetForkCBInfo failed");
}


// ---------------------------------------------------------------------------
//	GetFSObject														  [public]
/**
	Passes back the FSObject for the file containing the fork
	
	@param	outFSObject	FSObject for the fork's file						*/

void
FileFork::GetFSObject(
	FSObject&	outFSObject) const
{
	FSRef fileRef;
	GetFSRef(fileRef);

	outFSObject = fileRef;
}


// ---------------------------------------------------------------------------
//	GetForkName														  [public]
/**
	Passes back the name of the fork
	
	@param	outForkName		Name of the fork								*/

void
FileFork::GetForkName(
	HFSUniStr255&	outForkName) const
{
	OSStatus err = ::FSGetForkCBInfo(UseRefNum(), 0, nil, nil, nil, nil,
									&outForkName);
	PPx_ThrowIfOSError_(err, "FSGetForkCBInfo failed");
}


// ---------------------------------------------------------------------------
//	GetForkInfo														  [public]
/**
	Passes back the FSForkInfo struct for the fork
	
	@param	outForkInfo		FSForkInfo struct to fill in					*/

void
FileFork::GetForkInfo(
	FSForkInfo&	outForkInfo) const
{
	OSStatus err = ::FSGetForkCBInfo(UseRefNum(), 0, nil, nil,
										&outForkInfo, nil, nil);
	PPx_ThrowIfOSError_(err, "FSGetForkCBInfo failed");
}


// ---------------------------------------------------------------------------
//	Open															  [public]
/**
	Opens a file fork with the specified permissions
	
	@param	inFile			FSRef for the file
	@param	inPermissions	Access permissions
	@param	inForkName		Name of the fork to open
	
	@return	File reference number for accessing the fork					*/

SInt16
FileFork::Open(
	const FSRef&			inFile,
	SInt8					inPermissions,
	const HFSUniStr255&		inForkName)
{
	if (mRefNum != refNum_Invalid) {
		PPx_Throw_(LogicError, err_Logic, "Fork is already open");
	}

	OSStatus err = ::FSCreateFork(&inFile, inForkName.length, inForkName.unicode);
	if ((err != noErr) && (err != errFSForkExists)) {
		PPx_Throw_(OSError, err, "FSCreateFork failed");
	}

	SInt16 forkRefNum;
	err = ::FSOpenFork(&inFile, inForkName.length, inForkName.unicode,
						inPermissions, &forkRefNum);
	PPx_ThrowIfOSError_(err, "FSOpenFork failed");

	mRefNum = forkRefNum;
	return mRefNum;
}


// ---------------------------------------------------------------------------
//	Close															  [public]
/**
	Close a file fork */

void
FileFork::Close()
{
	if (mOwnsRefNum && (mRefNum != refNum_Invalid)) {
		OSStatus err = ::FSCloseFork(mRefNum);
		
			// Ignroe error. There's not much to do if closing fails.

		mRefNum = refNum_Invalid;
	}
}


// ---------------------------------------------------------------------------
//	Flush															  [public]
/**
	Flushes the fork cotents to disk										*/

void
FileFork::Flush()
{
	::FSFlushFork(UseRefNum());
}


} // namespace PPx