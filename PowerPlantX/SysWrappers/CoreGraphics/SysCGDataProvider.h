// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCGDataProvider.h
//
//	Copyright © 2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCGDataProvider.h
	@brief		Wrapper class for Core Graphics Data Provider
*/

#ifndef H_SysCGDataProvider
#define H_SysCGDataProvider
#pragma once

#include <SysCFObject.h>

#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	CGDataProvider
/**
	Wrapper class for Core Graphics Data Provider */

class	CGDataProvider : public CFObject<CGDataProviderRef> {
public:			
									// Constructors
						CGDataProvider();
						
						CGDataProvider(
								CGDataProviderRef	inProviderRef,
								bool				inRetain = true);
						
	explicit			CGDataProvider(
								const CGDataProviderCallbacks&	inCallbacks,
								void*							inUserData = nil);

	explicit			CGDataProvider(
								void*	inData,
								size_t	inSize,
								void	(*inReleaseDataFunc)(void* inUserData, const void* inData, size_t inSize) = nil,
								void*	inUserData = nil);

	explicit			CGDataProvider( CFURLRef inURL );

						CGDataProvider( const CGDataProvider& inOriginal );
						
	CGDataProvider&		operator = ( const CGDataProvider& inSource );
	
private:	
	typedef CFObject<CGDataProviderRef>	BaseT;
};


} // namespace PPx

#endif // H_SysCGDataProvider
