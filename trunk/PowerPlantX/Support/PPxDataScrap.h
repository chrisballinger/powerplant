// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataScrap.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDataScrap.h
	@brief		Classes for managing scraps which store and retrieve data
*/

#ifndef H_PPxDataScrap
#define H_PPxDataScrap
#pragma once

#include <SysCFString.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	ScrapPromiseKeeper
/**
	Abstract class for keeping promises to supply data for a scrap */

class	ScrapPromiseKeeper {
public:

	virtual				~ScrapPromiseKeeper()	{ }
	
	OSStatus			Invoke(
								ScrapRef		inScrap,
								ScrapFlavorType	inFlavor);

private:								
	virtual OSStatus	KeepScrapPromise(
								ScrapRef		inScrap,
								ScrapFlavorType	inFlavor) = 0;
};


// ---------------------------------------------------------------------------
//	Invoke
/**
	Non-virtual wrapper function for invoking the KeepScrapPromise function
	
	@param	inScrap		Scrap reference
	@param	inFlavor	Flavor of data										*/

inline OSStatus
ScrapPromiseKeeper::Invoke(
	ScrapRef		inScrap,
	ScrapFlavorType	inFlavor)
{
	return KeepScrapPromise(inScrap, inFlavor);
}


// ===========================================================================
//	DataScrap
/**
	A named scrap for storing and retrieving data */

class	DataScrap {
public:
						DataScrap( CFStringRef inScrapName );
						
	void				GetData(
								ScrapFlavorType		inFlavor,
								Size&				ioByteCount,
								void*				outDataPtr) const;
								
	Size				GetDataSize( ScrapFlavorType inDataType ) const;
	
	bool				HasData( ScrapFlavorType inDataType ) const;
								
	void				ClearData();
	
	void				SetData(
								ScrapFlavorType		inFlavor,
								Size				inDataSize,
								const void*			inDataPtr,
								ScrapFlavorFlags	inFlags = kScrapFlavorMaskNone,
								bool				inClear = true);
								
	void				PromiseData(
								ScrapFlavorType		inFlavor,
								Size				inDataSize = kScrapFlavorSizeUnknown,
								ScrapFlavorFlags	inFlags = kScrapFlavorMaskNone,
								bool				inClear = true);
	
	void				SetPromiseKeeper( ScrapPromiseKeeper* inPromiseKeeper );
	
private:
	CFString				mScrapName;
	ScrapPromiseKeeper*		mPromiseKeeper;
};


// ===========================================================================
//	Clipboard
/**
	Special instance of a DataScrap for the system Clipboard */

namespace Clipboard {

	DataScrap&			Instance();
}


// ===========================================================================
//	FindScrap
/**
	Special instance of a DataScrap for the system FindScrap */

namespace FindScrap {

	DataScrap&			Instance();
}

} // namespace PPx

#endif	// H_PPxDataScrap
