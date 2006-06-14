// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysScrap.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysScrap.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	GetNamedScrap
/**
	Returns the ScrapRef for the named scrap
	
	@param	inScrapName		Name of the scrap
	@param	inClear			Whether to clear the scrap
	
	@return	ScrapRef for the named scrap									*/

ScrapRef
SysScrap::GetNamedScrap(
	CFStringRef		inScrapName,
	bool			inClear)
{
	ScrapRef	scrap;
	OptionBits	options = inClear ? kScrapClearNamedScrap
								  : kScrapGetNamedScrap;
	
	OSStatus	status = ::GetScrapByName(inScrapName, options, &scrap);
	PPx_ThrowIfOSError_(status, "GetScrapByName failed");
	
	return scrap;
}
	

// ---------------------------------------------------------------------------
//	GetData
/**
	Gets flavor data from the named scrap
	
	@param	inScrapName		Name of the scrap
	@param	inFlavor		Flavor of data
	@param	ioByteCount		On input, maximum bytes to get;
							On output, actual bytes returned
	@param	outDataPtr		Pointer to data buffer
	
	Call GetDataSize if you need to know the size of the data before
	getting it.																*/

void
SysScrap::GetData(
	CFStringRef		inScrapName,
	ScrapFlavorType	inFlavor,
	Size&			ioByteCount,
	void*			outDataPtr)
{
	ScrapRef	scrap = GetNamedScrap(inScrapName);

	OSStatus	status = ::GetScrapFlavorData(scrap, inFlavor, &ioByteCount,
												outDataPtr);
	PPx_ThrowIfOSError_(status, "GetScrapFlavorData failed");
}


// ---------------------------------------------------------------------------
//	GetDataSize
/**
	Returns the size of the specified flavor of data in the scrap
	
	@param	inScrapName		Name of the scrap
	@param	inFlavor		Flavor of data
	
	@return	Size in bytes of the data										*/

Size
SysScrap::GetDataSize(
	CFStringRef		inScrapName,
	ScrapFlavorType	inFlavor)
{
	ScrapRef	scrap = GetNamedScrap(inScrapName);

	Size		dataSize;
	OSStatus	status = ::GetScrapFlavorSize(scrap, inFlavor, &dataSize);
	PPx_ThrowIfOSError_(status, "GetScrapFlavorSize failed");
	
	return dataSize;
}
	

// ---------------------------------------------------------------------------
//	HasData
/**
	Returns whether the scrap has data of the specified flavor
	
	@param	inScrapName		Name of the scrap
	@param	inFlavor		Flavor of data
	
	@return	Whether the scrap has data of the specified flavor				*/

bool
SysScrap::HasData(
	CFStringRef		inScrapName,
	ScrapFlavorType	inFlavor)
{
	ScrapRef	scrap = GetNamedScrap(inScrapName);

	ScrapFlavorFlags	flags;
	OSStatus	status = ::GetScrapFlavorFlags(scrap, inFlavor, &flags);
	
	return (status == noErr);
}
	

// ---------------------------------------------------------------------------
//	ClearData
/**
	Clears all data from the scrap */

void
SysScrap::ClearData(
	CFStringRef		inScrapName)
{
	GetNamedScrap(inScrapName, clear_Yes);
}
	

// ---------------------------------------------------------------------------
//	SetData
/**
	Puts data into the scrap
	
	@param	inScrapName		Name of the scrap
	@param	inFlavor		Flavor of data
	@param	inDataSize		Number of bytes of data
	@param	inDataPtr		Pointer to data buffer
	@param	inFlags			Options for storing data
	@param	inClear			Whether to clear the scrap before adding data	*/

void
SysScrap::SetData(
	CFStringRef			inScrapName,
	ScrapFlavorType		inFlavor,
	Size				inDataSize,
	const void*			inDataPtr,
	ScrapFlavorFlags	inFlags,
	bool				inClear)
{
	ScrapRef	scrap = GetNamedScrap(inScrapName, inClear);
	
	OSStatus	status = ::PutScrapFlavor(scrap, inFlavor, inFlags,
											inDataSize, inDataPtr);
	PPx_ThrowIfOSError_(status, "PutScrapFlavor failed");
}
	

// ---------------------------------------------------------------------------
//	PromiseData
/**
	Puts a promise to supply data into the scrap
	
	@param	inScrapName		Name of the scrap
	@param	inFlavor		Flavor of data
	@param	inDataSize		Number of bytes of data
	@param	inFlags			Options for storing data
	@param	inClear			Whether to clear the scrap before adding data
	
	The system will call your promise keeper function if a client
	requests the flavor data from the scrap.
	
	If you do not know how much data there is, pass -1 for inDataSize		*/

void
SysScrap::PromiseData(
	CFStringRef				inScrapName,
	ScrapFlavorType			inFlavor,
	Size					inDataSize,
	ScrapFlavorFlags		inFlags,
	bool					inClear)
{
	ScrapRef	scrap = GetNamedScrap(inScrapName, inClear);
	
	OSStatus	status = ::PutScrapFlavor(scrap, inFlavor, inFlags,
											inDataSize, nil);
	PPx_ThrowIfOSError_(status, "PutScrapFlavor failed");
}
	

// ---------------------------------------------------------------------------
//	SetPromiseKeeper
/**
	Specifies the promise keeper function that will supply promised data
	
	@param	inScrapName			Name of the scrap
	@param	inPromiseUPP		Pointer to promise keeper object
	@param	inUserData			Pointer to user-defined data				*/

void
SysScrap::SetPromiseKeeper(
	CFStringRef				inScrapName,
	ScrapPromiseKeeperUPP	inPromiseUPP,
	const void*				inUserData)
{
	ScrapRef	scrap = GetNamedScrap(inScrapName);
	
	OSStatus	status = ::SetScrapPromiseKeeper(scrap, inPromiseUPP,
													inUserData);
	PPx_ThrowIfOSError_(status, "PutScrapFlavor failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysScrapPromiseKeeperUPP				Constructor				  [public]

SysScrapPromiseKeeperUPP::SysScrapPromiseKeeperUPP(
	ScrapPromiseKeeperProcPtr	inCallbackFunc)
{
	mScrapPromiseKeeperUPP = ::NewScrapPromiseKeeperUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysScrapPromiseKeeperUPP				Destructor				  [public]

SysScrapPromiseKeeperUPP::~SysScrapPromiseKeeperUPP()
{
	::DisposeScrapPromiseKeeperUPP(mScrapPromiseKeeperUPP);
}

} // namespace PPx