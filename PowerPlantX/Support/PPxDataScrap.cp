// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataScrap.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDataScrap.h>
#include <SysScrap.h>

namespace PPx {

namespace {	// unnamed

	// -----------------------------------------------------------------------
	//	Toolbox callback function for Scrap Promise Keepers

	pascal OSStatus
	ScrapPromiseKeeperCallback(
		ScrapRef			inScrap,
		ScrapFlavorType		inFlavor,
		void*				inUserData)
	{
		OSStatus	result = scrapPromiseNotKeptErr;
		
		try {
			ScrapPromiseKeeper*	promiser =
							static_cast<ScrapPromiseKeeper*>(inUserData);
							
			result = promiser->Invoke(inScrap, inFlavor);
		}
		
		catch (...) { }
		
		return result;
	}


	// -----------------------------------------------------------------------
	//	Returns UPP to Scrap Promise Keeper callback function
	
	ScrapPromiseKeeperUPP
	GetSysScrapPromiseKeeperUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static	SysScrapPromiseKeeperUPP
						sScrapPromiseKeeperUPP(ScrapPromiseKeeperCallback);
						
		return sScrapPromiseKeeperUPP.Get();
	}
	
} // unnamed namespace

#pragma mark -

// ---------------------------------------------------------------------------
//	DataScrap
/**
	Constructs from a scrap name *
	
	@param	inScrapName		Name to identify the scrap
	
	Scraps are system-wide entities. Other programs can get data from
	scrap if they know its name.											*/
	
DataScrap::DataScrap(
	CFStringRef	inScrapName)
	
	: mScrapName(inScrapName)
{
	mPromiseKeeper = nil;
}


// ---------------------------------------------------------------------------
//	GetData
/**
	Get data of the specified flavor from the scrap
	
	@param	inFlavor		Flavor of data to get
	@param	ioByteCount		On input, maximum bytes to get;
							On output, actual bytes returned
	@param	outDataPtr		Pointer to data buffer
	
	Call GetDataSize if you need to know the size of the data before
	getting it.																*/

void
DataScrap::GetData(
	ScrapFlavorType		inFlavor,
	Size&				ioByteCount,
	void*				outDataPtr) const
{
	SysScrap::GetData(mScrapName, inFlavor, ioByteCount, outDataPtr);
}


// ---------------------------------------------------------------------------
//	GetDataSize
/**
	Returns the size of the specified flavor of data in the scrap
	
	@param	inFlavor	Flavor of data
	
	@return	Size in bytes of the data										*/

SInt32
DataScrap::GetDataSize(
	ScrapFlavorType		inFlavor) const
{
	return SysScrap::GetDataSize(mScrapName, inFlavor);
}


// ---------------------------------------------------------------------------
//	HasData
/**
	Returns whether the scrap has data of the specified flavor
	
	@param	inFlavor	Flavor of data
	
	@return	Whether the scrap has data of the specified flavor				*/

bool
DataScrap::HasData(
	ScrapFlavorType		inFlavor) const
{
	return SysScrap::HasData(mScrapName, inFlavor);
}


// ---------------------------------------------------------------------------
//	ClearData
/**
	Clears all data from the scrap */

void
DataScrap::ClearData()
{
	return SysScrap::ClearData(mScrapName);
}


// ---------------------------------------------------------------------------
//	SetData
/**
	Put data into the scrap
	
	@param	inFlavor		Flavor of data
	@param	inDataSize		Number of bytes of data
	@param	inDataPtr		Pointer to data buffer
	@param	inFlags			Options for storing data
	@param	inClear			Whether to clear the scrap before adding data	*/

void
DataScrap::SetData(
	ScrapFlavorType		inFlavor,
	Size				inDataSize,
	const void*			inDataPtr,
	ScrapFlavorFlags	inFlags,
	bool				inClear)
{
	SysScrap::SetData(mScrapName, inFlavor, inDataSize, inDataPtr,
						inFlags, inClear);
}


// ---------------------------------------------------------------------------
//	PromiseData
/**
	Put a promise to supply data into the scrap
	
	@param	inFlavor		Flavor of data
	@param	inDataSize		Number of bytes of data
	@param	inFlags			Options for storing data
	@param	inClear			Whether to clear the scrap before adding data
	
	The system will caller your promise keeper function if a client
	requests the flavor data from the scrap.
	
	If you do not know how much data there is, pass -1 for inDataSize		*/

void
DataScrap::PromiseData(
	ScrapFlavorType		inFlavor,
	Size				inDataSize,
	ScrapFlavorFlags	inFlags,
	bool				inClear)
{
	SysScrap::PromiseData(mScrapName, inFlavor, inDataSize, inFlags, inClear);
}


// ---------------------------------------------------------------------------
//	SetPromiseKeeper
/**
	Specify the promise keeper object that will supply promised data
	
	@param	inPromiseKeeper		Pointer to promise keeper object
	
	ScrapPromiseKeeper is an abstract base class. You must create a
	subclass and override the KeepScrapPromise function. Then pass
	a pointer to an object of your subclass to this function.				*/

void
DataScrap::SetPromiseKeeper(
	ScrapPromiseKeeper*	inPromiseKeeper)
{
		// Promise keeper may be nil. In that case, we pass a nil UPP
		// so the Toolbox won't make any callback.
						
	ScrapPromiseKeeperUPP	theUPP = nil;
	if (inPromiseKeeper != nil) {
		theUPP = GetSysScrapPromiseKeeperUPP();
	}
	
		// Pass pointer to ScrapPromiseKeeper object as the
		// user data for the Toolbox callback
	
	SysScrap::SetPromiseKeeper(mScrapName, theUPP, inPromiseKeeper);				
	
	mPromiseKeeper = inPromiseKeeper;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Instance
/**
	Returns a reference to the Clipboard scrap. The Clipboard is the
	standard Mac OS scrap for copying and pasting data between programs.
	
	@return Reference to the Clipboard scrap								*/

DataScrap&
Clipboard::Instance()
{
	static DataScrap	sClipboard(kScrapClipboardScrap);
	
	return sClipboard;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Instance
/**
	Returns a reference to the FindScrap. The FindScrap is a standard
	system scrap for remembering the text to search for in find
	operations.
	
	@return	Reference to the FindScrap
	
	The Find scrap is a standard feature of programs written with Cocoa		*/

DataScrap&
FindScrap::Instance()
{
	static DataScrap	sFindScrap(kScrapFindScrap);
	
	return sFindScrap;
}


} // namespace PPx