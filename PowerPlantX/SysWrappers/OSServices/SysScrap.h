// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysScrap.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysScrap.h
	@brief		Wrapper functions for the Scrap Manager
*/

#ifndef H_SysScrap
#define H_SysScrap
#pragma once

#include <PPxPrefix.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysScrap
/**
	Wrapper functions for the Scrap Manager */

namespace	SysScrap {

	ScrapRef		GetNamedScrap(
							CFStringRef 	inScrapName,
							bool			inClear = clear_No);

	void			GetData(
							CFStringRef		inScrapName,
							ScrapFlavorType	inFlavor,
							Size&			ioByteCount,
							void*			outDataPtr);
								
	Size			GetDataSize(
							CFStringRef		inScrapName,
							ScrapFlavorType	inFlavor);
								
	bool			HasData(
							CFStringRef		inScrapName,
							ScrapFlavorType	inFlavor);

	void			ClearData( CFStringRef inScrapName );
	
	void			SetData(
							CFStringRef			inScrapName,
							ScrapFlavorType		inFlavor,
							Size				inDataSize,
							const void*			inDataPtr,
							ScrapFlavorFlags	inFlags = kScrapFlavorMaskNone,
							bool				inClear = clear_Yes);
								
	void			PromiseData(
							CFStringRef				inScrapName,
							ScrapFlavorType			inFlavor,
							Size					inDataSize = kScrapFlavorSizeUnknown,
							ScrapFlavorFlags		inFlags = kScrapFlavorMaskNone,
							bool					inClear = clear_Yes);
							
	void			SetPromiseKeeper(
							CFStringRef				inScrapName,
							ScrapPromiseKeeperUPP	inPromiseUPP,
							const void*				inUserData);
}


// ===========================================================================
//	SysScrapPromiseKeeperUPP
/**
	Wrapper class for a Scrap Promise Keeper callback function UPP */

class	SysScrapPromiseKeeperUPP {
public:
	explicit			SysScrapPromiseKeeperUPP(
								ScrapPromiseKeeperProcPtr	inCallbackFunc);
	
						~SysScrapPromiseKeeperUPP();
						
	ScrapPromiseKeeperUPP
						Get() const;
						
private:
	ScrapPromiseKeeperUPP	mScrapPromiseKeeperUPP;	/**< Callback function UPP */
	
private:									// Forbid copy and assignment
						SysScrapPromiseKeeperUPP( const SysScrapPromiseKeeperUPP& );
	SysScrapPromiseKeeperUPP&	operator = ( const SysScrapPromiseKeeperUPP& );
};


// ---------------------------------------------------------------------------
//	Get
/**
	Returns the UPP for a scrap promise keeper callback function
	
	@return UPP for a scrap promise keeper callback function				*/

inline
ScrapPromiseKeeperUPP
SysScrapPromiseKeeperUPP::Get() const
{
	return mScrapPromiseKeeperUPP;
}

} // namespace PPx

#endif	// H_SysScrap
