// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCGColorSpace.h
//
//	Copyright © 2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCGColorSpace.h
	@brief		Wrapper class for Core Graphics Color Space
*/

#ifndef H_SysCGColorSpace
#define H_SysCGColorSpace
#pragma once

#include <SysCFObject.h>

#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	CGColorSpace
/**
	Wrapper class for Core Graphics Color Space */

class	CGColorSpace : public CFObject<CGColorSpaceRef> {
public:			
									// Constructors
						CGColorSpace();
						
						CGColorSpace(
								CGColorSpaceRef	inColorSpaceRef,
								bool			inRetain = true);
						
	static CGColorSpace	GetSystemColorSpace();
	
	static CGColorSpace	GetDeviceGray();

	static CGColorSpace	GetDeviceRGB();

	static CGColorSpace	GetDeviceCMYK();

						CGColorSpace( const CGColorSpace& inOriginal );
						
	CGColorSpace&		operator = ( const CGColorSpace& inSource );

									// Accessors
	CFIndex				GetNumberOfComponents() const;

private:	
	typedef CFObject<CGColorSpaceRef>	BaseT;
};

} // namespace PPx

#endif // H_SysCGColorSpace
