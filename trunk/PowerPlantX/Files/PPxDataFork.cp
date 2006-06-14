// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataFork.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:18 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDataFork.h>
#include <limits>

namespace PPx {

// ---------------------------------------------------------------------------
//	DataFork														  [public]
/**
	Constructs a DataFork object for an already open data fork
	
	@param	inRefNum		Reference number for the data fork
	@param	inOwnsRefNum	Whether this object should close the fork when
								finished									*/

DataFork::DataFork(
	SInt16	inRefNum,
	bool	inOwnsRefNum)
	
	: FileFork(inRefNum, inOwnsRefNum)
{
}


// ---------------------------------------------------------------------------
//	DataFork														  [public]
/**
	Constructs a DataFork object for a file and opens the fork
	
	@param	inFile			FSRef for the file
	@param	inPermissions	Access permissions								*/

DataFork::DataFork(
	const FSRef&	inFile,
	SInt8			inPermissions)
	
	: FileFork(inFile, inPermissions, GetForkName())
{
}


// ---------------------------------------------------------------------------
//	ReadContents													  [public]
/**
	Reads entire contents of a data fork into a CFData object
	
	@return	CFData object containing the fork contents
	
	@note Since CFData uses signed 32-bit indexes, data length is limited
	to 2 Gigabytes
	
	@note The returned CFData object has variable capacity, so you can
	add more data afterwards												*/

CFData
DataFork::ReadContents()
{
	SInt64 fileSize = GetSize();
	if (fileSize > std::numeric_limits<CFIndex>::max()) {
		PPx_Throw_(RuntimeError, err_Runtime,
					"Length of file is larger than 2 GB");
	}

									// Create variable capacity CFData
	CFData contentsData(cfSize_Unlimited);
	contentsData.SetLength(fileSize);

	ReadData(contentsData.GetMutableBytePtr(), fileSize, fsFromStart, 0);

	return contentsData;
}


// ---------------------------------------------------------------------------
//	WriteContents													  [public]
/**
	Fills contents of a data fork with bytes from a CFData object
	
	@param	inData		The CFData object
	
	Any previous contents are overwritten and the fork size is set to
	the size of the CFData contents											*/

void
DataFork::WriteContents(
	const CFData&	inData)
{
	SInt64 fileSize = inData.GetLength();
	SetSize(fileSize);

	WriteData(inData.GetBytePtr(), fileSize, fsFromStart, 0);
}


// ---------------------------------------------------------------------------
//	WriteContents													  [public]
/**
	Fills contents of a data fork with bytes from a buffer
	
	@param	inBuffer		Pointer to the buffer
	@param	inBufferSize	Number of bytes in the buffer
	
	Any previous contents are overwritten and the fork size is set to
	the size of the buffer													*/

void
DataFork::WriteContents(
	const void*	inBuffer,
	ByteCount	inBufferSize)
{
	SetSize(inBufferSize);
	WriteData(inBuffer, inBufferSize, fsFromStart, 0);
}


// ---------------------------------------------------------------------------
//	ReadData														  [public]
/**
	Reads bytes from a data fork into a supplied buffer
	
	@param	inBuffer		Pointer to the buffer
	@param	inBufferSize	Capacity of the buffer
	@param	inPositionMode	Reference location in fork
	@param	inOffset		Offset from reference location at which to
								start reading								*/

void
DataFork::ReadData(
	void *inBuffer,
	ByteCount inBufferSize,
	UInt16 inPositionMode,
	SInt64 inOffset)
{
		// [Enhancement]  Actual bytes read is not accessible because
		// we throw if there's an error. Perhaps we can store the byte
		// count in the exception object.

	ByteCount bytesRead;
	OSStatus err = ::FSReadFork(UseRefNum(), inPositionMode, inOffset,
								inBufferSize, inBuffer, &bytesRead);
	PPx_ThrowIfOSError_(err, "FSReadFork failed");
}


// ---------------------------------------------------------------------------
//	WriteData														  [public]
/**
	Writes bytes from a buffer into a data fork
	
	@param	inBuffer		Pointer to bytes to write
	@param	inBufferSize	Number of bytes to write
	@param	inPositionMode	Reference location in fork
	@param	inOffset		Offset from reference location at which to
								start writing								*/

void
DataFork::WriteData(
	const void*	inBuffer,
	ByteCount	inBufferSize,
	UInt16		inPositionMode,
	SInt64		inOffset)
{
		// [Enhancement]  Actual bytes written is not accessible because
		// we throw if there's an error. Perhaps we can store the byte
		// count in the exception object.

	ByteCount bytesWritten;
	OSStatus err = ::FSWriteFork(UseRefNum(), inPositionMode, inOffset,
									inBufferSize, inBuffer, &bytesWritten);
	PPx_ThrowIfOSError_(err, "FSWriteFork failed");
}


// ---------------------------------------------------------------------------
//	GetForkName												 [static] [public]
/**
	Returns the constant system name for the data fork
	
	@return	Name of the data fork											*/

const HFSUniStr255&
DataFork::GetForkName()
{
	static HFSUniStr255			sDataForkName = { 0 };

	if (sDataForkName.length == 0) {
		OSStatus err = ::FSGetDataForkName(&sDataForkName);
		PPx_ThrowIfOSError_(err, "FSGetDataForkName failed");
	}

	return sDataForkName;
}


} // namespace PPx