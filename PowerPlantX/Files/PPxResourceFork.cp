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

#include <PPxResourceFork.h>
#include <PPxExceptions.h>


namespace PPx {

// ---------------------------------------------------------------------------
//	ResourceFork													  [public]
/**
	Constructs a ResourceFork object for an already open resource fork
	
	@param	inRefNum		Reference number for the resource fork
	@param	inOwnsRefNum	Whether this object should close the fork when
								finished									*/

ResourceFork::ResourceFork(
	SInt16	inRefNum,
	bool	inOwnsRefNum)
	
	: FileFork(inRefNum, inOwnsRefNum)
{
}


// ---------------------------------------------------------------------------
//	ResourceFork													  [public]
/**
	Constructs a ResourceFork object for a file and opens the fork
	
	@param	inFile			FSRef for a file
	@param	inPermissions	Access permissions								*/

ResourceFork::ResourceFork(
	const FSRef&	inFile,
	SInt8			inPermissions)
	
	: FileFork(inFile, inPermissions, GetForkName())
{
}


// ---------------------------------------------------------------------------
//	GetForkName												 [static] [public]
/**
	Returns the constant system name for the resource fork
	
	@return	Name of the resource fork										*/

const HFSUniStr255&
ResourceFork::GetForkName()
{
	static HFSUniStr255		sResourceForkName = { 0 };

	if (sResourceForkName.length == 0) {
		OSStatus status = ::FSGetResourceForkName(&sResourceForkName);
		PPx_ThrowIfOSError_(status, "FSGetResourceForkName failed");
	}

	return sResourceForkName;
}

} // namespace PPx
